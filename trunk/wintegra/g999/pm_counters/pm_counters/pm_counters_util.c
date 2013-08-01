/*************************************************************
 * (C) Copyright 2001-2011, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*************************************************************
 *  Appplication : Back Pressure base on Flow Control
 *  File Name    : modem_bp_util.c
 *
 ************************************************************/
#ifndef __BP_UTIL_C__
#define __BP_UTIL_C__

//////////////////////////////////////////////////////////////
//                                                          //
// the int ram partition change function - implementation   //
//                                                          //
//////////////////////////////////////////////////////////////

#include "include/core/hardware/wpi_hw_pce.h"
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#include "hardware/winpath3/wpi_hw_fmu.h"
#include "hardware/winpath3/wpi_hw_cfu_int.h"

/******** re-defined here the CFU RIF reference *******/
#define WPI_CFU(base) ((base) + WPI_CFU_BASE)
#define WPI_CFU_RX_OFFSET (0x0100)
#define WPI_CFU_RX_BASE(base) (wpi_hw_cfu_rx*)  \
      (WPI_CFU( base) + WPI_CFU_RX_OFFSET)

#define WT_CFU_RX_BUFFERS_NUM_IN_CFU_BLOCK       4096

#define WPI_REV_ID          WPI_WP_REV_3_B1

/*---------------------- WMM registers -----------------------*/
typedef struct
{
      WPI_PTR fifo_start[48];
      WP_U32 pad_0[16];
      WPI_PTR fifo_end[48];
      WP_U32 pad_1[16];
      WPI_PTR fifo_counter[48];
      WP_U32 pad_2[16];
      WPI_PTR fifo_max[48];
      WP_U32 pad_3[16];
      WPI_PTR iq_start[4];
      WPI_PTR iq_temp[4];
      WPI_PTR iq_end[4];
      WP_U32 lar;
      WP_U32 utar;
      WP_U32 gmr;
      WP_U32 ios;
      WP_U32 fer[2];
      WP_U32 ffr[2];
      WP_U32 hcr_reg;
      WPI_PTR hcr_addr;
      WP_U32 hcr_cmd;
      WP_U32 rwm;
      WP_U32 pad_4[16];
      WP_U32 dbf_8_15_base[8];
      WP_U32 pad_5[8];
      WP_U32 dbf_8_15_offset[8];
      WP_U32 ae_low;
      WP_U32 ae_hi;
} wt_wmm;

/*----------------------- fuse registers --------------------*/
#define WT_HW_WINPATH_DEVICE_ID_S          19
#define WT_HW_WINPATH_DEVICE_ID_W          8

#define WT_HW_WINPATH_REV_ID_S             27
#define WT_HW_WINPATH_REV_ID_W             4

#define WT_HW_WINPATH_SUB_REV_ID_LSB_S     31
#define WT_HW_WINPATH_SUB_REV_ID_LSB_W     1

#define WT_HW_WINPATH_SUB_REV_ID_MSB_S     0
#define WT_HW_WINPATH_SUB_REV_ID_MSB_W     3

typedef struct
{
   WP_U32 reg0;
   WP_U32 reg1;
} wt_fuse_reg;

/*---------------------- int ram partition ------------------*/
#define WT_SIU_INT_REG_RAM_CFG_MIPS_S 0
#define WT_SIU_INT_REG_RAM_CFG_MIPS_W 3

#define WT_SIU_INT_REG_RAM_CFG_PCE_TOP_S 4
#define WT_SIU_INT_REG_RAM_CFG_PCE_TOP_W 4

#define WT_SIU_INT_REG_RAM_CFG_PCE_HASH_S 12
#define WT_SIU_INT_REG_RAM_CFG_PCE_HASH_W 1

#define WT_SIU_INT_REG_RAM_CFG_CFU_TX_S 16
#define WT_SIU_INT_REG_RAM_CFG_CFU_TX_W 3

#define WT_SIU_INT_REG_RAM_CFG_POLICER_S 20
#define WT_SIU_INT_REG_RAM_CFG_POLICER_W 2

#define WT_SIU_INT_REG_RAM_CFG_CFU_RX_S 24
#define WT_SIU_INT_REG_RAM_CFG_CFU_RX_W 2

typedef struct
{
   WP_U32 addr_start;
   WP_U32 addr_size;
   WP_U32 pad[0x7];
   WP_VU32 hbm;
   WP_VU32 tear;
   WP_VU32 tesr;
   WP_VU32 hbms1;
   WP_VU32 hbms2;
   WP_U32  pad1;
   WP_VU32 ram_config;
} wt_siu_internal;


//////////////////////////////////////////////////////////////
//                     int ram ecc initialize               //
//////////////////////////////////////////////////////////////
#define HOST_WRITE_CMD_OP        0xd0100000
#define HOST_READ_CMD_OP         0xc8100000
#define HOST_CMD_WRITE_SIZE      0x400

/***********************************************
 * writeTrs
 ***********************************************/

static WP_U32 writeTrs( WP_U32 start_addr, WP_U32 size,
             WP_U32 hcr_reg, WP_U32 hcr_cmd,
             WP_U32 hcr_addr )
{
   WP_U32 addr, temp32;

   for( addr = start_addr; addr < start_addr+size; addr += HOST_CMD_WRITE_SIZE )
   {
      WPI_REG_SET(*((volatile WP_U32*)hcr_reg), 0);
      WPI_REG_SET(*((volatile WP_U32*)hcr_addr), addr);
      WPI_REG_SET(*((volatile WP_U32*)hcr_cmd), HOST_WRITE_CMD_OP);

      /*loop while host cmd valid bit is set:*/
      WPI_REG_GET(temp32, *((volatile WP_U32*)hcr_cmd));
      while((temp32 & 0x80000000))
         WPI_REG_GET(temp32, *((volatile WP_U32*)hcr_cmd));
   }
   return 0;
}

/***********************************************
 * readTrs
 ***********************************************/

static WP_U32 readTrs( WP_U32 start_addr, WP_U32 size,
             WP_U32 hcr_reg, WP_U32 hcr_cmd,
             WP_U32 hcr_addr )
{
   WP_U32 addr, temp32;

   for( addr = start_addr; addr < start_addr+size; addr += HOST_CMD_WRITE_SIZE )
   {
      WPI_REG_SET(*((volatile WP_U32*)hcr_reg), 0);
      WPI_REG_SET(*((volatile WP_U32*)hcr_addr), addr);
      WPI_REG_SET(*((volatile WP_U32*)hcr_cmd), HOST_READ_CMD_OP);

      /*loop while host cmd valid bit is set:*/
      WPI_REG_GET(temp32, *((volatile WP_U32*)hcr_cmd));
      while((temp32 & 0x80000000))
         WPI_REG_GET(temp32, *((volatile WP_U32*)hcr_cmd));
   }
   return 0;
}

/***********************************************
 * writeTrsOnInternalRam
 ***********************************************/
void WT_ReInitIntRam(WP_U32 wpid, WP_U8* reg_bases_ptr, wt_siu_internal *siu_internal)
{
#if !defined(WINSIM) && !defined(VERILOG)
   WP_U32 bus_base, bus_size, addr, host_offset;
   wpi_veneer_memory bus_mappings[WINPATH_NUM_MAX][WPI_BUS_LAST];
   wt_wmm *wmm;

   wmm = (wt_wmm *)WPI_WMM(reg_bases_ptr, 2);

   WPI_REG_GET(bus_base, siu_internal->addr_start);
   WPI_REG_GET(bus_size, siu_internal->addr_size);

   printf("Re-init INT RAM\n");
   //printf("START %08X SIZE %08X\n", bus_base, bus_size);

   //*(unsigned int*)MAP_SIU3_REG_CONFIG = 0x3 << 29;//enable ECC

   writeTrs( bus_base, HOST_CMD_WRITE_SIZE,
            (WP_U32) &(wmm->hcr_reg), (WP_U32) &(wmm->hcr_cmd), (WP_U32) &(wmm->hcr_addr) );

   WPL_BusesMapGet(&(bus_mappings[0][0]));
   host_offset = (WP_U32) bus_mappings[wpid][WP_BUS_INTERNAL].cached_offset;
   //printf("HOST OFFSET %08X\n", host_offset);

   for(addr = bus_base; addr < (bus_base+0x400); addr += 4)
   {
      WPI_MEM_SET(*((volatile WP_U32*)(addr+host_offset)), 0);
   }

   readTrs( bus_base, HOST_CMD_WRITE_SIZE,
            (WP_U32) &(wmm->hcr_reg), (WP_U32) &(wmm->hcr_cmd), (WP_U32) &(wmm->hcr_addr) );

   writeTrs( bus_base+HOST_CMD_WRITE_SIZE, bus_size-HOST_CMD_WRITE_SIZE,
            (WP_U32) &(wmm->hcr_reg), (WP_U32) &(wmm->hcr_cmd), (WP_U32) &(wmm->hcr_addr) );
#endif
}

/************************************************************************
 * Function:  WT_HwDeviceInfoGet
 *
 * Purpose:   get the winpath revision ID
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value: the winpath revision ID
 *
 * Called by:
 *
 ************************************************************************/
WP_U32 WT_HwDeviceInfoGet(WP_U32 wpid, WP_U8 *reg_bases_ptr)
{
   wt_fuse_reg *fuse_regs;
   WP_U32 device_id, rev_id, sub_rev_id;
   WP_U32 winpath_rev_id;
   WP_U32 reg0, reg1;

   /* set the pce registers address */
   fuse_regs = (wt_fuse_reg *) (reg_bases_ptr + WPI_FUSE_BASE);
   WPI_REG_GET(reg0, fuse_regs->reg0);
   WPI_REG_GET(reg1, fuse_regs->reg1);

   device_id = WPI_NCM_VALUE(WT_HW_WINPATH_DEVICE_ID, reg0);
   rev_id = WPI_NCM_VALUE(WT_HW_WINPATH_REV_ID, reg0);
   sub_rev_id = WPI_NCM_VALUE(WT_HW_WINPATH_SUB_REV_ID_LSB, reg0) +
                (WPI_NCM_VALUE(WT_HW_WINPATH_SUB_REV_ID_MSB, reg1) << 1);

   //printf("device_id %d rev_id %d sub_rev_id %d\n", device_id, rev_id, sub_rev_id);

   winpath_rev_id = WPI_REV_ID;
   switch(device_id)
   {
   case WPI_DEVICE_WP3:
      if(rev_id == WPI_REV_B)
      {
         if(sub_rev_id == 1)
            winpath_rev_id = WPI_WP_REV_3_B1;
         else if(sub_rev_id == 2)
            winpath_rev_id = WPI_WP_REV_3_B2;
      }
      break;
   case WPI_DEVICE_WP3_SL:
      winpath_rev_id = WPI_WP_REV_3_SLB;
      if(rev_id == WPI_REV_B)
      {
         if(sub_rev_id == 0)
            winpath_rev_id = WPI_WP_REV_3_SLB;
         else if(sub_rev_id == 1)
            winpath_rev_id = WPI_WP_REV_3_SLB1;
      }
      break;
   case WPI_DEVICE_WP3_SPO:
      winpath_rev_id = WPI_WP_REV_3_SPO;
      if(rev_id == WPI_REV_B)
      {
         if(sub_rev_id == 1)
            winpath_rev_id = WPI_WP_REV_3_SPO;
      }
      break;
   }

   switch(winpath_rev_id)
   {
   case WPI_WP_REV_3_SPO:
      printf("Chip Revision: WP3 SPO\n");
      break;
   case WPI_WP_REV_3_SLB:
      printf("Chip Revision: WP3 SL B\n");
      break;
   case WPI_WP_REV_3_SLB1:
      printf("Chip Revision: WP3 SL B.1\n");
      break;
   case WPI_WP_REV_3_A:
      printf("Chip Revision: WP3 Rev A\n");
      break;
   case WPI_WP_REV_3_B1:
      printf("Chip Revision: WP3 Rev B.1\n");
      break;
   case WPI_WP_REV_3_B2:
      printf("Chip Revision: WP3 Rev B.2\n");
      break;
   }

   return winpath_rev_id;
}

void WT_ReInitPceHash(WP_U8* reg_bases_ptr)
{
#if !defined(WINSIM) && !defined(VERILOG)
   WP_U32 i;
   wpi_pce_hash_ram_entry pce_hash_ram_entry;
   wpi_pce_regs* pce_regs = WPI_PCE_REG(reg_bases_ptr);

   pce_hash_ram_entry.CRC = 0;
   pce_hash_ram_entry.rule_pointer = WPI_PCE_RULE_INFO_INDEX_NULL;

   printf("Re-init PCE Hash\n");

   WPI_PceGfrEccSet(pce_regs, 0);
   WPI_PceGfrHashInit(pce_regs, 1);

   for(i=0; i<(2*WPI_PCE_HASH_BASIC_SIZE); i++)
   {
      WPI_PceHashRamEntrySet(pce_regs, &pce_hash_ram_entry, i, 0);
   }

   WPI_PceGfrHashInit(pce_regs, 0);
   WPI_PceGfrEccSet(pce_regs, 1);
#endif
}

/********************************************************************************************************************
 *
 * This function changes the internal RAM partition - currently policer and PCE hash and exact match are supported
 * It is assumed that the internal RAM, Policer and PCE are not used previously by application
 *
 * Note: this function should be called before WP_SysInit()
 *
 *******************************************************************************************************************/
void WT_SetIntRamPartition(WP_U32 wpid, void *cfg)
{
   WP_U8 *reg_bases_ptr;
   wt_siu_internal *siu_internal;
   WP_U32 siu_int_reg_ram_cfg;
   WP_U32 winpath_revision;
   WP_U32 pce_hash_prev_size, pce_em_prev_size, policer_prev_size, cfu_rx_prev_size, cfu_tx_prev_size;
   WP_U32 pce_hash_new_size, pce_em_new_size, policer_new_size, cfu_rx_new_size, cfu_tx_new_size;
   WT_int_ram_partition_set *partition = (WT_int_ram_partition_set *)cfg;
   WP_U32 KB = 1024;
   WP_U32 int_ram_total_units;

   printf("------------------ Partition Change: Start ------------------\n");
   printf("Policers: %d PCE HW rules %d\n", partition->policer_entries, partition->pce_hw_rules_num);

   if(partition->cfu_rx_buffers_num != WT_PARTITION_UNCHANGED)
      printf("CFU RX buffers %d\n", partition->cfu_rx_buffers_num);

   if(partition->cfu_tx_buffers_num != WT_PARTITION_UNCHANGED)
      printf("CFU RX buffers %d\n", partition->cfu_tx_buffers_num);

   if(partition->mips_cache_size != WT_PARTITION_UNCHANGED)
   {
      printf("Partition change FAILURE!!!\n Illegal partition parameters:\n"
             " mips_cache_size should remain unchanged\n");
      exit(1);
   }

   /* get the rif offset before driver init */
   reg_bases_ptr = WPL_RegBaseGet(wpid);

   /* get int ram partition register, and its data */
   siu_internal = (wt_siu_internal *)WPI_BIU(reg_bases_ptr, WPI_INTL_BIU_BASE);
   WPI_REG_GET(siu_int_reg_ram_cfg, siu_internal->ram_config);

   /* get the previous size of int ram and pce (they should have ECC init change) */
   pce_hash_prev_size = WPI_NCM_VALUE(WT_SIU_INT_REG_RAM_CFG_PCE_HASH, siu_int_reg_ram_cfg);
   pce_em_prev_size = WPI_NCM_VALUE(WT_SIU_INT_REG_RAM_CFG_PCE_TOP, siu_int_reg_ram_cfg);
   policer_prev_size = WPI_NCM_VALUE(WT_SIU_INT_REG_RAM_CFG_POLICER, siu_int_reg_ram_cfg);
   cfu_rx_prev_size = WPI_NCM_VALUE(WT_SIU_INT_REG_RAM_CFG_CFU_RX, siu_int_reg_ram_cfg);
   cfu_tx_prev_size = WPI_NCM_VALUE(WT_SIU_INT_REG_RAM_CFG_CFU_TX, siu_int_reg_ram_cfg);

   /* get the WP3 revision */
   winpath_revision = WT_HwDeviceInfoGet(wpid, reg_bases_ptr);

   /* check the number of policer entries and pce hw rules number with relation to wp_rev, if problems, return error */
   switch(winpath_revision)
   {
   case WPI_WP_REV_3_A:
   case WPI_WP_REV_3_B1:
   case WPI_WP_REV_3_B2:
      int_ram_total_units = 20;
      if((partition->policer_entries > 16384  && partition->policer_entries  != WT_PARTITION_UNCHANGED) ||
         (partition->pce_hw_rules_num > 32767 && partition->pce_hw_rules_num != WT_PARTITION_UNCHANGED) ||
         (partition->cfu_rx_buffers_num > 4096 && partition->cfu_rx_buffers_num != WT_PARTITION_UNCHANGED) ||
         (partition->cfu_tx_buffers_num > 8192 && partition->cfu_tx_buffers_num != WT_PARTITION_UNCHANGED))
      {
         printf("Partition change FAILURE!!! Illegal partition for this chip");
         exit(1);
      }
      break;
   case WPI_WP_REV_3_SLB:
   case WPI_WP_REV_3_SLB1:
   case WPI_WP_REV_3_SPO:
      int_ram_total_units = 6;
      if((partition->policer_entries > 0 && partition->policer_entries  != WT_PARTITION_UNCHANGED) ||
         (partition->pce_hw_rules_num > 8191 && partition->pce_hw_rules_num != WT_PARTITION_UNCHANGED) ||
         (partition->cfu_rx_buffers_num > 0 && partition->cfu_rx_buffers_num != WT_PARTITION_UNCHANGED) ||
         (partition->cfu_tx_buffers_num > 0 && partition->cfu_tx_buffers_num != WT_PARTITION_UNCHANGED))
      {
         printf("Partition change FAILURE!!! Illegal partition for this chip");
         exit(1);
      }
      break;
   }

   /*
   printf("Register address %08X value %08X\n OLD Used size: L2 CACHE %d POLICER %d PCE HASH %d PCE EM %d\n",
      (WP_U32)(&siu_internal->ram_config), siu_int_reg_ram_cfg,
      WPI_NCM_VALUE(WT_SIU_INT_REG_RAM_CFG_MIPS, siu_int_reg_ram_cfg),
      policer_prev_size, pce_hash_prev_size, pce_em_prev_size);
   */

   /* change the partition of policer and pce */
   pce_em_new_size = pce_hash_new_size = 0;
   if(partition->pce_hw_rules_num == WT_PARTITION_UNCHANGED)
   {
      pce_hash_new_size = pce_hash_prev_size;
      pce_em_new_size = pce_em_prev_size;
   }
   else if(partition->pce_hw_rules_num == 0)
      pce_em_new_size = 0;
   else if(partition->pce_hw_rules_num < 4*KB)
      pce_em_new_size = 1;
   else if(partition->pce_hw_rules_num < 8*KB)
      pce_em_new_size = 2;
   else if(partition->pce_hw_rules_num < 12*KB)
      pce_em_new_size = 3;
   else if(partition->pce_hw_rules_num < 16*KB)
      pce_em_new_size = 4;
   else if(partition->pce_hw_rules_num < 20*KB)
      pce_em_new_size = 5;
   else if(partition->pce_hw_rules_num < 24*KB)
   {
      pce_em_new_size = 6;
      pce_hash_new_size = 1;
   }
   else if(partition->pce_hw_rules_num < 28*KB)
   {
      pce_em_new_size = 7;
      pce_hash_new_size = 1;
   }
   else if(partition->pce_hw_rules_num < 32*KB)
   {
      pce_em_new_size = 8;
      pce_hash_new_size = 1;
   }

   if(partition->policer_entries  == WT_PARTITION_UNCHANGED)
      policer_new_size = policer_prev_size;
   else if(partition->policer_entries == 0)
      policer_new_size = 0;
   else if(partition->policer_entries <= 8192)
      policer_new_size = 1;
   else
      policer_new_size = 2;

   if(partition->cfu_rx_buffers_num == WT_PARTITION_UNCHANGED)
      cfu_rx_new_size = cfu_rx_prev_size;
   else if(partition->cfu_rx_buffers_num == 0)
      cfu_rx_new_size = 0;
   else
   {
      if(partition->cfu_rx_buffers_num <= 2048)
         cfu_rx_new_size = 1;
      else
         cfu_rx_new_size = 2;

      {
         WP_U32 ii;
         WP_U32 init_area_buffers;
         wpi_hw_cfu_rx* rx_hw;

         // re-init the CFU RX buffers to set the correct values of ECC
         rx_hw = WPI_CFU_RX_BASE(reg_bases_ptr);

         init_area_buffers = WT_CFU_RX_BUFFERS_NUM_IN_CFU_BLOCK + partition->cfu_rx_buffers_num;

         WPI_REG_SET(rx_hw->rx_data_fifo_ptr, 0);
         for(ii = 0 ; ii < (init_area_buffers * 16 / 4); ++ii)
         {
	        WPI_REG_SET(rx_hw->rx_data_fifo_data, 0);
         }
      }
   }

   if(partition->cfu_tx_buffers_num == WT_PARTITION_UNCHANGED)
      cfu_tx_new_size = cfu_tx_prev_size;
   else if(partition->cfu_tx_buffers_num == 0)
      cfu_tx_new_size = 0;
   else if(partition->cfu_tx_buffers_num <= 2048)
      cfu_tx_new_size = 1;
   else if(partition->cfu_tx_buffers_num <= 4096)
      cfu_tx_new_size = 2;
   else if(partition->cfu_tx_buffers_num <= 6144)
      cfu_tx_new_size = 3;
   else
      cfu_tx_new_size = 4;

   WPI_NCM_SET_VAL(WT_SIU_INT_REG_RAM_CFG_POLICER, siu_int_reg_ram_cfg, policer_new_size);
   WPI_NCM_SET_VAL(WT_SIU_INT_REG_RAM_CFG_PCE_TOP, siu_int_reg_ram_cfg, pce_em_new_size);
   WPI_NCM_SET_VAL(WT_SIU_INT_REG_RAM_CFG_PCE_HASH, siu_int_reg_ram_cfg, pce_hash_new_size);
   WPI_NCM_SET_VAL(WT_SIU_INT_REG_RAM_CFG_CFU_RX, siu_int_reg_ram_cfg, cfu_rx_new_size);
   WPI_NCM_SET_VAL(WT_SIU_INT_REG_RAM_CFG_CFU_TX, siu_int_reg_ram_cfg, cfu_tx_new_size);

   WPI_REG_SET(siu_internal->ram_config, siu_int_reg_ram_cfg);

   /* check if ECC supported (WP3 RevB) and the size of pce or ecc changed => reinit the ecc */
   if(pce_hash_new_size > pce_hash_prev_size)
   {
      /* reinit PCE hash */
      if(winpath_revision != WPI_WP_REV_3_A)
         WT_ReInitPceHash(reg_bases_ptr);
   }

   //if(policer_new_size != policer_prev_size || pce_em_new_size != pce_em_prev_size ||
   //   cfu_rx_new_size != cfu_rx_prev_size || cfu_rx_new_size != cfu_rx_prev_size)
   {
      /* reinit INT RAM */
      if(winpath_revision == WPI_WP_REV_3_B1 || winpath_revision == WPI_WP_REV_3_B2)
         WT_ReInitIntRam(wpid, reg_bases_ptr, siu_internal);
   }

/*
   printf("Register address %08X value %08X\n NEW Used size: L2 CACHE %d POLICER %d PCE HASH %d PCE EM %d\n",
      (WP_U32)(&siu_internal->ram_config), siu_int_reg_ram_cfg,
      WPI_NCM_VALUE(WT_SIU_INT_REG_RAM_CFG_MIPS, siu_int_reg_ram_cfg),
      policer_new_size, pce_hash_new_size, pce_em_new_size);
*/

   printf("------------------ Partition Change: Finish -----------------\n");

/*
   {
      WP_U32 i;
      wpi_fmu_l2 *hw_fmu_l2;

      hw_fmu_l2     = (wpi_fmu_l2*)(WPI_FMU_BASE_ADDRESS( reg_bases_ptr) + WPI_FMU_L2_OFFSET);
      printf("FMU init GCR, ptr 0x%08X data 0x%08X\n", &(hw_fmu_l2->l1gcr_ptr), &(hw_fmu_l2->l1gcr_data));
      WPI_REG_SET(hw_fmu_l2->l1gcr_ptr, 0);
      for(i=0; i<0x200000; i++)
      {
         WPI_REG_SET(hw_fmu_l2->l1gcr_data, 0);
      }
   }
   printf("LINE %d\n", __LINE__); WT_ReadFmuGcr(0);
*/
}

void WT_ReadFmuGcr(WP_U32 wpid)
{
   WP_U8 *reg_bases_ptr;
   WP_U32 i, temp32;
   wpi_fmu_l2 *hw_fmu_l2;

   /* get the rif offset before driver init */
   reg_bases_ptr = WPL_RegBaseGet(wpid);

   hw_fmu_l2     = (wpi_fmu_l2*)(WPI_FMU_BASE_ADDRESS( reg_bases_ptr) + WPI_FMU_L2_OFFSET);
   printf("FMU read GCR, ptr 0x%08X data 0x%08X\n", (WP_U32) &(hw_fmu_l2->l1gcr_ptr), (WP_U32) &(hw_fmu_l2->l1gcr_data));
   WPI_REG_SET(hw_fmu_l2->l1gcr_ptr, 0);
   for(i=0; i<0x20000; i++)
   {
      if(i%64 == 0 || i > 19450)
         printf("[0x%4X %5d] ", i, i);
      WPI_REG_GET(temp32, hw_fmu_l2->l1gcr_data);
   }
}

WT_int_ram_partition_set int_ram_partition =
{
   /* policer_entries */    0,
   /* pce_hw_rules_num */   4095,
   /* cfu_rx_buffers_num */ WT_PARTITION_UNCHANGED,
   /* cfu_tx_buffers_num */ WT_PARTITION_UNCHANGED,
   /* mips_cache_size */    WT_PARTITION_UNCHANGED,
};


#endif


