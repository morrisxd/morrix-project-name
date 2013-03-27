/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      OC12 CARD INITIALIZATION                        */
/************************************************************************/

/* This initialization code assumes that the host port for the PMC5357
   is implemented as a 64-bit port. */

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
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"     /* For WPI_ERROR */
#endif
#include "wpx_oc12_pmc5357.h"
#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif
#include "veneer/wpi_board_data_int.h"
#include "veneer/wpi_veneer.h"
#include "../board/wpx_board_if.h"

static void WPU_Delay(void);

WP_status WPU_Oc12Pmc5357CardInit(WP_U32 port, WP_U32 mode)
{
   WP_U32 major_mode;
   WP_U32 minor_mode;
   WP_U32 transaction_size;
   WP_U32 clk_source;
   wpu_pmc5357 *xcvr;
   wpi_match_mask save;
   WP_status status;

   transaction_size = mode & 0x00F00000;
   major_mode =       mode & 0x000F0000;
   minor_mode =       mode & 0x0000FFFF;
   clk_source =       mode & 0x0F000000;

   /* set init value */
   save.match_address = 0xffffffff;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   WPI_RETURN_IF_ERROR(status,
                       WPX_CpldSet(port, mode));
   
   WPU_Delay();

   switch (port)
   {
      case WP_PORT_UPI1:
      {
         WPX_BoardExtCardInit(WPX_MODE_OC12_PMC5357, WPX_SERIAL_CONNECTOR_0, &save);

         xcvr = (wpu_pmc5357 *) WPL_PHY_VIRTUAL(0, APP_UPI1_OC12_PMC5357);
         break;
      }
      case WP_PORT_UPI2:
      {
         WPX_BoardExtCardInit(WPX_MODE_OC12_PMC5357, WPX_SERIAL_CONNECTOR_1, &save);

         xcvr = (wpu_pmc5357 *) WPL_PHY_VIRTUAL(0, APP_UPI2_OC12_PMC5357);
         break;
      }
      default:
         status = WPI_ERROR(WP_ERR_PORT);
         goto clean_return;
   }
   switch (major_mode)
   {
      case WPU_MODE_OC12_ATM:
      {
         /* Master reset */
         WPI_MEM_BYTE_SET(xcvr->master_reset.reg[0], 0x9a);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->master_reset.reg[0], 0x1a);
         WPU_Delay();
         /* invert sdet polarity */
         /* polarity is inverted for WP2 */
         WPI_MEM_BYTE_SET(xcvr->crsi_config.reg[0], APP_OC12_CRSI_POLARITY);
         WPU_Delay();
         /* ATM mode */
         WPI_MEM_BYTE_SET(xcvr->rul3_config.reg[0], 0x04);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->tul3_config.reg[0], 0x04);
         WPU_Delay();
         /* Tx FIFO watermark */
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_high_watermark.reg[0], 0xc0);
         WPU_Delay();
         /* DLL reset */
         WPI_MEM_BYTE_SET(xcvr->dll_rfclk_reset.reg[0], 0x0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->dll_tfclk_reset.reg[0], 0x0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->dll_ptclk_reset.reg[0], 0x0);
         WPU_Delay();
         /* FIFO reset */
         WPI_MEM_BYTE_SET(xcvr->txcp_config_1.reg[0], 0x05);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->rxcp_fifo_cntl.reg[0], 0x11);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txcp_config_1.reg[0], 0x04);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->rxcp_fifo_cntl.reg[0], 0x00);
         WPU_Delay();
         /* Path signal label set */
         WPI_MEM_BYTE_SET(xcvr->tpop_path_signal_label.reg[0], 0x13);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->sptb_exp_path_signal_label.reg[0], 0x13);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC12_POS:
      {
         /* Master reset */
         WPI_MEM_BYTE_SET(xcvr->master_reset.reg[0], 0x9a);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->master_reset.reg[0], 0x1a);
         WPU_Delay();
         /* invert sdet polarity */
         WPI_MEM_BYTE_SET(xcvr->crsi_config.reg[0], APP_OC12_CRSI_POLARITY);
         WPU_Delay();
         /* POS mode */
         WPI_MEM_BYTE_SET(xcvr->rul3_config.reg[0], 0x06);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->tul3_config.reg[0], 0x06);
         WPU_Delay();
         /* Tx FIFO watermark */
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_control.reg[0], 0x0b);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_low_watermark.reg[0], 0xb0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_high_watermark.reg[0], 0xb0);
         WPU_Delay();
         /* DLL reset */
         WPI_MEM_BYTE_SET(xcvr->dll_rfclk_reset.reg[0], 0x0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->dll_tfclk_reset.reg[0], 0x0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->dll_ptclk_reset.reg[0], 0x0);
         WPU_Delay();
         /* FIFO reset */
         WPI_MEM_BYTE_SET(xcvr->rxfp_config.reg[0], 0x13);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_config.reg[0], 0x13);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->rxfp_config.reg[0], 0x12);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_config.reg[0], 0x12);
         WPU_Delay();
         /* Path signal label set */
         WPI_MEM_BYTE_SET(xcvr->tpop_path_signal_label.reg[0], 0x16);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->sptb_exp_path_signal_label.reg[0], 0x16);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC12_SPI:
      {
         /* Master reset */
         WPI_MEM_BYTE_SET(xcvr->master_reset.reg[0], 0x9a);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->master_reset.reg[0], 0x1a);
         WPU_Delay();
         /* invert sdet polarity */
         WPI_MEM_BYTE_SET(xcvr->crsi_config.reg[0], APP_OC12_CRSI_POLARITY);
         WPU_Delay();
         /* POS mode */
         WPI_MEM_BYTE_SET(xcvr->rul3_config.reg[0], 0x07);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->tul3_config.reg[0], 0x07);
         WPU_Delay();
         /* Tx FIFO watermark */
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_control.reg[0], 0x0b);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_low_watermark.reg[0], 0xb0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_high_watermark.reg[0], 0xb0);
         WPU_Delay();
         /* DLL reset */
         WPI_MEM_BYTE_SET(xcvr->dll_rfclk_reset.reg[0], 0x0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->dll_tfclk_reset.reg[0], 0x0);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->dll_ptclk_reset.reg[0], 0x0);
         WPU_Delay();
         /* FIFO reset */
         WPI_MEM_BYTE_SET(xcvr->rxfp_config.reg[0], 0x13);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_config.reg[0], 0x13);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->rxfp_config.reg[0], 0x12);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_config.reg[0], 0x12);
         WPU_Delay();
         /* Path signal label set */
         WPI_MEM_BYTE_SET(xcvr->tpop_path_signal_label.reg[0], 0x16);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->sptb_exp_path_signal_label.reg[0], 0x16);
         WPU_Delay();
         break;
      }
      default:
         status = WPI_ERROR(WP_ERR_PORT);
         goto clean_return;
   }
   switch(transaction_size)
   {
      case WPU_128_BYTES:
      {
         WPI_MEM_BYTE_SET(xcvr->rxfp_rx_high_watermark.reg[0], 0x7f);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_low_watermark.reg[0],  0x7e);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->txfp_tx_high_watermark.reg[0], 0x7f);
         WPU_Delay();

         WPI_MEM_BYTE_SET(xcvr->txfp_tx_control.reg[0], 0x02);
         WPU_Delay();

         break;
      }
      case WPU_64_BYTES:
      {
         break;
      }
   }
   switch(minor_mode)
   {
      case WPU_MODE_OC12_LOOPBACK_CLOSE:
      {
         WPI_MEM_BYTE_SET(xcvr->master_config_1.reg[0], 0x18);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC12_LOOPBACK_MIDDLE:
      {
         WPI_MEM_BYTE_SET(xcvr->master_config_2.reg[0], 0x1f);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC12_LOOPBACK_FAR:
      {
         WPI_MEM_BYTE_SET(xcvr->master_config_1.reg[0], 0x14);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC12_LOOPBACK_OFF:
         /* in case no loopback is used the clock is recovered from Adtech */
         if (clk_source != WPU_MODE_OC12_INT_CLOCK)
            WPI_REG_BYTE_SET(xcvr->master_config_2.reg[0], 0xf);
         WPU_Delay();
         break;
      default:
         break;
   }

   status = WP_OK;

  clean_return:

   /* Restore the registers.*/
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}

static void WPU_Delay(void)
{
   WPL_Delay(10);
}

