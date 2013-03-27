/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_line.c
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
#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif
#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif
#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif
#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif
#ifndef WUFEI_HW_CAD_INT_H
#include "wufei_hw_cad_int.h"
#endif
#ifndef WUFEI_HW_CAD_H
#include "wufei_hw_cad.h"
#endif
#ifndef WUFEI_HW_TDM_H
#include "wufei_hw_tdm.h"
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
#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif
#ifndef WUFEI_HW_GLOBALS_H
#include "wufei_hw_globals.h"
#endif
#ifndef WUFE_INT_H
#include "wufei_int.h"
#endif
#ifndef WUFEI_LINE_H
#include "wufei_line.h"
#endif
#ifndef WUFE_SBI_H
#include "wufe_sbi.h"
#endif
#ifndef WUFE_HW_SBI_INT_H
#include "wufei_hw_sbi_int.h"
#endif
#ifndef WUFEI_MEMORY_ACCESS_INT_H
#include "wufei_memory_access_int.h"
#endif
#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif
#ifndef WUFEI_HW_CLK_REC_INT_H
#include "wufei_hw_clk_rec_int.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif

static WP_CHAR *line_reg_line_mode[] = {
   "WUFE_STRUCT_NULL",
   "WUFE_STRUCT_LINE_CAD",
   "WUFE_STRUCT_LINE_SBI",
   "WUFE_STRUCT_LINE_GENERIC",
   "WUFE_STRUCT_LINE_SONET",
   "WUFE_STRUCT_LINE_SDH",
   "WUFE_STRUCT_LINE_DCC"
};

static WP_CHAR *line_reg_transfer_type[] = {
   "WUFE_CAD_TYPE_T1",
   "WUFE_CAD_TYPE_E1",
   "WUFE_CAD_TYPE_HMVIP_4_T1",
   "WUFE_CAD_TYPE_HMVIP_4_E1",
   "WUFE_CAD_TYPE_E3",
   "WUFE_CAD_TYPE_DS3",
   "WUFE_SBI_TYPE_T1",
   "WUFE_SBI_TYPE_E1",
   "WUFE_SBI_TYPE_DS3",
   "WUFE_SBI_TYPE_E3",
   "WUFE_SBI_TYPE_IDLE",
   "WUFE_GENERIC_TYPE_T1",
   "WUFE_GENERIC_TYPE_E1",
   "WUFE_GENERIC_TYPE_DS3",
   "WUFE_GENERIC_TYPE_E3",
   "WUFE_SDH_TYPE_T1",
   "WUFE_SDH_TYPE_E1",
   "WUFE_SDH_TYPE_DS3",
   "WUFE_SDH_TYPE_E3",
   "WUFE_SDH_TYPE_VC11",
   "WUFE_SDH_TYPE_VC12",
   "WUFE_SDH_TYPE_TU3",
   "WUFE_SDH_TYPE_VC3",
   "WUFE_SDH_TYPE_VC4",
   "WUFE_SDH_TYPE_VC44C",
   "WUFE_SDH_TYPE_VCAT",
   "WUFE_SONET_TYPE_T1",
   "WUFE_SONET_TYPE_E1",
   "WUFE_SONET_TYPE_DS3",
   "WUFE_SONET_TYPE_E3",
   "WUFE_SONET_TYPE_VT1_5",
   "WUFE_SONET_TYPE_VT2",
   "WUFE_SONET_TYPE_TU3",
   "WUFE_SONET_TYPE_STS1_SPE",
   "WUFE_SONET_TYPE_STS3",
   "WUFE_SONET_TYPE_STS12",
   "WUFE_SONET_TYPE_VCAT",
   "WUFE_DCC_TYPE_MS",
   "WUFE_DCC_TYPE_RS"
};

static WP_CHAR *line_reg_framed[] = {
   "WUFE_UNFRAMED",
   "WUFE_FRAMED"
};

static WP_CHAR *line_reg_loopback_type[] = {
      "WUFE_LINE_LOOPBACK_TYPE_LINE",
      "WUFE_LINE_LOOPBACK_TYPE_SYSTEM",
      "",
      "WUFE_LINE_LOOPBACK_TYPE_NONE"
};
static WP_CHAR *line_reg_pdh_loopback_type[] = {
      "WUFE_LINE_LOOPBACK_PDH_TYPE_LINE",
      "WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM",
      "WUFE_LINE_LOOPBACK_PDH_TYPE_NONE"
};



extern WP_U8 debug_flag;

static WUFE_status WUFEI_LineCadConfig(WUFE_line_cad         *line_cfg,
                                       WUFEI_system_registry *system_registry,
                                       WUFE_handle           *line_handle);
static WUFE_status WUFEI_LineSbiConfig(WUFE_line_sbi         *line_cfg,
                                       WUFEI_system_registry *system_registry,
                                       WUFE_handle           *line_handle);
static WUFE_status WUFEI_LineGenericConfig(WUFE_line_generic     *line_cfg,
                                           WUFEI_system_registry *system_registry,
                                           WUFE_handle           *line_handle);
static WUFE_status WUFEI_LineCadEnable(WUFEI_line_registry *line_registry,
                                       WP_U32 wufe_id,
                                       WP_U32 direction);
static WUFE_status WUFEI_LineSbiEnable(WUFEI_system_registry* system_registry,
                                       WUFEI_line_registry *line_registry,
                                       WP_U32 direction);
static WUFE_status WUFEI_LineCadDisable(WUFEI_line_registry *line_registry,
                                        WP_U32 wufe_id,
                                        WP_U32 direction);
static WUFE_status WUFEI_LineSbiDisable(WUFEI_system_registry* system_registry,
                                        WUFEI_line_registry *line_registry,
                                        WP_U32 direction);
static WUFE_status WUFEI_LineCadDisplay(WUFEI_line_registry *line_registry, WP_U32 wufe_id);
static WUFE_status WUFEI_LineSbiDisplay(WUFEI_line_registry *line_registry, WP_U32 wufe_id);
static WUFE_status WUFEI_LineLoopbackDisplay(WUFEI_line_registry *line_registry, WP_U32 wufe_id);
static WUFE_status WUFEI_LineCadCheckDecreaseOrder(WUFEI_system_registry *system_registry,
                                                   WUFE_line_cad         *line_cfg);
static WUFE_status WUFEI_LineTdmConfig(WUFEI_system_registry *system_registry,
                                       WUFEI_line_registry *line_registry,
                                       WP_U16 *hw_cad_tdm_ram_id);
static void WUFEI_LineTdmStm1SpeIndexMap(WUFEI_line_registry *line_registry,
                                         WP_U32 *stm1, WP_U32 *spe);

#define WUFEI_LineGenericEnable(line_registry,wufe_id,direction) 0
#define WUFEI_LineGenericDisable(line_registry,wufe_id,direction) 0
static WUFE_status WUFEI_LineSonetConfig(WUFE_line_sonet         *line_cfg,
                                         WUFEI_system_registry *system_registry,
                                         WUFE_handle           *line_handle);
static WUFE_status WUFEI_LineSdhConfig(WUFE_line_sdh         *line_cfg,
                                       WUFEI_system_registry *system_registry,
                                       WUFE_handle           *line_handle);
static WUFE_status WUFEI_LineDccConfig(WUFE_line_dcc         *line_cfg,
                                       WUFEI_system_registry *system_registry,
                                       WUFE_handle           *line_handle);

/*****************************************************************************
 *
 * Function:  WUFEI_SystemLineCheckParams
 *
 * Purpose: Checks the configuration parameters validity.
 *          Returns the maximal lines number
 *
 * Description:
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE
 *
 * Outputs:
 *      num_line - The lines number
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *       WUFE_ERR_ILLEGAL_NUM_LINES
 *       WUFE_ERR_INVALID_CONFIG
 * Called by:
 *     WUFEI_LineSystemSetup() function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemLineCheckParams(WUFEI_system_registry* system_registry,
                                               WP_U32* num_line)
{
   WUFE_status status;

   if( system_registry->cfg.line_interface < WUFE_INTERFACE_FIRST    ||
       system_registry->cfg.line_interface > WUFE_INTERFACE_LAST)
   { /* Invalid line_interface */
      WUFE_PRINT_ERR((wufei_log,
                      "the cfg line_interface type is invalid (%d)\n",
                      system_registry->cfg.line_interface));
      return WUFE_ERR_INVALID_CONFIG;

   }

   if( system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   { /*WUFE_SYS_SBI*/

      if (WUFEI_SystemHwConfigLiuSbi(system_registry) == WUFE_FALSE)
      {
         WUFE_PRINT_ERR((wufei_log, "SBI interface is not supported\n"));
         return WUFE_ERR_LINE_INVALID_TYPE;
      }

      if(( system_registry->cfg.max_line_number > WUFE_MAX_N_SBI_LINE )||
         ( system_registry->cfg.max_line_number == 0))
      {
         WUFE_PRINT_ERR((wufei_log,
                         "num_line(%d) > WUFE_MAX_N_SBI_LINE ||(num_line==0)\n",
                         system_registry->cfg.max_line_number));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }

      if (system_registry->cfg.max_line_number > WUFE_MAX_N_SBI_SPE  &&
          system_registry->cfg.max_line_number < WUFE_MAX_N_SBI_LINE)
      {/* if the user inteded to use more than line per SPE than
          force the max number of line to be the max possible for T1 configuration */
         system_registry->cfg.max_line_number = WUFE_MAX_N_SBI_LINE;
      }

      *num_line = system_registry->cfg.max_line_number;

   }
   else if ( system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_HMVIP )
   {
      if (WUFEI_SystemHwConfigLiuCad(system_registry) == WUFE_FALSE)
      {
         WUFE_PRINT_ERR((wufei_log, "CAD interface is not supported\n"));
         return WUFE_ERR_LINE_INVALID_TYPE;
      }

      if (WUFEI_SystemHwConfigFullModel(system_registry) == WUFE_TRUE)
      {
         /* check for more detailed information for CAD HMVIP or E1/T1 from
          * board support */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_BsCadLineIfCheck(system_registry->wufe_id,
                                                      system_registry->cfg.line_interface));
      }
      /* create the active line array */
      *num_line = system_registry->cfg.max_line_number;

      if(( *num_line > WUFE_MAX_N_CAD_LINE_HMVIP )||(*num_line==0))
      {
         WUFE_PRINT_ERR((wufei_log,"num_line > WUFE_MAX_N_CAD_LINE_HMVIP ||(num_line==0)\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }
   }
   else if ( system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_E1_T1 )
   {
      /* create the active line array */
      *num_line = system_registry->cfg.max_line_number;

      if(( *num_line > WUFE_MAX_N_CAD_LINE_E1T1 )||(*num_line == 0))
      {
         WUFE_PRINT_ERR((wufei_log,"num_line > WUFE_MAX_N_CAD_LINE_E1T1 ||(num_line==0)\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }
   }
   else if ( system_registry->cfg.line_interface == WUFE_INTERFACE_GENERIC )
   {
      if( (system_registry->cfg.max_line_number > WUFE_MAX_N_GENERIC_LINE )
          ||(system_registry->cfg.max_line_number == 0))
      {
         WUFE_PRINT_ERR((wufei_log,"num_line > WUFE_MAX_N_GENERIC_LINE ||(num_line==0)\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }

      /* create the active line array */
      *num_line = WUFE_MAX_N_GENERIC_LINE;
      /* line registry allocates mem for all lines*/
      system_registry->cfg.max_line_number = WUFE_MAX_N_GENERIC_LINE;

   }
   else if ( system_registry->cfg.line_interface == WUFE_INTERFACE_SDH ||
             system_registry->cfg.line_interface == WUFE_INTERFACE_SONET)
   {
      if( (system_registry->cfg.max_line_number > WUFE_MAX_OC48_LINES + WUFE_MAX_OC48_DCC_LINES)
          ||(system_registry->cfg.max_line_number == 0))
      {
         WUFE_PRINT_ERR((wufei_log,"num_line out of range\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }

      /* create the active line array */
      if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC3)
         system_registry->cfg.max_line_number = WUFE_MAX_OC3_LINES + WUFE_MAX_OC3_DCC_LINES;
      else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC6)
         system_registry->cfg.max_line_number = WUFE_MAX_OC6_LINES + WUFE_MAX_OC6_DCC_LINES;
      else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12)
         system_registry->cfg.max_line_number = WUFE_MAX_OC12_LINES + WUFE_MAX_OC12_DCC_LINES;
      else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC24)
         system_registry->cfg.max_line_number = WUFE_MAX_OC24_LINES;
      else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
         system_registry->cfg.max_line_number = WUFE_MAX_OC48_LINES+ WUFE_MAX_OC48_DCC_LINES;

      *num_line = system_registry->cfg.max_line_number;
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log,"WUFE Interface Invalid\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_LineSystemSbiSetup
 *
 * Purpose: Configure the SBI interface in the UFE
 *
 * Description:
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *       WUFE_ERR_INVALID_CONFIG
 * Called by:
 *     WUFEI_LineSystemSetup function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSystemSbiSetup(WUFEI_system_registry* system_registry)
{
   WUFE_status status;
   WP_U32 coreid, ufeid;

   ufeid = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwSbiGmrSetup(system_registry));

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwSbiSpeModeSetup(system_registry, coreid));

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwSbiRamSetup(system_registry, coreid));

      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSystemSetup
 *
 * Purpose:   configure the registery for the lines and reset the lines.
 *
 *
 * Description:
 *     This function reset the HW registers of all the lines.
 *     the function malloc all the memory needed for the registery for all the lines.
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     write to external HW of the lines mode registers (CAD)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_LineSystemSetup(WUFEI_system_registry* system_registry)
{
   WP_U32      num_line, ii;
   WUFE_status status;

   WUFEI_RETURN_IF_ERROR(status,WUFEI_SystemLineCheckParams(system_registry, &num_line));

   if( system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_LineSystemSbiSetup(system_registry));
   }
   else if(system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_E1_T1 ||
           system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_HMVIP ||
           system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_UNCHAN_DS3_E3)
   {/*Cad setup*/
      WP_U32 max_line;
      WP_U32 wufe_id = system_registry->wufe_id;

      if( system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_E1_T1)
         max_line = WUFE_MAX_N_CAD_LINE_E1T1;
      else if( system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_HMVIP)
         max_line = WUFE_MAX_N_CAD_LINE_HMVIP;
      else if( system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_UNCHAN_DS3_E3)
         max_line = WUFE_MAX_N_CAD_LINE_DS3;
      else
      {
         WUFE_PRINT_ERR((wufei_log,"invalid CAD mode\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }

      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
      {
         WUFEI_RETURN_IF_ERROR(status, WUFEI_HwCadSetup(wufe_id, max_line));
      }

   }

   system_registry->line_arr =
      (WUFEI_line_registry *)WUFEI_MALLOC(num_line*sizeof(WUFEI_line_registry));
   if (!(system_registry->line_arr))
      return (WUFE_ERR_MALLOC);

   /*set the state_mode to empty*/
   memset(system_registry->line_arr, 0, num_line*sizeof(WUFEI_line_registry));

   /*
    * Malloc the  tdm_ram_mng for each Line
    * the fields are
    * 1) which HMVIP to use (in SBI alwyas entry 0)
    * 2) Rx or TX           (i.e.  2)
    * 3) number of slots    (i.e. 32)
    */

   if( system_registry->cfg.line_interface == WUFE_INTERFACE_SBI ||
       system_registry->cfg.line_interface == WUFE_INTERFACE_GENERIC )
   { /* WUFE_SYS_SBI */

      for (ii = 0; ii < num_line ; ii++)
      {
         system_registry->line_arr[ii].tdm_ram_mng =
            (WUFEI_tdm_slots *)WUFEI_MALLOC(sizeof(WUFEI_tdm_slots));
         if (!(system_registry->line_arr[ii].tdm_ram_mng))
            return (WUFE_ERR_MALLOC);
      }
   }
   else if(system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_E1_T1 ||
           system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_HMVIP ||
           system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_UNCHAN_DS3_E3)
   {/* WUFE_SYS_CAD  */
      for (ii = 0; ii < num_line ; ii++)
      {
         system_registry->line_arr[ii].tdm_ram_mng =
            (WUFEI_tdm_slots *)WUFEI_MALLOC( sizeof(WUFEI_tdm_slots) * WUFE_MAX_HMVIP_INUSE );
         if (!(system_registry->line_arr[ii].tdm_ram_mng))
            return (WUFE_ERR_MALLOC);
      }
   }
   else if ( system_registry->cfg.line_interface == WUFE_INTERFACE_SDH ||
             system_registry->cfg.line_interface == WUFE_INTERFACE_SONET)
   {
      for (ii = 0; ii < num_line ; ii++)
      {
         system_registry->line_arr[ii].tdm_ram_mng =
            (WUFEI_tdm_slots *)WUFEI_MALLOC(sizeof(WUFEI_tdm_slots));
         if (!(system_registry->line_arr[ii].tdm_ram_mng))
            return (WUFE_ERR_MALLOC);
      }
      /*Init tug2 array*/
      memset(system_registry->tug2_arr,
             0,
             (sizeof(WUFEI_tug2_info) *
              WUFE_MAX_N_CORE*WUFE_MAX_N_AUG4*WUFE_MAX_N_AUG1*WUFE_MAX_N_AU3*WUFE_MAX_N_TUG2));
   }

   memset(system_registry->tdm_line_arr,
          WUFEI_EMPTY_TDM_ENTRY,
          (sizeof(WUFEI_tdm_line_entry) *
           WUFE_MAX_N_AUG4* WUFE_MAX_N_SBI_SPE * WUFEI_SPE_LINE_ADDRESS_SPACE));

   /* Malloc the CR line params (not relevant in UFE3) */
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE3)
   {
      for (ii = 0; ii < num_line ; ii++)
      {
         system_registry->line_arr[ii].clock_rec_line_params =
            (WUFE_line_clock_rec_params *)WUFEI_MALLOC(sizeof(WUFE_line_clock_rec_params));
         if (!(system_registry->line_arr[ii].clock_rec_line_params))
            return (WUFE_ERR_MALLOC);
         /* Init struct to zeros */
         memset(system_registry->line_arr[ii].clock_rec_line_params, 0, sizeof(WUFE_line_clock_rec_params));
      }
   }

   return WUFE_OK;

}


/*****************************************************************************
 *
 * Function:  WUFEI_LineSystemDelete
 *
 * Purpose:   Release memory allocated for lines in the UFE system
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry The regestry entry created for this UFE
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_LineSystemDelete(WUFEI_system_registry* system_registry)
{
   WP_U32 max_num_line, i;

   max_num_line = system_registry->cfg.max_line_number;

   if( system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      if(( max_num_line > WUFE_MAX_N_SBI_LINE  )||(max_num_line==0))
      {
         WUFE_PRINT_ERR(
            ( wufei_log,
       "WUFEI_LineSystemDelete: max_num_line > WUFE_MAX_N_SBI_LINE ||(max_num_line==0)\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }

   }
   else if(system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_HMVIP )
   {
      if(( max_num_line > WUFE_MAX_N_CAD_LINE_HMVIP  )||(max_num_line==0))
      {
         WUFE_PRINT_ERR((wufei_log,
       "WUFEI_LineSystemDelete: max_num_line > WUFE_MAX_N_CAD_LINE_HMVIP ||(max_num_line==0)\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }
   }
   else if(system_registry->cfg.line_interface == WUFE_INTERFACE_CAD_E1_T1 )
   {
      if(( max_num_line > WUFE_MAX_N_CAD_LINE_E1T1  )||(max_num_line==0))
      {
         WUFE_PRINT_ERR((wufei_log,
       "WUFEI_LineSystemDelete: max_num_line > WUFE_MAX_N_CAD_LINE_E1T1  ||(max_num_line==0)\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }
   }
   else if(system_registry->cfg.line_interface == WUFE_INTERFACE_GENERIC )
   {
      if(( max_num_line > WUFE_MAX_N_GENERIC_LINE)||(max_num_line==0))
      {
        WUFE_PRINT_ERR(
            ( wufei_log,
         "WUFEI_LineSystemDelete: max_num_line > WUFE_MAX_N_GENERIC_LINE ||(max_num_line==0)\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }
   }
   else if (system_registry->cfg.line_interface == WUFE_INTERFACE_SDH ||
            system_registry->cfg.line_interface == WUFE_INTERFACE_SONET)
   {
      if(( max_num_line > WUFE_MAX_OC48_LINES)||(max_num_line==0))
      {
         WUFE_PRINT_ERR(
            ( wufei_log,
              "WUFEI_LineSystemDelete: max_num_line out of range\n"));
         return WUFE_ERR_ILLEGAL_NUM_LINES;
      }
   }
   else
   {
      return WUFE_ERR_INVALID_CONFIG;
   }

   if (system_registry->line_arr)
   {
      for (i=0;i<max_num_line; i++)
      {
         if (system_registry->line_arr[i].tdm_ram_mng)
            WUFEI_FREE(system_registry->line_arr[i].tdm_ram_mng);
      }
   }
   
   if (system_registry->line_arr)
   {
      WUFEI_FREE(system_registry->line_arr);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_LineCreate
 *
 * Purpose:   configure the registery and HW for the UFE line
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     line_handle - UFE Line handle
 *     wufe_id - UFE identifier
 *     type - UFE Line type
 *     line_in_config - UFE line user configuration structure
 *
 * Outputs:
 *     UFE Line handle
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any user function.
 *
 ****************************************************************************/
WUFE_status WUFE_LineCreate(WUFE_handle   *line_handle,
                            WP_U32         wufe_id,
                            WUFE_struct_id type,
                            void *         line_in_config)
{
   WUFEI_system_registry *system_registry;
   WUFE_status status;
   WUFE_status rc = WUFE_OK;

   /* lock the wufe_id */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,&system_registry,status);

   /* Check whether it is CAD type or not */
   if (type == WUFE_STRUCT_LINE_CAD)
   {
      WUFE_line_cad *line_cfg;

      line_cfg = (WUFE_line_cad *)line_in_config;

      if(line_cfg->struct_id != WUFE_STRUCT_LINE_CAD)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid struct_id %d != WUFE_STRUCT_LINE_CAD\n",
                         line_cfg->struct_id));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_TYPE, wufe_id);
      }
      else
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_LineCadConfig(line_cfg,
                                                            system_registry,
                                                            line_handle),
                                        wufe_id);
      }
   }
   else if (type == WUFE_STRUCT_LINE_SBI)
   {
      WUFE_line_sbi *line_cfg;

      line_cfg = (WUFE_line_sbi *)line_in_config;

      if(line_cfg->struct_id != WUFE_STRUCT_LINE_SBI)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid struct_id %d != WUFE_STRUCT_LINE_SBI\n",
                         line_cfg->struct_id));
         WUFEI_IF_ERROR_RETURN_DETACH_W( status, WUFE_ERR_LINE_INVALID_TYPE, wufe_id);
      }
      else
      {
         /* check clock recovery configuration parameters */
         if (line_cfg->clock_rec_mode >= WUFE_LINE_CLOCK_REC_NA)
         {
            WUFEI_IF_ERROR_RETURN_DETACH_W( status, WUFE_ERR_CLOCK_REC_LINE_PARAMS, wufe_id);
         }

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_LineSbiConfig(line_cfg,
                                                            system_registry,
                                                            line_handle),
                                        wufe_id);
      }
   }
   else if(type == WUFE_STRUCT_LINE_GENERIC)
   {
      WUFE_line_generic *line_cfg;

      line_cfg = (WUFE_line_generic *)line_in_config;

      if(line_cfg->struct_id != WUFE_STRUCT_LINE_GENERIC)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid struct_id %d != WUFE_STRUCT_LINE_GENERIC\n",
                         line_cfg->struct_id));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_TYPE, wufe_id);
      }
      else
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_LineGenericConfig(line_cfg,
                                                                system_registry,
                                                                line_handle),
                                        wufe_id);
      }

   }
   else if(type == WUFE_STRUCT_LINE_SDH)
   {
      WUFE_line_sdh *line_cfg;

      if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid fpga type. WUFE_STRUCT_LINE_SDH valid for UFE4 only\n"));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_FPGA, wufe_id);
      }

      line_cfg = (WUFE_line_sdh *)line_in_config;

      if(line_cfg->struct_id != WUFE_STRUCT_LINE_SDH)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid struct_id %d != WUFE_STRUCT_LINE_SDH\n",
                         line_cfg->struct_id));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_TYPE, wufe_id);
      }
      else
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_LineSdhConfig(line_cfg,
                                                            system_registry,
                                                            line_handle),
                                        wufe_id);
      }

   }
   else if(type == WUFE_STRUCT_LINE_SONET)
   {
      WUFE_line_sonet *line_cfg;

      if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid fpga type. WUFE_STRUCT_LINE_SONET valid for UFE4 only\n"));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_FPGA, wufe_id);
      }

      line_cfg = (WUFE_line_sonet *)line_in_config;

      if(line_cfg->struct_id != WUFE_STRUCT_LINE_SONET)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid struct_id %d != WUFE_STRUCT_LINE_SONET\n",
                         line_cfg->struct_id));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_TYPE, wufe_id);
      }
      else
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_LineSonetConfig(line_cfg,
                                                            system_registry,
                                                            line_handle),
                                        wufe_id);
      }

   }
   else if(type == WUFE_STRUCT_LINE_DCC)
   {
      WUFE_line_dcc *line_cfg;

      if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid fpga type. WUFE_STRUCT_LINE_DCC valid for UFE4 only\n"));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_FPGA, wufe_id);
      }

      line_cfg = (WUFE_line_dcc *)line_in_config;

      if(line_cfg->struct_id != WUFE_STRUCT_LINE_DCC)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid struct_id %d != WUFE_STRUCT_LINE_DCC\n",
                         line_cfg->struct_id));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_TYPE, wufe_id);
      }
      else
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_LineDccConfig(line_cfg,
                                                            system_registry,
                                                            line_handle),
                                        wufe_id);
      }

   }
   else
   {
      /* NOT a valid struct */
      rc = WUFE_ERR_LINE_INVALID_TYPE;
   }

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return rc;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineGenericCfgCheck
 *
 * Purpose:   check the configuration of lines type Generic
 *
 *
 * Description:
 *
 * Inputs:
 *
 *     line_cfg           The user configuration
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be re`turned:
 *
 * Called by:
 *     WUFEI_LineGenericConfig.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineGenericCfgCheck(WUFE_line_generic *line_cfg)
{

   if(line_cfg->ufe_core_id >= WUFE_MAX_N_CORE)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->ufe_core_id %d\n",line_cfg->ufe_core_id));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->stm1 >= WUFE_MAX_N_STM1 )
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->stm1_id %d\n",line_cfg->stm1));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->spe >= WUFE_MAX_N_SPE )
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->spe_id %d\n",line_cfg->spe));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   if((WUFEI_SUB_SPE(line_cfg->tug2,line_cfg->tu))>= WUFE_MAX_N_LINE_PER_SPE)
   {/* illegal line number spe or sun spe*/
      WUFE_PRINT_ERR(
         (wufei_log,
          "ERROR:line_cfg->tug2 *line_cfg->tu %d\n",
          WUFEI_SUB_SPE(line_cfg->tug2,line_cfg->tu)));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   if (line_cfg->framed        > 1 ||
       line_cfg->cas_enable    > 1  )
   {
      WUFE_PRINT_ERR((wufei_log,
                      "\ninvalid config framed  %d \n cas_enable %d \n",
                      line_cfg->framed,
                      line_cfg->cas_enable));
      return WUFE_ERR_INVALID_CONFIG;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSonetCfgCheck
 *
 * Purpose:   check the configuration of lines type SONET
 *
 *
 * Description:
 *
 * Inputs:
 *
 *     line_cfg           The user configuration
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_LineSonetConfig.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSonetCfgCheck(WUFEI_system_registry *system_registry,
                                           WUFE_line_sonet *line_cfg)
{
   WP_U32 tug2_entry;

   if(line_cfg->sts12 >= WUFE_MAX_N_AUG4)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->sts12 %d out of range\n",line_cfg->sts12));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->sts3 >= WUFE_MAX_N_AUG1)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->sts3 %d out of range\n",line_cfg->sts3));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->sts1 >= WUFE_MAX_N_AU3)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->sts1 %d out of range\n",line_cfg->sts1));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->vt_group >= WUFE_MAX_N_TUG2)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->vt_group %d out of range\n",line_cfg->vt_group));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->vt >= WUFE_MAX_N_TU)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->vt %d out of range\n",line_cfg->vt));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   if ((line_cfg->vt == WUFE_MAX_N_TU-1) &&
       (line_cfg->transfer_type == WUFE_SONET_TYPE_E1 ||
        line_cfg->transfer_type == WUFE_SONET_TYPE_VT2))
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->vt %d not valid for the transfer type %s\n",
          line_cfg->vt,
          line_reg_transfer_type[line_cfg->transfer_type]));
      WUFE_PRINT_ERR(
         (wufei_log, "Location is sts12: %d, sts3: %d, sts1: %d, vt_group: %d\n",
          line_cfg->sts12,
          line_cfg->sts3,
          line_cfg->sts1,
          line_cfg->vt_group));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   tug2_entry = ((line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2) +
                 (line_cfg->sts3 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2) +
                 (line_cfg->sts1 * WUFE_MAX_N_TUG2) +
                 line_cfg->vt_group);

   if (system_registry->tug2_arr[tug2_entry].num_of_lines!=0 &&
       system_registry->tug2_arr[tug2_entry].transfer_type != line_cfg->transfer_type)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->vt_group %d cannot be configured for the transfer type %s, since it already contains the transfer type %s.\n",
          line_cfg->vt_group,
          line_reg_transfer_type[line_cfg->transfer_type],
          line_reg_transfer_type[system_registry->tug2_arr[tug2_entry].transfer_type]));
      WUFE_PRINT_ERR(
         (wufei_log, "Location is sts12: %d, sts3: %d, sts1: %d\n",
          line_cfg->sts12,
          line_cfg->sts3,
          line_cfg->sts1));
      return WUFE_ERR_INVALID_TUG2_TRANSFER_TYPE;
   }

   if (line_cfg->framed > WUFE_FRAMED ||
       line_cfg->cas_enable > WUFE_CAS_ENABLE)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "\ninvalid config framed  %d \n cas_enable %d \n",
                      line_cfg->framed,
                      line_cfg->cas_enable));
      return WUFE_ERR_INVALID_CONFIG;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSdhCfgCheck
 *
 * Purpose:   check the configuration of lines type SDH
 *
 *
 * Description:
 *
 * Inputs:
 *
 *     line_cfg           The user configuration
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_LineSdhConfig.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSdhCfgCheck(WUFEI_system_registry *system_registry,
                                         WUFE_line_sdh *line_cfg)
{
   WP_U32 tug2_entry;

   if(line_cfg->stm4 >= WUFE_MAX_N_AUG4)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->stm4 %d out of range\n",line_cfg->stm4));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->stm1 >= WUFE_MAX_N_AUG1)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->stm1 %d out of range\n",line_cfg->stm1));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->stm0 >= WUFE_MAX_N_AU3)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->stm0 %d out of range\n",line_cfg->stm0));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->tug2 >= WUFE_MAX_N_TUG2)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->tug2 %d out of range\n",line_cfg->tug2));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->tu >= WUFE_MAX_N_TU)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->tu %d out of range\n",line_cfg->tu));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   if ((line_cfg->tu == WUFE_MAX_N_TU-1) &&
       (line_cfg->transfer_type == WUFE_SDH_TYPE_E1 ||
        line_cfg->transfer_type == WUFE_SDH_TYPE_VC12))
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->tu %d not valid for the transfer type %s\n",
          line_cfg->tu,
          line_reg_transfer_type[line_cfg->transfer_type]));
      WUFE_PRINT_ERR(
         (wufei_log, "Location is stm4: %d, stm1: %d, stm0: %d, tug2: %d\n",
          line_cfg->stm4,
          line_cfg->stm1,
          line_cfg->stm0,
          line_cfg->tug2));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   tug2_entry = ((line_cfg->stm4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2) +
                 (line_cfg->stm1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2) +
                 (line_cfg->stm0 * WUFE_MAX_N_TUG2) +
                 line_cfg->tug2);
   if (system_registry->tug2_arr[tug2_entry].num_of_lines!=0 &&
       system_registry->tug2_arr[tug2_entry].transfer_type != line_cfg->transfer_type)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->tug2 %d cannot be configured for the transfer type %s, since it already contains the transfer type %s.\n",
          line_cfg->tug2,
          line_reg_transfer_type[line_cfg->transfer_type],
          line_reg_transfer_type[system_registry->tug2_arr[tug2_entry].transfer_type]));
      WUFE_PRINT_ERR(
         (wufei_log, "Location is stm4: %d, stm1: %d, stm0: %d\n",
          line_cfg->stm4,
          line_cfg->stm1,
          line_cfg->stm0));
      return WUFE_ERR_INVALID_TUG2_TRANSFER_TYPE;
   }

   if (line_cfg->framed > WUFE_FRAMED ||
       line_cfg->cas_enable > WUFE_CAS_ENABLE)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "\ninvalid config framed  %d \n cas_enable %d \n",
                      line_cfg->framed,
                      line_cfg->cas_enable));
      return WUFE_ERR_INVALID_CONFIG;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineDccCfgCheck
 *
 * Purpose:   check the configuration of line type DCC
 *
 *
 * Description:
 *
 * Inputs:
 * 
 *     system_registry    The registry entry created for this UFE
 *     line_cfg           The user configuration to be checked
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_LineDccConfig.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineDccCfgCheck(WUFEI_system_registry *system_registry,
                                         WUFE_line_dcc *line_cfg)
{
   WP_U32 max_dcc_lines;

   if(line_cfg->transfer_type != WUFE_DCC_TYPE_MS &&
      line_cfg->transfer_type != WUFE_DCC_TYPE_RS)
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:Illegal DCC transfer type\n"));
      return WUFE_ERR_INVALID_TRANSFER_TYPE;
   }

   /* line_port_id is the number of optical ports, each having two channels*/
   if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12)
   {
      max_dcc_lines = WUFE_MAX_OC12_DCC_LINES;
   }
   else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC6)
   {
      max_dcc_lines = WUFE_MAX_OC6_DCC_LINES;
   }
   else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC3)
   {
      max_dcc_lines = WUFE_MAX_OC3_DCC_LINES;
   }
   else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
   {
      max_dcc_lines = WUFE_MAX_OC48_DCC_LINES;
   }

   else
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR: DCC illegal bandwidth\n"));
      return WUFE_ERR_NOT_IMPLEMENTED;
   }
   

   /* On each optical port there are two DCC lines. This why we that the line_port_id is not bigger than (max_dcc_lines/2)*/
   if(line_cfg->line_port_id >= (max_dcc_lines/2))
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:Illegal DCC line port id %d >= max_dcc_lines/2 %d\n", line_cfg->line_port_id,max_dcc_lines/2));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadClockRecConfig
 *
 * Purpose:   configure the registery and HW for the lines
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
 *     write to external HW of the lines mode registers (CAD)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadClockRecConfig(WUFEI_system_registry *system_registry,
                                               WUFEI_line_registry *line_registry)
{
   WUFE_status status;
   WP_U32 wufe_id = system_registry->wufe_id;
   WP_U32 line_id = line_registry->line_id;

   if (line_registry->clock_rec_mode >= WUFE_LINE_CLOCK_REC_NA)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_CLOCK_REC_LINE_PARAMS, wufe_id);
   }

   if ((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) ||
       (line_registry->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY))
   {
      if (line_registry->ext_mode_active)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecTxInterfaceIdValidate(system_registry,
                                                                   line_registry->transfer_type,
                                                                   line_id+WUFE_CLOCK_REC_REGULAR_INSTANCES));
      }
      else
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecTxInterfaceIdValidate(system_registry,
                                                                   line_registry->transfer_type,
                                                                   line_id));
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_CadClockRecTxExtendedModeValidate(system_registry,
                                                                    line_registry->ext_mode_active,
                                                                    line_id));

      /* assign a clock recovery interface */
      /* lines in extended differential mode getting offset of 32 CDs */
      if (line_registry->ext_mode_active)
         line_registry->tx_clock_rec_if_id = line_id + WUFE_CLOCK_REC_REGULAR_INSTANCES;
      else
      { /* In regular mode, line ID can not be higher then 32 */
         if (line_id >= WUFE_CLOCK_REC_REGULAR_INSTANCES)
         {
            WUFE_PRINT_ERR((wufei_log,
                            "WUFE_LineCreate: tx clock recovery can't be enabled in extended differential mode\n"));
            WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_CAD_INVALID_CFG, wufe_id);
         }
         line_registry->tx_clock_rec_if_id = line_id;
      }


      /* Set the CR operation mode (adaptive/regular differential or extended differential) using
         mux2:1 in the BS logic: If CD<64 - use regular differential (or adaptive) if CD>=64 -
         use extended differential. */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_BsLineCadClockRecTypeSelect(
                               wufe_id,
                               system_registry->cfg.line_interface,
                               line_id,
                               line_registry->tx_clock_rec_if_id));
   }

   if (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX)
   {
      WP_U8 method;

      /* the Clock recovery Rx mode can be enabled in the differential mode only */
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_ClockRecMethodGet(system_registry, &method),
                                     wufe_id);
      if (method == WUFE_CLOCK_REC_METHOD_ADAPTIVE)
      {
         WUFE_PRINT_ERR((wufei_log,
             "WUFE_LineCreate: rx clock recovery can't be enabled in adaptive mode\n"));
         WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_CAD_INVALID_CFG, wufe_id);
      }
      else
      {
         /* CAD differential clock recovery method uses Rx timestamp */
         /* In this case the rx clock recovery interface id must be obtained */
         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_ClockRecRxInterfaceCreate(system_registry,
                                            line_registry),
            wufe_id);
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadConfig
 *
 * Purpose:   configure the registery and HW for the lines
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
 *     write to external HW of the lines mode registers (CAD)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadConfig(WUFE_line_cad         *line_cfg,
                                       WUFEI_system_registry *system_registry,
                                       WUFE_handle           *line_handle)
{
   WUFEI_line_registry   *line_registry;
   WUFE_line_clock_rec_params *tmp_clk_rec_line_params;
   WUFE_status            status;
   WP_U16 hw_cad_mode = 0, hw_cad_tdm_ram_id = 0;
   WUFEI_tdm_slots* tmp_tdm_ram_mng_ptr;
   WP_U32 wufe_id = system_registry->wufe_id;
   WP_U16 registry_entry, sub_registry_entry;

   if (line_cfg->cad_id >= WUFEI_MAX_N_CAD)
   {/* illegal line number */
      WUFE_PRINT_ERR((wufei_log,
          "ERROR: line_cfg->cad_id(%d) >= WUFEI_MAX_N_CAD( %d )\n",
          line_cfg->cad_id, WUFEI_MAX_N_CAD));
      return WUFE_ERR_LINE_ILLEGAL_ID ;
   }

   line_registry = &system_registry->line_arr[line_cfg->cad_id];

   if (line_registry->state_mode != WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log, "CAD already configured cad_id %d\n", line_cfg->cad_id));
      return WUFE_ERR_LINE_ALREADY_CONFIGURED;
   }

   /* start new line create */
   /* Get the TDM<-->PHY entry/entries */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineCadCheckDecreaseOrder(system_registry,
                                                         line_cfg));

   /* Set all the registry to 0xff specificly important to
      the Phy array to for the DS0 slot.(each entry points to a phy) */
   tmp_tdm_ram_mng_ptr = line_registry->tdm_ram_mng;
   tmp_clk_rec_line_params = line_registry->clock_rec_line_params;
   memset (line_registry,
           WUFEI_NOT_VALID_SLOT,/*0xFF*/
           sizeof(WUFEI_line_registry));
   line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;
   line_registry->clock_rec_line_params = tmp_clk_rec_line_params;
   /* Clear unused CR parameters (for UFE4, or to be set later here) */
   WUFEI_LineClockRecClearParams(line_registry);

   /* copy to the registry the important info */
   line_registry->line_id       = line_cfg->cad_id;
   line_registry->line_mode     = WUFE_STRUCT_LINE_CAD;
   line_registry->state_mode    = WUFEI_LINE_EMPTY;
   line_registry->n_attach_phy  = 0;
   line_registry->n_rx_active_phy  = 0;
   line_registry->n_tx_active_phy  = 0;
   line_registry->transfer_type = line_cfg->transfer_type;
   line_registry->framed        = line_cfg->framed;
   line_registry->ufe_core_id   = WUFEI_CAD_CORE_ID_GET(line_cfg->cad_id);
   line_registry->stm1_id       = 0;
   line_registry->spe_id        = 0;
   line_registry->sub_spe_entry = 0;
   line_registry->hmvip_num     = WUFE_HMVIP_NON;
   line_registry->cas_enable    = line_cfg->cas_enable;
   line_registry->clock_rec_mode = line_cfg->clock_rec_mode;
   line_registry->tx_clock_rec_if_id = 0xffff;
   line_registry->ext_mode_active = line_cfg->ext_mode_active;
   line_registry->rx_clock_rec_if_id = line_cfg->rx_clk_rec_if;
   line_registry->t1oe1_mode = WUFE_DISABLE;
   if (line_cfg->clock_rec_line_params)
   {
      memcpy(line_registry->clock_rec_line_params, line_cfg->clock_rec_line_params, sizeof(WUFE_line_clock_rec_params));
   }

   /* Changed in WUFEI_LineTdmConfig*/
   memset(line_registry->tdm_ram_mng, (WUFEI_NOT_VALID_SLOT)&0xff,
          sizeof(WP_U16)* WUFE_MAX_HMVIP_INUSE * 2 /*RX and TX */ * WUFE_N_SLOTS_E1);

   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      /* check clock recovery configuration parameters */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_LineCadClockRecConfig(system_registry, line_registry));

      if(line_registry->transfer_type == WUFE_CAD_TYPE_E1 ||
         line_registry->transfer_type == WUFE_CAD_TYPE_T1 )
      {
         if (WUFEI_SystemHwConfigFullModel(system_registry) == WUFE_TRUE)
         {
            /* configure the board support for E1/T1 sync generation */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_BsLineCadSetSyncType(
                                     wufe_id,
                                     line_registry->transfer_type,
                                     line_registry->line_id,
                                     system_registry->cfg.line_interface));
         }
      }

      /* The following line registry properties are not initiated:
       * spe_tdm_cad_id
       * hmvip_num
       */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwCadCheckAndConfig(&hw_cad_mode,
                                                      line_cfg,
                                                      line_registry));

      /* Get the TDM<-->PHY entry/entries */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_LineTdmConfig(system_registry,
                                                line_registry,
                                                &hw_cad_tdm_ram_id));

      /* set the mode */
      WUFEI_HwCadModeSet(wufe_id, line_cfg->cad_id, hw_cad_mode);

      /* set the id */
      WUFEI_HwCadIdSet(wufe_id, line_cfg->cad_id, hw_cad_tdm_ram_id);
   }
   else
   {
      registry_entry = line_cfg->cad_id / WUFEI_SPE_LINE_ADDRESS_SPACE;
      sub_registry_entry = line_cfg->cad_id % WUFEI_SPE_LINE_ADDRESS_SPACE;

      if (system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id ==
          WUFEI_EMPTY_TDM_ENTRY)
      {/* the 32slot unit is empty*/
         system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id = line_registry->line_id;
         line_registry->spe_id = registry_entry;
         line_registry->sub_spe_entry = sub_registry_entry;
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log,
                         "tdm_line_arr[%d][%d].line_id 0x%x is not empty\n",
                         registry_entry,
                         sub_registry_entry,
                         system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id));

         return WUFE_ERR_ENTRY_NOT_EMPTY;
      }

      /* Configure the tdm line mode table */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmLineConfig(system_registry,
                                                  line_registry,
                                                  line_cfg->cad_id));

      /* In case of ces and cas - configure the machine cas table */
      if (WUFEI_SystemHwConfigTransMachineValid(system_registry,line_registry->ufe_core_id) &&
          line_registry->cas_enable == WUFE_CAS_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwCesCasConfig(system_registry,
                                                    line_registry->ufe_core_id,
                                                    line_cfg->cad_id));
      }
   }

   /********************************************
    * commit in the registry
    * ----------------------
    * Note the : spe_id  and the spe_tdm_cad_id
    * are changed in WUFEI_LineTdmConfig.
    ********************************************/
   line_registry->state_mode    =  WUFEI_LINE_DISABLE;

   /* build the line handle */
   *line_handle=(WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_LINE) |
                 WUFEI_HANDLE_SUB_TYPE_F(WUFE_HANDLE_SUB_TYPE_CAD) |
                 WUFEI_HANDLE_UFE_ID_F(wufe_id) |
                 WUFEI_HANDLE_INDEX_F(line_cfg->cad_id) );

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSbiCfgCheck
 *
 * Purpose:   check the configuration of lines type SBI
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     line_cfg           The user configuration
 *     system_registry    UFE system registry pointer
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be re`turned:
 *
 * Called by:
 *     WUFEI_LineSbiConfig.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSbiCfgCheck(WUFEI_system_registry *system_registry,
                                         WUFE_line_sbi         *line_cfg)
{
   if (line_cfg->spe_num >= WUFE_MAX_N_SBI_SPE )
   {
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:line_cfg->spe_id %d\n",line_cfg->spe_num));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   if (line_cfg->sub_spe_id >= WUFE_MAX_N_LINE_PER_SPE)
   {/* illegal line number spe or sun spe*/
      WUFE_PRINT_ERR(
         (wufei_log, "ERROR:sbi.sub_spe_id %d  \n", line_cfg->sub_spe_id));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   if (system_registry->cfg.sbi.transfer_type_spe[line_cfg->spe_num] == WUFE_SBI_TYPE_IDLE)
   {
      WUFE_PRINT_ERR((wufei_log, "\nLineCreate for SBI that is IDLE \n"));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   /* checked a ready id the begging of WUFEI_LineSbiConfig
      line_cfg->spe_num
      line_cfg->sub_spe_id;*/
   if (line_cfg->framed        > 1 ||
       line_cfg->tx_clk_master > 1 ||
       line_cfg->cas_enable    > 1 ||
       line_cfg->t1oe1_mode    > 1)
   {
      WUFE_PRINT_ERR((wufei_log,
      "\ninvalid config framed  %d \n cas_enable %d tx_clk_master %d t1oe1_mode %d\n",
                      line_cfg->framed,
                      line_cfg->cas_enable,
                      line_cfg->tx_clk_master,
                      line_cfg->t1oe1_mode));
      return WUFE_ERR_INVALID_CONFIG;
   }

   if (line_cfg->tx_clk_master && (WUFEI_SystemClkRecHwConfigCtEn(system_registry) == WUFE_TRUE))
   {
      WUFE_status status;

      /* if clock translator exists and initialized, check that this line is compatible with
       * the clock translator settings */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_SbiClockTranslatorValidate(
                               system_registry,
                               line_cfg->clock_translator_id,
                               line_cfg->framed,
                               system_registry->cfg.sbi.transfer_type_spe[line_cfg->spe_num]));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSbiClockRecConfig
 *
 * Purpose:   check the configuration of lines type Generic
 *
 *
 * Description:
 *
 * Inputs:
 *         system_registry
 *         line_registry
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be re`turned:
 *
 * Called by:
 *     .WUFEI_LineSbiConfig
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSbiClockRecConfig(WUFEI_system_registry *system_registry,
                                               WUFEI_line_registry *line_registry)
{
   WUFE_status status;

   if(line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX)
      WUFEI_RETURN_IF_ERROR(status,
                           WUFEI_ClockRecRxInterfaceCreate(system_registry,
                                                           line_registry));

   if((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) ||
      (line_registry->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY))
   {
      WUFEI_RETURN_IF_ERROR(
         status,
         WUFEI_ClockRecTxInterfaceIdValidate(system_registry,
                                             line_registry->transfer_type,
                                             line_registry->clock_translator_id));

      line_registry->tx_clock_rec_if_id = line_registry->clock_translator_id;

      if (WUFEI_SystemHwConfigFullModel(system_registry) == WUFE_TRUE &&
         (line_registry->transfer_type == WUFE_SBI_TYPE_E3 ||
          line_registry->transfer_type == WUFE_SBI_TYPE_DS3))
      {
         WP_U8 ds3e3_mode;
         WP_U8 ds3e3_dco_clock_div;

         WUFEI_RETURN_IF_ERROR(
            status,
            WUFEI_BsTiclkInSpeConnect(system_registry->wufe_id,
                                      line_registry->spe_id,
                                      line_registry->clock_translator_id));

         if(line_registry->transfer_type == WUFE_SBI_TYPE_E3)
         {
            ds3e3_mode = WUFE_TRUE;
            ds3e3_dco_clock_div = WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_1074;
         }
         else
         {
            ds3e3_mode = WUFE_FALSE;
            ds3e3_dco_clock_div = WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_1398;
         }

         WUFEI_RETURN_IF_ERROR(
            status,
            WUFEI_HwClockRecInclkMuxSelConfig(system_registry->wufe_id,
                                              line_registry->spe_id,
                                              ds3e3_mode,
                                              line_registry->framed,
                                              ds3e3_dco_clock_div,
                                              WUFE_TRUE));

      }

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSbiCoreStm1SpeMap
 *
 * Purpose:   return the mapping of the STM1/SPE for line registry
 *
 *
 * Description:
 *
 *
 * Inputs:
 *
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_LineSbiConfig
 *
 ****************************************************************************/
static void WUFEI_LineSbiCoreStm1SpeMap(WP_U32 idx, WP_U32* core, WP_U32 *stm1, WP_U32 *spe)
{
   struct map
   {
         WP_U8 core;
         WP_U8 stm1;
         WP_U8 spe;

   } array[] = { {0,0,0 }, {0,0,1 }, {0,0,2 },
                 {0,1,0 }, {0,1,1 }, {0,1,2 },
                 {1,0,0 }, {1,0,1 }, {1,0,2 },
                 {1,1,0 }, {1,1,1 }, {1,1,2 }};

   *core = array[idx].core;
   *stm1 = array[idx].stm1;
   *spe = array[idx].spe;

   return;

}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSbiConfig
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
WUFE_status WUFEI_LineSbiConfig(WUFE_line_sbi         *line_cfg,
                                WUFEI_system_registry *system_registry,
                                WUFE_handle           *line_handle)
{
   WUFEI_line_registry   *line_registry;
   WUFE_status            status;
   WP_U16                 registry_line_id;
   WP_U16                 hw_tdm_reg_dummy;
   WUFEI_tdm_slots       *tmp_tdm_ram_mng_ptr;
   WP_U32                 core, stm1, spe;
   WP_U32                 wufe_id = system_registry->wufe_id;

   /* validate the user configuration*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineSbiCfgCheck(system_registry, line_cfg));

   /* map spe_num 0-11 to spe 0-3, stm1 0-1, core 0-1 */
   WUFEI_LineSbiCoreStm1SpeMap(line_cfg->spe_num, &core, &stm1, &spe);

   /* line_id is calculated in the following way:
      core_id*168 + stm*84 + spe*28 + sub_spe_id */
   registry_line_id =
      core * WUFE_MAX_N_STM1 * WUFE_MAX_N_SPE * WUFE_MAX_N_LINE_PER_SPE +
      stm1 * WUFE_MAX_N_SPE * WUFE_MAX_N_LINE_PER_SPE +
      spe * WUFE_MAX_N_LINE_PER_SPE + line_cfg->sub_spe_id;

   if (registry_line_id >= system_registry->cfg.max_line_number)
   {
      WUFE_PRINT_ERR(
         (wufei_log,
          "line_id( %d ) >= system_registry->cfg.max_line_number( %d )\n",
          registry_line_id, system_registry->cfg.max_line_number));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   line_registry = &system_registry->line_arr[registry_line_id];

   if (line_registry->state_mode != WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Line already configed (line_registry->state_mode=0x%X)\n",
                      line_registry->state_mode));

      return WUFE_ERR_LINE_ALREADY_CONFIGURED;
   }

   /* Set all the refistry to 0xff specificly important to
      the Phy array to for the DS0 slot.(each entry points to a phy) */
   tmp_tdm_ram_mng_ptr = line_registry->tdm_ram_mng;
   memset (line_registry, WUFEI_NOT_VALID_SLOT,/*0xFF*/ sizeof(WUFEI_line_registry));
   line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;

   /* copy the important info to the registry  */
   line_registry->line_mode     = WUFE_STRUCT_LINE_SBI;
   line_registry->n_attach_phy  = 0;
   line_registry->n_rx_active_phy  = 0;
   line_registry->n_tx_active_phy  = 0;
   line_registry->framed        = line_cfg->framed;
   line_registry->ufe_core_id   = core; /*mapped*/
   line_registry->stm1_id       = stm1; /*mapped*/
   line_registry->spe_id        = spe;  /*mapped*/
   line_registry->sub_spe_entry = line_cfg->sub_spe_id;
   line_registry->line_id       = registry_line_id;
   line_registry->hmvip_num     = WUFE_HMVIP_NON;
   line_registry->cas_enable    = line_cfg->cas_enable;
   line_registry->transfer_type =
      system_registry->cfg.sbi.transfer_type_spe[line_cfg->spe_num];
   line_registry->tx_clk_master =  line_cfg->tx_clk_master;
   line_registry->clock_translator_id = line_cfg->clock_translator_id;
   line_registry->clock_rec_mode =  line_cfg->clock_rec_mode;
   line_registry->tx_clock_rec_if_id = 0xffff;
   line_registry->ext_mode_active = 0; /* this field relevant for CAD mode only */
   line_registry->rx_clock_rec_if_id = line_cfg->rx_clk_rec_if;/*0-31 Adap + reg diff, 32-367 ext Diff */
   line_registry->t1oe1_mode = line_cfg->t1oe1_mode;


   /* attach the line to the TX/RX clock recovery*/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineSbiClockRecConfig(system_registry,line_registry));

   /* Changed in WUFEI_LineTdmConfig*/
   memset(line_registry->tdm_ram_mng, (WUFEI_NOT_VALID_SLOT)&0xff,
          sizeof(WP_U16) * 1 * 2 /*RX and TX */ * WUFE_N_SLOTS_E1);

   /* Get the TDM<-->PHY entry/entries */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineTdmConfig(system_registry,
                                             line_registry,
                                             &hw_tdm_reg_dummy));

   /* commits initial HW configuration */
   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwSbiRamConfig(system_registry, line_registry));

   /********************************************
    * commit in the registry
    * ----------------------
    * Note the : spe_id  and the spe_tdm_cad_id
    * are changed in WUFEI_LineTdmConfig.
    ********************************************/
   line_registry->state_mode = WUFEI_LINE_DISABLE;

   /* build the line handle */
   *line_handle=(WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_LINE) |
                 WUFEI_HANDLE_SUB_TYPE_F(WUFE_HANDLE_SUB_TYPE_SBI) |
                 WUFEI_HANDLE_UFE_ID_F(wufe_id) |
                 WUFEI_HANDLE_INDEX_F(registry_line_id));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineGenericConfig
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
static WUFE_status WUFEI_LineGenericConfig(WUFE_line_generic     *line_cfg,
                                           WUFEI_system_registry *system_registry,
                                           WUFE_handle           *line_handle)
{
   WUFEI_line_registry   *line_registry;
   WUFE_status status;
   WP_U16 registry_line_id;
   WP_U16 hw_tdm_reg_dummy;
   WUFEI_tdm_slots* tmp_tdm_ram_mng_ptr;
   WP_U32 wufe_id = system_registry->wufe_id;

   /* validate the user configuration*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineGenericCfgCheck(line_cfg));

   /* line_id is calculated in the following way:
      core_id*168 + stm*84 + spe*28 + sub_spe_id */
   registry_line_id =
      line_cfg->ufe_core_id * WUFE_MAX_N_STM1 * WUFE_MAX_N_SPE * WUFE_MAX_N_LINE_PER_SPE +
      line_cfg->stm1 * WUFE_MAX_N_SPE * WUFE_MAX_N_LINE_PER_SPE +
      line_cfg->spe * WUFE_MAX_N_LINE_PER_SPE +
      (WUFEI_SUB_SPE(line_cfg->tug2, line_cfg->tu));

   if (registry_line_id >= system_registry->cfg.max_line_number)
   {
      WUFE_PRINT_ERR(
         (wufei_log,
          "line_id( %d ) >= system_registry->cfg.max_line_number( %d )\n",
          registry_line_id, system_registry->cfg.max_line_number));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   line_registry = &system_registry->line_arr[registry_line_id];

   if (line_registry->state_mode != WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Line already configured (line_registry->state_mode=0x%X)\n",
                      line_registry->state_mode));

      return WUFE_ERR_LINE_ALREADY_CONFIGURED;
   }

   /* Set all the refistry to 0xff specificly important to
      the Phy array to for the DS0 slot.(each entry points to a phy) */
   tmp_tdm_ram_mng_ptr = line_registry->tdm_ram_mng;
   memset (line_registry, WUFEI_NOT_VALID_SLOT,/*0xFF*/ sizeof(WUFEI_line_registry));
   line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;

   /* copy to the registry the important info */
   line_registry->line_mode     = WUFE_STRUCT_LINE_GENERIC;
   line_registry->n_attach_phy  = 0;
   line_registry->n_rx_active_phy  = 0;
   line_registry->n_tx_active_phy  = 0;
   line_registry->framed        = line_cfg->framed;
   line_registry->ufe_core_id   = line_cfg->ufe_core_id;
   line_registry->stm1_id       = line_cfg->stm1;
   line_registry->spe_id        = line_cfg->spe;
   line_registry->sub_spe_entry = WUFEI_SUB_SPE(line_cfg->tug2,line_cfg->tu);
   line_registry->line_id       = registry_line_id;
   line_registry->hmvip_num     = WUFE_HMVIP_NON;
   line_registry->cas_enable    = line_cfg->cas_enable;
   line_registry->transfer_type = line_cfg->transfer_type;
   line_registry->tx_clk_master = WUFE_LINE_CLOCK_SLAVE;
   line_registry->clock_translator_id = WUFE_LINE_CLOCK_TRANSLATE_DISABLE;
   line_registry->clock_rec_mode =  WUFE_LINE_CLOCK_TRANSLATE_ORIG_CLK;
   line_registry->t1oe1_mode = WUFE_DISABLE;

   /* Changed in WUFEI_LineTdmConfig*/
   memset(line_registry->tdm_ram_mng,(WUFEI_NOT_VALID_SLOT)& 0xff,
          sizeof(WP_U16) * 1 * 2 /*RX and TX */ * WUFE_N_SLOTS_E1);

   /* Get the TDM<-->PHY entry/entries */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineTdmConfig(system_registry,
                                             line_registry,
                                             &hw_tdm_reg_dummy));

   /* assign a valid line state*/
   line_registry->state_mode = WUFEI_LINE_DISABLE;

   /* build the line handle */
   *line_handle=(WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_LINE) |
                 WUFEI_HANDLE_SUB_TYPE_F(WUFE_HANDLE_SUB_TYPE_GENERIC) |
                 WUFEI_HANDLE_UFE_ID_F(wufe_id) |
                 WUFEI_HANDLE_UFE_CORE_ID_F(line_cfg->ufe_core_id)|
                 WUFEI_HANDLE_INDEX_F(registry_line_id));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSdhConfig
 *
 * Purpose:   configure the registery for the lines type SDH
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry The registry entry created for this UFE
 *
 * Outputs:
 *     write the registry info for this line
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_LineCreate
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSdhConfig(WUFE_line_sdh         *line_cfg,
                                       WUFEI_system_registry *system_registry,
                                       WUFE_handle           *line_handle)
{
   WUFEI_line_registry   *line_registry;
   WUFE_line_clock_rec_params *tmp_clk_rec_line_params;
   WUFE_status status;
   WP_U16 registry_line_id, tdm_hw_entry, tug2_entry;
   WP_U16 i, j, registry_entry, sub_registry_entry;
   WUFEI_tdm_slots* tmp_tdm_ram_mng_ptr;
   WP_U32 vfp_index;
   WP_U32 wufe_id = system_registry->wufe_id;

   /* validate the user configuration*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineSdhCfgCheck(system_registry, line_cfg));

   /* line_id is calculated */
   registry_line_id =
      (line_cfg->stm4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->stm1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->stm0 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->tug2 * WUFE_MAX_N_TU) +
      line_cfg->tu;

   if (registry_line_id >= system_registry->cfg.max_line_number)
   {
      WUFE_PRINT_ERR(
         (wufei_log,
          "line_id( %d ) >= system_registry->cfg.max_line_number( %d )\n",
          registry_line_id, system_registry->cfg.max_line_number));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   line_registry = &system_registry->line_arr[registry_line_id];

   if (line_registry->state_mode != WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Line already configured (line_registry->state_mode=0x%X)\n",
                      line_registry->state_mode));

      return WUFE_ERR_LINE_ALREADY_CONFIGURED;
   }

   tmp_tdm_ram_mng_ptr = line_registry->tdm_ram_mng;
   tmp_clk_rec_line_params = line_registry->clock_rec_line_params;
   memset (line_registry, WUFEI_NOT_VALID_SLOT,/*0xFF*/ sizeof(WUFEI_line_registry));
   line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;
   line_registry->clock_rec_line_params = tmp_clk_rec_line_params;
   /* Clear unused CR parameters */
   WUFEI_LineClockRecClearParams(line_registry);

   /* copy to the registry the important info */
   line_registry->line_mode     = WUFE_STRUCT_LINE_SDH;
   line_registry->n_attach_phy  = 0;
   line_registry->n_rx_active_phy  = 0;
   line_registry->n_tx_active_phy  = 0;
   line_registry->framed        = line_cfg->framed;
   line_registry->ufe_core_id   = line_cfg->stm4 / WUFEI_NUM_AUG4_PER_CORE;
   line_registry->aug4          = line_cfg->stm4;
   line_registry->aug1          = line_cfg->stm1;
   line_registry->au3           = line_cfg->stm0;
   line_registry->tug2          = line_cfg->tug2;
   line_registry->tu            = line_cfg->tu;
   line_registry->line_id       = registry_line_id;
   line_registry->cas_enable    = line_cfg->cas_enable;
   line_registry->transfer_type = line_cfg->transfer_type;
   line_registry->pdh_loopback_type=WUFE_LINE_LOOPBACK_PDH_TYPE_NONE;
   if (line_cfg->clock_rec_line_params)
   {
      memcpy(line_registry->clock_rec_line_params, line_cfg->clock_rec_line_params, sizeof(WUFE_line_clock_rec_params));
   }

   memset(line_registry->tdm_ram_mng,(WUFEI_NOT_VALID_SLOT)& 0xff,
          sizeof(WP_U16) * 2 * WUFE_N_SLOTS_E1);

   /*Calculate the entry of the tdm for WUFEI_HwTdmLineConfig*/
   tdm_hw_entry =
      ((line_cfg->stm4 % WUFEI_NUM_AUG4_PER_CORE) * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->stm1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->stm0 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->tug2 * WUFE_MAX_N_TU) +
      line_cfg->tu;

   /*Caclculate the registry and sub_registry entries to find if the registry space for this line*/
   /*is already taken, and if it is not, mark that now it is.*/
   registry_entry =
      (line_cfg->stm4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) +
      (line_cfg->stm1 * WUFE_MAX_N_AU3) +
      line_cfg->stm0;

   sub_registry_entry =
      (line_cfg->tug2 * WUFE_MAX_N_TU) +
      line_cfg->tu;

   switch(line_registry->transfer_type)
   {
      /*********************/
      /******* VC44c *******/
      /*********************/
      case WUFE_SDH_TYPE_VC44C:
      {
         if(line_registry->tug2 != 0 ||
            line_registry->tu != 0 ||
            line_registry->au3 != 0 ||
            line_registry->aug1 != 0)
         {
            return WUFE_ERR_VC44C_STM1_STM0_TUG2_TU_MUST_BE_ZERO;
         }

         for (i=line_cfg->stm4;i<(line_cfg->stm4+WUFE_MAX_N_AUG1*WUFE_MAX_N_AU3);i++)
            for (j=0;j<(WUFE_MAX_N_TUG2*WUFE_MAX_N_TU);j++)
         {
            if (system_registry->tdm_line_arr[i][j].line_id != WUFEI_EMPTY_TDM_ENTRY)
               return WUFE_ERR_ENTRY_NOT_EMPTY;

            system_registry->tdm_line_arr[i][j].line_id = line_registry->line_id;
         }

         break;
      }
      /*******************/
      /******* VC4 *******/
      /*******************/
      case WUFE_SDH_TYPE_VC4:
      {
         if(line_registry->tug2 != 0 ||
            line_registry->tu != 0 ||
            line_registry->au3 != 0)
         {
            return WUFE_ERR_VC4_STM0_TUG2_TU_MUST_BE_ZERO;
         }

         for (i=((line_cfg->stm4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3)+(line_cfg->stm1 * WUFE_MAX_N_AU3));
              i<(((line_cfg->stm4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3)+(line_cfg->stm1 * WUFE_MAX_N_AU3))+WUFE_MAX_N_AU3);
              i++)
            for (j=0;j<(WUFE_MAX_N_TUG2*WUFE_MAX_N_TU);j++)
            {
               if (system_registry->tdm_line_arr[i][j].line_id != WUFEI_EMPTY_TDM_ENTRY)
                  return WUFE_ERR_ENTRY_NOT_EMPTY;
               
               system_registry->tdm_line_arr[i][j].line_id = line_registry->line_id;
            }

         break;
      }
      /**************************/
      /******* DS3 E3 VC3 *******/
      /**************************/
      case WUFE_SDH_TYPE_DS3:
      case WUFE_SDH_TYPE_E3:
      case WUFE_SDH_TYPE_VC3:
      {
         if(line_registry->tug2 != 0 || line_registry->tu != 0)
         {
            return WUFE_ERR_VC3_TUG2_TU_MUST_BE_ZERO;
         }

         for (i=0;i<(WUFE_MAX_N_TUG2*WUFE_MAX_N_TU);i++)
         {
            if (system_registry->tdm_line_arr[registry_entry][i].line_id != WUFEI_EMPTY_TDM_ENTRY)
               return WUFE_ERR_ENTRY_NOT_EMPTY;

            system_registry->tdm_line_arr[registry_entry][i].line_id = line_registry->line_id;
         }

         break;
      }
      /***********************/
      /*** E1 T1 VC11 VC12 ***/
      /***********************/
      case WUFE_SDH_TYPE_E1:
      case WUFE_SDH_TYPE_T1:
      case WUFE_SDH_TYPE_VC11:
      case WUFE_SDH_TYPE_VC12:
      {
         if (system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id ==
             WUFEI_EMPTY_TDM_ENTRY)
         {/* the 32slot unit is empty*/
            system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id = line_registry->line_id;
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log,
                            "tdm_line_arr[%d][%d].line_id 0x%x is not empty\n",
                            registry_entry,
                            sub_registry_entry,
                            system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id));

            return WUFE_ERR_ENTRY_NOT_EMPTY;
         }
         break;
      }
      default:
         return WUFE_ERR_HANDLE;
   }

   /* Configure the tdm line mode table */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwTdmLineConfig(system_registry,
                                               line_registry,
                                               tdm_hw_entry));

   /* In case of ces and cas - configure the machine cas table */
   if (WUFEI_SystemHwConfigTransMachineValid(system_registry,line_registry->ufe_core_id) &&
       line_registry->cas_enable == WUFE_CAS_ENABLE)
   {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwCesCasConfig(system_registry,
                                                    line_registry->ufe_core_id,
                                                    tdm_hw_entry));
   }

   tug2_entry =
      line_cfg->stm4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 +
      line_cfg->stm1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 +
      line_cfg->stm0 * WUFE_MAX_N_TUG2 +
      line_cfg->tug2;

   if (system_registry->tug2_arr[tug2_entry].num_of_lines ==0)
   {
      system_registry->tug2_arr[tug2_entry].transfer_type = line_cfg->transfer_type;
   }
   system_registry->tug2_arr[tug2_entry].num_of_lines++;
   vfp_index = (WUFE_MAX_N_AUG1 * line_registry->au3) + line_registry->aug1;

   if ( (line_registry->transfer_type == WUFE_SDH_TYPE_VC44C) ||
        (line_registry->transfer_type == WUFE_SDH_TYPE_VC4) ||
        (line_registry->transfer_type == WUFE_SDH_TYPE_DS3) ||
        (line_registry->transfer_type == WUFE_SDH_TYPE_E3))
   {
      WUFEI_HwFmVfpRxReset(wufe_id, line_registry->ufe_core_id, vfp_index);
      WUFEI_HwFmVfpTxSet(wufe_id, line_registry->ufe_core_id, vfp_index,
                         line_registry->transfer_type);
   }

   /* assign a valid line state*/
   line_registry->state_mode = WUFEI_LINE_DISABLE;

   /* build the line handle */
   *line_handle=(WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_LINE) |
                 WUFEI_HANDLE_SUB_TYPE_F(WUFE_HANDLE_SUB_TYPE_SDH) |
                 WUFEI_HANDLE_UFE_ID_F(wufe_id) |
                 WUFEI_HANDLE_UFE_CORE_ID_F(line_registry->ufe_core_id)|
                 WUFEI_HANDLE_INDEX_F(registry_line_id));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSonetConfig
 *
 * Purpose:   configure the registery for the lines type SONET
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry The registry entry created for this UFE
 *
 * Outputs:
 *     write the registry info for this line
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_LineCreate
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSonetConfig(WUFE_line_sonet         *line_cfg,
                                         WUFEI_system_registry *system_registry,
                                         WUFE_handle           *line_handle)
{
   WUFEI_line_registry   *line_registry;
   WUFE_line_clock_rec_params *tmp_clk_rec_line_params;
   WUFE_status status;
   WP_U16 registry_line_id, tdm_hw_entry, tug2_entry;
   WP_U16 i, j, registry_entry, sub_registry_entry;
   WUFEI_tdm_slots* tmp_tdm_ram_mng_ptr;
   WP_U32 wufe_id = system_registry->wufe_id;
   WP_U32 vfp_index;

   /* validate the user configuration*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineSonetCfgCheck(system_registry, line_cfg));

   /* line_id is calculated */
   registry_line_id =
      (line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->sts3 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->sts1 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->vt_group * WUFE_MAX_N_TU) +
      line_cfg->vt;

   if (registry_line_id >= system_registry->cfg.max_line_number)
   {
      WUFE_PRINT_ERR(
         (wufei_log,
          "line_id( %d ) >= system_registry->cfg.max_line_number( %d )\n",
          registry_line_id, system_registry->cfg.max_line_number));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   line_registry = &system_registry->line_arr[registry_line_id];

   if (line_registry->state_mode != WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Line already configured (line_registry->state_mode=0x%X)\n",
                      line_registry->state_mode));

      return WUFE_ERR_LINE_ALREADY_CONFIGURED;
   }

   tmp_tdm_ram_mng_ptr = line_registry->tdm_ram_mng;
   tmp_clk_rec_line_params = line_registry->clock_rec_line_params;
   memset (line_registry, WUFEI_NOT_VALID_SLOT,/*0xFF*/ sizeof(WUFEI_line_registry));
   line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;
   line_registry->clock_rec_line_params = tmp_clk_rec_line_params;
   /* Clear unused CR parameters */
   WUFEI_LineClockRecClearParams(line_registry);

   /* copy to the registry the important info */
   line_registry->line_mode     = WUFE_STRUCT_LINE_SONET;
   line_registry->n_attach_phy  = 0;
   line_registry->n_rx_active_phy  = 0;
   line_registry->n_tx_active_phy  = 0;
   line_registry->framed        = line_cfg->framed;
   line_registry->ufe_core_id   = line_cfg->sts12 / WUFEI_NUM_AUG4_PER_CORE;
   line_registry->aug4          = line_cfg->sts12;
   line_registry->aug1          = line_cfg->sts3;
   line_registry->au3           = line_cfg->sts1;
   line_registry->tug2          = line_cfg->vt_group;
   line_registry->tu            = line_cfg->vt;
   line_registry->line_id       = registry_line_id;
   line_registry->cas_enable    = line_cfg->cas_enable;
   line_registry->transfer_type = line_cfg->transfer_type;
   line_registry->pdh_loopback_type=WUFE_LINE_LOOPBACK_PDH_TYPE_NONE;
   if (line_cfg->clock_rec_line_params)
   {
      memcpy(line_registry->clock_rec_line_params, line_cfg->clock_rec_line_params, sizeof(WUFE_line_clock_rec_params));
   }

   memset(line_registry->tdm_ram_mng,(WUFEI_NOT_VALID_SLOT)& 0xff,
          sizeof(WP_U16) *2* WUFE_N_SLOTS_E1);

   /*Calculate the entry of the tdm for WUFEI_HwTdmLineConfig*/
   tdm_hw_entry =
      ((line_cfg->sts12 % WUFEI_NUM_AUG4_PER_CORE) * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->sts3 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->sts1 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
      (line_cfg->vt_group * WUFE_MAX_N_TU) +
      line_cfg->vt;

   /*Caclculate the registry and sub_registry entries to find if the registry space for this line*/
   /*is already taken, and if it is not, mark that now it is.*/
   registry_entry =
      (line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) +
      (line_cfg->sts3 * WUFE_MAX_N_AU3) +
      line_cfg->sts1;

   sub_registry_entry =
      (line_cfg->vt_group * WUFE_MAX_N_TU) +
      line_cfg->vt;

   switch (line_registry->transfer_type)
   {
      /*********************/
      /******* sts12 *******/
      /*********************/
      case WUFE_SONET_TYPE_STS12:
      {
         if (line_registry->tug2 != 0 ||
             line_registry->tu != 0 ||
             line_registry->au3 != 0 ||
             line_registry->aug1 != 0)
         {
            return WUFE_ERR_VC44C_STM1_STM0_TUG2_TU_MUST_BE_ZERO;
         }

         for (i=(line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3);
              i<((line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) + (WUFE_MAX_N_AUG1*WUFE_MAX_N_AU3));
              i++)
            for (j=0;j<WUFE_MAX_N_TUG2*WUFE_MAX_N_TU;j++)
            {
               if (system_registry->tdm_line_arr[i][j].line_id != WUFEI_EMPTY_TDM_ENTRY)
                  return WUFE_ERR_ENTRY_NOT_EMPTY;
               
               system_registry->tdm_line_arr[i][j].line_id = line_registry->line_id;
            }

         break;
      }
      /********************/
      /******* STS3 *******/
      /********************/
      case WUFE_SONET_TYPE_STS3:
      {
         if(line_registry->tug2 != 0 ||
            line_registry->tu != 0 ||
            line_registry->au3 != 0)
         {
            return WUFE_ERR_VC4_STM0_TUG2_TU_MUST_BE_ZERO;
         }

         for (i=(line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) + (line_cfg->sts3 * WUFE_MAX_N_AU3);
              i<((line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) + (line_cfg->sts3 * WUFE_MAX_N_AU3) + WUFE_MAX_N_AU3);
              i++)
            for (j=0;j<WUFE_MAX_N_TUG2*WUFE_MAX_N_TU;j++)
            {
               if (system_registry->tdm_line_arr[i][j].line_id != WUFEI_EMPTY_TDM_ENTRY)
                  return WUFE_ERR_ENTRY_NOT_EMPTY;
               
               system_registry->tdm_line_arr[i][j].line_id = line_registry->line_id;
            }
         
         break;
      }
      /******************************/
      /******* DS3 E3 STS1SPE *******/
      /******************************/
      case WUFE_SONET_TYPE_DS3:
      case WUFE_SONET_TYPE_E3:
      case WUFE_SONET_TYPE_STS1_SPE:
      {
         if(line_registry->tug2 != 0 || line_registry->tu != 0)
         {
            return WUFE_ERR_VC3_TUG2_TU_MUST_BE_ZERO;
         }

         for (i=0;i<WUFE_MAX_N_TUG2*WUFE_MAX_N_TU;i++)
         {
            if (system_registry->tdm_line_arr[registry_entry][i].line_id != WUFEI_EMPTY_TDM_ENTRY)
               return WUFE_ERR_ENTRY_NOT_EMPTY;

            system_registry->tdm_line_arr[registry_entry][i].line_id = line_registry->line_id;
         }

         break;
      }
      /***********************/
      /*** E1 T1 VT1_5 VT2 ***/
      /***********************/
      case WUFE_SONET_TYPE_E1:
      case WUFE_SONET_TYPE_T1:
      case WUFE_SONET_TYPE_VT1_5:
      case WUFE_SONET_TYPE_VT2:
      {
         if (system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id ==
             WUFEI_EMPTY_TDM_ENTRY)
         {/* the 32slot unit is empty*/
            system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id = line_registry->line_id;
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log,
                            "tdm_line_arr[%d][%d].line_id 0x%x is not empty\n",
                            registry_entry,
                            sub_registry_entry,
                            system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id));

            return WUFE_ERR_ENTRY_NOT_EMPTY;
         }
         break;
      }
      default:
         return WUFE_ERR_HANDLE;
   }

   /* Configure the tdm line mode table */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwTdmLineConfig(system_registry,
                                               line_registry,
                                               tdm_hw_entry));

   /* In case of ces and cas - configure the machine cas table */
   if (WUFEI_SystemHwConfigTransMachineValid(system_registry,line_registry->ufe_core_id) &&
       line_registry->cas_enable == WUFE_CAS_ENABLE)
   {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwCesCasConfig(system_registry,
                                                    line_registry->ufe_core_id,
                                                    tdm_hw_entry));
   }

   /* Update tug2 registry */
   tug2_entry =
      line_cfg->sts12 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 +
      line_cfg->sts3 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 +
      line_cfg->sts1 * WUFE_MAX_N_TUG2 +
      line_cfg->vt_group;

   if (system_registry->tug2_arr[tug2_entry].num_of_lines ==0)
   {
      system_registry->tug2_arr[tug2_entry].transfer_type = line_cfg->transfer_type;
   }
   system_registry->tug2_arr[tug2_entry].num_of_lines++;
   vfp_index = (WUFE_MAX_N_AUG1 * line_registry->au3) + line_registry->aug1;

   if ( (line_registry->transfer_type == WUFE_SONET_TYPE_STS12) ||
        (line_registry->transfer_type == WUFE_SONET_TYPE_STS3) ||
        (line_registry->transfer_type == WUFE_SONET_TYPE_E3) ||
        (line_registry->transfer_type == WUFE_SONET_TYPE_DS3))
   {
      WUFEI_HwFmVfpRxReset(wufe_id, line_registry->ufe_core_id, vfp_index);
      WUFEI_HwFmVfpTxSet(wufe_id, line_registry->ufe_core_id, vfp_index,
                         line_registry->transfer_type);
   }

   /* assign a valid line state*/
   line_registry->state_mode = WUFEI_LINE_DISABLE;

   /* build the line handle */
   *line_handle=(WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_LINE) |
                 WUFEI_HANDLE_SUB_TYPE_F(WUFE_HANDLE_SUB_TYPE_SONET) |
                 WUFEI_HANDLE_UFE_ID_F(wufe_id) |
                 WUFEI_HANDLE_UFE_CORE_ID_F(line_registry->ufe_core_id)|
                 WUFEI_HANDLE_INDEX_F(registry_line_id));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineDccConfig
 *
 * Purpose:   configure the registery for the line type DCC
 *
 *
 * Description:
 *
 * Inputs:
 *     line_cfg - The user configuration for the line
 *     system_registry - The registry entry created for this UFE
 *     line_handle - handle for the line returned to the application
 *
 * Outputs:
 *     write the registry info for this line, create a line handle
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_LineCreate
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineDccConfig(WUFE_line_dcc         *line_cfg,
                                       WUFEI_system_registry *system_registry,
                                       WUFE_handle           *line_handle)
{
   WUFEI_line_registry   *line_registry;
   WUFE_status status;
   WP_U16 registry_line_id, max_lines=0;
   WP_U32 wufe_id = system_registry->wufe_id;
   WUFEI_tdm_slots* tmp_tdm_ram_mng_ptr;

   /* validate the user configuration*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineDccCfgCheck(system_registry, line_cfg));

   if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12)
   {
      max_lines = WUFE_MAX_OC12_LINES;
   }
   else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
   {
      max_lines = WUFE_MAX_OC48_LINES;
   }

   /* line_id is calculated */
   if (line_cfg->transfer_type == WUFE_DCC_TYPE_MS)
   {/*MS DCC is the first channel for each optical port*/
      registry_line_id = max_lines + (line_cfg->line_port_id *2);
   }
   else /*WUFE_DCC_TYPE_RS*/
   {/*RS DCC is the second channel for each optical port*/
      registry_line_id = max_lines + (line_cfg->line_port_id * 2) +1;
   }

   if (registry_line_id >= system_registry->cfg.max_line_number)
   {
      WUFE_PRINT_ERR(
         (wufei_log,
          "line_id( %d ) >= system_registry->cfg.max_line_number( %d )\n",
          registry_line_id, system_registry->cfg.max_line_number));
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }

   line_registry = &system_registry->line_arr[registry_line_id];

   if (line_registry->state_mode != WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Line already configured (line_registry->state_mode=0x%X)\n",
                      line_registry->state_mode));

      return WUFE_ERR_LINE_ALREADY_CONFIGURED;
   }

   tmp_tdm_ram_mng_ptr = line_registry->tdm_ram_mng;
   memset (line_registry, WUFEI_NOT_VALID_SLOT,/*0xFF*/ sizeof(WUFEI_line_registry));
   line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;

   /* Clear unused CR parameters */
   WUFEI_LineClockRecClearParams(line_registry);

   /* copy to the registry the important info */
   line_registry->line_mode = WUFE_STRUCT_LINE_DCC;
   line_registry->n_attach_phy = 0;
   line_registry->n_rx_active_phy = 0;
   line_registry->n_tx_active_phy = 0;

   if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
   {
      if (system_registry->cfg.build_personality ==  WUFE_FRAMER_PERSONALITY_448_8_OC3)
      {
         line_registry->ufe_core_id = line_cfg->line_port_id/4;
      }
      else if (system_registry->cfg.build_personality ==  WUFE_FRAMER_PERSONALITY_448_4_OC12)
      {
         line_registry->ufe_core_id = line_cfg->line_port_id/2;
      }
   }
   else
   {
      line_registry->ufe_core_id = 0;
   }
   
   line_registry->line_id = registry_line_id;
   line_registry->transfer_type = line_cfg->transfer_type;
   line_registry->framed = WUFE_UNFRAMED;
   line_registry->clock_rec_line_params =0;
   line_registry->cas_enable = 0;
   
   memset(line_registry->tdm_ram_mng,(WUFEI_NOT_VALID_SLOT)& 0xff, 
          sizeof(WP_U16) *2/*RX and TX */);

   /* assign a valid line state*/
   line_registry->state_mode = WUFEI_LINE_DISABLE;

   /* build the line handle */
   *line_handle=(WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_LINE) |
                 WUFEI_HANDLE_SUB_TYPE_F(WUFE_HANDLE_SUB_TYPE_DCC) |
                 WUFEI_HANDLE_UFE_ID_F(wufe_id) |
                 WUFEI_HANDLE_UFE_CORE_ID_F(line_registry->ufe_core_id)|
                 WUFEI_HANDLE_INDEX_F(registry_line_id));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadDelete
 *
 * Purpose:   delete the registery for the line type CAD
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry The registry entry created for this UFE
 *
 * Outputs:
 *     clear the registry info for this line
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_LineDelete
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadDelete(WUFEI_line_registry   *line_registry,
                                       WUFEI_system_registry *system_registry)
{
   WUFE_status status;

   /* delete the tdm line mode table */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwTdmLineDelete(system_registry,
                                               line_registry->ufe_core_id,
                                               line_registry->line_id));

   system_registry->tdm_line_arr[line_registry->spe_id][line_registry->sub_spe_entry].line_id = WUFEI_EMPTY_TDM_ENTRY;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSonetSdhDelete
 *
 * Purpose:   delete the registery for the lines type SONET or SDH
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry The registry entry created for this UFE
 *
 * Outputs:
 *     clear the registry info for this line
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_LineDelete
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSonetSdhDelete(WUFEI_line_registry   *line_registry,
                                            WUFEI_system_registry *system_registry)
{
   WP_U16 i, j, tdm_hw_entry, registry_entry, sub_registry_entry, tug2_entry;
   WUFE_status status;

   tdm_hw_entry =
      (line_registry->aug4 % WUFEI_NUM_AUG4_PER_CORE) * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU +
      line_registry->aug1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU +
      line_registry->au3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU +
      line_registry->tug2 * WUFE_MAX_N_TU +
      line_registry->tu;

   /* delete the tdm line mode table */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwTdmLineDelete(system_registry,
                                               line_registry->ufe_core_id,
                                               tdm_hw_entry));

   registry_entry =
      line_registry->aug4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 +
      line_registry->aug1 * WUFE_MAX_N_AU3 +
      line_registry->au3;

   sub_registry_entry =
      line_registry->tug2 * WUFE_MAX_N_TU +
      line_registry->tu;

   switch(line_registry->transfer_type)
   {
      /*********************/
      /******* sts12 *******/
      /*********************/
      case WUFE_SONET_TYPE_STS12:
      case WUFE_SDH_TYPE_VC44C:
      {
         for (i=(line_registry->aug4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3);
              i<((line_registry->aug4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) + WUFE_MAX_N_AUG1*WUFE_MAX_N_AU3);
              i++)
            for (j=0;j<WUFE_MAX_N_TUG2*WUFE_MAX_N_TU;j++)
            {
               system_registry->tdm_line_arr[i][j].line_id = WUFEI_EMPTY_TDM_ENTRY;
            }
         
         break;
      }
      /********************/
      /******* STS3 *******/
      /********************/
      case WUFE_SONET_TYPE_STS3:
      case WUFE_SDH_TYPE_VC4:
      {
         for (i=((line_registry->aug4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) + (line_registry->aug1 * WUFE_MAX_N_AU3));
              i<((line_registry->aug4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3) + (line_registry->aug1 * WUFE_MAX_N_AU3)) + WUFE_MAX_N_AU3;
              i++)
            for (j=0;j<(WUFE_MAX_N_TUG2*WUFE_MAX_N_TU);j++)
            {
               system_registry->tdm_line_arr[i][j].line_id = WUFEI_EMPTY_TDM_ENTRY;
            }
         
         break;
      }
      /******************************/
      /******* DS3 E3 STS1SPE *******/
      /******************************/
      case WUFE_SONET_TYPE_DS3:
      case WUFE_SONET_TYPE_E3:
      case WUFE_SONET_TYPE_STS1_SPE:
      case WUFE_SDH_TYPE_VC3:
      case WUFE_SDH_TYPE_DS3:
      case WUFE_SDH_TYPE_E3:
      {
         for (i=0;i<WUFE_MAX_N_TUG2*WUFE_MAX_N_TU;i++)
         {
            system_registry->tdm_line_arr[registry_entry][i].line_id = WUFEI_EMPTY_TDM_ENTRY;
         }
         break;
      }
      /***********************/
      /*** E1 T1 VT1_5 VT2 ***/
      /***********************/
      case WUFE_SONET_TYPE_E1:
      case WUFE_SONET_TYPE_T1:
      case WUFE_SONET_TYPE_VT1_5:
      case WUFE_SONET_TYPE_VT2:
      case WUFE_SDH_TYPE_E1:
      case WUFE_SDH_TYPE_T1:
      case WUFE_SDH_TYPE_VC11:
      case WUFE_SDH_TYPE_VC12:
      {
         system_registry->tdm_line_arr[registry_entry][sub_registry_entry].line_id = WUFEI_EMPTY_TDM_ENTRY;
         break;
      }
      default:
         return WUFE_ERR_HANDLE;
   }

   tug2_entry =
      line_registry->aug4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 +
      line_registry->aug1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 +
      line_registry->au3 * WUFE_MAX_N_TUG2 +
      line_registry->tug2;

   system_registry->tug2_arr[tug2_entry].num_of_lines--;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineTdmDelete
 *
 * Purpose:   clears the line TDM slots and tdm_line_arr in the line registry
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
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_LineDelete() function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineTdmDelete(WUFEI_line_registry   *line_registry,
                                       WUFEI_system_registry *system_registry)
{
   WP_U32      tdm_cad_id;
   WP_U32      line_id;
   WP_U32      tdm_slot_start_index;
   WP_U32      tdm_clear_size;
   WP_U32      id;
   WP_U32      spe_id,sub_spe,stm1_id, core_id;
   WP_U32      tdm_stm1, tdm_spe;

   line_id    = line_registry->line_id;
   tdm_cad_id = line_registry->sub_spe_entry;

   core_id = line_registry->ufe_core_id;
   stm1_id = line_registry->stm1_id;
   spe_id  = line_registry->spe_id;
   sub_spe = line_registry->sub_spe_entry;

   if (spe_id == WUFEI_LINE_SPE_IRRELEVANT)
      return WUFE_ERR_HANDLE;

   /*calculates the STM1/SPE TDM addr*/
   WUFEI_LineTdmStm1SpeIndexMap(line_registry, &tdm_stm1, &tdm_spe);

   /* Configuration is ready -> write to MEM */
   tdm_slot_start_index =
      ((tdm_stm1*(WUFE_MAX_N_SPE+1) + tdm_spe) * WUFEI_SPE_LINE_ADDRESS_SPACE +
       line_registry->sub_spe_entry) * WUFEI_N_SLOT_PER_LINE;

   tdm_clear_size = sizeof(WUFEI_hw_tdm_entry) * line_registry->hmvip_num * WUFEI_N_SLOT_PER_LINE;

   /************************/
   /* Clean the TDM RX RAM */
   /************************/
   id = WUFEI_ID(system_registry->wufe_id, line_registry->ufe_core_id);

   /* Clear the TDM RX RAM */
   WUFEI_IndirectMemset(id, WUFEI_RX_TDM(tdm_slot_start_index), 0x00, tdm_clear_size);

   /************************/
   /* Clean the TDM TX RAM */
   /************************/
   WUFEI_IndirectMemset(id, WUFEI_TX_TDM(tdm_slot_start_index), 0x00, tdm_clear_size);

   switch(line_registry->transfer_type)
   {
      case   WUFE_CAD_TYPE_E1:
      case   WUFE_CAD_TYPE_T1:
      case   WUFE_CAD_TYPE_E3:
      case   WUFE_CAD_TYPE_DS3:
      {
         /* check if the entry in the STS ARRAy is equal to the line id to be deleted */
         if (system_registry->tdm_line_arr[spe_id][tdm_cad_id].line_id != line_id)
            return WUFE_ERR_INCONSISTENT_SPE_ARRAY;

         system_registry->tdm_line_arr[spe_id][tdm_cad_id].line_id = WUFEI_EMPTY_TDM_CAD_ENTRY;
         break;
      }
      case   WUFE_SBI_TYPE_DS3:
      case   WUFE_SBI_TYPE_E3:
      {
         if(sub_spe != 0)
            return WUFE_ERR_DS3_SUB_SPE_MUST_BE_ZERO;
         /*fall through*/
      }
      case WUFE_SBI_TYPE_T1:
      case WUFE_SBI_TYPE_E1:
      case WUFE_GENERIC_TYPE_E1:
      case WUFE_GENERIC_TYPE_T1:
      {
         /*calculates the entry*/
         WP_U32 entry =
            core_id*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE + stm1_id*WUFE_MAX_N_SPE + spe_id;

         if (system_registry->tdm_line_arr[entry][sub_spe].line_id != line_id)
            return WUFE_ERR_INCONSISTENT_SPE_ARRAY;

         system_registry->tdm_line_arr[entry][sub_spe].line_id = WUFEI_EMPTY_TDM_CAD_ENTRY;

         break;
      }
      case   WUFE_CAD_TYPE_HMVIP_4_E1:
      case   WUFE_CAD_TYPE_HMVIP_4_T1:
      {
         /* check if the entry in the STS ARRAy is equal to the line id to be deleted */
         if ((system_registry->tdm_line_arr[spe_id][tdm_cad_id].line_id   != line_id) |
             (system_registry->tdm_line_arr[spe_id][tdm_cad_id+1].line_id != line_id) |
             (system_registry->tdm_line_arr[spe_id][tdm_cad_id+2].line_id != line_id) |
             (system_registry->tdm_line_arr[spe_id][tdm_cad_id+3].line_id != line_id))

            return WUFE_ERR_INCONSISTENT_SPE_ARRAY;

         system_registry->tdm_line_arr[spe_id][tdm_cad_id].line_id   = WUFEI_EMPTY_TDM_CAD_ENTRY;
         system_registry->tdm_line_arr[spe_id][tdm_cad_id+1].line_id = WUFEI_EMPTY_TDM_CAD_ENTRY;
         system_registry->tdm_line_arr[spe_id][tdm_cad_id+2].line_id = WUFEI_EMPTY_TDM_CAD_ENTRY;
         system_registry->tdm_line_arr[spe_id][tdm_cad_id+3].line_id = WUFEI_EMPTY_TDM_CAD_ENTRY;
         break;
      }

      default:
         return WUFE_ERR_HANDLE;

   }
   if(line_registry->line_mode == WUFE_STRUCT_LINE_CAD)
   {
      line_registry->sub_spe_entry = 0xffff;
      line_registry->spe_id = WUFEI_LINE_SPE_IRRELEVANT;
   }

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_LineTdmStm1SpeIndexMap
 *
 * Purpose:   return the mapping of the STM1/SPE for line TDM entry
 *
 *
 * Description:
 *
 *
 * Inputs:
 *
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_LineGetTdmHwPtr
 *
 ****************************************************************************/
static void WUFEI_LineTdmStm1SpeIndexMap(WUFEI_line_registry *line_registry,
                                         WP_U32 *stm1, WP_U32 *spe)
{
   if(line_registry->transfer_type == WUFE_GENERIC_TYPE_E1 ||
      line_registry->transfer_type == WUFE_GENERIC_TYPE_T1 ||
      line_registry->transfer_type == WUFE_GENERIC_TYPE_DS3 ||
      line_registry->transfer_type == WUFE_GENERIC_TYPE_E3)
   {
      WP_U16 array[7][2] = { {0x0,0x0},
                             {0x0,0x2},
                             {0x1,0x1},
                             {0xff,0xff},
                             {0x0,0x1},
                             {0x1,0x0},
                             {0x1,0x2}};
      WP_U32 entry = (((line_registry->stm1_id & 0x1)<<2) |
                      (line_registry->spe_id & 0x3));

      *stm1 = array[entry][0];
      *spe = array[entry][1];
   }
   else
   {
      *stm1 = line_registry->stm1_id;
      *spe = line_registry->spe_id;
   }

   return;

}

/*****************************************************************************
 *
 * Function:  WUFEI_LineGetTdmHwPtr
 *
 * Purpose:   retrun the HW pointer for the TDM RAM SLOT
 *
 *
 * Description: This function claculate the
 *
 *
 *
 *
 * Inputs:
 *
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_LineGetTdmHwPtr(WUFEI_hw_tdm_entry **tdm_ram_slot_ptr,
                                  WP_U16               direction,
                                  WUFEI_line_registry *line_registry,
                                  WP_U16               slot_id)
{
   WP_U32 tu, tug2, spe, stm1;
   WP_U16 tdm_entry_index;

   if (line_registry->stm1_id > WUFE_MAX_N_STM1)
   {
      WUFE_PRINT_ERR((wufei_log,"stm1_id > WUFE_MAX_N_STM1\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   if (line_registry->spe_id > WUFE_MAX_N_SPE)
   {
      WUFE_PRINT_ERR((wufei_log,"spe_id > WUFE_MAX_N_SPE\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   if (line_registry->transfer_type != WUFE_CAD_TYPE_HMVIP_4_E1 &&
       line_registry->transfer_type != WUFE_CAD_TYPE_HMVIP_4_T1 &&
       line_registry->sub_spe_entry > WUFEI_N_SLOT_PER_LINE)
   {
      WUFE_PRINT_ERR((wufei_log,"sub_spe_entry > WUFEI_N_SLOT_PER_LINE\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   if (slot_id > line_registry->hmvip_num)
   {
      WUFE_PRINT_ERR((wufei_log,"hmvip_id > line_registry->hmvip_num \n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   tu = WUFEI_TU_V(line_registry->sub_spe_entry);
   tug2 = WUFEI_TUG2_V(line_registry->sub_spe_entry);

   /*calculates the STM1/SPE TDM addr*/
   WUFEI_LineTdmStm1SpeIndexMap(line_registry, &stm1, &spe);

   tdm_entry_index =
      WUFEI_TDM_ENTRY_SLOT_ID_F(slot_id)|
      WUFEI_TDM_ENTRY_TU_ID_F(tu)|
      WUFEI_TDM_ENTRY_TUG2_ID_F(tug2)|
      WUFEI_TDM_ENTRY_SPE_ID_F(spe)|
      WUFEI_TDM_ENTRY_STM1_ID_F(stm1);

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3 &&
       (line_registry->transfer_type == WUFE_CAD_TYPE_HMVIP_4_E1 ||
        line_registry->transfer_type == WUFE_CAD_TYPE_HMVIP_4_T1))
      tdm_entry_index = WUFEI_N_SLOT_PER_LINE*(line_registry->sub_spe_entry%128)
         + slot_id*WUFEI_N_SLOT_PER_LINE;

   if(direction == WUFEI_LINE_TDM_TX)
      *tdm_ram_slot_ptr = WUFEI_TX_TDM(tdm_entry_index);
   else if(direction == WUFEI_LINE_TDM_RX)
      *tdm_ram_slot_ptr = WUFEI_RX_TDM(tdm_entry_index);
   else
      return WUFE_ERR_INVALID_DIRECTION;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineTdmConfig
 *
 * Purpose:  1) Allocation of TDM HW slot for the Line.
 *           2) Clear the area in the TDM HW RX amd TX.
 *           CAD: assign SPE_id and sub_SPE_entry.
 *           SBI: assign sub_SPE_entry
 *
 *            Manage the TDM-CAD-ID array.
 *            the TDM-CAD-ID is an offset used by the HW to point from the LINE(CAD) to
 *            the right TDM-RAM entries.
 *            the TDM RAM is in groups of 32 slots for the E1.T1, in goups of 64 slots.
 *            for HMV-IP *2.In goups of is an array of all the
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
 *     write to external HW of the lines mode registers (CAD)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineTdmConfig(WUFEI_system_registry *system_registry,
                                       WUFEI_line_registry *line_registry,
                                       WP_U16 *hw_cad_tdm_ram_id)

{

   WP_U32 first_free=0, i;
   WP_U32 found_empty=0;
   WP_U32 tdm_slot_start_index;

   *hw_cad_tdm_ram_id =0xffff;

   if(line_registry->line_mode == WUFE_STRUCT_LINE_CAD)
   {
      WP_U32 sts_count;
      switch(line_registry->transfer_type)
      {
         case WUFE_CAD_TYPE_E3:
         case WUFE_CAD_TYPE_DS3:
            for (sts_count = 0;sts_count<WUFE_MAX_N_SPE;sts_count++)
            {
               if (system_registry->tdm_line_arr[sts_count][0].line_id==WUFEI_EMPTY_TDM_CAD_ENTRY)
               {
                  found_empty=1;
                  system_registry->tdm_line_arr[sts_count][0].line_id=line_registry->line_id;
                  *hw_cad_tdm_ram_id = WUFEI_CAD_ID_E3_T3_F(sts_count);
                  line_registry->sub_spe_entry=0;
                  break;
               }
            }

            if (!found_empty)
            {
               return WUFE_ERR_SPE_NOT_EMPTY;
            }
            break;
         case WUFE_CAD_TYPE_E1:
         case WUFE_CAD_TYPE_T1:
         {
            for (sts_count = 0;sts_count<WUFE_MAX_N_SPE;sts_count++)
            {
               if (found_empty)
               {/*an empty slot was found in the prevois SPE*/
                  break;
               }

               for (first_free=0;first_free<WUFEI_SPE_LINE_ADDRESS_SPACE;first_free++)
               {
                  if (system_registry->tdm_line_arr[sts_count][first_free].line_id ==
                      WUFEI_EMPTY_TDM_CAD_ENTRY)
                  {/* the 32slot unit is empty*/
                     system_registry->tdm_line_arr[sts_count][first_free].line_id =
                        line_registry->line_id;
                     *hw_cad_tdm_ram_id=(WUFEI_CAD_ID_E1_T1_F(first_free)|
                                         WUFEI_CAD_ID_SPE_F(sts_count));
                     line_registry->spe_id=sts_count;
                     line_registry->sub_spe_entry=first_free;
                     found_empty=1;
                     break;
                  }
               }
            }

            if (!found_empty)
            {
               return WUFE_ERR_CAD_SPE_FULL_OR_WITH_SPACES;
            }
            break;
         }
         /***********************/
         /*** 4*E1   4*T1  ******/
         /***********************/
         case WUFE_CAD_TYPE_HMVIP_4_E1:
         case WUFE_CAD_TYPE_HMVIP_4_T1:
         {
            WUFE_status status;
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwCadTdmConfig(system_registry,
                                                       line_registry,
                                                       hw_cad_tdm_ram_id));
            found_empty=1;

            break;
         }

         default:
            return WUFE_ERR_HANDLE;

      }/*transfer type*/
   }/* CAD mode STS is not assigned from the user*/
   else
   {/* SBI mode WUFE_STRUCT_LINE_SBI or WUFE_STRUCT_LINE_GENERIC  */
      WP_U32 spe_id,sub_spe,stm1_id, core_id;

      core_id = line_registry->ufe_core_id;
      stm1_id = line_registry->stm1_id;
      spe_id  = line_registry->spe_id;
      sub_spe = line_registry->sub_spe_entry;

      switch(line_registry->transfer_type)
      {
         /***********************/
         /******* DS3 T3   ******/
         /***********************/
         case WUFE_SBI_TYPE_DS3:
         case WUFE_SBI_TYPE_E3:
         {
            WP_U32 entry =
               core_id*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE + stm1_id*WUFE_MAX_N_SPE + spe_id;

            for (i=0;i<WUFEI_SPE_LINE_ADDRESS_SPACE;i++)
            {
               if (system_registry->tdm_line_arr[entry][i].line_id != WUFEI_EMPTY_TDM_CAD_ENTRY)
                  return WUFE_ERR_SPE_NOT_EMPTY;
            }
            if(line_registry->sub_spe_entry != 0)
            {
               return WUFE_ERR_DS3_SUB_SPE_MUST_BE_ZERO;
            }

            system_registry->tdm_line_arr[entry][0].line_id = line_registry->line_id;
            *hw_cad_tdm_ram_id = WUFEI_CAD_ID_E3_T3_F(entry);

            found_empty=1;
            break;
         }
         /***********************/
         /***   E1 T1      ******/
         /***********************/
         case WUFE_SBI_TYPE_E1:
         case WUFE_SBI_TYPE_T1:
         case WUFE_GENERIC_TYPE_E1:
         case WUFE_GENERIC_TYPE_T1:
         {
            /*calculates the next empty entry*/
            WP_U32 entry =
               core_id*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE + stm1_id*WUFE_MAX_N_SPE + spe_id;


            if (system_registry->tdm_line_arr[entry][sub_spe].line_id ==
                WUFEI_EMPTY_TDM_CAD_ENTRY)
            {
               /* the 32slot unit is empty*/
               system_registry->tdm_line_arr[entry][sub_spe].line_id = line_registry->line_id;
               found_empty=1;
            }
            else
            {
               WUFE_PRINT_ERR((wufei_log,
               "tdm_line_arr[core_id %d stm1_id %d spe_id %d][sub_spe %d].line_id 0x%x != ",
                               core_id, stm1_id, spe_id, sub_spe,
                               ((WP_U32) (system_registry->tdm_line_arr[entry][sub_spe].line_id))));

               WUFE_PRINT_ERR((wufei_log,
                               "WUFEI_EMPTY_TDM_CAD_ENTRY 0x%x\n",
                               ((WP_U32) WUFEI_EMPTY_TDM_CAD_ENTRY)));
               return WUFE_ERR_SPE_NOT_EMPTY;
            }
            break;
         }
         default:
            return WUFE_ERR_HANDLE;
      }

   }/* end of SBI mode SPE has to be 1 to 3*/

   if (!found_empty)
      return WUFE_ERR_HANDLE;
   else
   {
      WP_U32 tdm_clear_size, id;
      WP_U32 stm1, spe;

      /*calculates the STM1/SPE TDM addr*/
      WUFEI_LineTdmStm1SpeIndexMap(line_registry, &stm1, &spe);

      /* Configuration is ready -> write to MEM */
      tdm_slot_start_index = ((stm1*(WUFE_MAX_N_SPE+1) + spe) * WUFEI_SPE_LINE_ADDRESS_SPACE +
                              line_registry->sub_spe_entry) * WUFEI_N_SLOT_PER_LINE;

      tdm_clear_size =  (sizeof(WUFEI_hw_tdm_entry) * line_registry->hmvip_num *
                         WUFEI_N_SLOT_PER_LINE);
      id = WUFEI_ID(system_registry->wufe_id, line_registry->ufe_core_id);

      /************************/
      /* Clean the TDM RX RAM */
      /************************/
      WUFEI_IndirectMemset(id, WUFEI_RX_TDM(tdm_slot_start_index), 0x00, tdm_clear_size);

      /************************/
      /* Clean the TDM TX RAM */
      /************************/
      WUFEI_IndirectMemset(id, WUFEI_TX_TDM(tdm_slot_start_index), 0x00, tdm_clear_size);

      return WUFE_OK;
   }
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadCheckDecreaseOrder
 *
 * Purpose:
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     system_registry
 *     line_cfg
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadCheckDecreaseOrder(WUFEI_system_registry *system_registry,
                                                   WUFE_line_cad         *line_cfg)
{
   WP_U32 i;

   if (line_cfg->transfer_type != WUFE_CAD_TYPE_T1)
   {
      for (i=0;i< line_cfg->cad_id;i++)
      {
         if(system_registry->line_arr[i].state_mode != WUFEI_LINE_EMPTY)
         {
            if(system_registry->line_arr[i].transfer_type>line_cfg->transfer_type)
               return WUFE_ERR_LINES_NOT_SORTED_IN_DECREASING_ORDER;
         }
      }
   }
   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadDisable
 *
 * Purpose:   disables the CAD HW configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     line_registry
 *     wufe_id
 *     direction
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be re`turned:
 *       WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *     WUFE_LineDisable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadDisable( WUFEI_line_registry *line_registry,
                                         WP_U32 wufe_id,
                                         WP_U32 direction)
{
   WP_U16 enb_mask, mode;
   ((void)line_registry);
   ((void)wufe_id);   
   if (direction == WUFE_FULL_DUPLEX)
   {
      enb_mask=(WP_U16)(~(WUFEI_CAD_RX_MODE_ENABLE_M |
                          WUFEI_CAD_TX_MODE_ENABLE_M));
   }
   else if (direction == WUFE_RX_DIRECTION)
   {
      enb_mask=(WP_U16)(~(WUFEI_CAD_RX_MODE_ENABLE_M));
   }
   else if (direction == WUFE_TX_DIRECTION)
   {
      enb_mask=(WP_U16)(~(WUFEI_CAD_TX_MODE_ENABLE_M));
   }
   else
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   /* call MASK MEM clean command on Line mode */
   mode = WUFEI_HwCadModeGet(wufe_id, line_registry->line_id);
   mode &= enb_mask;

   WUFEI_HwCadModeSet(wufe_id, line_registry->line_id, mode);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSbiDisable
 *
 * Purpose:   disables the SBI HW configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry
 *     line_registry
 *     direction
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be re`turned:
 *
 * Called by:
 *     WUFE_LineDisable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSbiDisable( WUFEI_system_registry *system_registry,
                                         WUFEI_line_registry *line_registry,
                                         WP_U32 direction)
{
   WP_U32 wufe_id = system_registry->wufe_id;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if ( (line_registry->tx_clk_master &&
         (WUFEI_SystemClkRecHwConfigCtEn(system_registry) == WUFE_TRUE)))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_SbiClockTranslatorRelease(system_registry,
                                         line_registry->line_id,
                                         line_registry->clock_translator_id),
         wufe_id);
   }

   if(direction & WUFE_RX_DIRECTION)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFE_HwSbiRxDisable(line_registry, wufe_id));
   }

   if(direction & WUFE_TX_DIRECTION)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFE_HwSbiTxDisable(line_registry, wufe_id));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_LineDisable
 *
 * Purpose:   Disables the UFE Line according to the handle and direction
 *
 *
 * Description:
 *     This function disables the EMPHY modules of the UFE system.
 *     The disable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     line_handel The UFE Line.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *     WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_LineDisable(WUFE_handle ufe_line_handle, WP_U32 direction)
{
   WUFE_status status;
   WP_U32 wufe_id, line_id;
   WUFEI_line_registry   *line_registry;
   WUFEI_system_registry *system_registry;

   wufe_id=WUFEI_HANDLE_UFE_ID_V(ufe_line_handle);
   line_id=WUFEI_HANDLE_INDEX_V (ufe_line_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,
                                  &system_registry,status);

   if (line_id >= system_registry->cfg.max_line_number)
   {/* illegal line number */
      WUFE_PRINT_ERR(
         (wufei_log,
          "ERROR: cfg->cad_id(%d) >= sys_reg->cfg.max_line_number( %d ) \n",
          line_id,
          system_registry->cfg.max_line_number));
      return WUFE_ERR_LINE_ILLEGAL_ID ;
   }

   line_registry = &system_registry->line_arr[line_id];

   if (line_registry->state_mode == WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFE:ERROR the line is empty\n"));

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   else
   {
      if (line_registry->state_mode == WUFEI_LINE_DISABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_LINE_ALREADY_DISABLED;
      }
   }

   if (line_registry->n_rx_active_phy !=0 ||
       line_registry->n_tx_active_phy !=0)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "line_registry->n_rx_active_phy %d line_registgry->n_tx_active_phy %d\n",
                      line_registry->n_rx_active_phy, line_registry->n_tx_active_phy));

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_CONTAIN_ACTIVE_PHYS;
   }

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (line_registry->clock_rec_line_params)
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_ClockRecLineDisable(wufe_id, system_registry, line_id, line_registry),
                                     wufe_id);

      line_registry->state_mode = WUFEI_LINE_DISABLE;

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_OK;
   }

   if (line_registry->line_mode == WUFE_STRUCT_LINE_CAD)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_LineCadDisable(line_registry, wufe_id, direction),
         wufe_id);

   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SBI)
   {

      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_LineSbiDisable(system_registry, line_registry, direction),
         wufe_id);
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_GENERIC)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_LineGenericDisable(line_registry, wufe_id, direction),
         wufe_id);
   }

   /* This CR section is not relevant for UFE4 (only UFE3) */
   /* move RX clock generator to another source line*/
   if(line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX)
   {
      WP_U8 method;

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_ClockRecMethodGet(system_registry, &method),
                                     wufe_id);

      if (!(method == WUFE_CLOCK_REC_METHOD_ADAPTIVE &&
            line_registry->line_mode == WUFE_STRUCT_LINE_CAD))
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_ClockRecRxInterfaceDisable(system_registry,
                                             line_registry->rx_clock_rec_if_id,
                                             line_registry->line_id),
            wufe_id);
      }
   }

   if ((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) ||
       (line_registry->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY))
   {
      /* disable clock dividers */
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_ClockRecConfigDivider(wufe_id,
                                     system_registry,
                                     line_registry->tx_clock_rec_if_id,
                                     line_registry->transfer_type,
                                     0),
         wufe_id);

      /* Disable CR module in BS */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_BsLineCadClockRecDisable(
                               wufe_id,
                               system_registry->cfg.line_interface,
                               line_registry->line_id));
   }

   line_registry->state_mode = WUFEI_LINE_DISABLE;

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadEnable
 *
 * Purpose:   enables the CAD HW configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     line_registry
 *     wufe_id
 *     direction
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be re`turned:
 *
 * Called by:
 *     WUFE_LineEnable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadEnable(WUFEI_line_registry *line_registry,
                                       WP_U32 wufe_id,
                                       WP_U32 direction)
{
   WP_U16 enb_mask, mode;
   ((void)line_registry);
   ((void)wufe_id);   
   if (direction == WUFE_FULL_DUPLEX)
   {
      enb_mask=(WP_U16)(WUFEI_CAD_RX_MODE_ENABLE_M |
                        WUFEI_CAD_TX_MODE_ENABLE_M);
   }
   else if (direction ==WUFE_RX_DIRECTION)
   {
      enb_mask=(WP_U16)(WUFEI_CAD_RX_MODE_ENABLE_M);
   }
   else if (direction ==WUFE_TX_DIRECTION)
   {
      enb_mask=(WP_U16)(WUFEI_CAD_TX_MODE_ENABLE_M);
   }
   else
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   /* call MASK MEM set command on LINE.mode.enable*/
   mode = WUFEI_HwCadModeGet(wufe_id, line_registry->line_id);
   mode |= enb_mask;

   WUFEI_HwCadModeSet(wufe_id, line_registry->line_id, mode);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSbiEnable
 *
 * Purpose:   enables the SBI HW configuration
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry
 *     line_registry
 *     direction
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be re`turned:
 *
 * Called by:
 *     WUFE_LineEnable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSbiEnable(WUFEI_system_registry *system_registry,
                                       WUFEI_line_registry *line_registry,
                                       WP_U32 direction)
{
   WUFE_status status;
   ((void)system_registry);
   ((void)line_registry);   
   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction &  WUFE_RX_DIRECTION)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwSbiRxConfig(system_registry, line_registry, WUFE_ENABLE));
   }

   if (direction  &  WUFE_TX_DIRECTION )
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwSbiTxConfig(system_registry, line_registry, WUFE_ENABLE));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_LineEnable
 *
 * Purpose:   Enables the UFE Line according to the handle and direction
 *
 *
 * Description:
 *     This function enables the EMPHY modules of the UFE system.
 *     The enable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     line_handel The UFE Line.
 *     direction   The UFE direction to be enabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *     WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_LineEnable(WUFE_handle ufe_line_handle, WP_U32 direction)
{
   WUFE_status status;
   WP_U32 wufe_id, line_id;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;

   wufe_id=WUFEI_HANDLE_UFE_ID_V(ufe_line_handle);
   line_id=WUFEI_HANDLE_INDEX_V (ufe_line_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,
                                  &system_registry,status);

   if(system_registry->sys_enable.state_mode != WUFEI_SYSTEM_ENABLE)
   {
      WUFE_PRINT_ERR(
         (wufei_log,
          "ERROR: line enable must be after system(emphy) enable\n"));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_SYSTEM_NOT_ENABLED;
   }

   if (line_id >= system_registry->cfg.max_line_number)
   {/* illegal line number */
      WUFE_PRINT_ERR(
         (wufei_log,
          "ERROR: cfg->cad_id(%d) >= sys_reg->cfg.max_line_number( %d ) \n",
          line_id,
          system_registry->cfg.max_line_number));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_ILLEGAL_ID ;
   }

   line_registry =&system_registry->line_arr[line_id];
   if (line_registry->state_mode == WUFEI_LINE_EMPTY)
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   else
   {
      if (line_registry->state_mode == WUFEI_LINE_ENABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_LINE_ALREADY_ACTIVE;
      }
   }

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      line_registry->state_mode = WUFEI_LINE_ENABLE;

      if (line_registry->clock_rec_line_params)
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_ClockRecLineEnable(wufe_id, system_registry,
                                                                 line_id, line_registry),
                                        wufe_id);

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_OK;
   } /* WUFEI_FPGA_TYPE_UFE4 */

   /* This CR section is not relevant for UFE4 (only UFE3) */
   if((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) ||
      (line_registry->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY))
   {
      /* enable clock dividers */
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_ClockRecConfigDivider(wufe_id,
                                     system_registry,
                                     line_registry->tx_clock_rec_if_id,
                                     line_registry->transfer_type,
                                     1),
         wufe_id);

      /* Enable CR module in BS */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_BsLineCadClockRecEnable(
                               wufe_id,
                               system_registry->cfg.line_interface,
                               line_registry->line_id));
   }

   if (line_registry->line_mode == WUFE_STRUCT_LINE_CAD)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_LineCadEnable(line_registry,
                                                         wufe_id,
                                                         direction),
                                     wufe_id);
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SBI)
   {

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_LineSbiEnable(system_registry,
                                                         line_registry,
                                                         direction),
                                     wufe_id);


   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_GENERIC)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_LineGenericEnable(line_registry,
                                                             wufe_id,
                                                             direction),
                                     wufe_id);

   }
   else
   {
      WUFE_PRINT_ERR((wufei_log,
                      "protocol not CAD or SBI or Generic\n"));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_INVALID_REGISTRY_STATE;
   }

   /* move RX clock generator to another source line*/
   if(line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX)
   {
      WP_U8 method;

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_ClockRecMethodGet(system_registry, &method),
                                     wufe_id);

      if (!(method == WUFE_CLOCK_REC_METHOD_ADAPTIVE &&
            line_registry->line_mode == WUFE_STRUCT_LINE_CAD))
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_ClockRecRxInterfaceEnable(system_registry,
                                            line_registry->rx_clock_rec_if_id,
                                            line_registry->line_id),
            wufe_id);
      }
   }

   line_registry->state_mode = WUFEI_LINE_ENABLE;

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadDisplay
 *
 * Purpose:   Displays the SBI line structures and HW registers
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
 *    WUFE_LineDisplay() function
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadDisplay(WUFEI_line_registry *line_registry, WP_U32 wufe_id)
{
   WUFE_status            status;
   WP_U32                 line_id;
   WP_U16                 cad_mode, cad_id;

   /**********************/
   /** read the HW CAD ***/
   /**********************/
   line_id = line_registry->line_id;


   cad_mode = WUFEI_HwCadModeGet(wufe_id, line_id);
   WUFE_PRINT_DISP((wufei_log,
                    " CAD - line id %d (HW CAD mode:0x%04X )\n",
                    line_id,
                    cad_mode));

   cad_id = WUFEI_HwCadIdGet(wufe_id, line_id);
   WUFE_PRINT_DISP((wufei_log,
                    " pointer to TDM RAM (registry: 0x%04x) (HW CAD id: 0x%04x)\n",
                    (line_registry->spe_id * WUFEI_SPE_LINE_ADDRESS_SPACE +
                     line_registry->sub_spe_entry),
                     cad_id));
   WUFE_PRINT_DISP((wufei_log,
                    " hmvip_num  %d\n",
                    line_registry->hmvip_num));

   if (line_registry->hmvip_num > WUFE_MAX_HMVIP_INUSE)
   {
      WUFE_PRINT_DISP((wufei_log,
                       "There is no HMVIP num is greater than the HW\n"));
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFE_ERR_INVALID_REGISTRY_STATE,
         wufe_id);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSbiDisplay
 *
 * Purpose:   Displays the SBI line structures and HW registers
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
 *    WUFE_LineDisplay() function
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSbiDisplay(WUFEI_line_registry *line_registry, WP_U32 wufe_id)
{
   WUFE_status status;
   ((void)wufe_id);
   if(line_registry->transfer_type == WUFE_SBI_TYPE_T1 ||
      line_registry->transfer_type == WUFE_SBI_TYPE_E1)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwSbiLineDisplayE1T1Rx(line_registry, wufe_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwSbiLineDisplayE1T1Tx(line_registry, wufe_id));
   }
   else if(line_registry->transfer_type == WUFE_SBI_TYPE_DS3 ||
           line_registry->transfer_type == WUFE_SBI_TYPE_E3)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwSbiLineDisplayDS3E3Rx(line_registry, wufe_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwSbiLineDisplayDS3E3Tx(line_registry, wufe_id));
   }
   else
   {
      return WUFE_ERR_INVALID_TRANSFER_TYPE;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_LineDisplay
 *
 * Purpose:   Display all parameter of the UFE line handle
 *
 *
 * Description:
 *     This function display all information of the UFE  modules
 *     of the UFE.
 *
 *
 * Inputs:
 *     line_handel The UFE Line handle
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
WUFE_status WUFE_LineDisplay(WUFE_handle ufe_line_handle)
{
   WUFE_status            status;
   WP_U32                 wufe_id, line_id, max_lines=0;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;
   WP_U16 tdm_entry;

   wufe_id=WUFEI_HANDLE_UFE_ID_V(ufe_line_handle);
   line_id=WUFEI_HANDLE_INDEX_V (ufe_line_handle);

   WUFE_PRINT_DISP((wufei_log,
                    "\n #####  LINE DISPLAY (wufe_id %d, line_id %d) #####\n",
                    wufe_id, line_id));

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,
                                  &system_registry,status);

   line_registry =&system_registry->line_arr[line_id];

   switch (line_registry->state_mode)
   {/* LINE STATE */
      case WUFEI_LINE_EMPTY:
         WUFE_PRINT_DISP((wufei_log, " The line is empty\n"));
         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFE_ERR_LINE_EMPTY,
            wufe_id);
      case WUFEI_LINE_DISABLE:
         WUFE_PRINT_DISP((wufei_log, " The line is Disable\n"));
         break;
      case  WUFEI_LINE_ENABLE:
         WUFE_PRINT_DISP((wufei_log, " The line is Enable\n"));
         break;
      default:
         WUFE_PRINT_ERR((wufei_log, " Invalid LINE state\n"));

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFE_ERR_INVALID_REGISTRY_STATE,
                                        wufe_id);
   }

   WUFE_PRINT_DISP((wufei_log, "\n Line Registry Parameters: \n"));
   WUFE_PRINT_DISP((wufei_log, " line_mode:     %s\n",
                    line_reg_line_mode[line_registry->line_mode]));
   WUFE_PRINT_DISP((wufei_log, " n_attach_phy:  %d\n", line_registry->n_attach_phy));
   WUFE_PRINT_DISP((wufei_log, " n_rx_active phy:  %d\n", line_registry->n_rx_active_phy));
   WUFE_PRINT_DISP((wufei_log, " n_tx_active phy:  %d\n", line_registry->n_tx_active_phy));
   WUFE_PRINT_DISP((wufei_log, " transfer_type: %s\n",
                    line_reg_transfer_type[line_registry->transfer_type]));
   WUFE_PRINT_DISP((wufei_log, " framed:        %s\n",
                    line_reg_framed[line_registry->framed]));
   WUFE_PRINT_DISP((wufei_log, " ufe_core_id:   %d\n", line_registry->ufe_core_id));
   if (line_registry->line_mode == WUFE_STRUCT_LINE_SONET)
   {
      WUFE_PRINT_DISP((wufei_log, " sts12:       %d\n", line_registry->aug4));
      WUFE_PRINT_DISP((wufei_log, " sts3 :       %d\n", line_registry->aug1));
      WUFE_PRINT_DISP((wufei_log, " sts1:        %d\n", line_registry->au3));
      WUFE_PRINT_DISP((wufei_log, " vt_group:    %d\n", line_registry->tug2));
      WUFE_PRINT_DISP((wufei_log, " vt:          %d\n", line_registry->tu));
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SDH)
   {
      WUFE_PRINT_DISP((wufei_log, " stm4:        %d\n", line_registry->aug4));
      WUFE_PRINT_DISP((wufei_log, " stm1 :       %d\n", line_registry->aug1));
      WUFE_PRINT_DISP((wufei_log, " stm0:        %d\n", line_registry->au3));
      WUFE_PRINT_DISP((wufei_log, " tug2:        %d\n", line_registry->tug2));
      WUFE_PRINT_DISP((wufei_log, " tu:          %d\n", line_registry->tu));
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_DCC)
   {
      if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12)
      {
         max_lines = WUFE_MAX_OC12_LINES;
      }
      else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
      {
         max_lines = WUFE_MAX_OC48_LINES;
      }

      line_id = (line_registry->line_id - max_lines) /2;
      
      WUFE_PRINT_DISP((wufei_log, " line_port_id:        %d\n", line_id));

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      
      return WUFE_OK;
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SBI ||
            line_registry->line_mode == WUFE_STRUCT_LINE_GENERIC)
   {
      WUFE_PRINT_DISP((wufei_log, " spe_id:        %d\n", line_registry->spe_id));
      WUFE_PRINT_DISP((wufei_log, " stm1_id:       %d\n", line_registry->stm1_id));
      WUFE_PRINT_DISP((wufei_log, " sub_spe_entry: %d\n", line_registry->sub_spe_entry));
      WUFE_PRINT_DISP((wufei_log, " tug2:          %d\n",
                       WUFEI_TUG2_V(line_registry->sub_spe_entry)));
      WUFE_PRINT_DISP((wufei_log, " tu:            %d\n", WUFEI_TU_V(line_registry->sub_spe_entry)));
   }

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFE_PRINT_DISP((wufei_log, " cas_enable ('1' - enabled , '0' - disabled) :  %d\n", line_registry->cas_enable));
      if(line_registry->cas_enable)
      {
         WUFEI_HwCesCasDisplay(system_registry,line_registry->ufe_core_id,line_id);
      }
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_LineLoopbackDisplay(line_registry, wufe_id),
                                           wufe_id);
      tdm_entry =
         ((line_registry->aug4 % WUFEI_NUM_AUG4_PER_CORE) * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->aug1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->au3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->tug2 * WUFE_MAX_N_TU) +
         line_registry->tu;

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_HwTdmLineDisplay(wufe_id,
                                                            line_registry->ufe_core_id,
                                                            tdm_entry),
                                     wufe_id);

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_OK;
   }

   if (line_registry->line_mode == WUFE_STRUCT_LINE_CAD)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_LineCadDisplay(line_registry, wufe_id),
                                      wufe_id);

   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SBI)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                      WUFEI_LineSbiDisplay(line_registry, wufe_id),
                                      wufe_id);
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_GENERIC)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_LineLoopbackDisplay(line_registry, wufe_id),
                                     wufe_id);
   }
   else
   {
      WUFE_PRINT_DISP((wufei_log, "incorect line mode\n"));
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFE_ERR_INVALID_REGISTRY_STATE,
                                     wufe_id);
   }

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_LineDelete
 *
 * Purpose:   Deletes the UFE Line according to the handle.
 *
 *
 * Description:
 *     This function delete the UFE line.
 *     only disabled Line with no Phy attached to it can be disabled
 *
 *
 * Inputs:
 *     line_handel The UFE Line.
 *
 * Outputs:
 *     write to UFE HW of the disabled line.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_LINE_ILLEGAL_ID
 *     WUFE_ERR_LINE_NOT_DISABLED
 *     WUFE_ERR_LINE_CONTAIN_PHYS
 *
 *
 *
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_LineDelete(WUFE_handle ufe_line_handle)
{
   WUFE_status status;
   WP_U32 wufe_id, line_id, core_id;
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry   *line_registry;
   WUFEI_tdm_slots *  tmp_tdm_ram_mng_ptr;
   WUFE_line_clock_rec_params *tmp_clk_rec_line_params;

   wufe_id=WUFEI_HANDLE_UFE_ID_V(ufe_line_handle);
   line_id=WUFEI_HANDLE_INDEX_V (ufe_line_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   line_registry =&system_registry->line_arr[line_id];

   if (line_registry->state_mode == WUFEI_LINE_EMPTY)
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   else
   {
      if (line_registry->state_mode == WUFEI_LINE_ENABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_LINE_NOT_DISABLED;
      }
   }
   if (line_registry->n_attach_phy != 0)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "line_registry->n_attach_phy %d\n",
                      line_registry->n_attach_phy));

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_CONTAIN_PHYS;
   }

   tmp_tdm_ram_mng_ptr = line_registry->tdm_ram_mng;
   tmp_clk_rec_line_params = line_registry->clock_rec_line_params;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (line_registry->line_mode == WUFE_STRUCT_LINE_CAD)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_LineCadDelete(line_registry,system_registry),
            wufe_id);
      }
      if (line_registry->line_mode == WUFE_STRUCT_LINE_SONET ||
          line_registry->line_mode == WUFE_STRUCT_LINE_SDH)
      {
          if(line_registry->pdh_loopback_type!=WUFE_LINE_LOOPBACK_PDH_TYPE_NONE)
          {
              WUFE_LinePdhLoopbackConfigure(ufe_line_handle,
                                            WUFE_LINE_LOOPBACK_PDH_TYPE_NONE);
          }
          
          WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                         WUFEI_LineSonetSdhDelete(line_registry,
                                                                  system_registry),
                                         wufe_id);
      }

      memset (line_registry, WUFEI_NOT_VALID_SLOT,/*0xFF*/ sizeof(WUFEI_line_registry));
      line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;
      line_registry->clock_rec_line_params = tmp_clk_rec_line_params;
      /* Clear unused CR parameters */
      WUFEI_LineClockRecClearParams(line_registry);

      line_registry->state_mode    = WUFEI_LINE_EMPTY;

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_OK;
   }
   else
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_LineTdmDelete(line_registry,
                                                         system_registry),
                                     wufe_id);
   }

   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      core_id = line_registry->ufe_core_id;
      /*Delete SBI RAM entries for this line*/
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_HwSbiRamDelete(system_registry, line_registry, core_id),
         wufe_id);
   }

   if ((system_registry->cfg.line_interface == WUFE_INTERFACE_SBI) &&
       (line_registry->tx_clk_master &&
        (WUFEI_SystemClkRecHwConfigCtEn(system_registry) == WUFE_TRUE)))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(
         status,
         WUFEI_SbiClockTranslatorRelease(system_registry,
                                         line_id,
                                         line_registry->clock_translator_id),
         wufe_id);
   }

   if(line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX)
   {
      WP_U8 method;

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_ClockRecMethodGet(system_registry, &method),
                                     wufe_id);

      if (!(method == WUFE_CLOCK_REC_METHOD_ADAPTIVE &&
            line_registry->line_mode == WUFE_STRUCT_LINE_CAD))
      {
         /* release rx clock recovery interface id allocated during line create */
         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_ClockRecRxInterfaceDelete(system_registry,
                                            line_registry->rx_clock_rec_if_id,
                                            line_registry->line_id),
            wufe_id);

         line_registry->rx_clock_rec_if_id = 0xffff;
      }
   }

   if((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) ||
      (line_registry->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY))
      line_registry->tx_clock_rec_if_id = 0xffff;

   memset (line_registry, WUFEI_NOT_VALID_SLOT,/*0xFF*/ sizeof(WUFEI_line_registry));
   line_registry->tdm_ram_mng = tmp_tdm_ram_mng_ptr;

   line_registry->state_mode    = WUFEI_LINE_EMPTY;

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_LineCadReconfigure
 *
 * Purpose: reconfigures a UFE CAD Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE
 *     line_registry - line registry
 *     config - new line configuration
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *       WUFE_ERR_ILLEGAL_NUM_LINES
 *       WUFE_ERR_INVALID_CONFIG
 * Called by:
 *     WUFE_LineReconfigure() function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineCadReconfigure(WUFEI_system_registry *system_registry,
                                            WUFEI_line_registry *line_registry,
                                            WUFE_line_cad_reconfig *config)
{
   WP_U16 hw_cad_mode, new_hw_cad_mode;
   WP_U32 wufe_id;

   wufe_id = system_registry->wufe_id;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (config->cas_enable != WUFE_NO_CHANGE &&
          config->cas_enable != line_registry->cas_enable)
      {
         if (config->cas_enable > 1)
            return WUFE_ERR_CAS_INVALID_CFG;

         line_registry->cas_enable = config->cas_enable;
         /* In case of ces and cas - configure the machine cas table */
         if (WUFEI_SystemHwConfigTransMachineValid(system_registry,line_registry->ufe_core_id) &&
             line_registry->cas_enable == WUFE_CAS_ENABLE)
         {
            WUFE_status status;

            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwCesCasConfig(system_registry,
                                                       line_registry->ufe_core_id,
                                                       line_registry->line_id));
         }
      }
      return WUFE_OK;
   }

   new_hw_cad_mode = hw_cad_mode = WUFEI_HwCadModeGet(wufe_id, line_registry->line_id);

   if (config->multi_frame != WUFE_NO_CHANGE)
   {
      WP_U16 multi_frame = WUFEI_CAD_MODE_MULTI_FRAME_SYNC_V(hw_cad_mode);

      if (config->multi_frame != multi_frame)
      {
         if (config->multi_frame > 1)
            return WUFE_ERR_CAD_INVALID_CFG;
         WUFEI_REGISTER_CONFIG(new_hw_cad_mode,
                               WUFEI_CAD_MODE_MULTI_FRAME_SYNC_M,
                               WUFEI_CAD_MODE_MULTI_FRAME_SYNC_F(config->multi_frame));
      }
   }

   if (config->multi_frame_sync_level != WUFE_NO_CHANGE)
   {
      WP_U16 multi_frame_sync_level = WUFEI_CAD_MODE_M_FRAME_SYNC_V(hw_cad_mode);

      if (config->multi_frame_sync_level != multi_frame_sync_level)
      {
         if (config->multi_frame_sync_level > 1)
            return WUFE_ERR_CAD_INVALID_CFG;
         WUFEI_REGISTER_CONFIG(new_hw_cad_mode,
                               WUFEI_CAD_MODE_M_FRAME_SYNC_M,
                               WUFEI_CAD_MODE_M_FRAME_SYNC_F(config->multi_frame_sync_level));
      }
   }

   if (config->multi_frame_num != WUFE_NO_CHANGE)
   {
      WP_U16 multi_frame_num = WUFEI_CAD_MODE_MULTIFRAME_NUM_V(hw_cad_mode);

      if (config->multi_frame_num != multi_frame_num)
      {
         if (config->multi_frame_num > 1)
            return WUFE_ERR_CAD_INVALID_CFG;
         WUFEI_REGISTER_CONFIG(new_hw_cad_mode,
                               WUFEI_CAD_MODE_MULTIFRAME_NUM_M,
                               WUFEI_CAD_MODE_MULTIFRAME_NUM_F(config->multi_frame_num));
      }
   }

   if (config->cas_enable != WUFE_NO_CHANGE &&
       config->cas_enable != line_registry->cas_enable)
   {
      if (config->cas_enable > 1)
         return WUFE_ERR_CAD_INVALID_CFG;
      WUFEI_REGISTER_CONFIG(new_hw_cad_mode,
                            WUFEI_CAD_MODE_CAS_ENABLE_M,
                            WUFEI_CAD_MODE_CAS_ENABLE_F(config->cas_enable));
      line_registry->cas_enable = config->cas_enable;
   }

   if (config->cas_alignment_location != WUFE_NO_CHANGE)
   {
      WP_U16 cas_alignment_location = WUFEI_CAD_MODE_CAS_ALIGN_V(hw_cad_mode);

      if (config->cas_alignment_location != cas_alignment_location)
      {
         if (config->cas_alignment_location > 1)
            return WUFE_ERR_CAD_INVALID_CFG;
         WUFEI_REGISTER_CONFIG(new_hw_cad_mode,
                               WUFEI_CAD_MODE_CAS_ALIGN_M,
                               WUFEI_CAD_MODE_CAS_ALIGN_F(config->cas_alignment_location));
      }
   }

   if (hw_cad_mode != new_hw_cad_mode)
   {
      /* disable the control CAD register */
      new_hw_cad_mode &= ~(WUFEI_CAD_RX_MODE_ENABLE_M | WUFEI_CAD_TX_MODE_ENABLE_M);

      WUFEI_HwCadModeSet(wufe_id, line_registry->line_id, new_hw_cad_mode);
      /* update the register while enabling it */
      new_hw_cad_mode |= (WUFEI_CAD_RX_MODE_ENABLE_M | WUFEI_CAD_TX_MODE_ENABLE_M);

      WUFEI_HwCadModeSet(wufe_id, line_registry->line_id, new_hw_cad_mode);
   }
   if (config->clock_rec_mode != WUFE_NO_CHANGE &&
       config->clock_rec_mode != line_registry->clock_rec_mode)
   {
      WUFE_status status;

      if (config->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY ||
          line_registry->clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY)
         return WUFE_ERR_NOT_IMPLEMENTED;

      /* clock recovery reconfigure can be done only when there are
       * no PHYs for the specific line */
      if (line_registry->n_attach_phy != 0)
         return WUFE_ERR_LINE_CONTAIN_PHYS;

      if (line_registry->state_mode == WUFEI_LINE_ENABLE)
         return WUFE_ERR_LINE_NOT_DISABLED;

      /* if the Tx clock recovery wasn't set, init it */
      if (((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) == 0) &&
          (config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX))
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecTxInterfaceIdValidate(system_registry,
                                                                   line_registry->transfer_type,
                                                                   line_registry->line_id));

         line_registry->tx_clock_rec_if_id = line_registry->line_id;
      }

      /* if the Tx clock recovery was set and now it shouldn't be set, clear it */
      if ( (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) &&
           ((config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX) == 0) )
      {
         line_registry->tx_clock_rec_if_id = 0xffff;
      }

      if ((config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX) ||
          (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX))
      {
         WP_U8 method;

         /* the Clock recovery Rx mode can be enabled in the differential mode only */
         WUFEI_RETURN_IF_ERROR(status, WUFEI_ClockRecMethodGet(system_registry, &method));

         /* if the Rx clock recovery wasn't set, init it */
         if (((line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX) == 0) &&
             (config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX))
         {
            if (method == WUFE_CLOCK_REC_METHOD_DIFFERENTIAL)
            {
               /* CAD differential clock recovery method uses Rx timestamp */
               /* In this case the rx clock recovery interface id must be obtained */
               WUFEI_RETURN_IF_ERROR( status,
                           WUFEI_ClockRecRxInterfaceCreate(system_registry,
                                                           line_registry));
            }
            else
            {
               WUFE_PRINT_ERR(
                  (wufei_log,
                   "WUFE_LineCadReconfigure: no rx clock recovery in adaptive mode\n"));
               return WUFE_ERR_CAD_INVALID_CFG;
            }
         }

         /* if the Rx clock recovery was set and now it shouldn't be set, clear it */
         if ( (line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX) &&
              ((config->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX) == 0) )
         {
            if (method == WUFE_CLOCK_REC_METHOD_DIFFERENTIAL)
            {
               /* release rx clock recovery interface id allocated during line create */
               WUFEI_RETURN_IF_ERROR(
                  status,
                  WUFEI_ClockRecRxInterfaceDelete(system_registry,
                                                  line_registry->rx_clock_rec_if_id,
                                                  line_registry->line_id));

               line_registry->rx_clock_rec_if_id = 0xffff;
            }
         }
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSonetReconfigure
 *
 * Purpose: reconfigures a UFE SONET Line
 *
 * Description:
 *
 * Inputs:
 *     line_registry - line registry
 *     config - new line configuration
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *       WUFE_ERR_ILLEGAL_NUM_LINES
 *       WUFE_ERR_INVALID_CONFIG
 * Called by:
 *     WUFE_LineReconfigure() function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSonetReconfigure(WUFEI_system_registry *system_registry,
                                              WUFEI_line_registry *line_registry,
                                              WUFE_line_sonet_reconfig *config)
{
   WUFE_status status;

   if (config->cas_enable != WUFE_NO_CHANGE &&
       config->cas_enable != line_registry->cas_enable)
   {
      WP_U16 tdm_hw_entry;

      if (config->cas_enable > 1)
         return WUFE_ERR_CAS_INVALID_CFG;

      line_registry->cas_enable = config->cas_enable;
      tdm_hw_entry =
         ((line_registry->aug4 % WUFEI_NUM_AUG4_PER_CORE) * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->aug1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->au3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->tug2 * WUFE_MAX_N_TU) +
         line_registry->tu;

      /* Configure the tdm line mode table */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmLineConfig(system_registry,
                                                  line_registry,
                                                  tdm_hw_entry));
      
      /* In case of ces and cas - configure the machine cas table */
      if (WUFEI_SystemHwConfigTransMachineValid(system_registry,line_registry->ufe_core_id) &&
          line_registry->cas_enable == WUFE_CAS_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwCesCasConfig(system_registry,
                                                    line_registry->ufe_core_id,
                                                    tdm_hw_entry));
      }
   }

   if (config->clock_rec_line_params)
   {
      if ((config->clock_rec_line_params)->tx_clk_rec_enable != (line_registry->clock_rec_line_params)->tx_clk_rec_enable)
      {
         if ((config->clock_rec_line_params)->tx_clk_rec_enable)
         {
            /* Enable TX line CR in the line registry */
            line_registry->clock_rec_mode = WUFE_LINE_CLOCK_REC_ENABLE_TX;
            /* Enable TX line CR in the HW */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_ClockRecLineEnable(system_registry->wufe_id, system_registry,
                                                           line_registry->line_id, line_registry));
         }
         else
         {
            /* Disable TX line CR in the HW */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_ClockRecLineDisable(system_registry->wufe_id, system_registry,
                                                            line_registry->line_id, line_registry));
            /* Disable TX line CR in the line registry */
            line_registry->clock_rec_mode = WUFE_LINE_CLOCK_REC_DISABLE;
         }
      }

      if ((config->clock_rec_line_params)->clk_rec_physical_clock_enable != (line_registry->clock_rec_line_params)->clk_rec_physical_clock_enable)
      {
      }

      if ((config->clock_rec_line_params)->clk_rec_physical_clock_id != (line_registry->clock_rec_line_params)->clk_rec_physical_clock_id)
      {
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSdhReconfigure
 *
 * Purpose: reconfigures a UFE SDH Line
 *
 * Description:
 *
 * Inputs:
 *     line_registry - line registry
 *     config - new line configuration
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *       WUFE_ERR_ILLEGAL_NUM_LINES
 *       WUFE_ERR_INVALID_CONFIG
 * Called by:
 *     WUFE_LineReconfigure() function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_LineSdhReconfigure(WUFEI_system_registry *system_registry,
                                            WUFEI_line_registry *line_registry,
                                            WUFE_line_sdh_reconfig *config)
{
   WUFE_status status;

   if (config->cas_enable != WUFE_NO_CHANGE &&
       config->cas_enable != line_registry->cas_enable)
   {
      WP_U16 tdm_hw_entry;

      if (config->cas_enable > 1)
         return WUFE_ERR_CAS_INVALID_CFG;

      line_registry->cas_enable = config->cas_enable;
      tdm_hw_entry =
         ((line_registry->aug4 % WUFEI_NUM_AUG4_PER_CORE) * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->aug1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->au3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
         (line_registry->tug2 * WUFE_MAX_N_TU) +
         line_registry->tu;


      /* Configure the tdm line mode table */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmLineConfig(system_registry,
                                                  line_registry,
                                                  tdm_hw_entry));
      

      /* In case of ces and cas - configure the machine cas table */
      if (WUFEI_SystemHwConfigTransMachineValid(system_registry,line_registry->ufe_core_id) &&
          line_registry->cas_enable == WUFE_CAS_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwCesCasConfig(system_registry,
                                                    line_registry->ufe_core_id,
                                                    tdm_hw_entry));
      }
   }

   if (config->clock_rec_line_params)
   {

      if ((config->clock_rec_line_params)->tx_clk_rec_enable != (line_registry->clock_rec_line_params)->tx_clk_rec_enable)
      {

         if ((config->clock_rec_line_params)->tx_clk_rec_enable)
         {
            /* Enable TX line CR in the line registry */
            line_registry->clock_rec_mode = WUFE_LINE_CLOCK_REC_ENABLE_TX;
            /* Enable TX line CR in the HW */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_ClockRecLineEnable(system_registry->wufe_id, system_registry,
                                                           line_registry->line_id, line_registry));
         }
         else
         {
            /* Disable TX line CR in the HW */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_ClockRecLineDisable(system_registry->wufe_id, system_registry,
                                                            line_registry->line_id, line_registry));
            /* Disable TX line CR in the line registry */
            line_registry->clock_rec_mode = WUFE_LINE_CLOCK_REC_DISABLE;

         }
      }

      if ((config->clock_rec_line_params)->clk_rec_physical_clock_enable != (line_registry->clock_rec_line_params)->clk_rec_physical_clock_enable)
      {
      }


      if ((config->clock_rec_line_params)->clk_rec_physical_clock_id != (line_registry->clock_rec_line_params)->clk_rec_physical_clock_id)
      {
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFE_LineReconfigure
 *
 * Purpose: Change line parameters
 *
 * Description:
 *     This functions changes allowed line parameters.
 *
 * Inputs:
 *     line_handle     UFE line handle
 *     type            UFE line type (CAD, SBI)
 *     config          UFE line reconfig structure
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     WUFE_OK upon success, an error code otherwise.
 *
 ****************************************************************************/
WUFE_status WUFE_LineReconfigure(WUFE_handle line_handle,
                                 WUFE_struct_id type,
                                 void *config)
{
   WUFEI_system_registry *system_registry;
   WUFEI_line_registry *line_registry;
   WUFE_status status;
   WP_U32 wufe_id, line_id;
   ((void)type);
   wufe_id = WUFEI_HANDLE_UFE_ID_V(line_handle);
   line_id = WUFEI_HANDLE_INDEX_V(line_handle);

   /* lock the wufe_id */
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   line_registry =&system_registry->line_arr[line_id];

   if (line_registry->line_mode == WUFE_STRUCT_LINE_CAD)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_LineCadReconfigure(system_registry,
                                                              line_registry,
                                                              (WUFE_line_cad_reconfig *)config),
                                     wufe_id);
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SBI)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_HwSbiLineReconfigure(wufe_id,
                                                                system_registry,
                                                                line_registry,
                                                                (WUFE_line_sbi_reconfig *)config),
                                     wufe_id);
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SONET)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_LineSonetReconfigure(system_registry,
                                                                line_registry,
                                                                (WUFE_line_sonet_reconfig *)config),
                                     wufe_id);
   }
   else if (line_registry->line_mode == WUFE_STRUCT_LINE_SDH)
   {
     WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                    WUFEI_LineSdhReconfigure(system_registry,
                                                             line_registry,
                                                             (WUFE_line_sdh_reconfig *)config),
                                     wufe_id);
   }
   else
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFE_ERR_LINE_INVALID_TYPE, wufe_id);
   }

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineRegistryGet
 *
 * Purpose:  return the line of a specified line id
 *
 *
 * Description:
 *     This function retreives the line registry of a given line
 *
 * Inputs: system_registry - system registry
 *         line_id - line id
 *         line_registry - line registry
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
WUFE_status WUFEI_LineRegistryGet(WUFEI_system_registry *system_registry,
                                  WP_U16 line_id,
                                  WUFEI_line_registry **line_registry)
{
   if (line_id >= system_registry->cfg.max_line_number)
      return WUFE_ERR_LINE_ILLEGAL_ID;

   *line_registry = &system_registry->line_arr[line_id];

   if((*line_registry)->state_mode == WUFEI_LINE_EMPTY)
   {
      return WUFE_ERR_LINE_EMPTY;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_LineLoopbackConfigure
 *
 * Purpose:
 *
 *
 * Description: This function sets a UFE Socket Interfaces loopback from
 *             a given type for a specified UFE Line
 *
 *
 * Inputs:
 *     line_handel The UFE Line.
 *     type        The Socket loopback type.
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *     WUFE_ERR_LINE_ILLEGAL_ID
 *     WUFE_ERR_INVALID_TRANSFER_TYPE
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_LineLoopbackConfigure(WUFE_handle ufe_line_handle,
                                       WUFE_line_loopback_type type)
{
   WUFE_status status;
   WP_U32 wufe_id, line_id, core_id;
   WUFEI_line_registry   *line_registry;
   WUFEI_system_registry *system_registry;
   WP_U16 lpbk_mode,val;
   WUFEI_hw_tdm_lpbk_system* hw_tdm_lpbk_system_ptr;
   WUFEI_hw_tdm_lpbk_line* hw_tdm_lpbk_line_ptr;
   WP_U32 spe, stm1;
   WP_U16 tdm_entry, tu, tug2, loopback_type;

   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_line_handle);
   line_id = WUFEI_HANDLE_INDEX_V (ufe_line_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,
                                  &system_registry,status);

   if (line_id >= system_registry->cfg.max_line_number)
   {/* illegal line number */
      WUFE_PRINT_ERR((wufei_log,
          "ERROR: cfg->cad_id(%d) >= sys_reg->cfg.max_line_number( %d ) \n",
          line_id, system_registry->cfg.max_line_number));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_ILLEGAL_ID ;
   }

   line_registry = &system_registry->line_arr[line_id];

   if (line_registry->state_mode == WUFEI_LINE_EMPTY)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFE:ERROR the line is empty\n"));

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_LINE_ILLEGAL_ID;
   }
   core_id = line_registry->ufe_core_id;

   if( WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      /*The socket loopback supports only the E1/T1 lines*/
      if(line_registry->transfer_type == WUFE_CAD_TYPE_T1 ||
         line_registry->transfer_type == WUFE_SBI_TYPE_T1 ||
         line_registry->transfer_type == WUFE_GENERIC_TYPE_T1)
      {
         /*The clk value should be read from clk_cfg_reg
           CHECK div_val = clk(100MHz)/ T1clk(1.544MHz)*/
         val = WUFEI_TDM_LPBK_SYS_DIV_VAL_F(WUFEI_SYS_TRIB_CLK_RATIO_T1);
         val &= ~WUFEI_TDM_LPBK_SYS_TRANS_MODE_M;

      }
      else if (line_registry->transfer_type == WUFE_CAD_TYPE_E1 ||
               line_registry->transfer_type == WUFE_SBI_TYPE_E1 ||
               line_registry->transfer_type == WUFE_GENERIC_TYPE_E1)
      {
         /*CHECK div_val = clk(100MHz)/ T1clk(2.048MHz)*/
         val =( WUFEI_TDM_LPBK_SYS_DIV_VAL_F(WUFEI_SYS_TRIB_CLK_RATIO_E1)|
                WUFEI_TDM_LPBK_SYS_TRANS_MODE_M);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE:ERROR transfer type\n"));

         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_INVALID_TRANSFER_TYPE;
      }

      tu = WUFEI_TU_V(line_registry->sub_spe_entry);
      tug2 = WUFEI_TUG2_V(line_registry->sub_spe_entry);

      /*calculates the STM1/SPE TDM addr*/
      WUFEI_LineTdmStm1SpeIndexMap(line_registry, &stm1, &spe);

      tdm_entry =
         WUFEI_TDM_ENTRY_TU_ID_F(tu)|
         WUFEI_TDM_ENTRY_TUG2_ID_F(tug2)|
         WUFEI_TDM_ENTRY_SPE_ID_F(spe)|
         WUFEI_TDM_ENTRY_STM1_ID_F(stm1);

      /*Sets the required loopback*/
      if(type & WUFE_LINE_LOOPBACK_TYPE_LINE)
      {
         if (!system_registry->hw_config.tdm_line_loop)
         {
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            WUFE_PRINT_ERR((wufei_log,
                            "WUFE:ERROR TDM line loopback is not defined in the synthesis\n"));
            return WUFE_ERR_LINE_LOOPBACK_TYPE_LINE_NOT_ENABLED_IN_HW;
         }

         hw_tdm_lpbk_line_ptr = WUFEI_TDM_LPBK_LINE;
         lpbk_mode = ((WUFEI_TDM_LPBK_LINE_ID_M & tdm_entry)|
                      WUFEI_TDM_LPBK_LINE_EN_M |
                      WUFEI_TDM_LPBK_LINE_UNFRM_F(!line_registry->framed));

         WUFEI_MEM_16_SET(WUFEI_ID(wufe_id, core_id),
                          &(hw_tdm_lpbk_line_ptr->config),
                          lpbk_mode);
      }
      else if (type & WUFE_LINE_LOOPBACK_TYPE_SYSTEM)
      {
         if (!system_registry->hw_config.tdm_sys_loop)
         {
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            WUFE_PRINT_ERR((wufei_log,
                            "WUFE:ERROR TDM system loopback is not defined in the synthesis\n"));
            return WUFE_ERR_LINE_LOOPBACK_TYPE_SYSTEM_NOT_ENABLED_IN_HW;
         }

         hw_tdm_lpbk_system_ptr = WUFEI_TDM_LPBK_SYS;

         debug_flag=0;
         WUFEI_MEM_16_SET(WUFEI_ID(wufe_id, core_id),
                          &(hw_tdm_lpbk_system_ptr->port_id),
                          WUFEI_TDM_LPBK_SYS_ID_M & tdm_entry);


         lpbk_mode = ( val | WUFEI_TDM_LPBK_SYS_EN_M |
                       WUFEI_TDM_LPBK_SYS_UNFRM_F(!line_registry->framed));

         WUFEI_MEM_16_SET(WUFEI_ID(wufe_id, core_id),
                          &(hw_tdm_lpbk_system_ptr->config),
                          lpbk_mode);
         debug_flag =0;
      }
      else if (type & WUFE_LINE_LOOPBACK_TYPE_NONE)
      {
         hw_tdm_lpbk_line_ptr = WUFEI_TDM_LPBK_LINE;
         hw_tdm_lpbk_system_ptr = WUFEI_TDM_LPBK_SYS;

         WUFEI_MEM_16_SET(WUFEI_ID(wufe_id, core_id),
                          &(hw_tdm_lpbk_line_ptr->config),
                          0);

         WUFEI_MEM_16_SET(WUFEI_ID(wufe_id, core_id),
                          &(hw_tdm_lpbk_system_ptr->config),
                          0);
      }
   }
   else if( WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if(type & WUFE_LINE_LOOPBACK_TYPE_LINE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "WUFE:ERROR TDM line loopback is not supported\n"));
         return WUFE_ERR_LINE_LOOPBACK_TYPE_LINE_NOT_ENABLED_IN_HW;
      }
      else if (type & WUFE_LINE_LOOPBACK_TYPE_SYSTEM)
      {
         if(line_registry->transfer_type == WUFE_CAD_TYPE_T1 ||
            line_registry->transfer_type == WUFE_SDH_TYPE_T1 ||
            line_registry->transfer_type == WUFE_SONET_TYPE_T1 ||
            line_registry->transfer_type == WUFE_DCC_TYPE_MS ||
            line_registry->transfer_type == WUFE_DCC_TYPE_RS)
         {
            loopback_type = WUFEI_TDM_LOOPBACK_T1;
         }
         else if(line_registry->transfer_type == WUFE_CAD_TYPE_E1 ||
                 line_registry->transfer_type == WUFE_SDH_TYPE_E1 ||
                 line_registry->transfer_type == WUFE_SONET_TYPE_E1)
         {
            loopback_type = WUFEI_TDM_LOOPBACK_E1;
         }
         else if (line_registry->transfer_type == WUFE_SDH_TYPE_DS3 ||
                  line_registry->transfer_type == WUFE_SONET_TYPE_DS3 ||
                  line_registry->transfer_type == WUFE_SDH_TYPE_E3 ||
                  line_registry->transfer_type == WUFE_SONET_TYPE_E3)
         {
            loopback_type = WUFEI_TDM_LOOPBACK_VC3;
         }
         else if (line_registry->transfer_type == WUFE_SDH_TYPE_VC4 ||
                  line_registry->transfer_type == WUFE_SONET_TYPE_STS3)
         {
            loopback_type = WUFEI_TDM_LOOPBACK_OC3;
         }
         else if (line_registry->transfer_type == WUFE_SDH_TYPE_VC44C ||
                  line_registry->transfer_type == WUFE_SONET_TYPE_STS12)
         {
            loopback_type = WUFEI_TDM_LOOPBACK_OC12;
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "WUFE:ERROR transfer type\n"));

            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            return WUFE_ERR_INVALID_TRANSFER_TYPE;
         }

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwTdmLoopbackSet(system_registry,
                                                               1,
                                                               line_registry->framed,
                                                               line_registry->cas_enable,
                                                               loopback_type,
                                                               core_id),
                                        wufe_id);
      }
      else if (type & WUFE_LINE_LOOPBACK_TYPE_NONE)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwTdmLoopbackSet(system_registry,
                                                               0,
                                                               0,
                                                               0,
                                                               0,
                                                               core_id),
                                        wufe_id);
      }
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }


   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   line_registry->loopback_type = type;
   return WUFE_OK;

}


/*****************************************************************************
 *
 * Function:  WUFE_LinePdhLoopbackConfigure
 *
 * Purpose:
 *
 *
 * Description: This function sets a UFE socket PDH Interfaces loopback from
 *             a given type for a specified UFE Line - there can be up to WUFEI_MAX_PDH_LOOPBACK at the same time
 *
 *
 * Inputs:
 *     line_handel The UFE Line.
 *     type        The PDH loopback type(system or line)
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *     WUFE_ERR_LINE_ILLEGAL_ID
 *     WUFE_ERR_INVALID_TRANSFER_TYPE
 *	   WUFE_ERR_WUFE_LINE_PDH_LINE_AND_SYSTEM_LOOPBACKS_MUST_NOT_BE_ENABLED_SIMOULTANOUSLY
 *	   WUFE_ERR_WUFE_LINE_PDH_LOOPBACK_MAX_NUM_OF_LOOPBACKS
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_LinePdhLoopbackConfigure(WUFE_handle ufe_line_handle,
                                       	  WUFE_line_pdh_loopback_type type)
{
	WUFE_status status;
	WP_U32 wufe_id, line_id, core_id;
	WUFEI_line_registry   *line_registry;
	WUFEI_system_registry *system_registry;
	WP_U16 tdm_hw_entry,loopback_index=0;

	wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_line_handle);
	line_id = WUFEI_HANDLE_INDEX_V (ufe_line_handle);

	/* lock the wufe_id UFE*/
	WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,
			&system_registry,status);

	if (line_id >= system_registry->cfg.max_line_number)
	{/* illegal line number */
		WUFE_PRINT_ERR((wufei_log,
				"ERROR: line_id(%d) >= sys_reg->cfg.max_line_number( %d ) \n",
				line_id, system_registry->cfg.max_line_number));
		WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
		return WUFE_ERR_LINE_ILLEGAL_ID ;
	}

	line_registry = &system_registry->line_arr[line_id];

	if (line_registry->state_mode == WUFEI_LINE_EMPTY)
	{
		WUFE_PRINT_ERR((wufei_log,
				"WUFE:ERROR the line is empty\n"));

		WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
		return WUFE_ERR_LINE_ILLEGAL_ID;
	}
	if( line_registry->transfer_type!=  WUFE_SDH_TYPE_T1 &&
            line_registry->transfer_type!=  WUFE_SDH_TYPE_E1 &&
            line_registry->transfer_type!=  WUFE_SONET_TYPE_T1 &&
            line_registry->transfer_type!=  WUFE_SONET_TYPE_E1 &&
            line_registry->transfer_type!=  WUFE_SDH_TYPE_DS3 &&
            line_registry->transfer_type!=  WUFE_SONET_TYPE_DS3
           )
	{
		WUFE_PRINT_ERR((wufei_log, "ERROR pdh loopback support only the following transfer types: WUFE_SDH_TYPE_T1,WUFE_SDH_TYPE_E1,WUFE_SONET_TYPE_T1,WUFE_SONET_TYPE_E1 :\n"));
		WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
		return WUFE_ERR_LINE_INVALID_TYPE;
	}
	core_id = line_registry->ufe_core_id;
/*Pdh loopback supports UFE4 only */
	if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
	{
		WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
		WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
		return WUFE_ERR_INVALID_CONFIG;
	}
	else
	{
		tdm_hw_entry =
				((line_registry->aug4 % WUFEI_NUM_AUG4_PER_CORE) * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
				(line_registry->aug1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
				(line_registry->au3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) +
				(line_registry->tug2 * WUFE_MAX_N_TU) +
				line_registry->tu;

		if(type == WUFE_LINE_LOOPBACK_PDH_TYPE_LINE)
		{
			if(line_registry->pdh_loopback_type==WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM)
			{
				WUFE_PRINT_ERR((wufei_log, "for a single PDH only one loopback can be enabled Line or System loopback\n"));
				WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
				return WUFE_ERR_LINE_PDH_LINE_AND_SYSTEM_LOOPBACKS_MUST_NOT_BE_ENABLED_SIMULTANEOUSLY;
			}
			else if(line_registry->pdh_loopback_type==WUFE_LINE_LOOPBACK_PDH_TYPE_LINE)
			{
				WUFE_PRINT_ERR((wufei_log, "PDH loopback type already enabled for the requested line\n"));
				WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
				return WUFE_ERR_LINE_PDH_LOOPBACK_ALREADY_ENABLED;
			}
			status = WUFEI_HwTdmPdhLoopbackGetIndex(type,
										  	  	   system_registry,
										  	  	   &loopback_index);
			if(WUFEI_MAX_PDH_LOOPBACK==loopback_index)
			{
				WUFE_PRINT_ERR((wufei_log, "there can be up to %d loopbacks at the same time\n",WUFEI_MAX_PDH_LOOPBACK));
				WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
				return WUFE_ERR_LINE_PDH_LOOPBACK_MAX_NUM_OF_LOOPBACKS;
			}
			/* Configure the tdm line mode table in order to enable the loopback*/
			WUFEI_RETURN_IF_ERROR(status,
					WUFEI_HwTdmPdhLoopbackConfig(wufe_id,
					                             core_id,
					                             loopback_index,
					                             tdm_hw_entry,
					                             type));
			WUFEI_RETURN_IF_ERROR(status,
					WUFEI_HwTdmPdhLoopbackSet(system_registry,
							               loopback_index,
							               type,
							               core_id,
							               WUFE_ENABLE));
			system_registry->pdh_line_loopback[loopback_index] = line_id;
		}
		else if (type == WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM)
		{
			if(line_registry->pdh_loopback_type==WUFE_LINE_LOOPBACK_PDH_TYPE_LINE)
			{
				WUFE_PRINT_ERR((wufei_log, "for a single PDH only one loopback can be enabled Line or System loopback\n"));
				WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
				return WUFE_ERR_LINE_PDH_LINE_AND_SYSTEM_LOOPBACKS_MUST_NOT_BE_ENABLED_SIMULTANEOUSLY;
			}
			else if(line_registry->pdh_loopback_type==WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM)
			{
				WUFE_PRINT_ERR((wufei_log, "PDH loopback type already enabled for the requested line\n"));
				WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
				return WUFE_ERR_LINE_PDH_LOOPBACK_ALREADY_ENABLED;
			}
			status = WUFEI_HwTdmPdhLoopbackGetIndex(type,
										  	  	   system_registry,
										  	  	   &loopback_index);
			if(WUFEI_MAX_PDH_LOOPBACK==loopback_index)
			{
				WUFE_PRINT_ERR((wufei_log, "there can be up to %d loopbacks at the same time\n",WUFEI_MAX_PDH_LOOPBACK));
				WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
				return WUFE_ERR_LINE_PDH_LOOPBACK_MAX_NUM_OF_LOOPBACKS;
			}

			/* Configure the tdm line mode table in order to enable the loopback*/
			WUFEI_RETURN_IF_ERROR(status,
					WUFEI_HwTdmPdhLoopbackConfig(wufe_id,
					                             core_id,
					                             loopback_index,
					                             tdm_hw_entry,
					                             type));
			WUFEI_RETURN_IF_ERROR(status,
					WUFEI_HwTdmPdhLoopbackSet(system_registry,
							               loopback_index,
							               type,
							               core_id,
							               WUFE_ENABLE));
			system_registry->pdh_system_loopback[loopback_index] = line_id;

		}
		else if (type == WUFE_LINE_LOOPBACK_PDH_TYPE_NONE)
		{
			/*clear the registry loopback array of the relevant loopback index*/
			if(line_registry->pdh_loopback_type == WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM)
			{
				system_registry->pdh_system_loopback[line_registry->pdh_loopback_index] = WUFEI_MAX_OC48_LINES;
				loopback_index = line_registry->pdh_loopback_index;
			}
			else if(line_registry->pdh_loopback_type == WUFE_LINE_LOOPBACK_PDH_TYPE_LINE)
			{
				system_registry->pdh_line_loopback[line_registry->pdh_loopback_index] = WUFEI_MAX_OC48_LINES;
				loopback_index = line_registry->pdh_loopback_index;
			}
			/* Configure the tdm line mode table in order to enable the loopback*/
			WUFEI_RETURN_IF_ERROR(status,
					WUFEI_HwTdmPdhLoopbackConfig(wufe_id,
					                             core_id,
					                             loopback_index,
					                             tdm_hw_entry,
					                             type));
			WUFEI_RETURN_IF_ERROR(status,
					WUFEI_HwTdmPdhLoopbackSet(system_registry,
							               loopback_index,
							               line_registry->pdh_loopback_type,
							               core_id,
							               WUFE_DISABLE));
		}
		else
		{
			WUFE_PRINT_ERR((wufei_log, "Invalid PDH Loopback type\n"));
			WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
			return WUFE_ERR_WUFE_LINE_PDH_LOOPBACK_SUPPORT_ONLY_UFE4;
		}

	}


	WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
	line_registry->pdh_loopback_type = type;
	line_registry->pdh_loopback_index = loopback_index;

	return WUFE_OK;

}


static WUFE_status WUFEI_LineLoopbackDisplay(WUFEI_line_registry *line_registry, WP_U32 wufe_id)
{
   WP_U32 id, core_id;
   WP_U16 val;
   WUFEI_hw_tdm_lpbk_system* hw_tdm_lpbk_system_ptr;
   WUFEI_hw_tdm_lpbk_line* hw_tdm_lpbk_line_ptr;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {

      if(line_registry->loopback_type == (WP_U32) LOOPBACK_DISABLED)
         WUFE_PRINT_DISP((wufei_log," loopback_type: loopback off\n"));
      else
         WUFE_PRINT_DISP((wufei_log, " loopback_type: %s\n",
            line_reg_loopback_type[line_registry->loopback_type -1]));

      if(line_registry->pdh_loopback_type == (WP_U32) WUFE_LINE_LOOPBACK_PDH_TYPE_NONE)
          WUFE_PRINT_DISP((wufei_log," pdh loopback_type: pdh loopback off\n"));
      else
          WUFE_PRINT_DISP((wufei_log, " pdh loopback_type: %s, pdh loopback_index: %d\n",
                           line_reg_pdh_loopback_type[line_registry->pdh_loopback_type],line_registry->pdh_loopback_index));
      return WUFE_OK;
   }
   else
   { /*UFE3*/
      if (line_registry->state_mode == WUFEI_LINE_EMPTY)
      {
         WUFE_PRINT_ERR((wufei_log,
                            "WUFE:ERROR the line is empty\n"));
            return WUFE_ERR_LINE_ILLEGAL_ID;
      }

      core_id = line_registry->ufe_core_id;
      id = WUFEI_ID(wufe_id, core_id);

      debug_flag = 0;

      hw_tdm_lpbk_line_ptr = WUFEI_TDM_LPBK_LINE;
      WUFEI_MEM_16_GET(id, &(hw_tdm_lpbk_line_ptr->config), &val);
      WUFE_PRINT_DISP((wufei_log, "line loopback config 0x%04x\n",val));

      hw_tdm_lpbk_system_ptr = WUFEI_TDM_LPBK_SYS;
      WUFEI_MEM_16_GET(id, &(hw_tdm_lpbk_system_ptr->port_id), &val);
      WUFE_PRINT_DISP((wufei_log, "system loopback port_id 0x%04x\n",val));

      WUFEI_MEM_16_GET(id, &(hw_tdm_lpbk_system_ptr->config), &val);
      WUFE_PRINT_DISP((wufei_log, "system loopback config 0x%04x\n",val));

      debug_flag = 0;
   }


      return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSetPhyClockRecTxCreate
 *
 * Purpose:
 *
 * Description: This function sets all PHYs that belong to a given Line
 *              to be Clock Recovery Slave PHYs, during TX CR reconfigure
 *
 * Inputs:
 *     line_id  - The UFE Line id.
 *     system_registry  - The system registry.
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwSbiLineReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_LineSetPhyClockRecTxCreate(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry)
{
   WP_U32 i;
   WUFE_status status;

   for(i=0; i<system_registry->cfg.max_phy_number; i++)
   {
      if (system_registry->phy_arr[i].line_id == line_id)
      {
         WUFEI_phy_registry* phy_registry;

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_PhyRegistryGet(system_registry,
                                                    i,
                                                    &phy_registry));
         /*Set PHY to TX clock recovery slave*/
         WUFEI_PhyClockRecStatusSetTxSlave(phy_registry);
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSetPhyClockRecTxDelete
 *
 * Purpose:
 *
 * Description: This function deletes TX CR interface and sets all PHY to CR
 *              mode none
 *
 * Inputs:
 *     line_id  - The UFE Line id.
 *     system_registry  - The system registry.
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwSbiLineReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_LineSetPhyClockRecTxDelete(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry)
{
   WP_U32 i;
   WUFE_status status;

   for(i=0; i<system_registry->cfg.max_phy_number; i++)
   {
      if (system_registry->phy_arr[i].line_id == line_id)
      {

         WUFEI_phy_registry* phy_registry;

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_PhyRegistryGet(system_registry,
                                                    i,
                                                    &phy_registry));

         /*Free the clock recovery TX interface - only if PHY is TX master*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecPhyDelete(system_registry,
                                                       phy_registry->phy_id));

         /*Set PHY to TX clock recovery mode none*/
         WUFEI_PhyClockRecStatusSetTxNone(phy_registry);

       }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_PhyClockRecRxModeDisable
 *
 * Purpose:
 *
 * Description: This function deletes RX CR PHY from shaper
 *
 * Inputs:
 *     phy_registry  - The phy registry.
 *
 * Outputs:
 *
 *
 * Return Value: none
 *
 * Called by:
 *     WUFEI_LineSetPhyClockRecRxDelete
 *
 ****************************************************************************/
void WUFEI_PhyClockRecRxModeDisable(WUFEI_phy_registry* phy_registry)
{
   WUFE_phy *cfg_phy;

   cfg_phy = &(phy_registry->cfg);
   cfg_phy->type.trans.rx_clk_rec_mode = WUFE_DISABLE;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSetPhyClockRecRxDelete
 *
 * Purpose:
 *
 * Description: This function deletes RX CR interface, sets all PHY to CR
 *              mode none and remove PHY from shaper
 *
 * Inputs:
 *     line_id  - The UFE Line id.
 *     system_registry  - The system registry.
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwSbiLineReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_LineSetPhyClockRecRxDelete(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry)
{
   WP_U32 i;
   WUFE_status status;

   for(i=0; i<system_registry->cfg.max_phy_number; i++)
   {
      if (system_registry->phy_arr[i].line_id == line_id)
      {
         WUFEI_phy_registry* phy_registry;

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_PhyRegistryGet(system_registry,
                                                    i,
                                                    &phy_registry));
         /*Set PHY to RX clock recovery mode none*/
         WUFEI_PhyClockRecStatusSetRxNone(phy_registry);

         /* Clear clk_rec_mode from registry - this phy isn't shaped anymore */
         WUFEI_PhyClockRecRxModeDisable(phy_registry);
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LineSetPhyClockRecRxCreate
 *
 * Purpose:
 *
 * Description: This function deletes RX CR interface and sets all PHY to CR
 *              mode none
 *
 * Inputs:
 *     line_id  - The UFE Line id.
 *     system_registry  - The system registry.
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwSbiLineReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_LineSetPhyClockRecRxCreate(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry)
{
   WP_U32 i;
   WUFE_status status;

   for(i=0; i<system_registry->cfg.max_phy_number; i++)
   {
      if (system_registry->phy_arr[i].line_id == line_id)
      {
         WUFEI_phy_registry* phy_registry;

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_PhyRegistryGet(system_registry,
                                                    i,
                                                    &phy_registry));

         /*Set PHY to RX clock recovery slave*/
         WUFEI_PhyClockRecStatusSetRxSlave(phy_registry);
      }
   }

   return WUFE_OK;
}
