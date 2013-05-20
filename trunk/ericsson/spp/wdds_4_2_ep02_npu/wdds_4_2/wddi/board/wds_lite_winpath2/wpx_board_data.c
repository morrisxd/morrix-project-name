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
 * For:  Board WDS2 Lite
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

#ifndef WPI_HW_H
#include <include/core/hardware/wpi_hw.h>
#endif

#include "wpx_phy_data.h"

static wpi_heap_config_block config_block = {
   WPL_HEAP_SIZE};

#define MAP_Host_BIU_MC_B_1_MSK 0xff803214
#define MAP_Host_BIU_MC_B_1_MTC 0x1d803007

static wpi_match_mask_block match_mask = {
   {
   /* For GENET, the extend_turnoff case is default. */
      {APP_GENET_HOST_IF, (APP_GENET_REGS | 0x00004007), 0xfff0d028},                   /* GENET */
      {APP_GENET_HOST_IF, (APP_GENET_REGS | 0x00004007), 0xfff0d028}                    /* GENET */
   },
   {
      {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xff800000)|0x00004007, 0xff800400},    /* OC3   */
      {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xff800000)|0x00004007, 0xff800400},    /* OC3   */
      {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xff800000)|0x00004007, 0xff800400}     /* OC3   */
   },
   {
      {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xff800000) | 0x00004007, 0xff800400}, /* OC12  */
      {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xff800000) | 0x00004007, 0xff800400}, /* OC12  */
      {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xff800000) | 0x00004007, 0xff800400} /* OC12  */
   },
   {
      {APP_TDM_HOST_IF, (APP_TDM4_QFALC & 0xff800000) | 0x00001007, 0xFF806A20},        /* TDM   */
      {APP_TDM_HOST_IF, (APP_TDM4_QFALC & 0xff800000) | 0x00001007, 0xFF806A20},        /* TDM   */
      {0, 0, 0}                                                                         /* TDM-UFE   */
   },
   {
      {APP_UFE_HOST_IF, (APP_UFE_UPI1 & 0xff800000) | 0x00003007, 0xff807f24},          /* UFE   */
      {APP_UFE_HOST_IF, (APP_UFE_UPI1 & 0xff800000) | 0x00003007, 0xff807f24},          /* UFE   */
      {APP_UFE_HOST_IF, (APP_UFE_UPI1 & 0xff800000) | 0x00003007, 0xff807f24}          /* UFE   */
   }
};

wpi_board_data_block wpi_board_data  = {
   "wds_lite_winpath2",
   &config_block,
   0,
   NULL,
   &match_mask
};

WP_U32 WPX_AppWpIntRegBaseGet(WP_U32 wpid)
{
   return (WP_U32)WPL_RegBaseGet(wpid);
}

WP_U32 WPX_MapGsBoardMuxGet(WP_U32 wpid)
{
   return WPX_AppWpIntRegBaseGet(wpid) |  0x100000;
}

wpi_veneer_memory * WPX_VeneerMemoryGet()
{
   return (void *)NULL;
}
