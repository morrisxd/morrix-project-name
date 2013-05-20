/*****************************************************************************
 * (C) Copyright 2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_ufe3_data.c
 *
 * For:  Board WDS2 UFE3
 *
 * Purpose: Instantiate UFE3 board-specific data structure so that UFE can
 * access it by name rather than by #define.
 * The data struture is defined in wpx_ufe/fpga/wufei_board_data.h
 *
 ****************************************************************************/
#ifndef WUFEI_BOARD_DATA_H
#include "wufei_board_data.h"
#endif

#ifndef WPX_UFE3_DATA_H
#include "wpx_ufe3_data.h"
#endif

wufei_mem_space ufe_mem_space[APP_UFE_MEM_SPACE_CONNECTORS] = {
   {APP_UFE_UPI1, APP_UFE_BASE_TEMUX0_UPI1, 0, 0},
   {APP_UFE_UPI2, APP_UFE_BASE_TEMUX0_UPI2, 0, 0},
   {APP_UFE_UPI3, APP_UFE_BASE_TEMUX0_UPI3, 0, 0}
};



wufei_mem_space_block ufe_mem_space_block[APP_UFE_MEM_SPACE_CONNECTORS] =
{
   {
      1,
      &ufe_mem_space[0]
   },
   {
      1,
      &ufe_mem_space[1]
   },
   {
      1,
      &ufe_mem_space[2]
   }
};
