/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_phy.c
 *
 * Purpose: the UFE EMPHY macro and structures
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif
#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif
#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif
#ifndef WUFEI_MEMORY_ACCESS_INT_H
#include "wufei_memory_access_int.h"
#endif
#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif
#ifndef WUFEI_HW_PHY_H
#include "wufei_hw_phy.h"
#endif
#ifndef WUFEI_HW_TDM_H
#include "wufei_hw_tdm.h"
#endif
#ifndef WUFEI_LINE_H
#include "wufei_line.h"
#endif
#ifndef WUFEI_PHY_H
#include "wufei_phy.h"
#endif
#ifndef WUFEI_PHY_INT_H
#include "wufei_phy_int.h"
#endif
#ifndef WUFEI_HW_SBI_INT_H
#include "wufei_hw_sbi_int.h"
#endif
#ifndef WUFE_INT_H
#include "wufei_int.h"
#endif
#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif
#ifndef WUFEI_HW_CORE_INT_H
#include "wufei_hw_core_int.h"
#endif
#ifndef WUFEI_HW_TDM_INT_H
#include "wufei_hw_tdm_int.h"
#endif
#ifndef WUFEI_HW_MACHINE_INT_H
#include "wufei_hw_machine_int.h"
#endif
#ifndef WUFEI_HW_FM_INT_H
#include "wufei_hw_fm_int.h"
#endif
#ifndef WUFEI_HW_GLOBALS_H
#include "wufei_hw_globals.h"
#endif
#ifndef WUFEI_HW_GLOBALS_INT_H
#include "wufei_hw_globals_int.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#include "include/core/utility/wpi_services.h"

extern WP_U8 debug_flag;


static WUFE_status WUFEI_PhyDisableCheck(WP_U32 wufe_id, WP_U32 core_id,
                                         WUFEI_system_registry *system_registry,
                                         WUFEI_phy_registry *phy_registry);
static WUFE_status WUFEI_PhyReconfigureRxSlots(WP_U32 wufe_id,
                                               WUFEI_system_registry *system_registry,
                                               WUFEI_phy_registry *phy_registry,
                                               WUFE_phy *usr_phy_cfg,
                                               WUFE_phy *reg_phy_cfg,
                                               WUFEI_line_registry *line_registry);
static WUFE_status WUFEI_PhyReconfigureTxSlots(WP_U32 wufe_id,
                                               WUFEI_system_registry *system_registry,
                                               WUFEI_phy_registry *phy_registry,
                                               WUFE_phy *usr_phy_cfg,
                                               WUFE_phy *reg_phy_cfg,
                                               WUFEI_line_registry *line_registry);
static WUFE_status WUFEI_PhyReconfigureHdlc(WP_U32 wufe_id,
                                            WUFEI_phy_registry *phy_registry,
                                            WUFE_phy *usr_phy_cfg,
                                            WUFEI_line_registry *line_registry,
                                            WUFE_phy *reg_phy_cfg);
static WUFE_status WUFEI_PhyReconfigurePos(WP_U32 wufe_id,
                                           WUFEI_phy_registry *phy_registry,
                                           WUFE_phy *usr_phy_cfg,
                                           WUFEI_line_registry *line_registry,
                                           WUFE_phy *reg_phy_cfg);
static WUFE_status WUFEI_PhyReconfigureRxFifo(WUFE_phy *usr_phy_cfg,
                                              WUFE_phy *reg_phy_cfg);
static WUFE_status WUFEI_PhyReconfigureTxFifo(WUFEI_system_registry *system_registry,
                                              WUFE_phy *usr_phy_cfg,
                                              WUFE_phy *reg_phy_cfg);
static WUFE_status WUFEI_PhyReconfigureProtocol(WUFEI_system_registry *system_registry,
                                                WUFEI_phy_registry *phy_registry,
                                                WUFE_phy *usr_phy_cfg,
                                                WUFE_phy *reg_phy_cfg,
                                                WUFEI_line_registry *line_registry);

static WUFE_status WUFEI_PhyTdmSlotsClear(WP_U32 wufe_id,
                                          WP_U32 core_id,
                                          WP_U32 emphy_port_id,
                                          WP_U32 direction,
                                          WUFE_SlotGroup *slot_group,
                                          WUFEI_line_registry *line_registry);

static WUFE_status WUFEI_PhyTdmSlotsDisplay(WP_U32 wufe_id,
                                            WP_U32 core_id,
                                            WP_U32 phy_id,
                                            WP_U32 direction,
                                            WUFE_SlotGroup *slot_group,
                                            WUFEI_line_registry *line_registry);

static WUFE_status WUFEI_PhyTdmSlotsSet(WP_U32 wufe_id,
                                        WP_U32 core_id,
                                        WP_U32 phy_id,
                                        WP_U32 direction,
                                        WUFE_SlotGroup *slot_group,
                                        WUFEI_line_registry *line_registry);

static WUFE_status WUFEI_PhyTdmSlotsCheck(WUFE_SlotGroup *slot_group,
                                          WP_U32 direction,
                                          WUFEI_line_registry *line_registry);

static WUFE_status WUFEI_PhyCfgProtocol(WUFEI_rspt *rspt,
                                        WP_U32      rx_enb,
                                        WUFEI_tspt *tspt,
                                        WP_U32      tx_enb,
                                        WUFE_phy   *ufe_phy_cfg,
                                        WUFEI_system_registry *system_registry,
                                        WUFEI_line_registry *line_registry);

static WUFE_status WUFEI_PhyCfgHdlc(WUFEI_rspt *rspt,
                                    WP_U32      rx_enb,
                                    WUFEI_tspt *tspt,
                                    WP_U32      tx_enb,
                                    WUFE_phy   *ufe_phy_cfg);

static WUFE_status WUFEI_PhyCfgAtm(WUFEI_rspt *rspt,
                                   WP_U32      rx_enb,
                                   WUFEI_tspt *tspt,
                                   WP_U32      tx_enb,
                                   WUFE_phy   *ufe_phy_cfg);

static WUFE_status WUFEI_PhyCfgTrans(WUFEI_system_registry* system_registry,
                                     WUFEI_rspt *rspt,
                                     WP_U32      rx_enb,
                                     WUFEI_tspt *tspt,
                                     WP_U32      tx_enb,
                                     WUFE_phy   *ufe_phy_cfg,
                                     WP_U32      cas);

static WUFE_status WUFEI_AssignPhyId(WP_U16 *phy_id, WP_U16* emphy_port_id,
                                     WUFEI_system_registry* system_registry,
                                     WP_U32 core_id);

static WUFE_status WUFEI_AssignDccPhyId(WP_U16 *phy_id, WP_U16* emphy_port_id,
                                        WUFEI_system_registry* system_registry,
                                        WP_U32 core_id, WP_U32 line_id);

static WUFE_status WUFEI_FreePhyId(WP_U16 phy_id, WP_U16 emphy_port_id,
                                   WUFEI_system_registry* system_registry,
                                   WP_U32 core_id);

static WUFE_status WUFEI_PhyReconfigureRxClkRecMode(WUFEI_phy_registry *phy_registry,
                                                    WUFE_phy *usr_phy_cfg,
                                                    WUFE_phy *reg_phy_cfg,
                                                    WUFEI_line_registry *line_registry);

static WUFE_status WUFEI_PhyCheckPosData(WP_U32      rx_enb,
                                         WP_U32      tx_enb,
                                         WUFE_phy   *ufe_phy_cfg);

WUFE_status  WUFEI_WriteRspt(WP_U32 wufe_id,
                             WP_U32 core_id,
                             WUFEI_rspt rspt,
                             WUFEI_rspt *rspt_ptr)
{
   WP_U32 id = WUFEI_ID(wufe_id, core_id);

   WUFEI_INDIRECT_ADDRESS_SET(id, rspt_ptr);

   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl1);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl2);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl3);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl4);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl5);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl6);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl7);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl8);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.mode);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.cntrl10);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.fifom.res1);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.fifom.res2);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.fifom.cntrl1);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.fifom.write_bd_ptr);
   WUFEI_INDIRECT_DATA_16_SET(id, rspt.fifom.max_buff_buff_count);/* buff count always 0*/
   return WUFE_OK;

}

WUFE_status  WUFEI_ReadRspt(WP_U32 wufe_id,
                            WP_U32 core_id,
                            WUFEI_rspt *rspt,
                            WUFEI_rspt *rspt_ptr)
{
   WP_U32 id = WUFEI_ID(wufe_id, core_id);

   WUFEI_INDIRECT_ADDRESS_SET(id, rspt_ptr);

   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl1));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl2));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl3));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl4));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl5));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl6));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl7));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl8));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.mode));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->type.common.cntrl10));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->fifom.res1));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->fifom.res2));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->fifom.cntrl1));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->fifom.write_bd_ptr));
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->fifom.max_buff_buff_count));
   /* buff count always 0*/
   WUFEI_INDIRECT_DATA_16_GET(id, &(rspt->fifom.read_bd_ptr));
   return WUFE_OK;
}

WUFE_status  WUFE_PrintRspt(WP_U32 wufe_id, WP_U32 core_id, WP_U16 phy_id)
{
   WUFEI_rspt *rspt_ptr;
   WP_U32 i;
   WP_U16 rspt_i_value[16];
   WP_U32 id = WUFEI_ID(wufe_id, core_id);

   rspt_ptr = WUFEI_RX_PHY(phy_id);
   WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.common.cntrl1));

   WUFE_PRINT_DISP((wufei_log, "\nRSPT :0x%x \n", phy_id));
   for(i=0; i<16 ; i++)
   {
      WUFEI_INDIRECT_DATA_16_GET( id, &rspt_i_value[i] );
   }

   for(i=0; i<16 ; i++)
   {
      WUFE_PRINT_DISP((wufei_log, "0x%02x: 0x%04X\n",i, rspt_i_value[i]));
   }
   return WUFE_OK;
}

WUFE_status  WUFEI_WriteTspt(WP_U32 wufe_id,
                             WP_U32 core_id,
                             WUFEI_tspt tspt,
                             WUFEI_tspt *tspt_ptr)
{
   WP_U32 id = WUFEI_ID(wufe_id, core_id);

   WUFEI_INDIRECT_ADDRESS_SET(id, tspt_ptr);

   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl1);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl2);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl3);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl4);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl5);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl6);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl7);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl8);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.mode);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.cntrl10);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.fifom.cntrl1);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.fifom.tx_start_th_max_buff);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.fifom.next_read_bd_ptr);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.fifom.read_bd_ptr);
   WUFEI_INDIRECT_DATA_16_SET(id, tspt.fifom.last_count_buff_count); /* always 0*/
   return WUFE_OK;

}

WUFE_status  WUFEI_ReadTspt(WP_U32 wufe_id,
                            WP_U32 core_id,
                            WUFEI_tspt *tspt,
                            WUFEI_tspt *tspt_ptr)
{
   WP_U32 id = WUFEI_ID(wufe_id, core_id);

   WUFEI_INDIRECT_ADDRESS_SET(id, tspt_ptr);

   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl1));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl2));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl3));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl4));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl5));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl6));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl7));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl8));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.mode));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->type.common.cntrl10));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->fifom.cntrl1));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->fifom.tx_start_th_max_buff));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->fifom.next_read_bd_ptr));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->fifom.read_bd_ptr));
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->fifom.last_count_buff_count)); /* always 0*/
   WUFEI_INDIRECT_DATA_16_GET(id, &(tspt->fifom.write_bd_ptr)); /* always 0*/
   return WUFE_OK;

}

WUFE_status  WUFE_PrintTspt(WP_U32 wufe_id, WP_U32 core_id, WP_U16 phy_id)
{
   WUFEI_tspt *tspt_ptr;

   WP_U32 i;
   WP_U16 tspt_i_value[16];
   WP_U32 id = WUFEI_ID(wufe_id, core_id);

   tspt_ptr=WUFEI_TX_PHY(phy_id);
   WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->type.common.cntrl1));

   WUFE_PRINT_DISP((wufei_log, "\nTSPT :0x%x \n", phy_id));
   for(i=0; i<16 ; i++)
   {

      WUFEI_INDIRECT_DATA_16_GET( id, &tspt_i_value[i] );
   }

   for(i=0; i<16 ; i++)
      WUFE_PRINT_DISP((wufei_log, "0x%02x: 0x%04X\n",i, tspt_i_value[i]));

   return WUFE_OK;
}

WUFE_status  WUFE_PrintSpt(WP_U32 wufe_id, WP_U16 phy_id)
{
   WUFEI_tspt *tspt_ptr;

   WP_U32  i;
   WP_U16  tspt_i_value[16];

   tspt_ptr=WUFEI_TX_PHY(phy_id);
   WUFEI_INDIRECT_ADDRESS_SET(wufe_id, &(tspt_ptr->type.common.cntrl1));

   WUFE_PRINT_DISP((wufei_log, "\nthe TSPT :0x%x \n", phy_id));
   for(i=0; i<16 ; i++)
   {

      WUFEI_INDIRECT_DATA_16_GET( wufe_id, &tspt_i_value[i] );
   }

   for(i=0; i<16 ; i++)
      WUFE_PRINT_DISP((wufei_log, "0x%02x: 0x%04X\n",i, tspt_i_value[i]));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhySystemSetup
 *
 * Purpose:   configure the phys registery and HW.
 *
 *
 * Description:
 *   This function initiats the phy objects, registry and HW.
 *   Registry:
 *   ---------
 *     Malloc the memory needed for the registery objects for all the phys.
 *     1. Clear the obj
 *     2. Set the rx_active and tx_active to WUFE_PHY_EMPTY.
 *     3. Set the line id to WUFEI_LINE_NOT_CONNECTED.
 *   HW:
 *   ---
 *     1. Clear the RX/TX mode.
 *     2. Alloc 1 buffer per phy direction.
 *     3. Clear the counters.
 *
 * Inputs:
 *     system_registry    The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the phys (RSPT amd TSPT)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *    WUFE_ERR_ILLEGAL_NUM_PHYS
 *
 *
 *
 * Called by: WUFE_SystemConfig
 *
 ****************************************************************************/
WUFE_status WUFEI_PhySystemSetup(WUFEI_system_registry* system_registry)
{
   WP_U32 max_n_phys_per_ufe, i, j;
   WUFEI_rspt rspt;
   WUFEI_rspt *rspt_ptr = NULL;
   WUFEI_tspt tspt;
   WUFEI_tspt *tspt_ptr = NULL;
   WP_U32 wufe_id = system_registry->wufe_id;
   WP_U32 tx_first_phy_bd, id;
   WP_U32 coreid;
   WUFE_status status;

   max_n_phys_per_ufe = system_registry->cfg.max_phy_number;

   if ((max_n_phys_per_ufe > WUFE_MAX_N_PHY )|| (max_n_phys_per_ufe == 0))
      return WUFE_ERR_ILLEGAL_NUM_PHYS;

   /**************************************************************
    * REGISTRY
    * ---------
    * Create the driver phy objects
    * 1. malloc phy array
    * 2. clear the phy object
    *    (the rx_active and tx_active are set to WUFE_PHY_EMPTY(=0)
    * 3. set the line id  to NOT connected
    **************************************************************/
   system_registry->phy_arr =
      (WUFEI_phy_registry *)WUFEI_MALLOC(max_n_phys_per_ufe*sizeof(WUFEI_phy_registry));
   if (!(system_registry->phy_arr))
      return (WUFE_ERR_MALLOC);
   memset(system_registry->phy_arr,
          0,
          max_n_phys_per_ufe*sizeof(WUFEI_phy_registry));

   for (i=0;i<max_n_phys_per_ufe;i++)
   {
      system_registry->phy_arr[i].line_id = WUFEI_LINE_NOT_CONNECTED;
   }

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_UfeCorePhyIdBlockCreate(system_registry));

   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      /**************************************************************
       * HW - TSPT/RSPT
       * ---------
       * 1. Reset the Phy mode
       * 2. assign one BD to each RSPT
       * 2. assign one BD to each TSPT
       **************************************************************/
      for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid++)
      {
         memset(&rspt, 0, sizeof(WUFEI_rspt));
         memset(&tspt, 0, sizeof(WUFEI_tspt));

         id = WUFEI_ID(wufe_id, coreid);

         for (i=0; i<system_registry->max_phy_per_core; i++)
         {
            /* set the Assign the BD for each phy. */
            rspt_ptr = WUFEI_RX_PHY(i);

            WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.common.cntrl1));
            for(j=0; j <10; j++)
               WUFEI_INDIRECT_DATA_16_SET(id, 0); /* rspt mode = 0; */

            WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->fifom.res1));
            WUFEI_INDIRECT_DATA_16_SET(id, 0); /* res1 0; */
            WUFEI_INDIRECT_DATA_16_SET(id, 0); /* res2 0; */
            WUFEI_INDIRECT_DATA_16_SET(id, 0); /* cntrl1 0; */
            WUFEI_INDIRECT_DATA_16_SET(id, i); /* rspt write_bd_ptr  =0; */
            WUFEI_INDIRECT_DATA_16_SET(id, 0); /* rspt max_buff buff_count  =0; */
            WUFEI_INDIRECT_DATA_16_SET(id, i); /* rspt read_bd_ptr  =0;*/

#if WUFE_DEBUG_VERIFY_WRITE
            {
               WUFEI_rspt debug_rspt;
               WUFEI_tspt debug_tspt;
               WUFE_status debug_status;

               WUFEI_RETURN_IF_ERROR(
                  debug_status,
                  WUFEI_PhyCheckIdleState(1 /*rx_enb*/,
                                          0/*tx_enb*/,
                                          &debug_rspt,
                                          &debug_tspt,
                                          i,
                                          wufe_id,
                                          coreid,
                                          system_registry,
                                          0/* inconsist_flag*/));

            }
#endif
         }

         tx_first_phy_bd = system_registry->rx_fbp_size[coreid];

         for (i=0; i<system_registry->max_phy_per_core; i++)
         {
            /* set the Assign the BD for each phy. */
            tspt_ptr =WUFEI_TX_PHY(i);

            WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->type.common.cntrl1));

            for(j=0; j <10; j++)
               WUFEI_INDIRECT_DATA_16_SET(id, 0); /* rspt mode = 0; */

            WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->fifom.cntrl1));
            WUFEI_INDIRECT_DATA_16_SET(id, 0); /* tspt cntrl1  ;*/
            WUFEI_INDIRECT_DATA_16_SET(id, 0); /* tx_start_th_max_buff  ;*/
            WUFEI_INDIRECT_DATA_16_SET(id, tx_first_phy_bd+i); /* tspt next_read_bd_ptr  ;*/
            WUFEI_INDIRECT_DATA_16_SET(id, tx_first_phy_bd+i); /* tspt read_bd_ptr   */
            WUFEI_INDIRECT_DATA_16_SET(id, 0);                 /* tspt last/buf counters =0; */
            WUFEI_INDIRECT_DATA_16_SET(id, tx_first_phy_bd+i); /* tspt write_bd_ptr ; */

#if WUFE_DEBUG_VERIFY_WRITE
            {
               WUFEI_rspt debug_rspt;
               WUFEI_tspt debug_tspt;
               WUFE_status debug_status;

               WUFEI_RETURN_IF_ERROR(
                  debug_status,
                  WUFEI_PhyCheckIdleState(0 /*rx_enb*/,
                                          1/*tx_enb*/,
                                          &debug_rspt,
                                          &debug_tspt,
                                          i,
                                          wufe_id,
                                          coreid,
                                          system_registry,
                                          0/* inconsist_flag*/));

            }
#endif
         }
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhySystemDelete
 *
 * Purpose:   release memory allocated for PHYs in the UFE system
 *
 * Description:
 *
 * Inputs:
 *     system_registry    The regestry entry created for this UFE
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by: WUFEI_SystemDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_PhySystemDelete(WUFEI_system_registry* system_registry)
{
   if (system_registry->phy_arr)
   {
      WUFEI_FREE(system_registry->phy_arr);
   }

   return (WUFEI_UfeCorePhyIdBlockDelete(system_registry));
}


static WUFE_status WUFEI_PhyTdmSlotsDisplay(WP_U32 wufe_id,
                                            WP_U32 core_id,
                                            WP_U32 phy_id,
                                            WP_U32 direction,
                                            WUFE_SlotGroup *slot_group,
                                            WUFEI_line_registry *line_registry)
{
   WP_U32 i, tu, tug2, id;
   WUFEI_hw_tdm_entry tdm_entry;
   WUFEI_hw_tdm_entry *tdm_slot_ptr;
   WUFEI_hw_tdm_entry *tdm_ram_slot_ptr = 0;
   WUFE_status status;
   ((void)phy_id); 
   memset(&tdm_entry, 0, sizeof(WUFEI_hw_tdm_entry));

   tu = WUFEI_TU_V(line_registry->sub_spe_entry);
   tug2 = WUFEI_TUG2_V(line_registry->sub_spe_entry);

   id = WUFEI_ID(wufe_id,core_id);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineGetTdmHwPtr(&tdm_ram_slot_ptr,
                                               direction,
                                               line_registry,
                                               slot_group->hmvip_id));

   for (i=0; i<slot_group->n_slots; i++)
   {
      tdm_slot_ptr = &(tdm_ram_slot_ptr[slot_group->slot_arr[i]]);

      debug_flag = 0;

      WUFEI_INDIRECT_ADDRESS_SET(id, tdm_slot_ptr);
      WUFEI_INDIRECT_DATA_16_GET(id, &tdm_entry);

      WUFE_PRINT_DISP((wufei_log,"%2d: 0x%04x\t", slot_group->slot_arr[i], tdm_entry));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
      debug_flag = 0;
   }

   return WUFE_OK;

}

static WUFE_status WUFEI_PhyTdmSlotsSet(WP_U32 wufe_id,
                                        WP_U32 core_id,
                                        WP_U32 phy_id,
                                        WP_U32 direction,
                                        WUFE_SlotGroup *slot_group,
                                        WUFEI_line_registry *line_registry)
{
   WP_U32 i, tu, tug2, id;
   WUFEI_hw_tdm_entry tdm_entry;
   WUFEI_hw_tdm_entry *tdm_slot_ptr;
   WP_U16 min_slot_id = 0xffff;
   WUFEI_hw_tdm_entry *tdm_ram_slot_ptr = 0;
   WUFE_status status;

   tu = WUFEI_TU_V(line_registry->sub_spe_entry);
   tug2 = WUFEI_TUG2_V(line_registry->sub_spe_entry);

   id = WUFEI_ID(wufe_id,core_id);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineGetTdmHwPtr(&tdm_ram_slot_ptr,
                                               direction,
                                               line_registry,
                                               slot_group->hmvip_id));

   for (i=0; i< slot_group->n_slots;i++)
      if( slot_group->slot_arr[i]<min_slot_id)
         min_slot_id=slot_group->slot_arr[i];

   for (i=0; i<slot_group->n_slots; i++)
   {
      tdm_slot_ptr = &(tdm_ram_slot_ptr[slot_group->slot_arr[i]]);
      tdm_entry =
         WUFEI_TDM_ENTRY_ACTIVE_F(WUFE_ENABLE)|
         WUFEI_TDM_ENTRY_PHY_ID_F(phy_id);
      if(min_slot_id == slot_group->slot_arr[i])
         tdm_entry |= WUFEI_TDM_ENTRY_FIRST_F(WUFE_ENABLE);

      WUFEI_INDIRECT_ADDRESS_SET(id, tdm_slot_ptr);
      WUFEI_INDIRECT_DATA_16_SET(id, tdm_entry);

      line_registry->tdm_ram_mng[slot_group->hmvip_id].
         tdm_slots[direction][slot_group->slot_arr[i]] = phy_id;
   }

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFE_PhyTdmSlotsControl
 *
 * Purpose:  Sets/Clears the PHY TDM RAM entry of a specific PHY
 *
 *
 * Description:
 *     This function Sets/ Clears the TDM RAM entry in the specified direction
 *     Used to generate the ATM Sync Indication event
 *
 * Inputs:
 *     ufe_phy_handle - The phy handle
 *     direction - The set/clear direction
 *     enable - WUFE_ENABLE - the phy is set, WUFE_DISABLE - the phy is cleared
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WUFE_status WUFE_PhyTdmSlotsControl(WUFE_handle ufe_phy_handle,
                                    WP_U32 direction,
                                    WP_U8 enable)
{
   WP_U32 wufe_id, core_id, emphy_port_id;
   WP_U16 phy_id;
   WUFE_status status;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_phy_registry   *phy_registry;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR: WUFE_PhyTdmSlotsControl not implemented for UFE4\n"));
      return WUFE_ERR_NOT_IMPLEMENTED;
   }

   /*********************************/
   /* Get registry, system          */
   /*********************************/
   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);

   /* lock the wufe_id and get the system registry */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   /*********************************/
   /* Get registry, phy             */
   /*********************************/
   emphy_port_id =  WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                               system_registry->core1_phyid_offset,
                                               WUFEI_HANDLE_INDEX_V(ufe_phy_handle));

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   if (phy_id >=system_registry->cfg.max_phy_number)
   {/* err phy id */
      WUFE_PRINT_ERR((wufei_log, "ERROR phy_id=%d ", phy_id));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_HANDLE;
   }
   phy_registry =&system_registry->phy_arr[phy_id];

   /*********************************/
   /* Get registry, line            */
   /*********************************/
   if (phy_registry->line_id == WUFEI_LINE_NOT_CONNECTED)
   {/* empry line id*/
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_PHY_NOT_CONNECTED;
   }
   line_registry =&system_registry->line_arr[phy_registry->line_id];

   if(line_registry->state_mode != WUFEI_LINE_ENABLE)
   {
      if (line_registry->state_mode != WUFEI_LINE_DISABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log, "ERROR line_registry->state_mode != WUFEI_LINE_DISABLE "));
         return WUFE_ERR_HANDLE;
      }
      else
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_LINE_NOT_ENABLED;
      }
   }

   if (core_id != line_registry->ufe_core_id)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR core id %d  ",core_id));
      return WUFE_ERR_HANDLE;
   }

   if(enable)
   {
      if(direction & WUFE_TX_DIRECTION)
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTdmSlotsSet( wufe_id,
                                                              core_id,
                                                              phy_id,
                                                              WUFEI_LINE_TDM_TX,
                                                              &(phy_registry->cfg.tx_slot_group),
                                                              line_registry), wufe_id);
      if(direction & WUFE_RX_DIRECTION)
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTdmSlotsSet( wufe_id,
                                                              core_id,
                                                              phy_id,
                                                              WUFEI_LINE_TDM_RX,
                                                              &(phy_registry->cfg.tx_slot_group),
                                                              line_registry), wufe_id);
   }
   else
   {
      if(direction & WUFE_TX_DIRECTION)
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTdmSlotsClear(wufe_id,
                                                               core_id,
                                                               phy_registry->emphy_port_id,
                                                               WUFEI_LINE_TDM_TX,
                                                               &(phy_registry->cfg.tx_slot_group),
                                                               line_registry), wufe_id);
      if(direction & WUFE_RX_DIRECTION)
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTdmSlotsClear(wufe_id,
                                                               core_id,
                                                               phy_registry->emphy_port_id,
                                                               WUFEI_LINE_TDM_RX,
                                                               &(phy_registry->cfg.tx_slot_group),
                                                               line_registry), wufe_id);
   }


   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;

}

static WUFE_status WUFEI_AssignPhyId(WP_U16 *phy_id, WP_U16* emphy_port_id,
                                     WUFEI_system_registry* system_registry,
                                     WP_U32 core_id)
{
   WP_U32 i;
   WUFE_status status;

   *phy_id = WUFEI_LINE_NOT_CONNECTED;
   *emphy_port_id = WUFEI_LINE_NOT_CONNECTED;

   for(i=0; i<system_registry->cfg.max_phy_number ;i++)
   {
      if (system_registry->phy_arr[i].line_id == WUFEI_LINE_NOT_CONNECTED)
      {
         /*internal phy_id pool*/
         *phy_id = i;
         /*internal emphy phy_id pool*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_UfeCorePhyIdBlockSet( system_registry,
                                                           core_id,
                                                           *phy_id,
                                                           emphy_port_id));

         return WUFE_OK;
      }
   }

   return WUFE_ERR_NO_FREE_PHY;

}

static WUFE_status WUFEI_AssignDccPhyId(WP_U16 *phy_id, WP_U16* emphy_port_id,
                                        WUFEI_system_registry* system_registry,
                                        WP_U32 core_id, WP_U32 line_id)
{
   WP_U16* phy_id_pool;
   WP_U32 dcc_phy_num;
   WP_U32 max_lines = 0;
   
   if(!system_registry->phy_id_block ||
      !system_registry->phy_id_block->phy_id_pool[core_id])
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR phy could not be allocated\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }
   
   phy_id_pool = system_registry->phy_id_block->phy_id_pool[core_id];

   /* The phy id assigned to DCC according to line port id */
   if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12)
   {
      max_lines = WUFEI_MAX_OC12_LINES;
   }
   else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
   {
      max_lines = WUFEI_MAX_OC48_LINES;
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR unexpected bandwidth value\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   dcc_phy_num = (line_id - max_lines) + WUFE_MAX_OC48_LINES;

   if (system_registry->phy_arr[dcc_phy_num].line_id != WUFEI_LINE_NOT_CONNECTED ||
       phy_id_pool[dcc_phy_num] != WUFEI_PHY_NOT_CONNECTED)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR phy for this DCC channel already taken\n"));
      return WUFE_ERR_NO_FREE_PHY;
   }

   /*In DCC the emphy_port_id and phy_id are the same*/
   *phy_id = dcc_phy_num;
   *emphy_port_id = dcc_phy_num;
   phy_id_pool[dcc_phy_num] = dcc_phy_num;
   
   return WUFE_OK;
}

static WUFE_status WUFEI_FreePhyId(WP_U16 phy_id, WP_U16 emphy_port_id,
                                   WUFEI_system_registry* system_registry,
                                   WP_U32 core_id)
{
   WUFE_status status;

   if(phy_id >= system_registry->cfg.max_phy_number)
      return WUFE_ERR_ILLEGAL_PHY_ID;

   memset(&(system_registry->phy_arr[phy_id]),0, sizeof(WUFEI_phy_registry));

   system_registry->phy_arr[phy_id].line_id = WUFEI_LINE_NOT_CONNECTED;

   /*internal emphy phy_id pool*/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_UfeCorePhyIdBlockPut( system_registry,
                                                     core_id,
                                                     emphy_port_id));

   return WUFE_OK;
}

WUFE_status WUFEI_PhySystemTdmSlotsSetup(WUFEI_system_registry *system_registry,
                                        WP_U32 core_id)
{
   WP_U32 i, wufe_id;
   WUFEI_hw_tdm_entry *tdm_slot_ptr;

   wufe_id = system_registry->wufe_id;

   tdm_slot_ptr = WUFEI_TX_TDM(0);
   WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), tdm_slot_ptr);
   for(i=0; i<0xc00/*3K*/; i++)
      WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), 0);

   tdm_slot_ptr = WUFEI_TX_TDM(0x1000);
   WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), tdm_slot_ptr);
   for(i=0; i<0xc00/*3K*/; i++)
      WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), 0);

   tdm_slot_ptr = WUFEI_RX_TDM(0);
   WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), tdm_slot_ptr);
   for(i=0; i<0xc00/*3K*/; i++)
      WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), 0);

   tdm_slot_ptr = WUFEI_RX_TDM(0x1000);
   WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), tdm_slot_ptr);
   for(i=0; i<0xc00/*3K*/; i++)
      WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), 0);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyTdmSlotsClear
 *
 * Purpose:   remove the configuration of the DS0 slots from the phy in the line.
 *
 *
 * Description:
 *     This function checks if all the slot group to be used by the phy is all vacant.
 *     The line config set all the values to be WUFEI_NOT_VALID_SLOT during WUFEI_CadConfig.
 *
 *     For framed line the each slot holds either the phy_id or WUFEI_NOT_VALID_SLOT
 *     For clear channel the only slot used is the slot #0 (holds phy_id or WUFEI_NOT_VALID_SLOT).
 *
 * Inputs:
 *     slot_group    - The slots to be used by the phy.
 *     line_registry - The regestry entry created for the UFE line
 *
 * Outputs:
 *     non
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_SLOT_CONFIG
 *
 * Called by:
 *     WUFE_PhyDelete
 *
 ****************************************************************************/
static WUFE_status WUFEI_PhyTdmSlotsClear(WP_U32 wufe_id,
                                          WP_U32 core_id,
                                          WP_U32 emphy_port_id,
                                          WP_U32 direction,
                                          WUFE_SlotGroup *slot_group,
                                          WUFEI_line_registry *line_registry)
{
   WP_U32 hmvip_id, i;
   WUFEI_hw_tdm_entry *tdm_ram_slot_ptr=0;
   WUFEI_hw_tdm_entry *tdm_slot_ptr;
   WUFE_status status;

   hmvip_id=slot_group->hmvip_id;

   if (hmvip_id >= line_registry->hmvip_num)
   {
      /*for HMVIP =[0..3] else must be 0*/
      WUFE_PRINT_ERR((wufei_log, "Illegal HMVIP id in the slot config\n"));
      return WUFE_ERR_SLOT_CONFIG;
   }

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineGetTdmHwPtr(&tdm_ram_slot_ptr,
                                               direction,
                                               line_registry,
                                               hmvip_id));


   if (line_registry->framed != WUFE_UNFRAMED)
   {
      /************/
      /*  framed  */
      /************/
      if (slot_group->n_slots==0 || slot_group->n_slots > WUFE_N_SLOTS_E1)
      {
         WUFE_PRINT_ERR(
            ( wufei_log,
              "line framed | n_slots=%d | slot_group->n_slotsd(%d)> WUFE_N_SLOTS_E1(%d)\n",
              slot_group->n_slots,
              slot_group->n_slots,
              WUFE_N_SLOTS_E1));
         return WUFE_ERR_SLOT_CONFIG;
      }

      for (i=0;i<slot_group->n_slots;i++)
      {
         WP_U32 tmp_slot_id=slot_group->slot_arr[i];

         /* Check the slots is in the valid range  */
         if(tmp_slot_id >= WUFE_N_SLOTS_E1)
         {
            WUFE_PRINT_ERR((wufei_log, "slot id too high\n"));
            return WUFE_ERR_SLOT_CONFIG;
         }

         /* Check if HMVIP T1 every 4 slot should not be used */
         if(line_registry->transfer_type == WUFE_CAD_TYPE_T1)
         {  /* Check the slots is in the valid range  */
            if(tmp_slot_id >= WUFE_N_SLOTS_T1)
            {
               WUFE_PRINT_ERR((wufei_log, "FRAMED:slot_id >= WUFE_N_SLOTS_T1\n"));
               return WUFE_ERR_SLOT_CONFIG;
            }
         }
         else
         {
            /* Check if HMVIP T1 every 4 slot shoult not be used  */
            if(line_registry->transfer_type == WUFE_CAD_TYPE_HMVIP_4_T1)
            {
               if(tmp_slot_id%4 ==0)
               {
                  WUFE_PRINT_ERR((wufei_log, "HMVIP*4 T1 can not use every 4th slot\n"));
                  return WUFE_ERR_SLOT_CONFIG;
               }

            }
         }

         /* Check the Slot is already in use*/
         if (line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][tmp_slot_id] != emphy_port_id)
         {
            WUFE_PRINT_ERR(
               (wufei_log,
                "invalid slot cfg Frame line_reg->tdm_ram_mng[%d(hmvip)][%d(dir)][%d(slot_id)]=%d"
                , hmvip_id,
                direction,
                tmp_slot_id,
                line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][tmp_slot_id]));
            return WUFE_ERR_SLOT_CONFIG;
         }
         else
         {
            /*change the internal registry */
            line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][tmp_slot_id] =
               WUFEI_NOT_VALID_SLOT;

            /* clear the entry in the TDM RAM. */
            tdm_slot_ptr = &(tdm_ram_slot_ptr[tmp_slot_id]);
            WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), tdm_slot_ptr);
            WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), 0);

         }

      }
   }
   else
   {
      /****************************/
      /* unframed - clear channel */
      /****************************/
      /* When working with clear-channel the slot #0 is
         the representetive of the phy in the tdm_ram_mng. */
      if (line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][0] != emphy_port_id)
      {
         WUFE_PRINT_ERR(
            (wufei_log,
             "invalid slot cfg Unframed line_reg->tdm_ram_mng[%d(hmvip)][%d(direction)][0]=%d",
             hmvip_id, direction, line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][0]));
         return WUFE_ERR_SLOT_CONFIG;
      }
      else
      {
         line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][0] = WUFEI_NOT_VALID_SLOT;
         /* clear the entry in the TDM RAM. */
         tdm_slot_ptr = &(tdm_ram_slot_ptr[0]);
         WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), tdm_slot_ptr);
         WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), 0);
      }
   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyTdmSlotsCheck
 *
 * Purpose:   Checks the configuration of the DS0 slots of the phy in the line.
 *
 *
 * Description:
 *     This function checks if all the slot group to be used by the phy is all vacant.
 *     The line config set all the values to be WUFEI_NOT_VALID_SLOT during WUFEI_CadConfig.
 *
 *     For framed line the each slot holds either the phy_id or WUFEI_NOT_VALID_SLOT
 *     For clear channel the only slot used is the slot #0 (holds phy_id or WUFEI_NOT_VALID_SLOT).
 *
 * Inputs:
 *     slot_group    - The slots to be used by the phy.
 *     line_registry - The regestry entry created for the UFE line
 *
 * Outputs:
 *     non
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_SLOT_CONFIG
 *
 * Called by:
 *     WUFE_PhyCreate
 *
 ****************************************************************************/
static WUFE_status WUFEI_PhyTdmSlotsCheck(WUFE_SlotGroup *slot_group,
                                          WP_U32 direction,
                                          WUFEI_line_registry *line_registry)
{
   WP_U32 hmvip_id, i;

   if (slot_group ==NULL || slot_group->slot_arr == NULL)
   {
      WUFE_PRINT_ERR((wufei_log, "Slot group or slot array not defined\n"));
      return WUFE_ERR_SLOT_CONFIG;
   }

   hmvip_id=slot_group->hmvip_id;
   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {

      if (hmvip_id >= line_registry->hmvip_num)
      {

         /*for HMVIP =[0..3] else must be 0*/
         WUFE_PRINT_ERR((wufei_log, "Illegal HMVIP id in the slot config\n"));
         WUFE_PRINT_ERR((wufei_log, "(hmvip_id %d>= line_registry->hmvip_num %d\n",
                         hmvip_id,
                         line_registry->hmvip_num));
         return WUFE_ERR_SLOT_CONFIG;
      }
   }

   if(line_registry->framed != WUFE_UNFRAMED)
   {
      /************/
      /*  framed  */
      /************/
      if (slot_group->n_slots==0 || slot_group->n_slots > WUFE_N_SLOTS_E1)
      {

         WUFE_PRINT_ERR(
            ( wufei_log,
              "framed | n_slots=%d | slot_group->n_slotsd(%d)> WUFE_N_SLOTS_E1(%d)\n",
              slot_group->n_slots,
              slot_group->n_slots,
              WUFE_N_SLOTS_E1));
         return WUFE_ERR_SLOT_CONFIG;
      }

      for (i=0; i< slot_group->n_slots;i++)
      {

         WP_U32 tmp_slot_id=slot_group->slot_arr[i];

         if (tmp_slot_id >= WUFE_N_SLOTS_E1)
         {
            WUFE_PRINT_ERR(
               ( wufei_log,
                 "line is framed or slots=%d IS GREATER THAN 31\n",
                 tmp_slot_id));

         }

         if(line_registry->transfer_type == WUFE_CAD_TYPE_T1 ||
            line_registry->transfer_type == WUFE_SBI_TYPE_T1 ||
            line_registry->transfer_type == WUFE_SDH_TYPE_T1 ||
            line_registry->transfer_type == WUFE_SONET_TYPE_T1)
         {  /* Check the slots is in the valid range  */
            if(tmp_slot_id >= WUFE_N_SLOTS_T1 )
            {
               WUFE_PRINT_ERR((wufei_log, "FRAMED:slot_id >= WUFE_N_SLOTS_T1\n"));
               return WUFE_ERR_SLOT_CONFIG;
            }
         }
         else
         {
            /* Check if HMVIP T1 every 4 slot shoult not be used  */
            if(line_registry->transfer_type == WUFE_CAD_TYPE_HMVIP_4_T1)
            {
               if(tmp_slot_id%4 ==0)
               {
                  WUFE_PRINT_ERR((wufei_log, "HMVIP*4 T1 can not use every 4th slot\n"));
                  return WUFE_ERR_SLOT_CONFIG;
               }
            }
            else
            {  /* Check the slots is in the valid range  */
               if(tmp_slot_id >= WUFE_N_SLOTS_E1)
               {
                  WUFE_PRINT_ERR(
                     (wufei_log,
                      "slot id too high (%d) the max is %d\n",
                      tmp_slot_id, WUFE_N_SLOTS_E1));
                  return WUFE_ERR_SLOT_CONFIG;
               }
            }
         }
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /* Check the Slot is already in use*/
            if (line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][tmp_slot_id] !=
                WUFEI_NOT_VALID_SLOT)
            {
               WUFE_PRINT_ERR(
                  (wufei_log,
                   "Framed slot occupied line_reg->tdm_ram_mng[%d(hmvip)][%d(dir)][%d(slot_id)]=%d\n"
                   , hmvip_id,
                   direction,
                   tmp_slot_id,
                   line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][tmp_slot_id]));
               WUFE_PRINT_ERR(
                  (wufei_log,
                   "Framed slot is occupied with phy %d\n",
                   line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][tmp_slot_id]));
               return WUFE_ERR_SLOT_CONFIG;
            }
         }

      }
   }
   else /* Unframed */
   {
      slot_group->n_slots = 1;
      slot_group->slot_arr[0] = 0;

      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         if (line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][0] != WUFEI_NOT_VALID_SLOT)
         {
            WUFE_PRINT_ERR(
               (wufei_log,
                "Unframed slot occupied line->tdm_ram_mng[%d(hmvip)][%d(direction)][0]=%d\n",
                hmvip_id,
                direction,
                line_registry->tdm_ram_mng[hmvip_id].tdm_slots[direction][0]));
            return WUFE_ERR_SLOT_CONFIG;
         }
      }

   }
   return WUFE_OK;
}
/*****************************************************************************
 *
 * Function:  WUFEI_PhyCheckFifoRx
 *
 * Purpose:     check the configuration of the FIFO Rx
 * Description: as above.
 * Inputs:  WUFE_RxFifo      rx_fifo_param
 * Outputs: none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     else WUFE_ERR_INVALID_FIFO_CFG.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyCheckFifoRx(WUFE_RxFifo rx_fifo_param)
{
   /***********************/
   /* check RX FIFO cofig */
   /***********************/
   if ((rx_fifo_param.fifo_max_size>254)||
       (rx_fifo_param.priority_table>3))
   {
      WUFE_PRINT_ERR((wufei_log, "WUFEI_PhyCheckFifoRx\n"));

      return WUFE_ERR_INVALID_FIFO_CFG;
   }
   return WUFE_OK;
}
/*****************************************************************************
 *
 * Function:  WUFEI_PhyCheckFifoTx
 *
 * Purpose:     check the configuration of the FIFO Tx
 * Description: as above.
 * Inputs:  WUFE_TxFifo      tx_fifo_param
 * Outputs: none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     else WUFE_ERR_INVALID_FIFO_CFG.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyCheckFifoTx(WUFE_TxFifo tx_fifo_param)
{
   /***********************/
   /* check TX FIFO cofig */
   /***********************/
   if ((tx_fifo_param.fifo_max_size>254)||
       (tx_fifo_param.priority_table>3) ||
       (tx_fifo_param.fast_mode>1)      ||
       (tx_fifo_param.transmit_th > 254)||
       (tx_fifo_param.wait_type > 3)    ||
       (tx_fifo_param.transmit_th >= tx_fifo_param.fifo_max_size))
   {
      WUFE_PRINT_ERR((wufei_log, "tx_fifo_param.transmit_th %d\n",tx_fifo_param.transmit_th));
      WUFE_PRINT_ERR((wufei_log, "tx_fifo_param.fifo_max_size  %d\n",
                      tx_fifo_param.fifo_max_size));
      WUFE_PRINT_ERR((wufei_log, "WUFEI_PhyCheckFifoTx\n"));
      return WUFE_ERR_INVALID_FIFO_CFG;
   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyTxFbpCounter
 *
 * Purpose:     maintain the Tx max size for all the phys.
 *
 * Description:
 *            the sum of all the UFE phys max size should be less than the
 *            number of available buffers in the FBP
 *            This function maintan the counter of total number of BD in use.
 *
 * Inputs:  WUFE_TxFifo      tx_fifo_param
 * Outputs: none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     else WUFE_ERR_INVALID_FIFO_CFG.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyTxFbpCounter(WUFEI_system_registry* system_registry,
                                  WP_U8 add_max_size,
                                  WP_U8 sub_max_size)
{

   if ((WP_S32)(system_registry->phy_assign_cnt - sub_max_size) < 0)
      return WUFE_ERR_PHY_ASSIGNED_ERR;

   system_registry->phy_assign_cnt -= sub_max_size;

   if((WP_S32)(system_registry->phy_assign_cnt + add_max_size) >
      (system_registry->cfg.fifo_manager.tx_fbp_size -
       system_registry->cfg.max_phy_number          ))

      return WUFE_ERR_PHY_SUM_MAX_COUNT_TOO_BIG;

   system_registry->phy_assign_cnt += add_max_size;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyFifoCfg
 *
 * Purpose:   configures the RSPT and the TSPT part of the Fifo manager
 *
 * Description:
 *     This function
 *
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyFifoCfg(WP_U32                 rx_enb,
                             WP_U32                 tx_enb,
                             WUFEI_rspt            *rspt,
                             WUFEI_tspt            *tspt,
                             WUFE_phy              *ufe_phy_cfg,
                             WUFEI_system_registry *system_registry)
{
   WUFE_status status;
   ((void)system_registry);
   if (rx_enb)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyCheckFifoRx(ufe_phy_cfg->rx_fifo_param));

      /* config the FIFO Rx parameters */
      rspt->fifom.max_buff_buff_count =
         WUFEI_PHY_FIFOM_RX_MAX_BUFF_F(ufe_phy_cfg->rx_fifo_param.fifo_max_size);
      rspt->fifom.cntrl1 = WUFEI_PHY_FIFOM_RX_PRI_F(ufe_phy_cfg->rx_fifo_param.priority_table);

   }/*rx_enb*/
   if (tx_enb)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_PhyCheckFifoTx(ufe_phy_cfg->tx_fifo_param));

      /* config the FIFO Tx parameters */
      tspt->fifom.cntrl1 =
         WUFEI_PHY_FIFOM_TX_FAST_F(ufe_phy_cfg->tx_fifo_param.fast_mode)|
         WUFEI_PHY_FIFOM_TX_PRI_F(ufe_phy_cfg->tx_fifo_param.priority_table)|
         WUFEI_PHY_FIFOM_TX_WAIT_IN_F(ufe_phy_cfg->tx_fifo_param.wait_type)|
         WUFEI_PHY_FIFOM_TX_WAIT_COUNT_F(0);

      tspt->fifom.tx_start_th_max_buff =
         WUFEI_PHY_FIFOM_TX_MAX_BUFF_F(ufe_phy_cfg->tx_fifo_param.fifo_max_size)|
         WUFEI_PHY_FIFOM_TX_BUFF_TH_F (ufe_phy_cfg->tx_fifo_param.transmit_th);

      /* The last counter, Buffer counter and Wait counter are set to 0 */
   }/*tx_enb*/

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyCheckIdleState
 *
 * Purpose:   1) Check the PHY structures RSPT and TSPT
 *            2) configures the RSPT and  TSPT pointers according to the HW
 *
 * Description:
 *     This function
 *
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyCheckIdleState(WP_U32 rx_enb,
                                    WP_U32 tx_enb,
                                    WUFEI_rspt *rspt,
                                    WUFEI_tspt *tspt,
                                    WP_U32 phy_id,
                                    WP_U32 wufe_id,
                                    WP_U32 core_id,
                                    WUFEI_system_registry *system_registry,
                                    WP_U32 inconsist_flag)
{
   WUFEI_tspt  tmp_tspt;
   WUFEI_tspt *tspt_ptr;
   WUFEI_rspt  tmp_rspt;
   WUFEI_rspt *rspt_ptr;
   WUFE_status status;
   WP_U32 id = WUFEI_ID(wufe_id,core_id);

   memset(&tmp_rspt, 0, sizeof(WUFEI_rspt));
   memset(&tmp_tspt, 0, sizeof(WUFEI_tspt));

   if (tx_enb)
   {
      tspt_ptr=WUFEI_TX_PHY(phy_id);
      memset(&tmp_tspt, 0xff, sizeof(WUFEI_tspt));

      /* read TSPT mode check if the Enable bit is set*/
      WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->type.common.mode));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_tspt.type.common.mode));
      if (WUFEI_PHY_ATM_TX_MODE_ENABLE_V(tmp_tspt.type.common.mode))
      {
         WUFE_PRINT_DBG(
            (wufei_log,
             "tspt %d:..mode=0x%08X    WUFEI_PHY_ATM_TX_MODE_ENABLE_V(tmp_tspt...mode)=0x%08X\n",
             phy_id,
             tmp_tspt.type.common.mode,
             WUFEI_PHY_ATM_TX_MODE_ENABLE_V(tmp_tspt.type.common.mode)));

         return WUFE_ERR_PHY_ACTIVE;
      }

      /* read TSPT mode check FIFO Manager state */
      WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->fifom.cntrl1));

      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_tspt.fifom.cntrl1));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_tspt.fifom.tx_start_th_max_buff));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_tspt.fifom.next_read_bd_ptr));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_tspt.fifom.read_bd_ptr));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_tspt.fifom.last_count_buff_count));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_tspt.fifom.write_bd_ptr));

      /* check the read write pointers nwrite and counters*/
      if (WUFEI_PHY_FIFOM_TX_LAST_COUNT_V(tmp_tspt.fifom.last_count_buff_count) != 0 ||
          WUFEI_PHY_FIFOM_TX_BUFF_COUNT_V(tmp_tspt.fifom.last_count_buff_count) != 0 ||
          tmp_tspt.fifom.next_read_bd_ptr != tmp_tspt.fifom.write_bd_ptr)
      {
         if(WUFEI_PHY_FIFOM_TX_BUFF_COUNT_V(tmp_tspt.fifom.last_count_buff_count) ==0)
         {
            if(inconsist_flag == 0)
            {
               WUFEI_RETURN_IF_ERROR(
                  status, WUFEI_PhyCheckIdleState( 0 /*rx_enb*/,
                                                   tx_enb,
                                                   rspt,
                                                   tspt,
                                                   phy_id,
                                                   wufe_id,
                                                   core_id,
                                                   system_registry,
                                                   1/* inconsist_flag*/));
            }
            else
            {
               WUFE_PRINT_ERR((wufei_log,
                               "phy_id %d last_cnt_bff_cnt 0x%x!=0 \ntx_s_th_max_bff 0x%x!=0\n",
                               phy_id,
                               tmp_tspt.fifom.last_count_buff_count,
                               tmp_tspt.fifom.tx_start_th_max_buff));
               WUFE_PRINT_ERR((wufei_log,
                               "next_read_bd_ptr f 0x%x!=  write_bd_ptrf 0x%x\n",
                               tmp_tspt.fifom.next_read_bd_ptr,
                               tmp_tspt.fifom.write_bd_ptr));

               return WUFE_ERR_FIFO_INCONSIST_FBP_PTR;
            }

         }
         else
         {
            return WUFE_ERR_PHY_FIFO_NOT_EMPTY;
         }

      }
      if (WUFEI_PHY_FIFOM_TX_VALID_BD_V(tmp_tspt.fifom.cntrl1) !=0)
      {

         if(inconsist_flag == 0)
         {

            WUFEI_RETURN_IF_ERROR(
               status, WUFEI_PhyCheckIdleState( 0 /*rx_enb*/,
                                                tx_enb,
                                                rspt,
                                                tspt,
                                                phy_id,
                                                wufe_id,
                                                core_id,
                                                system_registry,
                                                1/* inconsist_flag*/));
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log,
                            "invlid state - valid buff in disable state 0x%x\n",
                            tmp_tspt.fifom.cntrl1 ));
            return WUFE_ERR_FIFO_INCONSIST_FBP_PTR;
         }

      }

      tspt->fifom.next_read_bd_ptr= tmp_tspt.fifom.next_read_bd_ptr;
      tspt->fifom.read_bd_ptr     = tmp_tspt.fifom.next_read_bd_ptr;
      tspt->fifom.write_bd_ptr    = tmp_tspt.fifom.next_read_bd_ptr;

      /* ignore WUFEI_PHY_FIFOM_TX_WAIT_COUNT_V*/

   }

   if (rx_enb)
   {
      rspt_ptr=WUFEI_RX_PHY(phy_id);

      /* read RSPT mode check if the Enable bit is set*/
      WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.common.mode));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_rspt.type.common.mode));

      if (WUFEI_PHY_ATM_TX_MODE_ENABLE_V(tmp_rspt.type.common.mode))
      {
         WUFE_PRINT_DBG((wufei_log,
                         "phy_id %d tmp_rspt.type.common.mode=0x%08X  \n",phy_id,
                         tmp_rspt.type.common.mode));
         WUFE_PRINT_DBG((wufei_log,
                         "WUFEI_PHY_ATM_RX_MODE_ENABLE_V(tmp_rspt.type.common.mode)=0x%08X\n",
                         WUFEI_PHY_ATM_RX_MODE_ENABLE_V(tmp_rspt.type.common.mode)));

         return WUFE_ERR_PHY_ACTIVE;
      }

      /* read RSPT */
      WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->fifom.write_bd_ptr));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_rspt.fifom.write_bd_ptr));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_rspt.fifom.max_buff_buff_count));
      WUFEI_INDIRECT_DATA_16_GET(id, &(tmp_rspt.fifom.read_bd_ptr));

      /* check the read write pointers and counters*/
      if (tmp_rspt.fifom.write_bd_ptr!=tmp_rspt.fifom.read_bd_ptr              ||
          WUFEI_PHY_FIFOM_RX_BUFF_COUNT_V(tmp_rspt.fifom.max_buff_buff_count)!= 0)
      {
         if(WUFEI_PHY_FIFOM_RX_BUFF_COUNT_V(tmp_rspt.fifom.max_buff_buff_count) ==0)
         {
            if(inconsist_flag == 0)
            {
               WUFEI_RETURN_IF_ERROR(
                  status, WUFEI_PhyCheckIdleState( rx_enb,
                                                   0/*tx_enb*/,
                                                   rspt,
                                                   tspt,
                                                   phy_id,
                                                   wufe_id,
                                                   core_id,
                                                   system_registry,
                                                   1/* inconsist_flag*/));
            }
            else
            {
               WUFE_PRINT_ERR((wufei_log,
                   "tmp_rspt.fifom.write_bd_ptr 0x%x  tmp_rspt.fifom.read_bd_ptr 0x%x \n",
                   tmp_rspt.fifom.write_bd_ptr,
                   tmp_rspt.fifom.read_bd_ptr));

               WUFE_PRINT_ERR((wufei_log,
                   "tmp_rspt.fifom.max_buff_buff_count 0x%x\n",
                   tmp_rspt.fifom.max_buff_buff_count));

               WUFE_PRINT_ERR((wufei_log,
                   "PHY_FIFOM_RX_BUFF_COUNT_V(phy_id %d t_rspt.fifom.max_buff_buff_count) 0x%x\n",
                   phy_id,
                   WUFEI_PHY_FIFOM_RX_BUFF_COUNT_V(tmp_rspt.fifom.max_buff_buff_count)));

               return WUFE_ERR_FIFO_INCONSIST_FBP_PTR;
            }
         }
         else
            return WUFE_ERR_PHY_FIFO_NOT_EMPTY;
      }
      rspt->fifom.write_bd_ptr = tmp_rspt.fifom.write_bd_ptr;
      rspt->fifom.read_bd_ptr  = tmp_rspt.fifom.write_bd_ptr;
   }
   return WUFE_OK;
}

static WUFE_status WUFEI_PhyCfgProtocol(WUFEI_rspt *rspt,
                                        WP_U32      rx_enb,
                                        WUFEI_tspt *tspt,
                                        WP_U32      tx_enb,
                                        WUFE_phy   *ufe_phy_cfg,
                                        WUFEI_system_registry *system_registry,
                                        WUFEI_line_registry *line_registry)
{
   WUFE_status status;

  /*******************/
   /* check Protocol  */
   /*******************/
   switch(ufe_phy_cfg->struct_id)
   {/* Check and config the PROTOCOL information */
      case WUFE_STRUCT_PHY_ATM:
         if (WUFEI_SystemHwConfigAtmMachineValid(system_registry,
                                                 line_registry->ufe_core_id) == WUFE_FALSE)
            return WUFE_ERR_HW_CONFIG_STATE_MACHINE;

         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            if (ufe_phy_cfg->type.atm.rx_cell_delineation != WUFE_PHY_ATM_RX_CELL_DELINEATION_NIBBLE_RESOLUTION &&
                (line_registry->transfer_type==WUFE_SDH_TYPE_DS3 || line_registry->transfer_type==WUFE_SONET_TYPE_DS3))
               return WUFE_ERR_PHY_CONFIG;
         }
         
         WUFEI_RETURN_IF_ERROR( status,
                                WUFEI_PhyCfgAtm(rspt,
                                                rx_enb,
                                                tspt,
                                                tx_enb,
                                                ufe_phy_cfg));

         break;

      case WUFE_STRUCT_PHY_HDLC:
         if (WUFEI_SystemHwConfigHdlcMachineValid(system_registry,
                                                  line_registry->ufe_core_id) == WUFE_FALSE)
           return WUFE_ERR_HW_CONFIG_STATE_MACHINE;

         WUFEI_RETURN_IF_ERROR( status,
                                WUFEI_PhyCfgHdlc(rspt,
                                                 rx_enb,
                                                 tspt,
                                                 tx_enb,
                                                 ufe_phy_cfg));

         break;

      case WUFE_STRUCT_PHY_POS:
         if (WUFEI_SystemHwConfigPosMachineValid(system_registry,
                                                  line_registry->ufe_core_id) == WUFE_FALSE)
           return WUFE_ERR_HW_CONFIG_STATE_MACHINE;

         WUFEI_RETURN_IF_ERROR( status,
                                WUFEI_PhyCheckPosData(rx_enb,
                                                      tx_enb,
                                                      ufe_phy_cfg));

         break;

      case WUFE_STRUCT_PHY_TRANS:
         if (WUFEI_SystemHwConfigTransMachineValid(system_registry,
                                                   line_registry->ufe_core_id) == WUFE_FALSE)
            return WUFE_ERR_HW_CONFIG_STATE_MACHINE;

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_PhyCfgTrans(system_registry,
                                                 rspt,
                                                 rx_enb,
                                                 tspt,
                                                 tx_enb,
                                                 ufe_phy_cfg,
                                                 line_registry->cas_enable));

         break;
      default:
         return WUFE_ERR_PHY_INVALID_TYPE;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyCfgHdlc
 *
 * Purpose:   configures the RSPT and TSPT in case the Phy Type is HDLC
 *             *
 * Description:
 *     This function checks for invalid configyrations regarding the HDLC machine
 *     and configurtes them in the RSPT and TSPT.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_PhyCfgHdlc(WUFEI_rspt *rspt,
                                    WP_U32      rx_enb,
                                    WUFEI_tspt *tspt,
                                    WP_U32      tx_enb,
                                    WUFE_phy   *ufe_phy_cfg)
{
   if (rx_enb)
   {
      if ((ufe_phy_cfg->type.hdlc.rx_frame_include_crc_mode>1)||
          (ufe_phy_cfg->type.hdlc.rx_frame_crc_size>1))
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx_frame_include_crc_mode (%d) > 1\n",
                         ufe_phy_cfg->type.hdlc.rx_frame_include_crc_mode));
         WUFE_PRINT_ERR((wufei_log, "\trx_frame_crc_size (%d) > 1\n",
                         ufe_phy_cfg->type.hdlc.rx_frame_crc_size));

         return WUFE_ERR_PHY_CONFIG;
      }

      rspt->type.hdlc.mode =
         WUFEI_PHY_RX_MODE_TYPE_F(WUFE_PHY_MODE_HDLC)|
         WUFEI_PHY_HDLC_RX_MODE_IN_CRC_F(ufe_phy_cfg->type.hdlc.rx_frame_include_crc_mode)|
         WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_F(ufe_phy_cfg->type.hdlc.rx_frame_crc_size);

      rspt->type.hdlc.cntrl1=WUFEI_PHY_HDLC_RX_INIT_CNTRL1;
      rspt->type.hdlc.crc1=0xffff;
      rspt->type.hdlc.crc2=0xffff;

   }
   if (tx_enb)
   {
      WP_U8 init_val;

      if ((ufe_phy_cfg->type.hdlc.tx_frame_include_crc_mode>1)||
          (ufe_phy_cfg->type.hdlc.tx_frame_crc_size>1)||
          (ufe_phy_cfg->type.hdlc.tx_no_flag_between_frames>1)||
          (ufe_phy_cfg->type.hdlc.tx_min_flag_num>16)         ||
          (ufe_phy_cfg->type.hdlc.tx_min_flag_num<1)          ||
          (ufe_phy_cfg->type.hdlc.tx_underrun_interrupt_mode>1))
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx_frame_include_crc_mode (%d) > 1\n",
                        ufe_phy_cfg->type.hdlc.tx_frame_include_crc_mode ));
         WUFE_PRINT_ERR((wufei_log, "\ttx_frame_crc_size (%d) > 1\n",
                        ufe_phy_cfg->type.hdlc.tx_frame_crc_size));
         WUFE_PRINT_ERR((wufei_log, "\ttx_no_flag_between_frames (%d) > 1\n",
                         ufe_phy_cfg->type.hdlc.tx_no_flag_between_frames));
         WUFE_PRINT_ERR((wufei_log, "\ttx_min_flag_num (%d) > 16\n",
                        ufe_phy_cfg->type.hdlc.tx_min_flag_num));
         WUFE_PRINT_ERR((wufei_log, "\ttx_min_flag_num (%d) < 1\n",
                         ufe_phy_cfg->type.hdlc.tx_min_flag_num));
         WUFE_PRINT_ERR((wufei_log, "\ttx_underrun_interrupt_mode (%d) > 1\n",
                         ufe_phy_cfg->type.hdlc.tx_underrun_interrupt_mode ));

         return WUFE_ERR_PHY_CONFIG;
      }

      /*
        if ufe_phy_cfg->type.hdlc.tx_no_flag_between_frames ==1
        0x7e --> 0xff
      */
      if( ufe_phy_cfg->type.hdlc.tx_no_flag_between_frames == 0)
         init_val = 0x7e;
      else
         init_val = 0xff;

      tspt->type.hdlc.cntrl1 = init_val <<8; /*0x7e00*/
      tspt->type.hdlc.cntrl2 = 0x0000;
      tspt->type.hdlc.cntrl3 = 0x0010;
      tspt->type.hdlc.cntrl4 = init_val ;    /*0x007e;*/
      tspt->type.hdlc.crc1   = 0xffff;
      tspt->type.hdlc.crc2   = 0xffff;
      tspt->type.hdlc.data1  = init_val <<8; /*0x7e00*/
      tspt->type.hdlc.data2  = 0x0000;

      tspt->type.hdlc.mode =
         WUFEI_PHY_TX_MODE_TYPE_F(WUFE_PHY_MODE_HDLC)|
         WUFEI_PHY_HDLC_TX_MODE_IN_CRC_F(ufe_phy_cfg->type.hdlc.tx_frame_include_crc_mode)|
         WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_F(ufe_phy_cfg->type.hdlc.tx_frame_crc_size)|
         WUFEI_PHY_HDLC_TX_MODE_NFBF_F(ufe_phy_cfg->type.hdlc.tx_no_flag_between_frames)|
         WUFEI_PHY_HDLC_TX_MINUMFL_F(ufe_phy_cfg->type.hdlc.tx_min_flag_num-1)|
         WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_F(ufe_phy_cfg->type.hdlc.tx_underrun_interrupt_mode);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyCheckPosData
 *
 * Purpose:   Verifies that user parameters are correct
 *             *
 * Description:
 *     This function checks for invalid configyrations regarding the HDLC machine
 *     and configurtes them in the RSPT and TSPT.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_PhyCheckPosData(WP_U32      rx_enb,
                                         WP_U32      tx_enb,
                                         WUFE_phy   *ufe_phy_cfg)
{
   if (rx_enb)
   {
      if ((ufe_phy_cfg->type.pos.rx_descrambler_mode>1)||
          (ufe_phy_cfg->type.pos.rx_frame_crc_size>1))
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\rx_descrambler_mode (%d) > 1\n",
                         ufe_phy_cfg->type.pos.rx_descrambler_mode));
         WUFE_PRINT_ERR((wufei_log, "\trx_frame_crc_size (%d) > 1\n",
                         ufe_phy_cfg->type.pos.rx_frame_crc_size));

         return WUFE_ERR_PHY_CONFIG;
      }
   }
   if (tx_enb)
   {
      if ((ufe_phy_cfg->type.pos.tx_frame_crc_size>1)||
          (ufe_phy_cfg->type.pos.tx_scrambler_mode>1)||
          (ufe_phy_cfg->type.pos.tx_min_flag_num>16)         ||
          (ufe_phy_cfg->type.pos.tx_min_flag_num<1))
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\tx_frame_crc_size (%d) > 1\n",
                        ufe_phy_cfg->type.pos.tx_frame_crc_size ));
         WUFE_PRINT_ERR((wufei_log, "\tx_scrambler_mode (%d) > 1\n",
                        ufe_phy_cfg->type.pos.tx_scrambler_mode));
         WUFE_PRINT_ERR((wufei_log, "\ttx_min_flag_num (%d) > 16\n",
                        ufe_phy_cfg->type.hdlc.tx_min_flag_num));
         WUFE_PRINT_ERR((wufei_log, "\ttx_min_flag_num (%d) < 1\n",
                         ufe_phy_cfg->type.hdlc.tx_min_flag_num));

         return WUFE_ERR_PHY_CONFIG;
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyCfgTrans
 *
 * Purpose:   configures the RSPT and TSPT in case the Phy Type is Transparent
 *             *
 * Description:
 *     This function checks for invalid configurations regarding the transparent machine
 *     and configurtes them in the RSPT and TSPT.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_PhyCfgTrans(WUFEI_system_registry* system_registry,
                                     WUFEI_rspt *rspt,
                                     WP_U32      rx_enb,
                                     WUFEI_tspt *tspt,
                                     WP_U32      tx_enb,
                                     WUFE_phy   *ufe_phy_cfg,
                                     WP_U32      cas)
{
   WP_U16 max_data_u_size;

   if (rx_enb)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         if (ufe_phy_cfg->type.trans.rx_data_unit_size < 4)
         {
            WUFE_PRINT_ERR((wufei_log,
                            "ufe_phy_cfg->type.trans.rx_data_unit_size %d < 4 \n",
                            ufe_phy_cfg->type.trans.rx_data_unit_size));
            return WUFE_ERR_PHY_CONFIG;
         }

         if (system_registry->hw_config.max_buf_size == WUFE_MAX_BUF_SIZE_64)
            max_data_u_size = WUFEI_MAX_BUF_SIZE_64;
         else if (system_registry->hw_config.max_buf_size == WUFE_MAX_BUF_SIZE_128)
            max_data_u_size = WUFEI_MAX_BUF_SIZE_128;
         else if (system_registry->hw_config.max_buf_size == WUFE_MAX_BUF_SIZE_256)
            max_data_u_size = WUFEI_MAX_BUF_SIZE_256;
         else
         {
            WUFE_PRINT_ERR((wufei_log,
                            "Error in system_registry - Check that the UFE4 device is properly programmed!!!\n"));
            return WUFE_ERR_INVALID_REGISTRY_STATE;
         }
      }
      else
      {
         if (ufe_phy_cfg->type.trans.rx_data_unit_size < 2)
         {
            WUFE_PRINT_ERR((wufei_log,
                            "ufe_phy_cfg->type.trans.rx_data_unit_size %d < 2 \n",
                            ufe_phy_cfg->type.trans.rx_data_unit_size));
            return WUFE_ERR_PHY_CONFIG;
         }
         max_data_u_size =WUFEI_MAX_BUF_SIZE_64;
      }

      if (cas == WUFE_CAS_DISABLE)
      {
         if (ufe_phy_cfg->type.trans.rx_data_unit_size > max_data_u_size)
         {
            WUFE_PRINT_ERR((wufei_log,
                            "ufe_phy_cfg->type.trans.rx_data_unit_size %d > %d \n",
                            ufe_phy_cfg->type.trans.rx_data_unit_size, max_data_u_size));

            return WUFE_ERR_PHY_CONFIG;
         }
      }
      else
      {
         if (ufe_phy_cfg->type.trans.rx_data_unit_size < ufe_phy_cfg->rx_slot_group.n_slots )
         {
            WUFE_PRINT_ERR((wufei_log,
                "phy_cfg->type.trans.rx_data_unit_size %d < phy_cfg->rx_slot_group.n_slots %d\n",
                ufe_phy_cfg->type.trans.rx_data_unit_size , ufe_phy_cfg->rx_slot_group.n_slots));
            return WUFE_ERR_PHY_CONFIG;
         }

         if((ufe_phy_cfg->type.trans.rx_data_unit_size +
             (ufe_phy_cfg->rx_slot_group.n_slots+1)/2 ) > max_data_u_size)
         {
            WUFE_PRINT_ERR((wufei_log,
                            "rx_data_unit_size %d +(rx_slot_group.n_slots %d +1)/2 ) > %d\n",
                            ufe_phy_cfg->type.trans.rx_data_unit_size ,
                            ufe_phy_cfg->rx_slot_group.n_slots,max_data_u_size));

            return WUFE_ERR_PHY_CONFIG;
         }
      }

      if(ufe_phy_cfg->type.trans.rx_data_unit_size % ufe_phy_cfg->rx_slot_group.n_slots != 0 )
      {
         WUFE_PRINT_ERR((wufei_log,
                         "rx_data_unit_size (%d) mod rx_slot_group.n_slots (%d) !=0 \n",
                         ufe_phy_cfg->type.trans.rx_data_unit_size,
                         ufe_phy_cfg->rx_slot_group.n_slots));

         return WUFE_ERR_PHY_CONFIG;
      }

      if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         rspt->type.trans.cntrl4 = 0x00c0;

         rspt->type.trans.mode =
            WUFEI_PHY_RX_MODE_TYPE_F(WUFE_PHY_MODE_TRANS)|
            WUFEI_PHY_TRANS_RX_MODE_NSPP_F(ufe_phy_cfg->rx_slot_group.n_slots -1)|
            WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_F(ufe_phy_cfg->type.trans.rx_data_unit_size -1);
      }
   }
   if (tx_enb)
   {
      if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         if(ufe_phy_cfg->type.trans.tx_underrun_interrupt_mode>1)
         {
            WUFE_PRINT_ERR((wufei_log, "WUFE Phy Trans TX configuration is invalid:\n"));
            WUFE_PRINT_ERR((wufei_log, "\ttx_underrun_interrupt_mode (%d) > 1\n",
                            ufe_phy_cfg->type.trans.tx_underrun_interrupt_mode ));

            return WUFE_ERR_PHY_CONFIG;
         }

         tspt->type.trans.mode =
            WUFEI_PHY_TX_MODE_TYPE_F(WUFE_PHY_MODE_TRANS)|
            WUFEI_PHY_TRANS_TX_MODE_NSPP_F(ufe_phy_cfg->tx_slot_group.n_slots -1) |
            WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_F
            (ufe_phy_cfg->type.trans.tx_underrun_interrupt_mode);
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyCfgAtm
 *
 * Purpose:   configures the RSPT and in case the Phy Type is ATM
 *             *
 * Description:
 *     This function
 *
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_PhyCfgAtm(WUFEI_rspt *rspt,
                                   WP_U32      rx_enb,
                                   WUFEI_tspt *tspt,
                                   WP_U32      tx_enb,
                                   WUFE_phy   *ufe_phy_cfg)
{
   if (rx_enb)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         if (ufe_phy_cfg->type.atm.rx_cell_delineation != WUFE_PHY_ATM_RX_CELL_DELINEATION_NIBBLE_RESOLUTION &&
             ufe_phy_cfg->type.atm.rx_cell_delineation != WUFE_PHY_ATM_RX_CELL_DELINEATION_BYTE_RESOLUTION)
            WUFE_PRINT_ERR((wufei_log, "\trx_cell_delineation (%d) can only be byte or nibble resolution in UFE4\n",
                            ufe_phy_cfg->type.atm.rx_cell_delineation));
      }
      else
      {
         if (ufe_phy_cfg->type.atm.rx_cell_delineation>3)
            WUFE_PRINT_ERR((wufei_log, "\trx_cell_delineation (%d) > 3\n",
                            ufe_phy_cfg->type.atm.rx_cell_delineation));
      }

      /* RX */
      if ((ufe_phy_cfg->type.atm.rx_descrambler_mode>1)||
          (ufe_phy_cfg->type.atm.rx_coset_mode>1) ||
          (ufe_phy_cfg->type.atm.rx_idle_cell_discard>1) ||
          (ufe_phy_cfg->type.atm.rx_unassigned_cell_discard>1) ||
          (ufe_phy_cfg->type.atm.rx_hec_error_discard>1) ||
          (ufe_phy_cfg->type.atm.rx_hec_correction_mode>1) ||
          (ufe_phy_cfg->type.atm.rx_alpha>16) ||
          (ufe_phy_cfg->type.atm.rx_alpha<1) ||
          (ufe_phy_cfg->type.atm.rx_delta>16) ||
          (ufe_phy_cfg->type.atm.rx_delta==0))
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx_descrambler_mode (%d) > 1\n",
                         ufe_phy_cfg->type.atm.rx_descrambler_mode));
         WUFE_PRINT_ERR((wufei_log, "\trx_coset_mode (%d) > 1\n",
                         ufe_phy_cfg->type.atm.rx_coset_mode));
         WUFE_PRINT_ERR((wufei_log, "\trx_idle_cell_discard (%d) > 1\n",
                         ufe_phy_cfg->type.atm.rx_idle_cell_discard));
         WUFE_PRINT_ERR((wufei_log, "\trx_unassigned_cell_discard (%d) > 1\n",
                         ufe_phy_cfg->type.atm.rx_unassigned_cell_discard));
         WUFE_PRINT_ERR((wufei_log, "\trx_hec_error_discard (%d) > 1\n",
                         ufe_phy_cfg->type.atm.rx_hec_error_discard));
         WUFE_PRINT_ERR((wufei_log, "\trx_hec_correction_mode (%d) > 1\n",
                         ufe_phy_cfg->type.atm.rx_hec_correction_mode));
         WUFE_PRINT_ERR((wufei_log, "\trx_alpha (%d) > 16 || == 0\n",
                         ufe_phy_cfg->type.atm.rx_alpha));
         WUFE_PRINT_ERR((wufei_log, "\trx_delta (%d) > 16 || == 0\n",
                         ufe_phy_cfg->type.atm.rx_delta));

         return WUFE_ERR_PHY_CONFIG;
      }
      else if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         rspt->type.atm.mode =
            WUFEI_PHY_RX_MODE_TYPE_F(WUFE_PHY_MODE_ATM)|
            WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_F(ufe_phy_cfg->type.atm.rx_descrambler_mode)|
            WUFEI_PHY_ATM_RX_MODE_COSET_F(ufe_phy_cfg->type.atm.rx_coset_mode)|
            WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_F(ufe_phy_cfg->type.atm.rx_idle_cell_discard)|
            WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_F(
               ufe_phy_cfg->type.atm.rx_unassigned_cell_discard)|
            WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_F(ufe_phy_cfg->type.atm.rx_hec_error_discard)|
            WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_F(ufe_phy_cfg->type.atm.rx_hec_correction_mode)|
            WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_F(ufe_phy_cfg->type.atm.rx_cell_delineation);

         rspt->type.atm.alfa_delta =
            WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_F(ufe_phy_cfg->type.atm.rx_delta-1)|
            WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_F(ufe_phy_cfg->type.atm.rx_alpha-1);

         /* init "shift-counter" to avoid old data, for cell delination purpose*/
         rspt->type.atm.cntrl1=WUFEI_PHY_ATM_RX_INIT_CNTRL1;
      }
   }

   if(tx_enb)
   {
      if((ufe_phy_cfg->type.atm.tx_scrambler_mode>1) ||
         (ufe_phy_cfg->type.atm.tx_coset_mode>1) ||
         (ufe_phy_cfg->type.atm.tx_idle_unassigned_cell_mode>1) ||
         (ufe_phy_cfg->type.atm.tx_underrun_interrupt_mode>1)||
         (ufe_phy_cfg->tx_fifo_param.transmit_th !=0))
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx_scrambler_mode (%d) > 1\n",
                         ufe_phy_cfg->type.atm.tx_scrambler_mode));
         WUFE_PRINT_ERR((wufei_log, "\ttx_coset_mode (%d) > 1\n",
                         ufe_phy_cfg->type.atm.tx_coset_mode));
         WUFE_PRINT_ERR((wufei_log, "\ttx_idle_unassigned_cell_mode (%d) > 1\n",
                         ufe_phy_cfg->type.atm.tx_idle_unassigned_cell_mode));
         WUFE_PRINT_ERR((wufei_log, "\ttx_underrun_interrupt_mode (%d) > 1\n",
                         ufe_phy_cfg->type.atm.tx_underrun_interrupt_mode));
         WUFE_PRINT_ERR((wufei_log, "\ttransmit_th (%d) !=0 \n",
                         ufe_phy_cfg->tx_fifo_param.transmit_th));

         return WUFE_ERR_PHY_CONFIG;
      }
      else if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         tspt->type.atm.mode =
            WUFEI_PHY_TX_MODE_TYPE_F(WUFE_PHY_MODE_ATM)|
            WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_F(ufe_phy_cfg->type.atm.tx_scrambler_mode)|
            WUFEI_PHY_ATM_TX_MODE_COSET_F(ufe_phy_cfg->type.atm.tx_coset_mode)|
            WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_F(
               ufe_phy_cfg->type.atm.tx_underrun_interrupt_mode)|
            WUFEI_PHY_ATM_TX_MODE_CLP_F(ufe_phy_cfg->type.atm.tx_idle_unassigned_cell_mode);

         /* init the cntrl 1.data_ptr to 0xC for the begining of buffer*/
         tspt->type.atm.cntrl1=WUFEI_PHY_ATM_TX_INIT_CNTRL1;
         tspt->type.atm.data2=ufe_phy_cfg->type.atm.tx_idle_unassigned_cell_mode;
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_PhyCreate
 *
 * Purpose:   create new phy according to the
 *
 *
 * Description:
 *     This function
 *
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFE_PhyCreate(WUFE_handle *ufe_phy_handle,
                           WUFE_handle  ufe_line_handle,
                           WUFE_phy    *ufe_phy_cfg,
                           WP_U32       direction)
{
   WP_U32 wufe_id, core_id, line_id, wp_port_id;
   WP_U16 phy_id, emphy_port_id;
   WP_U32 rx_enb, tx_enb;
   WUFE_status status;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_phy_registry   *phy_registry;
   WUFEI_rspt rspt;
   WUFEI_rspt *rspt_ptr;
   WUFEI_tspt tspt;
   WUFEI_tspt *tspt_ptr;

   memset(&rspt, 0, sizeof(WUFEI_rspt));
   memset(&tspt, 0, sizeof(WUFEI_tspt));

   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhySetRxTxEnb(direction, &tx_enb, &rx_enb));

   /*********************************/
   /* Get registry, system          */
   /*********************************/
   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_line_handle);

   /* lock the wufe_id and get the system registry */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   /*********************************/
   /* Get registry, line            */
   /*********************************/
   line_id = WUFEI_HANDLE_INDEX_V (ufe_line_handle);
   if ((WUFEI_HANDLE_TYPE_V(ufe_line_handle)    != WUFE_HANDLE_TYPE_LINE) ||
       (WUFEI_HANDLE_SUB_TYPE_V(ufe_line_handle)!= WUFE_HANDLE_SUB_TYPE_CAD &&
        WUFEI_HANDLE_SUB_TYPE_V(ufe_line_handle)!= WUFE_HANDLE_SUB_TYPE_SBI &&
        WUFEI_HANDLE_SUB_TYPE_V(ufe_line_handle)!= WUFE_HANDLE_SUB_TYPE_SONET &&
        WUFEI_HANDLE_SUB_TYPE_V(ufe_line_handle)!= WUFE_HANDLE_SUB_TYPE_SDH &&
        WUFEI_HANDLE_SUB_TYPE_V(ufe_line_handle)!= WUFE_HANDLE_SUB_TYPE_DCC &&
        WUFEI_HANDLE_SUB_TYPE_V(ufe_line_handle)!= WUFE_HANDLE_SUB_TYPE_GENERIC ))
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR line handle =0x%08X ", ufe_line_handle));
      return WUFE_ERR_HANDLE;
   }

   if (line_id >= system_registry->cfg.max_line_number)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR line id %d  ",line_id));
      return WUFE_ERR_HANDLE;
   }

   /* Get the line registry */
   line_registry = &system_registry->line_arr[line_id];

   if (line_registry->state_mode != WUFEI_LINE_DISABLE &&
       line_registry->state_mode != WUFEI_LINE_ENABLE)
   {/* Empty line*/
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR line_registry  line handle 0x%08X ", ufe_line_handle));
      return WUFE_ERR_HANDLE;
   }
   core_id = line_registry->ufe_core_id;

   /*************************************
    *   Check phase of CFG and HW State:
    *   -------------------------------
    *    TDM slots
    *    FIFO Manager
    *    Protocol
    *    get the designated phy id
    *    HW
    *************************************/

   /*******************/
   /* check TDM slots */
   /*******************/
   if (rx_enb)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_PhyTdmSlotsCheck(&(ufe_phy_cfg->rx_slot_group),
                                                             WUFEI_LINE_TDM_RX,
                                                             line_registry),
                                      wufe_id);
   }

   if (tx_enb)
   {

      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_PhyTdmSlotsCheck(&(ufe_phy_cfg->tx_slot_group),
                                                             WUFEI_LINE_TDM_TX,
                                                             line_registry),
                                      wufe_id);

      if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTxFbpCounter(system_registry,
                                                              ufe_phy_cfg->tx_fifo_param.fifo_max_size,
                                                              0),
                                        wufe_id);
      }
   }

   /*******************/
   /* check FIFO      */
   /*******************/
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      /* Check and config the FIFO MANAGER information */
      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_PhyFifoCfg(rx_enb,
                                                       tx_enb,
                                                       &rspt,
                                                       &tspt,
                                                       ufe_phy_cfg,
                                                       system_registry),
                                      wufe_id);
   }

   /*******************/
   /* check Protocol  */
   /*******************/
   WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                   WUFEI_PhyCfgProtocol(&rspt,
                                                        rx_enb,
                                                        &tspt,
                                                        tx_enb,
                                                        ufe_phy_cfg,
                                                        system_registry,
                                                        line_registry),
                                   wufe_id);

   /* Get next free phy ID */
   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4 &&
       line_registry->line_mode == WUFE_STRUCT_LINE_DCC)
   {
      if (ufe_phy_cfg->struct_id != WUFE_STRUCT_PHY_HDLC)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log, "ERROR: DCC phy must be of type HDLC\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }
      
      /*In case of DCC, specific phy id is assigned.*/
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_AssignDccPhyId(&phy_id, &emphy_port_id,
                                                          system_registry, core_id, line_id),
                                     wufe_id);
   }
   else
   {/*UFE3 or UFE4 not DCC*/
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_AssignPhyId(&phy_id, &emphy_port_id,
                                                       system_registry, core_id),
                                     wufe_id);
   }


   /*******************/
   /* check HW        */
   /*******************/
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {

      /* Check the HW struct of the phy
       * check the Pointers
       * fix the RSPT and TSPT according to the HW
       */
      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_PhyCheckIdleState(rx_enb,
                                                              tx_enb,
                                                              &rspt,
                                                              &tspt,
                                                              emphy_port_id,
                                                              wufe_id,
                                                              core_id,
                                                              system_registry,
                                                              0/* inconsist_flag*/),
                                      wufe_id);
   }

   /**************************************************************
    *  Commit Phase:
    *  -------------
    *     HW
    *     registry
    *     prepare handel
    *************************************************************/

   /**************************************************************
    *  HW Commit :
    *  -------------
    * write TSPT (disabled)
    * write to the TX TDM RAM(enabeld)
    * write RSPT (disabled)
    * write to the RX TDM RAM(enabeld)
    ************************************************************************/

   /* the offset in the TDM RAM table [in groups of 32 slots] */

   if (tx_enb)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         /* commit the TSPT configuration */
         tspt_ptr = WUFEI_TX_PHY(emphy_port_id);
         WUFEI_WriteTspt(wufe_id,
                         core_id,
                         tspt,
                         tspt_ptr);
         /* commit the TX TDM slot configuration */
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTdmSlotsSet(wufe_id,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFEI_LINE_TDM_TX,
                                                             &(ufe_phy_cfg->tx_slot_group),
                                                             line_registry),
                                        wufe_id);

         /*Configure the E3 ATM G751 bit in the SBI Mode RAM*/
         if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM &&
            line_registry->transfer_type==WUFE_SBI_TYPE_E3 &&
            line_registry->framed == WUFE_FRAMED)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwSbiE3AtmConfig(wufe_id,
                                                                  line_registry,
                                                                  WUFE_TX_DIRECTION),
                                           wufe_id);;
         }
      }
   }

   if (rx_enb)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         /* commit the RSPT configuration */
         rspt_ptr = WUFEI_RX_PHY(emphy_port_id);
         WUFEI_WriteRspt(wufe_id,
                         core_id,
                         rspt,
                         rspt_ptr);

         /* commit the RX TDM slot configuration */
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTdmSlotsSet(wufe_id,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFEI_LINE_TDM_RX,
                                                             &(ufe_phy_cfg->rx_slot_group),
                                                             line_registry),
                                        wufe_id);

         /*Configure the E3 ATM G751 bit in the SBI Mode RAM*/
         if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM &&
            line_registry->transfer_type==WUFE_SBI_TYPE_E3 &&
            line_registry->framed == WUFE_FRAMED)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwSbiE3AtmConfig(wufe_id,
                                                                  line_registry,
                                                                  WUFE_RX_DIRECTION),
                                           wufe_id);
         }
      }
   }

   /********************************************
    * commit in the registry
    * ----------------------
    * system registry (changes in phy and line registries)
    *
    * line_registry:
    * line_registry TDM is set in WUFEI_PhyTdmSlotsSet
    *
    * phy_registry
    * line_id the value from WUFEI_AssignPhyId
    * rx_active tx_active
    * cfg
    ********************************************/
   line_registry->n_attach_phy++;
   phy_registry = &system_registry->phy_arr[phy_id];
   phy_registry->line_id = line_id;
   phy_registry->phy_id = phy_id;
   phy_registry->emphy_port_id = emphy_port_id;

   memcpy(&(phy_registry->cfg),ufe_phy_cfg, sizeof(WUFE_phy));
   phy_registry->master = WUFEI_PHY_CLK_REC_MASTER_NA;

   if (rx_enb)
   {
      phy_registry->rx_active = WUFEI_PHY_DISABLE;
      if(line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX &&
         ufe_phy_cfg->type.trans.rx_clk_rec_mode == WUFE_ENABLE)
         phy_registry->master |= WUFEI_PHY_CLK_REC_SLAVE_RX;


      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                         WUFEI_HwFmPhyCreate(system_registry,
                                                             core_id,
                                                             phy_registry,
                                                             line_registry->transfer_type,
                                                             line_registry->framed,
                                                             WUFE_RX_DIRECTION),
                                         wufe_id);

         if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_TRANS)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwCesPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 &(ufe_phy_cfg->rx_slot_group),
                                                                 ufe_phy_cfg->type.trans.rx_data_unit_size,
                                                                 WUFE_RX_DIRECTION),
                                            wufe_id);
         else if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwAtmPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 ufe_phy_cfg,
                                                                 WUFE_RX_DIRECTION),
                                            wufe_id);
         else if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_HDLC)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwHdlcPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 ufe_phy_cfg,
                                                                 WUFE_RX_DIRECTION),
                                            wufe_id);
         else if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_POS)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwPosPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 ufe_phy_cfg,
                                                                 WUFE_RX_DIRECTION),
                                            wufe_id);
         else
         {
            WUFE_PRINT_ERR((wufei_log, "ERROR Only transparent,HDLC and ATM machines are implemented in UFE4\n"));
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            return WUFE_ERR_NOT_IMPLEMENTED;
         }

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwTdmSlotsSet(wufe_id,
                                                            core_id,
                                                            emphy_port_id,
                                                            WUFEI_LINE_TDM_RX,
                                                            &(ufe_phy_cfg->rx_slot_group),
                                                            line_registry),
                                        wufe_id);
      }
   }

   if (tx_enb)
   {
      phy_registry->tx_active = WUFEI_PHY_DISABLE;
      if(line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX)
         phy_registry->master |= WUFEI_PHY_CLK_REC_SLAVE_TX;

      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                         WUFEI_HwFmPhyCreate(system_registry,
                                                             core_id,
                                                             phy_registry,
                                                             line_registry->transfer_type,
                                                             line_registry->framed,
                                                             WUFE_TX_DIRECTION),
                                         wufe_id);

         if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_TRANS)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwCesPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 &(ufe_phy_cfg->tx_slot_group),
                                                                 0,
                                                                 WUFE_TX_DIRECTION),
                                            wufe_id);
         else if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwAtmPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 ufe_phy_cfg,
                                                                 WUFE_TX_DIRECTION),
                                            wufe_id);
         else if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_HDLC)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwHdlcPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 ufe_phy_cfg,
                                                                 WUFE_TX_DIRECTION),
                                            wufe_id);
         else if(ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_POS)
            WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                            WUFEI_HwPosPhyCreate(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 ufe_phy_cfg,
                                                                 WUFE_TX_DIRECTION),
                                            wufe_id);
         else
         {
            WUFE_PRINT_ERR((wufei_log, "ERROR Only transparent, HDLC and ATM machines are implemented in UFE4\n"));
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            return WUFE_ERR_NOT_IMPLEMENTED;
         }

         if(line_registry->transfer_type == WUFE_SDH_TYPE_E3 ||
            line_registry->transfer_type == WUFE_SONET_TYPE_E3 ||
            line_registry->transfer_type == WUFE_SDH_TYPE_DS3 ||
            line_registry->transfer_type == WUFE_SONET_TYPE_DS3)
         {
            /* set the start tx threshold for clear channel lines */
            WUFEI_HwFmVfpTxStartThresholdSet(wufe_id, core_id, (line_registry->aug1* WUFE_MAX_N_AU3) + line_registry->au3,
                                             line_registry->transfer_type, ufe_phy_cfg->struct_id);
         }
         else
         {
            /* set the start tx threshold for clear channel lines */
            WUFEI_HwFmVfpTxStartThresholdSet(wufe_id, core_id, line_registry->aug1,
                                             line_registry->transfer_type, ufe_phy_cfg->struct_id);
         }
         
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwTdmSlotsSet(wufe_id,
                                                            core_id,
                                                            emphy_port_id,
                                                            WUFEI_LINE_TDM_TX,
                                                            &(ufe_phy_cfg->tx_slot_group),
                                                            line_registry),
                                        wufe_id);
      }
   }

   /*********************
    * prepare PHY handle
    *********************/
   wp_port_id = WUFEI_PHY_ID_TO_PHY_HANDLE(core_id,
                                           system_registry->core1_phyid_offset,
                                           emphy_port_id);

   *ufe_phy_handle = (WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_PHY) |
                      WUFEI_HANDLE_UFE_ID_F(wufe_id) |
                      WUFEI_HANDLE_UFE_CORE_ID_F(core_id)|
                      WUFEI_HANDLE_INDEX_F(wp_port_id));

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFE_PhyEnable
 *
 * Purpose:   Enable the phy according to the handle and direction
 *
 *
 * Description:
 *     This function
 *
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFE_PhyEnable(WUFE_handle ufe_phy_handle,
                           WP_U32      direction)
{
   WUFE_status status;
   WP_U32 rx_enb, tx_enb;
   WP_U16 phy_id, emphy_port_id;
   WP_U32 wufe_id, core_id;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_phy_registry    *phy_registry;
   WUFE_phy              *ufe_phy_cfg;

   WUFEI_rspt rspt;
   WUFEI_tspt tspt;
   WUFEI_rspt *rspt_ptr ;
   WUFEI_tspt *tspt_ptr ;

   memset(&rspt, 0, sizeof(WUFEI_rspt));
   memset(&tspt, 0, sizeof(WUFEI_tspt));

   if (WUFEI_HANDLE_TYPE_V(ufe_phy_handle) != WUFE_HANDLE_TYPE_PHY)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR phy handle 0x%08X ", ufe_phy_handle));
      return WUFE_ERR_HANDLE;
   }

   /*********************************/
   /* Get registry, system          */
   /*********************************/
   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);

   /* lock the wufe_id and get the system registry */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   /* Set the directions varibals enable/disable */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhySetRxTxEnb(direction,&tx_enb,&rx_enb));

   /*********************************/
   /* Get registry, phy             */
   /*********************************/
   emphy_port_id = WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                              system_registry->core1_phyid_offset,
                                              WUFEI_HANDLE_INDEX_V(ufe_phy_handle));
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   if (phy_id >= system_registry->cfg.max_phy_number)
   {/* err phy id */
      WUFE_PRINT_ERR((wufei_log, "ERROR phy_id=%d ", phy_id));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_HANDLE;
   }

   phy_registry = &system_registry->phy_arr[phy_id];
   ufe_phy_cfg = &(phy_registry->cfg);

   /*********************************/
   /* Get registry, line            */
   /*********************************/
   if (phy_registry->line_id == WUFEI_LINE_NOT_CONNECTED)
   {/* empry line id*/
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_PHY_NOT_CONNECTED;
   }
   line_registry =&system_registry->line_arr[phy_registry->line_id];

   if(line_registry->state_mode != WUFEI_LINE_ENABLE)
   {
      if (line_registry->state_mode != WUFEI_LINE_DISABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log, "ERROR line_registry->state_mode != WUFEI_LINE_DISABLE "));
         return WUFE_ERR_HANDLE;
      }
      else
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_LINE_NOT_ENABLED;
      }
   }

   if (core_id != line_registry->ufe_core_id)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR core id %d  ",core_id));
      return WUFE_ERR_HANDLE;
   }

   /************************************
    * Config the RSPT and the TSPT
    * ----------------------------
    *
    *    (no TDM slots check)
    *    FIFO Manager
    *    Protocol
    *    HW
    *************************************/
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      /*******************/
      /* config FIFO     */
      /*******************/
      /* Check and config the FIFO MANAGER information */
      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_PhyFifoCfg(rx_enb, tx_enb, &rspt, &tspt, ufe_phy_cfg, system_registry),
                                      wufe_id);
      /*******************/
      /* check Protocol  */
      /*******************/
      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_PhyCfgProtocol(&rspt,
                                                           rx_enb,
                                                           &tspt,
                                                           tx_enb,
                                                           ufe_phy_cfg,
                                                           system_registry,
                                                           line_registry),
                                      wufe_id);

      /*******************/
      /* check HW        */
      /*******************/

      /* Check the HW struct of the phy
       * check the Pointers
       * fix the RSPT and TSPT according to the HW
       */
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_PhyCheckIdleState(rx_enb,
                                                             tx_enb,
                                                             &rspt,
                                                             &tspt,
                                                             emphy_port_id,
                                                             wufe_id,
                                                             core_id,
                                                             system_registry,
                                                             0/* inconsist_flag*/),
                                     wufe_id);
   }
/*
   else
   {
      Check read/write pointers in the fifo manager
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_HwFmPhyCheckMemPtr(wufe_id,
                                  core_id,
                                  emphy_port_id,
                                  WUFE_FULL_DUPLEX),
         wufe_id);
   }
*/
   /**************************************************************
    *  Commit Phase:
    *  -------------
    *     HW
    *     write TSPT (disabled)
    *     registry tx  enable;
    *     write RSPT (disabled)
    *
    *************************************************************/
   if(tx_enb)
   {
      if ( phy_registry->tx_active != WUFEI_PHY_DISABLE )
      {
         WUFE_PRINT_ERR((wufei_log, "the Tx Phy state is not  READY ( active or empty)"));
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_PHY_NOT_DISABLED;
      }

      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwAtmPhyEnable(system_registry,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFE_TX_DIRECTION),
                                           wufe_id);
         }
         else if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_TRANS)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwCesPhyEnable(system_registry,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFE_TX_DIRECTION),
                                           wufe_id);
         }
         else if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_HDLC)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwHdlcPhyEnable(system_registry,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFE_TX_DIRECTION),
                                           wufe_id);
         }
         else if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_POS)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwPosPhyEnable(system_registry,
                                                                core_id,
                                                                emphy_port_id,
                                                                WUFE_TX_DIRECTION),
                                           wufe_id);
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "ERROR Only transparent, HDLC and ATM machines are implemented in UFE4\n"));
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            return WUFE_ERR_NOT_IMPLEMENTED;
         }
      }
      else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         tspt_ptr = WUFEI_TX_PHY(emphy_port_id);
         WUFEI_WriteTspt(wufe_id, core_id, tspt, tspt_ptr);

         if( (phy_registry->master & WUFEI_PHY_CLK_REC_MASTER_TX) ||
             (phy_registry->master & WUFEI_PHY_CLK_REC_SLAVE_TX))
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_ClockRecPhyEnable(system_registry,
                                                                   core_id,
                                                                   emphy_port_id,
                                                                   phy_id,
                                                                   WUFE_TX_DIRECTION),
                                           wufe_id);
         }

         tspt.type.common.mode |= (WP_U16)WUFEI_PHY_ATM_TX_MODE_ENABLE_M;

         WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), &(tspt_ptr->type.common.mode));
         WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), tspt.type.common.mode);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }

      phy_registry->tx_active = WUFEI_PHY_ENABLE;
      line_registry->n_tx_active_phy++;
   }

   if(rx_enb)
   {
      if ( phy_registry->rx_active != WUFEI_PHY_DISABLE )
      {
         WUFE_PRINT_ERR((wufei_log, "the Rx Phy state is not  READY ( active or empty)"));
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_PHY_NOT_DISABLED;
      }

      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwAtmPhyEnable(system_registry,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFE_RX_DIRECTION),
                                           wufe_id);
         }
         else if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_TRANS)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwCesPhyEnable(system_registry,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFE_RX_DIRECTION),
                                           wufe_id);
         }
         else if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_HDLC)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwHdlcPhyEnable(system_registry,
                                                             core_id,
                                                             emphy_port_id,
                                                             WUFE_RX_DIRECTION),
                                           wufe_id);
         }
         else if (ufe_phy_cfg->struct_id == WUFE_STRUCT_PHY_POS)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwPosPhyEnable(system_registry,
                                                                core_id,
                                                                emphy_port_id,
                                                                WUFE_RX_DIRECTION),
                                           wufe_id);
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "ERROR Only transparent, HDLC and ATM machines are implemented in UFE4\n"));
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            return WUFE_ERR_NOT_IMPLEMENTED;
         }
      }
      else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         rspt_ptr = WUFEI_RX_PHY(emphy_port_id);

         WUFEI_WriteRspt(wufe_id, core_id, rspt, rspt_ptr);
         if(phy_registry->master & WUFEI_PHY_CLK_REC_SLAVE_RX)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_ClockRecPhyEnable(system_registry,
                                                                   core_id,
                                                                   emphy_port_id,
                                                                   phy_id,
                                                                   WUFE_RX_DIRECTION),
                                           wufe_id);
         }
         /*entry 8*/
         rspt.type.common.mode |= (WP_U16)WUFEI_PHY_ATM_RX_MODE_ENABLE_M;

         WUFEI_INDIRECT_ADDRESS_SET(WUFEI_ID(wufe_id,core_id), &(rspt_ptr->type.common.mode));
         WUFEI_INDIRECT_DATA_16_SET(WUFEI_ID(wufe_id,core_id), rspt.type.common.mode);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }

      phy_registry->rx_active = WUFEI_PHY_ENABLE;
      line_registry->n_rx_active_phy++;
   }
   


      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmActiveEnableSet(wufe_id,
                                                       core_id,
                                                       emphy_port_id, 
                                                       &(ufe_phy_cfg->tx_slot_group),
                                                       line_registry));
      

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_PhyDisable
 *
 * Purpose:   Disable the phy according to the handle and direction
 *
 *
 * Description:
 *     This function does GRACEFULL DISABLE.
 *     The function only signals the HW not to recieve new frames.
 *
 *
 * Inputs: ufe_phy_handle - the phy to be disabled
 *         direction:       the direction can be: WUFE_FULL_DUPLEX
 *                                                WUFE_RX_DIRECTION
 *                                                WUFE_TX_DIRECTION
 *
 * Outputs: the HW phy clear enable bit - gracefull stop.
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFE_PhyDisable(WUFE_handle ufe_phy_handle,
                            WP_U32      direction)
{
   WUFE_status status;
   WP_U16 emphy_port_id, phy_id;
   WP_U32 rx_enb, tx_enb;
   WP_U32 wufe_id, core_id, id;
   WUFEI_system_registry *system_registry;
   WUFEI_phy_registry    *phy_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_rspt *rspt_ptr;
   WUFEI_rspt  rspt;
   WUFEI_tspt *tspt_ptr;
   WUFEI_tspt  tspt;

   memset(&rspt, 0, sizeof(WUFEI_rspt));
   memset(&tspt, 0, sizeof(WUFEI_tspt));

   if (WUFEI_HANDLE_TYPE_V(ufe_phy_handle) != WUFE_HANDLE_TYPE_PHY)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR phy handle 0x%08X ", ufe_phy_handle));
      return WUFE_ERR_HANDLE;
   }

   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   if (direction != WUFE_FULL_DUPLEX)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            WUFE_PRINT_ERR((wufei_log, "ERROR half duplex not supported for UFE4"));
            return WUFE_ERR_HALF_DUPLEX_NOT_SUPPORTED_FOR_UFE4;
         }
      
      
   }

   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhySetRxTxEnb(direction, &tx_enb, &rx_enb));

   emphy_port_id = WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                              system_registry->core1_phyid_offset,
                                              WUFEI_HANDLE_INDEX_V(ufe_phy_handle));

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   if (phy_id >= system_registry->cfg.max_phy_number)
   {/* err phy id */
      WUFE_PRINT_ERR((wufei_log, "ERROR phy_id=%d ", phy_id));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_HANDLE;
   }

   phy_registry = &system_registry->phy_arr[phy_id];
   line_registry = &system_registry->line_arr[phy_registry->line_id];

   if (core_id != line_registry->ufe_core_id)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR core id %d  ",core_id));
      return WUFE_ERR_HANDLE;
   }

   id = WUFEI_ID(wufe_id, core_id);

   if(rx_enb)
   {
      if(phy_registry->rx_active == WUFEI_PHY_ENABLE)
      {
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_ATM)
            {
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwAtmPhyDisable(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 WUFE_RX_DIRECTION),
                                              wufe_id);
            }
            else if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_TRANS)
            {
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwCesPhyDisable(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 WUFE_RX_DIRECTION),
                                              wufe_id);
            }
            else if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_HDLC)
            {
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwHdlcPhyDisable(system_registry,
                                                                 core_id,
                                                                 emphy_port_id,
                                                                 WUFE_RX_DIRECTION),
                                              wufe_id);
            }
            else if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_POS)
            {
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwPosPhyDisable(system_registry,
                                                                    core_id,
                                                                    emphy_port_id,
                                                                    WUFE_RX_DIRECTION),
                                              wufe_id);
            }
            else
            {
               WUFE_PRINT_ERR((wufei_log, "ERROR Only transparent, HDLC and ATM machines are implemented in UFE4\n"));
               WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
               return WUFE_ERR_NOT_IMPLEMENTED;
            }
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /*Disable RSPT[0x08]*/
            rspt_ptr = WUFEI_RX_PHY(emphy_port_id);

            WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.common.mode));
            WUFEI_INDIRECT_DATA_16_GET(id, &(rspt.type.common.mode));

            if(!WUFEI_PHY_ATM_RX_MODE_ENABLE_V(rspt.type.common.mode))
               WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_INVALID_REGISTRY_STATE, wufe_id);
            rspt.type.common.mode &= (~WUFEI_PHY_ATM_RX_MODE_ENABLE_M);

            WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.common.mode));
            WUFEI_INDIRECT_DATA_16_SET(id, rspt.type.common.mode);

            WPL_Delay(1000);
            /*Disable RSPT[0x09] and TSU mode*/
            if(phy_registry->master & WUFEI_PHY_CLK_REC_SLAVE_RX)
            {

               WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                               WUFEI_ClockRecPhyDisable(system_registry,
                                                                        core_id,
                                                                        emphy_port_id,
                                                                        phy_id,
                                                                        WUFE_RX_DIRECTION),
                                               wufe_id);
            }
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }


         phy_registry->rx_active = WUFEI_PHY_DISABLE;
         line_registry->n_rx_active_phy--;
      }
      else if( phy_registry->rx_active == WUFEI_PHY_DISABLE)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR RX Phy call disable twice %d ", phy_id));

         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_PHY_ALREADY_DISABLED, wufe_id);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR RX invalid phy to disable %d ", phy_id));

         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_PHY_NOT_CONNECTED, wufe_id);
      }

   }

   if(tx_enb)
   {
      if(phy_registry->tx_active == WUFEI_PHY_ENABLE)
      {
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_ATM)
            {
#if 0
               if (WUFEI_HwAtmPhyUnderrunGet(system_registry, core_id,emphy_port_id))
               {
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                                 WUFEI_HwFmPhyClearPtr(wufe_id,
                                                                       core_id,
                                                                       emphy_port_id,
                                                                       WUFE_TX_DIRECTION),
                                                 wufe_id);
               }

#endif

               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwAtmPhyDisable(system_registry,
                                                                    core_id,
                                                                    emphy_port_id,
                                                                    WUFE_TX_DIRECTION),
                                              wufe_id);
            }
            else if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_TRANS)
            {
#if 0
               if (WUFEI_HwCesPhyUnderrunGet(system_registry, core_id,emphy_port_id))
               {
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                                 WUFEI_HwFmPhyClearPtr(wufe_id,
                                                                       core_id,
                                                                       emphy_port_id,
                                                                       WUFE_TX_DIRECTION),
                                                 wufe_id);
               }
#endif

               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwCesPhyDisable(system_registry,
                                                                    core_id,
                                                                    emphy_port_id,
                                                                    WUFE_TX_DIRECTION),
                                              wufe_id);
            }
            else if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_HDLC)
            {
#if 0
               if (WUFEI_HwHdlcPhyUnderrunGet(system_registry, core_id,emphy_port_id))
               {
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                                 WUFEI_HwFmPhyClearPtr(wufe_id,
                                                                       core_id,
                                                                       emphy_port_id,
                                                                       WUFE_TX_DIRECTION),
                                                 wufe_id);
               }
#endif
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwHdlcPhyDisable(system_registry,
                                                                     core_id,
                                                                     emphy_port_id,
                                                                     WUFE_TX_DIRECTION),
                                              wufe_id);
            }
            else if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_POS)
            {
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_HwPosPhyDisable(system_registry,
                                                                    core_id,
                                                                    emphy_port_id,
                                                                    WUFE_TX_DIRECTION),
                                              wufe_id);
            }
            else
            {
               WUFE_PRINT_ERR((wufei_log, "ERROR Only transparent, HDLC and ATM machines are implemented in UFE4\n"));
               WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
               return WUFE_ERR_NOT_IMPLEMENTED;
            }
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            if(phy_registry->master & (WUFEI_PHY_CLK_REC_MASTER_TX |WUFEI_PHY_CLK_REC_SLAVE_TX))
            {
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFEI_ClockRecPhyDisable(system_registry,
                                                                       core_id,
                                                                       emphy_port_id,
                                                                       phy_id,
                                                                       WUFE_TX_DIRECTION),
                                              wufe_id);
            }

            tspt_ptr = WUFEI_TX_PHY(emphy_port_id);

            WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->type.common.mode));
            WUFEI_INDIRECT_DATA_16_GET(id, &(tspt.type.common.mode));
            if(!WUFEI_PHY_ATM_TX_MODE_ENABLE_V(tspt.type.common.mode))
               WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_INVALID_REGISTRY_STATE, wufe_id);
            tspt.type.common.mode &= (~WUFEI_PHY_ATM_TX_MODE_ENABLE_M);

            WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->type.common.mode));
            WUFEI_INDIRECT_DATA_16_SET(id, tspt.type.common.mode); /* tspt mode enable; */
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }

         phy_registry->tx_active = WUFEI_PHY_DISABLE;
         line_registry->n_tx_active_phy--;
      }
      else if( phy_registry->tx_active == WUFEI_PHY_DISABLE)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR TX Phy call disable twice %d ", phy_id));

         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_PHY_ALREADY_DISABLED, wufe_id);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR TX invalid phy to disable %d ", phy_id));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_PHY_NOT_CONNECTED, wufe_id);
      }

   }
   /* give the buffers time to clean */
   WPL_Delay(2000);

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_PhyDisplay
 *
 * Purpose:   Display all parameter of the UFE phy handle
 *
 *
 * Description:
 *     This function display all information of the UFE  modules
 *     of the UFE.
 *
 *
 * Inputs:
 *     phy_handel The UFE Phy handle
 *
 * Outputs:
 *     string to display
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_PhyDisplay(WUFE_handle ufe_phy_handle)
{
   WUFE_status            status;
   WP_U32                 wufe_id, core_id, line_id, slot;
   WP_U16                 phy_id,emphy_port_id, i;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_phy_registry    *phy_registry;

   WUFEI_tspt  tmp_tspt;
   WUFEI_tspt *tspt_ptr;
   WUFEI_rspt  tmp_rspt;
   WUFEI_rspt *rspt_ptr;

   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   emphy_port_id = WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                              system_registry->core1_phyid_offset,
                                              WUFEI_HANDLE_INDEX_V(ufe_phy_handle));
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   if (phy_id >= system_registry->cfg.max_phy_number)
   {/* err phy id */
      WUFE_PRINT_ERR((wufei_log, "ERROR phy_id=%d ", phy_id));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_HANDLE;
   }

   phy_registry =&system_registry->phy_arr[phy_id];
   line_id = phy_registry->line_id;

   line_registry =&system_registry->line_arr[line_id];
   if (core_id != line_registry->ufe_core_id)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR core id %d  ",core_id));
      return WUFE_ERR_HANDLE;
   }

   if(phy_registry->rx_active != WUFEI_PHY_DISABLE  &&
      phy_registry->rx_active != WUFEI_PHY_ENABLE   &&
      phy_registry->tx_active != WUFEI_PHY_DISABLE  &&
      phy_registry->tx_active != WUFEI_PHY_ENABLE)
   {
      WUFE_PRINT_DISP((wufei_log,"The phy is not connected (phy_id #%d) \t",phy_id));
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_HANDLE,wufe_id);
   }

   WUFE_PRINT_DISP((wufei_log,
                    "\n #####  PHY DISPLAY (line_id = %d, wufe_id %d) #####\n",
                    line_id, wufe_id));

   /********************
    *  TX
    *
    ********************/
   switch (phy_registry->tx_active)
   {
      case WUFEI_PHY_EMPTY:
         WUFE_PRINT_DISP((wufei_log,"The PHY Tx is NOT active\n"));
         break;

      case WUFEI_PHY_DISABLE:
      case WUFEI_PHY_ENABLE:

         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WUFEI_HwTptDisplay(system_registry,
                               core_id,
                               emphy_port_id);
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            tspt_ptr = WUFEI_TX_PHY(emphy_port_id);
            memset(&tmp_tspt,0xff,sizeof(WUFEI_tspt));

            WUFEI_ReadTspt(wufe_id, core_id, &tmp_tspt,tspt_ptr);
            WUFE_PrintTspt(wufe_id, core_id, emphy_port_id);
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }

         if (phy_registry->tx_active == WUFEI_PHY_DISABLE)
         {
            WUFE_PRINT_DISP((wufei_log,"The PHY Tx is disabled\n"));
            if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
            {
               if (WUFEI_PHY_ATM_TX_MODE_ENABLE_V(tmp_tspt.type.common.mode))
               {
               WUFE_PRINT_DISP((wufei_log,"Error the HW PHY Tx is Enabled \n"));
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }
            }
         }
         else /*WUFE_PHY_ENABLE*/
         {
            WUFE_PRINT_DISP((wufei_log,"The PHY Tx is Enabled\n"));
            if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
            {
               if (!WUFEI_PHY_ATM_TX_MODE_ENABLE_V(tmp_tspt.type.common.mode))
               {
                  WUFE_PRINT_DISP((wufei_log,"Error the HW PHY Tx is Disabled \n"));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }
               WUFE_PRINT_DISP((wufei_log,"\tFIFO TX (phy_id %d)\n", phy_id));
               if( phy_registry->cfg.tx_fifo_param.fifo_max_size ==
                   WUFEI_PHY_FIFOM_TX_MAX_BUFF_V(tmp_tspt.fifom.tx_start_th_max_buff))
               {
                  WUFE_PRINT_DISP((wufei_log,"fifo_max_size %d\n",
                                   phy_registry->cfg.tx_fifo_param.fifo_max_size));
               }
               else
               {
                  WUFE_PRINT_DISP((wufei_log,"ERROR TX fifo_max_size\n"));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }

               if(phy_registry->cfg.tx_fifo_param.priority_table ==
                  WUFEI_PHY_FIFOM_TX_PRI_V(tmp_tspt.fifom.cntrl1))
               {
                  WUFE_PRINT_DISP((wufei_log,"fifo priority %d \n",
                                   phy_registry->cfg.tx_fifo_param.priority_table));
               }
               else
               {
                  WUFE_PRINT_DISP((wufei_log,"ERROR priority TX \n reg value:%d, hw value:%d\n",
                                   phy_registry->cfg.tx_fifo_param.priority_table,
                                   WUFEI_PHY_FIFOM_TX_PRI_V(tmp_tspt.fifom.cntrl1)));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }

               if(phy_registry->cfg.tx_fifo_param.fast_mode==
                  WUFEI_PHY_FIFOM_TX_FAST_V(tmp_tspt.fifom.cntrl1))
               {
                  WUFE_PRINT_DISP((wufei_log,"fast_mode     %d\n",
                                   phy_registry->cfg.tx_fifo_param.fast_mode));
               }
               else
               {
                  WUFE_PRINT_DISP(
                     (wufei_log,
                      "ERROR FIFO M  TX  fast mode\n registry value: %d, hw value %d\n",
                      phy_registry->cfg.tx_fifo_param.fast_mode,
                      WUFEI_PHY_FIFOM_TX_FAST_V(tmp_tspt.fifom.cntrl1)));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }

               if(phy_registry->cfg.tx_fifo_param.transmit_th ==
                  WUFEI_PHY_FIFOM_TX_BUFF_TH_V(tmp_tspt.fifom.tx_start_th_max_buff))
               {
                  WUFE_PRINT_DISP((wufei_log,"transmit threshold %d\n",
                                   phy_registry->cfg.tx_fifo_param.transmit_th));
               }
               else
               {
                  WUFE_PRINT_DISP((wufei_log,"ERROR FIFO M TX transmit threshold\n"));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }

               if( phy_registry->cfg.tx_fifo_param.wait_type ==
                   WUFEI_PHY_FIFOM_TX_WAIT_IN_V(tmp_tspt.fifom.cntrl1))
               {
                  WUFE_PRINT_DISP((wufei_log,"wait type          %d\n",
                                   phy_registry->cfg.tx_fifo_param.wait_type));
               }
               else
               {
                  WUFE_PRINT_DISP(
                     (wufei_log,"ERROR FIFO M  TX wait type\n registry value:%d, HW value %d\n",
                      phy_registry->cfg.tx_fifo_param.wait_type,
                      WUFEI_PHY_FIFOM_TX_WAIT_IN_V(tmp_tspt.fifom.cntrl1)));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }
            }
         }
         WUFE_PRINT_DISP((wufei_log,"\nTDM RAM TX (phy_id %d)\n", phy_id));
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwTdmSlotsDisplay(wufe_id,
                                                                   core_id,
                                                                   emphy_port_id,
                                                                   WUFEI_LINE_TDM_TX,
                                                                   &(phy_registry->cfg.tx_slot_group),
                                                                   line_registry),
                                           wufe_id);
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_PhyTdmSlotsDisplay( wufe_id,
                                                                     core_id,
                                                                     emphy_port_id,
                                                                     WUFEI_LINE_TDM_TX,
                                                                     &(phy_registry->cfg.tx_slot_group),
                                                                     line_registry), wufe_id);

            WUFE_PRINT_DISP((wufei_log,"\n"));

            if(line_registry->cas_enable)
            {
               WUFE_PRINT_DISP((wufei_log,"\t CAS RAM TX (phy_id %d)\n", phy_id));
               for (i = 0; i<phy_registry->cfg.tx_slot_group.n_slots; i++)
               {
                  slot = phy_registry->cfg.tx_slot_group.slot_arr[i];
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                                 WUFEI_HwSbiSlotTxCasDisplay(line_registry,
                                                                             slot,
                                                                             wufe_id), wufe_id);
                  if(i%4==3)
                     WUFE_PRINT_DISP((wufei_log,"\n"));
               }
               WUFE_PRINT_DISP((wufei_log,"\n"));
            }
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }

         break;
      default:
         WUFE_PRINT_ERR((wufei_log,
                         "The PHY Tx State is not valid (phy_registry->tx_active:%d )\n",
                         phy_registry->tx_active));
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_INVALID_REGISTRY_STATE;
   }/*end TX */

   /********************
    *  RX
    *
    ********************/
   switch (phy_registry->rx_active)
   {
      case WUFEI_PHY_EMPTY:
         WUFE_PRINT_DISP((wufei_log,"The PHY Rx is NOT active\n"));
         break;

      case WUFEI_PHY_DISABLE:
      case WUFEI_PHY_ENABLE:
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WUFEI_HwRptDisplay(system_registry,
                               core_id,
                               emphy_port_id);
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            rspt_ptr=WUFEI_RX_PHY(emphy_port_id);
            memset(&tmp_rspt,0xff,sizeof(WUFEI_rspt));

            WUFEI_ReadRspt(wufe_id, core_id, &tmp_rspt, rspt_ptr);
            WUFE_PrintRspt(wufe_id, core_id, emphy_port_id);
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }

         if (phy_registry->rx_active ==WUFEI_PHY_DISABLE)
         {
            WUFE_PRINT_DISP((wufei_log,"The PHY Rx is disabled\n"));
            if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
            {
               if (WUFEI_PHY_ATM_RX_MODE_ENABLE_V(tmp_rspt.type.common.mode))
               {
                  WUFE_PRINT_DISP((wufei_log,"Error the HW PHY Rx is Enabled \n"));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }
            }
         }
         else
         {
            WUFE_PRINT_DISP((wufei_log,"The PHY Rx is Enabled\n"));
            if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
            {
               if (!WUFEI_PHY_ATM_RX_MODE_ENABLE_V(tmp_rspt.type.common.mode))
               {
                  WUFE_PRINT_DISP((wufei_log,"Error the HW PHY Rx is Disabled \n"));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }
               WUFE_PRINT_DISP((wufei_log,"\tFIFO RX (phy_id %d)\n", phy_id));
               if( phy_registry->cfg.rx_fifo_param.fifo_max_size ==
                   WUFEI_PHY_FIFOM_RX_MAX_BUFF_V(tmp_rspt.fifom.max_buff_buff_count))
               {
                  WUFE_PRINT_DISP((wufei_log,"fifo_max_size %d\n",
                                   phy_registry->cfg.rx_fifo_param.fifo_max_size));
               }
               else
               {
                  WUFE_PRINT_DISP((wufei_log,"ERROR fifo_max_size\n"));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }

               if(phy_registry->cfg.rx_fifo_param.priority_table ==
                  WUFEI_PHY_FIFOM_RX_PRI_V(tmp_rspt.fifom.cntrl1))
               {
                  WUFE_PRINT_DISP((wufei_log,"fifo priority %d\n",
                                   phy_registry->cfg.rx_fifo_param.priority_table));
               }
               else
               {
                  WUFE_PRINT_DISP((wufei_log,"ERROR priority RX \n"));
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_INVALID_REGISTRY_STATE,wufe_id);
               }
            }
         }
         WUFE_PRINT_DISP((wufei_log,"\nTDM RAM RX (phy_id %d)\n", phy_id));
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwTdmSlotsDisplay(wufe_id,
                                                                   core_id,
                                                                   emphy_port_id,
                                                                   WUFEI_LINE_TDM_RX,
                                                                   &(phy_registry->cfg.rx_slot_group),
                                                                   line_registry),
                                           wufe_id);
            WUFE_PRINT_DISP((wufei_log,"\n"));
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwFmPhyDisplay(wufe_id,
                                                                core_id,
                                                                emphy_port_id),
                                                                wufe_id);

         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_PhyTdmSlotsDisplay( wufe_id,
                                                                     core_id,
                                                                     emphy_port_id,
                                                                     WUFEI_LINE_TDM_RX,
                                                                     &(phy_registry->cfg.rx_slot_group),
                                                                     line_registry), wufe_id);
            WUFE_PRINT_DISP((wufei_log,"\n"));

            if(line_registry->cas_enable)
            {
               WUFE_PRINT_DISP((wufei_log,"\nCAS RAM RX (phy_id %d)\n", phy_id));
               for (i = 0; i<phy_registry->cfg.rx_slot_group.n_slots; i++)
               {
                  slot = phy_registry->cfg.rx_slot_group.slot_arr[i];
                  WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                                 WUFEI_HwSbiSlotRxCasDisplay(line_registry,
                                                                             slot,
                                                                             wufe_id), wufe_id);
                  if(i%4==3)
                     WUFE_PRINT_DISP((wufei_log,"\n"));
               }
               WUFE_PRINT_DISP((wufei_log,"\n"));
            }
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }

         break;

      default:
         WUFE_PRINT_DISP((wufei_log,"The PHY Rx State is not valid\n"));
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_INVALID_REGISTRY_STATE;
   }/*end RX */

   if(phy_registry->master == WUFEI_PHY_CLK_REC_MASTER_NA)
      WUFE_PRINT_DISP((wufei_log,"The PHY has no clock recovery\n"));

   if(phy_registry->master & WUFEI_PHY_CLK_REC_MASTER_TX)
      WUFE_PRINT_DISP((wufei_log,"The PHY TX clock master\n"));

   if(phy_registry->master & WUFEI_PHY_CLK_REC_SLAVE_RX)
      WUFE_PRINT_DISP((wufei_log,"The PHY RX clock slave\n"));

   if(phy_registry->master &  WUFEI_PHY_CLK_REC_SLAVE_TX)
      WUFE_PRINT_DISP((wufei_log,"The PHY TX clock slave\n"));

   if(phy_registry->master == WUFEI_PHY_CLK_REC_SLAVE_FULL_DUPLEX)
      WUFE_PRINT_DISP((wufei_log,"The PHY FULL_DUPLEX clock slave\n"));

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;

}
/*****************************************************************************
 *
 * Function:  WUFE_PhyDelete
 *
 * Purpose:   Delete the UFE phy
 *
 * Description:
 *
 * Inputs:
 *     phy_handel The UFE Phy handle
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_PhyDelete(WUFE_handle ufe_phy_handle)
{
   WUFE_status            status;
   WP_U32                 wufe_id, core_id;
   WP_U16                 phy_id,emphy_port_id;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_phy_registry    *phy_registry;
   WUFE_phy              *reg_phy_cfg;

   /***************************************************
    * get registery :
    * --------------
    * Get System
    * Get Line
    * Get Phy
    ***************************************************/

   /*********************************/
   /* Get registry, system & attach */
   /*********************************/
   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);

   /* lock the wufe_id and get the system registry */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   /*********************************/
   /* Get registry, phy             */
   /*********************************/
   emphy_port_id = WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                              system_registry->core1_phyid_offset,
                                              WUFEI_HANDLE_INDEX_V(ufe_phy_handle));

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   if (phy_id >= system_registry->cfg.max_phy_number)
   {/* err phy id */
      WUFE_PRINT_ERR((wufei_log, "ERROR phy_id=%d ", phy_id));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_HANDLE;
   }

   phy_registry = &system_registry->phy_arr[phy_id];
   reg_phy_cfg = &(phy_registry->cfg);

   /*********************************/
   /* Get registry, line            */
   /*********************************/
   if (phy_registry->line_id == WUFEI_LINE_NOT_CONNECTED)
   {/* empty line id*/
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_PHY_NOT_CONNECTED;
   }
   line_registry = &system_registry->line_arr[phy_registry->line_id];
   if(line_registry->state_mode != WUFEI_LINE_ENABLE &&
      line_registry->state_mode != WUFEI_LINE_DISABLE)
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR line_registry->state_mode %d ",line_registry->state_mode));
      return WUFE_ERR_HANDLE;
   }
   if (core_id != line_registry->ufe_core_id)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR core id %d  ",core_id));
      return WUFE_ERR_HANDLE;
   }

   /***************************************************
    * check the phy is disable
    ***************************************************/
   if ( phy_registry->rx_active == WUFEI_PHY_ENABLE ||
        phy_registry->tx_active == WUFEI_PHY_ENABLE )
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_PHY_NOT_DISABLED;
   }

   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      /* CHECK IF IDLE */
      /*******************/
      /* check HW        */
      /*******************/

      /* Check the HW struct of the phy
       * check the Pointers
       * fix the RSPT and TSPT according to the HW
       */
      {
         WUFEI_rspt tmp_rspt;
         WUFEI_tspt tmp_tspt;

         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_PhyCheckIdleState(phy_registry->rx_active,  /* 0 empty  1 disable*/
                                    phy_registry->tx_active,  /* 0 empty  1 disable*/
                                    &tmp_rspt,
                                    &tmp_tspt,
                                    emphy_port_id,
                                    wufe_id,
                                    core_id,
                                    system_registry,
                                    0/* inconsist_flag*/),
            wufe_id);
      }
   }
/*
   else
   {
      Check read/write pointers in the fifo manager
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_HwFmPhyCheckMemPtr(wufe_id,
                                  core_id,
                                  emphy_port_id,
                                  WUFE_FULL_DUPLEX),
         wufe_id);
   }
*/

   /*    RX  */
   if(phy_registry->rx_active == WUFEI_PHY_DISABLE)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwTdmSlotsClear(wufe_id,
                                                              core_id,
                                                              emphy_port_id,
                                                              WUFEI_LINE_TDM_RX,
                                                              &(reg_phy_cfg->rx_slot_group),
                                                              line_registry),
                                        wufe_id);

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwPhyDelete(system_registry,
                                                          core_id,
                                                          emphy_port_id,
                                                          WUFE_RX_DIRECTION),
                                        wufe_id);

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwFmDeletePhy(system_registry,
                                                            core_id,
                                                            phy_registry,
                                                            line_registry->transfer_type,
                                                            line_registry->framed,
                                                            WUFE_RX_DIRECTION),
                                        wufe_id);

      }
      else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         /*Clear the E3 ATM G751 bit in the SBI Mode RAM*/
         if(phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_ATM &&
            line_registry->transfer_type==WUFE_SBI_TYPE_E3 &&
            line_registry->framed == WUFE_FRAMED)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwSbiE3AtmClear(wufe_id,
                                                                 line_registry,
                                                                 WUFE_RX_DIRECTION),
                                           wufe_id);
         }


         /* clean the TDM RAM*/
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyTdmSlotsClear(wufe_id,
                                                               core_id,
                                                               emphy_port_id,
                                                               WUFEI_LINE_TDM_RX,
                                                               &(reg_phy_cfg->rx_slot_group),
                                                               line_registry),
                                        wufe_id);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }

      phy_registry->rx_active = WUFEI_PHY_EMPTY;
   }

   /*    TX  */
   if(phy_registry->tx_active == WUFEI_PHY_DISABLE)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwTdmSlotsClear(wufe_id,
                                                              core_id,
                                                              emphy_port_id,
                                                              WUFEI_LINE_TDM_TX,
                                                              &(reg_phy_cfg->tx_slot_group),
                                                              line_registry),
                                        wufe_id);

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwPhyDelete(system_registry,
                                                          core_id,
                                                          emphy_port_id,
                                                          WUFE_TX_DIRECTION),
                                        wufe_id);

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwFmDeletePhy(system_registry,
                                                            core_id,
                                                            phy_registry,
                                                            line_registry->transfer_type,
                                                            line_registry->framed,
                                                            WUFE_TX_DIRECTION),
                                        wufe_id);

      }
      else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         /*Clear the E3 ATM G751 bit in the SBI Mode RAM*/
         if(phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_ATM &&
            line_registry->transfer_type==WUFE_SBI_TYPE_E3 &&
            line_registry->framed == WUFE_FRAMED)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_HwSbiE3AtmClear(wufe_id,
                                                                 line_registry,
                                                                 WUFE_TX_DIRECTION),
                                           wufe_id);
         }

         /* clean the TDM RAM*/
         WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                         WUFEI_PhyTdmSlotsClear(wufe_id,
                                                                core_id,
                                                                emphy_port_id,
                                                                WUFEI_LINE_TDM_TX,
                                                                &(reg_phy_cfg->tx_slot_group),
                                                                line_registry),
                                         wufe_id);

         /* SUBTRACT THE MAXZISE  FROM GLOBAL COUNTER */
         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_PhyTxFbpCounter(system_registry,
                                  0,
                                  reg_phy_cfg->tx_fifo_param.fifo_max_size),
            wufe_id);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }

      phy_registry->tx_active = WUFEI_PHY_EMPTY;
   }

   if( line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX)
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_ClockRecPhyDelete(system_registry, phy_id),
                                     wufe_id);

   /* RELEASE THE PHY ID*/
   WUFEI_FreePhyId(phy_id, emphy_port_id, system_registry, core_id);

   line_registry->n_attach_phy--;

   /* deatach*/
   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFE_PhyGetLineId
 *
 * Purpose:  return the line id of a specified phy
 *
 *
 * Description:
 *     This function retreives the line id of a given phy
 *
 * Inputs: ufe_phy_handle - UFE phy handle
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFE_PhyGetLineId(WUFE_handle ufe_phy_handle, WP_U32 *line_id)
{
   WUFE_status status;
   WP_U32 wufe_id,core_id;
   WP_U16 phy_id,emphy_port_id;
   WUFEI_system_registry *system_registry;
   WUFEI_phy_registry *phy_registry;

   /* check correctness of phy handle */
   if (WUFEI_HANDLE_TYPE_V(ufe_phy_handle)!= WUFE_HANDLE_TYPE_PHY)
   {
      WUFE_PRINT_ERR((wufei_log, "WUFE_PhyGetLineId: invalid phy handle 0x%08X ",
                      ufe_phy_handle));
      return WUFE_ERR_HANDLE;
   }

   /*********************************/
   /* Get registry, system          */
   /*********************************/
   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);

   /* lock the wufe_id and get the system registry */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   /* get the phy index */
   emphy_port_id = WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                              system_registry->core1_phyid_offset,
                                              WUFEI_HANDLE_INDEX_V(ufe_phy_handle));

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   if (phy_id >= system_registry->cfg.max_phy_number)
   {/* err phy id */
      WUFE_PRINT_ERR((wufei_log, "ERROR phy_id=%d ", phy_id));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_HANDLE;
   }

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_PhyRegistryGet(system_registry,
                                                       phy_id,
                                                       &phy_registry),
                                  wufe_id);

   *line_id = WUFEI_PhyLineIdGet(phy_registry);

   /* deatach*/
   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_PhyRegistryGet
 *
 * Purpose:  return the line id of a specified phy
 *
 *
 * Description:
 *     This function retreives the phy registry of a given phy
 *
 * Inputs: system_registry - system registry
 *         phy_id - phy id
 *         phy_registry - phy registry
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *     internal functions.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyRegistryGet(WUFEI_system_registry *system_registry,
                                 WP_U16 phy_id,
                                 WUFEI_phy_registry **phy_registry)
{
   if (phy_id >=system_registry->cfg.max_phy_number)
   {
      return WUFE_ERR_ILLEGAL_PHY_ID;
   }

   *phy_registry =&system_registry->phy_arr[phy_id];

   if ((*phy_registry)->line_id == WUFEI_LINE_NOT_CONNECTED)
   {
      return WUFE_ERR_PHY_NOT_CONNECTED;
   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyDataUnitSizeGet
 *
 * Purpose:
 *
 *
 * Description:
 *     This function retrieves the data unit size of a phy in the specified direction
 *
 * Inputs: phy_registry  - phy registry
 *         direction  - WUFE_TX_DIRECTION / WUFE_RX_DIRECTION
 *         phy_registry - phy registry
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *     internal functions.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyDataUnitSizeGet(WUFEI_system_registry *system_registry,
                                     WP_U16 phy_id, WP_U32 *data_unit_size, WP_U32 direction)
{
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WP_U16 line_id;
   WUFE_status status;

   if (!system_registry->clk_rec)
      return WUFE_ERR_CLOCK_REC_NOT_ENABLED;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));

   if ( direction == WUFE_TX_DIRECTION)
   {
      line_id = phy_registry->line_id;

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_LineRegistryGet(system_registry,
                                                  line_id,
                                                  &line_registry));

      *data_unit_size =
         system_registry->clk_rec->tx_arr[WUFEI_LineClockRecIfTxGet(line_registry)].
         tx_data_unit_size;
   }
   else if(direction == WUFE_RX_DIRECTION)
   {
      *data_unit_size = phy_registry->cfg.type.trans.rx_data_unit_size;
      /* *data_unit_size = phy_registry->cfg.trans.rx_data_unit_size;*/
   }
   else
   {
      return WUFE_ERR_PHY_INVALID_TYPE;

   }
   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function: WUFEI_PhyNumSlotsGet
 *
 * Purpose:
 *
 *
 * Description:
 *     This function retrieves the number of stots of a  phy in the
 *     specified direction
 *
 * Inputs: phy_registry  - phy registry
 *         direction  - WUFE_TX_DIRECTION / WUFE_RX_DIRECTION
 *         phy_registry - phy registry
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *     internal functions.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyNumSlotsGet(WUFEI_phy_registry *phy_registry,
                                 WP_U32 *n_slots, WP_U32 direction)
{

   if ( direction == WUFE_TX_DIRECTION)
   {
      *n_slots = phy_registry->cfg.tx_slot_group.n_slots;
   }
   else if(direction == WUFE_RX_DIRECTION)
   {
      *n_slots = phy_registry->cfg.rx_slot_group.n_slots;
   }
   else
   {
      return WUFE_ERR_INVALID_FIFO_CFG;

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_PhySetRxTxEnb
 *
 * Purpose:   set the value of rx enb and tx_enb
 *            according to direction.
 *
 * Description:
 *     This is used in order to sorten code readablity.
 *
 * Inputs:
 *      direction:    the direction can be: WUFE_FULL_DUPLEX
 *                                          WUFE_RX_DIRECTION
 *                                          WUFE_TX_DIRECTION
 *
 * Outputs:
 *        tx_enb {WUFE_ENABLE or WUFE_DISABLE}
 *        rx_enb {WUFE_ENABLE or WUFE_DISABLE}
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhySetRxTxEnb(WP_U32 direction,WP_U32 *tx_enb,WP_U32 *rx_enb)
{
   /* Set the directions varibals enable/disable */
   if (direction == WUFE_FULL_DUPLEX)
   {
      *tx_enb=WUFE_ENABLE;
      *rx_enb=WUFE_ENABLE;
   }
   else if (direction==WUFE_RX_DIRECTION)
   {
      *rx_enb=WUFE_ENABLE;
      *tx_enb=WUFE_DISABLE;
   }
   else if (direction==WUFE_TX_DIRECTION)
   {
      *rx_enb=WUFE_DISABLE;
      *tx_enb=WUFE_ENABLE;
   }
   else
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:WUFEI_PhyClockRecTSUpdate
 *
 * Purpose:   updates the TSPT and RSPT clock recovery entries
 *
 * Description:
 *     This is used in order to sorten code readablity.
 *
 * Inputs:
 *      wufe_id
 *      phy_id
 *      direction - WUFE_RX_DIRECTION/WUFE_TX_DIRECTION
 *      enable
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *    WUFEI_ClockRecRxTimestampConfig/WUFEI_ClockRecTxTimestampConfig function.
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyClockRecTSUpdate(WP_U32 wufe_id,
                                      WP_U32 core_id,
                                      WP_U16 phy_id,
                                      WP_U16 clk_rec_if,
                                      WP_U32 direction,
                                      WP_U8 enable)
{
   WP_U16 value = 0;
   WP_U32 rx_enb, tx_enb;
   WUFE_status status;
   WP_U32 id = WUFEI_ID(wufe_id,core_id);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhySetRxTxEnb(direction, &tx_enb, &rx_enb));

   if(tx_enb)
   {
      WUFEI_tspt *tspt_ptr;

      if(enable)
      {
         value = (WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_F(clk_rec_if)|
                  WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_M);
      }
      else
      {
         value = WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_F(0);
         value &= ~WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_M;
      }

      tspt_ptr=WUFEI_TX_PHY(phy_id);

      WUFEI_INDIRECT_ADDRESS_SET(id, &(tspt_ptr->type.trans.clk_rec_ts));
      WUFEI_INDIRECT_DATA_16_SET(id, value);

   }
   else if(rx_enb)
   {
      WUFEI_rspt *rspt_ptr;
      WP_U16 threshold;

      rspt_ptr=WUFEI_RX_PHY(phy_id);

      /* max_buffers validate*/
      WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->fifom.max_buff_buff_count));
      WUFEI_INDIRECT_DATA_16_GET(id, &value);
      if(WUFEI_PHY_FIFOM_RX_MAX_BUFF_V(value)>= 5)
         threshold = 3;
      else if(WUFEI_PHY_FIFOM_RX_MAX_BUFF_V(value)>= 3)
         threshold = 2;
      else
         threshold = 1;

      if(enable)
      {
         WP_U16 tmp_th=0;

         /*writes temp_treshhold RSPT[0x02]*/
         WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.trans.cntrl3));
         WUFEI_INDIRECT_DATA_16_GET(id, &tmp_th);
         tmp_th = WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_F(threshold);
         WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.trans.cntrl3));
         WUFEI_INDIRECT_DATA_16_SET(id, tmp_th);

         value = ( WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_F(threshold)|
                   WUFEI_PHY_TRANS_RX_CLK_REC_MASK_F(1)|
                   WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_M);
      }
      else
      {
         /*RSPT[0x09]*/
         WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.trans.clk_rec_ts));
         WUFEI_INDIRECT_DATA_16_GET(id, &value);

         value &= ~WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_M;
      }
      /*RSPT[0x09]*/
      WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.trans.clk_rec_ts));
      WUFEI_INDIRECT_DATA_16_SET(id, value);
   }
   else
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   return WUFE_OK;
}

static WUFE_status WUFEI_PhyDisableCheck(WP_U32 wufe_id, WP_U32 core_id,
                                         WUFEI_system_registry *system_registry,
                                         WUFEI_phy_registry *phy_registry)
{
   WUFE_status status = WUFE_OK;

   if (phy_registry->rx_active == WUFEI_PHY_ENABLE ||
       phy_registry->tx_active == WUFEI_PHY_ENABLE )
      return WUFE_ERR_PHY_NOT_DISABLED;

   /*******************/
   /* check HW        */
   /*******************/
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      WUFEI_rspt tmp_rspt;
      WUFEI_tspt tmp_tspt;

      /* Check the HW struct of the phy
       * check the Pointers
       * fix the RSPT and TSPT according to the HW
       */
      status = WUFEI_PhyCheckIdleState(phy_registry->rx_active,  /* 0 empty  1 disable*/
                                       phy_registry->tx_active,  /* 0 empty  1 disable*/
                                       &tmp_rspt,
                                       &tmp_tspt,
                                       phy_registry->emphy_port_id,
                                       wufe_id,
                                       core_id,
                                       system_registry,
                                       0/* inconsist_flag*/);
   }

   return status;
}

static WUFE_status WUFEI_PhyReconfigureRxSlots(WP_U32 wufe_id,
                                               WUFEI_system_registry *system_registry,
                                               WUFEI_phy_registry *phy_registry,
                                               WUFE_phy *usr_phy_cfg,
                                               WUFE_phy *reg_phy_cfg,
                                               WUFEI_line_registry *line_registry)
{
   WUFE_status status;
   WP_U32 core_id = line_registry->ufe_core_id;
   ((void)system_registry);
   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmSlotsClear(wufe_id,
                                                  core_id,
                                                  phy_registry->emphy_port_id,
                                                  WUFEI_LINE_TDM_RX,
                                                  &(reg_phy_cfg->rx_slot_group),
                                                  line_registry));

      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsCheck(&(usr_phy_cfg->rx_slot_group),
                                                           WUFEI_LINE_TDM_RX,
                                                           line_registry));

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmSlotsSet(wufe_id,
                                                core_id,
                                                phy_registry->emphy_port_id,
                                                WUFEI_LINE_TDM_RX,
                                                &(usr_phy_cfg->rx_slot_group),
                                                line_registry));

      if (reg_phy_cfg->struct_id == WUFE_STRUCT_PHY_TRANS &&
          line_registry->cas_enable)
      {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwCesConfigureSlots(wufe_id,
                                                      core_id,
                                                      &(usr_phy_cfg->rx_slot_group),
                                                      phy_registry->emphy_port_id,
                                                      WUFE_RX_DIRECTION));
   }
   }
   else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsClear(wufe_id,
                                                           core_id,
                                                           phy_registry->emphy_port_id,
                                                           WUFEI_LINE_TDM_RX,
                                                           &(reg_phy_cfg->rx_slot_group),
                                                           line_registry));

      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsCheck(&(usr_phy_cfg->rx_slot_group),
                                                           WUFEI_LINE_TDM_RX,
                                                           line_registry));

      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsSet(wufe_id,core_id,phy_registry->phy_id,
                                                         WUFEI_LINE_TDM_RX,
                                                         &(usr_phy_cfg->rx_slot_group),
                                                         line_registry));
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   memcpy(&(reg_phy_cfg->rx_slot_group), &(usr_phy_cfg->rx_slot_group), sizeof(WUFE_SlotGroup));

   return WUFE_OK;
}

static WUFE_status WUFEI_PhyReconfigureHdlc(WP_U32 wufe_id,
                                            WUFEI_phy_registry *phy_registry,
                                            WUFE_phy *usr_phy_cfg,
                                            WUFEI_line_registry *line_registry,
                                            WUFE_phy *reg_phy_cfg)
{
   WUFE_status status;
   WP_U32 core_id;
   ((void)wufe_id);
   ((void)phy_registry);   
   core_id = line_registry->ufe_core_id;

   if (usr_phy_cfg->type.hdlc.rx_frame_crc_size != WUFE_HDLC_NO_CHANGE)
   {
      if (usr_phy_cfg->type.hdlc.rx_frame_crc_size > WUFE_PHY_HDLC_CRC_32)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx_frame_crc_size (%d) > CRC 32\n",
                         usr_phy_cfg->type.hdlc.rx_frame_crc_size));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwHdlcCrcReconfig(wufe_id,
                                                    core_id,
                                                    phy_registry->emphy_port_id,
                                                    WUFE_RX_DIRECTION,
                                                    usr_phy_cfg->type.hdlc.rx_frame_crc_size));

      reg_phy_cfg->type.hdlc.rx_frame_crc_size = usr_phy_cfg->type.hdlc.rx_frame_crc_size;
   }

   if (usr_phy_cfg->type.hdlc.tx_frame_crc_size != WUFE_HDLC_NO_CHANGE)
   {
      if (usr_phy_cfg->type.hdlc.tx_frame_crc_size > WUFE_PHY_HDLC_CRC_32)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx_frame_crc_size (%d) > CRC 32\n",
                         usr_phy_cfg->type.hdlc.tx_frame_crc_size));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwHdlcCrcReconfig(wufe_id,
                                                    core_id,
                                                    phy_registry->emphy_port_id,
                                                    WUFE_TX_DIRECTION,
                                                    usr_phy_cfg->type.hdlc.tx_frame_crc_size));

      reg_phy_cfg->type.hdlc.tx_frame_crc_size = usr_phy_cfg->type.hdlc.tx_frame_crc_size;
   }

   if (usr_phy_cfg->type.hdlc.tx_no_flag_between_frames != WUFE_HDLC_NO_CHANGE)
   {
      if (usr_phy_cfg->type.hdlc.tx_no_flag_between_frames > WUFE_PHY_HDLC_TX_NO_FLAGS)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx no flags between frames (%d) > NO_FLAGS\n",
                         usr_phy_cfg->type.hdlc.tx_no_flag_between_frames));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwHdlcNoFlagsReconfig(wufe_id,
                                                        core_id,
                                                        phy_registry->emphy_port_id,
                                                        usr_phy_cfg->type.hdlc.tx_no_flag_between_frames));

      reg_phy_cfg->type.hdlc.tx_no_flag_between_frames = usr_phy_cfg->type.hdlc.tx_no_flag_between_frames;
   }

   if (usr_phy_cfg->type.hdlc.tx_min_flag_num != WUFE_HDLC_NO_CHANGE)
   {
      if (usr_phy_cfg->type.hdlc.tx_min_flag_num > 16 ||
          usr_phy_cfg->type.hdlc.tx_min_flag_num < 1)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy HDLC TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx min flags (%d) must be between 1 and 16\n",
                         usr_phy_cfg->type.hdlc.tx_min_flag_num));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwHdlcMinFlagsReconfig(wufe_id,
                                                         core_id,
                                                         phy_registry->emphy_port_id,
                                                         usr_phy_cfg->type.hdlc.tx_min_flag_num));

      reg_phy_cfg->type.hdlc.tx_min_flag_num = usr_phy_cfg->type.hdlc.tx_min_flag_num;
   }

   return WUFE_OK;
}

/* (usr_phy_cfg->type.atm.rx_descrambler_mode != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.rx_coset_mode != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.rx_idle_cell_discard != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.rx_unassigned_cell_discard != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.rx_hec_error_discard != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.rx_hec_correction_mode != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.rx_alpha != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.rx_delta != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.tx_scrambler_mode != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.tx_coset_mode != WUFE_HDLC_NO_CHANGE ||
    usr_phy_cfg->type.atm.tx_idle_unassigned_cell_mode != WUFE_HDLC_NO_CHANGE )
*/
static WUFE_status WUFEI_PhyReconfigureAtm(WP_U32 wufe_id,
                                            WUFEI_phy_registry *phy_registry,
                                            WUFE_phy *usr_phy_cfg,
                                            WUFEI_line_registry *line_registry,
                                            WUFE_phy *reg_phy_cfg)
{

   WUFE_status status;
   WP_U32 core_id;
   ((void)wufe_id);
   ((void)phy_registry); 
   core_id = line_registry->ufe_core_id;

   if (usr_phy_cfg->type.atm.rx_descrambler_mode != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_descrambler_mode > WUFE_PHY_ATM_RX_DESCRAMBLER_ENABLE)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx descrambler mode (%d) > RX DESCRAMBLER ENABLE\n",
               usr_phy_cfg->type.atm.rx_descrambler_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmScramblingReconfig(wufe_id,
                                                          core_id,
                                                          phy_registry->emphy_port_id,
                                                          WUFE_RX_DIRECTION,
                                                          usr_phy_cfg->type.atm.rx_descrambler_mode));

      reg_phy_cfg->type.atm.rx_descrambler_mode = usr_phy_cfg->type.atm.rx_descrambler_mode;
   }

   if (usr_phy_cfg->type.atm.rx_coset_mode != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_coset_mode > WUFE_PHY_ATM_RX_ADD_COSET)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx coset mode (%d) > RX ADD COSET\n",
               usr_phy_cfg->type.atm.rx_coset_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmCosetReconfig(wufe_id,
                                                     core_id,
                                                     phy_registry->emphy_port_id,
                                                     WUFE_RX_DIRECTION,
                                                     usr_phy_cfg->type.atm.rx_coset_mode));

      reg_phy_cfg->type.atm.rx_coset_mode = usr_phy_cfg->type.atm.rx_coset_mode;
   }

   if (usr_phy_cfg->type.atm.rx_idle_cell_discard != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_idle_cell_discard > WUFE_PHY_ATM_RX_IDLE_CELL_DISCARD)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx idle cell discard (%d) > RX IDLE CELL DISCARD\n",
               usr_phy_cfg->type.atm.rx_idle_cell_discard));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmIdleCellModeReconfig(wufe_id,
                                                            core_id,
                                                            phy_registry->emphy_port_id,
                                                            usr_phy_cfg->type.atm.rx_idle_cell_discard));

      reg_phy_cfg->type.atm.rx_idle_cell_discard = usr_phy_cfg->type.atm.rx_idle_cell_discard;
   }

   if (usr_phy_cfg->type.atm.rx_unassigned_cell_discard != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_unassigned_cell_discard > WUFE_PHY_ATM_RX_UNASSIGNED_CELL_DISCARD)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx unassigned cell discard (%d) > RX UNASSIGNED CELL DISCARD\n",
               usr_phy_cfg->type.atm.rx_unassigned_cell_discard));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmUnassignedCellModeReconfig(wufe_id,
                                                                  core_id,
                                                                  phy_registry->emphy_port_id,
                                                                  usr_phy_cfg->type.atm.rx_unassigned_cell_discard));

      reg_phy_cfg->type.atm.rx_unassigned_cell_discard = usr_phy_cfg->type.atm.rx_unassigned_cell_discard;
   }

   if (usr_phy_cfg->type.atm.rx_hec_error_discard != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_hec_error_discard > WUFE_PHY_ATM_RX_HEC_ERR_DISCARD)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx hec error discard (%d) > RX HEC ERR DISCARD\n",
               usr_phy_cfg->type.atm.rx_hec_error_discard));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmHecErrorDiscardModeReconfig(wufe_id,
                                                                   core_id,
                                                                   phy_registry->emphy_port_id,
                                                                   usr_phy_cfg->type.atm.rx_hec_error_discard));

      reg_phy_cfg->type.atm.rx_hec_error_discard = usr_phy_cfg->type.atm.rx_hec_error_discard;
   }

   if (usr_phy_cfg->type.atm.rx_hec_correction_mode != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_hec_correction_mode > WUFE_PHY_ATM_RX_HEC_CORECTION_ENABLE)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx hec correction mode (%d) > RX HEC CORECTION ENABLE\n",
               usr_phy_cfg->type.atm.rx_hec_correction_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmHecErrorCorrectionModeReconfig(wufe_id,
                                                                      core_id,
                                                                      phy_registry->emphy_port_id,
                                                                      usr_phy_cfg->type.atm.rx_hec_correction_mode));

      reg_phy_cfg->type.atm.rx_hec_correction_mode = usr_phy_cfg->type.atm.rx_hec_correction_mode;
   }

   if (usr_phy_cfg->type.atm.rx_cell_delineation != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_cell_delineation > WUFE_PHY_ATM_RX_CELL_DELINEATION_BYTE_RESOLUTION)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx cell delineation (%d) > RX CELL DELINEATION BYTE RESOLUTION\n",
               usr_phy_cfg->type.atm.rx_cell_delineation));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmCellDelineationModeReconfig(wufe_id,
                                                                   core_id,
                                                                   phy_registry->emphy_port_id,
                                                                   usr_phy_cfg->type.atm.rx_cell_delineation));

      reg_phy_cfg->type.atm.rx_cell_delineation = usr_phy_cfg->type.atm.rx_cell_delineation;
   }

   if (usr_phy_cfg->type.atm.rx_alpha != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_alpha < 1 || usr_phy_cfg->type.atm.rx_alpha > 16)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx alpha (%d) > must be between 1 and 16\n",
               usr_phy_cfg->type.atm.rx_alpha));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmRxAlphaReconfig(wufe_id,
                                                       core_id,
                                                       phy_registry->emphy_port_id,
                                                       usr_phy_cfg->type.atm.rx_alpha));

      reg_phy_cfg->type.atm.rx_alpha = usr_phy_cfg->type.atm.rx_alpha;
   }

   if (usr_phy_cfg->type.atm.rx_delta != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.rx_delta < 1 || usr_phy_cfg->type.atm.rx_delta > 16)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx delta (%d) must be between 1 and 16\n",
               usr_phy_cfg->type.atm.rx_delta));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmRxDeltaReconfig(wufe_id,
                                                       core_id,
                                                       phy_registry->emphy_port_id,
                                                       usr_phy_cfg->type.atm.rx_delta));

      reg_phy_cfg->type.atm.rx_delta = usr_phy_cfg->type.atm.rx_delta;
   }

   if (usr_phy_cfg->type.atm.tx_scrambler_mode != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.tx_scrambler_mode > WUFE_PHY_ATM_TX_SCRAMBLER_ENABLE)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx_scrambler_mode (%d) > TX SCRAMBLER ENABLE\n",
               usr_phy_cfg->type.atm.tx_scrambler_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmScramblingReconfig(wufe_id,
                                                          core_id,
                                                          phy_registry->emphy_port_id,
                                                          WUFE_TX_DIRECTION,
                                                          usr_phy_cfg->type.atm.tx_scrambler_mode));

      reg_phy_cfg->type.atm.tx_scrambler_mode = usr_phy_cfg->type.atm.tx_scrambler_mode;
   }

   if (usr_phy_cfg->type.atm.tx_coset_mode != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.tx_coset_mode > WUFE_PHY_ATM_TX_ADD_COSET)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx coset mode (%d) > TX ADD COSET\n",
               usr_phy_cfg->type.atm.tx_coset_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmCosetReconfig (wufe_id,
                                                      core_id,
                                                      phy_registry->emphy_port_id,
                                                      WUFE_TX_DIRECTION,
                                                      usr_phy_cfg->type.atm.tx_coset_mode));

      reg_phy_cfg->type.atm.tx_coset_mode = usr_phy_cfg->type.atm.tx_coset_mode;
   }

   if (usr_phy_cfg->type.atm.tx_idle_unassigned_cell_mode != WUFE_ATM_NO_CHANGE)
   {
      if (usr_phy_cfg->type.atm.tx_idle_unassigned_cell_mode > WUFE_PHY_ATM_TX_IDLE_CELL_MODE)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy ATM RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx idle unassigned cell mode (%d) > TX IDLE CELL MODE\n",
               usr_phy_cfg->type.atm.tx_idle_unassigned_cell_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwAtmIdleUnassignedCellModeReconfig(wufe_id,
                                                                      core_id,
                                                                      phy_registry->emphy_port_id,
                                                                      usr_phy_cfg->type.atm.tx_idle_unassigned_cell_mode));

      reg_phy_cfg->type.atm.tx_idle_unassigned_cell_mode = usr_phy_cfg->type.atm.tx_idle_unassigned_cell_mode;
   }
   return WUFE_OK;

}

static WUFE_status WUFEI_PhyReconfigurePos(WP_U32 wufe_id,
                                           WUFEI_phy_registry *phy_registry,
                                           WUFE_phy *usr_phy_cfg,
                                           WUFEI_line_registry *line_registry,
                                           WUFE_phy *reg_phy_cfg)
{
   WUFE_status status;
   WP_U32 core_id;
   ((void)wufe_id);
   ((void)phy_registry); 
   core_id = line_registry->ufe_core_id;

   if (usr_phy_cfg->type.pos.rx_frame_crc_size != WUFE_POS_NO_CHANGE)
   {
      if (usr_phy_cfg->type.pos.rx_frame_crc_size > WUFE_PHY_POS_CRC_32)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy POS RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx_frame_crc_size (%d) > CRC 32\n",
                         usr_phy_cfg->type.pos.rx_frame_crc_size));

         return WUFE_ERR_PHY_CONFIG;
      }

      if (reg_phy_cfg->type.pos.rx_frame_crc_size != usr_phy_cfg->type.pos.rx_frame_crc_size)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwPosCrcReconfig(wufe_id,
                                                      core_id,
                                                      phy_registry->emphy_port_id,
                                                      WUFE_RX_DIRECTION,
                                                      usr_phy_cfg->type.pos.rx_frame_crc_size));

         reg_phy_cfg->type.pos.rx_frame_crc_size = usr_phy_cfg->type.pos.rx_frame_crc_size;
      }
   }

   if (usr_phy_cfg->type.pos.tx_frame_crc_size != WUFE_POS_NO_CHANGE)
   {
      if (usr_phy_cfg->type.pos.tx_frame_crc_size > WUFE_PHY_POS_CRC_32)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy POS TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx_frame_crc_size (%d) > CRC 32\n",
                         usr_phy_cfg->type.pos.tx_frame_crc_size));

         return WUFE_ERR_PHY_CONFIG;
      }

      if (reg_phy_cfg->type.pos.tx_frame_crc_size != usr_phy_cfg->type.pos.tx_frame_crc_size)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwPosCrcReconfig(wufe_id,
                                                      core_id,
                                                      phy_registry->emphy_port_id,
                                                      WUFE_TX_DIRECTION,
                                                      usr_phy_cfg->type.pos.tx_frame_crc_size));
      
         reg_phy_cfg->type.pos.tx_frame_crc_size = usr_phy_cfg->type.pos.tx_frame_crc_size;
      }
   }

   if (usr_phy_cfg->type.pos.rx_descrambler_mode != WUFE_POS_NO_CHANGE)
   {
      if (usr_phy_cfg->type.pos.rx_descrambler_mode > WUFE_PHY_POS_RX_DESCRAMBLER_ENABLE)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy POS RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\trx descrambler mode (%d) > RX DESCRAMBLER ENABLE\n",
               usr_phy_cfg->type.pos.rx_descrambler_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      if (reg_phy_cfg->type.pos.rx_descrambler_mode != usr_phy_cfg->type.pos.rx_descrambler_mode)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwPosScramblingReconfig(wufe_id,
                                                             core_id,
                                                             phy_registry->emphy_port_id,
                                                             WUFE_RX_DIRECTION,
                                                             usr_phy_cfg->type.pos.rx_descrambler_mode));

         reg_phy_cfg->type.pos.rx_descrambler_mode = usr_phy_cfg->type.pos.rx_descrambler_mode;
      }
   }

   if (usr_phy_cfg->type.pos.tx_scrambler_mode != WUFE_POS_NO_CHANGE)
   {
      if (usr_phy_cfg->type.pos.tx_scrambler_mode > WUFE_PHY_POS_TX_SCRAMBLER_ENABLE)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy POS RX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx_scrambler_mode (%d) > TX SCRAMBLER ENABLE\n",
               usr_phy_cfg->type.pos.tx_scrambler_mode));

         return WUFE_ERR_PHY_CONFIG;
      }

      if (reg_phy_cfg->type.pos.tx_scrambler_mode != usr_phy_cfg->type.pos.tx_scrambler_mode)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwPosScramblingReconfig(wufe_id,
                                                             core_id,
                                                             phy_registry->emphy_port_id,
                                                             WUFE_TX_DIRECTION,
                                                             usr_phy_cfg->type.pos.tx_scrambler_mode));

         reg_phy_cfg->type.pos.tx_scrambler_mode = usr_phy_cfg->type.pos.tx_scrambler_mode;
      }
   }

   if (usr_phy_cfg->type.pos.tx_min_flag_num != WUFE_POS_NO_CHANGE)
   {
      if (usr_phy_cfg->type.pos.tx_min_flag_num > 16 ||
          usr_phy_cfg->type.pos.tx_min_flag_num < 1)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE Phy POS TX configuration is invalid:\n"));
         WUFE_PRINT_ERR((wufei_log, "\ttx min flags (%d) must be between 1 and 16\n",
                         usr_phy_cfg->type.pos.tx_min_flag_num));

         return WUFE_ERR_PHY_CONFIG;
      }

      if (reg_phy_cfg->type.pos.tx_min_flag_num != usr_phy_cfg->type.pos.tx_min_flag_num)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwPosMinFlagsReconfig(wufe_id,
                                                           core_id,
                                                           phy_registry->emphy_port_id,
                                                           usr_phy_cfg->type.pos.tx_min_flag_num));

         reg_phy_cfg->type.pos.tx_min_flag_num = usr_phy_cfg->type.pos.tx_min_flag_num;
      }
   }

   return WUFE_OK;
}

static WUFE_status WUFEI_PhyReconfigureTxSlots(WP_U32 wufe_id,
                                               WUFEI_system_registry *system_registry,
                                               WUFEI_phy_registry *phy_registry,
                                               WUFE_phy *usr_phy_cfg,
                                               WUFE_phy *reg_phy_cfg,
                                               WUFEI_line_registry *line_registry)
{
   WUFE_status status;
   WP_U32 core_id = line_registry->ufe_core_id;
   ((void)system_registry);
   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmSlotsClear(wufe_id,
                                                  core_id,
                                                  phy_registry->emphy_port_id,
                                                  WUFEI_LINE_TDM_TX,
                                                  &(reg_phy_cfg->tx_slot_group),
                                                  line_registry));

      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsCheck(&(usr_phy_cfg->tx_slot_group),
                                                           WUFEI_LINE_TDM_TX,
                                                           line_registry));

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmSlotsSet(wufe_id,
                                                core_id,
                                                phy_registry->emphy_port_id,
                                                WUFEI_LINE_TDM_TX,
                                                &(usr_phy_cfg->tx_slot_group),
                                                line_registry));

      if (reg_phy_cfg->struct_id == WUFE_STRUCT_PHY_TRANS &&
          line_registry->cas_enable)
      {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwCesConfigureSlots(wufe_id,
                                                      core_id,
                                                      &(usr_phy_cfg->tx_slot_group),
                                                      phy_registry->emphy_port_id,
                                                      WUFE_TX_DIRECTION));
      }
   }
   else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsClear(wufe_id, core_id,
                                                           phy_registry->emphy_port_id,
                                                           WUFEI_LINE_TDM_TX,
                                                           &(reg_phy_cfg->tx_slot_group),
                                                           line_registry));

      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsCheck(&(usr_phy_cfg->tx_slot_group),
                                                           WUFEI_LINE_TDM_TX,
                                                           line_registry));

      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTdmSlotsSet(wufe_id,core_id,phy_registry->phy_id,
                                                         WUFEI_LINE_TDM_TX,
                                                         &(usr_phy_cfg->tx_slot_group),
                                                         line_registry));
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   memcpy(&(reg_phy_cfg->tx_slot_group), &(usr_phy_cfg->tx_slot_group), sizeof(WUFE_SlotGroup));

   return WUFE_OK;
}

static WUFE_status WUFEI_PhyReconfigureRxClkRecMode(WUFEI_phy_registry *phy_registry,
                                                    WUFE_phy *usr_phy_cfg,
                                                    WUFE_phy *reg_phy_cfg,
                                                    WUFEI_line_registry *line_registry)

{

   if( !(line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX))
       return WUFE_ERR_CLOCK_REC_LINE_PARAMS;

   if(usr_phy_cfg->type.trans.rx_clk_rec_mode == WUFE_DISABLE &&
      reg_phy_cfg->type.trans.rx_clk_rec_mode == WUFE_ENABLE)
       phy_registry->master &= ~WUFEI_PHY_CLK_REC_SLAVE_RX;

   else if(usr_phy_cfg->type.trans.rx_clk_rec_mode == WUFE_ENABLE &&
           reg_phy_cfg->type.trans.rx_clk_rec_mode == WUFE_DISABLE)
      phy_registry->master |= WUFEI_PHY_CLK_REC_SLAVE_RX;

   reg_phy_cfg->type.trans.rx_clk_rec_mode = usr_phy_cfg->type.trans.rx_clk_rec_mode;
   return WUFE_OK;
}

static WP_U32 WUFEI_PhyReconfigureCheckRxFifo(WUFE_phy *usr_phy_cfg)
{
   WUFE_RxFifo tmp_rx_fifo;

   memset(&tmp_rx_fifo, 0xff, sizeof(WUFE_RxFifo));
   if (memcmp(&usr_phy_cfg->rx_fifo_param, &tmp_rx_fifo, sizeof(WUFE_RxFifo)) != 0)
      return 1;
   else
      return 0;
}

static WUFE_status WUFEI_PhyReconfigureRxFifo(WUFE_phy *usr_phy_cfg,
                                              WUFE_phy *reg_phy_cfg)

{
   WUFE_status status;

   WUFE_PRINT_ERR((wufei_log, "RX fifo reconfig \n"));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyCheckFifoRx(usr_phy_cfg->rx_fifo_param));

   memcpy(&(reg_phy_cfg->rx_fifo_param), &(usr_phy_cfg->rx_fifo_param), sizeof(WUFE_RxFifo));

   return status;
}


static WP_U32 WUFEI_PhyReconfigureCheckTxFifo(WUFE_phy *usr_phy_cfg)
{
   WUFE_TxFifo tmp_tx_fifo;

   memset(&tmp_tx_fifo, 0xff, sizeof(WUFE_TxFifo));
   if (memcmp(&usr_phy_cfg->tx_fifo_param, &tmp_tx_fifo, sizeof(WUFE_TxFifo)) != 0)
      return 1;
   else
      return 0;
}

static WUFE_status WUFEI_PhyReconfigureTxFifo(WUFEI_system_registry *system_registry,
                                              WUFE_phy *usr_phy_cfg,
                                              WUFE_phy *reg_phy_cfg)

{
   WUFE_status status;

   WUFE_PRINT_ERR((wufei_log, "TX fifo reconfig \n"));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyCheckFifoTx(usr_phy_cfg->tx_fifo_param));
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyTxFbpCounter(system_registry,
                                                          usr_phy_cfg->tx_fifo_param.fifo_max_size,
                                                          reg_phy_cfg->tx_fifo_param.fifo_max_size));
   }

   memcpy(&(reg_phy_cfg->tx_fifo_param), &(usr_phy_cfg->tx_fifo_param), sizeof(WUFE_TxFifo));

   return WUFE_OK;
}


static WUFE_status WUFEI_PhyReconfigureProtocol(WUFEI_system_registry *system_registry,
                                                WUFEI_phy_registry *phy_registry,
                                                WUFE_phy *usr_phy_cfg,
                                                WUFE_phy *reg_phy_cfg,
                                                WUFEI_line_registry *line_registry)
{
   WP_U32 rx_enb, tx_enb;
   WUFEI_rspt  rspt_dummy;
   WUFEI_tspt  tspt_dummy;
   WUFE_status status;

   if( (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) ||
       (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX))
       WUFEI_RETURN_IF_ERROR(status, WUFE_ERR_CLOCK_REC_INVALID_PROTOCOL_CONFIG);

   if( phy_registry->rx_active == WUFEI_PHY_DISABLE)
      rx_enb=WUFE_ENABLE;
   else
      rx_enb=WUFE_DISABLE;

   if( phy_registry->tx_active == WUFEI_PHY_DISABLE)
      tx_enb=WUFE_ENABLE;
   else
      tx_enb=WUFE_DISABLE;

   /*******************/
   /* check Protocol  */
   /*******************/
   WUFEI_RETURN_IF_ERROR( status,
                          WUFEI_PhyCfgProtocol(&rspt_dummy,
                                               rx_enb,
                                               &tspt_dummy,
                                               tx_enb,
                                               usr_phy_cfg,
                                               system_registry,
                                               line_registry));

   memcpy(&(reg_phy_cfg->type), &(usr_phy_cfg->type), sizeof(usr_phy_cfg->type));
   reg_phy_cfg->struct_id = usr_phy_cfg->struct_id;

   return WUFE_OK;
}


WUFE_status WUFE_PhyReconfigure(WUFE_handle ufe_phy_handle,
                                WUFE_phy   *usr_phy_cfg)
{
   WUFE_status            status;
   WP_U32                 wufe_id, core_id, emphy_port_id;
   WP_U16                 phy_id;
   WUFE_phy              *reg_phy_cfg;
   WUFEI_phy_registry    *phy_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_system_registry *system_registry;


   /***************************************************
    * get registery :
    * --------------
    * Get System
    * Get Line
    * Get Phy
    ***************************************************/

   /*********************************/
   /* Get registry, system          */
   /*********************************/
   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);
   /* lock the wufe_id and get the system registry */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   /*********************************/
   /* Get registry, phy             */
   /*********************************/
   emphy_port_id =  WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                               system_registry->core1_phyid_offset,
                                               WUFEI_HANDLE_INDEX_V(ufe_phy_handle));

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_PhyRegistryGet(system_registry,
                                                       phy_id,
                                                       &phy_registry),
                                  wufe_id);

   reg_phy_cfg = &(phy_registry->cfg);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_LineRegistryGet(system_registry,
                                                        phy_registry->line_id,
                                                        &line_registry),
                                  wufe_id);

   if (core_id != line_registry->ufe_core_id)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR core id %d  ",core_id));
      return WUFE_ERR_HANDLE;
   }

   /***************************************************
    * check the phy is disable
    ***************************************************/
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_PhyDisableCheck(wufe_id,
                                                        core_id,
                                                        system_registry,
                                                        phy_registry),
                                  wufe_id);
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      /* check RX clock shaper reconfigure*/
      if( (reg_phy_cfg->struct_id == WUFE_STRUCT_PHY_TRANS) &&
          (reg_phy_cfg->type.trans.rx_clk_rec_mode !=
           usr_phy_cfg->type.trans.rx_clk_rec_mode))
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyReconfigureRxClkRecMode(phy_registry,
                                                                         usr_phy_cfg,
                                                                         reg_phy_cfg,
                                                                         line_registry),
                                        wufe_id);
   }

   /***************************************************
    * check and configure TDM parameters:
    * ------------------------------
    *  if slot_tx_enb - remove the old slot TX configuration.
    *                 - check slot configuration TX
    *                 - commit slot configuration TX
    *
    *  if slot_rx_enb - remove the old slot RX configuration.
    *                 - check slot configuration RX
    *                 - commit slot configuration RX
    *
    ***************************************************/
   if (usr_phy_cfg->rx_slot_group.n_slots != WUFE_NO_CHANGE)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_PhyReconfigureRxSlots(wufe_id,
                                                                         system_registry,
                                                                         phy_registry,
                                                                         usr_phy_cfg,
                                                                         reg_phy_cfg,
                                                                         line_registry),
                                     wufe_id);
   }

   if (usr_phy_cfg->tx_slot_group.n_slots != WUFE_NO_CHANGE)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "ERROR - In UFE4 modification of Tx slots is not valid (configuration of Rx affects the Tx as well"));
         
         return WUFE_ERR_INVALID_CONFIG;
      }
      
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_PhyReconfigureTxSlots(wufe_id,
                                                                         system_registry,
                                                                         phy_registry,
                                                                         usr_phy_cfg,
                                                                         reg_phy_cfg,
                                                                         line_registry),
                                     wufe_id);
   }

   /***************************************************
    * check and configure FIFO parameters:
    * ------------------------------
    *  if fifo_rx_enb - check the FIFO M configuration RX
    *                 - copy  the FIFO M configuration RX
    *                 - DO NOT REWRITE the RSPT
    *
    *  if fifo_tx_enb - check the FIFO M configuration TX
    *                 - copy  the FIFO M configuration TX
    *                 - DO NOT REWRITE the TSPT
    *
    ***************************************************/
   if (WUFEI_PhyReconfigureCheckRxFifo(usr_phy_cfg))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_PhyReconfigureRxFifo(usr_phy_cfg,
                                                                reg_phy_cfg),
                                     wufe_id);
   }

   if (WUFEI_PhyReconfigureCheckTxFifo(usr_phy_cfg))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_PhyReconfigureTxFifo(system_registry,
                                                                usr_phy_cfg,
                                                                reg_phy_cfg),
                                     wufe_id);
   }

   /***************************************************
    * check and configure PROTOCOL parameters:
    * ------------------------------
    * if protocol_enb -check the protocol
    *                 - copy the parameters to the Registry
    ***************************************************/
   if (usr_phy_cfg->struct_id != WUFE_NO_CHANGE)
   {
      if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyReconfigureProtocol(system_registry,
                                                                     phy_registry,
                                                                     usr_phy_cfg,
                                                                     reg_phy_cfg,
                                                                     line_registry),
                                        wufe_id);
      }
   }

   /***************************************************
    * check and configure HDLC parameters:
    ***************************************************/
   if (reg_phy_cfg->struct_id == WUFE_STRUCT_PHY_HDLC &&
       WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (usr_phy_cfg->type.hdlc.rx_frame_crc_size != WUFE_HDLC_NO_CHANGE ||
          usr_phy_cfg->type.hdlc.tx_frame_crc_size != WUFE_HDLC_NO_CHANGE ||
          usr_phy_cfg->type.hdlc.tx_no_flag_between_frames != WUFE_HDLC_NO_CHANGE ||
          usr_phy_cfg->type.hdlc.tx_min_flag_num != WUFE_HDLC_NO_CHANGE)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyReconfigureHdlc(wufe_id,
                                                                 phy_registry,
                                                                 usr_phy_cfg,
                                                                 line_registry,
                                                                 reg_phy_cfg),
                                        wufe_id);
      }
   }

   /***************************************************
    * check and configure ATM parameters:
    ***************************************************/
   if (reg_phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM &&
         WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (usr_phy_cfg->type.atm.rx_descrambler_mode != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.rx_coset_mode != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.rx_idle_cell_discard != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.rx_unassigned_cell_discard != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.rx_hec_error_discard != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.rx_hec_correction_mode != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.rx_alpha != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.rx_delta != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.tx_scrambler_mode != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.tx_coset_mode != WUFE_ATM_NO_CHANGE ||
          usr_phy_cfg->type.atm.tx_idle_unassigned_cell_mode != WUFE_ATM_NO_CHANGE )
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyReconfigureAtm(wufe_id,
                                                                 phy_registry,
                                                                 usr_phy_cfg,
                                                                 line_registry,
                                                                 reg_phy_cfg),
                                                                 wufe_id);
      }
   }

   /***************************************************
    * check and configure POS parameters:
    ***************************************************/
   if (reg_phy_cfg->struct_id == WUFE_STRUCT_PHY_POS &&
       WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (usr_phy_cfg->type.pos.rx_frame_crc_size != WUFE_POS_NO_CHANGE ||
          usr_phy_cfg->type.pos.rx_descrambler_mode != WUFE_POS_NO_CHANGE ||
          usr_phy_cfg->type.pos.tx_frame_crc_size != WUFE_POS_NO_CHANGE ||
          usr_phy_cfg->type.pos.tx_scrambler_mode != WUFE_POS_NO_CHANGE ||
          usr_phy_cfg->type.pos.tx_min_flag_num != WUFE_POS_NO_CHANGE)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_PhyReconfigurePos(wufe_id,
                                                                phy_registry,
                                                                usr_phy_cfg,
                                                                line_registry,
                                                                reg_phy_cfg),
                                        wufe_id);
      }
   }

   /* relese and return*/
   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_PhyStateGet
 *
 * Purpose:   Reads the internal PHY state indication
 *           (HUNT./ PRESYNC./  SYNC (Correction Mode)./ SYNC (Detection Mode)).
 *
 * Description:
 *           If the Single Bit HEC Error Correction feature (HEN bit) is selected,
 *           both SYNC modes (Correction and Detection) are operational. If the
 *           HEN bit is cleared, only SYNC Correction mode is operational.
 *
 *
 * Inputs: ufe_phy_handle - the PHY to monitor
 *         sync_state - the output value for the PHY state
 *
 *
 *
 * Outputs: sync_state - WUFE_PHY_STATE_SYNC
 *                       WUFE_PHY_STATE_HUNT
 *                       WUFE_PHY_STATE_PRESYNC
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
WUFE_status WUFE_PhyStateGet( WUFE_handle ufe_phy_handle,
                              WUFE_phy_state* sync_state)
{
   WUFE_status status;
   WP_U32 wufe_id, core_id, id;
   WP_U16 phy_id,emphy_port_id;
   WUFEI_system_registry *system_registry;
   WUFEI_phy_registry    *phy_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_rspt *rspt_ptr ;
   WUFEI_rspt  rspt;

   memset(&rspt, 0, sizeof(WUFEI_rspt));

   wufe_id=WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   emphy_port_id = WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
                                              system_registry->core1_phyid_offset,
                                              WUFEI_HANDLE_INDEX_V(ufe_phy_handle));
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                             core_id, &phy_id, emphy_port_id),
                                  wufe_id);

   if (phy_id >=system_registry->cfg.max_phy_number)
   {/* err phy id */
      WUFE_PRINT_ERR((wufei_log, "ERROR phy_id=%d ", phy_id));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_HANDLE;
   }

   phy_registry = &system_registry->phy_arr[phy_id];
   line_registry = &system_registry->line_arr[phy_registry->line_id];
   if(line_registry->state_mode != WUFEI_LINE_ENABLE)
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR line_registry->state_mode %d ",line_registry->state_mode));
      return WUFE_ERR_HANDLE;
   }
   if (core_id != line_registry->ufe_core_id)
   {/* Illegal line id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      WUFE_PRINT_ERR((wufei_log, "ERROR core id %d  ",core_id));
      return WUFE_ERR_HANDLE;
   }

   id = WUFEI_ID(wufe_id, core_id);
   if(phy_registry->rx_active != WUFEI_PHY_ENABLE  )
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR RX PHY disabled %d ", phy_id));
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_PHY_DISABLED, wufe_id);
   }

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFE_phy *phy_cfg;

      phy_cfg = &(phy_registry->cfg);
      if (phy_cfg->struct_id == WUFE_STRUCT_PHY_ATM)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                         WUFEI_HwAtmPhyStateGet(system_registry,
                                                                core_id,
                                                                emphy_port_id,
                                                                sync_state),
                                         wufe_id);
      }
      else
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_NOT_IMPLEMENTED, wufe_id);
      }
   }
   else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      rspt_ptr = WUFEI_RX_PHY(emphy_port_id);

      WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.atm.cntrl2));
      WUFEI_INDIRECT_DATA_16_GET(id, &(rspt.type.atm.cntrl2));

      if( WUFEI_PHY_ATM_PORT_INSYNC_STATE_V(rspt.type.atm.cntrl2))
         *sync_state = WUFE_PHY_STATE_SYNC;
      else
         *sync_state = WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_V(rspt.type.atm.cntrl2);
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyHwCheck
 *
 * Purpose:   Check read/write to the RSPT memory
 *
 *
 * Description:
 * check the HW - RSPT and verify each entry could be written and read correcly
 * Set incremental value to all RSPT RAM in the following format
 * - 4 MSB are core ID
 * - 12 LSB are incremental value
 *
 * First loop
 * 1) Write values
 * 2) Every 16th write check addres accurecy
 *
 * Second loop
 * 1) check all values with the above structure
 * 2) Every last indrect Write for each RSPT check addres accurecy
 *
 * Inputs:
 *     system_registry    The regestry entry created for this UFE
 *
 * Outputs:
 *     None
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *    WUFE_ERR_ILLEGAL_NUM_PHYS
 *
 *
 *
 * Called by:
 *           Could be could in WUFEI_PhySystemSetup
 *           Otherwise must be call before WUFE_SystemConfig
 *
 ****************************************************************************/
WUFE_status WUFEI_PhyHwCheck(WUFEI_system_registry* system_registry) {
   WP_U32 coreid, i,id,counter,pattern, spt;
   WP_U16 val_to_check=0;
   WUFEI_rspt *rspt_ptr =  WUFEI_RX_PHY(0);
   WP_U32 wufe_id = system_registry->wufe_id;

   /* Set the RSPT RAM to incremental values*/

   printf("\n\n *********** WUFEI_PhyHwCheck ************\n");

   for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      /* counter contain core ID and incremental value */
      counter =0;
      id = WUFEI_ID(wufe_id, coreid);
      WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.common.cntrl1));

      for (i=0; i<(system_registry->max_phy_per_core ) ; i++)
      {

         for (spt=0; spt<=16; spt++)
         {
            WP_U16  low_addr_val=0, high_addr_val;
            counter++;
            pattern = counter & 0x0fff;
            pattern += coreid <<12;
            WUFEI_INDIRECT_DATA_16_SET(id,pattern);


            if(15==spt)
            {
               /* VERIFY THE ADDRESS OF THE INDRECT ADDRESS */
               WUFEI_MEM_16_GET(id,WUFEI_INDIRECT_ADD_HIGH_REG,
                                &high_addr_val);

               if (high_addr_val != 0 /*((WP_U32)&(rspt_ptr->type.common.cntrl1))>>16*/ )
               {
                  printf("Error counter incremented incorrectly addr_val MSB 0x%04X  !=0x%04X counter \n",
                         high_addr_val,counter);
                  return WUFE_ERR_MEM_INCONSIST;
               }

               WUFEI_MEM_16_GET(id,WUFEI_INDIRECT_ADD_LOW_REG,
                                &low_addr_val);
               if (low_addr_val !=counter)
               {
                  /* may be an issue of PCI retry mechanism */
                  printf("Error counter incremented incorrectly addr_val LSB 0x%04X  !=0x%04X counter \n",
                         low_addr_val,counter);
                  return WUFE_ERR_MEM_INCONSIST;

               }
            }
         }
      }
   }

   for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      counter =0;
      id = WUFEI_ID(wufe_id, coreid);
      WUFEI_INDIRECT_ADDRESS_SET(id, &(rspt_ptr->type.common.cntrl1));

      for (i=0; i<(system_registry->max_phy_per_core ) ; i++)
      {
         for (spt=0; spt<=16; spt++)
         {
            WP_U16  low_addr_val=0, high_addr_val;
            counter++;
            pattern = counter & 0x0fff;
            pattern += coreid <<12;
            WUFEI_INDIRECT_DATA_16_GET(id,&val_to_check);

            if (val_to_check != pattern)
            {
               printf("Error access val_to_check %d != counter %d\n",val_to_check, counter);
               return WUFE_ERR_MEM_INCONSIST;
            }

            if (15==spt)
            {

               /* VERIFY THE ADDRESS OF THE INDRECT ADDRESS */

               WUFEI_MEM_16_GET(id,WUFEI_INDIRECT_ADD_HIGH_REG,
                                &high_addr_val);
               if (high_addr_val != 0 /*(&(rspt_ptr->type.common.cntrl1))>>16*/ )
               {
                  printf("Error the counter was incremented incorrectly al_to_check addr_val MSB 0x%04X != 0x%04X counter \n",
                         high_addr_val,counter);
                  return WUFE_ERR_MEM_INCONSIST;
               }

               WUFEI_MEM_16_GET(id,WUFEI_INDIRECT_ADD_LOW_REG,
                                &low_addr_val);
               if (low_addr_val !=counter)
               {
                  /* may be an issue of PCI retry mechanism */
                  printf("Error the counter was incremented incorrectly al_to_check addr_val LSB 0x%04X  !=0x%04X counter \n",
                         low_addr_val,counter);
                  return WUFE_ERR_MEM_INCONSIST;

               }
            }
         }
      }
   }

   return WUFE_OK;
}
