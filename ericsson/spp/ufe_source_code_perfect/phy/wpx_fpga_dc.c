/*****************************************************************************
 * (C) Copyright 2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_fpga_dc.c
 *
 * Purpose: FPGA daughter card (DC) functions
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "wpx_fpga_dc.h"
#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif
#include "veneer/wpi_board_data_int.h"
#include "veneer/wpi_veneer.h"

/*****************************************************************************
 *
 * Function:   WPU_FpgaDaughterCardInit
 *
 * Purpose:    Configure fpga daughter card to work as UTOPIA or POS phy
 *
 * Description:
 *     This function initializes fpga dc registers.
 *     Should be called after WDDI upi ports are enabled and after pinmux
 *     registers are configured
 *
 * Inputs:
 *     port        defines what serial interface used
 *     mode        phy loopback mode
 *     config      User-supplied initialization values
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/
WP_status WPU_FpgaDaughterCardInit(WP_U32 port, WP_U32 mode, WP_port_upi *config)
{
   WP_U32 status = WP_OK;
   wpi_match_mask save;
   wpu_fpga_dc *fpga_dc_regs;
   WP_U16 param, loopback, du;
   WP_U8 utopia_pos_mode, clav_mode, phy_mode, level, pause_support; 
   WP_U8 ms_mode, bus_width, parity;
   WP_U16 du_size;
   WP_U8 utopia_ext_header_size = 0;
   WP_U8 dividing_value = 1;
   
   WPI_WDDI_ATTACH();
 
   memset((WP_U8*)&save, 0, sizeof(wpi_match_mask));

   switch (port)
   {
      case WP_PORT_UPI1:
      {
         /* Save current match/mask, install OC-3 values. */
         WPX_BoardExtCardInit(WPX_MODE_FPGA_DC, WPX_SERIAL_CONNECTOR_0, &save);
         fpga_dc_regs = (wpu_fpga_dc *) WPL_PHY_VIRTUAL(0, APP_UPI1_FPGA_DC);
         break;
      }
      case WP_PORT_UPI2:
      {
         /* Save current match/mask, install OC-3 values. */
         WPX_BoardExtCardInit(WPX_MODE_FPGA_DC, WPX_SERIAL_CONNECTOR_1, &save);
         fpga_dc_regs = (wpu_fpga_dc *) WPL_PHY_VIRTUAL(0, APP_UPI2_FPGA_DC);
         break;
      }
      default:
      {
         status = WPI_ERROR(WP_ERR_PORT);
         /* Restore the registers. */
         WPX_RestoreBank(&save);
         
         /* Relinquish the lock (on the match/mask registers). */
         WPI_WDDI_DETACH();
         return status;
      }
   }
   /*In FPGA daughter card all parameters shoud be equal for RX and TX,
     so tx_characteristics field is used. Suppoused that rx_characteristics
     field has the same values */
   if(config->tx_characteristics & WP_UPI_MULTI)
   {
      phy_mode = WPU_FPGA_DC_MPHY;
   }
   else
   {
      phy_mode = WPU_FPGA_DC_SPHY;
   }
   
   if(config->tx_characteristics & WP_UPI_MULTI_CLAV_ENABLE)
   {
      clav_mode = WPU_FPGA_DC_SCLAV;
   }
   else
   {
      clav_mode = WPU_FPGA_DC_MCLAV;
   }

   if(config->tx_characteristics & WP_UPI_PRT_ODD)
   {
      parity = WPU_FPGA_DC_PARITY_ODD;
   }
   else
   {
      parity = WPU_FPGA_DC_PARITY_EVEN;
   }

   if(config->tx_characteristics & WP_UPI_LEVEL3)
   {
      level = WPU_FPGA_DC_LEVEL3;
   }
   else
   {
      level = WPU_FPGA_DC_LEVEL2;
   }

   if(config->tx_characteristics & WP_UPI_BUS_WIDTH_32)
   {
      bus_width = WPU_FPGA_DC_BUS32;
   }
   else if(config->tx_characteristics & WP_UPI_BUS_WIDTH_16)
   {
      bus_width = WPU_FPGA_DC_BUS16;
   }
   else
   {
      bus_width = WPU_FPGA_DC_BUS8;
   }
   
   if((config->upimode == WP_UPI_UTOPIA) ||
      (config->upimode == WP_UPI_UTOPIA_NOHEC) ||
      (config->upimode == WP_UPI_UTOPIA_IMA) ||
      (config->upimode == WP_UPI_UTOPIA_GBOND) ||
      (config->upimode == WP_UPI_UTOPIA_ATM_IMA) ||
      (config->upimode == WP_UPI_UTOPIA_ATM_PACKET))
   {
      utopia_pos_mode = WPU_FPGA_DC_UTOPIA;
      if(level == WPU_FPGA_DC_LEVEL3)
      {
         pause_support = WPU_FPGA_DC_NO_PAUSE_SUPP;
      }
      else
      {
         pause_support = WPU_FPGA_DC_PAUSE_SUPP;
      }
   }
   else
   {
      utopia_pos_mode = WPU_FPGA_DC_POS;
      pause_support = WPU_FPGA_DC_PAUSE_SUPP;
   }

   /* Master/slave mode should be opposite from
      WDDI UPI port mode */
   if(config->tx_characteristics & WP_UPI_SLAVE)
   {
      ms_mode = WPU_FPGA_DC_MASTER;
   }
   else
   {
      ms_mode = WPU_FPGA_DC_SLAVE;
   }

   param =  
      (WPU_FPGA_DC_INV_MODE << WPU_FPGA_DC_UNV_MODE_OFFSET) |/* 0 always */
      (parity << WPU_FPGA_DC_PARITY_OFFSET) | /* as UPI port*/
      (pause_support << WPU_FPGA_DC_PAUSE_SUPPORT_OFFSET) | /* 1 always*/
      (ms_mode << WPU_FPGA_DC_MASTER_SLAVE_OFFSET) | /*negative from WDDI*/
      (utopia_pos_mode << WPU_FPGA_DC_ATM_POS_OFFSET) | /* as UPI port */
      (bus_width << WPU_FPGA_DC_BUS_WIDTH_OFFSET) | /* as UPI port */
      (level << WPU_FPGA_DC_LEVEL_OFFSET) | /* as UPI port */
      (clav_mode << WPU_FPGA_DC_SCLAV_MCLAV_OFFSET) | /* as UPI port */
      (phy_mode << WPU_FPGA_DC_SPHY_MPHY_OFFSET);/* as UPI port */
   
   WPI_MEM_SHORT_SET(fpga_dc_regs->param, param);
   
   if(mode & WPU_FPGA_DC_MODE_LOOPBACK_OFFSET)
   {
      loopback = WPU_FPGA_DC_CORE_LB;
   }
   else
   {
      loopback = WPU_FPGA_DC_NO_CORE_LB;
   }
   
   WPI_MEM_SHORT_SET(fpga_dc_regs->loopback, loopback);
   
   /*For 32 bit bus data unit size have to be divided by 4
     For 16 bit bus data unit size have to be divided by 2*/
   if(bus_width == WPU_FPGA_DC_BUS32)
   {
      dividing_value = 4;
   }
   else if(bus_width == WPU_FPGA_DC_BUS16)
   {
      dividing_value = 2;
   }
   else
   {
      dividing_value = 1;
   }

   /*For utopia mode, data unit size - cell size and extended header size
     Extended header size shoud be multiplication of 4 when working with 32 bit bus
     and multiplication of 2 when working with 16 bit bus*/
   if(utopia_pos_mode == WPU_FPGA_DC_UTOPIA)
   {
      if(config->atm_ext != NULL)
      {
         /*using tx_atm_exthdrsz, supposed that rx_atm_exthdsz is same*/
         utopia_ext_header_size = config->atm_ext->tx_atm_exthdrsz;
      }
      else
      {
         /* default value */
         utopia_ext_header_size = 0;
      }
      
      du_size = WPU_UTOPIA_CELL_SIZE+utopia_ext_header_size;
      if(config->upimode != WP_UPI_UTOPIA_NOHEC)
      {
         du_size+=dividing_value;
      }
      
   }
   /* POS mode */
   else
   {
      if(config->tx_characteristics & WP_UPI_DATAUNIT_256BYTE)
      {
         du_size = 256;
      }
      else if(config->tx_characteristics & WP_UPI_DATAUNIT_128BYTE)
      {
         du_size = 128;
      }
      else
      {
         du_size = 64;
      }
   }

   du =  du_size/dividing_value;
   
   WPI_MEM_SHORT_SET(fpga_dc_regs->rx_du, du);
   WPI_MEM_SHORT_SET(fpga_dc_regs->tx_du, du);
   
   WPI_MEM_SHORT_SET(fpga_dc_regs->rx_clav_th, WPU_FPGA_DC_RX_CLAV_TH);
   WPI_MEM_SHORT_SET(fpga_dc_regs->tx_clav_th, WPU_FPGA_DC_TX_CLAV_TH);

   WPI_MEM_SHORT_SET(fpga_dc_regs->master_reset, WPU_FPGA_DC_ENABLE);
   
   /* Restore the registers. */
   WPX_RestoreBank(&save);
   
   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();
   return status;
}
