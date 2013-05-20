/*****************************************************************************
 * (C) Copyright 2003-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_board_functions.c
 *
 * For:  Board WDS3
 *
 * Purpose: WDS3 configuration related functions
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#ifndef WPX_SERDES_H
#include "wpx_serdes.h"
#endif

#ifndef WPX_PINMUX_H
#include "wpx_pinmux.h"
#endif

#ifndef WPI_PHY_DATA_H
#include "wpx_phy_data.h"
#endif

#include "wpl_mem_access.h"

#include "wpl_platform.h"

#if 0 // XALYO
static void WPX_BoardWriteZarlinkPllReg(WP_U32 wpid, WP_U32 address, WP_U32 data);
static void WPX_BoardReadZarlinkPllReg(WP_U32 wpid, WP_U32 address, WP_U32 *data);
WP_status WPX_ExternalPllClockOutConfig(WPX_pll_cfg *pllConfigParam);
static WP_status  WPX_Ael2005PollWriteOkBit(WP_U32 wpid,
                                            WP_U32 polled_value,
                                            WP_U32 polled_mask);
static WP_status  WPX_PhyAel2005FpgaWrite(WP_U32 xgi_port_offset,
                                          WP_U32 phy_addr,
                                          WP_U32 reg_addr,
                                          WP_U32 data);
static WP_status  WPX_PhyAel2005Write(WP_U32 wpid,
                                      WP_U32 dev_type,
                                      WP_U32 xgi_port_offset,
                                      WP_U32 phy_addr,
                                      WP_U32 reg_addr,
                                      WP_U32 data);
#endif

wpx_board_configuration current_configuration = WPX_CONFIGURE_DEFAULT_CHECKIN;

#define WPX_RESET 0x1
void WPX_Reboot(void)
{
#if 0 // XALYO
   wpx_fpga *fpga;

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(0, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_SET(fpga->reset_all, WPX_RESET);
#else
   printf("XS-AMC-WP3C: WPX_Reboot not implemented yet\n");
#endif
}

/*****************************************************************************
 *
 * Function: WP_IndicateProgress
 *
 * Purpose: Shouldn't be used in WDS3. Backward compatability only
 *
 ****************************************************************************/
WP_status WP_IndicateProgress(void)
{
   return WP_OK;
}

// XALYO
typedef struct
{
      WP_U32 brg;
} wpx_brg;

WP_status WPX_BoardConfigure(WP_U32 wpid, wpx_board_configuration configuration)
{
   WP_status status = WP_OK;

   switch(configuration) {
#if 0 // XALYO
      case WPX_CONFIGURE_DEFAULT_CHECKIN:
      case WPX_CONFIGURE_CHECKIN_TDI_16:
      case WPX_CONFIGURE_2UPI_2XGI_6SGMII2G:
      case WPX_CONFIGURE_1UPI_1XGI_4GMII_8SGMII:
      case WPX_CONFIGURE_1XGI_6RGMII_6SGMII:
      case WPX_CONFIGURE_1UPI_1XGI_6RTBI_6SGMII:
      case WPX_CONFIGURE_1XGI_6SMII_6SGMII:
      case WPX_CONFIGURE_2RMII_8TDI_1XGI_10SGMII:
      case WPX_CONFIGURE_8TDI_1UPI_1XGI_10SGMII:
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII:
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4:
      case WPX_CONFIGURE_2UPI_16bit_1XGI_10SGMII:
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII_UPI_CLK_100M:
      case WPX_CONFIGURE_2SGMII_2SGMII2G_16TDI:
      case WPX_CONFIGURE_6SGMII_1SGMII2G_2SMII:
      case WPX_CONFIGURE_2UPI_L3_2XGI_6SGMII:
      case WPX_CONFIGURE_UPI1_TDI8_16_10SGMII:
      case WPX_CONFIGURE_2XGI_6SGMII:
      case WPX_CONFIGURE_2UPI_2XGI_6SGMII2G_UFE448:
      case WPX_CONFIGURE_UPI3_UFE412:
#endif
           // XALYO
       case WPX_CONFIGURE_XS_1UPI_10SGMII:
       case WPX_CONFIGURE_XS_1UPI_1XGI_S2_6SGMII:
       case WPX_CONFIGURE_XS_1UPI_1XGI_S3_6SGMII:
       case WPX_CONFIGURE_XS_1UPI_2XGI_2SGMII:
       case WPX_CONFIGURE_XS_1POS_10SGMII:
       case WPX_CONFIGURE_XS_1POS_1XGI_S2_6SGMII:
       case WPX_CONFIGURE_XS_1POS_1XGI_S3_6SGMII:
       case WPX_CONFIGURE_XS_1POS_2XGI_2SGMII:
       case WPX_CONFIGURE_XS_3200:
       case WPX_CONFIGURE_XS_1UPI_2SGMII:
       case WPX_CONFIGURE_XS_1POS_2SGMII:

         break;
      default:
         return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);
   }

   // XALYO

    // Create BRG1 @ 125 MHz for the Quad Serdes
   {
       wpx_brg *brg_ptr;
       WP_U32 test_val;

       //brg_ptr = (wpx_brg *)(WPX_AppWpIntRegBaseGet(wpid) + 0xc200);
       brg_ptr = (wpx_brg *)(WPX_AppWpIntRegBaseGet(wpid) + 0xc208);

       //printf("brg @ %p\n", &brg_ptr->brg);fflush(stdout);

       WPI_MEM_SET(brg_ptr->brg, 0xC0000001);
       WP_MEM_GET(test_val, brg_ptr->brg);
   }

   /* Configure clock for XGI, Serdeses, BRG from Zarlink PLL */
// XALYO
   if(0) {
   WPI_RETURN_IF_ERROR(status, WPX_BoardZarlinkPllConfig(wpid, WPX_PLL_CONFIG_DEFAULT));
   }

   /* Set default value for Pinmux Pat Control (CMOS)
    * that have the slowest Slew mode */
   WPI_RETURN_IF_ERROR(status, WPX_PinMuxPadControlSet(wpid));

   WPI_RETURN_IF_ERROR(status,
                       WPX_PinMuxSet(wpid, configuration));

   WPI_RETURN_IF_ERROR(status,
                       WPX_SerDesSet(wpid, configuration));

// XALYO
   if(0) {
   WPI_RETURN_IF_ERROR(status,
                       WPX_FpgaUpdateMux(wpid, configuration));
   }

   current_configuration = configuration;

   return status;
}

WP_status WPU_PinMuxAllocateToPort(WP_U32 port, WP_U32 mode)
{
   /* do nothing, everything is done by EnetPhyInit */
   return WP_OK;
}

#if 0 // XALYO
void WPX_FpgaMatchRegSet(WP_U32 wpid)
{
   WP_U32 *fpga_match_reg = (WP_U32 *)WPL_RIF_VIRTUAL(wpid, (APP_MASK_MATCH_OFFSET+APP_MASK_MATCH_FPGA_OFFSET));

   WP_MEM_SET(*fpga_match_reg, (WPI_WDS3_FPGA_BASE & 0xffff0000) | 0x1007);
}

WP_status WPX_FpgaUpdateMux(WP_U32 wpid, wpx_board_configuration configuration)
{
   wpx_fpga *fpga;
   WP_U8 value=0;
   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   switch(configuration) {
      case WPX_CONFIGURE_1UPI_1XGI_4GMII_8SGMII:
      case WPX_CONFIGURE_1UPI_1XGI_6RTBI_6SGMII:
      case WPX_CONFIGURE_8TDI_1UPI_1XGI_10SGMII:
         /* serial connector 0 - tdi or enet phy
            serial connector 1 - upi */
         WP_MEM_BYTE_SET(fpga->serial_control, 0);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI2);
        break;
      case WPX_CONFIGURE_UPI1_TDI8_16_10SGMII:
         /* serial connector 0 - upi1
          * serial connector 1 - tdi 9 - 16 */
         WP_MEM_BYTE_SET(fpga->serial_control, 0);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1);

         break;
      case WPX_CONFIGURE_CHECKIN_TDI_16:
      case WPX_CONFIGURE_2RMII_8TDI_1XGI_10SGMII:
      case WPX_CONFIGURE_2SGMII_2SGMII2G_16TDI:
      case WPX_CONFIGURE_1XGI_6RGMII_6SGMII:
      case  WPX_CONFIGURE_2XGI_6SGMII:
         /* serial connector 0 - tdi or enet phy*/
         /* serial connector 1 - tdi*/
         WP_MEM_BYTE_SET(fpga->serial_control, 0);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_NON_UPI);
         break;
      case WPX_CONFIGURE_2UPI_2XGI_6SGMII2G:
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII:
      case WPX_CONFIGURE_DEFAULT_CHECKIN:
         /* serial connector 0 - upi
            serial connector 1 - upi
            serial connector 2 - upi */
         WP_MEM_BYTE_SET(fpga->serial_control, 0);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3);
         break;
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4:
      {
         /* serial connector 0 - upi
            serial connector 1 - upi
            serial connector 2 - upi */
         
         /* In UFE4 SPARTAN card the 2 LSB bits in serial_control are
            erasing UFE synthesis or initializing the firmware and
            therefore we leave them intact                                 */
         WP_MEM_BYTE_GET(value, fpga->serial_control);
         value &= 0x3;
         WP_MEM_BYTE_SET(fpga->serial_control, value);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3);
         WP_MEM_BYTE_SET(fpga->mux_select_2, WPX_FPGA_MUX_SELECT2_UPI1_UPI2_62M);
         break;
      }
      case WPX_CONFIGURE_UPI3_UFE412:
         /* serial connector 2 - upi*/
         WP_MEM_BYTE_GET(value, fpga->serial_control);
         value &= 0x3f;
         WP_MEM_BYTE_SET(fpga->serial_control, value);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3);
         WP_MEM_BYTE_SET(fpga->mux_select_2, WPX_FPGA_MUX_SELECT2_UPI3_62M);
         break;
      case WPX_CONFIGURE_2UPI_16bit_1XGI_10SGMII:
         /* serial connector 0 - upi 16 bit
            serial connector 1 - upi 16 bit
            serial connector 2 - HZ */
         WP_MEM_BYTE_SET(fpga->serial_control, 0);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3);
         break;
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII_UPI_CLK_100M:
         /* Same as 2UPI_1XGI_10SGMII but includes configuration for EMPHY
            clock to be 100MHz */
         WP_MEM_BYTE_SET(fpga->serial_control, 0);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3);
         WP_MEM_BYTE_SET(fpga->mux_select_2, WPX_FPGA_MUX_SELECT2_UPI1_UPI2_100M);
         break;
      case WPX_CONFIGURE_2UPI_L3_2XGI_6SGMII:
         WP_MEM_BYTE_SET(fpga->serial_control, 0);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3);
         WP_MEM_BYTE_SET(fpga->mux_select_2,WPX_FPGA_MUX_SELECT2_UPI1_UPI2_104MHZ);
         break;
      case WPX_CONFIGURE_1XGI_6SMII_6SGMII:
      case WPX_CONFIGURE_6SGMII_1SGMII2G_2SMII:
         break;
      case WPX_CONFIGURE_2UPI_2XGI_6SGMII2G_UFE448:
      {
         /*Bits 0,1 of the serial_control byte are for upi1
          * and bits 2,3 are for upi2*/
         WP_MEM_BYTE_GET(value, fpga->serial_control);
         value &= 0xf;
         WP_MEM_BYTE_SET(fpga->serial_control, value);
         WP_MEM_BYTE_SET(fpga->mux_select_1, WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3);
         WP_MEM_BYTE_SET(fpga->mux_select_2,WPX_FPGA_MUX_SELECT2_UPI1_UPI2_104MHZ);
         break;
      }
      default:
         return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);
   }
   return WP_OK;
}

void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid,WP_U32 connector_id)
{
   wpx_fpga *fpga;
   WP_U8 serial_control_value,mask;
   WP_U32 reg_value, gpio_base, reg_offset;

   gpio_base = WPX_AppWpIntRegBaseGet(wpid);

   /*Write the input/output values for SP111,112,113*/
   reg_offset = gpio_base + WPX_SERIAL_UFE4_GPIO_IO_OFFSET;

   WP_MEM_GET(reg_value, *((WP_U32 *) reg_offset));
   reg_value &= ~WPX_SERIAL_UFE4_GPIO_7_MASK;
   reg_value |= WPX_SERIAL_UFE4_GPIO_8_9_MASK;
   WP_MEM_SET(*((WP_U32 *)reg_offset), reg_value);

   /*Write the values for SP112,113*/
   reg_offset = gpio_base + WPX_SERIAL_UFE4_GPIO_VAL_OFFSET;

   WP_MEM_GET(reg_value, *((WP_U32 *)reg_offset));
   reg_value &=~WPX_SERIAL_UFE4_GPIO_8_9_MASK;
   WP_MEM_SET(*((WP_U32 *)reg_offset), reg_value);

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   if (connector_id == 0)
       mask = WPX_SERIAL_CONTROL_UFE4_PROGRAM_UPI1_MASK;
   else if (connector_id == 1)
       mask = WPX_SERIAL_CONTROL_UFE4_PROGRAM_UPI2_MASK;
   else
       mask = WPX_SERIAL_CONTROL_UFE4_PROGRAM_UPI3_MASK;

   /* clear the program bit */
   WP_MEM_BYTE_GET(serial_control_value, fpga->serial_control);
   serial_control_value &= ~mask;
   WP_MEM_BYTE_SET(fpga->serial_control, serial_control_value);

   /* set the program bit */
   WP_MEM_BYTE_GET(serial_control_value, fpga->serial_control);
   serial_control_value |= mask;
   WP_MEM_BYTE_SET(fpga->serial_control, serial_control_value);


   WPL_Delay(5000);
   
   /*Wait until gpio7 is asserted, gpio7 is connected to the init_b of the ufe448*/
/*
   #define WPX_INIT_B_WAIT 100
   reg_offset = gpio_base + WPX_SERIAL_UFE4_GPIO_IO_OFFSET;
   for (i=0;i<WPX_INIT_B_WAIT;i++)
   {

	   WPL_Delay (50);
	   WP_MEM_GET(reg_value, *((WP_U32 *) reg_offset));
	   reg_value &= WPX_SERIAL_UFE4_GPIO_7_MASK;
	   if (reg_value)
	   {
		   printf("gpio7 is asserted\n");
		   break;
	   }
	   if (i == (WPX_INIT_B_WAIT-1))
		   printf("Init_b signal has not been asserted\n");
   }
 */
}

void WPX_Ufe4HwReset(WP_U32 wpid,WP_U32 connector_id)
{
   wpx_fpga *fpga;
   WP_U8 serial_control_value,mask;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   if (connector_id == 0)
      mask = WPX_SERIAL_CONTROL_UFE4_RESET_UPI1_MASK;
   else if (connector_id == 1)
      mask = WPX_SERIAL_CONTROL_UFE4_RESET_UPI2_MASK;
   else
      mask = WPX_SERIAL_CONTROL_UFE4_RESET_UPI3_MASK;

   /* clear the UFE reset bit */
   WP_MEM_BYTE_GET(serial_control_value, fpga->serial_control);
   serial_control_value &= ~mask;
   WP_MEM_BYTE_SET(fpga->serial_control, serial_control_value);

   /* set the UFE reset bit */
   WP_MEM_BYTE_GET(serial_control_value, fpga->serial_control);
   serial_control_value |= mask;
   WP_MEM_BYTE_SET(fpga->serial_control, serial_control_value);
}

WP_U8 WPX_Ufe412CpldInterruptMaskGet(WP_U32 wpid)
{
   wpx_fpga *fpga;
   WP_U8 mask;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_GET(mask, fpga->int_mask);

   return mask;
}

void WPX_Ufe412CpldInterruptMaskSet(WP_U32 wpid, WP_U8 mask)
{
   wpx_fpga *fpga;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_SET(fpga->int_mask, mask);
}

void WPX_Ufe412CpldInterruptSelect(WP_U32 wpid)
{
   wpx_fpga *fpga;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   /* set the EINT3 interrupt bit */
   WP_MEM_BYTE_SET(fpga->interrupt_select, WPX_INTERRUPT_EINT3);
}

WP_status WPX_FpgaSet(WP_U32 port, WP_U32 mode)
{
   /* TBD */
   return WP_OK;
}

WP_status WPX_BoardExtCardInit(WP_U32 mode, WP_U32 connector, wpi_match_mask *save)
{
   WP_U32 *if_addr;
   WP_status status;

   /* mark the match address for WP3 */
   save->match_address = 0xffffffff;

   switch(mode)
   {
      case WPX_MODE_OC12_PMC5357:
      {
         switch(connector)
         {
            case WPX_SERIAL_CONNECTOR_0:
            {
               /* Get the virtual address of the match/mask register pair. */
               if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, wpi_board_data.match_mask->oc12[0].match_address);
               WPI_REG_SET(if_addr[1], wpi_board_data.match_mask->oc12[0].mask_value);
               WPI_REG_SET(if_addr[0], wpi_board_data.match_mask->oc12[0].match_value);
               break;
            }
            case WPX_SERIAL_CONNECTOR_1:
            {
               /* Get the virtual address of the match/mask register pair. */
               if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, APP_ZBT_HOST_IF_1);
               WPI_REG_SET(if_addr[0], 0x0);
               WPI_REG_SET(if_addr[1], 0x0);

               if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, wpi_board_data.match_mask->oc12[1].match_address);
               WPI_REG_SET(if_addr[1], wpi_board_data.match_mask->oc12[1].mask_value);
               WPI_REG_SET(if_addr[0], wpi_board_data.match_mask->oc12[1].match_value);
               break;
            }
            default:
               break;
         }
         break;
      }
      case WPX_MODE_OC3_PMC5351:
      {
         WPI_RETURN_IF_ERROR(status, WPX_PinMuxPullDownSet(0, connector + WP_PORT_UPI1));

         switch(connector)
         {
            case WPX_SERIAL_CONNECTOR_0:
            {
               /* Get the virtual address of the match/mask register pair. */
               if_addr = (WP_U32 *) WPL_PHY_VIRTUAL(0, wpi_board_data.match_mask->oc3[0].match_address);
               WPI_REG_SET(if_addr[1], wpi_board_data.match_mask->oc3[0].mask_value);
               WPI_REG_SET(if_addr[0], wpi_board_data.match_mask->oc3[0].match_value);
               break;
            }
            case WPX_SERIAL_CONNECTOR_1:
            {
               /* Get the virtual address of the match/mask register pair. */
               if_addr = (WP_U32 *) WPL_PHY_VIRTUAL(0, APP_ZBT_HOST_IF_1);
               WPI_REG_SET(if_addr[0], 0x0);
               WPI_REG_SET(if_addr[1], 0x0);

               if_addr = (WP_U32 *) WPL_PHY_VIRTUAL(0, wpi_board_data.match_mask->oc3[1].match_address);
               WPI_REG_SET(if_addr[1], wpi_board_data.match_mask->oc3[1].mask_value);
               WPI_REG_SET(if_addr[0], wpi_board_data.match_mask->oc3[1].match_value);
               break;
            }
            default:
               break;
         }
         break;
      }
      case WPX_MODE_FPGA_DC:
      {
         switch(connector)
         {
            case WPX_SERIAL_CONNECTOR_0:
            {
               /* Get the virtual address of the match/mask register pair. */
               if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, wpi_board_data.match_mask->fpga_dc[0].match_address);
               WPI_REG_SET(if_addr[1], wpi_board_data.match_mask->fpga_dc[0].mask_value);
               WPI_REG_SET(if_addr[0], wpi_board_data.match_mask->fpga_dc[0].match_value);
               break;
            }
            case WPX_SERIAL_CONNECTOR_1:
            {
               /* Get the virtual address of the match/mask register pair. */
               if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, APP_ZBT_HOST_IF_1);
               WPI_REG_SET(if_addr[0], 0x0);
               WPI_REG_SET(if_addr[1], 0x0);

               if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, wpi_board_data.match_mask->fpga_dc[1].match_address);
               WPI_REG_SET(if_addr[1], wpi_board_data.match_mask->fpga_dc[1].mask_value);
               WPI_REG_SET(if_addr[0], wpi_board_data.match_mask->fpga_dc[1].match_value);
               break;
            }
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

WP_status WPX_CrBoardPinConfig(WP_U32 wpid, wpx_wp_cr_pin_io wp_cr_pin_io)
{

   WP_status status;
   wpx_fpga_config fpga_config_brg, fpga_config_pps_fclk, fpga_config_sma_dir, fpga_config_serdes_ref_clk;
   wpx_fpga_config fpga_config_pll_source;

   fpga_config_brg.value = 0;
   fpga_config_pps_fclk.value = 0;
   fpga_config_sma_dir.value = 0;
   fpga_config_pll_source.value = 0;
   fpga_config_serdes_ref_clk.value = 0;

   /* Constrains */
   if (((wp_cr_pin_io.brg_b == WPX_FPGA_BRG_B_IN_SMA5) &&
        (wp_cr_pin_io.clk_rec_fclk1 == WPX_FPGA_CR_FCLK1_OUT_SMA5)) ||

       ((wp_cr_pin_io.clk_rec_fclk1 == WPX_FPGA_CR_FCLK1_OUT_SMA4) &&
        (wp_cr_pin_io.clk_rec_fclk2 == WPX_FPGA_CR_FCLK2_OUT_SMA4)) ||

       (((wp_cr_pin_io.clk_rec_fclk1 == WPX_FPGA_CR_FCLK1_OUT_SMA4) ||
         (wp_cr_pin_io.clk_rec_fclk2 == WPX_FPGA_CR_FCLK2_OUT_SMA4)) &&
        (wp_cr_pin_io.brg_d == WPX_FPGA_BRG_D_IN_SMA4)) ||

       ((wp_cr_pin_io.clk_rec_pps1_output == WPX_FPGA_CR_PPS1_OUT_SMA3) &&
        (wp_cr_pin_io.clk_rec_pps1_input == WPX_FPGA_CR_PPS1_IN_SMA3)) ||

       /* SMA2 mux must contains only one valid entry (or none) */
       ((wp_cr_pin_io.clk_rec_pps1_output == WPX_FPGA_CR_PPS1_OUT_SMA2) &&
        ((wp_cr_pin_io.clk_rec_pps2_output == WPX_FPGA_CR_PPS2_OUT_SMA2) ||
         (wp_cr_pin_io.sync_eth_clk0_output == WPX_FPGA_SYNC_ETH0_OUT_SMA2) ||
         (wp_cr_pin_io.sync_eth_clk1_output == WPX_FPGA_SYNC_ETH1_OUT_SMA2))) ||
       ((wp_cr_pin_io.clk_rec_pps2_output == WPX_FPGA_CR_PPS2_OUT_SMA2) &&
        ((wp_cr_pin_io.sync_eth_clk0_output == WPX_FPGA_SYNC_ETH0_OUT_SMA2) ||
         (wp_cr_pin_io.sync_eth_clk1_output == WPX_FPGA_SYNC_ETH1_OUT_SMA2))) ||
       ((wp_cr_pin_io.sync_eth_clk0_output == WPX_FPGA_SYNC_ETH0_OUT_SMA2) &&
        (wp_cr_pin_io.sync_eth_clk1_output == WPX_FPGA_SYNC_ETH1_OUT_SMA2)) ||

       /* Verify SMA2 direction */
       (((wp_cr_pin_io.clk_rec_pps1_output == WPX_FPGA_CR_PPS1_OUT_SMA2) ||
         (wp_cr_pin_io.clk_rec_pps2_output == WPX_FPGA_CR_PPS2_OUT_SMA2) ||
         (wp_cr_pin_io.sync_eth_clk0_output == WPX_FPGA_SYNC_ETH0_OUT_SMA2) ||
         (wp_cr_pin_io.sync_eth_clk1_output == WPX_FPGA_SYNC_ETH1_OUT_SMA2)) &&
        (wp_cr_pin_io.clk_rec_pps2_input == WPX_FPGA_CR_PPS2_IN_SMA2)))
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);


   /* configure HW register: BRG configuration */

   if (wp_cr_pin_io.brg_c == WPX_FPGA_BRG_C_IN_SYNT_CLK)
      fpga_config_brg.value |= WPX_FPGA_BRG_C_INPUT_SYNT_CLK;
   else if (wp_cr_pin_io.brg_c == WPX_FPGA_BRG_C_IN_SMA7)
      fpga_config_brg.value |= WPX_FPGA_BRG_C_INPUT_SMA7;
   else if (wp_cr_pin_io.brg_c == WPX_FPGA_BRG_C_IN_APLL0_CLK)
      fpga_config_brg.value |= WPX_FPGA_BRG_C_INPUT_APLL0_CLK;
   else if (wp_cr_pin_io.brg_c == WPX_FPGA_BRG_C_IN_P1_CLK0)
      fpga_config_brg.value |= WPX_FPGA_BRG_C_INPUT_P1_CLK0;
   else
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.brg_d == WPX_FPGA_BRG_D_IN_SYNT_CLK)
      fpga_config_brg.value |= WPX_FPGA_BRG_D_INPUT_SYNT_CLK;
   else if (wp_cr_pin_io.brg_d == WPX_FPGA_BRG_D_IN_SMA6)
      fpga_config_brg.value |= WPX_FPGA_BRG_D_INPUT_SMA6;
   else if (wp_cr_pin_io.brg_d == WPX_FPGA_BRG_D_IN_SMA4)
   {
      fpga_config_brg.value |= WPX_FPGA_BRG_D_INPUT_SMA4;
      fpga_config_sma_dir.value |= WPX_FPGA_SMA4_DIR_SMA_TO_WP;
   }
   else if (wp_cr_pin_io.brg_d == WPX_FPGA_BRG_D_IN_P0_CLK0)
      fpga_config_brg.value |= WPX_FPGA_BRG_D_INPUT_P0_CLK0;
   else
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.brg_b == WPX_FPGA_BRG_B_IN_SMA5)
      fpga_config_sma_dir.value |= WPX_FPGA_SMA5_DIR_SMA_TO_WP;
   else if (wp_cr_pin_io.brg_b != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.sma1_out == WPX_FPGA_SMA1_OUT_TCXO)
      fpga_config_pps_fclk.value |= WPX_FPGA_SMA1_OUTPUT_TCXO;
   else if (wp_cr_pin_io.sma1_out == WPX_FPGA_SMA1_OUT_APLL0_CLK)
      fpga_config_pps_fclk.value |= WPX_FPGA_SMA1_OUTPUT_APLL0_CLK;
   else if (wp_cr_pin_io.sma1_out == WPX_FPGA_SMA1_OUT_P0_CLK0)
      fpga_config_pps_fclk.value |= WPX_FPGA_SMA1_OUTPUT_P0_CLK0;
   else if (wp_cr_pin_io.sma1_out == WPX_FPGA_SMA1_OUT_P1_CLK0)
      fpga_config_pps_fclk.value |= WPX_FPGA_SMA1_OUTPUT_P1_CLK0;
   else if (wp_cr_pin_io.sma1_out != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);
   /* In any case, connect the mux output to SMA1 */
   fpga_config_sma_dir.value |= WPX_FPGA_SMA1_DIR_MUX_TO_SMA;


   /* configure HW register: PPS and FCLK configuration */

   if (wp_cr_pin_io.clk_rec_fclk1 == WPX_FPGA_CR_FCLK1_OUT_SMA5)
      fpga_config_sma_dir.value |= WPX_FPGA_SMA5_DIR_WP_TO_SMA;
   else if (wp_cr_pin_io.clk_rec_fclk1 == WPX_FPGA_CR_FCLK1_OUT_SMA4)
   {
      fpga_config_pps_fclk.value |= WPX_FPGA_CR_OUTPUT_FCLK_1;
      fpga_config_sma_dir.value |= WPX_FPGA_SMA4_DIR_WP_TO_SMA;
   }
   else if (wp_cr_pin_io.clk_rec_fclk1 != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.clk_rec_fclk2 == WPX_FPGA_CR_FCLK2_OUT_SMA4)
   {
      fpga_config_pps_fclk.value |= WPX_FPGA_CR_OUTPUT_FCLK_2;
      fpga_config_sma_dir.value |= WPX_FPGA_SMA4_DIR_WP_TO_SMA;
   }
   else if (wp_cr_pin_io.clk_rec_fclk2 != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);


   if (wp_cr_pin_io.clk_rec_pps1_output == WPX_FPGA_CR_PPS1_OUT_SMA3)
      fpga_config_sma_dir.value |= WPX_FPGA_SMA3_DIR_WP_TO_SMA;
   else if (wp_cr_pin_io.clk_rec_pps1_output == WPX_FPGA_CR_PPS1_OUT_SMA2)
   {
      fpga_config_pps_fclk.value |= WPX_FPGA_CR_OUTPUT_PPS_1;
      fpga_config_sma_dir.value |= WPX_FPGA_SMA2_DIR_WP_TO_SMA;
   }
   else if (wp_cr_pin_io.clk_rec_pps1_output != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.clk_rec_pps1_input == WPX_FPGA_CR_PPS1_IN_SMA3)
      fpga_config_sma_dir.value |= WPX_FPGA_SMA3_DIR_SMA_TO_WP;
   else if (wp_cr_pin_io.clk_rec_pps1_input != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.clk_rec_pps2_output == WPX_FPGA_CR_PPS2_OUT_SMA2)
   {
      fpga_config_pps_fclk.value |= WPX_FPGA_CR_OUTPUT_PPS_2;
      fpga_config_sma_dir.value |= WPX_FPGA_SMA2_DIR_WP_TO_SMA;
   }
   else if (wp_cr_pin_io.clk_rec_pps2_output != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.clk_rec_pps2_input == WPX_FPGA_CR_PPS2_IN_SMA2)
      fpga_config_sma_dir.value |= WPX_FPGA_SMA2_DIR_SMA_TO_WP;
   else if (wp_cr_pin_io.clk_rec_pps2_input != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.sync_eth_clk0_output == WPX_FPGA_SYNC_ETH0_OUT_SMA2)
   {
      fpga_config_pps_fclk.value |= WPX_FPGA_SYNC_ETH_CLK0;
      fpga_config_sma_dir.value |= WPX_FPGA_SMA2_DIR_WP_TO_SMA;
   }
   else if (wp_cr_pin_io.sync_eth_clk0_output != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.sync_eth_clk1_output == WPX_FPGA_SYNC_ETH1_OUT_SMA2)
   {
      fpga_config_pps_fclk.value |= WPX_FPGA_SYNC_ETH_CLK1;
      fpga_config_sma_dir.value |= WPX_FPGA_SMA2_DIR_WP_TO_SMA;
   }
   else if (wp_cr_pin_io.sync_eth_clk1_output != WPX_FPGA_NA)
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);


 /* Configure HW register: External PLL clock sources */
   if (wp_cr_pin_io.osci_sel == WPX_FPGA_OSCI_SOURCE_SYNT_CLK)
      fpga_config_pll_source.value |= WPX_FPGA_OSCI_SRC_FROM_SYNT_CLK;
   else if (wp_cr_pin_io.osci_sel == WPX_FPGA_OSCI_SOURCE_SMA7)
      fpga_config_pll_source.value |= WPX_FPGA_OSCI_SRC_FROM_SMA7;
   else
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.pll_ref_clk2_sel == WPX_FPGA_PLL_REFCLK2_SYNC_ETH_CLK1)
      fpga_config_pll_source.value |= WPX_FPGA_PLL_REFCLK2_FROM_SYNC_ETH_CLK1;
   else if (wp_cr_pin_io.pll_ref_clk2_sel == WPX_FPGA_PLL_REFCLK2_CLK_REC_FCLK_2)
      fpga_config_pll_source.value |= WPX_FPGA_PLL_REFCLK2_FROM_CLK_REC_FCLK_2;
   else if (wp_cr_pin_io.pll_ref_clk2_sel == WPX_FPGA_PLL_REFCLK2_SMA7)
      fpga_config_pll_source.value |= WPX_FPGA_PLL_REFCLK2_FROM_SMA7;
   else
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);

   if (wp_cr_pin_io.pll_ref_clk1_sel == WPX_FPGA_PLL_REFCLK1_SYNC_ETH_CLK0)
      fpga_config_pll_source.value |= WPX_FPGA_PLL_REFCLK1_FROM_SYNC_ETH_CLK0;
   else if (wp_cr_pin_io.pll_ref_clk1_sel == WPX_FPGA_PLL_REFCLK1_CLK_REC_FCLK_1)
      fpga_config_pll_source.value |= WPX_FPGA_PLL_REFCLK1_FROM_CLK_REC_FCLK_1;
   else if (wp_cr_pin_io.pll_ref_clk1_sel == WPX_FPGA_PLL_REFCLK1_SMA6)
      fpga_config_pll_source.value |= WPX_FPGA_PLL_REFCLK1_FROM_SMA6;
   else
      return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);



   /* Configure the SerdesX reference clocks */
   if (wp_cr_pin_io.serdes_x_select == WPX_FPGA_SERDES_CLK_APLL0)
   {
      fpga_config_serdes_ref_clk.value |= WPX_FPGA_SERDES_X_APLL0_CLK;
   }
   else
   {
      fpga_config_serdes_ref_clk.value |= WPX_FPGA_SERDES_X_SELECT_CLK;

      if (wp_cr_pin_io.serdes1_ref_clk == WPX_FPGA_SERDES_CLK_100)
         fpga_config_serdes_ref_clk.value |= WPX_FPGA_SERDES_1_CLK_100;

      else
         fpga_config_serdes_ref_clk.value |= WPX_FPGA_SERDES_1_CLK_125;

      if (wp_cr_pin_io.serdes0_ref_clk == WPX_FPGA_SERDES_CLK_100)
         fpga_config_serdes_ref_clk.value |= WPX_FPGA_SERDES_0_CLK_100;
      else
         fpga_config_serdes_ref_clk.value |= WPX_FPGA_SERDES_0_CLK_125;
   }

   /* Update FPGA */
   WPI_RETURN_IF_ERROR(status,
                       WPX_CrBoardConfiguration(wpid, WPX_CONFIGURE_BRG_MUX,
                                                &fpga_config_brg));
   WPI_RETURN_IF_ERROR(status,
                       WPX_CrBoardConfiguration(wpid, WPX_CONFIGURE_PPS_FCLK_SELECT,
                                                &fpga_config_pps_fclk));
   WPI_RETURN_IF_ERROR(status,
                       WPX_CrBoardConfiguration(wpid, WPX_CONFIGURE_SMA_DIRECTION,
                                                 &fpga_config_sma_dir));
   WPI_RETURN_IF_ERROR(status,
                       WPX_CrBoardConfiguration(wpid, WPX_CONFIGURE_PLL_SOURCE,
                                                &fpga_config_pll_source));
   WPI_RETURN_IF_ERROR(status,
                       WPX_CrBoardConfiguration(wpid, WPX_CONFIGURE_SERDES_REF_CLK,
                                                &fpga_config_serdes_ref_clk));

   return WP_OK;
}

WP_status WPX_CrBoardConfiguration(WP_U32 wpid,
                                   wpx_board_configuration configuration,
                                   wpx_fpga_config *config_value)
{
   wpx_fpga *fpga;

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);
   switch(configuration) {
      case WPX_CONFIGURE_BRG_MUX:
         WPX_ConfigBrgMux(fpga, config_value);
         break;
      case WPX_CONFIGURE_PPS_FCLK_SELECT:
         WPX_ConfigPpsAndFclkSelect(fpga, config_value);
         break;
      case WPX_CONFIGURE_SMA_DIRECTION:
         WPX_ConfigSmaDirection(fpga, config_value);
         break;
     case WPX_CONFIGURE_PLL_SOURCE:
         WPX_ConfigPllSource(fpga, config_value);
         break;
      case WPX_CONFIGURE_SERDES_REF_CLK:
         WPX_ConfigMux3Reg(fpga, config_value);
         break;
      default:
         return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);
   }
   return WP_OK;
}

void WPX_ConfigBrgMux(wpx_fpga *fpga, wpx_fpga_config *config)
{
   WP_MEM_BYTE_SET(fpga->brg_mux_select, config->value);
}

void WPX_ConfigPpsAndFclkSelect(wpx_fpga *fpga, wpx_fpga_config *config)
{
   WP_MEM_BYTE_SET(fpga->pps_and_fclk_select, config->value);
}

void WPX_ConfigSmaDirection(wpx_fpga *fpga, wpx_fpga_config *config)
{
   WP_MEM_BYTE_SET(fpga->sma_direction, config->value);
}
void WPX_ConfigPllSource(wpx_fpga *fpga, wpx_fpga_config *config)
{
  WP_MEM_BYTE_SET(fpga->pll_source, config->value);
}
void WPX_ConfigMux3Reg(wpx_fpga *fpga, wpx_fpga_config *config)
{
   WP_MEM_BYTE_SET(fpga->mux_select_3, config->value);
}

WP_status WPX_BoardXgiConfig(WP_U32 wpid,
                             WP_port port)
{
   WP_status status;

   WPI_RETURN_IF_ERROR(status, WPX_BoardPhyAel2005Config(wpid, port));

   return WP_OK;
}

WP_status WPX_BoardZarlinkPllConfig(WP_U32 wpid, WP_U32 setup_cmd)
{
   wpx_fpga *fpga;
   WP_U32 revision = 0;
   WP_boolean reset_pll = WP_TRUE;
   
   if(WPX_SerdesUsedByPCIe(wpid, WPX_SERDES0) || WPX_SerdesUsedByPCIe(wpid, WPX_SERDES1))
      reset_pll = WP_FALSE;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);




   WPX_BoardRevisionGet(wpid, fpga, &revision);

#if 0
   printf("REVISION = %d\n", revision);
#endif

   switch (setup_cmd)
   {
      case WPX_PLL_CONFIG_DEFAULT:

         /* Reset PLL */
         if(reset_pll)
            WP_MEM_BYTE_SET(fpga->reset_pll, 0);
         WP_Delay(10);
         if(reset_pll)
            WP_MEM_BYTE_SET(fpga->reset_pll, 1);
         WP_Delay(1000);
         if(reset_pll)
            WP_MEM_BYTE_SET(fpga->spi_pll, 0x8); /* SPI PLL write enable */
         WP_Delay(1000);

         /* Set default config: */

         WPX_BoardWriteZarlinkPllReg(wpid, 0x1d, 0x00); /* filter BW is 0.1Hz */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x1e, 0x00); /* Disable freerun offset */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x1f, 0x2);  /* Default freerun mode      */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x20, 0x0);  /* Set DPLL 1 to input ref 0 */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x65, 0x4);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x67, 0x35);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x68, 0xc);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x69, 0x6);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6a, 0x12);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6b, 0x37);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6c, 0x6);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6d, 0x99);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6e, 0x6);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6f, 0x7f);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x70, 0x1);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x64, 0x8);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x68, 0x0);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x64, 0x0);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x44, 0x0);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x52, 0x11); /* Set apll_clk0/1_freq to 125MHz */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x57, 0x0);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x5c, 0x0);
         WPX_BoardWriteZarlinkPllReg(wpid, 0x60, 0xa3);

         if(revision == WPX_BOARD_REV4)
         {

            /* In WDS3 rev4 10G phy ref at 156.25MHz from diff1  */
            /* APLL_CLK_1 is connected to WDS-FPGA and APLL_CLK_0 is connected to first BRGI */

            WPX_BoardWriteZarlinkPllReg(wpid, 0x4a, 0xd4);
            WPX_BoardWriteZarlinkPllReg(wpid, 0x4b, 0x30);
            WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x5f); /* Enable apll_clk0/1 and apll_fp0/1,fsel0=0,fsel1=1,eth_en=1 */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x61, 0x31);

         }
         else
         {
            /* In WDS3 rev3 10G phy ref at 156.25MHz from diff0 rev4 diff 0 */
            /* APLL_CLK_0 is connected to WDS-FPGA and APLL_CLK_1 is connected to first BRGI */

            WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x4f); /* Enable apll_clk0/1 and apll_fp0/1,fsel0=0,fsel1=0,eth_en=1 */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x61, 0x33);
         }

         /* use default values for ref  pre-divders */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x7e, 0x00);
         break;

      case WPX_PLL_CONFIG_APLL_125:
         /* same as defult config */

         WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Enable apll_clk0/1 and apll_fp0/1,fsel0=1,fsel1=1,eth_en=1 */

         break;

      case WPX_PLL_CONFIG_APLL_125_FREERUN:

         WPX_BoardWriteZarlinkPllReg(wpid, 0x1e, 0x02); /* Enable freerun offset */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Enable apll_clk0/1 and apll_fp0/1,fsel0=1,fsel1=1,eth_en=1 */

         break;

      case WPX_PLL_CONFIG_APLL_125_MANUAL_NORMAL:

         WPX_BoardWriteZarlinkPllReg(wpid, 0x1f, 0x00); /* Set DPLL1 to manual normal mode. */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Enable apll_clk0/1 and apll_fp0/1,fsel0=1,fsel1=1,eth_en=1 */

         break;


      case WPX_PLL_CONFIG_APLL_125_P0_CLK0_100:
         /* Set APLL to 125MHz, locked to ref1. Set P0_CLK0 to 100MHz, locked to ref0
            ref1 is set to work with 20MHz and ref0 is set to work with 10MHz. */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x1d, 0x06); /* filter sets to fast lock   */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x1f, 0x00); /* Set DPLL1 to manual normal mode. */
//          WPX_BoardWriteZarlinkPllReg(wpid, 0x20, 0x00); /* Set DPLL 1 to input ref 0 */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x20, 0x01); /* Set DPLL 1 to input ref 1 */ //lior
         WPX_BoardWriteZarlinkPllReg(wpid, 0x2c, 0x00); /* Set DPLL2 to manual normal mode. */ //lior
         WPX_BoardWriteZarlinkPllReg(wpid, 0x2d, 0x00); /* Set DPLL 2 to input ref 0 */ //lior
//         WPX_BoardWriteZarlinkPllReg(wpid, 0x36, 0x8f); //default value. so is 0x37
         WPX_BoardWriteZarlinkPllReg(wpid, 0x38, 0xd4); /* p0_clk0 = 100MHz => 100M/8k =  */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x39, 0x30); /* = 12500 = 0x30d4 */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x4a, 0xd4); /* p1_clk0 = 100MHz => 100M/8k =  */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x4b, 0x30); /* = 12500 = 0x30d4 */

         WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Set APLLn_clk to ENET mode.      */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x52, 0x11); /* Set APLLn_clk to 125MHz.         */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x65, 0x09); /* ref0 with CustomA, ref1 with CustomB  */ //lior: was 0x09. now set to auto detect (0x00)

         WPX_BoardWriteZarlinkPllReg(wpid, 0x67, 0xe2); /* ref0 = 10MHz => CustomA = 10M/8K */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x68, 0x04); /* = 1250 = 0x04e2  */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x69, 0x0f); /* custA SCM low limit */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6a, 0x2d); /* custA SCM high limit */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6b, 0xc4); /* custA_cfm_low (0x7c4) */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6c, 0x07); /* custA_cfm_low */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6d, 0x3f); /* custA_cfm_high (0x83f) */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6e, 0x08); /* custA_cfm_high */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x6f, 0xff); /* custA_cfm_cycle (N) */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x70, 0x00); /* custA_cfm_div (D) */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x71, 0xc4); /* ref1 = 20MHz => CustomB = 20M/8K */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x72, 0x09); /* = 2500 = 0x09c4 */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x73, 0x07); /* custB SCM low limit */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x74, 0x17); /* custB SCM high limit */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x75, 0xc4); /* custB_cfm_low (0x7c4) */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x76, 0x07); /* custB_cfm_low */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x77, 0x3f); /* custB_cfm_high (0x83f) */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x78, 0x08); /* custB_cfm_high */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x79, 0x7f); /* custB_cfm_cycle (N) */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x7a, 0x01); /* custB_cfm_div (D) */

         break;


      default:
         return WPI_ERROR(WP_ERR_CONFIG);
      }

   WP_Delay(1000);
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPX_BoardPhyAel2005Config
 *
 * Purpose:    XGY PHY configuration
 *
 *
 * Description: Configures XGI PHY using the FPGA SPI I/F or
 *                                 using MDIO I/F in a case a connection from ENET Host to the PHY exists, Clause45 is used
 *
 *
 * Inputs:
 *       wpid    ID of the Winpath
 *       port    XGI port number -- WP_PORT_XGI1 or WP_PORT_XGI2
 *
 *
 * Outputs:
 *
 *
 *
 * Return Value:
 *     WP_OK if no errors, else returns one of the following error codes:
 *                    WP_ERR_WDDI, WP_ERR_CONFIG, WP_ERR_MII_MGMT_WRONG_MIND_STATUS
 *
 * Called by:
 *     WPX_BoardXgiConfig
 *
 ****************************************************************************/
WP_status WPX_BoardPhyAel2005Config(WP_U32 wpid, WP_port port)
{
   WP_status status = WP_OK;
   wpx_fpga* fpga = NULL;
   WP_U32 phy_port = 0;
   WP_U32 dev_type = 0;
   WP_U8 dip_switch = 0;
   WP_U8 sfp_enable = 0;
   WP_mii_mgmt_unit mii_mgmt_unit = {0};


   sfp_enable = (WPX_SFP_EN_SERD_0_1_ENABLE<<WPX_SFP_EN_SERD_0_1_MASK_SHIFT) |
      (WPX_SFP_EN_SERD_3_ENABLE <<WPX_SFP_EN_SERD_3_MASK_SHIFT)|
      (WPX_SFP_EN_SERD_4_ENABLE <<WPX_SFP_EN_SERD_4_MASK);

   WPX_FpgaMatchRegSet(wpid);
   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_GET(dip_switch, fpga->dip_switch);
   if((dip_switch & WPX_FPGA_DIP_SWITCH_BOARD_TYPE_MASK)>>WPX_FPGA_DIP_SWITCH_BOARD_TYPE_SHIFT
      == WPX_FPGA_DUAL_XGI)
      sfp_enable |= WPX_SFP_EN_SERD_2_ENABLE_DUAL_XGI <<WPX_SFP_EN_SERD_2_MASK_SHIFT;
   else
      sfp_enable |= WPX_SFP_EN_SERD_2_ENABLE_SINGLE_XGI <<WPX_SFP_EN_SERD_2_MASK_SHIFT;
   WP_MEM_BYTE_SET(fpga->sfp_tx_control, sfp_enable);

   WP_Delay(1000);

   switch (port)
   {
      case WP_PORT_XGI1:
      {
         phy_port = WPX_XGI1_PHY_OFFSET;
      }
      break;

      case WP_PORT_XGI2:
      {
         phy_port = WPX_XGI2_PHY_OFFSET;
      }
      break;

      default:
      {
         return WPI_ERROR(WP_ERR_CONFIG);
      }
   }

   if (WPI_SysMiiMgmtIsClause45Supported())
   {
      /* Clause45 Initialization */
      /* Set Ethernet mode register, enabling general Ethernet controller modes */
      
      memset(&mii_mgmt_unit, 0, sizeof(mii_mgmt_unit));

      mii_mgmt_unit.clock_divider = WP_MII_MGMT_MDC_DIVIDER_4096;

      WPI_RETURN_IF_ERROR(status, WP_SysMiiMgmtConfig(wpid,
                                                      WP_MII_MGMT_MDC_3,
                                                      &mii_mgmt_unit));
   }

   dev_type = WP_MII_MGMT_DEV_TYPE;

   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x00000000, 0x0000a040));
   WP_Delay(1000);

   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c017, 0xfeb0));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c013, 0xf341));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c210, 0x8000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c210, 0x8100));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c210, 0x8000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c210, 0x0000));
   WP_Delay(1000);
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c003, 0x181));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c010, 0x448a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c04a, 0x5200));
   WP_Delay(1000);
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc00, 0x20c5));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc01, 0x3c05));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc02, 0x6536));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc03, 0x2fe4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc04, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc05, 0x6624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc06, 0x2015));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc07, 0x3145));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc08, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc09, 0x27ff));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc0a, 0x300f));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc0b, 0x2c8b));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc0c, 0x300b));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc0d, 0x4009));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc0e, 0x400e));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc0f, 0x2f52));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc10, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc11, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc12, 0x2202));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc13, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc14, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc15, 0x2662));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc16, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc17, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc18, 0xd01e));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc19, 0x2862));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc1a, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc1b, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc1c, 0x2004));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc1d, 0x3c84));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc1e, 0x6436));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc1f, 0x2007));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc20, 0x3f87));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc21, 0x8676));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc22, 0x40b7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc23, 0xa746));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc24, 0x4047));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc25, 0x5673));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc26, 0x29c2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc27, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc28, 0x13d2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc29, 0x8bbd));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc2a, 0x28f2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc2b, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc2c, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc2d, 0x2122));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc2e, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc2f, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc30, 0x5cc3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc31, 0x0314));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc32, 0x2982));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc33, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc34, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc35, 0xd019));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc36, 0x20c2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc37, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc38, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc39, 0x2a04));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc3a, 0x3c74));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc3b, 0x6435));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc3c, 0x2fa4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc3d, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc3e, 0x6624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc3f, 0x5563));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc40, 0x2d82));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc41, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc42, 0x13d2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc43, 0x464d));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc44, 0x28f2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc45, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc46, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc47, 0x20c2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc48, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc49, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc4a, 0x2fb4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc4b, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc4c, 0x6624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc4d, 0x5563));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc4e, 0x2d82));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc4f, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc50, 0x13d2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc51, 0x2eb2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc52, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc53, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc54, 0x2002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc55, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc56, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc57, 0x0004));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc58, 0x2982));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc59, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc5a, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc5b, 0x2122));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc5c, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc5d, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc5e, 0x5cc3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc5f, 0x0317));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc60, 0x2f52));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc61, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc62, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc63, 0x2982));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc64, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc65, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc66, 0x22cd));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc67, 0x301d));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc68, 0x28f2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc69, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc6a, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc6b, 0x21a2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc6c, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc6d, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc6e, 0x5aa3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc6f, 0x2e02));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc70, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc71, 0x1312));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc72, 0x2d42));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc73, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc74, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc75, 0x2ff7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc76, 0x30f7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc77, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc78, 0x3c04));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc79, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc7a, 0x2807));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc7b, 0x31a7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc7c, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc7d, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc7e, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc7f, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc80, 0x2807));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc81, 0x3187));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc82, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc83, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc84, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc85, 0x2fe4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc86, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc87, 0x6437));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc88, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc89, 0x3c04));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc8a, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc8b, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc8c, 0x2514));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc8d, 0x3c64));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc8e, 0x6436));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc8f, 0xdff4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc90, 0x6436));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc91, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc92, 0x40a4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc93, 0x643c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc94, 0x4016));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc95, 0x8c6c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc96, 0x2b24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc97, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc98, 0x6435));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc99, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc9a, 0x2b24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc9b, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc9c, 0x643a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc9d, 0x4025));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc9e, 0x8a5a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cc9f, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca0, 0x27c1));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca1, 0x3011));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca2, 0x1001));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca3, 0xc7a0));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca4, 0x0100));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca5, 0xc502));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca6, 0x53ac));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca7, 0xc503));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca8, 0xd5d5));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cca9, 0xc600));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccaa, 0x2a6d));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccab, 0xc601));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccac, 0x2a4c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccad, 0xc602));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccae, 0x0111));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccaf, 0xc60c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb0, 0x5900));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb1, 0xc710));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb2, 0x0700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb3, 0xc718));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb4, 0x0700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb5, 0xc720));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb6, 0x4700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb7, 0xc801));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb8, 0x7f50));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccb9, 0xc802));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccba, 0x7760));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccbb, 0xc803));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccbc, 0x7fce));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccbd, 0xc804));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccbe, 0x5700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccbf, 0xc805));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc0, 0x5f11));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc1, 0xc806));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc2, 0x4751));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc3, 0xc807));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc4, 0x57e1));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc5, 0xc808));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc6, 0x2700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc7, 0xc809));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc8, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccc9, 0xc821));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccca, 0x0002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cccb, 0xc822));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cccc, 0x0014));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cccd, 0xc832));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccce, 0x1186));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cccf, 0xc847));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd0, 0x1e02));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd1, 0xc013));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd2, 0xf341));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd3, 0xc01a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd4, 0x0446));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd5, 0xc024));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd6, 0x1000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd7, 0xc025));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd8, 0x0a00));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccd9, 0xc026));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccda, 0x0c0c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccdb, 0xc027));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccdc, 0x0c0c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccdd, 0xc029));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccde, 0x00a0));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccdf, 0xc030));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce0, 0x0a00));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce1, 0xc03c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce2, 0x001c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce3, 0xc005));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce4, 0x7a06));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce5, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce6, 0x27c1));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce7, 0x3011));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce8, 0x1001));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cce9, 0xc620));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccea, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cceb, 0xc621));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccec, 0x003f));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cced, 0xc622));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccee, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccef, 0xc623));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf0, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf1, 0xc624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf2, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf3, 0xc625));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf4, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf5, 0xc627));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf6, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf7, 0xc628));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf8, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccf9, 0xc62c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccfa, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccfb, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccfc, 0x2806));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccfd, 0x3cb6));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccfe, 0xc161));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ccff, 0x6134));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd00, 0x6135));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd01, 0x5443));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd02, 0x0303));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd03, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd04, 0x000b));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd05, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd06, 0x2104));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd07, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd08, 0x2105));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd09, 0x3805));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd0a, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd0b, 0xdff4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd0c, 0x4005));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd0d, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd0e, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd0f, 0x5dd3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd10, 0x0306));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd11, 0x2ff7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd12, 0x38f7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd13, 0x60b7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd14, 0xdffd));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd15, 0x000a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd16, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000cd17, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ca00, 0x0080));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000ca12, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c323, 0x4619));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(wpid,dev_type,phy_port,0x00000001,0x0000c323, 0x4659));

   return WP_OK;
}


static void WPX_BoardWriteZarlinkPllReg(WP_U32 wpid, WP_U32 address, WP_U32 data)
{
   wpx_fpga *fpga;
   WP_U8 current_bit;
   WP_S32 ii;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_SET(fpga->spi_pll, 0x8);
   WP_Delay(WPX_ZARLING_DELAY);

   WP_MEM_BYTE_SET(fpga->spi_pll, 0xa);

   WP_Delay(WPX_ZARLING_DELAY);
   for(ii = WPX_ZARLINK_PLL_LAST_ADDRESS_BIT; ii >= 0; ii-- )
   {
      current_bit= 0x01 & (address>>ii);
      WP_MEM_BYTE_SET(fpga->spi_pll, current_bit|0x8);
      WP_MEM_BYTE_SET(fpga->spi_pll, current_bit|0xa);
   }

   WP_Delay(WPX_ZARLING_DELAY);
   for(ii = WPX_ZARLINK_PLL_LAST_DATA_BIT; ii >= 0; ii-- )
   {
      current_bit= 0x01 & (data>>ii);
      WP_MEM_BYTE_SET(fpga->spi_pll, current_bit|0x8);
      WP_MEM_BYTE_SET(fpga->spi_pll, current_bit|0xa);
   }
   WP_Delay(WPX_ZARLING_DELAY);
   WP_MEM_BYTE_SET(fpga->spi_pll, 0xc);
   WP_Delay(WPX_ZARLING_DELAY);
}

/*****************************************************************************
 *
 * Function:  WPX_BoardReadZarlinkPllReg
 *
 * Purpose: Reads zarlink PLL register
 *
 * Inputs:
 *     ufeid - the UFE id
 *     address - the address of the register
 *     data    - the data read from the register
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WPX_BoardReadZarlinkPllReg(WP_U32 wpid, WP_U32 address, WP_U32 *data)
{
   wpx_fpga *fpga;
   WP_U8 current_bit;
   WP_S32 ii;
   WP_U8 value;

   *data = 0;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *) WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_SET(fpga->spi_pll, 0x9);
   WP_Delay(WPX_ZARLING_DELAY);

   WP_MEM_BYTE_SET(fpga->spi_pll, 0xb);
   WP_Delay(WPX_ZARLING_DELAY);

   /*
    * Sends the address to the Zarlink PLL bit by bit.
    */
   for (ii = WPX_ZARLINK_PLL_LAST_ADDRESS_BIT; ii >= 0; ii--)
   {
      current_bit = 0x01 & (address >> ii);
      WP_MEM_BYTE_SET(fpga->spi_pll, current_bit | 0x8);
      WP_MEM_BYTE_SET(fpga->spi_pll, current_bit | 0xa);
   }
   WP_Delay(WPX_ZARLING_DELAY);

   /*
    * Reads the data from the Zarlink PLL bit by bit.
    */
   for (ii = WPX_ZARLINK_PLL_LAST_DATA_BIT; ii >= 0; ii--)
   {
      WP_MEM_BYTE_SET(fpga->spi_pll, 0x8);
      WP_MEM_BYTE_SET(fpga->spi_pll, 0xa);
      WP_MEM_BYTE_GET(value, fpga->spi_read);
      *data = *data | ((value & 0x01) << ii);
   }
   WP_Delay(WPX_ZARLING_DELAY);
   WP_MEM_BYTE_SET(fpga->spi_pll, 0x8);
   WP_Delay(WPX_ZARLING_DELAY);
   WP_MEM_BYTE_SET(fpga->spi_pll, 0xc);
   WP_Delay(WPX_ZARLING_DELAY);
}

/*****************************************************************************
 *
 * Function:  WPX_ExternalPllClockOutConfig
 *
 * Purpose: Set external PLL APLL, p0_clk0, p1_clk0
 *
 * Inputs:
 *     pllConfigParam - the PLL configuration struct
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPX_ExternalPllClockOutConfig(WPX_pll_cfg *pllConfigParam)
{
   WP_U32 upperByte,lowerByte;
   WP_U32 rate = 0 ,apll_cfg = 0;

   /* p0_clk0 or p1_clk0 */
   if(pllConfigParam->clock_out != wpx_apll_clk0)
   {
      if(pllConfigParam->clock_rate % 8)
      {
         return WPI_ERROR(WP_ERR_ERR_INVALID_PLL_CLOCK_RATE);
      }
      rate = pllConfigParam->clock_rate / 8;

      lowerByte = (rate & 0xFF);
      upperByte = ((rate >> 8) & 0xFF);

      if(pllConfigParam->clock_out == wpx_p1_clk0)
      {
         WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid, 0x4a, lowerByte);
         WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid, 0x4b, upperByte);
      }
      else
      {
         WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid, 0x38, lowerByte);
         WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid, 0x39, upperByte);
      }
   }
   else /* apll_clk0 */
   {
      WPX_BoardReadZarlinkPllReg(pllConfigParam->wpid,0x51,&apll_cfg);

      /* sets bits 4 and 6 to 0
         0 - the default, for SONET/SDH frequencies
         1 - set to 1 for Ethernet frequencies */
      apll_cfg = (apll_cfg & 0xaf);

      switch(pllConfigParam->apll_rate)
      {
         case wpx_f_125:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg | 0x50));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,1);
            break;

         case wpx_f_62_5:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg | 0x50));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,2);
            break;

         case wpx_f_77_76:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,2);
            break;

         case wpx_f_38_88:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,3);
            break;

         case wpx_f_19_44:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,4);
            break;

         case wpx_f_50:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg | 0x50));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,5);
            break;

         case wpx_f_9_72:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,5);
            break;

         case wpx_f_25:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg | 0x50));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,6);
            break;

         case wpx_f_12_5:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg | 0x50));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,7);
            break;

         case wpx_f_51_84:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,10);
            break;

         case wpx_f_25_92:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,11);
            break;

         case wpx_f_12_96:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,12);
            break;

         case wpx_f_6_48:
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x51,(apll_cfg));
            WPX_BoardWriteZarlinkPllReg(pllConfigParam->wpid,0x52,13);
            break;
         default:
            return WPI_ERROR(WP_ERR_ERR_INVALID_PLL_CLOCK_RATE);
      }
   }
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPX_PhyAel2005Write
 *
 * Purpose:    Writing to an XGY PHY register
 *
 *
 * Description: Writing to an XGI PHY using the FPGA SPI I/F or
 *                                    using MDIO I/F in a case a connection from ENET Host to the PHY exists, Clause45 is used
 *
 *
 * Inputs:
 *       wpid             ID of the Winpath
 *       dev_type         device number, for Clause45 case
 *       xgi_port_offset  PHY port number
 *       phy_addr         PHY address
 *       reg_addr         register address
 *       data             value to be written
 *
 *
 * Outputs:
 *
 *
 *
 * Return Value:
 *     WP_OK if no errors, else returns one of the following error codes:
 *                    WP_ERR_WDDI, WP_ERR_CONFIG, WP_ERR_MII_MGMT_WRONG_MIND_STATUS
 *
 * Called by:
 *     WPX_BoardPhyAel2005Config
 *
 ****************************************************************************/
static WP_status WPX_PhyAel2005Write(WP_U32 wpid,
                                     WP_U32 dev_type,
                                     WP_U32 xgi_port_offset,
                                     WP_U32 phy_addr,
                                     WP_U32 reg_addr,
                                     WP_U32 data)
{
   WP_status status;
   WP_U32    phy_param;

   if (WPI_SysMiiMgmtIsClause45Supported())
   {
      phy_param = WP_SYS_MII_MGMT_DEV_TYPE(dev_type) | WP_MII_MGMT_MDIO4 | xgi_port_offset;

      WPI_RETURN_IF_ERROR(status,
                          WP_SysMiiMgmtWrite(wpid, phy_param, reg_addr, data));
   }
   else
   {
      WPI_RETURN_IF_ERROR(status,
                          WPX_PhyAel2005FpgaWrite(xgi_port_offset, phy_addr, reg_addr, data));
   }


   return WP_OK;
}

static WP_status WPX_PhyAel2005FpgaWrite(WP_U32 xgi_port_offset,
                                         WP_U32 phy_addr,
                                         WP_U32 reg_addr,
                                         WP_U32 data)
{
   WP_status status;
   wpx_fpga *fpga;
   WP_U32 temp1, temp2;

   WPX_FpgaMatchRegSet(0);
   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(0, WPI_WDS3_FPGA_BASE);

   temp1= ((0x20|xgi_port_offset) <<8) & 0x0000ff00;
   WP_MEM_GET(temp2, fpga->fpga_config);
   temp2=temp2 & 0xff000000;
   temp1=temp1|temp2;
   WP_MEM_SET(fpga->fpga_config, temp1);

   temp1= phy_addr<<24;
   temp2= (reg_addr & 0x000000ff)<<8;
   temp1= temp1|temp2;

   WP_MEM_SET(fpga->fpga_reg_addr, temp1);

   temp1=(reg_addr & 0x0000ff00)<<16;
   WP_MEM_SET(fpga->fpga_write_data, temp1);
   WPI_RETURN_IF_ERROR(status,
                       WPX_Ael2005PollWriteOkBit(0, WPX_AEL2005_WRITE_OK_BIT, WPX_AEL2005_WRITE_OK_MASK));

   WP_MEM_BYTE_SET(fpga->fpga_smw, 0x1);
   WPI_RETURN_IF_ERROR(status,
                       WPX_Ael2005PollWriteOkBit(0, WPX_AEL2005_WRITE_OK_BIT, WPX_AEL2005_WRITE_OK_MASK));

   WP_MEM_BYTE_SET(fpga->fpga_smw, 0x0);


   temp1= ((xgi_port_offset<<8) & 0x00001f00);
   WP_MEM_GET(temp2, fpga->fpga_config);
   temp2=temp2 & 0xff000000;
   temp1=temp1|temp2;
   WP_MEM_SET(fpga->fpga_config, temp1);

   temp1=(data & 0x000000ff)<<8;
   temp2= (phy_addr &0x0000001f)<<24;
   temp1= temp1|temp2;
   WP_MEM_SET(fpga->fpga_reg_addr, temp1);

   temp1=(data & 0x0000ff00)<<16;
   WP_MEM_SET(fpga->fpga_write_data, temp1);
   WPI_RETURN_IF_ERROR(status,
                       WPX_Ael2005PollWriteOkBit(0, WPX_AEL2005_WRITE_OK_BIT, WPX_AEL2005_WRITE_OK_MASK));

   WP_MEM_BYTE_SET(fpga->fpga_smw, 0x1);
   WPI_RETURN_IF_ERROR(status,
                       WPX_Ael2005PollWriteOkBit(0, WPX_AEL2005_WRITE_OK_BIT, WPX_AEL2005_WRITE_OK_MASK));

   WP_MEM_BYTE_SET(fpga->fpga_smw, 0x0);
   return WP_OK;
}

static WP_status  WPX_Ael2005PollWriteOkBit(WP_U32 wpid,
                                            WP_U32 polled_value,
                                            WP_U32 polled_mask)
{
   wpx_fpga *fpga;
   WP_U32 data, ii;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   for(ii = 0; ii< WPX_AEL2005_POOL_LOOPS; ii++)
   {
      WP_Delay(WPX_AEL2005_POOL_DELAY);
      WP_MEM_GET(data, fpga->fpga_read_data);
      if((data & polled_mask) == (polled_value & polled_mask))
         break;
   }
   if(ii ==  WPX_AEL2005_POOL_LOOPS)
      return WPI_ERROR(WP_ERR_WDDI);
   else
      return WP_OK;
}

WP_status WPX_SfpTxEnable(WP_U32 wpid,
                          WP_U8 enable,
                          WP_U32 mask)
{
   wpx_fpga *fpga;
   WP_U8 sfp_tx_control, dip_switch;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_GET(sfp_tx_control, fpga->sfp_tx_control);
   if(enable == WP_ENABLE)
   {
      if(mask & WPX_SFP_EN_SERD_0_1)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_0_1_MASK;
         sfp_tx_control |= WPX_SFP_EN_SERD_0_1_ENABLE<<WPX_SFP_EN_SERD_0_1_MASK_SHIFT;
      }
      if(mask & WPX_SFP_EN_SERD_2)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_2_MASK;

         WP_MEM_BYTE_GET(dip_switch, fpga->dip_switch);
         if((dip_switch & WPX_FPGA_DIP_SWITCH_BOARD_TYPE_MASK)>>WPX_FPGA_DIP_SWITCH_BOARD_TYPE_SHIFT
            == WPX_FPGA_DUAL_XGI)
            sfp_tx_control |= WPX_SFP_EN_SERD_2_ENABLE_DUAL_XGI<<WPX_SFP_EN_SERD_2_MASK_SHIFT;
         else
            sfp_tx_control |= WPX_SFP_EN_SERD_2_ENABLE_SINGLE_XGI<<WPX_SFP_EN_SERD_2_MASK_SHIFT;
      }
      if(mask & WPX_SFP_EN_SERD_3)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_3_MASK;
         sfp_tx_control |= WPX_SFP_EN_SERD_3_ENABLE<<WPX_SFP_EN_SERD_3_MASK_SHIFT;
      }
      if(mask & WPX_SFP_EN_SERD_4)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_4_MASK;
         sfp_tx_control |= WPX_SFP_EN_SERD_4_ENABLE<<WPX_SFP_EN_SERD_4_MASK_SHIFT;
      }
   }
   else
   {
      if(mask & WPX_SFP_EN_SERD_0_1)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_0_1_MASK;
         sfp_tx_control |= WPX_SFP_EN_SERD_0_1_DISABLE<<WPX_SFP_EN_SERD_0_1_MASK_SHIFT;
      }
      if(mask & WPX_SFP_EN_SERD_2)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_2_MASK;

         WP_MEM_BYTE_GET(dip_switch, fpga->dip_switch);
         if((dip_switch & WPX_FPGA_DIP_SWITCH_BOARD_TYPE_MASK)>>WPX_FPGA_DIP_SWITCH_BOARD_TYPE_SHIFT
            == WPX_FPGA_DUAL_XGI)
            sfp_tx_control |= WPX_SFP_EN_SERD_2_DISABLE_DUAL_XGI<<WPX_SFP_EN_SERD_2_MASK_SHIFT;
         else
            sfp_tx_control |= WPX_SFP_EN_SERD_2_DISABLE_SINGLE_XGI<<WPX_SFP_EN_SERD_2_MASK_SHIFT;
      }
      if(mask & WPX_SFP_EN_SERD_3)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_3_MASK;
         sfp_tx_control |= WPX_SFP_EN_SERD_3_DISABLE<<WPX_SFP_EN_SERD_3_MASK_SHIFT;
      }
      if(mask & WPX_SFP_EN_SERD_4)
      {
         sfp_tx_control &= ~WPX_SFP_EN_SERD_4_MASK;
         sfp_tx_control |= WPX_SFP_EN_SERD_4_DISABLE<<WPX_SFP_EN_SERD_4_MASK_SHIFT;
      }
   }


   WP_MEM_BYTE_SET(fpga->sfp_tx_control, sfp_tx_control);

   WP_Delay(100);
   return WP_OK;
}

/* Freq offset is a 28bit signed value */
#define WPX_ZLPLL_FREERUN_OFFSET_MAX_PPB 0x07FFFFFF
#define WPX_ZLPLL_FREERUN_OFFSET_MIN_PPB 0x08000000

void WPX_BoardWriteZarlinkPll1FreeRunFreqOffset(WP_U32 wpid, WP_S32 pll_ppb_offset)
{

   WP_S32 max_value = WPX_ZLPLL_FREERUN_OFFSET_MAX_PPB;
   WP_S32 min_value = WPX_ZLPLL_FREERUN_OFFSET_MIN_PPB;
   WP_U8 reg_add0=0 , reg_val0=0 ;
   WP_U8 reg_add1=0 , reg_val1=0 ;
   WP_U8 reg_add2=0 , reg_val2=0 ;
   WP_U8 reg_add3=0 , reg_val3=0 ;

     /* Cut-off value */
   if(pll_ppb_offset > max_value)
   {
   	pll_ppb_offset = max_value;  /* max value */
   }

   if(pll_ppb_offset < -max_value)
   {
     	pll_ppb_offset = min_value;  /* min value */
   }

   reg_add0= 0x65;
   reg_val0= (WP_U8)pll_ppb_offset;
   reg_add1= 0x66;
   reg_val1= (WP_U8)(pll_ppb_offset >> 8);
   reg_add2= 0x67;
   reg_val2= (WP_U8)(pll_ppb_offset >> 16);
   reg_add3= 0x68;
   reg_val3= (WP_U8)(pll_ppb_offset >> 24);

   WPX_BoardWriteZarlinkPllReg(wpid, 0x64, 0x01);  /* access page 1 */
   /* set values */
   WPX_BoardWriteZarlinkPllReg(wpid,reg_add0,reg_val0);
   WPX_BoardWriteZarlinkPllReg(wpid,reg_add1,reg_val1);
   WPX_BoardWriteZarlinkPllReg(wpid,reg_add2,reg_val2);
   WPX_BoardWriteZarlinkPllReg(wpid,reg_add3,reg_val3);

   WPX_BoardWriteZarlinkPllReg(wpid, 0x64, 0x00);  /* set access back to page 0 */
}

WP_status WPX_BoardRevisionGet(WP_U32 wpid, wpx_fpga *fpga, WP_U32 *revision)
{
   WP_status status = WP_OK;
   WP_U8 dip_switch;
   WP_MEM_BYTE_GET(dip_switch, fpga->dip_switch);
   if(((dip_switch & WPX_FPGA_DIP_SWITCH_REVISION_MASK)>>WPX_FPGA_DIP_SWITCH_REVISION_SHIFT) == WPX_BOARD_REV1_3)
   {
      *revision = WPX_BOARD_REV1_3;
   }
   else
   {
      *revision = WPX_BOARD_REV4;
   }
   return status;
}
#endif

// XALYO
#undef int
#undef char
#undef short
#undef long
#undef float
#undef double
#undef signed
#undef unsigned

void WPX_WddiCompilerVersion(char *str) {
    sprintf(str, "%d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    return;
}

typedef struct pci_map_t {
    unsigned char *fpga;
    unsigned char *wpath;
    unsigned char *param;
    unsigned char *packet;
    unsigned char *intl;
    unsigned char *appl;
    unsigned int  fpga_size;
    unsigned int  wpath_size;
    unsigned int  param_size;
    unsigned int  packet_size;
    unsigned int  intl_size;
    unsigned int  appl_size;
} pci_map_t;

int xp2_dc_pll_reg_read(unsigned int wpid, unsigned int reg, unsigned char *pval);
int xp2_dc_pll_reg_write(unsigned int wpid, unsigned int reg, unsigned char val);

// Copied, adapted from production tests
static pci_map_t map;

#ifdef __linux__
static unsigned int xsByteSwap(unsigned int data)
{
#ifdef WPX_TARGET_mips_linux
    return data;
#else
    unsigned int x;

    x = data;
   
    return (((x & 0x000000ff) << 24) |
            ((x & 0x0000ff00) <<  8) |
            ((x & 0x00ff0000) >>  8) |
            ((x & 0xff000000) >> 24));
#endif
}

static unsigned short xsByteSwap16(unsigned short data)
{
#ifdef WPX_TARGET_mips_linux
    return data;
#else
    unsigned short x;
   
    x = data;
   
    return (((x & 0x00ff) << 8) |
            ((x & 0xff00) >> 8));
#endif
}
#endif

pci_map_t *wp3_map_get(int wpid)
{
#ifdef __linux__
    map.wpath = (unsigned char *) WPL_RIF_VIRTUAL(wpid, 0) + 0x1e000000;
#ifdef WPX_TARGET_mips_linux
    map.fpga  = (unsigned char *) WPL_PHY_VIRTUAL(wpid, 0) + 0x1f100000; // no swap
#else
    map.fpga  = (unsigned char *) WPL_PHY_VIRTUAL(wpid, 0) + 0x1f000000; // swap
#endif
    //printf("WinPath @ %p, FPGA @ %p\n", map.wpath, map.fpga); fflush(stdout);
#else
    map.wpath = (unsigned char *)0xbe000000;
    map.fpga  = (unsigned char *)0xbf100000;
#endif

    return &map;
}

static int wp3_get(unsigned int wpid, unsigned int *pval, unsigned int addr)
{
    pci_map_t *map;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

#ifdef __linux__
    *pval = xsByteSwap(*(volatile unsigned int *)(map->wpath + addr));
#else
    *pval = *(volatile unsigned int *)(map->wpath + addr);
#endif

    return 0;
}

static int wp3_put(unsigned int wpid, unsigned int val, unsigned int addr)
{
    pci_map_t *map;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

#ifdef __linux__
    *(volatile unsigned int *)(map->wpath + addr) = xsByteSwap(val);
#else
    *(volatile unsigned int *)(map->wpath + addr) = val;
#endif

    return 0;
}

int xp2_get(unsigned int wpid, unsigned int *pval, unsigned int addr)
{
    pci_map_t *map;
    unsigned int val;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

    val = *(volatile unsigned int *)(map->fpga + addr);

    *pval = val;

    return 0;
}

int xp2_put(unsigned int wpid, unsigned int val, unsigned int addr)
{
    pci_map_t *map;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

    *(volatile unsigned int *)(map->fpga + addr) = val;

    return 0;
}

int xp2_get_16(unsigned int wpid, unsigned short *pval, unsigned int addr)
{
    pci_map_t *map;
    unsigned int val;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

    val = *(volatile unsigned short *)(map->fpga + addr);

    *pval = val;

    return 0;
}

int xp2_put_16(unsigned int wpid, unsigned short val, unsigned int addr)
{
    pci_map_t *map;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

    *(volatile unsigned short *)(map->fpga + addr) = val;

    return 0;
}

static int xp2_dc_inited = 0;
static int xp2_dc_init(unsigned int wpid) {
    unsigned int val, id;

    if(xp2_dc_inited == 0) {

        xp2_get(wpid, &id, 0x2c);

        // Only on XS-AMC-WP3C
        if(id != 0x00104567) {
            printf("----> ERROR: Calling xp2_dc_init from id %x!\n", id);
            return -1;
        }

        // Deassert reset on Daughter Card
        xp2_get(wpid, &val, 0x1020);
        xp2_put(wpid,  val | 1, 0x1020);

        // Sleep 100 ms
        WP_Delay(100000);

        xp2_dc_inited = 1;
    }

    return 0;
}

int xp2_dc_get(unsigned int wpid, unsigned short *pval, unsigned int addr)
{
    pci_map_t *map;
    unsigned short val;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

    xp2_dc_init(wpid);

    val = *(volatile unsigned short *)(map->fpga + 0x300000 + addr);

    *pval = val;

    return 0;
}

int xp2_dc_put(unsigned int wpid, unsigned short val, unsigned int addr)
{
    pci_map_t *map;

    xp2_dc_init(wpid);

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

    *(volatile unsigned short *)(map->fpga + 0x300000 + addr) = val;

    return 0;
}

int ufe4_get(unsigned int wpid, unsigned int *pval, unsigned int addr)
{
    pci_map_t *map;
    unsigned int val;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }
#ifdef __linux__
    val = xsByteSwap(*(volatile unsigned int *)(map->fpga + 0x300000 + addr));
#else
    val = *(volatile unsigned int *)(map->fpga + 0x300000 + addr);
#endif

    *pval = val;

    return 0;
}

int ufe4_put(unsigned int wpid, unsigned int val, unsigned int addr)
{
    pci_map_t *map;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

#ifdef __linux__
    *(volatile unsigned int *)(map->fpga + 0x300000 + addr) = xsByteSwap(val);
#else
    *(volatile unsigned int *)(map->fpga + 0x300000 + addr) = val;
#endif

    return 0;
}

int ufe4_get_16(unsigned int wpid, unsigned short *pval, unsigned int addr)
{
    pci_map_t *map;
    unsigned short val;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

#ifdef __linux__
    val = xsByteSwap16(*(volatile unsigned short *)(map->fpga + 0x300000 + addr));
#else
    val = *(volatile unsigned short *)(map->fpga + 0x300000 + addr);
#endif

    *pval = val;

    return 0;
}

int ufe4_put_16(unsigned int wpid, unsigned short val, unsigned int addr)
{
    pci_map_t *map;

    map = wp3_map_get(wpid);
    if(map == NULL) {
        return -1;
    }

#ifdef __linux__
    *(volatile unsigned short *)(map->fpga + 0x300000 + addr) = xsByteSwap16(val);
#else
    *(volatile unsigned short *)(map->fpga + 0x300000 + addr) = val;
#endif

    return 0;
}

WP_status WPX_SfpTxEnable(WP_U32 wpid,
                          WP_U8  enable,
                          WP_U32 port)
{
    unsigned short sval;
    unsigned int   id;
    unsigned int   val;

    xp2_get(wpid, &id, 0x2c);

    //XS-AMC-WP3C
    if(id == 0x00104567) {
        if(enable) {
            switch(port) {
            case 0 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval & 0xfffe, 8);
                }
                break;
            case 1 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval & 0xffef, 8);
                }
                break;
            case 2 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval & 0xfeff, 8);
                }
                break;
            case 3 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval & 0xefff, 8);
                }
                break;
            default :
                return -1;
            }
        } else {
            switch(port) {
            case 0 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval | 0x0001, 8);
                }
                break;
            case 1 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval | 0x0010, 8);
                }
                break;
            case 2 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval | 0x0100, 8);
                }
                break;
            case 3 :
                {
                    xp2_dc_get(wpid, &sval, 8);
                    xp2_dc_put(wpid, sval | 0x1000, 8);
                }
                break;
            default :
                return -1;
            }
        }
    }

    //XS-3200
    if(id == 0x00114567) {
        if(enable) {
            switch(port) {
            case 0 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            case 1 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            case 2 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            case 3 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            default :
                return -1;
            }
        } else {
            switch(port) {
            case 0 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            case 1 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            case 2 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            case 3 :
                {
                    printf("WPX_SfpTxEnable: not implemented yet\n");
                }
                break;
            default :
                return -1;
            }
        }
    }

    //XS-3100
    if(id == 0x00124567) {
        if(enable) {
            switch(port) {
            case 0 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val & 0xfffffffe, 0x1048);
                }
                break;
            case 1 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val & 0xffffffef, 0x1048);
                }
                break;
            case 2 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val & 0xfffffeff, 0x1048);
                }
                break;
            case 3 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val & 0xffffefff, 0x1048);
                }
                break;
            default :
                return -1;
            }
        } else {
            switch(port) {
            case 0 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val | 0x00000001, 0x1048);
                }
                break;
            case 1 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val | 0x00000010, 0x1048);
                }
                break;
            case 2 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val | 0x00000100, 0x1048);
                }
                break;
            case 3 :
                {
                    xp2_get(wpid, &val, 0x1048);
                    xp2_put(wpid, val | 0x00001000, 0x1048);
                }
                break;
            default :
                return -1;
            }
        }
    }

    return 0;
}

WP_status WPX_PortModeSet(WP_U32 wpid,
                          WP_U8 mode,
                          WP_U32 port)
{
    unsigned short sval;
    unsigned int   val, id;

    xp2_get(wpid, &id, 0x2c);

    //XS-AMC-WP3C
    if(id == 0x00104567) {

        if(port > 3) return -1;

        switch(mode) {
        case WPX_PORT_MODE_ATM :
            {
                xp2_dc_get(wpid, &sval, 12);
                xp2_dc_put(wpid, (sval & ~(3 << (2*port))) | (2 << (2*port)), 12);
            }
            break;
        case WPX_PORT_MODE_ETH :
            {
                xp2_dc_get(wpid, &sval, 12);
                xp2_dc_put(wpid, (sval & ~(3 << (2*port))) | (1 << (2*port)), 12);
            }
            break;
        default :
            return -1;
        }
    }

    // XS-3200
    if(id == 0x00114567) {

        if(port != 3) return -1;

        switch(mode) {
        case WPX_PORT_MODE_ATM :
            {
                xp2_get(wpid, &val, 0x1020);
                xp2_put(wpid, val & ~4, 0x1020);
            }
            break;
        case WPX_PORT_MODE_ETH :
            {
                xp2_get(wpid, &val, 0x1020);
                xp2_put(wpid, val | 4, 0x1020);
            }
            break;
        default :
            return -1;
        }
    }

    // XS-3100
    if(id == 00124567) {
        return -1;
    }

    return 0;
}

WP_status WPX_PortModeGet(WP_U32 wpid,
                          WP_U8 *pmode,
                          WP_U32 port)
{
    unsigned short sval;
    unsigned int   val, id;

    xp2_get(wpid, &id, 0x2c);

    //XS-AMC-WP3C
    if(id == 0x00104567) {

        if(port > 3) return -1;

        xp2_dc_get(wpid, &sval, 12);

        if((sval & (3 << (2*port))) == (2 << (2*port))) {
            *pmode = WPX_PORT_MODE_ATM;
        } else {
            *pmode = WPX_PORT_MODE_ETH;
        }
    }

    // XS-3200
    if(id == 0x00114567) {

        if(port != 3) return -1;

        xp2_get(wpid, &val, 0x1020);

        if(val & 4) {
            *pmode = WPX_PORT_MODE_ETH;
        } else {
            *pmode = WPX_PORT_MODE_ATM;
        }
    }

    // XS-3100
    if(id == 00124567) {
        return -1;
    }

    return 0;
}

WP_status WPX_PortLedSet(WP_U32 wpid,
                          WP_U8 mode,
                          WP_U32 port)
{
    unsigned int val;

    if(mode == WPX_PORT_LED_ON) {
        xp2_get(wpid, &val, 0x1020);
        xp2_put(wpid, val & ~(1 << (port + 9)), 0x1020);
    } else {
        xp2_get(wpid, &val, 0x1020);
        xp2_put(wpid, val |  (1 << (port + 9)), 0x1020);
    }

    return 0;
}

static int phy_inited = 0;

static WP_status phy_init(WP_U32 wpid)
{  
    unsigned short sval;
    unsigned char  cval;
    unsigned int   val, id;

    xp2_get(wpid, &id, 0x2c);

    if(phy_inited == 0) {
        //XS-AMC-WP3C
        if(id == 0x00104567) {
            // Set PHY MSK
            wp3_put(wpid, 0xfff0459a, 0x1081c);
            //*(volatile unsigned int *)0xbe01081c = 0xfff0459a;

            // Assert PLL reset
            xp2_dc_get(wpid, &sval, 6);
            xp2_dc_put(wpid, sval & 0xfffe, 6);

            WP_Delay(100000);

            // Deassert PLL reset
            xp2_dc_get(wpid, &sval, 6);
            xp2_dc_put(wpid, sval | 0x0001, 6);

            WP_Delay(100000);

            // Assert PHY reset
            xp2_dc_get(wpid, &sval, 18);
            xp2_dc_put(wpid, sval & 0xfffe, 18);

            WP_Delay(100000);

            xp2_dc_pll_reg_read(wpid, 0x00, &cval);

            //printf("PLL reg 0 = %02x\n", cval);

            xp2_dc_pll_reg_write(wpid, 0x61, 0x23);
            xp2_dc_pll_reg_write(wpid, 0x38, 0x6a);
            xp2_dc_pll_reg_write(wpid, 0x39, 0x18);
            xp2_dc_pll_reg_write(wpid, 0x52, 0x42);

            // Enable PLL
            xp2_dc_get(wpid, &sval, 6);
            xp2_dc_put(wpid, sval | 0x0003, 6);

            WP_Delay(100000);

            // Deassert PHY reset
            xp2_dc_get(wpid, &sval, 18);
            xp2_dc_put(wpid, sval | 0x0001, 18);

            WP_Delay(100000);

            xp2_dc_get(wpid, &sval, 0x20000);
            //printf("PHY reg 0 = %04x\n", sval);
        }
        if(id == 0x00124567) {
            // Set PHY MSK
            *(volatile unsigned int *)0xbe010824 = 0xfff0459a;

            // Assert PLL reset
            xp2_get(wpid, &val, 0x1044);
            xp2_put(wpid, val & 0xfffffffe, 0x1044);

            WP_Delay(100000);

            // Deassert PLL reset
            xp2_get(wpid, &val, 0x1044);
            xp2_put(wpid, val | 0x00000001, 0x1044);

            WP_Delay(100000);

            // Assert PHY reset
            xp2_get(wpid, &val, 0x1020);
            xp2_put(wpid, val & 0xfffbffff, 0x1020);

            WP_Delay(100000);

            xp2_dc_pll_reg_read(wpid, 0x00, &cval);

            //printf("PLL reg 0 = %02x\n", cval);

            xp2_dc_pll_reg_write(wpid, 0x61, 0x23); // 77.76 MHz diff
            xp2_dc_pll_reg_write(wpid, 0x38, 0x6a); // 50 MHz util bus
            xp2_dc_pll_reg_write(wpid, 0x39, 0x18);
            xp2_dc_pll_reg_write(wpid, 0x52, 0x42); // 77.76 MHz single

            // Enable PLL
            xp2_get(wpid, &val, 0x1044);
            xp2_put(wpid, val | 0x00000003, 0x1044);

            WP_Delay(100000);

            // Deassert PHY reset
            xp2_get(wpid, &val, 0x1020);
            xp2_put(wpid, val | 0x00000001, 0x1020);

            WP_Delay(100000);

            xp2_get_16(wpid, &sval, 0x300000);
            //printf("PHY reg 0 = %04x\n", sval);
        }

        phy_inited = 1;
    }

    return 0;
}

WP_status WPX_PhyRegRead(WP_U32 wpid,
                         WP_U32 reg,
                         WP_U16 *val)
{
    unsigned int   id;

    phy_init(wpid);

    xp2_get(wpid, &id, 0x2c);

    if(id == 0x00104567) {
        xp2_dc_get(wpid, val, 0x20000 + reg);
    }

    if(id == 0x00124567) {
        xp2_get_16(wpid, val, 0x300000 + reg);
    }

    return 0;
}

WP_status WPX_PhyRegWrite(WP_U32 wpid,
                          WP_U32 reg,
                          WP_U16 val)
{
    unsigned int   id;

    phy_init(wpid);

    xp2_get(wpid, &id, 0x2c);

    if(id == 0x00104567) {
        xp2_dc_put(wpid, val, 0x20000 + reg);
    }

    if(id == 0x00124567) {
        xp2_put_16(wpid, val, 0x300000 + reg);
    }

    return 0;
}

static unsigned int wpathGPIO(unsigned int wpid, unsigned char dir, unsigned int gpio, unsigned char no, unsigned char val)
{
    unsigned short val16;
    unsigned int   val32;
    unsigned char  bitVal=0;
    unsigned int   readReg;
    unsigned int   writeReg;
    unsigned int   dirBit;
    unsigned int   readBit;
    unsigned int   writeBit;
    unsigned int   id;

    //printf("wpathGPIO: dir = %d gpio = %x no = %x, val = %x\n", dir, gpio, no, val);

    // Format in no is:
    //     bit    7: 1 -> Need to set dir bit
    //     bit    6: 1 -> Write bitno is different and need to be taken from gpio
    //     bit    5: 1 -> Write register is different and need to be taken from gpio
    //     bit 4..0: read bitno (up to 31 bits)
    //
    // Format in gpio is:
    //     bit     31: 0->MEZ FPGA, 1->AMC FPGA
    //     bit 30..29: unused
    //     bit 28..24: write bitno
    //     bit 23..17: unused
    //     bit 16..12: dir bitno
    //     bit 11..00: read register

    readReg = gpio & 0x00000fff;

    if(no & 0x20) {
        writeReg = readReg - 1; // The write register is always preceding the read register
    } else {
        writeReg = readReg;
    }

    readBit = no & 0x1f;

    if(no & 0x40) {
        writeBit = (gpio & 0x1f000000) >> 24;
    } else {
        writeBit = readBit;
    }

    dirBit  = (gpio & 0x0001f000) >> 12;

    if(gpio & 0x80000000) {
        // WP3 FPGA
        readReg  *=4;
        writeReg *=4;

        if(dir == 1) {

            // Set write dir
            if(no & 0x80) {
                xp2_get(wpid, &val32, writeReg);
            val32 |= (1 << dirBit);
            xp2_put(wpid, val32, writeReg);
            }

            // write
            xp2_get(wpid, &val32, writeReg);
            val32 &= ~(1 << writeBit);
            xp2_put(wpid, val32 | ((val & 1) << writeBit), writeReg);
        }

        if(dir == 0) {
            // Set read dir
            if(no & 0x80) {
                xp2_get(wpid, &val32, writeReg);
                val32 &= ~(1 << dirBit);
                xp2_put(wpid, val32, writeReg);
            }
        }

        // Read
        xp2_get(wpid, &val32, readReg);
        bitVal = (val32 & (1<<readBit))?1:0;

    } else {
        xp2_get(wpid, &id, 0x2c);

        // XS-AMC-WP3C Mezzanine FPGA
        if(id == 0x00104567) {
            readReg  *=2;
            writeReg *=2;

            if(dir == 1) {

                // Set write dir
                if(no & 0x80) {
                    xp2_dc_get(wpid, &val16, writeReg);
                    val16 |= (1 << dirBit);
                    xp2_dc_put(wpid, val16, writeReg);
                }

                // write
                xp2_dc_get(wpid, &val16, writeReg);
                val16 &= ~(1 << writeBit);
                xp2_dc_put(wpid, val16 | ((val & 1) << writeBit), writeReg);
            }

            if(dir == 0) {
                // Set read dir
                if(no & 0x80) {
                    xp2_dc_get(wpid, &val16, writeReg);
                    val16 &= ~(1 << dirBit);
                    xp2_dc_put(wpid, val16, writeReg);
                }
            }

            // Read
            xp2_dc_get(wpid, &val16, readReg);
            bitVal = (val16 & (1<<readBit))?1:0;
        }

        // XS-3200 UFE4
        if(id == 0x00114567) {
            readReg  *=4;
            writeReg *=4;

            if(dir == 1) {

                // Set write dir
                if(no & 0x80) {
                    ufe4_get(wpid, &val32, writeReg);
                    val32 |= (1 << dirBit);
                    ufe4_put(wpid, val32, writeReg);
                }

                // write
                ufe4_get(wpid, &val32, writeReg);
                val32 &= ~(1 << writeBit);
                ufe4_put(wpid, val32 | ((val & 1) << writeBit), writeReg);
            }

            if(dir == 0) {
                // Set read dir
                if(no & 0x80) {
                    ufe4_get(wpid, &val32, writeReg);
                    val32 &= ~(1 << dirBit);
                    ufe4_put(wpid, val32, writeReg);
                }
            }

            // Read
            ufe4_get(wpid, &val32, readReg);
            bitVal = (val32 & (1<<readBit))?1:0;
        }
    }

    return bitVal;
}


static void zarlinkWait(void) {
    int count;

    count = 100000;
    while(count--);

    return;
}

int xp2_dc_pll_reg_read(unsigned int wpid, unsigned int reg, unsigned char *pval)
{
    unsigned char val;
    unsigned char csb, sck, si, so;
    unsigned int  gpio_csb, gpio_sck, gpio_si, gpio_so, id;

    xp2_get(wpid, &id, 0x2c);


    if(id == 0x00104567) {
        csb = 2;
        sck = 3;
        si  = 4;
        so  = 11;

        gpio_csb = 3;
        gpio_sck = 3;
        gpio_si  = 3;
        gpio_so  = 3;
    }

    if(id == 0x00124567) {
        csb = 2;
        sck = 3;
        si  = 4;
        so  = 19;

        gpio_csb = 0x80000411;
        gpio_sck = 0x80000411;
        gpio_si  = 0x80000411;
        gpio_so  = 0x80000411;
    }

    // Initial conditions
    wpathGPIO(wpid, 1, gpio_csb, csb, 1);
    wpathGPIO(wpid, 1, gpio_sck, sck, 1);
    wpathGPIO(wpid, 1, gpio_si,  si,  1);
    zarlinkWait();

    // SCK low to select MSB transmission
    wpathGPIO(wpid, 1, gpio_sck, sck, 0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_csb, csb, 0);
    zarlinkWait();

    // Read
    wpathGPIO(wpid, 1, gpio_si,  si,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck, 1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck, 0);
    zarlinkWait();

    // Put address
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 6); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 5); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 4); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 3); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 2); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 1); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 0); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();

    // Get data
    val = 0;

    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 7;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 6;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 5;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 4;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 3;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 2;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 1;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    val |= wpathGPIO(wpid, 0, gpio_so, so, 0) << 0;
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();

    // Deselect
    wpathGPIO(wpid, 1, gpio_csb, csb, 1);
    zarlinkWait();

    *pval = val;

    return 0;
}

int xp2_dc_pll_reg_write(unsigned int wpid, unsigned int reg, unsigned char val)
{
    unsigned char csb, sck, si;
    unsigned int  gpio_csb, gpio_sck, gpio_si, id;

    xp2_get(wpid, &id, 0x2c);

    if(id == 0x00104567) {
        csb = 2;
        sck = 3;
        si  = 4;

        gpio_csb = 3;
        gpio_sck = 3;
        gpio_si  = 3;
    }

    if(id == 0x00124567) {
        csb = 2;
        sck = 3;
        si  = 4;

        gpio_csb = 0x80000411;
        gpio_sck = 0x80000411;
        gpio_si  = 0x80000411;
    }

    // Initial conditions
    wpathGPIO(wpid, 1, gpio_csb, csb, 1);
    wpathGPIO(wpid, 1, gpio_sck, sck, 1);
    wpathGPIO(wpid, 1, gpio_si,  si,  1);
    zarlinkWait();

    // SCK low to select MSB transmission
    wpathGPIO(wpid, 1, gpio_sck, sck, 0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_csb, csb, 0);
    zarlinkWait();

    // Write
    wpathGPIO(wpid, 1, gpio_si,  si,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck, 1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck, 0);
    zarlinkWait();

    // Put address
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 6); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 5); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 4); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 3); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 2); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 1); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, reg >> 0); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();

    // Put Data
    wpathGPIO(wpid, 1, gpio_si, si, val >> 7); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, val >> 6); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, val >> 5); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, val >> 4); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, val >> 3); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, val >> 2); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, val >> 1); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_si, si, val >> 0); // 0
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  1);
    zarlinkWait();
    wpathGPIO(wpid, 1, gpio_sck, sck,  0);
    zarlinkWait();

    // Deselect
    wpathGPIO(wpid, 1, gpio_csb, csb, 1);
    zarlinkWait();

    return 0;
}

void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid,WP_U32 connector_id)
{
    unsigned int val;

    // Fix EMPHY pads pullup/down
    wp3_put(wpid, 0x00020042, 0x8080); // pull-up on TENB SP001
    wp3_put(wpid, 0x00420002, 0x809c); // pull-up on RENB SP014
    wp3_put(wpid, 0x00820002, 0x8088); // pull-down on PTPA SP004
    wp3_put(wpid, 0x00020082, 0x80a0); // pull-down on PRPA SP017

    // Set CS4#
    wp3_get(wpid, &val, 0x10820);
    wp3_put(wpid, 0x1f302007, 0x10820);
    wp3_put(wpid, 0xfff07f24, 0x10824);
    //wp3_put(wpid, 0xfff07f3c, 0x10824);

    // Set CS2# and CS3# in order to make sure it doesn't disturb UFE4...
    wp3_put(wpid, 0xfff0423c, 0x10814);
    wp3_put(wpid, 0xfff0423c, 0x1081C);

    wp3_put(wpid, 0x00666666, 0x10890);

    // De-Assert reset
    xp2_get(wpid, &val, 0x1020);
    //xp2_put(wpid,  val & ~1, 0x1020);
    xp2_put(wpid,  val | 1, 0x1020);

    // Set PROGRAM_B bit -> PROGRAM_B pin goes low
    xp2_get(wpid, &val, 0x1020);
    if((val & 8) != 8) {
        xp2_put(wpid,  val |  8, 0x1020);
    }

    // Make sure PROGRAM_B stays low for 500ns
    WP_Delay(1);

    // Check that INIT_B is low
    xp2_get(wpid, &val, 0x1024);
    if((val & 0x200) != 0x000) {
        printf("WPX_FpgaFirmwareInitializationStart: ERROR --> INIT_B stayed high\n");
    }

    // Clear PROGRAM_B bit -> PROGRAM_B pin goes high
    xp2_get(wpid, &val, 0x1020);
    xp2_put(wpid,  val & ~8, 0x1020);

    // Wait 10 millisecond an check INIT_B is high
    WP_Delay(10000);
    xp2_get(wpid, &val, 0x1024);
    if((val & 0x200) != 0x200) {
        printf("WPX_FpgaFirmwareInitializationStart: ERROR --> INIT_B stayed low\n");
    }

    // Assert CSI_B
    wp3_put(wpid, 0x00131313, 0xca28);

    // CSI_B has a weak pull-down, make sure it goes low, wait 1 ms
    WP_Delay(1000);

    return;
}

WP_status WPX_FpgaFirmwareInitializationStatus(WP_U32 wpid)
{  
    unsigned int val;

    // Deassert CSI_B
    wp3_put(wpid, 0x13131313, 0xca28);
    WP_Delay(1000);

    // Check DONE is high
    xp2_get(wpid, &val, 0x1024);
    if((val & 0x100) != 0x100) {
        printf("WPX_FpgaFirmwareInitializationStart: ERROR --> DONE stayed low\n");
        return -1;
    }

    return 0;
}

void WPX_Ufe4HwReset(WP_U32 wpid,WP_U32 connector_id)
{
   unsigned int val;

   // Clear UFE4_RESET_N bit -> UFE4_RESET_N pin goes low
   xp2_get(wpid, &val, 0x1020);
   xp2_put(wpid,  val & ~1, 0x1020);

   // Make sure UFE4_RESET_N stays low for 500ns
   WP_Delay(1);

   // SET UFE4_RESET_N bit -> UFE4_RESET_N pin goes high
   xp2_put(wpid,  val |  1, 0x1020);

   return;
}

WP_status WPX_CrBoardPinConfig(WP_U32 wpid, wpx_wp_cr_pin_io wp_cr_pin_io)
{
    printf("WPX_CrBoardPinConfig: not implemented\n");
    return -1;
}

WP_U8 WPX_Ufe412CpldInterruptMaskGet(WP_U32 wpid)
{
       return 0;
}

void WPX_Ufe412CpldInterruptMaskSet(WP_U32 wpid, WP_U8 mask)
{
        return;
}



