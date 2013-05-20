/*****************************************************************************
 * (C) Copyright 2003-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_reboot.c
 *
 * For:  Board WEB2
 *
 * Purpose: Reboot WEB, if it is permitted by the target.
 ****************************************************************************/
#ifndef WPI_BOARD_DATA_INT_H
#include "veneer/wpi_board_data_int.h"
#endif

#ifndef WPX_HEAP_CONFIG_H
#include "wpx_heap_config.h"
#endif

#ifndef WPI_VENEER_H
#include "veneer/wpi_veneer.h"
#endif

#ifndef WP_BUS_H
#include "api/wp_bus.h"
#endif

#ifndef WPX_BOARD_DATA_H
#include "wpx_board_data.h"
#endif

#include "include/core/hardware/wpi_reg_uart.h"
#include "../board/wpx_board_if.h"

#ifndef WPI_REG_GPIO_H
#include "hardware/winpath2/wpi_hw_reg_gpio.h"
#endif

#ifndef WPI_PHY_DATA_H
#include "wpx_phy_data.h"
#endif

#define WPI_RESET_NORMAL 0x10
#define WPI_RS_FORCE_REG 0xbd03010c

void WPX_Reboot(void)
{
   WP_U32 *reset_reg = (WP_U32*)WPI_RS_FORCE_REG;

   WPI_ENFORCE_ORDER;
   WPI_REG_SET( (*reset_reg), WPI_RESET_NORMAL);
   WPI_ENFORCE_ORDER;
}

/*****************************************************************************
 *
 * Function: WP_IndicateProgress
 *
 * Purpose: Indicate progress using WEB hex display.
 *
 ****************************************************************************/
/*static WP_U8 hexcounter = 0;*/
WP_status WP_IndicateProgress(void)
{
   /*  wpx_cpld *cpld = (wpx_cpld *) WPL_PHY_VIRTUAL(0, CPLD_FIBER_EN);
   WP_U32 inverted;

   inverted = ~++hexcounter;
   WPI_MEM_SET(cpld->led_1, inverted << 24);
   */
   return WP_OK;
}

WP_U32 WPX_CpldSet(WP_U32 port, WP_U32 mode)
{
   wpx_cpld_sec *cpld2;
   wpx_cpld *cpld;
   WP_U32 temp_reg;

   cpld2 = (wpx_cpld_sec *) WPL_PHY_VIRTUAL(0, CPLD_JTAG);
   cpld = (wpx_cpld *)WPL_PHY_VIRTUAL(0, CPLD_FIBER_EN);

   switch (port)
   {
      case WP_PORT_UPI1:
      {
         WPI_MEM_GET(temp_reg, cpld2->ser_ctl);
         temp_reg &= 0xFEFFFFFF;
         WPI_MEM_SET(cpld2->ser_ctl, temp_reg);
         WPL_Delay(100);
         break;
      }
      case WP_PORT_UPI2:
      {
         WPI_MEM_GET(temp_reg, cpld2->ser_ctl);
         temp_reg &= 0xFBFFFFFF;
         WPI_MEM_SET(cpld2->ser_ctl, temp_reg);
         WPL_Delay(100);
         break;
      }
      case WP_PORT_TDM1:
      case WP_PORT_TDM2:
      case WP_PORT_TDM3:
      case WP_PORT_TDM4:
      case WP_PORT_TDM5:
      case WP_PORT_TDM6:
      case WP_PORT_TDM7:
      case WP_PORT_TDM8:
      {
         WPI_MEM_GET(temp_reg, cpld->card_ctl);
         temp_reg &= 0x87FFFFFF;
         WPI_MEM_SET(cpld->card_ctl, temp_reg);

         WPL_Delay(100);
         break;
      }
      case WP_UFE_UPI2_TDM1:
      case WP_UFE_UPI2_TDM2:
      case WP_UFE_UPI2_TDM3:
      case WP_UFE_UPI2_TDM4:
      case WP_UFE_UPI2_TDM5:
      case WP_UFE_UPI2_TDM6:
      case WP_UFE_UPI2_TDM7:
      case WP_UFE_UPI2_TDM8:
      {
         WPI_MEM_SET(cpld->card_ctl, 0x95000000);
         WPL_Delay(100);
         break;
      }
      default:
         return WPI_ERROR(WP_ERR_PORT);
   }
   return WP_OK;
}

WP_status WPU_PinMuxReset(void)
{
   WP_U32 wpid = 0;
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
   WPI_REG_SET(gpreg->gpcr_h, 0xffcfffd8);
   WPI_REG_SET(gpreg->gpcm1r_h, 0x00000007);
   WPI_REG_SET(gpreg->gpcm2r_h, 0x00000000);
   return WP_OK;
}

/*****************************************************
 * Configuration for UPI1, UPI2, 8 TDI, 4 RGMII
 ****************************************************/
wpi_gpio default_checkin_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* B */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* F */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* G */ 0xffcfffc0, 0x00000000, 0x00000000, 0x0000001f, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};
WP_U32 default_checkin_card_set = 0x87ffffff;

/*****************************************************
 * Configuration for UPI1, 16 TDI, 4 RGMII
 ****************************************************/
wpi_gpio checkin_tdi_16_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* B */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0x00000000, {0, 0, 0},
   /* F */ 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0x00000000, {0, 0, 0},
   /* G */ 0xffcfffc0, 0x00000000, 0x00000000, 0x00000017, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};
WP_U32 checkin_tdi_16_card_set = 0x9fffffff;

/*****************************************************
 * Configuration for UFE (currently over UPI2 only)
 ****************************************************/
wpi_gpio ufe_upi2_tdm_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* B */ 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* F */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* G */ 0xffcfffc0, 0x00000000, 0x00000000, 0x0000001f, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};
WP_U32 ufe_upi2_tdm_cpld_set = 0x95000000;

/*****************************************************
 * Configuration for SMII 4-11 on UPI1 slot - WinNet1
 ****************************************************/
wpi_gpio smii_4_11_wnet1_on_upi1_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* B */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ ~0x5FF83FF0, 0x00000000, 0x00000000, 0x5FF83FF0, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* F */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* G */ 0xffcfffc0, 0x00000000, 0x00000000, 0x0000001f, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};

/*****************************************************
 * Configuration for SMII 4-11 on TDI slot - WinNet1
 ****************************************************/
wpi_gpio smii_4_11_on_wnet1_tdi_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ ~0x5FF83FF0, 0x00000000, 0x00000000, 0x5FF83FF0, 0x00000000, {0, 0, 0},
   /* B */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* F */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* G */ 0xffcfffc0, 0x00000000, 0x00000000, 0x0000001f, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};
WP_U32 smii_4_11_on_tdi2_cpld_set = 0x87ffffff;

/*****************************************************
 * Configuration for SMII 4-11 on UPI1 slot - WinNet2
 ****************************************************/
wpi_gpio smii_4_11_wnet2_on_upi1_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* B */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ ~0x5FF83FF0, 0x00000000, 0x00000000, 0x5FF83FF0, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* F */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* G */ 0xffcfffc0, 0x00000000, 0x00000000, 0x0000001f, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};
/*****************************************************
 * Configuration for SMII 4-11 on TDI slot  WinNet2
 ****************************************************/
wpi_gpio smii_4_11_wnet2_on_tdi_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* B */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ ~0x5FF83FF0, 0x00000000, 0x00000000, 0x5FF83FF0, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* F */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* G */ 0xffcfffc0, 0x00000000, 0x00000000, 0x0000001f, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};

/*****************************************************
 * Configuration for UPI1, 16 TDI
 ****************************************************/
wpi_gpio tdi_16_upi1_gpio = {
   /*        GPCR        GPDR        GPVR        GPCM1R      GPCM2R      PAD  */
   /* A */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* B */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* C */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* D */ 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, {0, 0, 0},
   /* E */ 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0x00000000, {0, 0, 0},
   /* F */ 0x00000003, 0x00000000, 0x00000000, 0xfffffffc, 0x00000000, {0, 0, 0},
   /* G */ 0x07c40fc0, 0x00000000, 0x00000000, 0x0000001f, 0x00000000, {0, 0, 0},
   /* H */ 0xffcfffd8, 0x00000000, 0x00000000, 0x00000007, 0x00000000, {0, 0, 0}
};
WP_U32 tdi_16_upi1_card_set = 0x9fffffff;

/*****************************************************
 * End of configuration data
 ****************************************************/

/***********************************
 * For WDS2, there is only one WP
 **********************************/
void WPX_CopyGpio(wpi_gpio *from) {
   wpi_gpio *to = WPI_GPIO(WPX_AppWpIntRegBaseGet(0));
   WPI_REG_SET(to->gpcr_a,   from->gpcr_a);
   WPI_REG_SET(to->gpdr_a,   from->gpdr_a);
   WPI_REG_SET(to->gpvr_a,   from->gpvr_a);
   WPI_REG_SET(to->gpcm1r_a, from->gpcm1r_a);
   WPI_REG_SET(to->gpcm2r_a, from->gpcm2r_a);
   WPI_REG_SET(to->gpcr_b,   from->gpcr_b);
   WPI_REG_SET(to->gpdr_b,   from->gpdr_b);
   WPI_REG_SET(to->gpvr_b,   from->gpvr_b);
   WPI_REG_SET(to->gpcm1r_b, from->gpcm1r_b);
   WPI_REG_SET(to->gpcm2r_b, from->gpcm2r_b);
   WPI_REG_SET(to->gpcr_c,   from->gpcr_c);
   WPI_REG_SET(to->gpdr_c,   from->gpdr_c);
   WPI_REG_SET(to->gpvr_c,   from->gpvr_c);
   WPI_REG_SET(to->gpcm1r_c, from->gpcm1r_c);
   WPI_REG_SET(to->gpcm2r_c, from->gpcm2r_c);
   WPI_REG_SET(to->gpcr_d,   from->gpcr_d);
   WPI_REG_SET(to->gpdr_d,   from->gpdr_d);
   WPI_REG_SET(to->gpvr_d,   from->gpvr_d);
   WPI_REG_SET(to->gpcm1r_d, from->gpcm1r_d);
   WPI_REG_SET(to->gpcm2r_d, from->gpcm2r_d);
   WPI_REG_SET(to->gpcr_e,   from->gpcr_e);
   WPI_REG_SET(to->gpdr_e,   from->gpdr_e);
   WPI_REG_SET(to->gpvr_e,   from->gpvr_e);
   WPI_REG_SET(to->gpcm1r_e, from->gpcm1r_e);
   WPI_REG_SET(to->gpcm2r_e, from->gpcm2r_e);
   WPI_REG_SET(to->gpcr_f,   from->gpcr_f);
   WPI_REG_SET(to->gpdr_f,   from->gpdr_f);
   WPI_REG_SET(to->gpvr_f,   from->gpvr_f);
   WPI_REG_SET(to->gpcm1r_f, from->gpcm1r_f);
   WPI_REG_SET(to->gpcm2r_f, from->gpcm2r_f);
   WPI_REG_SET(to->gpcr_g,   from->gpcr_g);
   WPI_REG_SET(to->gpdr_g,   from->gpdr_g);
   WPI_REG_SET(to->gpvr_g,   from->gpvr_g);
   WPI_REG_SET(to->gpcm1r_g, from->gpcm1r_g);
   WPI_REG_SET(to->gpcm2r_g, from->gpcm2r_g);
   WPI_REG_SET(to->gpcr_h,   from->gpcr_h);
   WPI_REG_SET(to->gpdr_h,   from->gpdr_h);
   WPI_REG_SET(to->gpvr_h,   from->gpvr_h);
   WPI_REG_SET(to->gpcm1r_h, from->gpcm1r_h);
   WPI_REG_SET(to->gpcm2r_h, from->gpcm2r_h);
}

void WPX_CopyCardSet(WP_U32 from) {
   wpx_cpld *cpld = (wpx_cpld *) WPL_PHY_VIRTUAL(0, CPLD_FIBER_EN);
   WPI_MEM_SET(cpld->card_ctl, from);
}

WP_status WPX_BoardConfigure(WP_U32 wpid, wpx_board_configuration configuration)
{
   WP_status result = WP_OK;

   switch(configuration) {
      case WPX_CONFIGURE_DEFAULT_CHECKIN:
         WPX_CopyGpio(&default_checkin_gpio);
         WPX_CopyCardSet(default_checkin_card_set);
         break;
      case WPX_CONFIGURE_CHECKIN_TDI_16:
         WPX_CopyGpio(&checkin_tdi_16_gpio);
         WPX_CopyCardSet(checkin_tdi_16_card_set);
         break;
      case WPX_CONFIGURE_UFE_UPI2_TDM:
         WPX_CopyGpio(&ufe_upi2_tdm_gpio);
         WPX_CopyCardSet(ufe_upi2_tdm_cpld_set);
         break;
      case WPX_CONFIGURE_TDI_16_UPI1:
         WPX_CopyGpio(&tdi_16_upi1_gpio);
         WPX_CopyCardSet(tdi_16_upi1_card_set);
         break;
      default:
         result = WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);
   }
   return result;
}

WP_status WPU_PinMuxAllocateToPort(WP_U32 port, WP_U32 mode)
{
   /* do nothing, everything is done by EnetPhyInit */
   return WP_OK;
}

WP_status WPX_BoardExtCardInit(WP_U32 mode, WP_U32 connector, wpi_match_mask *save)
{
   switch(mode)
   {
      case WPX_MODE_OC12_PMC5357:
      {
         switch(connector)
         {
            case WPX_SERIAL_CONNECTOR_0:
               /* Save current match/mask, install OC-12 values. */
               WPX_SetBank(save, &wpi_board_data.match_mask->oc12[0]);
               break;
            case WPX_SERIAL_CONNECTOR_1:
               /* Save current match/mask, install OC-12 values. */
               WPX_SetBank(save, &wpi_board_data.match_mask->oc12[1]);
               break;
            default:
               break;
         }
         break;
      }
      case WPX_MODE_OC3_PMC5351:
      {
         switch(connector)
         {
            case WPX_SERIAL_CONNECTOR_0:
               /* Save current match/mask, install OC-3 values. */
               WPX_SetBank(save, &wpi_board_data.match_mask->oc3[0]);
               break;
            case WPX_SERIAL_CONNECTOR_1:
               /* Save current match/mask, install OC-3 values. */
               WPX_SetBank(save, &wpi_board_data.match_mask->oc3[1]);
               break;
            default:
               break;
         }
         break;
      }

      default:
         break;
   }
   
   return WP_OK;
}

void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid)
{
   return;
   
}
