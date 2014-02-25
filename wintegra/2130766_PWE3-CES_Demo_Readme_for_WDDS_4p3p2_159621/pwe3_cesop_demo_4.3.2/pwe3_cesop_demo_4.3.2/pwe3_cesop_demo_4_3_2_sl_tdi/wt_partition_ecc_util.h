/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

//////////////////////////////////////////////////////////////
//                                                          //
// the int ram partition change function - implementation   //
//                                                          //
//////////////////////////////////////////////////////////////

/* default rev id if not written in chip */
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
