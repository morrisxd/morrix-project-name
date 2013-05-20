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

#include "wpx_phy_data.h"

#ifndef WPI_HW_H
#include <include/core/hardware/wpi_hw.h>
#endif

#define WPL_PAGES 1024
#define WPL_PAGE_SIZE 4096
#define WPL_HOST_START_ADDRESS     0x30000 /* 48K words for DPS image */
#ifdef WPL_HOST_SIZE
#undef WPL_HOST_SIZE
#endif
#define WPL_HOST_SIZE              (WPL_PAGES * WPL_PAGE_SIZE - WPL_HOST_START_ADDRESS)
#define WPL_PARAM_START_ADDRESS    0x10000000
#define WPL_PARAM_SIZE             0x00400000
#define WPL_PACKET_START_ADDRESS   0x50000000
#define WPL_PACKET_SIZE            0x00400000
#define WPL_RIF_START_ADDRESS      0x00400000
#define WPL_RIF_SIZE               0x40000
#define WPL_INTERNAL_START_ADDRESS 0x1f000000
#define WPL_INTERNAL_SIZE          0x280000

static wpi_heap_config_block config_block = {
   WPL_HEAP_SIZE};

static wpi_veneer_memory veneer_memory[] = {
   /*
    *  In constructing this table,
    *
    *  the argument to the WPL_PHYS_RANGE_F macro "rangeIndex" is used
    *  to characterize the memory ranges as follows:
    *      0 ==> this physical address range corresponds to something that is
    *            NOT known by WDDI (e.g., a device)
    *      1 - 44 ==> this physical address range corresponds to some memory
    *            known by WDDI.  If the values are equal in 2 or more entries,
    *            then those entries describe the SAME actual physical memory.
    *            Distinct physical memory ranges must have distinct rangeIndex
    *            values.
    *
    *  the host memory, starting at WPI_HOST_START_ADDRESS, is the shared
    *      memory; no other memory falls into this category
    *
    *  In this hardcoded table, we do not include those physical ranges
    *  that would have rangeIndex == 0.
    *
    *  For starters, we will see that only the uncached offset is used,
    *  to suppress cached references.
    */

#define WPL_BOARD_HOST_RANGE     1
#define WPL_BOARD_PARAM_RANGE    2
#define WPL_BOARD_PACKET_RANGE   3
#define WPL_BOARD_INTERNAL_RANGE 4
#define WPL_BOARD_RIF_RANGE      5

   {0, WP_BUS_HOST, 1,
    WPL_SHARED_MEMORY_F(1) + WPL_PHYS_RANGE_F(WPL_BOARD_HOST_RANGE),
    WPL_HOST_START_ADDRESS, WPL_HOST_SIZE, 0, 0},

   {0, WP_BUS_PARAM, 2,
    WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_PARAM_RANGE),
    WPL_PARAM_START_ADDRESS, WPL_PARAM_SIZE, 0, 0},

   {0, WP_BUS_PACKET, 0,
    WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_PACKET_RANGE),
    WPL_PACKET_START_ADDRESS, WPL_PACKET_SIZE, 0, 0},
   /* For the internal bus, specify the start of the internal space
    * and the length of the register + internal memory.
    * The WDDI knows the structure of this space. */
   {0, WP_BUS_INTERNAL, 0,
    WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_INTERNAL_RANGE),
    WPL_INTERNAL_START_ADDRESS, WPL_INTERNAL_SIZE, 0, 0},
   /* RIF definitions */
   {0, WPI_BUS_RIF, 0,
    WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_RIF_RANGE),
    WPL_RIF_START_ADDRESS, WPL_RIF_SIZE, 0, 0}
};

static wpi_match_mask_block match_mask = {
   /* For GENET, the extend_turnoff case is default. */
   {
      {APP_GENET_HOST_IF, (APP_GENET_REGS | 0x00004007), 0xfff0d028},                   /* GENET */
      {APP_GENET_HOST_IF, (APP_GENET_REGS | 0x00004007), 0xfff0d028},                   /* GENET */
   },
   {
      {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xfff00000)|0x00004007, 0xfff00400},    /* OC3   */
      {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xfff00000)|0x00004007, 0xfff00400},    /* OC3   */
      {APP_UPI_HOST_IF, (APP_UPI1_OC3_PMC5351 & 0xfff00000)|0x00004007, 0xfff00400},    /* OC3   */
   },
   {
      {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xfff00000) | 0x00004007, 0xfff00400}, /* OC12  */
      {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xfff00000) | 0x00004007, 0xfff00400}, /* OC12  */
      {APP_UPI_HOST_IF, (APP_UPI1_OC12_PMC5357 & 0xfff00000) | 0x00004007, 0xfff00400}, /* OC12  */
   },
   {
      {APP_TDM_HOST_IF, (APP_TDM4_QFALC & 0xfff00000) | 0x00001007, 0xfff04358},        /* TDM   */
      {APP_TDM_HOST_IF, (APP_TDM4_QFALC & 0xfff00000) | 0x00001007, 0xfff04358},        /* TDM   */
      {0, 0, 0}                                                                         /* TDM-UFE   */
   },
   {
      {APP_UFE_HOST_IF, (APP_UFE_UPI1   & 0xfff00000) | 0x00004007, 0xfff07f20},         /* UFE   */
      {APP_UFE_HOST_IF, (APP_UFE_UPI1   & 0xfff00000) | 0x00004007, 0xfff07f20},         /* UFE   */
      {APP_UFE_HOST_IF, (APP_UFE_UPI1   & 0xfff00000) | 0x00004007, 0xfff07f20}          /* UFE   */
   }
};

wpi_board_data_block wpi_board_data  = {
   "virtual",
   &config_block,
   sizeof(veneer_memory) / sizeof(wpi_veneer_memory),
   veneer_memory,
   &match_mask
};

WP_U32 WPX_AppWpIntRegBaseGet(WP_U32 wpid)
{
   return (WP_U32)WPL_RegBaseGet(wpid);
}

WP_U32 WPX_MapGsBoardMuxGet(WP_U32 wpid)
{
   return 0x0;
}

/* workaround to read the winpath3 device id */
WP_U32 WPX_BoardDeviceInfoGet(void)
{
   return 0x00000600; /* bits lsb 9-12 hold the WP3-SL configuration: 4 - WP3-SL, 3-WP3 */
}
