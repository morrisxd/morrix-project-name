/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#ifndef WT_PARTITION_ECC_UTIL_H
#define WT_PARTITION_ECC_UTIL_H
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
WP_U32 WT_HwDeviceInfoGet(WP_U32 wpid, WP_U8 *reg_bases_ptr);

extern WT_int_ram_partition_set int_ram_partition;
#endif//WT_PARTITION_ECC_UTIL_H
