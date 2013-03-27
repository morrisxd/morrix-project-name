
/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_clk_translator.c
 *
 * Purpose: the UFE clock translator functions
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_LINE_H
#include "wufe_line.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFEI_MEMORY_ACCESS_INT_H
#include "wufei_memory_access_int.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif

#ifndef WUFE_INT_H
#include "wufei_int.h"
#endif

#ifndef WUFEI_LINE_H
#include "wufei_line.h"
#endif

#ifndef WUFEI_CLK_TRANSLATOR_H
#include "wufei_clk_translator.h"
#endif

#ifndef WUFEI_HW_CLK_TRANSLATOR_H
#include "wufei_hw_clk_translator.h"
#endif

#ifndef WUFEI_HW_CLK_TRANSLATOR_INT_H
#include "wufei_hw_clk_translator_int.h"
#endif

#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif

static WUFE_status WUFEI_SbiClockTranslatorFindNewLine (WUFEI_system_registry *system_registry,
                                                        WP_U16 ct_id,
                                                        WP_U16 *new_line_id);
static void WUFEI_LineStmSpeSubspeToLineIndexMap(WP_U32 StmSpeSubspe,
                                                 WP_U32 *line_index);


/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorSetup
 *
 * Purpose:   initialize SBI clock translator mode registers
 *
 *
 * Description:
 *     This function disabled all SBI clock translators and writes value
 *     for each tributary in order to recognize later if the specific clock
 *     translator is available in the UFE synthesis.
 *
 * Inputs:
 *     system_registry  - The registry entry created for this UFE
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *     WUFE_SystemConfig()
 *
 ****************************************************************************/
WUFE_status WUFEI_SbiClockTranslatorSetup(WUFEI_system_registry* system_registry)
{
   WP_U16 hw_sbi_clock_translators, hw_sbi_ext_diff_clock_translators;
   WP_U32 size, i, max_instances, n_core;
   WP_U16 *arr;
   WP_U32 wufe_id;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   wufe_id = system_registry->wufe_id;
   /* Nothing to be done if clock translators not used */
   if (system_registry->hw_config.clk_rec_hw_config == 0 ||
       WUFEI_SystemClkRecHwConfigCtEn(system_registry) == WUFE_FALSE ||
       (system_registry->cfg.sbi.max_clock_translators == 0 &&
        system_registry->cfg.sbi.max_ext_diff_clock_translators == 0))
   {
      system_registry->sbi_clock_translator_block = 0;
      return WUFE_OK;
   }

   hw_sbi_clock_translators = WUFEI_SystemClkRecHwConfigCtAdapUnitsGet(system_registry);
   hw_sbi_ext_diff_clock_translators = WUFEI_SystemClkRecHwConfigCtDiffUnitsGet(system_registry);

   if ((system_registry->cfg.sbi.max_clock_translators > hw_sbi_clock_translators) ||
       (system_registry->cfg.sbi.max_ext_diff_clock_translators > hw_sbi_ext_diff_clock_translators))
      return WUFE_ERR_MAX_SBI_CT;

   /* allocate internal memory for SBI clock translator parameters */
   if (hw_sbi_ext_diff_clock_translators > 0)
   {
      size = (sizeof(WUFEI_sbi_clock_translator)*WUFE_CLOCK_REC_REGULAR_INSTANCES +
              sizeof(WUFEI_sbi_clock_translator)*system_registry->cfg.sbi.max_ext_diff_clock_translators);
   }
   else
   { /* if no ext diff in synthesis - not need to allocate memory */
      size = (sizeof(WUFEI_sbi_clock_translator)*system_registry->cfg.sbi.max_clock_translators);
   }
   system_registry->sbi_clock_translator_block =(WUFEI_sbi_clock_translator *)WUFEI_MALLOC(size);
   if (!(system_registry->sbi_clock_translator_block))
      return WUFE_ERR_MALLOC;
   /* clear memory for clock translator block */
   memset(system_registry->sbi_clock_translator_block, 0, size);


   if (hw_sbi_ext_diff_clock_translators > 0)
      max_instances = WUFE_CLOCK_REC_REGULAR_INSTANCES +
         system_registry->cfg.sbi.max_ext_diff_clock_translators;
   else
      max_instances = system_registry->cfg.sbi.max_clock_translators;
   size = sizeof(WP_U16)*system_registry->cfg.max_line_number;

   for (i=0; i<max_instances; i++)
   {
      arr = (WP_U16*)WUFEI_MALLOC(size);
      if (!arr)
         return WUFE_ERR_MALLOC;
      memset(arr, 0, size);
      system_registry->sbi_clock_translator_block[i].line_id_arr = arr;
      /* clear the registers */
      WUFEI_HwSbiClockTranslatorSet(wufe_id, i, 0);
   }
   /* Clear all CT RAM entries */
   if (WUFEI_UfeCoreBandwidthGet(wufe_id) == 0)
      n_core = 2;
   else
      n_core = 1;
   for( i=0; i<(WUFEI_SBI_CT_RAM_SIZE_PER_CORE*n_core); i++)
      WUFEI_HwSbiClockTranslatorIndirectSet(wufe_id, i, 0);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorDisplay
 *
 * Purpose:   Display information about SBI clock translators
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_SystemDisplay
 *     WUFE_SbiClockTranslatorDisplay
 *
 ****************************************************************************/
WUFE_status WUFEI_SbiClockTranslatorDisplay(WUFEI_system_registry *system_registry)
{
   WP_U32 i;
   WP_U16 value = 0, ct_trib_id = 0;
   WP_U32 wufe_id, n_core;

#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   wufe_id = system_registry->wufe_id;

   if (WUFEI_SystemClkRecHwConfigCtEn(system_registry) == WUFE_FALSE)
   {
      WUFE_PRINT_DISP((wufei_log,
                       "SBI clock translators are not defined\n"));
      return WUFE_OK;
   }

   WUFE_PRINT_DISP((wufei_log,
                    "Max SBI clock translators for regular mode: %d \n",
                    system_registry->cfg.sbi.max_clock_translators));
   WUFE_PRINT_DISP((wufei_log,
                    "Max SBI clock translators for extended differential mode: %d \n",
                    system_registry->cfg.sbi.max_ext_diff_clock_translators));

   /* get registry of the specific clock translator */
   for (i=0;
        i<(system_registry->cfg.sbi.max_clock_translators +
           system_registry->cfg.sbi.max_ext_diff_clock_translators);
        i++)
   {
      /* get SBI clock translator mode registers base address */
      WUFEI_HwSbiClockTranslatorGet(wufe_id, i, &value);
      if (value != 0x0)
         WUFE_PRINT_DISP((wufei_log,
                          "SBI clock translator %d address %x register value: 0x%04x\n",
                          i, WUFEI_HwSbiClockTranslatorAddrGet(wufe_id, i), value));
   }
   
   /* get indirect RAM information. display only fields != 0 */
   if (WUFEI_UfeCoreBandwidthGet(wufe_id) == 0)
      n_core = 2;
   else
      n_core = 1;
   for (i=0; i<(WUFEI_SBI_CT_RAM_SIZE_PER_CORE*n_core); i++)
   {
      WUFEI_HwSbiClockTranslatorIndirectGet(system_registry->wufe_id, i, &ct_trib_id);
      if (ct_trib_id != 0x0)
         WUFE_PRINT_DISP((wufei_log,
                          "SBI Clock Translator RAM: Entry 0x%x, value 0x%x\n", i, ct_trib_id));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorDelete
 *
 * Purpose:  frees the allocated clock translator memory
 *
 *
 * Description:
 *     This function frees the clock translator memory resources
 *
 * Inputs:
 *     system_registry  - The registry entry created for this UFE
 *     clock_translator_id - The ID of clock translator
 *     line_validate - check compatibility with the line or not
 *     transfer_type_spe - line transfer type for validation
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_SBI_CT_ID_NOT_EXIST
 *     WUFE_ERR_SBI_CT_NOT_INITIALIZED
 *     WUFE_ERR_SBI_CT_TRIB_TYPE_NOT_MATCH
 *
 *
 * Called by:
 *
 ****************************************************************************/
WUFE_status WUFEI_SbiClockTranslatorDelete(WUFEI_system_registry* system_registry)
{
   WP_U32 i;

#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (system_registry->sbi_clock_translator_block == 0)
      return WUFE_OK;

   for (i=0;
        i<(system_registry->cfg.sbi.max_clock_translators +
           system_registry->cfg.sbi.max_ext_diff_clock_translators);
        i++)
   {
      WUFEI_FREE(system_registry->sbi_clock_translator_block[i].line_id_arr);
   }

   WUFEI_FREE(system_registry->sbi_clock_translator_block);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorValidate
 *
 * Purpose:   checks clock translator compatibility
 *
 *
 * Description:
 *     This function checks if the specific clock translator is enabled and
 *     it is compatible with the give line.
 *
 * Inputs:
 *     system_registry  - The registry entry created for this UFE
 *     clock_translator_id - The ID of clock translator
 *     line_validate - check compatibility with the line or not
 *     transfer_type_spe - line transfer type for validation
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_SBI_CT_ID_NOT_EXIST
 *     WUFE_ERR_SBI_CT_NOT_INITIALIZED
 *     WUFE_ERR_SBI_CT_TRIB_TYPE_NOT_MATCH
 *
 *
 * Called by:
 *
 ****************************************************************************/
WUFE_status WUFEI_SbiClockTranslatorValidate(WUFEI_system_registry* system_registry,
                                             WP_U16 clock_translator_id,
                                             WUFE_un_framed line_framed_mode,
                                             WUFE_line_transf_type line_tributary_type)
{
   WP_U32 tributary_type, framed_mode;
   WP_U16 reg_value=0, ct_trib_id, number_of_active;
   
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* if CT_id < 32 => regular, if CT_id > 32 => extended  ==> check accordently. */
   if (clock_translator_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (clock_translator_id >= system_registry->cfg.sbi.max_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }
   else /* extended mode */
   {
      if (clock_translator_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->cfg.sbi.max_ext_diff_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }

   ct_trib_id = 0;
   if (system_registry->sbi_clock_translator_block[clock_translator_id].in > 0)
   {
      WUFEI_HwSbiClockTranslatorGet(system_registry->wufe_id, clock_translator_id, &reg_value);
      /* Read the Tx clock translator register according to the ct id */

      number_of_active = 0;
      ct_trib_id = 0x0;
      /* find number of active entries for spesific ct_id */
      WUFEI_HwSbiClockTranslatorFindActive(system_registry->wufe_id,
                                           clock_translator_id, &number_of_active);
      /* one and only one entry can be both enable and with the same CT id. */
      if (number_of_active == 0)
         return WUFE_ERR_SBI_CT_NOT_INITIALIZED;
      if (number_of_active > 1)
         return WUFE_ERR_SBI_CT_MORE_THAN_ONE_ACTIVE;
      
      tributary_type = WUFEI_HW_SBI_TX_CT_TRIB_TYPE_V(reg_value);
      framed_mode = WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_V(reg_value);

      if ((framed_mode != line_framed_mode) ||
          (!((line_tributary_type == WUFE_SBI_TYPE_T1 &&
              tributary_type == WUFEI_SBI_CT_TRIB_T1) ||
             (line_tributary_type == WUFE_SBI_TYPE_E1 &&
              tributary_type == WUFEI_SBI_CT_TRIB_E1) /*||  ds3 e3 do not exist any more.
             (line_tributary_type == WUFE_SBI_TYPE_DS3 &&
              tributary_type == WUFEI_SBI_CT_TRIB_T3) ||
             (line_tributary_type == WUFE_SBI_TYPE_E3 &&
              tributary_type == WUFEI_SBI_CT_TRIB_E3)*/)))
         return WUFE_ERR_SBI_CT_TRIB_TYPE_NOT_MATCH;
   }

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorRelease
 *
 * Purpose:   Free a SBI clock translator
 *
 *
 * Description:
 *     This function frees the specific SBI clock translator.
 *
 * Inputs:
 *     system_registry  - UFE system registry
 *     line_id          - UFE line id
 *     ct_id            - Clock Translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_SBI_CT_ID_NOT_EXIST
 *
 * Called by:
 *     user application
 *
 ****************************************************************************/
WUFE_status WUFEI_SbiClockTranslatorRelease(WUFEI_system_registry *system_registry,
                                            WP_U32 line_id,
                                            WP_U16 ct_id)
{
   WP_U16 line_trib_id, new_trib_line_id;
   WUFE_status status = WUFE_OK;

#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* if CT_id < 32 => regular, if CT_id > 32 => extended  ==> check accordently. */
   if (ct_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (ct_id >= system_registry->cfg.sbi.max_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }
   else /* extended mode */
   {
      if (ct_id-WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->cfg.sbi.max_ext_diff_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }

   if (system_registry->sbi_clock_translator_block[ct_id].in == 0)
      return WUFE_OK;

   /* nothing to release when this line doesn't work with this clock translator */
   if (system_registry->sbi_clock_translator_block[ct_id].line_id_arr[line_id] == 0)
      return WUFE_OK;

   system_registry->sbi_clock_translator_block[ct_id].in--;
   line_trib_id =
         system_registry->sbi_clock_translator_block[ct_id].line_id_arr[line_id] & 0x7fff;
   system_registry->sbi_clock_translator_block[ct_id].line_id_arr[line_id] = 0;

   if (system_registry->sbi_clock_translator_block[ct_id].in == 0)
   {
      /* Write 0 (CT register) according to the ct id */
      WUFEI_HwSbiClockTranslatorIndirectSet(system_registry->wufe_id, line_trib_id, 0);
   }
   else
   {
      WP_U16 enable = 0, ct_trib_id = 0;

      /* extranct the ct data from RAM for a given line */
      WUFEI_HwSbiClockTranslatorInfoGet(system_registry->wufe_id, ct_id,
                                        line_trib_id, &ct_trib_id, &enable);
      if ((ct_trib_id == ct_id) && (enable))
      {
         /* find another line for this clock translator */
         status =  WUFEI_SbiClockTranslatorFindNewLine(system_registry,
                                                       ct_id,
                                                       &new_trib_line_id);
         if(status == WUFE_OK)
         {
            /* disable the clock translator from "old" line */
            WUFEI_HwSbiClockTranslatorIndirectSet(system_registry->wufe_id, line_trib_id, 0);
            /* update the clock translator to the line's tributary id */
            WUFEI_HwSbiClockTranslatorTribUpdate(system_registry->wufe_id,
                                                 ct_id,
                                                 new_trib_line_id);
         }
         if(status == (WUFE_status)WUFE_ERR_SBI_CT_LINE_NOT_FOUND)
            return WUFE_OK;
      }
   }

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorFindNewLine
 *
 * Purpose:   Looks for a next line that belongs to the specific clock translator
 *
 * Description:
 *     The functions checks the line_id_arr array where there is an entry with
 *     1 set. The trbiutary_id of this entry is returned in the new_line_id. If the line_id
 *     is not found an error is returned.
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     ct_id  SBI clock translator number
 *
 * Outputs:
 *     new_line_id - new line tributary id
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_SBI_CT_ID_NOT_EXIST,
 *     WUFE_ERR_SBI_CT_LINE_NOT_FOUND
 *
 * Called by:
 *     WUFEI_SbiClockTranslatorRelease
 *
 ****************************************************************************/
static WUFE_status WUFEI_SbiClockTranslatorFindNewLine (WUFEI_system_registry *system_registry,
                                                        WP_U16 ct_id,
                                                        WP_U16 *new_trib_line_id)
{
   WP_U32 i;
   WP_U32 max_lines, new_line_id = 0;
   WUFEI_line_registry *line_registry;

#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* if CT_id < 32 => regular, if CT_id > 32 => extended  ==> check accordently. */
   if (ct_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (ct_id >= system_registry->cfg.sbi.max_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }
   else /* extended mode */
   {
      if (ct_id-WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->cfg.sbi.max_ext_diff_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }

   max_lines = system_registry->cfg.max_line_number;
   *new_trib_line_id = 0;

   for (i=0; i<max_lines; i++)
      if (system_registry->sbi_clock_translator_block[ct_id].line_id_arr[i] > 0)
      {
         /* new_trib_line_id is represent as core-stm-spe-subspe (trib_line_id) */
         *new_trib_line_id =
            system_registry->sbi_clock_translator_block[ct_id].line_id_arr[i] & 0x7fff;

         /* Map between trib_line_id and line_id */
         WUFEI_LineStmSpeSubspeToLineIndexMap(*new_trib_line_id, &new_line_id);

         line_registry = &system_registry->line_arr[new_line_id];
         
         if(line_registry->state_mode == WUFEI_LINE_ENABLE)
            return WUFE_OK;

      }

   return WUFE_ERR_SBI_CT_LINE_NOT_FOUND;
}


/*****************************************************************************
 *
 * Function:  WUFEI_LineStmSpeSubspeToLineIndexMap
 *
 * Purpose:   Calculate the line id from the core-stm-spe-subspe expression
 *
 * Description:
 *     The function extract the line ID from the core-stm-spe-subspe expression
 *     in the following way:
 *     original value: ------------------------------
 *            field:   | core | stm | spe | sub_spe |
 *            #bits:   |  1   |  1  |  2  |    5    |
 *                     ------------------------------
 *     extrancted value:
 *            core*168 + stm*84 + spe*2 + sub_spe (where sub_spe running from
 *            0 to 20 (E1) or 27 (T1)
 *
 * Inputs:
 *     StmSpeSubspe  the core-stm-spe-subspe expression
 *
 * Outputs:
 *     line_index - line id in regular expression
 *
 * Return Value:
 *
 * Called by:
 *     WUFEI_SbiClockTranslatorFindNewLine
 *
 ****************************************************************************/
static void WUFEI_LineStmSpeSubspeToLineIndexMap(WP_U32 StmSpeSubspe, WP_U32 *line_index)
{
   WP_U16 offset[4] = {0, 5, 7, 8};
   WP_U16 core, stm, spe, sub_spe;
   
#ifdef WUFE_UFE2PLUS
   return;
#endif
   
   core = (StmSpeSubspe & WUFE_CORE_MASK)>>offset[3];
   stm = (StmSpeSubspe & WUFE_STM_MASK)>>offset[2];
   spe = (StmSpeSubspe & WUFE_SPE_MASK)>>offset[1];
   sub_spe = (StmSpeSubspe & WUFE_SUBSPE_MASK)>>offset[0];
   
   *line_index = core*WUFE_CORE_OFFSET + stm*WUFE_STM_OFFSET + spe*WUFE_SPE_OFFSET + sub_spe;

   return;
}


/*****************************************************************************
 *
 * Function:  WUFE_ClockRecCtRamEntryRead
 *
 * Purpose:   Display information about indirect SBI clock translator register
 *
 *
 * Description: API function
 *
 *
 * Inputs:
 *     wufe_id          - UFE id number
 *     entry            - RAM entry point
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     user application
 *
 ****************************************************************************/
WP_U16 WUFE_ClockRecCtRamEntryRead(WP_U32 wufe_id, WP_U16 entry)
{
   WP_U16 ct_ram = 0;
   ((void)wufe_id);
   ((void)entry);   
#ifdef WUFE_UFE2PLUS
   return 0;
#endif

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      WUFEI_HwClockRecCtRamEntryRead(wufe_id, &ct_ram, entry);
   
      return ct_ram;
   }
   else
      return WUFE_CLOCK_REC_INVALID_VALUE;
}

/*****************************************************************************
 *
 * Function:  WUFE_SbiClockTranslatorDisplay
 *
 * Purpose:   Display information about SBI clock translators
 *
 *
 * Description: API function
 *
 *
 * Inputs:
 *     wufe_id          - UFE id number
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     user application
 *
 ****************************************************************************/
WUFE_status WUFE_SbiClockTranslatorDisplay(WP_U32 wufe_id)
{
   WUFE_status status;
   WUFEI_system_registry *system_registry;
   
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE3)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Invalid FPGA type. WUFE_SbiClockTranslatorDisplay valid for UFE3 only.\n"));
      return WUFE_ERR_LINE_INVALID_FPGA;
   }

   /**************************************
    * check emphy
    *
    * check FIFO counters
    *
    * check Idle patterns
    **************************************/
   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   status = WUFEI_SbiClockTranslatorDisplay(system_registry);

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return status;
}


/*****************************************************************************
 *
 * Function:  WUFEI_ClockTranslatorInit
 *
 * Purpose:   Initializes a clock translator
 *
 *
 * Description:
 *     This function initializes the specific SBI clock translator.
 *
 * Inputs:
 *     system_registry  - UFE system registry
 *     line_id          - UFE line id
 *     ct_id            - Clock Translator id
 *     tributary_id     - Line TRIBUTARY id
 *     framed_mode      - Line framing type
 *     transfer_type    - Line transfer type
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_SBI_CT_ID_NOT_EXIST
 *
 * Called by:
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockTranslatorInit(WUFEI_system_registry *system_registry,
                                      WP_U32 line_id,
                                      WP_U32 ct_id,
                                      WP_U16 tributary_id,
                                      WP_U16 framed_mode,
                                      WUFE_line_transf_type transfer_type)
{
   ((void)framed_mode);   
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* if CT_id < 32 => regular, if CT_id > 32 => extended  ==> check accordently. */
   if (ct_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (ct_id >= system_registry->cfg.sbi.max_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }
   else /* differential mode */
   {
      if (ct_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->cfg.sbi.max_ext_diff_clock_translators)
         return WUFE_ERR_SBI_CT_ID_NOT_EXIST;
   }

   if (system_registry->sbi_clock_translator_block[ct_id].in == 0)
   {
      WP_U8 tributary_type;
      WP_U32 adap_ct_id, diff_ct_id;

      if (transfer_type == WUFE_SBI_TYPE_T1)
         tributary_type = WUFEI_SBI_CT_TRIB_T1;
      else /* DS3 and E3 types do not exist any more */
         tributary_type = WUFEI_SBI_CT_TRIB_E1;

      if (ct_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         adap_ct_id = ct_id;
         diff_ct_id = WUFEI_SBI_CT_NON_ACTIVE_INSTANCE;
      }
      else
      {
         adap_ct_id = WUFEI_SBI_CT_NON_ACTIVE_INSTANCE;
         diff_ct_id = ct_id - WUFE_CLOCK_REC_REGULAR_INSTANCES;
      }
      
      WUFEI_HwClockTranslatorWrite(system_registry->wufe_id,
                                   ct_id,
                                   adap_ct_id, /* 0-31 */
                                   diff_ct_id, /* 0-335 */
                                   tributary_type,
                                   framed_mode,
                                   tributary_id);

   }

   /* Add the line_id to the list */
   system_registry->sbi_clock_translator_block[ct_id].in++;
   system_registry->sbi_clock_translator_block[ct_id].line_id_arr[line_id] =
      (tributary_id | 0x8000);

   return WUFE_OK;
}
