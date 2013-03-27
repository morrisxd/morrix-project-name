/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#include <stdio.h>
#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif

#ifndef WUFEI_LINE_H
#include "wufei_line.h"
#endif

#ifndef WUFEI_HW_SBI_H
#include "wufei_hw_sbi.h"
#endif

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif

#ifndef WUFEI_CLK_TRANSLATOR_H
#include "wufei_clk_translator.h"
#endif

#ifndef WUFEI_HW_CLK_TRANSLATOR_INT_H
#include "wufei_hw_clk_translator_int.h"
#endif

#ifndef WUFEI_LINE_INT_H
#include "wufei_line_int.h"
#endif

#ifndef WUFEI_BOARD_SUPPORT_INT_H
#include "wufei_board_support_int.h"
#endif

#ifndef WUFEI_BOARD_SUPPORT_H
#include "wufei_board_support.h"
#endif

extern WP_U8 debug_flag;

void WUFEI_HwSbiEventRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_sbi*)sbi_base)->event_reg.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

void WUFEI_HwSbiEventRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_sbi*)sbi_base)->event_reg.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

void WUFEI_HwSbiEventMaskRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_sbi*)sbi_base)->event_mask_reg.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

void WUFEI_HwSbiEventMaskRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_sbi*)sbi_base)->event_mask_reg.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiIdirectAddrSet
 *
 * Purpose: Indirect address set for the SBI RAMs
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      indirect_addr - indirect address
 * Inputs:
 *
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwSbiIdirectAddrSet(WP_U32 ufeid, WP_U32 coreid, WP_U32 indirect_addr)
{
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WP_U32 core_base = ((coreid == WUFE_CORE_ID_0) ? WUFEI_INDIRECT_CORE0_OFFSET : WUFEI_INDIRECT_CORE1_OFFSET);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_sbi*)sbi_base)->indla.value;
   indha_ptr = &((WUFEI_hw_sbi*)sbi_base)->indha.value;

   WUFEI_REG_INDIRECT_ADDRESS_SET(indha_ptr/WUFEI_ADDR_DRV_TO_HW,
                                  indla_ptr/WUFEI_ADDR_DRV_TO_HW,
                                  indirect_addr*WUFEI_INDRCT_ADDR_ABS_TO_DRV,
                                  0, core_base);
   return;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiIdirectDataSet
 *
 * Purpose: Indirect address set for the SBI RAMs
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      value - value to set
 * Inputs:
 *
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwSbiIdirectDataSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;

   inddat_ptr = &((WUFEI_hw_sbi*)sbi_base)->inddat.value;

   WUFEI_REG_SET(inddat_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiIdirectDataGet
 *
 * Purpose: Indirect address set for the SBI RAMs
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      value - value to get
 * Inputs:
 *
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwSbiIdirectDataGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;

   inddat_ptr = &((WUFEI_hw_sbi*)sbi_base)->inddat.value;

   WUFEI_REG_GET(inddat_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiIdirectMemset
 *
 * Purpose: Indirect address set for the SBI RAMs
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      addr - start address
 *      value - value to get
 *      size - memset size
 * Inputs:
 *
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwSbiIdirectMemset(WP_U32 id, WP_U32 addr, WP_U16 value, WP_U32 size)
{
   WP_U32 i;
   WP_U32 ufeid = WUFEI_UFE_ID_V(id);
   WP_U32 coreid = WUFEI_UFE_CORE_ID_V(id);

   WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, addr);
   for(i = 0; i<size; i++)
   {
      WUFEI_HwSbiIdirectDataSet(ufeid, value);
   }

   return;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiCfgCheck
 *
 * Purpose: Checks if all SPEs are idle
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_INVALID_CONFIG
 *
 * Called by:
 *     WUFEI_HwSbiGmrSetup
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwSbiCfgCheck(WUFEI_system_registry* system_registry)
{
   WP_U32 i, idle_spe_cnt = 0;

   if (system_registry->cfg.sbi.in_clock  >= WUFE_SBI_CLK_INVALID ||
       system_registry->cfg.sbi.stm_mode  != WUFE_SBI_STM_INVALID ||
       system_registry->cfg.sbi.rx_parity >= WUFE_SBI_PARITY_INVALID  ||
       system_registry->cfg.sbi.tx_parity >= WUFE_SBI_PARITY_INVALID  ||
       system_registry->cfg.sbi.tx_parity == 0 ||
       system_registry->cfg.sbi.add_c1fp  >= WUFE_SBI_C1FP_INVALID ||
       system_registry->cfg.sbi.drop_c1fp >= WUFE_SBI_C1FP_INVALID ||
       system_registry->cfg.sbi.loopback  >= WUFE_SBI_LOOPBACK_INVALID ||
       system_registry->cfg.sbi.bus_master >= WUFE_SBI_BUS_INVALID )
   {
      WUFE_PRINT_ERR((wufei_log,"invalid SBI config\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   for(i=0; i<WUFE_MAX_N_SBI_SPE; i++)
   {
      if (system_registry->cfg.sbi.transfer_type_spe[i] > WUFE_SBI_TYPE_IDLE ||
          system_registry->cfg.sbi.transfer_type_spe[i] < WUFE_SBI_TYPE_T1 )
      {
         WUFE_PRINT_ERR((wufei_log,"invalid SBI transfer_type_spe config %d\n",
                         system_registry->cfg.sbi.transfer_type_spe[i] ));
         return WUFE_ERR_INVALID_CONFIG;
      }
      if (system_registry->cfg.sbi.transfer_type_spe[i] == WUFE_SBI_TYPE_IDLE)
         idle_spe_cnt++;
   }

   if (idle_spe_cnt == WUFE_MAX_N_SBI_SPE)
   {
      WUFE_PRINT_ERR((wufei_log,"all SPE are IDLE\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiGmrSetup
 *
 * Purpose: Enables the SBI Global mode register configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiSetup
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiGmrSetup(WUFEI_system_registry* system_registry)
{
   /*clean the SBI registers and SBI RAM */
   WUFE_status status;
   WP_U16 gmr_mask =0;
   WP_U16 *gmr_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   /* validate the user configuration*/
   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwSbiCfgCheck(system_registry));

   /* Clean Direct registers*/
   if(system_registry->cfg.sbi.loopback & WUFE_SBI_LOOPBACK_SYSTEM)
      gmr_mask |= WUFEI_SBI_SYS_LOOP_F(1);

   if(system_registry->cfg.sbi.loopback & WUFE_SBI_LOOPBACK_LINE)
   {
      gmr_mask |= WUFEI_SBI_LIN_LOOP_F(1);
      if (~(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
         WUFEI_RETURN_IF_ERROR(status,WUFEI_Ac1fpDelaySet(ufeid, WUFEI_BS_AC1FP_DELAY_2_CYCLE));
   }

   if(system_registry->cfg.sbi.rx_parity != WUFE_SBI_PARITY_NONE)
   {
      gmr_mask |=  WUFEI_SBI_MODE_RX_PRTY_CHK_F(1);
      if(system_registry->cfg.sbi.rx_parity == WUFE_SBI_PARITY_EVEN)
         gmr_mask |=  WUFEI_SBI_MODE_RX_PRTY_EVEN_F(1);
   }

   if(system_registry->cfg.sbi.tx_parity == WUFE_SBI_PARITY_EVEN)
      gmr_mask |=  WUFEI_SBI_MODE_TX_PRTY_EVEN_F(1);

   gmr_ptr = &((WUFEI_hw_sbi*)sbi_base)->gmr.value;

   WUFEI_REG_SET(gmr_ptr, gmr_mask);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiSpeModeSetup
 *
 * Purpose: Enables the SBI RX/TX SPE mode register configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *     coreid              UFE core identifier
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiSetup
 *
 ****************************************************************************/

WUFE_status WUFEI_HwSbiSpeModeSetup(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   /*clean the SBI registers and SBI RAM */
   WP_U32 stm1, spe, ufeid = system_registry->wufe_id;
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WUFEI_hw_sbi_core* core_ptr;

   core_ptr = &((WUFEI_hw_sbi*)sbi_base)->core[coreid];

   for(stm1 = 0; stm1 < WUFE_MAX_N_STM1; stm1++)
   {
      WP_U16* reg;
      WP_U16 rx_value =0, tx_value = 0;

      for(spe = 0; spe < WUFE_MAX_N_SPE; spe++)
      {
         WP_U32 idx = WUFEI_HW_SBI_N_SPE(coreid,stm1,spe);
         WUFE_line_transf_type transf_type;

         transf_type = system_registry->cfg.sbi.transfer_type_spe[idx];

         if(transf_type != WUFE_SBI_TYPE_IDLE)
         {
            WP_U16 value = WUFEI_SBI_TRIBUTARY_TYPE(transf_type);

            value = WUFEI_SBI_SPE_X_TRANFER_TYPE_F(value, spe);

            /* SBI RSPE mode registers */
            rx_value |= value;

            /* SBI TSPE mode registers */
            tx_value |= value;

         }
      }

      reg = &(core_ptr->stm1_rx[stm1].value);
      WUFEI_REG_SET(reg, rx_value);

      reg = &(core_ptr->stm1_tx[stm1].value);
      WUFEI_REG_SET(reg, tx_value);

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiRamSetup
 *
 * Purpose: resets the SBI CAS RAM and tributary RAM
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *     coreid              UFE core identifier
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiSetup
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiRamSetup(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   /*clean the SBI registers and SBI RAM */
   WP_U32 id = WUFEI_ID(system_registry->wufe_id, coreid);

   /********************************/
   /*** clear SBI trib mode ram   */
   /********************************/
   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_MODE_RX(0),
                            0x0000,
                            WUFEI_INDIR_SBI_MODE_RX_SIZE);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_MODE_TX(0),
                            0x0000,
                            WUFEI_INDIR_SBI_MODE_TX_SIZE);


   /****************************************/
   /*** clear SBI trib state machine ram   */
   /****************************************/
   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_STATE_RX(0),
                            0x0000,
                            WUFEI_INDIR_SBI_STATE_RX_SIZE);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_STATE_TX(0),
                            0x0000,
                            WUFEI_INDIR_SBI_STATE_TX_SIZE);

   /********************************/
   /*** clear SBI trib cas ram   */
   /********************************/
   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_CAS_RX(0),
                            0xfefe,
                            WUFEI_INDIR_SBI_CAS_RX_SIZE);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_CAS_TX(0),
                            0xffff,
                            WUFEI_INDIR_SBI_CAS_TX_SIZE);


   /***********************/
   /*** clear ALM/V5 RAM **/
   /***********************/
   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_RX_ALM_V5(0),
                            0x0000,
                            WUFEI_INDIR_SBI_RX_ALM_V5_SIZE);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiRamDelete
 *
 * Purpose: Deletes the SBI STM RAM and MODE RAM for a specific tributary
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     line_id             The id of the tributary to be deleted
 *     coreid              UFE core identifier
 *
 * Outputs:
 *     write to SBI RAM indirect register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFE_LineDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiRamDelete(WUFEI_system_registry *system_registry,
                                 WUFEI_line_registry   *line_registry,
                                 WP_U32 core_id)
{
   /*clean the SBI RAM for a specific tributary*/
   WP_U32 id = WUFEI_ID(system_registry->wufe_id, core_id);
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, core_id);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_STATE_RX(line_id),
                            0x0000,
                            2);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_STATE_TX(line_id),
                            0x0000,
                            1);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_MODE_RX(line_id),
                            0x0000,
                            1);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_MODE_TX(line_id),
                            0x0000,
                            1);

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiGmrEnable
 *
 * Purpose: Enables the SBI Global mode register configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *
 * Outputs:
 *     write to SBI global mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiGmrEnable(WUFEI_system_registry* system_registry, WP_U32 property)
{
   /* clear property bits for idle SPE */
   if ((property & WUFE_SYS_SBI_RX_1) &&
       (system_registry->cfg.sbi.transfer_type_spe[0] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_1 | WUFE_SYS_SBI_TX_1);
   if ((property & WUFE_SYS_SBI_RX_2) &&
       (system_registry->cfg.sbi.transfer_type_spe[1] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_2 | WUFE_SYS_SBI_TX_2);
   if ((property & WUFE_SYS_SBI_RX_3) &&
       (system_registry->cfg.sbi.transfer_type_spe[2] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_3 | WUFE_SYS_SBI_TX_3);
   if ((property & WUFE_SYS_SBI_RX_4) &&
       (system_registry->cfg.sbi.transfer_type_spe[3] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_4 | WUFE_SYS_SBI_TX_4);
   if ((property & WUFE_SYS_SBI_RX_5) &&
       (system_registry->cfg.sbi.transfer_type_spe[4] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_5 | WUFE_SYS_SBI_TX_5);
   if ((property & WUFE_SYS_SBI_RX_6) &&
       (system_registry->cfg.sbi.transfer_type_spe[5] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_6 | WUFE_SYS_SBI_TX_6);
   if ((property & WUFE_SYS_SBI_RX_7) &&
       (system_registry->cfg.sbi.transfer_type_spe[6] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_7 | WUFE_SYS_SBI_TX_7);
   if ((property & WUFE_SYS_SBI_RX_2) &&
       (system_registry->cfg.sbi.transfer_type_spe[7] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_8 | WUFE_SYS_SBI_TX_8);
   if ((property & WUFE_SYS_SBI_RX_9) &&
       (system_registry->cfg.sbi.transfer_type_spe[8] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_9 | WUFE_SYS_SBI_TX_9);
   if ((property & WUFE_SYS_SBI_RX_10) &&
       (system_registry->cfg.sbi.transfer_type_spe[9] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_10 | WUFE_SYS_SBI_TX_10);
   if ((property & WUFE_SYS_SBI_RX_11) &&
       (system_registry->cfg.sbi.transfer_type_spe[10] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_11 | WUFE_SYS_SBI_TX_11);
   if ((property & WUFE_SYS_SBI_RX_12) &&
       (system_registry->cfg.sbi.transfer_type_spe[11] == WUFE_SBI_TYPE_IDLE))
      property = property & ~(WUFE_SYS_SBI_RX_12 | WUFE_SYS_SBI_TX_12);


   if (property & WUFE_SYS_SBI_FULL)
   {
      WP_U32 ufeid = system_registry->wufe_id;
      WP_U16 gmr_mask =0;
      WP_U16 *gmr_ptr;
      WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

      /* PREPARE THE SBI GMR REGISTER MASK */
      if(system_registry->cfg.sbi.loopback & WUFE_SBI_LOOPBACK_SYSTEM)
         gmr_mask |= WUFEI_SBI_SYS_LOOP_M;

      if(system_registry->cfg.sbi.loopback &WUFE_SBI_LOOPBACK_LINE)
         gmr_mask |= WUFEI_SBI_LIN_LOOP_M;

      if(system_registry->cfg.sbi.rx_parity != WUFE_SBI_PARITY_NONE)
      {
         gmr_mask |= WUFEI_SBI_MODE_RX_PRTY_CHK_F(1);
         if(system_registry->cfg.sbi.rx_parity == WUFE_SBI_PARITY_EVEN)
            gmr_mask |= WUFEI_SBI_MODE_RX_PRTY_EVEN_F(1);
      }

      if(system_registry->cfg.sbi.tx_parity == WUFE_SBI_PARITY_EVEN)
         gmr_mask |= WUFEI_SBI_MODE_TX_PRTY_EVEN_F(1);

      if ((property & WUFE_SYS_SBI_RX) &&
          (system_registry->sys_enable.sbi_rx == WUFEI_SBI_DISABLE))
      {
         system_registry->sys_enable.sbi_rx = WUFEI_SBI_ENABLE;
         gmr_mask |= WUFEI_SBI_MODE_RX_ENABLE_F(1) ;
      }

      if ((property & WUFE_SYS_SBI_TX) &&
          (system_registry->sys_enable.sbi_tx == WUFEI_SBI_DISABLE))
      {
         system_registry->sys_enable.sbi_rx = WUFEI_SBI_ENABLE;
         gmr_mask |= WUFEI_SBI_MODE_TX_ENABLE_F(1) ;
      }

      /* MASK READY */
      gmr_ptr = &((WUFEI_hw_sbi*)sbi_base)->gmr.value;

      WUFEI_REG_SET(gmr_ptr, gmr_mask);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiSpeModeEnable
 *
 * Purpose: Enables the SBI RX/TX SPE mode registers configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *     coreid              UFE core identifier
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiSpeModeEnable(WUFEI_system_registry* system_registry,
                                     WP_U32 property, WP_U32 coreid)
{

   if (property & WUFE_SYS_SBI_FULL)
   {
      WP_U32 stm1, spe, ufeid = system_registry->wufe_id;
      WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
      WUFEI_hw_sbi_core* core_ptr;

      core_ptr = &((WUFEI_hw_sbi*)sbi_base)->core[coreid];

      for(stm1 = 0; stm1 < WUFE_MAX_N_STM1; stm1++)
      {
         WP_U16* reg;
         WP_U16 rx_value, tx_value;

         reg = &(core_ptr->stm1_rx[stm1].value);
         WUFEI_REG_GET(reg, &rx_value);

         reg = &(core_ptr->stm1_tx[stm1].value);
         WUFEI_REG_GET(reg, &tx_value);

         for(spe = 0; spe < WUFE_MAX_N_SPE; spe++)
         {
            WP_U32 idx = WUFEI_HW_SBI_N_SPE(coreid,stm1,spe);
            WUFE_line_transf_type transf_type;

            transf_type = system_registry->cfg.sbi.transfer_type_spe[idx];

            if ((property & WUFEI_SYS_SBI_RX(idx)) &&
                (system_registry->sys_enable.sbi_spe_rx[idx] == WUFEI_SBI_DISABLE))
            {

               WP_U16 value = WUFEI_SBI_TRIBUTARY_TYPE(transf_type);

               /* SBI RSPE mode registers */
               system_registry->sys_enable.sbi_spe_rx[idx] = WUFEI_SBI_ENABLE;
               value = ( WUFEI_SBI_SPE_X_TRANFER_TYPE_F(value, spe) |
                         WUFEI_SBI_SPE_X_ENABLE_F(1, spe));
               rx_value |= value;

            }

            if ((property & WUFEI_SYS_SBI_TX(idx)) &&
                (system_registry->sys_enable.sbi_spe_tx[idx] == WUFEI_SBI_DISABLE))
            {

               WP_U16 value = WUFEI_SBI_TRIBUTARY_TYPE(transf_type);

               /* SBI TSPE mode registers */
               system_registry->sys_enable.sbi_spe_tx[idx] = WUFEI_SBI_ENABLE;
               value = ( WUFEI_SBI_SPE_X_TRANFER_TYPE_F(value, spe) |
                         WUFEI_SBI_SPE_X_ENABLE_F(1, spe));
               tx_value |= value;
            }
         }

         reg = &(core_ptr->stm1_rx[stm1].value);
         WUFEI_REG_SET(reg, rx_value);

         reg = &(core_ptr->stm1_tx[stm1].value);
         WUFEI_REG_SET(reg, tx_value);

      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiGmrDisable
 *
 * Purpose: Disables the SBI register configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *     coreid              UFE core identifier
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiGmrDisable(WUFEI_system_registry* system_registry, WP_U32 property)
{

   if ((property &WUFE_SYS_SBI_RX_1)|(property &WUFE_SYS_SBI_RX_2)|(property &WUFE_SYS_SBI_RX_3)|
       (property &WUFE_SYS_SBI_RX_4)|(property &WUFE_SYS_SBI_RX_5)|(property &WUFE_SYS_SBI_RX_6)|
       (property &WUFE_SYS_SBI_RX_7)|(property &WUFE_SYS_SBI_RX_8)|(property &WUFE_SYS_SBI_RX_9)|
       (property&WUFE_SYS_SBI_RX_11)|(property&WUFE_SYS_SBI_RX_12)|
       (property &WUFE_SYS_SBI_TX_1)|(property &WUFE_SYS_SBI_TX_2)|(property &WUFE_SYS_SBI_TX_3)|
       (property &WUFE_SYS_SBI_TX_4)|(property &WUFE_SYS_SBI_TX_5)|(property &WUFE_SYS_SBI_TX_6)|
       (property &WUFE_SYS_SBI_TX_7)|(property &WUFE_SYS_SBI_TX_8)|(property &WUFE_SYS_SBI_TX_9)|
       (property&WUFE_SYS_SBI_TX_11)|(property&WUFE_SYS_SBI_TX_12))
   {

      WP_U16 gmr_mask =0;
      WP_U16 *gmr_ptr;
      WP_U32 ufeid = system_registry->wufe_id;
      WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

      /* PREPARE THE SBI GMR REGISTER MASK */
      if(system_registry->cfg.sbi.loopback & WUFE_SBI_LOOPBACK_SYSTEM)
         gmr_mask |= WUFEI_SBI_SYS_LOOP_M;

      if(system_registry->cfg.sbi.loopback &WUFE_SBI_LOOPBACK_LINE)
         gmr_mask |= WUFEI_SBI_LIN_LOOP_M;

      if(system_registry->cfg.sbi.rx_parity != WUFE_SBI_PARITY_NONE)
      {
         gmr_mask |= WUFEI_SBI_MODE_RX_PRTY_CHK_F(1);
         if(system_registry->cfg.sbi.rx_parity == WUFE_SBI_PARITY_EVEN)
            gmr_mask |= WUFEI_SBI_MODE_RX_PRTY_EVEN_F(1);
      }

      if(system_registry->cfg.sbi.tx_parity == WUFE_SBI_PARITY_EVEN)
         gmr_mask |= WUFEI_SBI_MODE_TX_PRTY_EVEN_F(1);

      if ((property & WUFE_SYS_SBI_RX) &&
          (system_registry->sys_enable.sbi_rx== WUFEI_SBI_ENABLE))
      {
         system_registry->sys_enable.sbi_rx = WUFEI_SBI_DISABLE;
         gmr_mask &= ~WUFEI_SBI_MODE_RX_ENABLE_M ;
      }

      if ((property & WUFE_SYS_SBI_TX) &&
          (system_registry->sys_enable.sbi_tx== WUFEI_SBI_ENABLE))
      {
         system_registry->sys_enable.sbi_rx = WUFEI_SBI_DISABLE;
         gmr_mask &= ~WUFEI_SBI_MODE_TX_ENABLE_M ;
      }

      /* MASK READY */
      gmr_ptr = &((WUFEI_hw_sbi*)sbi_base)->gmr.value;

      WUFEI_REG_SET(gmr_ptr, gmr_mask);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiSpeModeDisable
 *
 * Purpose: Disabless the SBI RX/TX spe mode registers configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *     coreid              UFE core identifier
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiSpeModeDisable(WUFEI_system_registry* system_registry,
                                      WP_U32 property, WP_U32 coreid)
{

   if ((property &WUFE_SYS_SBI_RX_1)|(property &WUFE_SYS_SBI_RX_2)|(property &WUFE_SYS_SBI_RX_3)|
       (property &WUFE_SYS_SBI_RX_4)|(property &WUFE_SYS_SBI_RX_5)|(property &WUFE_SYS_SBI_RX_6)|
       (property &WUFE_SYS_SBI_RX_7)|(property &WUFE_SYS_SBI_RX_8)|(property &WUFE_SYS_SBI_RX_9)|
       (property&WUFE_SYS_SBI_RX_11)|(property&WUFE_SYS_SBI_RX_12)|
       (property &WUFE_SYS_SBI_TX_1)|(property &WUFE_SYS_SBI_TX_2)|(property &WUFE_SYS_SBI_TX_3)|
       (property &WUFE_SYS_SBI_TX_4)|(property &WUFE_SYS_SBI_TX_5)|(property &WUFE_SYS_SBI_TX_6)|
       (property &WUFE_SYS_SBI_TX_7)|(property &WUFE_SYS_SBI_TX_8)|(property &WUFE_SYS_SBI_TX_9)|
       (property&WUFE_SYS_SBI_TX_11)|(property&WUFE_SYS_SBI_TX_12))
   {
      WP_U32 stm1, spe, ufeid = system_registry->wufe_id;
      WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
      WUFEI_hw_sbi_core* core_ptr;

      core_ptr = &((WUFEI_hw_sbi*)sbi_base)->core[coreid];

      for(stm1 = 0; stm1 < WUFE_MAX_N_STM1; stm1++)
      {
         for(spe = 0; spe < WUFE_MAX_N_SPE; spe++)
         {
            WP_U32 idx = WUFEI_HW_SBI_N_SPE(coreid,stm1,spe);
            WUFE_line_transf_type transf_type;

            transf_type = system_registry->cfg.sbi.transfer_type_spe[idx];

            if ((property & WUFEI_SYS_SBI_RX(spe)) &&
                (system_registry->sys_enable.sbi_spe_rx[idx] == WUFEI_SBI_ENABLE))
            {
               WP_U16* reg;
               WP_U16 value = WUFEI_SBI_TRIBUTARY_TYPE(transf_type);

               /* SBI RSPE mode registers */
               system_registry->sys_enable.sbi_spe_rx[idx] = WUFEI_SBI_DISABLE;
                reg = &(core_ptr->stm1_rx[stm1].value);
               value = ( WUFEI_SBI_SPE_X_TRANFER_TYPE_F(value, spe) |
                         WUFEI_SBI_SPE_X_ENABLE_F(0, spe));

               WUFEI_REG_SET(reg, value);
            }

            if ((property & WUFEI_SYS_SBI_TX(spe)) &&
                (system_registry->sys_enable.sbi_spe_tx[idx] == WUFEI_SBI_ENABLE))
            {
               WP_U16* reg;
               WP_U16 value = WUFEI_SBI_TRIBUTARY_TYPE(transf_type);

               /* SBI RSPE mode registers */
               system_registry->sys_enable.sbi_spe_tx[idx] = WUFEI_SBI_DISABLE;
               reg = &(core_ptr->stm1_tx[stm1].value);
               value = ( WUFEI_SBI_SPE_X_TRANFER_TYPE_F(value, spe) |
                         WUFEI_SBI_SPE_X_ENABLE_F(0, spe));

               WUFEI_REG_SET(reg, value);
            }

         }
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiGmrReconfigure
 *
 * Purpose: Reconfigures the SBI global mode register
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     config              new SBI configuration
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiGmrReconfigure(WUFEI_system_registry* system_registry,
                                      WUFE_sbi_global_reconfig *config)
{
   WP_U16 current_sbi_gmr;
   WP_U16 new_sbi_gmr, *gmr_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WUFE_status status;

   gmr_ptr = &((WUFEI_hw_sbi*)sbi_base)->gmr.value;

   WUFEI_REG_GET(gmr_ptr, &current_sbi_gmr);

   new_sbi_gmr = current_sbi_gmr;

   if (config->in_clock != WUFE_NO_CHANGE)
   {
      if (config->in_clock >= WUFE_SBI_CLK_INVALID)
      {
         WUFE_PRINT_ERR((wufei_log, "invalid SBI in_clock config\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }
      system_registry->cfg.sbi.in_clock = config->in_clock;
   }

   if (config->rx_parity != WUFE_NO_CHANGE)
   {
      WP_U16 tmp_value = 0;
      if (config->rx_parity >= WUFE_SBI_PARITY_INVALID)
      {
         WUFE_PRINT_ERR((wufei_log, "invalid SBI rx_parity config\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }
      if (config->rx_parity != WUFE_SBI_PARITY_NONE)
      {
         tmp_value |= WUFEI_SBI_MODE_RX_PRTY_CHK_F(1);
         if(config->rx_parity == WUFE_SBI_PARITY_EVEN)
            tmp_value |= WUFEI_SBI_MODE_RX_PRTY_EVEN_F(1);
      }
      WUFEI_REGISTER_CONFIG(new_sbi_gmr,
                            (WUFEI_SBI_MODE_RX_PRTY_CHK_M |
                             WUFEI_SBI_MODE_RX_PRTY_EVEN_M),
                            tmp_value);
      system_registry->cfg.sbi.rx_parity = config->rx_parity;
   }

   if (config->tx_parity != WUFE_NO_CHANGE)
   {
      WP_U16 tmp_value;
      if (config->tx_parity == WUFE_SBI_PARITY_NONE ||
          config->tx_parity >= WUFE_SBI_PARITY_INVALID)
      {
         WUFE_PRINT_ERR((wufei_log, "invalid SBI tx_parity config\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }
      tmp_value = (config->tx_parity == WUFE_SBI_PARITY_EVEN) ?
         WUFEI_SBI_MODE_TX_PRTY_EVEN_F(1) : 0;
      WUFEI_REGISTER_CONFIG(new_sbi_gmr,
                            WUFEI_SBI_MODE_TX_PRTY_EVEN_M,
                            tmp_value);
      system_registry->cfg.sbi.tx_parity = config->tx_parity;
   }

   if (config->loopback != WUFE_NO_CHANGE)
   {
      if (config->loopback >= WUFE_SBI_LOOPBACK_INVALID)
      {
         WUFE_PRINT_ERR((wufei_log, "invalid SBI loopback config\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }

      WUFEI_REGISTER_CONFIG(new_sbi_gmr,
                            (WUFEI_SBI_LIN_LOOP_M | WUFEI_SBI_SYS_LOOP_M),
                            0);

      if (~(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
         WUFEI_RETURN_IF_ERROR(status,WUFEI_Ac1fpDelaySet(ufeid, WUFEI_BS_AC1FP_DELAY_NONE));

      /* Loopback = SBI line Loopback*/
      if(config->loopback  ==  WUFE_SBI_LOOPBACK_LINE)
      {
         WUFEI_REGISTER_CONFIG(new_sbi_gmr,
                               WUFEI_SBI_LIN_LOOP_M,
                               (WUFEI_SBI_LIN_LOOP_F(1)));

         if (~(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
            WUFEI_RETURN_IF_ERROR(status,WUFEI_Ac1fpDelaySet(ufeid, WUFEI_BS_AC1FP_DELAY_2_CYCLE));
      }

      /* Loopback = SBI System Loopback*/
      else if(config->loopback  == WUFE_SBI_LOOPBACK_SYSTEM)
      {
         WUFEI_REGISTER_CONFIG(new_sbi_gmr,
                               WUFEI_SBI_SYS_LOOP_M,
                               WUFEI_SBI_SYS_LOOP_F(1));
      }

      system_registry->cfg.sbi.loopback = config->loopback;
   }

   if (new_sbi_gmr != current_sbi_gmr)
      WUFEI_REG_SET(gmr_ptr, new_sbi_gmr);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiSpeModeReconfigure
 *
 * Purpose: Reconfigures the SBI global mode register
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     config              new SBI configuration
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemSbiReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiSpeModeReconfigure(WP_U32 ufeid, WP_U32 coreid,
                                          WP_U32 stm1, WP_U32 spe,
                                          WUFE_line_transf_type spe_type)
{
   WP_U16 reg;
   WP_U16 value;
   WP_U16 *rx_spe_reg;
   WP_U16 *tx_spe_reg;
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   WUFEI_hw_sbi_core* core_ptr;

   core_ptr = &((WUFEI_hw_sbi*)sbi_base)->core[coreid];

   if (spe_type > WUFE_SBI_TYPE_IDLE || spe_type < WUFE_SBI_TYPE_T1 )
   {
      WUFE_PRINT_ERR((wufei_log, "invalid SBI transfer type config\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   rx_spe_reg = &(core_ptr->stm1_rx[stm1].value);
   tx_spe_reg = &(core_ptr->stm1_tx[stm1].value);

   if (spe_type != WUFE_SBI_TYPE_IDLE)
   {
      value = WUFEI_SBI_SPE_X_TRANFER_TYPE_F(WUFEI_SBI_TRIBUTARY_TYPE(spe_type), spe);

      /* Update SPE Rx register */
      WUFEI_REG_GET(rx_spe_reg, &reg);
      WUFEI_REGISTER_CONFIG(reg, WUFEI_SBI_SPE_X_TRANFER_TYPE_M(spe), value);
      WUFEI_REG_SET(rx_spe_reg, reg);

      WUFEI_REG_GET(tx_spe_reg, &reg);
      WUFEI_REGISTER_CONFIG(reg, WUFEI_SBI_SPE_X_TRANFER_TYPE_M(spe), value);
      WUFEI_REG_SET(tx_spe_reg, reg);
   }
   else
   {
      WUFEI_REG_SET(rx_spe_reg, WUFEI_SBI_SPE_X_ENABLE_F(0, spe));
      WUFEI_REG_SET(tx_spe_reg, WUFEI_SBI_SPE_X_ENABLE_F(0, spe));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiRxConfig
 *
 * Purpose:   configure the registery and HW for the lines type SBI
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (SBI RAM)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiRxConfig(WUFEI_system_registry *system_registry,
                                WUFEI_line_registry *line_registry,
                                WP_U8 enable)
{
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_rx_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U16 hw_sbi_mode, mf_mode;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

    /* multiframe mode*/
   if(line_registry->framed && line_registry->cas_enable)
   {
      mf_mode = WUFEI_LN_SBI_MF_CAS;
   }
   else
   {
      mf_mode = WUFEI_LN_SBI_MF_NONE;
   }

   hw_sbi_mode = ( WUFEI_LN_SBI_RX_ENABLE_F(enable) |
                   WUFEI_LN_SBI_RX_FRAMED_F(line_registry->framed) |
                   WUFEI_LN_SBI_RX_MF_MODE_F(mf_mode));

   if(line_registry->transfer_type == WUFE_SBI_TYPE_T1)
      hw_sbi_mode |= WUFEI_LN_SBI_RX_T1oE1_MODE_F(line_registry->t1oe1_mode);

   if (enable && (line_registry->e3_atm_g751_enabled[0] ==1))
   {
      hw_sbi_mode |= WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_M;
   }
   
   /* Rx tributary RAM status */
   wufe_hw_sbi_rx_ptr = WUFEI_INDIR_SBI_MODE_RX(line_id);

   WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));

   /* set the Rx mode */
   WUFEI_HwSbiIdirectDataSet(ufeid, hw_sbi_mode);

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiE3AtmConfig
 *
 * Purpose:   configure the SBI mode ram for E3 ATM
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (SBI RAM)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyCreate.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiE3AtmConfig(WP_U32 ufeid,
                                   WUFEI_line_registry *line_registry,
                                   WP_U32 direction)
{
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_rx_ptr;
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_tx_ptr;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U16 hw_sbi_mode;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   if (direction == WUFE_RX_DIRECTION)
   {
      /* Rx tributary RAM */
      wufe_hw_sbi_rx_ptr = WUFEI_INDIR_SBI_MODE_RX(line_id);

      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
      WUFEI_HwSbiIdirectDataGet(ufeid, &hw_sbi_mode);
      
      hw_sbi_mode |=  WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_M;
      
      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
      WUFEI_HwSbiIdirectDataSet(ufeid, hw_sbi_mode);

      line_registry->e3_atm_g751_enabled[0] =1;
   }

   if (direction == WUFE_TX_DIRECTION)
   {
      /* Tx tributary RAM */
      wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);

      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
      WUFEI_HwSbiIdirectDataGet(ufeid, &hw_sbi_mode);
      
      hw_sbi_mode |=  WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_M;
      
      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
      WUFEI_HwSbiIdirectDataSet(ufeid, hw_sbi_mode);

      line_registry->e3_atm_g751_enabled[1] =1;
   }
   
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiE3AtmClear
 *
 * Purpose:   Clear the SBI mode ram of the E3 ATM bit
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (SBI RAM)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyDelete.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiE3AtmClear(WP_U32 ufeid,
                                  WUFEI_line_registry *line_registry,
                                  WP_U32 direction)
{
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_rx_ptr;
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_tx_ptr;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U16 hw_sbi_mode;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   if (direction == WUFE_RX_DIRECTION)
   {
      /* Rx tributary RAM */
      wufe_hw_sbi_rx_ptr = WUFEI_INDIR_SBI_MODE_RX(line_id);

      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
      WUFEI_HwSbiIdirectDataGet(ufeid, &hw_sbi_mode);
      
      hw_sbi_mode &=  ~WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_M;
      
      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
      WUFEI_HwSbiIdirectDataSet(ufeid, hw_sbi_mode);

      line_registry->e3_atm_g751_enabled[0] =0;
   }

   if (direction == WUFE_TX_DIRECTION)
   {
      /* Tx tributary RAM */
      wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);

      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
      WUFEI_HwSbiIdirectDataGet(ufeid, &hw_sbi_mode);
      
      hw_sbi_mode &=  ~WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_M;
      
      WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
      WUFEI_HwSbiIdirectDataSet(ufeid, hw_sbi_mode);

      line_registry->e3_atm_g751_enabled[0] =0;
   }
   
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiTxConfig
 *
 * Purpose:   configure the registery and HW for the lines type SBI
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (SBI RAM)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiTxConfig(WUFEI_system_registry *system_registry,
                               WUFEI_line_registry *line_registry,
                               WP_U8 enable)
{
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_tx_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U16 hw_sbi_mode, mf_mode;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

    /* multiframe mode*/
   if(line_registry->framed && line_registry->cas_enable)
   {
      mf_mode = WUFEI_LN_SBI_MF_CAS;
   }
   else
   {
      mf_mode = WUFEI_LN_SBI_MF_NONE;
   }

   hw_sbi_mode = ( WUFEI_LN_SBI_TX_ENABLE_F(enable) |
                   WUFEI_LN_SBI_TX_FRAMED_F(line_registry->framed) |
                   WUFEI_LN_SBI_TX_MF_MODE_F(mf_mode));

   if(line_registry->transfer_type == WUFE_SBI_TYPE_E1 ||
      line_registry->transfer_type == WUFE_SBI_TYPE_T1)
      hw_sbi_mode |= WUFEI_LN_SBI_TX_CLK_MASTER_F(line_registry->tx_clk_master);

   if (line_registry->tx_clk_master &&
       (WUFEI_SystemClkRecHwConfigCtEn(system_registry) == WUFE_TRUE))
   {
      WUFE_status status;
      WP_U16 line_trib_id;
      
      if(enable)
      {
         if (line_registry->e3_atm_g751_enabled[1] == 1)
         {
            hw_sbi_mode |= WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_M;
         }
         
      /* clock translators are initialized during WUFE_LineEnable()*/
         line_trib_id = WUFEI_HwSbiClockTranslatorTribIdGet(coreid,
                                                                   line_registry->stm1_id,
                                                                   line_registry->spe_id,
                                                                   line_registry->sub_spe_entry);
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockTranslatorInit(system_registry,
                                                         line_registry->line_id,
                                                         line_registry->clock_translator_id,
                                                         line_trib_id,
                                                         line_registry->framed,
                                                         line_registry->transfer_type));
      }

      hw_sbi_mode |= WUFEI_LN_SBI_TX_CLKT_SEL_F(line_registry->clock_translator_id);

   }

   /* Tx tributary RAM status */
   wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);

   WUFEI_HwSbiIdirectAddrSet(ufeid, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));

   /* set the Tx mode */
   WUFEI_HwSbiIdirectDataSet(ufeid, hw_sbi_mode);

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiRamConfig
 *
 * Purpose:   configure the registery and HW for the lines type SBI
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (SBI RAM)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiRamConfig( WUFEI_system_registry *system_registry,
                                  WUFEI_line_registry   *line_registry)
{
   WUFE_status status;
   WP_U32 id = WUFEI_ID(system_registry->wufe_id, line_registry->ufe_core_id);

   /********************************/
   /*** clear SBI trib cas ram     */
   /********************************/
   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_CAS_RX(line_registry->line_id),
                            0xfefe,
                            sizeof(WUFEI_hw_sbi_cas_ram)/2);

   WUFEI_HwSbiIdirectMemset(id,
                            (WP_U32)WUFEI_INDIR_SBI_CAS_TX(line_registry->line_id),
                            0xffff,
                            sizeof(WUFEI_hw_sbi_cas_ram)/2);

   /***********************************/
   /*** configure rx/tx mod registers **/
   /***********************************/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwSbiRxConfig(system_registry, line_registry, WUFE_DISABLE));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwSbiTxConfig(system_registry, line_registry, WUFE_DISABLE));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiRxDisable
 *
 * Purpose:   configure the registery and HW for the lines type SBI
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (SBI RAM)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFE_HwSbiRxDisable(WUFEI_line_registry *line_registry,
                                WP_U32 wufe_id)
{
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_rx_ptr;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   wufe_hw_sbi_rx_ptr = WUFEI_INDIR_SBI_MODE_RX(line_id);

   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));

   /* set the Rx mode */
   WUFEI_HwSbiIdirectDataSet(wufe_id, 0);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiTxDisable
 *
 * Purpose:   configure the registery and HW for the lines type SBI
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (SBI RAM)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFE_HwSbiTxDisable( WUFEI_line_registry *line_registry,
                                 WP_U32 wufe_id)
{
   WUFEI_hw_sbi_mode_ram  *wufe_hw_sbi_tx_ptr;
   WP_U32 coreid =  line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);

   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));

   /* set the Tx mode */
   WUFEI_HwSbiIdirectDataSet(wufe_id, 0);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:   WUFEI_SbiStatusRamRxCheck
 *
 * Purpose:   Check if the SBI state RAM is changing for RX
 *
 * Description:
 *     If the SBI RAM is diffrent between multiple reads the function returns WP_OK.
 *     if all the reads return the same value return error.
 *
 *
 * Inputs:
 *     wufe_id - wufe_id
 *     core_id - the core id
 *     line_id - the line id
 *     line_enable - 0 line disable, 1 line enable
 *
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_LINE_MALFUNCTION 1) in case the Line is enabled but state machine do not progress
 *                               2) in case the Line is disabled but state machine progress
 *
 * Called by:
 *    WUFEI_HwSbiLineDisplayE1T1Rx() function
 *
 ****************************************************************************/
WUFE_status WUFEI_SbiStatusRamRxCheck(WP_U32 wufe_id,WP_U32 core_id,WP_U32 line_id,WP_U32 line_enable)
{
  WUFEI_hw_sbi_rx_state_ram  wufe_hw_sbi_state_rx;
  WUFEI_hw_sbi_rx_state_ram *rx_state_ptr;
  WP_U32 value_rx_one,trib_value_check;
  WP_U32 i, trib_alive=WP_FALSE;

  /* get the address of the status ram of the specific line */
  rx_state_ptr = WUFEI_INDIR_SBI_STATE_RX(line_id);

  /* Get the SBI ram value the first time */
  WUFEI_HwSbiIdirectAddrSet(wufe_id, core_id, (WP_U32)&(rx_state_ptr->dummy0));
  WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_state_rx.dummy0);
  WUFEI_HwSbiIdirectAddrSet(wufe_id, core_id, (WP_U32)&(rx_state_ptr->dummy1));
  WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_state_rx.dummy1);
  value_rx_one = (wufe_hw_sbi_state_rx.dummy0 << 16 | wufe_hw_sbi_state_rx.dummy1);

  /* read the tributary */
  for (i=0; i<=WUFEI_SBI_RAM_CHECK_ALIVE_MAX_SAMPLE; i++)
  {
     /* Reread sbi status ram again*/
     WUFEI_HwSbiIdirectAddrSet(wufe_id, core_id, (WP_U32)&(rx_state_ptr->dummy0));
     WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_state_rx.dummy0);
     WUFEI_HwSbiIdirectAddrSet(wufe_id, core_id, (WP_U32)&(rx_state_ptr->dummy1));
     WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_state_rx.dummy1);
     trib_value_check = (wufe_hw_sbi_state_rx.dummy0 << 16 | wufe_hw_sbi_state_rx.dummy1);
     printf("Performing line state RAM check : \n");
     printf("Iteration:%d ,trib_value_check 0x%08x , value_rx_one 0x%8x\n",
            i,
            trib_value_check,
            value_rx_one);

     if (value_rx_one != trib_value_check)
     {
        /* THE SBI is progressing - break*/
        trib_alive=WP_TRUE;
        break;
     }
  }

  if (WP_TRUE == trib_alive && line_enable)
  {/* the above for loop got to the "break" code*/
     WUFE_PRINT_DISP((wufei_log,"SBI line Rx is active\n"));
  }
  else if (WP_FALSE == trib_alive && !line_enable)
  {
     WUFE_PRINT_DISP((wufei_log,"SBI line Rx is not active\n"));
  }
  else if ((WP_FALSE == trib_alive && line_enable) ||
           (WP_TRUE  == trib_alive && !line_enable))
  {
     WUFE_PRINT_DISP((wufei_log,
                      "WARNING: SBI line Rx is suspected of being malfunctioning \n"));
     return   WUFE_ERR_LINE_MALFUNCTION;
  }

  return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WUFEI_SbiStatusRamTxCheck
 *
 * Purpose:   Check if the SBI state RAM is changing for TX
 *
 * Description:
 *     If the SBI RAM is diffrent between multiple reads the function return WP_OK.
 *     if all the reads return the same value return error.
 *
 *
 * Inputs:
 *     wufe_id - wufe_id
 *     core_id - the core id
 *     line_id - the line id
 *     line_enable - 0 line disable, 1 line enable
 *
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_LINE_MALFUNCTION 1) in case the Line is enabled but state machine do not progress
 *                               2) in case the Line is disabled but state machine progress
 *
 * Called by:
 *    WUFEI_HwSbiLineDisplayE1T1Tx() function
 *
 ****************************************************************************/
WUFE_status WUFEI_SbiStatusRamTxCheck(WP_U32 wufe_id,WP_U32 core_id,WP_U32 line_id,WP_U32 line_enable)
{
  WUFEI_hw_sbi_tx_state_ram  wufe_hw_sbi_state_tx;
  WUFEI_hw_sbi_tx_state_ram *tx_state_ptr;
  WP_U16 value_tx_one, trib_value_check;
  WP_U32 i, trib_alive=WP_FALSE;

  /* get the address of the status ram of the specific line */
  tx_state_ptr = WUFEI_INDIR_SBI_STATE_TX (line_id);

  /* Get the SBI ram value the first time */
  WUFEI_HwSbiIdirectAddrSet(wufe_id, core_id, (WP_U32)&(tx_state_ptr->dummy));
  WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_state_tx.dummy);
  value_tx_one = wufe_hw_sbi_state_tx.dummy;

  /* read the tributary */
  for (i=0; i<=WUFEI_SBI_RAM_CHECK_ALIVE_MAX_SAMPLE; i++)
  {
     /* Reread sbi atatus ram again*/
     WUFEI_HwSbiIdirectAddrSet(wufe_id, core_id, (WP_U32)&(tx_state_ptr->dummy));
     WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_state_tx.dummy);
     trib_value_check = wufe_hw_sbi_state_tx.dummy;
     printf("Performing line state RAM check : \n");
     printf(" (WP_U32)&(tx_state_ptr->dummy) 0x%08X %d trib_value_check 0x%04x  value_tx_one0x%4x\n",
            (WP_U32)&(tx_state_ptr->dummy),
            i,
            trib_value_check,
            value_tx_one
        );

     if (value_tx_one != trib_value_check)
     {
        /* THE SBI is progressing - break*/
        trib_alive=WP_TRUE;
        break;
     }
  }

  if (WP_TRUE == trib_alive && line_enable)
  {/* the above for loop got to the "break" code*/
     WUFE_PRINT_DISP((wufei_log,"SBI line Tx is active\n"));
  }
  else if (WP_FALSE == trib_alive && !line_enable)
  {
     WUFE_PRINT_DISP((wufei_log,"SBI line Tx is not active\n"));
  }
  else if ((WP_FALSE == trib_alive && line_enable) ||
           (WP_TRUE  == trib_alive && !line_enable))
  {
     WUFE_PRINT_DISP((wufei_log,
                      "WARNING: SBI line Tx is suspected of being malfunctioning \n"));
     return   WUFE_ERR_LINE_MALFUNCTION;
  }

  return WP_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiLineDisplayE1T1Rx
 *
 * Purpose:   Displays the SBI line structures and HW registers for
 *            E1T1 line in Rx direction
 *
 *
 * Description:
 *     This function displays the SBI line structures and HW
 *
 *
 * Inputs:
 *     line_registry - UFE line registry
 *     wufe_id - wufe_id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_LineSbiDisplay() function
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiLineDisplayE1T1Rx(WUFEI_line_registry *line_registry, WP_U32 wufe_id)
{
   WUFEI_hw_sbi_mode_ram  *wufe_hw_sbi_rx_ptr;
   WUFEI_hw_sbi_mode_ram  wufe_hw_sbi_rx_local;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);
   WUFE_status status;

   wufe_hw_sbi_rx_ptr = WUFEI_INDIR_SBI_MODE_RX(line_id);

   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_rx_local.mode);

   WUFE_PRINT_DISP((wufei_log, "\n Line SBI RX config:\n------------------\n"));
   WUFE_PRINT_DISP((wufei_log, "SBI Rx RAM (mode: 0x%04x)\n", wufe_hw_sbi_rx_local.mode));

   WUFE_PRINT_DISP((wufei_log,
                    "\tRX Enable:           %d\n",
                    WUFEI_LN_SBI_RX_ENABLE_V(wufe_hw_sbi_rx_local.mode)));
   WUFE_PRINT_DISP((wufei_log,
                    "\tRX Framed:           %d\n",
                    WUFEI_LN_SBI_RX_FRAMED_V(wufe_hw_sbi_rx_local.mode)));
   WUFE_PRINT_DISP((wufei_log,
                    "\tMF mode:             %d\n",
                    WUFEI_LN_SBI_RX_MF_MODE_V(wufe_hw_sbi_rx_local.mode)));
   WUFE_PRINT_DISP((wufei_log,
                    "\tT1 over E1 mode:     %d\n",
                    WUFEI_LN_SBI_RX_T1oE1_MODE_V(wufe_hw_sbi_rx_local.mode)));

#if WUFE_DEBUG_VERIFY_WRITE
   /* check if the SBI has a chaning values */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_SbiStatusRamRxCheck(wufe_id,
                                                   coreid,
                                                   line_id,
                                                   WUFEI_LN_SBI_RX_ENABLE_V(wufe_hw_sbi_rx_local.mode)));

#endif

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiLineDisplayE1T1Tx
 *
 * Purpose:   Displays the SBI line structures and HW registers for
 *            E1T1 line in Rx direction
 *
 *
 * Description:
 *     This function displays the SBI line structures and HW
 *
 *
 * Inputs:
 *     line_registry - UFE line registry
 *     wufe_id - wufe_id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_LineSbiDisplay() function
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiLineDisplayE1T1Tx(WUFEI_line_registry *line_registry, WP_U32 wufe_id)
{
   WUFEI_hw_sbi_mode_ram  *wufe_hw_sbi_tx_ptr;
   WUFEI_hw_sbi_mode_ram  wufe_hw_sbi_tx_local;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);
   WUFE_status status;

   /*SBI TX tributary status RAM*/
   wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);
   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_tx_local.mode);

   WUFE_PRINT_DISP((wufei_log, "\n Line SBI TX config:\n------------------\n"));
   WUFE_PRINT_DISP((wufei_log, "SBI Tx RAM (mode: 0x%04x)\n", wufe_hw_sbi_tx_local.mode));

   WUFE_PRINT_DISP((wufei_log,
                    "\tTX Enable:           %d\n",
                    WUFEI_LN_SBI_TX_ENABLE_V(wufe_hw_sbi_tx_local.mode)));

   WUFE_PRINT_DISP((wufei_log,
                    "\tTX Framed:           %d\n",
                    WUFEI_LN_SBI_TX_FRAMED_V(wufe_hw_sbi_tx_local.mode)));
   WUFE_PRINT_DISP((wufei_log,
                    "\tMF mode:             %d\n",
                    WUFEI_LN_SBI_TX_MF_MODE_V(wufe_hw_sbi_tx_local.mode)));
   WUFE_PRINT_DISP((wufei_log,
                    "\tT1 over E1 mode:     %d\n",
                    WUFEI_LN_SBI_TX_T1oE1_MODE_V(wufe_hw_sbi_tx_local.mode)));

   if(WUFEI_LN_SBI_TX_CLK_MASTER_V(wufe_hw_sbi_tx_local.mode))
      WUFE_PRINT_DISP((wufei_log, "SBI Trib Clock:   MASTER\n"));
   else
      WUFE_PRINT_DISP((wufei_log, "SBI Trib Clock:   SLAVE\n"));

   WUFE_PRINT_DISP((wufei_log, "SBI clock translator id: %d\n",
                    WUFEI_LN_SBI_TX_CLKT_SEL_V(wufe_hw_sbi_tx_local.mode)));

#if WUFE_DEBUG_VERIFY_WRITE
   /* check if the SBI has a chaning values */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_SbiStatusRamTxCheck(wufe_id,
                                                   coreid,
                                                   line_id,
                                                   WUFEI_LN_SBI_TX_ENABLE_V(wufe_hw_sbi_tx_local.mode)));
#endif

   return WUFE_OK;

}


WUFE_status WUFEI_HwSbiSlotTxCasDisplay(WUFEI_line_registry *line_registry, WP_U32 slot, WP_U32 wufe_id)
{
   WUFEI_hw_sbi_cas_ram  *wufe_hw_sbi_ptr;
   WP_U16 value;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   /*SBI TX tributary CAS RAM*/
   wufe_hw_sbi_ptr = WUFEI_INDIR_SBI_CAS_TX(line_id*WUFEI_SBI_CAS_RAM_ENTRIES + (slot/(WUFE_N_SLOTS_E1/WUFEI_SBI_CAS_RAM_ENTRIES)));
   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_ptr->cas_ram[0]));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &value);

   value &= (WUFEI_SBI_CAS_RAM_SLOT_MASK << (WUFEI_SBI_CAS_RAM_SLOT_SIZE * (slot % (WUFE_N_SLOTS_E1/WUFEI_SBI_CAS_RAM_ENTRIES))));
   value >>= (WUFEI_SBI_CAS_RAM_SLOT_SIZE * (slot % (WUFE_N_SLOTS_E1/WUFEI_SBI_CAS_RAM_ENTRIES)));
   WUFE_PRINT_DISP((wufei_log,"%2d: 0x%04x\t", slot, value));

   return WUFE_OK;
}

WUFE_status WUFEI_HwSbiSlotRxCasDisplay(WUFEI_line_registry *line_registry, WP_U32 slot, WP_U32 wufe_id)
{
   WUFEI_hw_sbi_cas_ram  *wufe_hw_sbi_ptr;
   WP_U16 value;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   /*SBI RX tributary CAS RAM*/
   wufe_hw_sbi_ptr = WUFEI_INDIR_SBI_CAS_RX(line_id*WUFEI_SBI_CAS_RAM_ENTRIES + (slot/(WUFE_N_SLOTS_E1/WUFEI_SBI_CAS_RAM_ENTRIES)));
   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_ptr->cas_ram[0]));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &value);

   value &= (WUFEI_SBI_CAS_RAM_SLOT_MASK << (WUFEI_SBI_CAS_RAM_SLOT_SIZE * (slot % (WUFE_N_SLOTS_E1/WUFEI_SBI_CAS_RAM_ENTRIES))));
   value >>= (WUFEI_SBI_CAS_RAM_SLOT_SIZE * (slot % (WUFE_N_SLOTS_E1/WUFEI_SBI_CAS_RAM_ENTRIES)));
   WUFE_PRINT_DISP((wufei_log,"%2d: 0x%04x\t", slot, value));

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiLineDisplayDS3E3Rx
 *
 * Purpose:   Displays the SBI line structures and HW registers for
 *            E1T1 line in Rx direction
 *
 *
 * Description:
 *     This function displays the SBI line structures and HW
 *
 *
 * Inputs:
 *     line_registry - UFE line registry
 *     wufe_id - wufe_id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_LineSbiDisplay() function
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiLineDisplayDS3E3Rx(WUFEI_line_registry *line_registry, WP_U32 wufe_id)
{
   WUFEI_hw_sbi_mode_ram  *wufe_hw_sbi_rx_ptr;
   WUFEI_hw_sbi_mode_ram  wufe_hw_sbi_rx_local;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   wufe_hw_sbi_rx_ptr = WUFEI_INDIR_SBI_MODE_RX(line_id);
   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32) &(wufe_hw_sbi_rx_ptr->mode));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_rx_local.mode);

   WUFE_PRINT_DISP((wufei_log, "\n Line SBI RX config:\n------------------\n"));
   WUFE_PRINT_DISP((wufei_log, "SBI Rx RAM (mode: 0x%04x)\n",wufe_hw_sbi_rx_local.mode));

   WUFE_PRINT_DISP((wufei_log,
                    "\tRX Enable:           %d\n",
                    WUFEI_LN_SBI_RX_ENABLE_V(wufe_hw_sbi_rx_local.mode)));

   WUFE_PRINT_DISP((wufei_log,
                    "\tRX Framed:           %d\n",
                    WUFEI_LN_SBI_RX_FRAMED_V(wufe_hw_sbi_rx_local.mode)));

   if (line_registry->transfer_type == WUFE_SBI_TYPE_E3)
      WUFE_PRINT_DISP((wufei_log,
                       "\tE3 ATM G751:         %d\n",
                       WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_V(wufe_hw_sbi_rx_local.mode)));
   
   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiLineDisplayDS3E3Tx
 *
 * Purpose:   Displays the SBI line structures and HW registers for
 *            E1T1 line in Rx direction
 *
 *
 * Description:
 *     This function displays the SBI line structures and HW
 *
 *
 * Inputs:
 *     line_registry - UFE line registry
 *     wufe_id - wufe_id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_LineSbiDisplay() function
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiLineDisplayDS3E3Tx(WUFEI_line_registry *line_registry, WP_U32 wufe_id)
{
   WUFEI_hw_sbi_mode_ram  *wufe_hw_sbi_tx_ptr;
   WUFEI_hw_sbi_mode_ram  wufe_hw_sbi_tx_local;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   /*SBI TX tributary status RAM*/
   wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);
   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &wufe_hw_sbi_tx_local.mode);

   WUFE_PRINT_DISP((wufei_log, "\n Line SBI TX config:\n------------------\n"));
   WUFE_PRINT_DISP((wufei_log, "SBI Tx RAM (mode: 0x%04x)\n", wufe_hw_sbi_tx_local.mode));

   WUFE_PRINT_DISP((wufei_log,
                    "\tTX Enable:           %d\n",
                    WUFEI_LN_SBI_TX_ENABLE_V(wufe_hw_sbi_tx_local.mode)));

   WUFE_PRINT_DISP((wufei_log,
                    "\tTX Framed:           %d\n",
                    WUFEI_LN_SBI_TX_FRAMED_V(wufe_hw_sbi_tx_local.mode)));

   if (line_registry->transfer_type == WUFE_SBI_TYPE_E3)
      WUFE_PRINT_DISP((wufei_log,
                       "\tE3 ATM G751:         %d\n",
                       WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_V(wufe_hw_sbi_tx_local.mode)));

   if(WUFEI_LN_SBI_TX_CLK_MASTER_V(wufe_hw_sbi_tx_local.mode))
      WUFE_PRINT_DISP((wufei_log, "SBI Trib Clock:   MASTER\n"));
   else
      WUFE_PRINT_DISP((wufei_log, "SBI Trib Clock:   SLAVE\n"));

   WUFE_PRINT_DISP((wufei_log, "SBI clock translator id: %d\n",
                    WUFEI_LN_SBI_TX_CLKT_SEL_V(wufe_hw_sbi_tx_local.mode)));

   return WUFE_OK;

}
/*****************************************************************************
 *
 * Function: WUFEI_HwSbiLineReconfigure
 *
 * Purpose:   Displays the SBI line structures and HW registers for
 *            E1T1 line in Rx direction
 *
 *
 * Description:
 *     This function displays the SBI line structures and HW
 *
 *
 * Inputs:
 *     line_registry - UFE line registry
 *     wufe_id - wufe_id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_LineSbiDisplay() function
 *
 ****************************************************************************/
WUFE_status WUFEI_HwSbiLineReconfigure(WP_U32 wufe_id,
                                       WUFEI_system_registry *system_registry,
                                       WUFEI_line_registry *line_registry,
                                       WUFE_line_sbi_reconfig *config)
{
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_rx_ptr;
   WUFEI_hw_sbi_mode_ram *wufe_hw_sbi_tx_ptr;
   WP_U16 mode_rx, mode_tx, new_mode_rx, new_mode_tx;
   WP_U8 update_reg, update_done_reg;
   WP_U32 sbi_ct_system_valid;
   WUFE_status status;
   WP_U32 coreid = line_registry->ufe_core_id;
   WP_U32 line_id = WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_registry->line_id, coreid);

   wufe_hw_sbi_rx_ptr = WUFEI_INDIR_SBI_MODE_RX(line_id);
   wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);

   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &mode_rx);
   WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
   WUFEI_HwSbiIdirectDataGet(wufe_id, &mode_tx);

   new_mode_rx = mode_rx;
   new_mode_tx = mode_tx;
   update_reg = 0;

   if ( (config->cas_enable != WUFE_NO_CHANGE) &&
        (config->cas_enable != line_registry->cas_enable) )
   {
      WP_U16 mf_mode;

      if (config->cas_enable > 1)
      {
         WUFE_PRINT_ERR((wufei_log, "\ninvalid cas_enable %d \n", config->cas_enable));
         return WUFE_ERR_INVALID_CONFIG;
      }

      if(config->cas_enable)
         mf_mode = WUFEI_LN_SBI_MF_CAS;
      else
         mf_mode = WUFEI_LN_SBI_MF_NONE;

      WUFEI_REGISTER_CONFIG(new_mode_rx,
                            WUFEI_LN_SBI_RX_MF_MODE_M,
                            WUFEI_LN_SBI_RX_MF_MODE_F(mf_mode));

      WUFEI_REGISTER_CONFIG(new_mode_tx,
                            WUFEI_LN_SBI_TX_MF_MODE_M,
                            WUFEI_LN_SBI_TX_MF_MODE_F(mf_mode));

      line_registry->cas_enable = config->cas_enable;
   }

   if ( (config->t1oe1_mode != (WP_U8)WUFE_NO_CHANGE) &&
        (config->t1oe1_mode != line_registry->t1oe1_mode) )
   {
      if ((config->t1oe1_mode > 1) ||
          (line_registry->transfer_type != WUFE_SBI_TYPE_T1))
      {
         WUFE_PRINT_ERR((wufei_log, "\ninvalid t1oe1_mode %d \n", config->t1oe1_mode));
         return WUFE_ERR_INVALID_CONFIG;
      }

      WUFEI_REGISTER_CONFIG(new_mode_rx,
                            WUFEI_LN_SBI_RX_T1oE1_MODE_M,
                            WUFEI_LN_SBI_RX_T1oE1_MODE_F(config->t1oe1_mode));

      line_registry->t1oe1_mode = config->t1oe1_mode;

   }

   /* while modifying the tx_clk_master and clock_translator_id their correct values
    * mast be checked: the clock_translator_id can be modified only when the
    * tx_clk_master is MASTER and SBI clock translator mode is enabled in the FPGA.
    * In addition, the tx_clk_master can't be changed from SLAVE to MASTER mode without
    * specifying the clock_translator_id if the clock translator mode is enabled
    * in the FPGA.*/
   /* update_reg holds 0x1 for tx_clk_master modify and 0x2 for clock_translator_id
    * modify. The update_done_reg clears the corresponding bit when modifiction
    * of the specific parameter has been completed. If update_reg > 0 and update_done_reg == 0
    * the modification was completed successfully. Otherwise, the modification failed.
    */
   if ((config->tx_clk_master != WUFE_NO_CHANGE) &&
       (config->tx_clk_master != line_registry->tx_clk_master))
      update_reg |= 0x1;
   if (config->clock_translator_id != (WP_U16)WUFE_NO_CHANGE)
      update_reg |= 0x2;
   update_done_reg = update_reg;

   sbi_ct_system_valid = WUFEI_SystemClkRecHwConfigCtEn(system_registry);

   if ((update_reg == 1 && line_registry->tx_clk_master == WUFE_LINE_CLOCK_MASTER) ||
       (update_reg == 1 && line_registry->tx_clk_master == WUFE_LINE_CLOCK_SLAVE &&
        (sbi_ct_system_valid == WUFE_FALSE)) ||
       (update_reg == 3 && line_registry->tx_clk_master == WUFE_LINE_CLOCK_SLAVE))
   {
      /* chagne the tx_clk_master */
      if (config->tx_clk_master > 1)
      {
         WUFE_PRINT_ERR((wufei_log, "\ninvalid tx_clk_master %d \n", config->tx_clk_master));
         return WUFE_ERR_INVALID_CONFIG;
      }

      WUFEI_REGISTER_CONFIG(new_mode_tx,
                            (WUFEI_LN_SBI_TX_CLK_MASTER_M),
                            (WUFEI_LN_SBI_TX_CLK_MASTER_F(config->tx_clk_master)));
      update_done_reg &= ~0x1;

      /* when the clock has changed from master to slave remove it from the
         SBI CT register list */
      if (config->tx_clk_master == WUFE_LINE_CLOCK_SLAVE &&
          line_registry->tx_clk_master == WUFE_LINE_CLOCK_MASTER &&
          sbi_ct_system_valid == WUFE_TRUE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_SbiClockTranslatorRelease(
                                  system_registry,
                                  line_registry->line_id,
                                  line_registry->clock_translator_id));
      }

   }
   if ((sbi_ct_system_valid == WUFE_TRUE) &&
       ((update_reg == 2 && line_registry->tx_clk_master == WUFE_LINE_CLOCK_MASTER) ||
        (update_reg == 3 && line_registry->tx_clk_master == WUFE_LINE_CLOCK_SLAVE)))
   {
      if (config->clock_translator_id != line_registry->clock_translator_id)
      {
         /* chagne the clock_translator_id */
         WP_U16 tmp_mode_tx;
         WP_U16 line_trib_id;

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_SbiClockTranslatorValidate(system_registry,
                                                                config->clock_translator_id,
                                                                line_registry->framed,
                                                                line_registry->transfer_type));

         /* if the previously the line was configured as master with
          * some clock translator ID, the old clock translator must be cleared
          * and the new one configured */
         if (line_registry->tx_clk_master == WUFE_LINE_CLOCK_MASTER)
         {
            /* remove this line from the prev CT and insert to the new CT */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_SbiClockTranslatorRelease(
                                     system_registry,
                                     line_registry->line_id,
                                     line_registry->clock_translator_id));
         }

         line_trib_id = WUFEI_HwSbiClockTranslatorTribIdGet(coreid,
                                                            line_registry->stm1_id,
                                                            line_registry->spe_id,
                                                            line_registry->sub_spe_entry);
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockTranslatorInit(system_registry,
                                                         line_registry->line_id,
                                                         config->clock_translator_id,
                                                         line_trib_id,
                                                         line_registry->framed,
                                                         line_registry->transfer_type));

         /* update the SBI Tx registers point to the new CT */
         wufe_hw_sbi_tx_ptr = WUFEI_INDIR_SBI_MODE_TX(line_id);
         WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
         WUFEI_HwSbiIdirectDataGet(wufe_id, &tmp_mode_tx);
         WUFEI_REGISTER_CONFIG(tmp_mode_tx,
                               WUFEI_LN_SBI_TX_CLKT_SEL_M,
                               WUFEI_LN_SBI_TX_CLKT_SEL_F(config->clock_translator_id));
         WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
         WUFEI_HwSbiIdirectDataSet(wufe_id, tmp_mode_tx);
      }
      update_done_reg &= ~0x2;
   }

   if (update_done_reg != 0)
      return WUFE_ERR_LINE_RECONFIG;
   else
   {
      if (update_reg & 0x1)
         line_registry->tx_clk_master = config->tx_clk_master;
      if (update_reg & 0x2)
         line_registry->clock_translator_id = config->clock_translator_id;
   }

   if (new_mode_rx != mode_rx)
   {
      /* in order to update the register first disable
         the line and reenable it after that */
      new_mode_rx &= ~WUFEI_LN_SBI_RX_ENABLE_M;
      WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
      WUFEI_HwSbiIdirectDataGet(wufe_id, &new_mode_rx);
      new_mode_rx |= WUFEI_LN_SBI_RX_ENABLE_M;
      WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_rx_ptr->mode));
      WUFEI_HwSbiIdirectDataSet(wufe_id, new_mode_rx);
   }
   if (new_mode_tx != mode_tx)
   {
      /* in order to update the register first disable
         the line and reenable it after that */
      new_mode_tx &= ~WUFEI_LN_SBI_TX_ENABLE_M;
      WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
      WUFEI_HwSbiIdirectDataGet(wufe_id, &new_mode_tx);
      new_mode_tx |= WUFEI_LN_SBI_TX_ENABLE_M;
      WUFEI_HwSbiIdirectAddrSet(wufe_id, coreid, (WP_U32)&(wufe_hw_sbi_tx_ptr->mode));
      WUFEI_HwSbiIdirectDataSet(wufe_id, new_mode_tx);

   }

   if (config->clock_rec_mode != WUFE_NO_CHANGE)
   {
      if (config->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY ||
          line_registry->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY)
         return WUFE_ERR_NOT_IMPLEMENTED;

      /* if the Tx clock recovery wasn't set, init it */
      if (((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) == 0) &&
          (config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX))
      {
         WUFEI_RETURN_IF_ERROR(
            status,
            WUFEI_ClockRecTxInterfaceIdValidate(system_registry,
                                                line_registry->transfer_type,
                                                line_registry->clock_translator_id));

         line_registry->tx_clock_rec_if_id = line_registry->clock_translator_id;
         line_registry->clock_rec_mode |= WUFE_LINE_CLOCK_REC_ENABLE_TX;
      }

      /* if the Tx clock recovery was set and now it shouldn't be set, clear it */
      if ( (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) &&
           ((config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) == 0) )
      {
         /*all PHYs will be turned clock recovery mode none;
           if there is a master on that line the clock recovery interface
           will be disabled*/
         WUFEI_RETURN_IF_ERROR(
            status,
            WUFEI_LineSetPhyClockRecTxDelete(line_registry->line_id,
                                             system_registry));

         line_registry->tx_clock_rec_if_id = 0xffff;
         line_registry->clock_rec_mode &= ~WUFE_LINE_CLOCK_REC_ENABLE_TX;
      }

      /* if the Rx clock recovery wasn't set, init it */
      if (((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX) == 0) &&
          (config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX))
      {
         if (line_registry->state_mode == WUFEI_LINE_ENABLE)
            return WUFE_ERR_LINE_NOT_DISABLED;

         if (config->rx_clk_rec_if == 0xffff)
            return WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID;

         line_registry->rx_clock_rec_if_id = config->rx_clk_rec_if;

         /* Obtain the rx clock recovery interface id */
         WUFEI_RETURN_IF_ERROR(
            status,
            WUFEI_ClockRecRxInterfaceCreate(system_registry,line_registry));

         line_registry->clock_rec_mode |= WUFE_LINE_CLOCK_REC_ENABLE_RX;
      }

      /* if the Rx clock recovery was set */
      if (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX)
      {
         if (line_registry->state_mode == WUFEI_LINE_ENABLE)
            return WUFE_ERR_LINE_NOT_DISABLED;

         if ((config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX) == 0)
         {
            /*  and now it shouldn't be set, clear it */
            /* release rx clock recovery interface id allocated during line create */
            WUFEI_RETURN_IF_ERROR(
               status,
               WUFEI_LineSetPhyClockRecRxDelete(line_registry->line_id,
                                                system_registry));

            WUFEI_RETURN_IF_ERROR(
               status,
               WUFEI_ClockRecRxInterfaceDelete(system_registry,
                                                line_registry->rx_clock_rec_if_id,
                                                line_registry->line_id));

            line_registry->rx_clock_rec_if_id = 0xffff;
            line_registry->clock_rec_mode &= ~WUFE_LINE_CLOCK_REC_ENABLE_RX;
         }
         else
         {
            /* the new clock recovery is also enabled in rx direction, replace the new
             * rx_clk_rec_if */
            if (config->rx_clk_rec_if != 0xffff &&
                config->rx_clk_rec_if != line_registry->rx_clock_rec_if_id)
            {
               /* release the old rx_clk_rec_if_id */
               WUFEI_RETURN_IF_ERROR(
                  status,
                  WUFEI_ClockRecRxInterfaceDelete(system_registry,
                                                  line_registry->rx_clock_rec_if_id,
                                                  line_registry->line_id));

               line_registry->rx_clock_rec_if_id = config->rx_clk_rec_if;

               /* set a new rx_clk_rec_if_id */
               WUFEI_RETURN_IF_ERROR(
                  status,
                  WUFEI_ClockRecRxInterfaceCreate(system_registry, line_registry));

            }
         }
      }
   }

   return WUFE_OK;
}

void WUFEI_HwSbiGmrDisplay(WP_U32 sbi_base)
{
   WP_U16 gmr_mask, *gmr_ptr;

   gmr_ptr = &((WUFEI_hw_sbi*)sbi_base)->gmr.value;

   WUFEI_REG_GET(gmr_ptr, &gmr_mask);

   WUFE_PRINT_DISP((wufei_log, "SBI GMR: 0x%04x\n", gmr_mask));
   WUFE_PRINT_DISP((wufei_log, "\tSystem loopback: 0x%04x\n", WUFEI_SBI_SYS_LOOP_V(gmr_mask)));
   WUFE_PRINT_DISP((wufei_log, "\tLine loopback: 0x%04x\n", WUFEI_SBI_LIN_LOOP_V(gmr_mask)));
   WUFE_PRINT_DISP((wufei_log, "\tRX parity check enable: 0x%04x\n",
                    WUFEI_SBI_MODE_RX_PRTY_CHK_V(gmr_mask)));
   WUFE_PRINT_DISP((wufei_log, "\tRX parity mode (0 - odd, 1 - even): 0x%04x\n",
                    WUFEI_SBI_MODE_RX_PRTY_EVEN_V(gmr_mask)));
   WUFE_PRINT_DISP((wufei_log, "\tTX parity mode (0 - odd, 1 - even): 0x%04x\n",
                    WUFEI_SBI_MODE_TX_PRTY_EVEN_V(gmr_mask)));
   WUFE_PRINT_DISP((wufei_log, "\tRX Enable: 0x%04x\n", WUFEI_SBI_MODE_RX_ENABLE_V(gmr_mask)));
   WUFE_PRINT_DISP((wufei_log, "\tTX Enable: 0x%04x\n", WUFEI_SBI_MODE_TX_ENABLE_V(gmr_mask)));
}

void WUFEI_HwSbiSpeRegsDisplay(WP_U32 sbi_base)
{
   WP_U32 stm1;
   WUFEI_hw_sbi_core* core_ptr;
   WP_U32 coreid;

   for (coreid=0; coreid<WUFE_MAX_N_CORE; coreid++)
   {
      WUFE_PRINT_DISP((wufei_log, "\nSBI SPE STM REGISTERS for core %d:\n", coreid));
      core_ptr = &((WUFEI_hw_sbi*)sbi_base)->core[coreid];

      for(stm1 = 0; stm1 < WUFE_MAX_N_STM1; stm1++)
      {
         WP_U16* reg;
         WP_U16 rx_value =0, tx_value = 0;

         reg = &(core_ptr->stm1_rx[stm1].value);
         WUFEI_REG_GET(reg, &rx_value);

         reg = &(core_ptr->stm1_tx[stm1].value);
         WUFEI_REG_GET(reg, &tx_value);

         WUFE_PRINT_DISP((wufei_log, "STM%d - ", stm1));
         WUFE_PRINT_DISP((wufei_log, "RX SPE register: 0x%04x ", rx_value));
         WUFE_PRINT_DISP((wufei_log, "TX SPE register: 0x%04x\n", tx_value));
      }
   }
}

void WUFEI_HwSbiGlobalDisplay(WUFEI_system_registry *system_registry)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 sbi_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   WUFE_PRINT_DISP((wufei_log, "\nSBI GLOBAL REGISTERS:\n------------------\n"));
   WUFEI_HwSbiGmrDisplay(sbi_base);
   WUFEI_HwSbiSpeRegsDisplay(sbi_base);
}
