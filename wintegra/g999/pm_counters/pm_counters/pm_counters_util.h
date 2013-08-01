/*************************************************************
 * (C) Copyright 2001-2011, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*************************************************************
 *  Appplication : Back Pressure base on Flow Control
 *  File Name    : modem_bp_util.h
 *
 ************************************************************/
#ifndef __BP_UTIL_H__
#define __BP_UTIL_H__

#define WPI_REV_ID                  WPI_WP_REV_3_B1
#define WT_PARTITION_UNCHANGED      0xFFFFFFFF

typedef struct
{
   WP_U32 policer_entries;
   WP_U32 pce_hw_rules_num;
   WP_U32 cfu_rx_buffers_num;
   WP_U32 cfu_tx_buffers_num;
   WP_U32 mips_cache_size;
} WT_int_ram_partition_set;

void WT_SetIntRamPartition(WP_U32 wpid, void *cfg);
void WT_ReadFmuGcr(WP_U32 wpid);
extern WT_int_ram_partition_set int_ram_partition;

#endif
