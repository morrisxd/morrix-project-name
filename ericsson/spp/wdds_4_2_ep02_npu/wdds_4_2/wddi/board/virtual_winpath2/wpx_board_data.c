/*****************************************************************************
 * (C) Copyright 2004-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_board_data.c
 *
 * For:  Board virtual
 *
 * Purpose: Instantiate board-specific data structure so that WDDI can
 * access it by name rather than by #define.
 * The data struture is defined in sources/veneer/wpi_board_data_int.h
 *
 ****************************************************************************/

#ifndef WPI_BOARD_DATA_INT_H
#include "veneer/wpi_board_data_int.h"
#endif

#ifndef WPI_VENEER_H
#include "veneer/wpi_veneer.h"
#endif

#ifndef WP_BUS_H
#include "api/wp_bus.h"
#endif

#ifndef WPX_HEAP_CONFIG_H
#include "wpx_heap_config.h"
#endif

#ifndef WPX_BOARD_DATA_H
#include "wpx_board_data.h"
#endif

#include "wpx_phy_data.h"

#define WPL_PAGES 2000
#define WPL_PAGE_SIZE 4096
#define WPL_HOST_START_ADDRESS 0x20000

static wpi_heap_config_block config_block = {
   WPL_HEAP_SIZE};

static wpi_match_mask_block match_mask = {
     /* For GENET, the extend_turnoff case is default. */
     {
        {APP_GENET_HOST_IF, (APP_GENET_REGS | 0x00004007), 0xfff0d028},                   /* GENET */
        {APP_GENET_HOST_IF, (APP_GENET_REGS | 0x00004007), 0xfff0d028}                   /* GENET */
     },
     {
        {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xfff00000)|0x00004007, 0xfff00400},    /* OC3   */
        {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xfff00000)|0x00004007, 0xfff00400},    /* OC3   */
        {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xfff00000)|0x00004007, 0xfff00400}    /* OC3   */
     },
     {
        {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xfff00000) | 0x00004007, 0xfff00400}, /* OC12  */
        {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xfff00000) | 0x00004007, 0xfff00400}, /* OC12  */
        {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xfff00000) | 0x00004007, 0xfff00400}  /* OC12  */
     },
     {
        {APP_TDM_HOST_IF, (APP_TDM1_COMET & 0xfff00000) | 0x00001007, 0xFF806A20},        /* TDM   */
        {APP_TDM_HOST_IF, (APP_TDM1_COMET & 0xfff00000) | 0x00001007, 0xFF806A20},        /* TDM   */
        {0, 0, 0}                                                                         /* TDM-UFE   */
     },
     {
        {APP_UFE_HOST_IF, (APP_UFE_UPI1 & 0xfff00000) | 0x00003007, 0xfff07f20},  /* UFE   */
        {APP_UFE_HOST_IF, (APP_UFE_UPI1 & 0xfff00000) | 0x00003007, 0xfff07f20},  /* UFE   */
        {APP_UFE_HOST_IF, (APP_UFE_UPI1 & 0xfff00000) | 0x00003007, 0xfff07f20}   /* UFE   */
     }
};

wpi_board_data_block wpi_board_data  = {
   "virtual",
   &config_block,
   0,
   NULL,
   &match_mask
};

WP_U32 WPX_AppWpIntRegBaseGet(WP_U32 wpid)
{
   return 0x0;
}

WP_U32 WPX_MapGsBoardMuxGet(WP_U32 wpid)
{
   return 0x0;
}

wpi_veneer_memory * WPX_VeneerMemoryGet()
{
   return (void *)NULL;
}
