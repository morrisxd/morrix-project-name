/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      OC3 CARD INITIALIZATION                         */
/************************************************************************/

/* This initialization code assumes that the host port for the PMC5351
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
#include "wpx_oc3_pmc5351.h"
#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif
#include "veneer/wpi_board_data_int.h"
#include "veneer/wpi_veneer.h"

static void WPU_Delay(void);


WP_status WPU_Oc3Pmc5351CardInit(WP_U32 port, WP_U32 mode)
{
   WP_U32 major_mode;
   WP_U32 transaction_size;
   WP_U32 phy0_mode;
   WP_U32 phy1_mode;
   WP_U32 phy2_mode;
   WP_U32 phy3_mode;
   wpu_pmc5351 *xcvr;
   WP_U32 ii;
   wpi_match_mask save;
   WP_U32 status;

   transaction_size = mode & 0x00F00000;
   major_mode =       mode & 0x000F0000;
   phy0_mode =        mode & 0x0000000F;
   phy1_mode =        mode & 0x000000F0;
   phy2_mode =        mode & 0x00000F00;
   phy3_mode =        mode & 0x0000F000;

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
         /* Save current match/mask, install OC-3 values. */
         WPX_BoardExtCardInit(WPX_MODE_OC3_PMC5351, WPX_SERIAL_CONNECTOR_0, &save);


         xcvr = (wpu_pmc5351 *) WPL_PHY_VIRTUAL(0, APP_UPI1_OC3_PMC5351);
         break;
      }
      case WP_PORT_UPI2:
      {
         /* Save current match/mask, install OC-3 values. */
         WPX_BoardExtCardInit(WPX_MODE_OC3_PMC5351, WPX_SERIAL_CONNECTOR_1, &save);

         xcvr = (wpu_pmc5351 *) WPL_PHY_VIRTUAL(0, APP_UPI2_OC3_PMC5351);
         break;
      }
      default:
         status = WPI_ERROR(WP_ERR_PORT);
         goto clean_return;
   }

   switch (major_mode)
   {
      case WPU_MODE_OC3_ATM:
      {
         /* Master reset */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0xfa);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0x7a);
         WPU_Delay();
         /* Path signal label set */
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].tpop_path_signal_label.reg[0], 0x13);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].sptb_exp_path_signal_label.reg[0], 0x13);
            WPU_Delay();
         }
         /* FIFO reset */
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].rxcp_fifo_cntl.reg[0], 0x11);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txcp_config_1.reg[0], 0x05);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txcp_config_1.reg[0], 0x04);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].rxcp_fifo_cntl.reg[0], 0x10);
            WPU_Delay();
         }
         /* Tx FIFO watermark */
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_high_watermark.reg[0], 0xc0);
            WPU_Delay();
         }
         /* H4INSB bit set */
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txcp_cell_status.reg[0], 0x0c);
            WPU_Delay();
         }
         /* Enable ATM */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_config_2.reg[0], 0x10);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_POS:
      {
         /* Master reset */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0xfa);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0x7a);
         WPU_Delay();
         /* Path signal label set */
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].tpop_path_signal_label.reg[0], 0x16);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].sptb_exp_path_signal_label.reg[0], 0x16);
            WPU_Delay();
         }
         /* FIFO reset */
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].rxfp_config.reg[0], 0x13);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_config.reg[0], 0x13);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].rxfp_config.reg[0], 0x12);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_config.reg[0], 0x12);
            WPU_Delay();
         }
         /* Tx FIFO watermark */
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_control.reg[0], 0x0b);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_low_watermark.reg[0], 0xb0);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_high_watermark.reg[0], 0xb0);
            WPU_Delay();
         }
         /* Enable POS */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_config_2.reg[0], 0x13);
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
         for (ii = 0; ii < 4; ii++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_control.reg[0],       0x04);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].rxfp_rx_high_watemark.reg[0], 0x80);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_low_watermark.reg[0],  0x80);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_high_watermark.reg[0], 0x81);
            WPU_Delay();

            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_control.reg[0], 0x02);
            WPU_Delay();


         };
         break;
      }
      case WPU_64_BYTES:
      {
         for (ii = 0; ii < 4; ii++)
         {
            
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_control.reg[0],       0x04);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].rxfp_rx_high_watemark.reg[0], 0x40);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_low_watermark.reg[0],  0xc0);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_high_watermark.reg[0], 0xc1);
            WPU_Delay();

            WPI_MEM_BYTE_SET(xcvr->phy[ii].txfp_tx_control.reg[0], 0x02);
            WPU_Delay();
         }
         break;
      }
   }

   switch(phy0_mode)
   {
      printf("phy config loopback phy 0 \n");
      
      case WPU_MODE_OC3_PHY0_LOOPBACK_CLOSE:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_cntl.reg[0], 0x08);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY0_LOOPBACK_FAR:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_cntl.reg[0], 0x02);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY0_LOOPBACK_OFF:
         WPI_REG_BYTE_SET(xcvr->phy[0].master_cntl.reg[0], 0x00);
         WPU_Delay();
         break;
      default:
         break;
   }

   switch(phy1_mode)
   {
      printf("phy config loopback phy 1 \n");
      case WPU_MODE_OC3_PHY1_LOOPBACK_CLOSE:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[1].master_cntl.reg[0], 0x08);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY1_LOOPBACK_FAR:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[1].master_cntl.reg[0], 0x02);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY1_LOOPBACK_OFF:
         WPI_REG_BYTE_SET(xcvr->phy[1].master_cntl.reg[0], 0x00);
         WPU_Delay();
         break;
      default:
         break;
   }

   switch(phy2_mode)
   {
      printf("phy config loopback phy 2 \n");
      case WPU_MODE_OC3_PHY2_LOOPBACK_CLOSE:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[2].master_cntl.reg[0], 0x08);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY2_LOOPBACK_FAR:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[2].master_cntl.reg[0], 0x02);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY2_LOOPBACK_OFF:
         WPI_REG_BYTE_SET(xcvr->phy[2].master_cntl.reg[0], 0x00);
         WPU_Delay();
         break;
      default:
         break;
   }

   switch(phy3_mode)
   {
      printf("phy config loopback phy 3 \n");
      case WPU_MODE_OC3_PHY3_LOOPBACK_CLOSE:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[3].master_cntl.reg[0], 0x08);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY3_LOOPBACK_FAR:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[3].master_cntl.reg[0], 0x02);
         WPU_Delay();
         break;
      }
      case WPU_MODE_OC3_PHY3_LOOPBACK_OFF:
         WPI_REG_BYTE_SET(xcvr->phy[3].master_cntl.reg[0], 0x00);
         WPU_Delay();
         break;
      default:
         break;
   }

   status = WP_OK;

  clean_return:

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}


static void WPU_Delay(void)
{
   WPL_Delay(10);
}

WP_status WPU_Oc3Pmc5351Control(WP_U32 port, WP_U32 phy, WP_U32 op)
{
   wpu_pmc5351 *xcvr;
   wpi_match_mask save;
   WP_U32 status;
   WP_U8 control;

  
   /* set init value */
   save.match_address = 0xffffffff;

   /* Check port and get its address */
   switch (port)
   {
      case WP_PORT_UPI1:
         xcvr = (wpu_pmc5351 *) WPL_PHY_VIRTUAL(0, APP_UPI1_OC3_PMC5351);
         break;
      case WP_PORT_UPI2:
         xcvr = (wpu_pmc5351 *) WPL_PHY_VIRTUAL(0, APP_UPI2_OC3_PMC5351);
         break;
      default:
         return WPI_ERROR(WP_ERR_PORT);
   }

   /* For the device addressed, reduce phy mod 4 */
   phy &= 3;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   /* Save current match/mask, install OC-3 values. */
   WPX_BoardExtCardInit(WPX_MODE_OC3_PMC5351, WPX_SERIAL_CONNECTOR_0+port-WP_PORT_UPI1, &save);
   
   WPI_RETURN_IF_ERROR(status, WPX_CpldSet(port, 0));
   WPU_Delay();

   /* Perform the operation, if possible. */
   switch (op) {
      case WPU_OC3_PMC5351_HCS_PASS:
         WPI_REG_BYTE_GET(control, xcvr->phy[phy].rxcp_config_2.reg[0]);
         control += 0x40;
         WPI_MEM_BYTE_SET(xcvr->phy[phy].rxcp_config_2.reg[0], control);
         break;
      case  WPU_OC3_PMC5351_HCS_FILTER:
         WPI_REG_BYTE_GET(control, xcvr->phy[phy].rxcp_config_2.reg[0]);
         control &= ~0x40;
         WPI_MEM_BYTE_SET(xcvr->phy[phy].rxcp_config_2.reg[0], control);
         break;
      case WPU_OC3_PMC5351_PHY_DISABLE:
         control = 0x80;
         WPI_MEM_BYTE_SET(xcvr->phy[phy].channel_reset.reg[0], control);
         break;
      case WPU_OC3_PMC5351_PHY_ENABLE:
         control = 0;
         WPI_MEM_BYTE_SET(xcvr->phy[phy].channel_reset.reg[0], control);
         break;
      case WPU_OC3_PMC5351_PHY_BASE_4:
         /* This is total programming to replace the Init function. */
         /* It sets the PHY base to 4 */
         /* Master reset */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0xfa);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0x7a);
         WPU_Delay();
         for (phy = 0; phy < 4; phy++) {
            /* Path signal label set */
            WPI_MEM_BYTE_SET(xcvr->phy[phy].tpop_path_signal_label.reg[0], 0x13);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].sptb_exp_path_signal_label.reg[0], 0x13);
            WPU_Delay();
            /* FIFO reset */
            WPI_MEM_BYTE_SET(xcvr->phy[phy].rxcp_fifo_cntl.reg[0], 0x11);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txcp_config_1.reg[0], 0x05);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txcp_config_1.reg[0], 0x04);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].rxcp_fifo_cntl.reg[0], 0x10);
            WPU_Delay();
            /* Tx FIFO watermark */
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txfp_tx_high_watermark.reg[0], 0xc0);
            WPU_Delay();
            /* H4INSB bit set */
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txcp_cell_status.reg[0], 0x0c);
            WPU_Delay();
         }
         /* Enable ATM */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_config_2.reg[0], 0x30);
         WPU_Delay();
         break;
      case WPU_OC3_PMC5351_POS_PHY_BASE_4:
         /* Master reset */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0xfa);
         WPU_Delay();
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_reset.reg[0], 0x7a);
         WPU_Delay();
         /* Path signal label set */
         for (phy = 0; phy < 4; phy++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[phy].tpop_path_signal_label.reg[0], 0x16);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].sptb_exp_path_signal_label.reg[0], 0x16);
            WPU_Delay();
         }
         /* FIFO reset */
         for (phy = 0; phy < 4; phy++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[phy].rxfp_config.reg[0], 0x13);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txfp_config.reg[0], 0x13);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].rxfp_config.reg[0], 0x12);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txfp_config.reg[0], 0x12);
            WPU_Delay();
         }
         /* Tx FIFO watermark */
         for (phy = 0; phy < 4; phy++)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txfp_tx_control.reg[0], 0x2b);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txfp_tx_low_watermark.reg[0], 0xb0);
            WPU_Delay();
            WPI_MEM_BYTE_SET(xcvr->phy[phy].txfp_tx_high_watermark.reg[0], 0xb0);
            WPU_Delay();
         }
         /* Enable POS */
         WPI_MEM_BYTE_SET(xcvr->phy[0].master_config_2.reg[0], 0x33);
         WPU_Delay();
         break;

      default:
         status = WPI_ERROR(WP_ERR_PORT);
         goto clean_return;
   }

   status = WP_OK;

  clean_return:

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return WP_OK;
}
