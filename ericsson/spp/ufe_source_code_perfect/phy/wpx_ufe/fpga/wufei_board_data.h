/*****************************************************************************
 * (C) Copyright 2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_board_data.h
 *
 * Purpose: Define UFE memory space structure
 *
 ****************************************************************************/
#ifndef WUFEI_BOARD_DATA_H
#define WUFEI_BOARD_DATA_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

typedef struct
{
   WP_U32 ufe_fpga_base;
   WP_U32 pmc_tmx_base;
   WP_U32 pmc_arrow_base;
   WP_U32 flexmux_base;
} wufei_mem_space;

typedef struct
{
   WP_U32 n_items;
   wufei_mem_space* ufe_bases;
} wufei_mem_space_block;


#endif
