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


static void WPX_BoardWriteZarlinkPllReg(WP_U32 wpid, WP_U32 address, WP_U32 data);
static void WPX_BoardReadZarlinkPllReg(WP_U32 wpid, WP_U32 address, WP_U32 *data);
WP_status WPX_ExternalPllClockOutConfig(WPX_pll_cfg *pllConfigParam);
static WP_status  WPX_Ael2005PollWriteOkBit(WP_U32 wpid,
                                            WP_U32 polled_value,
                                            WP_U32 polled_mask);
static WP_status  WPX_PhyAel2005Write(WP_U32 xgi_port_offset,
                                      WP_U32 phy_addr,
                                      WP_U32 reg_addr,
                                      WP_U32 data);

wpx_board_configuration current_configuration = WPX_CONFIGURE_DEFAULT_CHECKIN;

#define WPX_RESET 0x1
void WPX_Reboot(void)
{
   wpx_fpga *fpga;

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(0, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_SET(fpga->reset_all, WPX_RESET);
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



WP_status WPX_BoardConfigure(WP_U32 wpid, wpx_board_configuration configuration)
{
   WP_status status = WP_OK;

   switch(configuration) {
      case WPX_CONFIGURE_DEFAULT_CHECKIN:
      case WPX_CONFIGURE_CHECKIN_TDI_16:
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII:
      case WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4:
      case WPX_CONFIGURE_8TDI_1UPI_1XGI_10SGMII:
      case WPX_CONFIGURE_2UPI_2XGI_6SGMII2G:
      case WPX_CONFIGURE_1UPI_1XGI_4GMII_8SGMII:
      case WPX_CONFIGURE_1XGI_6RGMII_6SGMII:
      case WPX_CONFIGURE_1UPI_1XGI_6RTBI_6SGMII:
      case WPX_CONFIGURE_1XGI_6SMII_6SGMII:
      case WPX_CONFIGURE_2RMII_8TDI_1XGI_10SGMII:
      case WPX_CONFIGURE_2SGMII_2SGMII2G_16TDI:
         break;
      default:
         return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);
   }

   /* Configure clock for XGI, Serdeses, BRG from Zarlink PLL */
   WPI_RETURN_IF_ERROR(status, WPX_BoardZarlinkPllConfig(wpid, WPX_PLL_CONFIG_DEFAULT));


   WPI_RETURN_IF_ERROR(status,
                       WPX_PinMuxSet(wpid, configuration));

   WPI_RETURN_IF_ERROR(status,
                       WPX_SerDesSet(wpid, configuration));

   WPI_RETURN_IF_ERROR(status,
                       WPX_FpgaUpdateMux(wpid, configuration));

   current_configuration = configuration;

   return status;
}

WP_status WPU_PinMuxAllocateToPort(WP_U32 port, WP_U32 mode)
{
   /* do nothing, everything is done by EnetPhyInit */
   return WP_OK;
}

void WPX_FpgaMatchRegSet(WP_U32 wpid)
{
   WP_U32 *fpga_match_reg = (WP_U32 *)WPL_RIF_VIRTUAL(wpid, (APP_MASK_MATCH_OFFSET+APP_MASK_MATCH_FPGA_OFFSET));

   WP_MEM_SET(*fpga_match_reg, (WPI_WDS3_FPGA_BASE & 0xffff0000) | 0x1007);
}

WP_status WPX_FpgaUpdateMux(WP_U32 wpid, wpx_board_configuration configuration)
{
   wpx_fpga *fpga;

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
      case WPX_CONFIGURE_CHECKIN_TDI_16:
      case WPX_CONFIGURE_2RMII_8TDI_1XGI_10SGMII:
      case WPX_CONFIGURE_2SGMII_2SGMII2G_16TDI:
      case WPX_CONFIGURE_1XGI_6RGMII_6SGMII:
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
         WP_MEM_BYTE_SET(fpga->mux_select_2, 0);
         break;
      case WPX_CONFIGURE_1XGI_6SMII_6SGMII:
         break;
      default:
         return WPI_ERROR(WP_ERR_BOARD_CONFIGURATION);
   }


   return WP_OK;
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


WP_status WPX_BoardZarlinkPllConfig(WP_U32 wpid, WP_U32 setup_cmd)
{
   wpx_fpga *fpga;
   WP_U32 revision = WPX_BOARD_REV_SLA_SLB;

   WPX_FpgaMatchRegSet(wpid);

   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WP_MEM_BYTE_SET(fpga->reset_pll, 0);
   WP_Delay(10);
   WP_MEM_BYTE_SET(fpga->reset_pll, 1);
   WP_Delay(1000);
   WP_MEM_BYTE_SET(fpga->spi_pll, 0x8);
   WP_Delay(1000);

   WPX_BoardRevisionGet(wpid, fpga, &revision);

   WPX_BoardWriteZarlinkPllReg(wpid, 0x1f, 0x2);
   WPX_BoardWriteZarlinkPllReg(wpid, 0x20, 0x0); /* Set DPLL 1 to input ref 0 */
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
   WPX_BoardWriteZarlinkPllReg(wpid, 0x64, 0x8); /* Set Page to offset '8' */
   WPX_BoardWriteZarlinkPllReg(wpid, 0x68, 0x0);
   WPX_BoardWriteZarlinkPllReg(wpid, 0x64, 0x0); /* Set Page to offset '0' back */
   WPX_BoardWriteZarlinkPllReg(wpid, 0x44, 0x0);
   WPX_BoardWriteZarlinkPllReg(wpid, 0x57, 0x0);
   WPX_BoardWriteZarlinkPllReg(wpid, 0x5c, 0x0);
   WPX_BoardWriteZarlinkPllReg(wpid, 0x1d, 0x26);

   if(revision == WPX_BOARD_REV_SPO)
   {
      WPX_BoardWriteZarlinkPllReg(wpid, 0x4a, 0xd4);
      WPX_BoardWriteZarlinkPllReg(wpid, 0x4b, 0x30);
      WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f);
      WPX_BoardWriteZarlinkPllReg(wpid, 0x52, 0x01);
      WPX_BoardWriteZarlinkPllReg(wpid, 0x60, 0xa3);
      WPX_BoardWriteZarlinkPllReg(wpid, 0x61, 0x11);
   }
   else
   {
      WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x4f);
      WPX_BoardWriteZarlinkPllReg(wpid, 0x60, 0xa3);
      WPX_BoardWriteZarlinkPllReg(wpid, 0x61, 0x33);
   }

   /* use default values for ref  pre-divders */

   WPX_BoardWriteZarlinkPllReg(wpid, 0x7e, 0x00);
   if((setup_cmd != WPX_PLL_CONFIG_DEFAULT) &&
      (revision == WPX_BOARD_REV_SPO))
   {
      /* TBD */
      printf("Warning! APLL_125 configuration support should be added for REVB !\n");
   }
   else
   {
      switch (setup_cmd)
      {
         case WPX_PLL_CONFIG_APLL_125:
            WPX_BoardWriteZarlinkPllReg(wpid, 0x1e, 0x04); /* Disable freerun offset BITS [3:2] must be set to 01*/
            WPX_BoardWriteZarlinkPllReg(wpid, 0x1d, 0x00); /* filter BW is 0.1Hz */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x52, 0x11); /* Set APLLn_clk to 125MHz. */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Set APLLn_clk to ENET mode. */
            break;
         case WPX_PLL_CONFIG_APLL_125_FREERUN:
            WPX_BoardWriteZarlinkPllReg(wpid, 0x1e, 0x06); /* Enable freerun offset BITS [3:2] must be set to 01*/
            WPX_BoardWriteZarlinkPllReg(wpid, 0x1d, 0x00); /* filter BW is 0.1Hz */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x52, 0x11); /* Set APLLn_clk to 125MHz. */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Set APLLn_clk to ENET mode. */
            break;

         case WPX_PLL_CONFIG_APLL_125_MANUAL_NORMAL:

            WPX_BoardWriteZarlinkPllReg(wpid, 0x1d, 0x00); /* filter BW is 0.1Hz.              */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x1f, 0x00); /* Set DPLL1 to manual normal mode. */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x52, 0x11); /* Set APLLn_clk to 125MHz.         */
            WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Set APLLn_clk to ENET mode.      */
            break;


   case WPX_PLL_CONFIG_APLL_125_P0_CLK0_100:
         /* Set APLL to 125MHz, locked to ref1. Set P0_CLK0 to 100MHz, locked to ref0
            ref1 is set to work with 20MHz and ref0 is set to work with 10MHz. */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x1d, 0x06); /* filter sets to fast lock   */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x1f, 0x00); /* Set DPLL1 to manual normal mode. */
//          WPX_BoardWriteZarlinkPllReg(wpid, 0x20, 0x00); /* Set DPLL 1 to input ref 0 */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x20, 0x01); /* Set DPLL 1 to input ref 1 */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x2c, 0x00); /* Set DPLL2 to manual normal mode. */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x2d, 0x00); /* Set DPLL 2 to input ref 0 */
//         WPX_BoardWriteZarlinkPllReg(wpid, 0x36, 0x8f); //default value. so is 0x37
         WPX_BoardWriteZarlinkPllReg(wpid, 0x38, 0xd4); /* p0_clk0 = 100MHz => 100M/8k =  */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x39, 0x30); /* = 12500 = 0x30d4 */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x4a, 0xd4); /* p1_clk0 = 100MHz => 100M/8k =  */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x4b, 0x30); /* = 12500 = 0x30d4 */


         WPX_BoardWriteZarlinkPllReg(wpid, 0x51, 0x7f); /* Set APLLn_clk to ENET mode.      */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x52, 0x11); /* Set APLLn_clk to 125MHz.         */
         WPX_BoardWriteZarlinkPllReg(wpid, 0x65, 0x09); /* ref0 with CustomA, ref1 with CustomB  */ // was 0x09. now set to auto detect (0x00)

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

         case WPX_PLL_CONFIG_DEFAULT:
            break;
         default:
            printf("Warning! Worng configuration for Zarlink PLL!\n");
      }

   }

   WP_Delay(1000);
   return WP_OK;
}

WP_status WPX_BoardPhyAel2005Config(WP_U32 wpid, WP_port port)
{
   WP_status status;
   wpx_fpga *fpga;
   WP_U32 phy_port=0;

   WPX_FpgaMatchRegSet(wpid);
   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(wpid, WPI_WDS3_FPGA_BASE);

   WPI_MEM_BYTE_SET(fpga->sfp_tx_control, 0xa);
   WP_Delay(1000);

   if(port == WP_PORT_XGI1)
      phy_port = WPX_XGI1_PHY_OFFSET;
   else if (port == WP_PORT_XGI2)
      phy_port = WPX_XGI2_PHY_OFFSET;
   else
      return WPI_ERROR(WP_ERR_CONFIG);

   WPI_RETURN_IF_ERROR(status,WPX_PhyAel2005Write(phy_port,0x00000001,0x00000000, 0x0000a040));
   WP_Delay(1000);

   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c017, 0xfeb0));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c013, 0xf341));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c210,0x8000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c210, 0x8100));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c210, 0x8000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c210, 0x0000));
   WP_Delay(1000);
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c003, 0x181));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c010, 0x448a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c04a, 0x5200));
   WP_Delay(1000);
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc00, 0x20c5));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc01, 0x3c05));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc02, 0x6536));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc03, 0x2fe4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc04, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc05, 0x6624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc06, 0x2015));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc07, 0x3145));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc08, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc09, 0x27ff));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc0a, 0x300f));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc0b, 0x2c8b));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc0c, 0x300b));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc0d, 0x4009));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc0e, 0x400e));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc0f, 0x2f52));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc10, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc11, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc12, 0x2202));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc13, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc14, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc15, 0x2662));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc16, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc17, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc18, 0xd01e));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc19, 0x2862));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc1a, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc1b, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc1c, 0x2004));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc1d, 0x3c84));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc1e, 0x6436));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc1f, 0x2007));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc20, 0x3f87));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc21, 0x8676));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc22, 0x40b7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc23, 0xa746));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc24, 0x4047));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc25, 0x5673));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc26, 0x29c2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc27, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc28, 0x13d2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc29, 0x8bbd));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc2a, 0x28f2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc2b, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc2c, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc2d, 0x2122));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc2e, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc2f, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc30, 0x5cc3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc31, 0x0314));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc32, 0x2982));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc33, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc34, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc35, 0xd019));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc36, 0x20c2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc37, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc38, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc39, 0x2a04));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc3a, 0x3c74));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc3b, 0x6435));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc3c, 0x2fa4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc3d, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc3e, 0x6624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc3f, 0x5563));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc40, 0x2d82));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc41, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc42, 0x13d2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc43, 0x464d));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc44, 0x28f2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc45, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc46, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc47, 0x20c2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc48, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc49, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc4a, 0x2fb4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc4b, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc4c, 0x6624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc4d, 0x5563));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc4e, 0x2d82));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc4f, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc50, 0x13d2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc51, 0x2eb2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc52, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc53, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc54, 0x2002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc55, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc56, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc57, 0x0004));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc58, 0x2982));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc59, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc5a, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc5b, 0x2122));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc5c, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc5d, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc5e, 0x5cc3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc5f, 0x0317));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc60, 0x2f52));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc61, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc62, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc63, 0x2982));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc64, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc65, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc66, 0x22cd));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc67, 0x301d));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc68, 0x28f2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc69, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc6a, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc6b, 0x21a2));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc6c, 0x3012));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc6d, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc6e, 0x5aa3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc6f, 0x2e02));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc70, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc71, 0x1312));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc72, 0x2d42));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc73, 0x3002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc74, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc75, 0x2ff7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc76, 0x30f7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc77, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc78, 0x3c04));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc79, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc7a, 0x2807));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc7b, 0x31a7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc7c, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc7d, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc7e, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc7f, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc80, 0x2807));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc81, 0x3187));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc82, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc83, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc84, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc85, 0x2fe4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc86, 0x3cd4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc87, 0x6437));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc88, 0x20c4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc89, 0x3c04));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc8a, 0x6724));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc8b, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc8c, 0x2514));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc8d, 0x3c64));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc8e, 0x6436));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc8f, 0xdff4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc90, 0x6436));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc91, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc92, 0x40a4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc93, 0x643c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc94, 0x4016));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc95, 0x8c6c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc96, 0x2b24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc97, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc98, 0x6435));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc99, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc9a, 0x2b24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc9b, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc9c, 0x643a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc9d, 0x4025));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc9e, 0x8a5a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cc9f, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca0, 0x27c1));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca1, 0x3011));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca2, 0x1001));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca3, 0xc7a0));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca4, 0x0100));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca5, 0xc502));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca6, 0x53ac));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca7, 0xc503));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca8, 0xd5d5));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cca9, 0xc600));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccaa, 0x2a6d));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccab, 0xc601));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccac, 0x2a4c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccad, 0xc602));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccae, 0x0111));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccaf, 0xc60c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb0, 0x5900));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb1, 0xc710));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb2, 0x0700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb3, 0xc718));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb4, 0x0700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb5, 0xc720));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb6, 0x4700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb7, 0xc801));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb8, 0x7f50));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccb9, 0xc802));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccba, 0x7760));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccbb, 0xc803));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccbc, 0x7fce));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccbd, 0xc804));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccbe, 0x5700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccbf, 0xc805));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc0, 0x5f11));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc1, 0xc806));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc2, 0x4751));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc3, 0xc807));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc4, 0x57e1));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc5, 0xc808));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc6, 0x2700));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc7, 0xc809));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc8, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccc9, 0xc821));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccca, 0x0002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cccb, 0xc822));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cccc, 0x0014));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cccd, 0xc832));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccce, 0x1186));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cccf, 0xc847));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd0, 0x1e02));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd1, 0xc013));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd2, 0xf341));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd3, 0xc01a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd4, 0x0446));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd5, 0xc024));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd6, 0x1000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd7, 0xc025));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd8, 0x0a00));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccd9, 0xc026));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccda, 0x0c0c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccdb, 0xc027));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccdc, 0x0c0c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccdd, 0xc029));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccde, 0x00a0));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccdf, 0xc030));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce0, 0x0a00));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce1, 0xc03c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce2, 0x001c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce3, 0xc005));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce4, 0x7a06));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce5, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce6, 0x27c1));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce7, 0x3011));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce8, 0x1001));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cce9, 0xc620));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccea, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cceb, 0xc621));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccec, 0x003f));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cced, 0xc622));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccee, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccef, 0xc623));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf0, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf1, 0xc624));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf2, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf3, 0xc625));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf4, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf5, 0xc627));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf6, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf7, 0xc628));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf8, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccf9, 0xc62c));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccfa, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccfb, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccfc, 0x2806));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccfd, 0x3cb6));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccfe, 0xc161));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ccff, 0x6134));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd00, 0x6135));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd01, 0x5443));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd02, 0x0303));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd03, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd04, 0x000b));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd05, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd06, 0x2104));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd07, 0x3c24));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd08, 0x2105));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd09, 0x3805));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd0a, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd0b, 0xdff4));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd0c, 0x4005));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd0d, 0x6524));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd0e, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd0f, 0x5dd3));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd10, 0x0306));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd11, 0x2ff7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd12, 0x38f7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd13, 0x60b7));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd14, 0xdffd));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd15, 0x000a));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd16, 0x1002));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000cd17, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ca00, 0x0080));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000ca12, 0x0000));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c323, 0x4619));
   WPI_RETURN_IF_ERROR(status, WPX_PhyAel2005Write(phy_port,0x00000001,0x0000c323, 0x4659));

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
         printf("Clock rate is not a multiplication of 8KHz\n");
         return WP_ERR_ERR_INVALID_PLL_CLOCK_RATE;
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
            printf("this is almost impossible!\n");
      }
   }
   return WP_OK;
}

static WP_status  WPX_PhyAel2005Write(WP_U32 xgi_port_offset, WP_U32 phy_addr, WP_U32 reg_addr, WP_U32 data)
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
   if(((dip_switch & WPX_FPGA_DIP_SWITCH_BOARD_TYPE_MASK)>>WPX_FPGA_DIP_SWITCH_REVISION_SHIFT)
      == 0)
   {
      *revision = WPX_BOARD_REV_SLA_SLB;
   }
   else
   {
      *revision = WPX_BOARD_REV_SPO;
   }
   return status;
}

void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid)
{
   return;

}