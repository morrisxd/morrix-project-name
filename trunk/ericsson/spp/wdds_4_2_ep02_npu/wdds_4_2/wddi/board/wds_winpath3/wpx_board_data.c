/*****************************************************************************
 * (C) Copyright 2004-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_board_data.c
 *
 * For:  Board WDS3
 *
 * Purpose: Instantiate board-specific data structure so that WDDI can
 * access it by name rather than by #define.
 * The data struture is defined in sources/veneer/wpi_board_data_int.h
 *
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

#ifndef WPX_APP_DATA_H
#include "wpx_app_data.h"
#endif

#include "wpx_phy_data.h"

#include "wpl_platform.h"

static wpi_heap_config_block config_block = {
   WPL_HEAP_SIZE};

static wpi_match_mask_block match_mask = {
   {
      /* For GENET, the extend_turnoff case is default. */
      {APP_GENET_HOST_IF_1, (APP_GENET_REGS | 0x00004007), 0xfff0d028},                   /* GENET */
      {APP_GENET_HOST_IF_2, (APP_GENET_REGS | 0x00004007), 0xfff0d028}                    /* GENET */
   },
   {
      {APP_UPI_HOST_IF_1, (APP_UPI1_OC3_PMC5351 & 0xfff00000)|0x00001007, 0xfff04f18},    /* OC3   */
      {APP_UPI_HOST_IF_2, (APP_UPI2_OC3_PMC5351 & 0xfff00000)|0x00001007, 0xfff04f18},    /* OC3   */
      {APP_UPI_HOST_IF_3, (APP_UPI3_OC3_PMC5351 & 0xfff00000)|0x00001007, 0xfff04f18}     /* OC3   */
   },
   {
      {APP_UPI_HOST_IF_1, (APP_UPI1_OC12_PMC5357 & 0xfff00000) | 0x00001007, 0xfff04f18}, /* OC12  */
      {APP_UPI_HOST_IF_2, (APP_UPI2_OC12_PMC5357 & 0xfff00000) | 0x00001007, 0xfff04f18}, /* OC12  */
      {APP_UPI_HOST_IF_3, (APP_UPI3_OC12_PMC5357 & 0xfff00000) | 0x00001007, 0xfff04f18}  /* OC12  */
   },
   {
      {APP_TDM_HOST_IF_1, (APP_TDM1_COMET & 0xfff00000) | 0x00001007, 0xfff06a20},        /* TDM   */
      {APP_TDM_HOST_IF_2, (APP_TDM9_COMET & 0xfff00000) | 0x00001007, 0xfff06a20},        /* TDM   */
      {APP_TDM_HOST_IF_3, (APP_TDM1_UFE_COMET & 0xfff00000) | 0x00001007, 0xfff06a20}     /* TDM-UFE(CAD)   */
   },
   {
      {APP_UFE_HOST_IF_1, (APP_UFE_UPI1 & 0xfff00000) | 0x00002007, 0xfff07f24},          /* UFE   */
      {APP_UFE_HOST_IF_2, (APP_UFE_UPI2 & 0xfff00000) | 0x00002007, 0xfff07f24},          /* UFE   */
      {APP_UFE_HOST_IF_3, (APP_UFE_UPI3 & 0xfff00000) | 0x00002007, 0xfff07f24},          /* UFE   */
   },
   {0, 0, 0},                       /* PTP - this phy isn't used with wds_winpath3 board*/
   {
      {APP_UPI_HOST_IF_1, (APP_UPI1_FPGA_DC & 0xfff00000)|0x00002007, 0xfff05f18},    /* FPGA daughter card  */
      {APP_UPI_HOST_IF_2, (APP_UPI1_FPGA_DC & 0xfff00000)|0x00002007, 0xfff05f18},    /* FPGA daughter card  */
      {APP_UPI_HOST_IF_3, (APP_UPI1_FPGA_DC & 0xfff00000)|0x00002007, 0xfff05f18}     /* FPGA daughter card  */
   }
};

wpi_board_data_block wpi_board_data  = {
   "wds_winpath3",
   &config_block,
   0,
   NULL,
   &match_mask
};

/* Physical address of devices that connected by ASRAM bus 
   and selected by chip select:
   CS0 - boot flash - NOR     - offset - 0x800000-0xffffff
   CS1 - storage flash - NAND - offset - 0x0     -0x0fffff
   CS2 - ZBT                  - offset - 0x400000-0x7fffff
   CS3 - FPGA                 - offset - 0x100000-0x1fffff
   CS4 - Serial connector 1   - offset - 0x200000-0x2fffff
   CS5 - Serial connector 2   - offset - 0x400000-0x7fffff
   CS6 - Serial connector 3   - offset - 0x300000-0x3fffff
   CS7 - N/A
   Serial connector 2 offset equal to ZBT offset, 
   so only one of these devices can be used
   */

WP_U32 WPX_AppWpIntRegBaseGet(WP_U32 wpid)
{
   return (WP_U32) WPL_RegBaseGet(wpid);
}

wpi_veneer_memory * WPX_VeneerMemoryGet()
{
   return (void *)NULL;
}

/* workaround to read the winpath3 device id */
WP_U32 WPX_BoardDeviceInfoGet(void)
{
   return 0x00000600; /* bits lsb 9-12 hold the WP3-SL configuration: 4 - WP3-SL, 3-WP3 */
}
