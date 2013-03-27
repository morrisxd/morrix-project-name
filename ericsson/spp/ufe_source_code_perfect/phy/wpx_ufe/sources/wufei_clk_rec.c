/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_clk_rec.c
 *
 * Purpose: the UFE Clock Recovery external and internal functions
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_HW_CLK_REC_INT_H
#include "wufei_hw_clk_rec_int.h"
#endif

#ifndef WUFEI_HW_CORE_INT_H
#include "wufei_hw_core_int.h"
#endif

#ifndef WUFEI_HW_CAD_H
#include "wufei_hw_cad.h"
#endif

#ifndef WUFEI_HW_TDM_H
#include "wufei_hw_tdm.h"
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

#ifndef WUFEI_CLK_REC_H
#include "wufei_clk_rec.h"
#endif


#ifndef WUFEI_HW_CLK_REC_H
#include "wufei_hw_clk_rec.h"
#endif

#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif

#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#ifndef WUFEI_PHY_H
#include "wufei_phy.h"
#endif

#ifndef WUFEI_MEMORY_ACCESS_INT_H
#include "wufei_memory_access_int.h"
#endif

#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif

#ifndef WUFEI_HW_GLOBALS_INT_H
#include "wufei_hw_globals_int.h"
#endif

#ifndef WUFEI_HW_EVENTS_INT_H
#include "wufei_hw_events_int.h"
#endif

#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif

extern WP_U8 debug_flag;

/*
 * The default dco clock generators list
 * Calculated for 100MHz/100PPM dco reference clock
*/
static WUFE_clk_rec_drc_divisor_list  wufe_drc_divisor_default =
{
   {
      { 0x1f/*nominal_int*/, 0x6227caf1/*nominal_rem*/ },/*T1*/
      { 0x17/*nominal_int*/, 0x6a000000/*nominal_rem*/ },/*E1*/
      { 0x03/*nominal_int*/, 0x787dfc56/*nominal_rem*/ },/*DS3/4*/
      { 0x04/*nominal_int*/, 0xd1c206ac/*nominal_rem*/ } /*E3/4*/
   }
};

/*
 * The default extended differential dco clock generators list
 * Calculated for 100MHz/100PPM dco reference clock
*/
static WUFE_clk_rec_drc_divisor_list  wufe_ext_drc_divisor_default =
{
   {
      { 0x1f/*nominal_int*/, 0x6227caf1/*nominal_rem*/ },/*T1*/
      { 0x17/*nominal_int*/, 0x6a000000/*nominal_rem*/ },/*E1*/
      { 0x03/*nominal_int*/, 0x787dfc56/*nominal_rem*/ },/*DS3/4*/
      { 0x04/*nominal_int*/, 0xd1c206ac/*nominal_rem*/ } /*E3/4*/
   }
};

/* this function is defined within WDDI */
extern WP_U32 wpi_32x32_32(WP_U32 a, WP_U32 b, WP_U32 c);

static WUFE_status WUFEI_ClockRecSystemCheckParams(WUFEI_system_registry *system_registry,
                                                   WUFE_clock_rec_system *params);

static void WUFEI_ClockRecDrcDivisorDefaultSet(WUFE_clk_rec_drc_divisor_list *ref_clk,
                                               WUFE_clk_rec_drc_divisor_list *params);

static void WUFEI_ClockRecExtDrcDivisorDefaultSet(WUFE_clk_rec_drc_divisor_list *ref_clk,
                                               WUFE_clk_rec_drc_divisor_list *params);

static void WUFEI_ClockRecRxClockGenRamCreate (WP_U16 wufe_id,
                                               WP_U16 line_id,
                                               WP_U16 clk_rec_if,
                                               WUFE_line_transf_type line_transfer_type);

static void WUFEI_ClockRecRxClockGenRamDelete(WP_U32 wufe_id,WP_U16 line_id);

static WUFE_status WUFEI_ClockRecRxClockGenRamGetFromLine (WUFEI_system_registry *system_registry,
                                                           WP_U16 line_id,
                                                           WP_U16 *ram_entry,
                                                           WP_U32 *entry_id);

static WP_U32 WUFEI_ClockRecRxClockGenRamIsValid(WP_U16 wufe_id, WP_U16 line_id);

static WUFE_status WUFEI_ClockRecRxTimestampRegInit(WP_U32 wufe_id,
                                                    WP_U16 clk_rec_if,
                                                    WUFE_line_transf_type line_transfer_type,
                                                    WUFE_un_framed framed_mode);

static WUFE_status WUFEI_ClockRecRxTimestampEnable(WP_U32 wufe_id, WP_U16 clk_rec_if);

static WUFE_status WUFEI_ClockRecRxTimestampDelete(WP_U32 wufe_id, WP_U16 clk_rec_if);

static WUFE_status WUFEI_ClockRecRxTimestampDisable(WP_U32 wufe_id, WP_U16 clk_rec_if);

static WUFE_status WUFEI_ClockRecTxMasterSet(WUFEI_system_registry *system_registry,
                                             WUFEI_line_registry *line_registry,
                                             WUFEI_phy_registry *phy_registry,
                                             WP_U8 tx_data_unit_size);

static WUFE_status WUFEI_ClockRecTimestampDisplay (WUFEI_system_registry *system_registry,
                                                   WP_U16 phy_id,
                                                   WP_U32 direction);


static WUFE_status WUFEI_ClockRecRegistryIfDisplay(WUFEI_system_registry* system_registry,
                                                   WP_U16 phy_id);

static WUFE_status WUFEI_ClockRecSystemInfo(WP_U32 wufe_id,
                                            WUFEI_system_registry *system_registry,
                                            WUFE_clk_rec_info_system *info_system);

static WUFE_status WUFEI_ClockRecPhyInfo(WUFE_handle ufe_phy_handle,
                                         WUFEI_system_registry *system_registry,
                                         WUFE_clk_rec_info_phy *info_phy);

static WUFE_status WUFEI_ClockRecLineInfo(WUFEI_system_registry *system_registry,
                                          WUFE_handle ufe_line_handle,
                                          WUFE_clk_rec_info_line *clk_rec_info);

static WUFE_status WUFEI_ClockRecReset(WP_U32 wufe_id, WUFEI_system_registry* system_registry);

static WUFE_status WUFEI_ClockRecRxPortRamCreate(WP_U32 wufe_id,
                                                 WP_U16 clk_rec_if,
                                                 WP_U16 ram_size,
                                                 WP_U16 method);


static WUFE_status WUFEI_ClockRecRxPortRamDelete(WP_U32 wufe_id, WP_U16 clk_rec_if);

static WUFE_status WUFEI_ClockRecRxPortAndMaskRamReset(WP_U32 wufe_id, WP_U16 clk_rec_if);

static WUFE_status WUFEI_ClockRecRxPortRamEnable(WP_U32 wufe_id,
                                                 WP_U32 core_id,
                                                 WP_U16 clk_rec_if,
                                                 WP_U16 ram_size,
                                                 WP_U16 phy_id,
                                                 WP_U16 slot_id);

static WUFE_status WUFEI_ClockRecRxPortRamDisable(WP_U32 wufe_id,
                                                  WP_U32 core_id,
                                                  WP_U16 clk_rec_if,
                                                  WP_U16 phy_id,
                                                  WP_U16 method);

static WUFE_status WUFEI_ClockRecRxMaskRamEnable(WP_U32 wufe_id,
                                                 WP_U16 clk_rec_if,
                                                 WP_U16 slot_id,
                                                 WP_U32 data_unit_size,
                                                 WP_U16 n_slots);

static WUFE_status WUFEI_ClockRecRxMaskRamDisable(WP_U32 wufe_id,
                                                  WP_U16 clk_rec_if,
                                                  WP_U16 phy_id);

static WUFE_status WUFEI_ClockRecRxInterfaceUpdate(WUFEI_system_registry *system_registry,
                                                   WP_U16 clock_rec_if_id,
                                                   WP_U16 new_line_id,
                                                   WP_U16 old_line_id);

static WUFE_status WUFEI_ClockRecRxCgenUpdate(WUFEI_system_registry *system_registry,
                                              WP_U16 clock_rec_if_id,
                                              WP_U16 current_line_id,
                                              WP_U16 new_line_id);

static void WUFEI_ClockRec64BitDivisionCalculation(WP_U32 a, WP_U64 b,
                                                   WP_U32* int_val, WP_U32* rem_val);

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecSystemCheckParams
 *
 * Purpose:  Check Clock Recovery system configuration parameters
 *
 * Description:
 *
 * Inputs:
 *     params - global parameters per clock recovery system
 *     system_registry
 *
 * Outputs:
 *     none
 * Return Value:
 *     WUFE_OK Upon successful completion.
 *     Otherwise the following error codes may be returned:
 *          WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS
 *
 * Called by:
 *      WUFEI_ClockRecSystemSetup()
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecSystemCheckParams(WUFEI_system_registry *system_registry,
                                                   WUFE_clock_rec_system *params)

{
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* check clock recovery global parameters */
   if(params == NULL || system_registry->hw_config.clk_rec_hw_config == NULL)
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;

#if 0
   {
      WP_U32 wufe_id = system_registry->wufe_id;
      WUFE_PRINT_DISP((wufei_log,"HW: number of RX differential units:  %d\n",
                       WUFEI_HwClockRecNumOfRxDiffUnitsGet(wufe_id)));
      WUFE_PRINT_DISP((wufei_log,"HW: number of RX adaptive units:      %d\n",
                       WUFEI_HwClockRecNumOfRxAdapUnitsGet(wufe_id)));
      WUFE_PRINT_DISP((wufei_log,"HW: number of TX differential units:  %d\n",
                       WUFEI_HwClockRecNumOfTxDiffUnitsGet(wufe_id)));
      WUFE_PRINT_DISP((wufei_log,"HW: number of TX adaptive units:      %d\n",
                       WUFEI_HwClockRecNumOfTxAdapUnitsGet(wufe_id)));
      WUFE_PRINT_DISP((wufei_log,"HW: clock rec config RX EN:           %x\n",
                       WUFEI_SystemClkRecHwConfigRxEn(system_registry)));
      WUFE_PRINT_DISP((wufei_log,"HW: clock rec config TX adap units:   %d\n",
                       WUFEI_SystemClkRecHwConfigTxAdapUnitsGet(system_registry)));

   }
#endif
   if((params->max_rx_clk_rec_if == 0) && (params->max_tx_clk_rec_if == 0) &&
      (params->max_ext_diff_rx_clk_rec_if == 0) && (params->max_ext_diff_tx_clk_rec_if == 0))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: max_rx_clk_rec_if = %d, max_tx_clk_rec_if = %d, max_ext_diff_rx_clk_rec_if = %d, max_ext_diff_tx_clk_rec_if = %d invalid\n",
                      (WP_U32)params->max_rx_clk_rec_if,
                      (WP_U32)params->max_tx_clk_rec_if,
                      (WP_U32)params->max_ext_diff_rx_clk_rec_if,
                      (WP_U32)params->max_ext_diff_tx_clk_rec_if));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }

   if((params->max_rx_clk_rec_if > 0 || params->max_ext_diff_rx_clk_rec_if > 0) &&
      !WUFEI_SystemClkRecHwConfigRxEn(system_registry))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: max_rx_clk_rec_if = %d or max_ext_diff_rx_clk_rec_if  = %d invalid, no RX CR\n",
                      (WP_U32)params->max_rx_clk_rec_if,
                      (WP_U32)params->max_ext_diff_rx_clk_rec_if));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }

   if((params->max_tx_clk_rec_if > 0 || params->max_ext_diff_tx_clk_rec_if > 0) &&
      !WUFEI_SystemClkRecHwConfigTxEn(system_registry))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: max_tx_clk_rec_if = %d or max_ext_diff_tx_clk_rec_if invalid = %d, no TX CR\n",
                      (WP_U32)params->max_tx_clk_rec_if,
                      (WP_U32)params->max_ext_diff_tx_clk_rec_if));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }

   if (params->max_rx_clk_rec_if > WUFEI_SystemClkRecHwConfigRxAdapUnitsGet(system_registry))

   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: max_rx_clk_rec_if = %d is invalid\n",
                      (WP_U32)params->max_rx_clk_rec_if));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }

   if (params->max_tx_clk_rec_if > WUFEI_SystemClkRecHwConfigTxAdapUnitsGet(system_registry))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: max_tx_clk_rec_if = %d is invalid\n",
                      (WP_U32)params->max_tx_clk_rec_if));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }
   if (params->max_ext_diff_rx_clk_rec_if > WUFEI_SystemClkRecHwConfigRxDiffUnitsGet(system_registry))

   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: max_ext_diff_rx_clk_rec_if = %d is invalid\n",
                      (WP_U32)params->max_ext_diff_rx_clk_rec_if));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }

   if (params->max_ext_diff_tx_clk_rec_if > WUFEI_SystemClkRecHwConfigTxDiffUnitsGet(system_registry))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: max_ext_diff_tx_clk_rec_if = %d is invalid\n",
                      (WP_U32)params->max_ext_diff_tx_clk_rec_if));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }

   if((params->max_rx_clk_rec_if > 0 && params->method >= WUFE_CLOCK_REC_METHOD_NA) &&
      (params->max_tx_clk_rec_if > 0 && params->method >= WUFE_CLOCK_REC_METHOD_NA))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: method =%d is invalid\n",
                      (WP_U32)params->method));
      return WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS;
   }

   if ((params->max_tx_clk_rec_if > 0 || params->max_ext_diff_tx_clk_rec_if > 0) &&
       ((params->drc != NULL) || (params->ext_drc != NULL)))
   {
      if ((params->max_tx_clk_rec_if > 0) &&
          ((params->drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_int == 0) ||
           (params->drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_int == 0) ||
           (params->drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_int == 0)||
           (params->drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_int == 0)))
      {
         WUFE_PRINT_ERR((wufei_log,
                         "ERROR: nominal_divisor_int, for regular mode, is zero\n"));
         return (WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS);
      }
      if ((params->max_ext_diff_tx_clk_rec_if > 0) &&
          ((params->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_int == 0) ||
           (params->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_int == 0) ||
           (params->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_int == 0)||
           (params->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_int == 0)))
      {
         WUFE_PRINT_ERR((wufei_log,
                         "ERROR: nominal_divisor_int, for extended mode, is zero\n"));
         return (WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS);
      }

      if ((params->sync_mode != WUFE_CLOCK_REC_SYNC_POS) &&
          (params->sync_mode != WUFE_CLOCK_REC_SYNC_NEG))
      {
         WUFE_PRINT_ERR((wufei_log,
                         "ERROR: sync_mode = %d is invalid\n", (WP_U32)params->sync_mode));
         return (WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS);
      }

      if ((params->active_mode != WUFE_CLOCK_REC_ACTIVE_HIGH) &&
          (params->active_mode != WUFE_CLOCK_REC_ACTIVE_LOW))
      {
         WUFE_PRINT_ERR((wufei_log,
                         "ERROR: active_mode = %d is invalid\n", (WP_U32)params->active_mode));
         return (WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS);
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecDrcDivisorDefaultSet
 *
 * Purpose:  Initializes the clock recovery clock default set
 *
 * Description:
 *
 * Inputs:
 *     ref_clk
 *     params
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     WUFE_OK Upon successful completion.
 *     Otherwise the following error codes may be returned:
 *          WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS
 *          WUFE_ERR_CLOCK_REC_NUM_LINES
 *          WUFE_ERR_MALLOC
 *
 * Called by:
 *      WUFE_SystemConfig()
 *
 ****************************************************************************/
static void WUFEI_ClockRecDrcDivisorDefaultSet(WUFE_clk_rec_drc_divisor_list *ref_clk,
                                               WUFE_clk_rec_drc_divisor_list *params)
{
#ifdef WUFE_UFE2PLUS
   return;
#endif

   if (params)
      memcpy(ref_clk, params, sizeof(WUFE_clk_rec_drc_divisor_list));
   else
      memcpy(ref_clk, &wufe_drc_divisor_default, sizeof(WUFE_clk_rec_drc_divisor_list));
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecExtDrcDivisorDefaultSet
 *
 * Purpose:  Initializes the extended differential clock recovery DRC default set
 *
 * Description:
 *
 * Inputs:
 *     ref_clk
 *     params
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     WUFE_OK Upon successful completion.
 *     Otherwise the following error codes may be returned:
 *          WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS
 *          WUFE_ERR_CLOCK_REC_NUM_LINES
 *          WUFE_ERR_MALLOC
 *
 * Called by:
 *      WUFE_SystemConfig()
 *
 ****************************************************************************/
static void WUFEI_ClockRecExtDrcDivisorDefaultSet(WUFE_clk_rec_drc_divisor_list *ref_clk,
                                                  WUFE_clk_rec_drc_divisor_list *params)
{
#ifdef WUFE_UFE2PLUS
   return;
#endif

   if (params)
      memcpy(ref_clk, params, sizeof(WUFE_clk_rec_drc_divisor_list));
   else
      memcpy(ref_clk, &wufe_ext_drc_divisor_default, sizeof(WUFE_clk_rec_drc_divisor_list));
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecSystemSetup
 *
 * Purpose:  Initializes the clock recovery system
 *
 * Description: Initializes the Timestamp RX/TX mashine register
 *            for the specified Line. Allocates the memory for clock recovery
 *            resources.
 *
 * Inputs:
 *     system_registry - pointer to the UFE system registry structure
 *     params - global parameters per clock recovery system
 *
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     WUFE_OK Upon successful completion.
 *     Otherwise the following error codes may be returned:
 *          WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS
 *          WUFE_ERR_CLOCK_REC_NUM_LINES
 *          WUFE_ERR_MALLOC
 *
 * Called by:
 *      WUFE_SystemConfig()
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecSystemSetup(WUFEI_system_registry *system_registry,
                                      WUFE_clock_rec_system *params)
{
   WP_U32 wufe_id;
   WUFE_status status;
   WUFEI_clk_rec_registry *clk_rec_registry;

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_ClockRecSystemCheckParams(system_registry,
                                                         params));

   /* allocate memory for clock recovery registry */
   clk_rec_registry = (WUFEI_clk_rec_registry*)WUFEI_MALLOC(sizeof(WUFEI_clk_rec_registry));
   if (!(clk_rec_registry))
      return (WUFE_ERR_MALLOC);

   /* clear the allocated memory */
   memset(clk_rec_registry, 0, sizeof(WUFEI_clk_rec_registry));
   system_registry->clk_rec = clk_rec_registry;

   /* copy the user configuration structure to the registry */
   memcpy(&clk_rec_registry->cfg, params, sizeof(WUFE_clock_rec_system));

   /* specify the drc nominal divisor for regular differential mode or adaptive mode */
   WUFEI_ClockRecDrcDivisorDefaultSet(&clk_rec_registry->drc, params->drc);
   /* specify the ext_drc nominal divisor for extended differential mode */
   WUFEI_ClockRecExtDrcDivisorDefaultSet(&clk_rec_registry->ext_drc, params->ext_drc);

   system_registry->clk_rec->max_hw_rx_clk_rec_if =
      WUFEI_SystemClkRecHwConfigRxAdapUnitsGet(system_registry);

   system_registry->clk_rec->max_hw_ext_diff_rx_clk_rec_if =
      WUFEI_SystemClkRecHwConfigRxDiffUnitsGet(system_registry);

   system_registry->clk_rec->max_hw_tx_clk_rec_if =
      WUFEI_SystemClkRecHwConfigTxAdapUnitsGet(system_registry);

   system_registry->clk_rec->max_hw_ext_diff_tx_clk_rec_if =
      WUFEI_SystemClkRecHwConfigTxDiffUnitsGet(system_registry);

   /* clean the clock recovery registers and clock recovery RX RAM */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_ClockRecReset(wufe_id, system_registry));

   /* allocate memory for RX clock recovery registry array */
   if (params->max_rx_clk_rec_if > 0 || params->max_ext_diff_rx_clk_rec_if > 0)
   {
      WP_U32 size, i, max_if;

      if (params->max_ext_diff_rx_clk_rec_if > 0)
      {
         size = (WUFE_CLOCK_REC_REGULAR_INSTANCES + params->max_ext_diff_rx_clk_rec_if) *
            sizeof(WUFEI_clk_rec_rx);
      }
      else
      {
         size = params->max_rx_clk_rec_if * sizeof(WUFEI_clk_rec_rx);
      }

      clk_rec_registry->rx_arr = (WUFEI_clk_rec_rx *) WUFEI_MALLOC(size);
      if (!(clk_rec_registry->rx_arr))
         return (WUFE_ERR_MALLOC);

      /*set the state_mode to empty*/
      memset(clk_rec_registry->rx_arr, 0, size);

      if (params->max_ext_diff_rx_clk_rec_if > 0)
         max_if = WUFE_CLOCK_REC_REGULAR_INSTANCES + params->max_ext_diff_rx_clk_rec_if;
      else
         max_if = params->max_rx_clk_rec_if;
      size = sizeof(WP_U8)*system_registry->cfg.max_line_number;
      for(i=0; i < max_if; i++)
      {
         clk_rec_registry->rx_arr[i].line_id_arr =(WP_U8*)WUFEI_MALLOC(size);
         if (!clk_rec_registry->rx_arr[i].line_id_arr)
            return WUFE_ERR_MALLOC;

         /*set empty line array*/
         memset(clk_rec_registry->rx_arr[i].line_id_arr, 0, size);
      }
   }

   /* allocate memory for Tx clock recovery registry array */
   if (params->max_tx_clk_rec_if > 0 || params->max_ext_diff_tx_clk_rec_if > 0)
   {
      WP_U32 size;

      if (params->max_ext_diff_tx_clk_rec_if > 0)
      {
         size = (WUFE_CLOCK_REC_REGULAR_INSTANCES + params->max_ext_diff_tx_clk_rec_if) *
            sizeof(WUFEI_clk_rec_tx);
      }
      else
      {
         size = params->max_tx_clk_rec_if * sizeof(WUFEI_clk_rec_tx);
      }

      clk_rec_registry->tx_arr = (WUFEI_clk_rec_tx *) WUFEI_MALLOC(size);
      if (!(clk_rec_registry->tx_arr))
         return (WUFE_ERR_MALLOC);

      /*set the state_mode to empty*/
      memset(clk_rec_registry->tx_arr, 0, size);
   }

   /* specify the clock recovery alg.*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwClockRecMethodSet(wufe_id, params->method));

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecReset
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 *
 * Inputs:
 *     wufe_id
 *     system_registry
 *
 * Outputs:
 *     TX/RX timestamp registers and the RX Clock Generator RAM  are reseted
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *      WUFEI_ClockRecSystemSetup()
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecReset(WP_U32 wufe_id, WUFEI_system_registry* system_registry)
{
   /*clean the SBI registers and SBI RAM */
   WUFE_status status;
   WUFEI_clk_rec_registry *clk_rec_registry = system_registry->clk_rec;
   WP_U16 i;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif
   
   if (!clk_rec_registry)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   /* clear Clock Generator Indirect RX RAM for SBI line interface only */
   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      WP_U32 size;
      WP_U32 n_core;
      
      if (WUFEI_UfeCoreBandwidthGet(wufe_id) == 0)
         n_core = 2;
      else
         n_core = 1;
      size = 168 * n_core * sizeof(WUFEI_hw_clk_rec_cgen_rx_ram);
      WUFEI_HwClockRecIdirectMemset(wufe_id, WUFEI_INDIR_CGEN_RX_RAM(0), 0, size);
   }

   /******************************************/
   /*** clear Timestamp General Cfg Register**/
   /******************************************/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwClockRecMethodSet(wufe_id, 0));

   /******************************************/
   /*** clear Timestamp Direct RX/TX Registers **/
   /******************************************/
   for(i = 0; i < clk_rec_registry->max_hw_rx_clk_rec_if; i++)
   {
      /*RX Timestamp registers reset*/
      WUFEI_HwClockRecRxTsModeRegSet(wufe_id, i, 0);

      /*x2 - for NO_MASK mode */
      WUFEI_HwClockRecIdirectMemset(wufe_id, WUFEI_INDIR_TS_RX_PORT_RAM(i),
                                    0,  sizeof(WUFEI_hw_clk_rec_ts_port_ram)*2);

      WUFEI_HwClockRecIdirectMemset(wufe_id, WUFEI_INDIR_TS_RX_MASK_RAM(i),
                                    0,  sizeof(WUFEI_hw_clk_rec_ts_mask_ram));

   }
   for(i = 0; i < clk_rec_registry->max_hw_ext_diff_rx_clk_rec_if; i++)
   {
      /* Rx Timestamp registers reset */
      WUFEI_HwClockRecRxTsBufferSizeRegSet(wufe_id, i, 0);
   }


   for(i = 0; i < clk_rec_registry->max_hw_tx_clk_rec_if; i++)
   {
      /*TX Timestamp registers reset*/
      WUFEI_HwClockRecTxTsModeRegSet(wufe_id, i, 0);
      WUFEI_HwClockRecTxTsBufferSizeRegSet(wufe_id, i, 0);
   }

   for(i = 0; i < clk_rec_registry->max_hw_ext_diff_tx_clk_rec_if; i++)
   {
      /* NO TX TS in diff mode!!! */
   }

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecSystemDelete
 *
 * Purpose:  Release memory allocated for clock recovery within the UFE system
 *
 * Description:
 *
 * Inputs:
 *     system_registry - pointer to the UFE system registry structure
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     WUFE_OK Upon successful completion.
 *     Otherwise returns an error code.
 *
 * Called by:
 *      WUFEI_SystemDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecSystemDelete(WUFEI_system_registry *system_registry)
{
   WUFEI_clk_rec_registry *clk_rec_registry = system_registry->clk_rec;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!clk_rec_registry)
      return WUFE_OK;

   if (clk_rec_registry->rx_arr)
   {
      WP_U32 i;
      for(i=0;
          i<(clk_rec_registry->cfg.max_rx_clk_rec_if +
             clk_rec_registry->cfg.max_ext_diff_rx_clk_rec_if);
          i++)
      {
         if (clk_rec_registry->rx_arr[i].line_id_arr)
            WUFEI_FREE(clk_rec_registry->rx_arr[i].line_id_arr);
      }
      WUFEI_FREE(clk_rec_registry->rx_arr);
   }

   if (clk_rec_registry->tx_arr)
      WUFEI_FREE(clk_rec_registry->tx_arr);

   WUFEI_FREE(clk_rec_registry);

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecBufferSize
 *
 * Purpose:   Calculates the buffer size for the Timestamp machine
 *
 * Description:
 *
 * Inputs:
 *     data_unit_size - tx data unit  size
 *     n_slots - number of slots
 *     line_transfer_type - T1, E1, DS3, E3, etc.
 *     framed_mode - framed or unframed
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     buffer_size - output value in bits
 *
 * Called by:
 *     WUFEI_ClockRecTxCreate  functions.
 *
 ****************************************************************************/
static WP_U16 WUFEI_ClockRecBufferSize(WP_U32 data_unit_size, WP_U32 n_slots,
                                       WUFE_line_transf_type line_transfer_type,
                                       WUFE_un_framed framed_mode)
{
   WP_U16 buffer_size;
#ifdef WUFE_UFE2PLUS
   return 0;
#endif

   if( data_unit_size % n_slots)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: Fractional data_unit_size %d, n_slots %d\n",
                      data_unit_size, n_slots));
      return WUFE_CLOCK_REC_BUFFER_SIZE_INVALID;
   }

   /*calculate the buffer size, according to the transfer type*/
   if(line_transfer_type == WUFE_SBI_TYPE_T1 ||
      line_transfer_type == WUFE_CAD_TYPE_T1)
   {
      /*the number of clock required to fill buffer in the UFE*/
      if (framed_mode == WUFE_UNFRAMED)
         /*unframed all 193 bits are data,
           therefore 192 T1 clocks will fill the buffer*/
         buffer_size = data_unit_size * 192 / 24 ;
      else
         /*framed mode only 192 bits are data (1 bit for signaling),
           therefore 193 T1 clocks will fill the buffer*/
         buffer_size = data_unit_size * 193 / n_slots;
   }
   else if(line_transfer_type == WUFE_SBI_TYPE_E1 ||
           line_transfer_type == WUFE_CAD_TYPE_E1)
   {
      if (framed_mode == WUFE_UNFRAMED)
         buffer_size = data_unit_size * 256 / 32;
      else
         buffer_size = data_unit_size * 256 / n_slots;
   }
   else if(line_transfer_type == WUFE_SBI_TYPE_DS3 ||
           line_transfer_type == WUFE_SBI_TYPE_E3 )
   {
      if(framed_mode == WUFE_UNFRAMED)
         buffer_size = data_unit_size * 2;
      else/*CHECK: DS3/E3, CAD HMVIP*/
         buffer_size = data_unit_size * 2;
   }
   else
   {
      buffer_size = WUFE_CLOCK_REC_BUFFER_SIZE_INVALID;
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: WUFEI_ClockRecBufferSize invalid transfer type\n"));
      return buffer_size;
   }

   return (buffer_size);
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecExtDiffBufferSize
 *
 * Purpose:   Calculates the buffer size for the Timestamp machine for extended
 *            differential mode only.
 *
 * Description:
 *
 * Inputs:
 *     data_unit_size - tx data unit  size
 *     n_slots - number of slots
 *     line_transfer_type - T1, E1, DS3, E3, etc.
 *     framed_mode - framed or unframed
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     buffer_size - output value in bits
 *
 * Called by:
 *     WUFEI_ClockRecRxEnable.
 *
 ****************************************************************************/
static WP_U16 WUFEI_ClockRecExtDiffBufferSize(WP_U32 data_unit_size, WP_U32 n_slots,
                                              WUFE_line_transf_type line_transfer_type,
                                              WUFE_un_framed framed_mode)
{
   WP_U16 buffer_size;
#ifdef WUFE_UFE2PLUS
   return 0;
#endif

   if( data_unit_size % n_slots)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: Fractional data_unit_size %d, n_slots %d\n",
                      data_unit_size, n_slots));
      return WUFE_CLOCK_REC_BUFFER_SIZE_INVALID;
   }

   /* calculate the buffer size, according to the transfer type and framing modes */
   /* In the current implementation, the dummy TS thread is opened every 125usec
      in both framed and unframed modes. */
   if(line_transfer_type == WUFE_SBI_TYPE_T1 ||
      line_transfer_type == WUFE_CAD_TYPE_T1)
   {
      buffer_size = 193;
   }
   else if(line_transfer_type == WUFE_SBI_TYPE_E1 ||
           line_transfer_type == WUFE_CAD_TYPE_E1)
   {
      buffer_size = 256;
   }
   else if(line_transfer_type == WUFE_SBI_TYPE_DS3 ||
           line_transfer_type == WUFE_SBI_TYPE_E3  ||
           line_transfer_type == WUFE_CAD_TYPE_DS3 ||
           line_transfer_type == WUFE_CAD_TYPE_E3)
   {
      if(framed_mode == WUFE_UNFRAMED)
         buffer_size = data_unit_size * 2;
      else/*CHECK: DS3/E3, CAD HMVIP*/
         buffer_size = data_unit_size * 2;
   }
   else
   {
      buffer_size = WUFE_CLOCK_REC_BUFFER_SIZE_INVALID;
      WUFE_PRINT_ERR((wufei_log,
                      "ERROR: WUFEI_ClockRecBufferSize invalid transfer type\n"));
      return buffer_size;
   }

   return (buffer_size);

}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxTimestampRegInit
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     wufe_id - UFE id
 *     clk_rec_if  - clock recovery timestamp unit id
 *     line_transfer_type - line transfer type
 *     framed_mode - line frame mode
 *
 * Outputs:
 *     TX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecRxCreate function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxTimestampRegInit(WP_U32 wufe_id,
                                                    WP_U16 clk_rec_if,
                                                    WUFE_line_transf_type line_transfer_type,
                                                    WUFE_un_framed framed_mode)
{
   WP_U16 mode;
   ((void)wufe_id);
   ((void)clk_rec_if);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* read the RX Timestamp mode register*/
   WUFEI_HwClockRecRxTsModeRegGet(wufe_id, clk_rec_if, &mode);

   /* Mask Ram is used only for the framed lines */
   if(framed_mode == WUFE_UNFRAMED ||
      line_transfer_type == WUFE_CAD_TYPE_DS3 ||
      line_transfer_type == WUFE_SBI_TYPE_DS3 ||
      line_transfer_type == WUFE_CAD_TYPE_E3 ||
      line_transfer_type == WUFE_SBI_TYPE_E3)
      mode |= WUFEI_HW_CLK_REC_RX_NO_MASK_M;
   else
      mode &= ~WUFEI_HW_CLK_REC_RX_NO_MASK_M;

   /* Fast port enable */
   if(line_transfer_type == WUFE_CAD_TYPE_E3 ||
      line_transfer_type == WUFE_CAD_TYPE_DS3 )
      mode |= WUFEI_HW_CLK_REC_RX_FAST_PORT_M;
   else
      mode &= ~WUFEI_HW_CLK_REC_RX_FAST_PORT_M;

   /* update the RX Timestamp mode register */
   WUFEI_HwClockRecRxTsModeRegSet(wufe_id, clk_rec_if, mode);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxTimestampEnable
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     wufe_id - UFE id
 *     clk_rec_if - clock recovery timestamp unit id
 *
 * Outputs:
 *     TX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecRxEnable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxTimestampEnable(WP_U32 wufe_id, WP_U16 clk_rec_if)
{
   WP_U16 mode =0;
   ((void)wufe_id);
   ((void)clk_rec_if);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* read the RX Timestamp mode register*/
   WUFEI_HwClockRecRxTsModeRegGet(wufe_id, clk_rec_if, &mode);

   /* Timestamp unit enable */
   mode |= WUFEI_HW_CLK_REC_RX_MODE_EN_M;

   /* update the RX Timestamp mode register */
   WUFEI_HwClockRecRxTsModeRegSet(wufe_id, clk_rec_if, mode);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxTimestampDisable
 *
 * Purpose:   Disables the Timestamp RX mode register
 *
 * Description:
 *
 * Inputs:
 *     wufe_id - UFE id
 *     clk_rec_if - clock recovery timestamp unit id
 *
 * Outputs:
 *     TX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecRxDisable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxTimestampDisable(WP_U32 wufe_id, WP_U16 clk_rec_if)
{
   WP_U16 mode =0;
   ((void)wufe_id);
   ((void)clk_rec_if);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* read the RX Timestamp mode register*/
   WUFEI_HwClockRecRxTsModeRegGet(wufe_id, clk_rec_if, &mode);

   mode &= ~WUFEI_HW_CLK_REC_RX_MODE_EN_M;

   /* update the RX Timestamp mode register */
   WUFEI_HwClockRecRxTsModeRegSet(wufe_id, clk_rec_if, mode);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxTimestampDelete
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     wufe_id - UFE id
 *     clk_rec_if - clock recovery timestamp unit id
 *
 * Outputs:
 *     TX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecRxDelete function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxTimestampDelete(WP_U32 wufe_id, WP_U16 clk_rec_if)
{
   ((void)wufe_id);
   ((void)clk_rec_if);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* update the RX Timestamp mode register */
   WUFEI_HwClockRecRxTsModeRegSet(wufe_id, clk_rec_if, 0);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTxCreate
 *
 * Purpose:   Initializes the Timestamp TX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     TX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecTxMasterSet function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecTxCreate(WUFEI_system_registry* system_registry, WP_U16 phy_id)
{
   WP_U16 line_id;
   WP_U16 buffer_size, clk_rec_if_id;
   WP_U32 data_unit_size, n_slots;
   WP_U32 wufe_id;
   WUFE_line_transf_type line_transfer_type;
   WUFE_un_framed framed_mode;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WUFE_status status;

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));
   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   clk_rec_if_id = WUFEI_LineClockRecIfTxGet(line_registry);

   /* get various PHY and LINE parameters for further timestamp required calculations */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyDataUnitSizeGet(system_registry,
                                                  phy_id, &data_unit_size,
                                                  WUFE_TX_DIRECTION));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyNumSlotsGet(phy_registry, &n_slots,
                                              WUFE_TX_DIRECTION));


   line_transfer_type = WUFEI_LineTransferTypeGet(line_registry);

   framed_mode = WUFEI_LineFramedModeGet(line_registry);

   buffer_size = WUFEI_ClockRecBufferSize(data_unit_size, n_slots,
                                          line_transfer_type, framed_mode);

   if(buffer_size == WUFE_CLOCK_REC_BUFFER_SIZE_INVALID)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFE:ERROR clock recovery: Invalid TX buffer size\n"));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_CLOCK_REC_INVALID_CONFIG;
   }

   /* Write the TX Timestamp buffer size register*/
   WUFEI_HwClockRecTxTsBufferSizeRegSet(wufe_id, clk_rec_if_id, buffer_size);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTxDelete
 *
 * Purpose:   Clears the Timestamp TX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     TX timestamp registers are cleared
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecPhyDelete function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecTxDelete(WUFEI_system_registry* system_registry, WP_U16 phy_id)
{
   WP_U16 line_id, clk_rec_if_id;
   WP_U32 wufe_id;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WUFE_status status;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));
   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   clk_rec_if_id = WUFEI_LineClockRecIfTxGet(line_registry);

   /* calculate and write the TX Timestamp buffer size register*/
   WUFEI_HwClockRecTxTsBufferSizeRegSet(wufe_id, clk_rec_if_id, 0);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTxEnable
 *
 * Purpose:   Sets enable bits in the Timestamp TX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     TX timestamp registers are cleared
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecPhyEnable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecTxEnable(WUFEI_system_registry* system_registry,
                                          WP_U32 core_id,
                                          WP_U16 emphy_port_id)
{
   WP_U16 line_id, phy_id;
   WP_U16 mode=0;
   WP_U32 wufe_id;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WUFE_status status;
   WP_U16 clk_rec_if;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                    core_id, &phy_id, emphy_port_id));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));
   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   clk_rec_if = WUFEI_LineClockRecIfTxGet(line_registry);

   if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (clk_rec_if >= system_registry->clk_rec->cfg.max_tx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
   }
   else  /* ext diff mode */
   {
      if (clk_rec_if - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_tx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
   }

   /*set or clear enable bit in the TX Timestamp mode register*/
   WUFEI_HwClockRecTxTsModeRegGet(wufe_id, clk_rec_if, &mode);
   mode |= WUFEI_HW_CLK_REC_TX_MODE_EN_M;
   WUFEI_HwClockRecTxTsModeRegSet(wufe_id, clk_rec_if, mode);
   if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   { /* regular mode */
      /*update FIFO manager in the TSPT*/
      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyClockRecTSUpdate(wufe_id, core_id,
                                                              emphy_port_id,
                                                              clk_rec_if,
                                                              WUFE_TX_DIRECTION,
                                                              WUFE_ENABLE));
   }
   else
   { /* In extended differential mode, must not enable entry 9 in TSPT */
      /*update FIFO manager in the TSPT*/
      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyClockRecTSUpdate(wufe_id, core_id,
                                                              emphy_port_id,
                                                              clk_rec_if,
                                                              WUFE_TX_DIRECTION,
                                                              WUFE_DISABLE));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTxDisable
 *
 * Purpose:   Sets low the enable bits in the Timestamp TX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     emphy_port_id - emphy HW port ID
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     TX timestamp registers are cleared
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecPhyDisable  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecTxDisable(WUFEI_system_registry* system_registry,
                                           WP_U32 core_id,
                                           WP_U16 emphy_port_id,
                                           WP_U16 phy_id)
{
   WP_U16 line_id;
   WP_U16 mode=0, clk_rec_if;
   WP_U32 wufe_id;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WUFE_status status;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));
   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   clk_rec_if = WUFEI_LineClockRecIfTxGet(line_registry);

   if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (clk_rec_if >= system_registry->clk_rec->cfg.max_tx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
   }
   else  /* ext diff mode */
   {
      if (clk_rec_if - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_tx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
   }

   /* needed only for regular mode, do nothing in extended mode */
   if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      /*set or clear enable bit in the TX Timestamp mode register*/
      WUFEI_HwClockRecTxTsModeRegGet(wufe_id, clk_rec_if, &mode);
      mode &= ~WUFEI_HW_CLK_REC_TX_MODE_EN_M;
      WUFEI_HwClockRecTxTsModeRegSet(wufe_id, clk_rec_if, mode);

      /*update FIFO manager in the TSPT*/
      WUFEI_RETURN_IF_ERROR(status,WUFEI_PhyClockRecTSUpdate(wufe_id, core_id,
                                                             emphy_port_id,
                                                             clk_rec_if,
                                                             WUFE_TX_DIRECTION,
                                                             WUFE_DISABLE));
   }

   return WUFE_OK;
}

/*****************************************************************************
 * Function:  WUFEI_ClockRecRxCreate
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     RX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecRxMasterSet function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxCreate(WUFEI_system_registry* system_registry,
                                          WUFEI_line_registry* line_registry,
                                          WP_U16 line_id)
{
   WUFEI_clk_rec_rx *clk_rec_if;
   WP_U16 clk_rec_if_id;
   WP_U32 wufe_id;
   WUFE_un_framed framed_mode;
   WUFE_line_transf_type line_transfer_type;
   WUFE_status status;

#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   /*get various PHY and LINE parameters for further timestamp required calculations*/
   line_transfer_type = WUFEI_LineTransferTypeGet(line_registry);

   framed_mode = WUFEI_LineFramedModeGet(line_registry);

   clk_rec_if_id = WUFEI_LineClockRecIfRxGet(line_registry);

   /*Initiates internal clock recovery structure*/
   clk_rec_if = &system_registry->clk_rec->rx_arr[clk_rec_if_id];
   clk_rec_if->transfer_type = line_transfer_type;
   clk_rec_if->framed = framed_mode;

   /* init RX TS only for CD<32 */
   if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      /*Initiate timestamp unit registers*/
      WUFEI_RETURN_IF_ERROR(status,WUFEI_ClockRecRxTimestampRegInit(wufe_id,
                                                                    clk_rec_if_id,
                                                                    line_transfer_type,
                                                                    framed_mode));
   }


   /*Initiate clock generator RAM*/
   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      WUFEI_ClockRecRxClockGenRamCreate(wufe_id,
                                        line_id,
                                        clk_rec_if_id,
                                        line_transfer_type);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxDelete
 *
 * Purpose:   Clears the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     RX timestamp registers are cleared
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecPhyDelete function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxDelete(WUFEI_system_registry* system_registry,
                                          WP_U16 line_id)
{
   WUFEI_clk_rec_rx *clk_rec_if;
   WP_U16 clk_rec_if_id;
   WP_U32 wufe_id;
   WUFEI_line_registry *line_registry;
   WUFE_status status;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   clk_rec_if_id = WUFEI_LineClockRecIfRxGet(line_registry);

   /*Delete internal clock recovery structure*/
   clk_rec_if = &system_registry->clk_rec->rx_arr[clk_rec_if_id];
   clk_rec_if->transfer_type = 0;
   clk_rec_if->framed = 0;

   if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      /* Delete the timestamp unit. needed for regular mode only. */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_ClockRecRxTimestampDelete(wufe_id, clk_rec_if_id));

   /*Delete clock generator RAM*/
   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
      WUFEI_ClockRecRxClockGenRamDelete(wufe_id, line_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxRamSize
 *
 * Purpose:   Calculates the Port & Mask Ram size
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *     line_transfer_type - line transfer type (E1/T1/DS3/E3)
 *     ram_size - maximal ram size
 *
 * Outputs:
 *     the maximal ram size value
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *      WUFE_ERR_CLOCK_REC_NOT_ENABLED
 *
 * Called by:
 *      WUFEI_ClockRecRxEnable  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxRamSize(WP_U16 clk_rec_if,
                                           WP_U32 wufe_id,
                                           WUFE_line_transf_type line_transfer_type,
                                           WP_U32 data_unit_size,
                                           WP_U16 *ram_size)
{
   WP_U16 mode = 0;
   ((void)wufe_id);
   ((void)clk_rec_if);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /*get the RX Timestamp mode configuration register*/
   WUFEI_HwClockRecRxTsModeRegGet(wufe_id, clk_rec_if, &mode);

   /*using single ram configuration (Port Ram)*/
   if(WUFEI_HW_CLK_REC_RX_NO_MASK_V(mode))
   {
      if (line_transfer_type == WUFE_SBI_TYPE_E3 ||
          line_transfer_type == WUFE_CAD_TYPE_E3 ||
          line_transfer_type == WUFE_SBI_TYPE_DS3 ||
          line_transfer_type == WUFE_CAD_TYPE_DS3)
         *ram_size = data_unit_size; /*clock is divided by 8 in UFE HW*/
      else
         *ram_size = data_unit_size*8;

   }
   /*using dual ram configuration (Port & Mask Ram)*/
   else
   {
      if (line_transfer_type == WUFE_SBI_TYPE_E1 ||
          line_transfer_type == WUFE_CAD_TYPE_E1)
      {
         *ram_size = 256;
      }
      else if(line_transfer_type == WUFE_SBI_TYPE_T1 ||
              line_transfer_type == WUFE_CAD_TYPE_T1)
      {
         *ram_size = 193;
      }
      else
      {
         *ram_size = data_unit_size; /*clock is divided by 8 in UFE HW*/
      }
   }

   /*slot 0 dedicated for dummy port*/
   (*ram_size)--;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxPortRamCreate
 *
 * Purpose:  Initiates the port ram
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxCreate  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxPortRamCreate(WP_U32 wufe_id,
                                                 WP_U16 clk_rec_if,
                                                 WP_U16 ram_size,
                                                 WP_U16 method)
{

   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
   WP_U16 value;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);
   ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);

   /*initiates the last slot*/
   if( ram_size >= WUFEI_MAX_N_HW_PORT_RAM_SLOT)
   {
      WP_U32 entry;

      entry = ram_size - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);

      if(!WUFEI_HW_CLK_REC_RX_PORT_LAST_V(value))
      {
         value |= WUFEI_HW_CLK_REC_RX_PORT_LAST_M;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataSet(wufe_id, value);
      }
   }
   else
   {
      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[ram_size]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);

      if(!WUFEI_HW_CLK_REC_RX_PORT_LAST_V(value))
      {
         value |= WUFEI_HW_CLK_REC_RX_PORT_LAST_M;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[ram_size]));
         WUFEI_HwClockRecIdirectDataSet(wufe_id, value);
      }
   }

   /*initiates the dummy port - first slot*/
   /*Differential only*/
   if(method == WUFE_CLOCK_REC_METHOD_DIFFERENTIAL)
   {
      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[0]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
      if(!WUFEI_HW_CLK_REC_RX_PORT_EN_V(value))
      {
         value |= WUFEI_HW_CLK_REC_RX_PORT_EN_M;
         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[0]));
         WUFEI_HwClockRecIdirectDataSet(wufe_id, value);
      }
   }


   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxPortRamDelete
 *
 * Purpose:  Initiates the port ram
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxDelete  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxPortRamDelete(WP_U32 wufe_id, WP_U16 clk_rec_if)
{

   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
   WP_U16 value = 0, i;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);
   ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);

   /*set last entry*/
   for( i=0; i<WUFEI_MAX_N_HW_PORT_RAM_SLOT*2; i++)
   {
      if( i>=WUFEI_MAX_N_HW_PORT_RAM_SLOT)
      {
         WP_U32 entry;

         entry = i - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataSet(wufe_id, 0);
      }
      else
      {
         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[i]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[i]));
         WUFEI_HwClockRecIdirectDataSet(wufe_id, 0);

      }

      if(WUFEI_HW_CLK_REC_RX_PORT_LAST_V(value))
         break;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxPortAndMaskRamReset
 *
 * Purpose:  Initiates the port ram and mask ram to zeros
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxEnable  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxPortAndMaskRamReset(WP_U32 wufe_id, WP_U16 clk_rec_if)
{

   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
   WP_U16 i;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);
   ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);

   for( i=0; i<WUFEI_MAX_N_HW_PORT_RAM_SLOT*2; i++)
   {
      if( i>=WUFEI_MAX_N_HW_PORT_RAM_SLOT)
      {
         WP_U32 entry;
         entry = i - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataSet(wufe_id, 0);
      }
      else
      {
         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[i]));
         WUFEI_HwClockRecIdirectDataSet(wufe_id, 0);
      }
   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxPortRamSlotIdGet
 *
 * Purpose:   Generates slot_id
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *     ram_size - maximal ram size
 *     phy_id - phy to add to the Port ram
 *
 * Outputs:
 *     the maximal ram size value
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxEnable  function.
 *
 ****************************************************************************/
#if 0
static WUFE_status WUFEI_ClockRecRxPortRamSlotIdGet( WP_U32 wufe_id,
                                               WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr,
                                               WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr,
                                               WP_U16 ram_size,
                                               WP_U16 *slot_id)
{
   WP_U16 value, rand_cnt = 0, i = 1;
   WP_U32 time_read_result;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

//   WPI_TimeRead(&time_read_result, wufe_id);
   WPI_TimeRead(&time_read_result);
   srand(time_read_result);

   /* calculates a random slot location*/
   do
   {
      if(rand_cnt++ < WUFEI_HW_CLK_REC_MAX_RAND)
      {
         /*acquire a random slot*/
         *slot_id = rand()% ram_size + 1;
      }
      else
      {
         /*acquire a free slot*/
         *slot_id = i++;

      }

      if( i >= ram_size)
         return WUFE_ERR_CLOCK_REC_NO_FREE_RAM_ENTRY;

      /*validate the slot is free*/
      if(*slot_id >= WUFEI_MAX_N_HW_PORT_RAM_SLOT)
      {
         WP_U32 entry;

         entry = *slot_id - WUFEI_MAX_N_HW_PORT_RAM_SLOT + 1;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
      }
      else
      {

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[*slot_id]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
      }

   }while(WUFEI_HW_CLK_REC_RX_PORT_EN_V(value));

   return WUFE_OK;

}

#else
static WUFE_status WUFEI_ClockRecRxPortRamSlotIdGet( WP_U32 wufe_id,
                                               WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr,
                                               WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr,
                                               WP_U16 ram_size,
                                               WP_U16 *slot_id)
{
   WP_U16 value = 0;
   WP_U32 i, max_slots = 0, n_slots = 0;
   ((void)wufe_id);
   ((void)ts_rx_port_ram_ptr);
   ((void)ts_rx_mask_ram_ptr);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   for( i = 1; i<=ram_size; i++)
   {
      /*find the max slots section*/
      if(i >= WUFEI_MAX_N_HW_PORT_RAM_SLOT)
      {
         WP_U32 entry;

         entry = i - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
      }
      else
      {

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[i]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
      }

      if(WUFEI_HW_CLK_REC_RX_PORT_EN_V(value))
      {
         if( max_slots < n_slots)
         {
            max_slots = n_slots;
            *slot_id = i - n_slots/2;
         }
         n_slots = 0;
      }
      else
         n_slots ++;

      if(WUFEI_HW_CLK_REC_RX_PORT_LAST_V(value) && max_slots == 0)
         *slot_id = ram_size;

   }

   /*initiate counter*/
   i = 1;

   do
   {
      if( i > ram_size)
         return WUFE_ERR_CLOCK_REC_NO_FREE_RAM_ENTRY;

      /*validate the slot is free*/
      if(*slot_id >= WUFEI_MAX_N_HW_PORT_RAM_SLOT)
      {
         WP_U32 entry;

         entry = *slot_id - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
      }
      else
      {

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[*slot_id]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
      }
      if(WUFEI_HW_CLK_REC_RX_PORT_EN_V(value))
      {
         /*acquire a free slot*/
         *slot_id = i++;

      }

   }while(WUFEI_HW_CLK_REC_RX_PORT_EN_V(value));

   return WUFE_OK;

}
#endif
/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxPortRamEnable
 *
 * Purpose:   Adds the phy to the Port ram
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *     ram_size - maximal ram size
 *     phy_id - phy to add to the Port ram
 *
 * Outputs:
 *     the maximal ram size value
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxEnable  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxPortRamEnable(WP_U32 wufe_id,
                                                 WP_U32 core_id,
                                                 WP_U16 clk_rec_if,
                                                 WP_U16 ram_size,
                                                 WP_U16 phy_id,
                                                 WP_U16 slot_id)

{
   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
   WP_U16 value=0;
   ((void)wufe_id);
   ((void)ram_size);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);
   ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);


   /*validate the stot is free*/
   if(slot_id >= WUFEI_MAX_N_HW_PORT_RAM_SLOT)
   {
      WP_U32 entry;

      entry = slot_id - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
   }
   else
   {

      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[slot_id]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
   }

   /*configure the ram entry*/
   if(WUFEI_HW_CLK_REC_RX_PORT_LAST_V(value))
      value = ( WUFEI_HW_CLK_REC_RX_PORT_EN_M |
                WUFEI_HW_CLK_REC_RX_PORT_LAST_M |
                WUFEI_HW_CLK_REC_RX_CORE_ID_F(core_id) |
                WUFEI_HW_CLK_REC_RX_PORT_ID_F(phy_id));
   else
      value = ( WUFEI_HW_CLK_REC_RX_PORT_EN_M |
                WUFEI_HW_CLK_REC_RX_CORE_ID_F(core_id) |
                WUFEI_HW_CLK_REC_RX_PORT_ID_F(phy_id));

   if(slot_id >= WUFEI_MAX_N_HW_PORT_RAM_SLOT)
   {
       WP_U32 entry;

       entry = slot_id - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

       WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
       WUFEI_HwClockRecIdirectDataSet(wufe_id, value);

   }
   else
   {
      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[slot_id]));
      WUFEI_HwClockRecIdirectDataSet(wufe_id, value);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxMaskRamEnable
 *
 * Purpose:   Adds the phy to the Mask ram
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *     ram_size - maximal ram size
 *     phy_id - phy to add to the Port ram
 *
 * Outputs:
 *     the maximal ram size value
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxEnable  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxMaskRamEnable(WP_U32 wufe_id,
                                                 WP_U16 clk_rec_if,
                                                 WP_U16 slot_id,
                                                 WP_U32 data_unit_size,
                                                 WP_U16 n_slots)
{
   WP_U16 mode = 0;
   ((void)slot_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /*get the RX Timestamp mode configuration register*/
   WUFEI_HwClockRecRxTsModeRegGet(wufe_id, clk_rec_if, &mode);

   if(!WUFEI_HW_CLK_REC_RX_NO_MASK_V(mode))
   {
      WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
      WP_U16 value, load_value, mask_cnt;
      WP_U32 time_read_result;

      ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);

      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[slot_id]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);

      if(data_unit_size % n_slots)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR invalid data_unit_size(%d) mod n_slots(%d)\n ",
                         data_unit_size, n_slots));
         return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);
      }
      else
      {
         load_value =  data_unit_size/n_slots - 1;
      }

//      WPI_TimeRead(&time_read_result, wufe_id);
      WPI_TimeRead(&time_read_result);
      srand(time_read_result);

      /* Random value 0-load value*/
      if(load_value)
         mask_cnt = rand() % (load_value + 1);
      else
         mask_cnt = 0;

      value = (WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_F(load_value)|
               WUFEI_HW_CLK_REC_RX_MASK_CNT_F(mask_cnt));

      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[slot_id]));
      WUFEI_HwClockRecIdirectDataSet(wufe_id, value);

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxMaskRamDisable
 *
 * Purpose:   Clears the Mask ram entry for a specified phy
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *     ram_size - maximal ram size
 *     emphy_port_id - phy to add to the Port ram
 *
 * Outputs:
 *     the maximal ram size value
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxEnable  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxMaskRamDisable(WP_U32 wufe_id,
                                                  WP_U16 clk_rec_if,
                                                  WP_U16 emphy_port_id)
{
   WP_U16 i, mode = 0, value = 0;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /*get the RX Timestamp mode configuration register*/
   WUFEI_HwClockRecRxTsModeRegGet(wufe_id, clk_rec_if, &mode);

   if(!WUFEI_HW_CLK_REC_RX_NO_MASK_V(mode))
   {
      WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
      ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);

      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[0]));
      for(i=0; (i<WUFEI_MAX_N_HW_PORT_RAM_SLOT) && (!WUFEI_HW_CLK_REC_RX_PORT_LAST_V(value)); i++)
      {
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
         if(WUFEI_HW_CLK_REC_RX_PORT_ID_V(value) == emphy_port_id)
         {
            WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;

            ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);

            WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[i]));
            WUFEI_HwClockRecIdirectDataSet(wufe_id, 0);
            return WUFE_OK;

         }
      }
      return WUFE_ERR_CLOCK_REC_INVALID_PHY_ID;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxEnable
 *
 * Purpose:   Sets enable bits in the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     RX timestamp registers are cleared
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecPhyEnable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxEnable(WUFEI_system_registry* system_registry,
                                          WP_U32 core_id,
                                          WP_U16 emphy_port_id)
{
   WP_U16 line_id, phy_id;
   WP_U16 clk_rec_if_id;
   WUFEI_clk_rec_rx *clock_rec_if;
   WP_U32 wufe_id, n_slots, data_unit_size;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WUFE_line_transf_type line_transfer_type;
   WUFE_un_framed framed_mode;
   WUFE_status status;
   WP_U16 slot_id=0, ram_size=0;
   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr = NULL;
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr = NULL;

#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_UfeCorePhyIdBlockGet(system_registry,
                                                    core_id, &phy_id, emphy_port_id));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));
   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   /* get various PHY and LINE parameters for further timestamp required calculations */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyDataUnitSizeGet(system_registry,
                                                  phy_id, &data_unit_size,
                                                  WUFE_RX_DIRECTION));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyNumSlotsGet(phy_registry, &n_slots,
                                              WUFE_RX_DIRECTION));


   line_transfer_type = WUFEI_LineTransferTypeGet(line_registry);

   framed_mode = WUFEI_LineFramedModeGet(line_registry);

   clk_rec_if_id = WUFEI_LineClockRecIfRxGet(line_registry);

   /* The first 32 CD use HW shapers */
   if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES) /* CD 32-367 no shapers required */
   {

      if (clk_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);

      /* calculate the ram size*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_ClockRecRxRamSize(clk_rec_if_id,wufe_id,
                                                    line_transfer_type,
                                                    data_unit_size, &ram_size));
      /* calculate port and mask RAM pointers */
      ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if_id);
      ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if_id);
   }


   clock_rec_if = &system_registry->clk_rec->rx_arr[clk_rec_if_id];

   if(clock_rec_if->phy_n == 0)
   {
      WUFE_clock_rec_system *clock_rec_system = &system_registry->clk_rec->cfg;

      /* initials the data unit size */
      clock_rec_if->data_unit_size = data_unit_size;

      if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         /* regular CR mode. */

         /* clear the port and mask RAMs */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxPortAndMaskRamReset(wufe_id,
                                                                   clk_rec_if_id));

         /*initiate the RX Timestamp port ram*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxPortRamCreate(wufe_id,
                                                             clk_rec_if_id,
                                                             ram_size,
                                                             clock_rec_system->method));
         /* get slot_id for port and mask RAM */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxPortRamSlotIdGet(wufe_id, ts_rx_port_ram_ptr,
                                                                ts_rx_mask_ram_ptr,
                                                                ram_size, &slot_id));
         /*sets the Mask ram*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxMaskRamEnable(wufe_id, clk_rec_if_id,
                                                             slot_id, data_unit_size,
                                                             n_slots));
         /*sets the Port ram*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxPortRamEnable(wufe_id, core_id, clk_rec_if_id,
                                                             ram_size, emphy_port_id, slot_id));

         /*update FIFO manager in the RSPT (entry 9)*/
         WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyClockRecTSUpdate(wufe_id, core_id,
                                                                 emphy_port_id,
                                                                 clk_rec_if_id,
                                                                 WUFE_RX_DIRECTION,
                                                                 WUFE_ENABLE));
         /*enable the timestamp mode register*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxTimestampEnable(wufe_id, clk_rec_if_id));
      } /* end of regular CR (CD<32) */
      else
      {
         /* In case of CD > 32 no need to use shapers nor enable the RSPT;
            PHYs work as regular PHYs */

         WP_U16 buffer_size_rx = WUFEI_ClockRecExtDiffBufferSize(data_unit_size, n_slots,
                                                                 line_transfer_type, framed_mode);
         if(buffer_size_rx == WUFE_CLOCK_REC_BUFFER_SIZE_INVALID)
         {
            WUFE_PRINT_ERR((wufei_log,
                            "WUFE:ERROR clock recovery: Invalid RX buffer size\n"));
            WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
            return WUFE_ERR_CLOCK_REC_INVALID_CONFIG;
         }

         /* In RX direction only, the buffer size should be set to value-1 (255, 192)
            This is synthesis issue related */
         buffer_size_rx = buffer_size_rx -1;

         /* calculate and write the RX Timestamp  */
         WUFEI_HwClockRecRxTsBufferSizeRegSet(wufe_id,
                                              clk_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES,
                                              buffer_size_rx);
      }
   }
   else
   {

      /* unframed line must acquire PHYs with a common data unit size*/
      if(framed_mode == WUFE_UNFRAMED &&  clock_rec_if->data_unit_size != data_unit_size)
         return WUFE_ERR_CLOCK_REC_INVALID_DATA_UNIT_SIZE;

      if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         /* In case of CD > 32 no need to use shapers nor enable the RSPT;
            PHYs work as regular PHYs */

         /*update FIFO manager in the RSPT*/
         WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyClockRecTSUpdate(wufe_id, core_id,
                                                                 emphy_port_id,
                                                                 clk_rec_if_id,
                                                                 WUFE_RX_DIRECTION,
                                                                 WUFE_ENABLE));
         /* get slot_id for port and mask RAM */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxPortRamSlotIdGet(wufe_id, ts_rx_port_ram_ptr,
                                                                ts_rx_mask_ram_ptr,
                                                                ram_size, &slot_id));

         /*sets the Mask ram*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxMaskRamEnable(wufe_id, clk_rec_if_id,
                                                             slot_id, data_unit_size,
                                                             n_slots));
         /*sets the Port ram*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxPortRamEnable(wufe_id, core_id, clk_rec_if_id,
                                                             ram_size, emphy_port_id, slot_id));
      }

   }

   /*total phys counter */
   clock_rec_if->phy_n++;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxPortRamDisable
 *
 * Purpose:   Removes the phy from the Port ram
 *
 * Description:
 *
 * Inputs:
 *     clk_rec_if - Clock Recovery interface
 *     emphy_port_id - phy to add to the Port ram
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:

 *
 * Called by:
 *      WUFEI_ClockRecRxDisable  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxPortRamDisable(WP_U32 wufe_id,
                                                  WP_U32 core_id,
                                                  WP_U16 clk_rec_if,
                                                  WP_U16 emphy_port_id,
                                                  WP_U16 method)
{
   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
   WP_U16 i, start, value = 0;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);
   ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);

   if(method == WUFE_CLOCK_REC_METHOD_DIFFERENTIAL)
      start = 1; /*first entry reserved for dummy port*/
   else
      start = 0;

   for(i=start;(i<WUFEI_MAX_N_HW_PORT_RAM_SLOT*2) &&
      (!WUFEI_HW_CLK_REC_RX_PORT_LAST_V(value)); i++)
   {
      if(i < WUFEI_MAX_N_HW_PORT_RAM_SLOT)
      {

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[i]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);
         if(WUFEI_HW_CLK_REC_RX_PORT_ID_V(value) == emphy_port_id &&
            WUFEI_HW_CLK_REC_RX_CORE_ID_V(value) == core_id &&
            WUFEI_HW_CLK_REC_RX_PORT_EN_V(value))
         {
            value &= ~(WUFEI_HW_CLK_REC_RX_PORT_EN_M |
                       WUFEI_HW_CLK_REC_RX_PORT_ID_M |
                       WUFEI_HW_CLK_REC_RX_CORE_ID_M);
            WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[i]));
            WUFEI_HwClockRecIdirectDataSet(wufe_id, value);

            return WUFE_OK;
         }
      }
      else
      {
         WP_U32 entry;
         entry = i - WUFEI_MAX_N_HW_PORT_RAM_SLOT;

         WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &value);

         if(WUFEI_HW_CLK_REC_RX_PORT_ID_V(value) == emphy_port_id &&
            WUFEI_HW_CLK_REC_RX_CORE_ID_V(value) == core_id &&
            WUFEI_HW_CLK_REC_RX_PORT_EN_V(value))
         {
            value &= ~(WUFEI_HW_CLK_REC_RX_PORT_EN_M |
                       WUFEI_HW_CLK_REC_RX_PORT_ID_M |
                       WUFEI_HW_CLK_REC_RX_CORE_ID_M);

            WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[entry]));
            WUFEI_HwClockRecIdirectDataSet(wufe_id, value);
            return WUFE_OK;
         }
      }
   }
   return WUFE_ERR_CLOCK_REC_INVALID_PHY_ID;
}

/*****************************************************************************
 *
 * Function: WUFEI_ClockRecRxDisable
 *
 * Purpose:   Sets low the enable bits in the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *     RX timestamp registers are cleared
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecPhyDisable function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxDisable(WUFEI_system_registry* system_registry,
                                           WP_U32 core_id,
                                           WP_U16 emphy_port_id,
                                           WP_U16 phy_id)
{
   WP_U16 line_id;
   WP_U16 clk_rec_if_id;
   WP_U32 wufe_id;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WUFE_status status;
   WUFEI_clk_rec_rx *clk_rec_if;
   WUFE_clock_rec_system *clock_rec_system;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));
   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   clk_rec_if_id = WUFEI_LineClockRecIfRxGet(line_registry);
   if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (clk_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }
   else /* 336 ext diff mode */
   {
      if (clk_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }

   if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      clock_rec_system = &system_registry->clk_rec->cfg;

      /*disables the mask ram entry*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_ClockRecRxMaskRamDisable(wufe_id, clk_rec_if_id, emphy_port_id));

      /*disables the port ram entry*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_ClockRecRxPortRamDisable(wufe_id, core_id, clk_rec_if_id,
                                                           emphy_port_id,
                                                           clock_rec_system->method));

      /*update FIFO manager in the TSPT*/
      WUFEI_RETURN_IF_ERROR(status,WUFEI_PhyClockRecTSUpdate(wufe_id, core_id,
                                                             emphy_port_id,
                                                             clk_rec_if_id,
                                                             WUFE_RX_DIRECTION,
                                                             WUFE_DISABLE));
   }

   /*total phys counter*/
   clk_rec_if = &system_registry->clk_rec->rx_arr[clk_rec_if_id];
   clk_rec_if->phy_n--;

   if(clk_rec_if->phy_n == 0)
   {
      if (clk_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         /* Delete the timestamp unit*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxTimestampDisable(wufe_id, clk_rec_if_id));

         /* Delete the RX Timestamp port ram */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxPortRamDelete(wufe_id, clk_rec_if_id));
      }
      else
      {
         /* In extended differential mode, need to disable the RX TS Buffer Size register */
         /* Because the register can not be read-modify-write, the disable clear it all */
         WUFEI_HwClockRecRxTsBufferSizeRegSet(wufe_id,
                                              clk_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES,
                                              WUFE_DISABLE);
      }

      clk_rec_if->data_unit_size = 0;

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRegistryDisplay
 *
 * Purpose:   Display clock recovery registry information
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *
 * Return Value:
 *     none
 *
 * Called by:
 *      WUFEI_ClockRecSystemDisplay
 *
 ****************************************************************************/
static void WUFEI_ClockRecRegistryDisplay(WUFEI_system_registry* system_registry)
{
   WUFE_clock_rec_system *clock_rec_system;
#ifdef WUFE_UFE2PLUS
   return;
#endif

   clock_rec_system = &system_registry->clk_rec->cfg;

   if(clock_rec_system == NULL)
   {
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery Configuration Structure not defined\n"));
      return;
   }

   WUFE_PRINT_DISP((wufei_log,
                    "\n------ Clock Recovery User Configuration ------\n"));
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: max_rx_clk_rec_if %d\n",
                    clock_rec_system->max_rx_clk_rec_if));
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: max_tx_clk_rec_if %d\n",
                    clock_rec_system->max_tx_clk_rec_if));
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: max_ext_diff_rx_clk_rec_if %d\n",
                    clock_rec_system->max_ext_diff_rx_clk_rec_if));
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: max_ext_diff_tx_clk_rec_if %d\n",
                    clock_rec_system->max_ext_diff_tx_clk_rec_if));

   if(clock_rec_system->method == WUFE_CLOCK_REC_METHOD_ADAPTIVE)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: method = ADAPTIVE\n"));
   else if(clock_rec_system->method == WUFE_CLOCK_REC_METHOD_DIFFERENTIAL)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: method = DIFFERENTIAL\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: method = NA\n"));

   if(clock_rec_system->sync_mode == WUFE_CLOCK_REC_SYNC_POS)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: sync_mode = SYNC_POS\n"));
   else if(clock_rec_system->sync_mode == WUFE_CLOCK_REC_SYNC_NEG)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: sync_mode = SYNC_NEG\n"));

   if(clock_rec_system->active_mode == WUFE_CLOCK_REC_ACTIVE_HIGH)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: sync_mode = ACTIVE_HIGH\n"));
   else if(clock_rec_system->active_mode == WUFE_CLOCK_REC_ACTIVE_LOW)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: sync_mode = ACTIVE_LOW\n"));

   if(clock_rec_system->drc != NULL)
   {
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: t1_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: e1_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: ds3_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: e3_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: t1_nominal_divisor_int 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_int));
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: e1_nominal_divisor_int 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_int));
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: ds3_nominal_divisor_int 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_int));
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery System: e3_nominal_divisor_int 0x%x\n",
                       clock_rec_system->drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_int));
   }
   if(clock_rec_system->ext_drc != NULL)
   {
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential t1_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential e1_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential ds3_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential e3_nominal_divisor_rem 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_rem));
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential t1_nominal_divisor_int 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_int));
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential e1_nominal_divisor_int 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_int));
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential ds3_nominal_divisor_int 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_int));
      WUFE_PRINT_DISP((wufei_log,
                       "Clock Recovery System: extended differential e3_nominal_divisor_int 0x%x\n",
                       clock_rec_system->ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_int));
   }
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRegistryIfDisplay
 *
 * Purpose:   Display clock recovery registry information for the specific
 *            clock recovery interface
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *
 * Outputs:
 *
 * Return Value:
 *     none
 *
 * Called by:
 *      WUFEI_ClockRecSystemDisplay
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRegistryIfDisplay(WUFEI_system_registry* system_registry,
                                                   WP_U16 phy_id)
{
   WP_U16 line_id;
   WUFEI_phy_registry *phy_registry;
   WUFEI_line_registry *line_registry;
   WUFE_status status;
   WP_U8 clock_rec_mode;
   WP_U8 phy_clk_rec_status;
   WP_U8 phy_rx_clk_rec_mode;
   WUFE_phy *reg_cfg;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));

   phy_clk_rec_status = WUFEI_PhyClockRecStatusGet(phy_registry);
   if(phy_clk_rec_status & WUFEI_PHY_CLK_REC_MASTER_TX)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery PHY: WUFEI_PHY_CLK_REC_MASTER_TX\n"));
   if(phy_clk_rec_status & WUFEI_PHY_CLK_REC_SLAVE_TX)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery PHY: WUFEI_PHY_CLK_REC_SLAVE_TX\n"));
   if(phy_clk_rec_status == WUFEI_PHY_CLK_REC_MASTER_NA)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery PHY: WUFEI_PHY_CLK_REC_MASTER_NA\n"));

   /* get rx_clk_rec_mode */
   reg_cfg = &(phy_registry->cfg);
   phy_rx_clk_rec_mode = reg_cfg->type.trans.rx_clk_rec_mode;
   if (phy_rx_clk_rec_mode & WUFE_ENABLE)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery PHY: RX PHY ENABLE (shaped)\n"));

   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Line: line_id %d, tx_if_id = %d, rx_if_id %d\n",
                    line_id, WUFEI_LineClockRecIfTxGet(line_registry),
                    WUFEI_LineClockRecIfRxGet(line_registry)));

   clock_rec_mode = WUFEI_LineClockRecModeGet(line_registry);
   if(clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery Line: WUFE_LINE_CLOCK_REC_ENABLE_TX\n"));
   else if (clock_rec_mode & ~WUFE_LINE_CLOCK_REC_DISABLE)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery Line: WUFE_LINE_CLOCK_REC_DISABLE_TX\n"));
   if(clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery Line: WUFE_LINE_CLOCK_REC_ENABLE_RX\n"));
   else if (clock_rec_mode & ~WUFE_LINE_CLOCK_REC_DISABLE)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery Line: WUFE_LINE_CLOCK_REC_DISABLE_RX\n"));
   if(clock_rec_mode & WUFE_LINE_CLOCK_REC_DISABLE)
      WUFE_PRINT_DISP((wufei_log,"Clock Recovery Line: WUFE_LINE_CLOCK_REC_DISABLE\n"));

   return WUFE_OK;

}


static WUFE_status WUFEI_ClockRecTsRxPortRamDisplay(WP_U16 clk_rec_if, WP_U32 wufe_id)
{
   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
   WP_U16 slot = 0, i;
   ((void)wufe_id);
   ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);

   WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[0]));
   WUFE_PRINT_DISP((wufei_log,"RX Port Ram\n"));
   for(i=0; i<WUFEI_MAX_N_HW_PORT_RAM_SLOT; i++)
   {
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &slot);
      WUFE_PRINT_DISP((wufei_log,"\tslot[%3d] = 0x%04x",i, slot));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
      if(WUFEI_HW_CLK_REC_RX_PORT_LAST_V(slot))
         break;
   }
   WUFE_PRINT_DISP((wufei_log,"\n"));

   return WUFE_OK;
}

static WUFE_status WUFEI_ClockRecTsRxPortAndMaskRamDisplay(WP_U16 clk_rec_if, WP_U32 wufe_id)
{
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;

   WP_U16 port_ram = 0, mask_ram = 0, i;
   ((void)wufe_id);
   ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);
   ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);

   WUFE_PRINT_DISP((wufei_log,"RX Port & Mask Ram\n"));
   for(i=0; i<WUFEI_MAX_N_HW_MASK_RAM_SLOT; i++)
   {
      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[i]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &port_ram);
      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[i]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &mask_ram);
      WUFE_PRINT_DISP((wufei_log,"\tslot[%3d] = 0x%04x 0x%04x\t",i, port_ram, mask_ram));
      if(i%3 == 2)
         WUFE_PRINT_DISP((wufei_log,"\n"));
      if(WUFEI_HW_CLK_REC_RX_PORT_LAST_V(port_ram))
         break;
   }
   WUFE_PRINT_DISP((wufei_log,"\n"));

   return WUFE_OK;
}

static WUFE_status WUFEI_ClockRecTsRxMaskRamDisplay(WP_U16 clk_rec_if, WP_U32 wufe_id)
{
   WUFEI_hw_clk_rec_ts_mask_ram *ts_rx_mask_ram_ptr;
   WP_U16 slot = 0, i;
   ((void)wufe_id);
   ts_rx_mask_ram_ptr = WUFEI_INDIR_TS_RX_MASK_RAM(clk_rec_if);

   WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_mask_ram_ptr->slot[0]));

   WUFE_PRINT_DISP((wufei_log,"RX Mask Ram\n"));
   for(i=0; i<WUFEI_MAX_N_HW_MASK_RAM_SLOT; i++)
   {
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &slot);
      WUFE_PRINT_DISP((wufei_log,"\tslot[%3d] = 0x%04x",i, slot));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
      if(WUFEI_HW_CLK_REC_RX_PORT_LAST_V(slot))
         break;
   }
   WUFE_PRINT_DISP((wufei_log,"\n"));

   return WUFE_OK;
}

static WUFE_status WUFEI_ClockRecTsUnitRxDisplay(WP_U16 clk_rec_if, WP_U32 wufe_id)
{
   WP_U16 mode = 0, timestamp_msb, timestamp_lsb;
   WP_U32 timestamp = 0;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* Needed only in regular mode. Do nothing in extended diff mode */
   if (clk_rec_if >= WUFE_CLOCK_REC_REGULAR_INSTANCES)
      return WUFE_OK;

   WUFEI_HwClockRecRxTsModeRegGet(wufe_id, clk_rec_if, &mode);
   WUFEI_HwClockRecRxTimestampRegGet(wufe_id, clk_rec_if, &timestamp);
   timestamp_msb =(WP_U16)(timestamp >> 16);
   timestamp_lsb =(WP_U16)timestamp;

   /*Display*/
   /*Mode register*/
   WUFE_PRINT_DISP((wufei_log,"\tmode            0x%04x",mode));
   if(WUFEI_HW_CLK_REC_RX_MODE_EN_V(mode))
      WUFE_PRINT_DISP((wufei_log," [Enabled,"));
   else
      WUFE_PRINT_DISP((wufei_log," [Disabled,"));
   if(WUFEI_HW_CLK_REC_RX_FAST_PORT_V(mode))
      WUFE_PRINT_DISP((wufei_log," fast port,"));
   else
      WUFE_PRINT_DISP((wufei_log," No fast port,"));
   if(WUFEI_HW_CLK_REC_RX_NO_MASK_V(mode))
      WUFE_PRINT_DISP((wufei_log," no Mask RAM]\n"));
   else
      WUFE_PRINT_DISP((wufei_log," Mask RAM]\n"));

   /*Timestamp*/
   WUFE_PRINT_DISP((wufei_log,"\ttimestamp_msb   0x%04x\n", timestamp_msb));
   WUFE_PRINT_DISP((wufei_log,"\ttimestamp_lsb   0x%04x\n", timestamp_lsb));

   return WUFE_OK;
}

static WUFE_status WUFEI_ClockRecTsUnitTxDisplay(WP_U16 clk_rec_if, WP_U32 wufe_id)
{
   WP_U16 mode = 0, buffer_size= 0;
   WP_U16 timestamp_msb, timestamp_lsb;
   WP_U32 timestamp = 0;
   ((void)wufe_id);
   ((void)clk_rec_if);   
   WUFEI_HwClockRecTxTsModeRegGet(wufe_id, clk_rec_if, &mode);
   WUFEI_HwClockRecTxTsBufferSizeRegGet(wufe_id, clk_rec_if, &buffer_size);
   WUFEI_HwClockRecTxTimestampRegGet(wufe_id, clk_rec_if, &timestamp);
   timestamp_msb =(WP_U16)(timestamp >> 16);
   timestamp_lsb =(WP_U16)timestamp;

   WUFE_PRINT_DISP((wufei_log,"\tmode            0x%04x",mode));

   if(WUFEI_HW_CLK_REC_TX_MODE_EN_V(mode))
      WUFE_PRINT_DISP((wufei_log," [Enabled]\n"));
   else
      WUFE_PRINT_DISP((wufei_log," [Disabled]\n"));

   WUFE_PRINT_DISP((wufei_log,"\ttimestamp_msb   0x%04x\n",timestamp_msb));
   WUFE_PRINT_DISP((wufei_log,"\ttimestamp_lsb   0x%04x\n",timestamp_lsb));
   WUFE_PRINT_DISP((wufei_log,"\tbuffer_size     0x%04x", buffer_size));
   WUFE_PRINT_DISP((wufei_log," [0x%04x]\n",
                    WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_V(buffer_size)));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTimestampDisplay
 *
 * Purpose:   Reads the clock recovery timestamp registers in a specified
 *            direction for a given PHY
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *     clk_rec_timestamp - pointer to the registers struct
 *     direction - TX/RX direction
 *
 * Outputs:
 *     TX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      Any  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecTimestampDisplay(WUFEI_system_registry *system_registry,
                                                  WP_U16 phy_id,
                                                  WP_U32 direction)
{
   WP_U16 line_id;
   WP_U32 wufe_id;
   WP_U16 clk_rec_if;
   WUFEI_phy_registry *phy_registry;
   WUFEI_line_registry *line_registry;
   WUFE_status status;
   WP_U32 rx_enb, tx_enb;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (system_registry->clk_rec == NULL)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   /*Get the direction*/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhySetRxTxEnb(direction, &tx_enb, &rx_enb));

   wufe_id = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));

   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   if(tx_enb)
   {
      clk_rec_if = WUFEI_LineClockRecIfTxGet(line_registry);

      if(clk_rec_if == 0xFFFF)
      {
         WUFE_PRINT_DISP((wufei_log,"TX: NO CLOCK RECOVERY DEFINED\n"));
         return WUFE_OK;
      }
      else
      {
         WUFE_PRINT_DISP((wufei_log,"TX: clock recovery interface: #%d \n",
                          clk_rec_if));

      }

      if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
         /*Display TS Unit Registers*/
         WUFEI_RETURN_IF_ERROR(status,WUFEI_ClockRecTsUnitTxDisplay(clk_rec_if, wufe_id));
   }
   else if(rx_enb)
   {
      clk_rec_if = line_registry->rx_clock_rec_if_id;

      if(clk_rec_if == 0xFFFF)
      {
         WUFE_PRINT_DISP((wufei_log,"RX: NO CLOCK RECOVERY DEFINED\n"));
         return WUFE_OK;
      }
      else
      {
         WUFE_PRINT_DISP((wufei_log,"RX: clock recovery interface: #%d \n",
                          clk_rec_if));
      }

      if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         /*Display TS Unit Registers*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecTsUnitRxDisplay(clk_rec_if, wufe_id));

         if(line_registry->framed)
         {

            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_ClockRecTsRxPortAndMaskRamDisplay(clk_rec_if, wufe_id));
         }
         else
         {
            /*Display Port Ram*/
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_ClockRecTsRxPortRamDisplay(clk_rec_if, wufe_id));

            /*Display Mask Ram*/
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_ClockRecTsRxMaskRamDisplay(clk_rec_if, wufe_id));
         }
      }
   }
   else
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }
   return WUFE_OK;


}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxClockGenRamGetFromLine
 *
 * Purpose:   Reads the clock generator RAM entry register for a given line
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     line_id  - specifies the Master line
 *     clk_rec_timestamp - pointer to the registers struct
 *     direction - TX/RX direction
 *
 * Outputs:
 *     TX timestamp registers are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *
 * Called by:
 *      Any  function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxClockGenRamGetFromLine (WUFEI_system_registry *system_registry,
                                                           WP_U16 line_id,
                                                           WP_U16 *ram_entry,
                                                           WP_U32 *entry_id)
{
   WP_U32  wufe_id;
   WUFEI_hw_clk_rec_cgen_rx_ram *cgen_rx_ram_ptr;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif


   if (system_registry->clk_rec == NULL)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   wufe_id = system_registry->wufe_id;

   *entry_id = line_id;

   cgen_rx_ram_ptr = WUFEI_INDIR_CGEN_RX_RAM(line_id);

   WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(cgen_rx_ram_ptr->cgen_rx));
   WUFEI_HwClockRecIdirectDataGet(wufe_id, ram_entry);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxClockGenRamIsValid
 *
 * Purpose:   Checks if the ram  entry is initialized
 *            for the specified Line
 *
 * Description:
 *
 *
 * Inputs:
 *     wufe_id - UFE id
 *     line_id  - specifies the Line
 *
 * Outputs:
 *     the entry valur
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecRxCreate  function.
 *
 ****************************************************************************/
static WP_U32 WUFEI_ClockRecRxClockGenRamIsValid(WP_U16 wufe_id, WP_U16 line_id)
{
   WP_U16 cgen_rx_ram = 0;
   WUFEI_hw_clk_rec_cgen_rx_ram *cgen_rx_ram_ptr;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return WUFE_FALSE;
#endif

   cgen_rx_ram_ptr = WUFEI_INDIR_CGEN_RX_RAM(line_id);
   WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(cgen_rx_ram_ptr->cgen_rx));
   WUFEI_HwClockRecIdirectDataGet(wufe_id, &cgen_rx_ram);

   if(WUFEI_HW_CLK_REC_CGENRX_EN_V(cgen_rx_ram))
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxClockGenRamCreate
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *     enable  - indicates if the clock recovery is enabled for the Line
 *
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecRxCreate  function.
 *
 ****************************************************************************/
static void WUFEI_ClockRecRxClockGenRamCreate (WP_U16 wufe_id,
                                               WP_U16 line_id,
                                               WP_U16 clk_rec_if,
                                               WUFE_line_transf_type line_transfer_type)
{
   WP_U16 cgen_rx_ram = 0;
   WUFEI_hw_clk_rec_cgen_rx_ram *cgen_rx_ram_ptr;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return;
#endif

   /* set Link Rate*/
   if(line_transfer_type == WUFE_SBI_TYPE_DS3 ||
      line_transfer_type == WUFE_SBI_TYPE_E3)
      cgen_rx_ram &= ~WUFEI_HW_CLK_REC_CGENRX_LINK_RATE_M;/* Working with  ClkRate-check */
   else
      cgen_rx_ram |= WUFEI_HW_CLK_REC_CGENRX_LINK_RATE_M;/* Working with  PHASE */
   /* set Clock Generator Id */
   cgen_rx_ram |= (WUFEI_HW_CLK_REC_CGENRX_ID_F(clk_rec_if)|
                   WUFEI_HW_CLK_REC_CGENRX_VALID_M |
                   WUFEI_HW_CLK_REC_CGENRX_EN_M );
   /* write the Rx clock generator register according to the line id */
   cgen_rx_ram_ptr = WUFEI_INDIR_CGEN_RX_RAM(line_id);
   WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(cgen_rx_ram_ptr->cgen_rx));
   WUFEI_HwClockRecIdirectDataSet(wufe_id, cgen_rx_ram);
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxClockGenRamDelete
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *     enable  - indicates if the clock recovery is enabled for the Line
 *
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      WUFEI_ClockRecRxDelete  function.
 *
 ****************************************************************************/
static void WUFEI_ClockRecRxClockGenRamDelete(WP_U32 wufe_id, WP_U16 line_id)
{
   WP_U16 cgen_rx_ram = 0;
   WUFEI_hw_clk_rec_cgen_rx_ram *cgen_rx_ram_ptr;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return;
#endif

   /*FREE the clock generator*/
   cgen_rx_ram |= WUFEI_HW_CLK_REC_CGENRX_ID_F(0);
   cgen_rx_ram &= ~(WUFEI_HW_CLK_REC_CGENRX_VALID_M |
                    WUFEI_HW_CLK_REC_CGENRX_EN_M);
   /* write the Rx clock generator register according to the line id */
   cgen_rx_ram_ptr = WUFEI_INDIR_CGEN_RX_RAM(line_id);
   WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(cgen_rx_ram_ptr->cgen_rx));
   WUFEI_HwClockRecIdirectDataSet(wufe_id, cgen_rx_ram);
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxClockGenRamDisplay
 *
 * Purpose:
 *
 * Description:
 *
 *
 * Inputs:
 *
 *
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      any  function.
 *
 ****************************************************************************/
void WUFE_ClockRecRxClockGenRamDisplay(WP_U32 wufe_id)
{
   WP_U16 i,cgen_rx_ram = 0;
   WUFEI_hw_clk_rec_cgen_rx_ram *cgen_rx_ram_ptr;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return;
#endif

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      /* write the Rx clock generator register according to the line id */
      cgen_rx_ram_ptr = WUFEI_INDIR_CGEN_RX_RAM(0);
      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(cgen_rx_ram_ptr->cgen_rx));
      for(i = 0; i < WUFEI_MAX_N_SBI_LINE; i++)
      {
         WUFEI_HwClockRecIdirectDataGet(wufe_id, &cgen_rx_ram);
         WUFE_PRINT_DISP((wufei_log,"line: %02d 0x%x\n",i,cgen_rx_ram));
      }
   }
}

/*****************************************************************************
 *
 * Function:
 *
 * Purpose:
 *
 * Description:
 *
 * Inputs:
 *
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      any  function.
 *
 ****************************************************************************/
WP_U16 WUFE_ClockRecCgenRamEntryRead(WP_U32 wufe_id, WP_U16 entry)
{
   WP_U16 cgen_rx_ram = 0;
   WUFEI_hw_clk_rec_cgen_rx_ram *cgen_rx_ram_ptr;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return 0;
#endif

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      /* read the Rx clock generator register according to the line id */
      cgen_rx_ram_ptr = WUFEI_INDIR_CGEN_RX_RAM(entry);
      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(cgen_rx_ram_ptr->cgen_rx));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &cgen_rx_ram);
      WUFE_PRINT_DISP((wufei_log,"entry: %d   cgen data: 0x%x\n",entry,cgen_rx_ram));
      return cgen_rx_ram;
   }
   else
      return WUFE_CLOCK_REC_INVALID_VALUE;
}

/*****************************************************************************
 *
 * Function:
 *
 * Purpose:
 *
 * Description:
 *
 * Inputs:
 *
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      any  function.
 *
 ****************************************************************************/
WP_U16 WUFE_ClockRecRxPortRamEntryRead(WP_U32 wufe_id, WP_U16 clk_rec_if, WP_U16 entry)
{
   WUFEI_hw_clk_rec_ts_port_ram *ts_rx_port_ram_ptr;
   WP_U16 slot = 0;
   ((void)wufe_id);
#ifdef WUFE_UFE2PLUS
   return 0;
#endif

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      ts_rx_port_ram_ptr = WUFEI_INDIR_TS_RX_PORT_RAM(clk_rec_if);

      WUFEI_HwClockRecIdirectAddrSet(wufe_id, &(ts_rx_port_ram_ptr->slot[entry]));
      WUFEI_HwClockRecIdirectDataGet(wufe_id, &slot);
      WUFE_PRINT_DISP((wufei_log,"slot[%3d] = 0x%04x\n",entry, slot));

      return slot;
   }
   else
      return WUFE_CLOCK_REC_INVALID_VALUE;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecPhyEnable
 *
 * Purpose:
 *     Ensure that the PHY can be enabled
 * Description:
 *     Validates the PhyEnable procedure for clock recovery mechanism
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id - the Phy to enable
 *     direction  - indicates the Phy direction RX/TX
 *
 * Outputs:
 *     Error is returned if the enable procedure is inconsistent for the
 *     clock recovery, the clock recovery entries are updated accordingly
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_CLOCK_REC_NO_MASTER_PHY - trying to enable slave Phy
 *                                          before a master Phy is enabled
 *           WUFE_ERR_CLOCK_REC_NOT_ENABLED - trying to enable clock recovery
 *                                          for none clock recovery phy
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid line is assigned to the Phy
 *
 * Called by:
 *      WUFEI_PhyEnable  function.
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecPhyEnable(WUFEI_system_registry *system_registry,
                                    WP_U32 core_id,
                                    WP_U16 emphy_port_id,
                                    WP_U16 phy_id,
                                    WP_U32 direction)
{
   WP_U32 rx_enb, tx_enb;
   WP_U16 line_id, clk_rec_if;
   WUFE_status status;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WP_U8 phy_clk_rec_status;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhySetRxTxEnb(direction, &tx_enb, &rx_enb));

   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyRegistryGet(system_registry,
                                                      phy_id,
                                                      &phy_registry));

   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineRegistryGet(system_registry,
                                                       line_id,
                                                       &line_registry));

   phy_clk_rec_status = WUFEI_PhyClockRecStatusGet(phy_registry);

   if(rx_enb)
   {
      WUFEI_clk_rec_rx *clk_rec_rx_registry;

      clk_rec_if = WUFEI_LineClockRecIfRxGet(line_registry);

      if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         if (clk_rec_if >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
      }
      else /* 336 ext diff mode */
      {
         if (clk_rec_if - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
             system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
      }

      clk_rec_rx_registry  = &system_registry->clk_rec->rx_arr[clk_rec_if];

      if (phy_clk_rec_status & WUFEI_PHY_CLK_REC_SLAVE_RX)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxEnable(system_registry,
                                                      core_id,
                                                      emphy_port_id));
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log,"ERROR: invalid clock recovery direction \n"));
         return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);
      }
   }

   if(tx_enb)
   {
      WUFEI_clk_rec_tx *clk_rec_tx_registry;

      clk_rec_if = WUFEI_LineClockRecIfTxGet(line_registry);

      if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         if (clk_rec_if >= system_registry->clk_rec->cfg.max_tx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
      }
      else /* extended diff mode */
      {
         if (clk_rec_if - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
             system_registry->clk_rec->cfg.max_ext_diff_tx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
      }

      clk_rec_tx_registry = &system_registry->clk_rec->tx_arr[clk_rec_if];

      if(phy_clk_rec_status & WUFEI_PHY_CLK_REC_MASTER_TX)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecTxEnable(system_registry,
                                                      core_id,
                                                      emphy_port_id));
         clk_rec_tx_registry->state |= WUFEI_CLK_REC_USED;
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecPhyDisable
 *
 * Purpose:
 *     Ensure that the PHY can be disabled
 * Description:
 *     Validates the PhyDisable procedure for clock recovery mechanism
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id - the Phy to enable
 *     direction  - indicates the Phy direction RX/TX
 *
 * Outputs:
 *     Error is returned if the disable procedure is incosistent for the
 *     clock recovery, the clock recovery entries are updated accordingly
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_CLOCK_REC_NO_MASTER_PHY - trying to enable slave Phy
 *                                          before a master Phy is enabled
 *           WUFE_ERR_CLOCK_REC_NOT_ENABLED - trying to enable clock recovery
 *                                          for none clock recovery phy
 *
 * Called by:
 *      WUFE_PhyDisable function.
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecPhyDisable(WUFEI_system_registry *system_registry,
                                     WP_U32 core_id,
                                     WP_U16 emphy_port_id,
                                     WP_U16 phy_id,
                                     WP_U32 direction)
{

   WP_U32 rx_enb, tx_enb;
   WP_U16 line_id,clk_rec_if;
   WUFE_status status;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WP_U8 phy_clk_rec_status;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhySetRxTxEnb(direction, &tx_enb, &rx_enb));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));

   line_id = WUFEI_PhyLineIdGet(phy_registry);
   phy_clk_rec_status = WUFEI_PhyClockRecStatusGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));
   if(rx_enb)
   {
      WUFEI_clk_rec_rx *clk_rec_rx_registry;

      clk_rec_if = WUFEI_LineClockRecIfRxGet(line_registry);

      if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         if (clk_rec_if >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
      }
      else /* 336 ext diff mode */
      {
         if (clk_rec_if - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
             system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
      }

      clk_rec_rx_registry  = &system_registry->clk_rec->rx_arr[clk_rec_if];

      if (phy_clk_rec_status & WUFEI_PHY_CLK_REC_SLAVE_RX)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecRxDisable(system_registry,
                                                       core_id,
                                                       emphy_port_id,
                                                       phy_id));
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log,"ERROR: invalid clock recovery direction\n"));
         return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);
      }
   }

   if(tx_enb)
   {
      WUFEI_clk_rec_tx *clk_rec_tx_registry;

      clk_rec_if = WUFEI_LineClockRecIfTxGet(line_registry);

      if (clk_rec_if < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         if (clk_rec_if >= system_registry->clk_rec->cfg.max_tx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
      }
      else /* ext diff mode */
      {
         if (clk_rec_if - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
             system_registry->clk_rec->cfg.max_ext_diff_tx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
      }

      clk_rec_tx_registry  = &system_registry->clk_rec->tx_arr[clk_rec_if];

      if (phy_clk_rec_status & WUFEI_PHY_CLK_REC_MASTER_TX)
      {

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClockRecTxDisable(system_registry,
                                                       core_id,
                                                       emphy_port_id,
                                                       phy_id));
         clk_rec_tx_registry->state &= ~WUFEI_CLK_REC_USED;

      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecPhyDelete
 *
 * Purpose:
 *     Ensure that the PHY can be deleted
 * Description:
 *     Keeps the clock recovery data structure consistent if a phy is
 *     deleted
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_registry - the Phy to enable
 *     direction  - indicates the Phy direction RX/TX
 *
 * Outputs:
 *     the clock recovery entry is updated
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      WUFE_PhyDelete function.
 *
 ****************************************************************************/
WUFE_status  WUFEI_ClockRecPhyDelete(WUFEI_system_registry* system_registry,
                                     WP_U16 phy_id)
{
   WUFE_status status;
   WP_U16 line_id, clk_rec_if;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WP_U8 phy_clk_rec_status;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhyRegistryGet(system_registry,
                                              phy_id,
                                              &phy_registry));

   line_id = WUFEI_PhyLineIdGet(phy_registry);
   phy_clk_rec_status = WUFEI_PhyClockRecStatusGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   if(phy_clk_rec_status & WUFEI_PHY_CLK_REC_MASTER_TX)
   {
      WUFEI_clk_rec_tx *clk_rec_tx_registry;
      clk_rec_if = WUFEI_LineClockRecIfTxGet(line_registry);
      clk_rec_tx_registry = &system_registry->clk_rec->tx_arr[clk_rec_if];
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_ClockRecTxDelete(system_registry,
                                                   phy_id));
      clk_rec_tx_registry->state &= ~WUFEI_CLK_REC_ACTIVE;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTxInterfaceIdValidate
 *
 * Purpose:
 *
 * Description:
 *     Validates the Tx clock recovery and translator id
 *
 * Inputs:
 *     system_registry - UFE system registry pointer
 *     translator_id  - the translator id
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      WUFEI_SbiLineConfig, WUFEI_CadConfig functions

****************************************************************************/
WUFE_status WUFEI_ClockRecTxInterfaceIdValidate(WUFEI_system_registry *system_registry,
                                                WUFE_line_transf_type transf_type,
                                                WP_U16 id)
{
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   if (id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if ((system_registry->clk_rec->cfg.max_tx_clk_rec_if == 0)||
          (system_registry->clk_rec->cfg.max_tx_clk_rec_if <= id))
         return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
   }
   else /* 336 ext diff */
   {
      if ((system_registry->clk_rec->cfg.max_ext_diff_tx_clk_rec_if == 0)||
          (system_registry->clk_rec->cfg.max_ext_diff_tx_clk_rec_if <=
           id - WUFE_CLOCK_REC_REGULAR_INSTANCES))
         return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);
   }

   if((transf_type == WUFE_SBI_TYPE_E3 || transf_type == WUFE_SBI_TYPE_DS3) &&
      id >= (WUFE_MAX_N_SPE*WUFE_MAX_N_STM1*WUFE_MAX_N_CORE))
      return (WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTxExtendedModeValidate
 *
 * Purpose:
 *
 * Description:
 *     Validates that if regular mode is selected then only line_ids under 32 is set
 *
 * Inputs:
 *     system_registry - UFE system registry pointer
 *     extended_active - if set to '1' then extended differential mode is selected
 *     line_id         - the ID of the selected line.
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *       WUFE_ERR_CLOCK_REC_INVALID_LINE_IN_EXTENDED_MODE
 *
 * Called by:
 *      WUFEI_LineCadClockRecConfig

****************************************************************************/
WUFE_status WUFEI_CadClockRecTxExtendedModeValidate(WUFEI_system_registry *system_registry,
                                                    WUFE_cad_cr_extended extended_active,
                                                    WP_U16 line_id)
{
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   if (!extended_active)
   {
      if (line_id >= WUFE_CLOCK_REC_REGULAR_INSTANCES)
         return (WUFE_ERR_CLOCK_REC_INVALID_LINE_IN_EXTENDED_MODE);
   }

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxInterfaceCreate
 *
 * Purpose:
 *
 * Description:
 *     Allocates the RX clock recovery id per line
 *
 * Inputs:
 *     system_registry - UFE system registry pointer
 *     line_registry - UFE line registry
 *
 * Outputs:
 *     the clock recovery entry is updated
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      WUFEI_SbiLineConfig function.
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecRxInterfaceCreate(WUFEI_system_registry *system_registry,
                                            WUFEI_line_registry *line_registry)
{
   WUFEI_clk_rec_rx *clock_rec_if;
   WUFE_status status;
   WP_U16 clock_rec_if_id, line_id;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   line_id = line_registry->line_id;
   clock_rec_if_id = line_registry->rx_clock_rec_if_id;

   /*check the clock recovery interface id range*/
   if (clock_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (system_registry->clk_rec->cfg.max_rx_clk_rec_if == 0 ||
          clock_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }
   else /* 336 diff mode */
   {
      if (system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if == 0 ||
          clock_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }


   clock_rec_if = &system_registry->clk_rec->rx_arr[clock_rec_if_id];
   clock_rec_if->line_id_arr[line_id] = 0;

   if(clock_rec_if->line_n == 0)
   {
      WUFEI_RETURN_IF_ERROR(status,WUFEI_ClockRecRxCreate(system_registry,
                                                          line_registry,
                                                          line_id));
      clock_rec_if->enable = WUFE_FALSE;
      clock_rec_if->line_id_arr[line_id] |= WUFEI_CLK_REC_OCCUPIED;
   }
   else
   {
      /*lines that belong to the same clock domain must be from the same transfer type*/
      if(clock_rec_if->transfer_type != line_registry->transfer_type)
         return WUFE_ERR_CLOCK_REC_INVALID_TRANSFER_TYPE;

      /*lines that belong to the same clock domain must be from the same framing mode*/
      if(clock_rec_if->framed != line_registry->framed)
         return WUFE_ERR_CLOCK_REC_INVALID_FRAMING_MODE;
   }

   clock_rec_if->line_n++;
   clock_rec_if->line_id_arr[line_id] |= WUFEI_CLK_REC_VALID;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxInterfaceEnable
 *
 * Purpose:
 *
 * Description:
 *     Allocates the RX clock recovery id per line
 *
 * Inputs:
 *     system_registry - UFE system registry pointer
 *     clock_rec_if_id_rx - free RX clock recovery value
 *
 * Outputs:
 *     the clock recovery entry is updated
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      WUFEI_LineEnable function.
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecRxInterfaceEnable(WUFEI_system_registry *system_registry,
                                            WP_U16 clock_rec_if_id,
                                            WP_U16 line_id)
{
   WUFEI_clk_rec_rx *clock_rec_if;
   WUFE_status status;
   WP_U32 wufe_id;

#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   /*check the clock recovery interface id range*/
   if (clock_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (system_registry->clk_rec->cfg.max_rx_clk_rec_if == 0 ||
          clock_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }
   else
   {
      if (system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if == 0 ||
          clock_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }

   wufe_id = system_registry->wufe_id;

   clock_rec_if = &system_registry->clk_rec->rx_arr[clock_rec_if_id];

   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      if((clock_rec_if->enable == WUFE_FALSE) &&
         (WUFEI_ClockRecRxClockGenRamIsValid(wufe_id, line_id) == WUFE_TRUE))
         clock_rec_if->enable = WUFE_TRUE;
   }
   else
   {
      clock_rec_if->enable = WUFE_TRUE;
   }

   if(clock_rec_if->enable == WUFE_FALSE)
   {
      WP_U16 old_line_id;

      /*find the old line in the same clock domain use for the clock generator*/
      for(old_line_id=0; old_line_id<system_registry->cfg.max_line_number; old_line_id++)
      {
         /*find a line connected to the clock interface*/
         if(clock_rec_if->line_id_arr[old_line_id] & WUFEI_CLK_REC_OCCUPIED)
         {
            if(old_line_id == line_id)
               continue;

            /*update the clock generator line*/
            WUFEI_RETURN_IF_ERROR(status, WUFEI_ClockRecRxInterfaceUpdate(system_registry,
                                                                          clock_rec_if_id,
                                                                          line_id,
                                                                          old_line_id));
            clock_rec_if->line_id_arr[old_line_id] &= ~WUFEI_CLK_REC_OCCUPIED;
            clock_rec_if->line_id_arr[line_id] |= WUFEI_CLK_REC_OCCUPIED;

            break;
         }
      }
      clock_rec_if->enable = WUFE_TRUE;
   }

   clock_rec_if->line_id_arr[line_id] |= WUFEI_CLK_REC_USED;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxInterfaceDisable
 *
 * Purpose:
 *
 * Description:
 *     Allocates the RX clock recovery id per line
 *
 * Inputs:
 *     system_registry - UFE system registry pointer
 *     clock_rec_if_id_rx - free RX clock recovery value
 *
 * Outputs:
 *     the clock recovery entry is updated
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      WUFEI_LineEnable function.
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecRxInterfaceDisable(WUFEI_system_registry *system_registry,
                                             WP_U16 clock_rec_if_id,
                                             WP_U16 line_id)
{
   WUFEI_clk_rec_rx *clock_rec_if;
   WUFE_status status;
   WP_U32 wufe_id;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   /*check the clock recovery interface id range*/
   if (clock_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (system_registry->clk_rec->cfg.max_rx_clk_rec_if == 0 ||
          clock_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }
   else
   {
      if (system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if == 0 ||
          clock_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }

   wufe_id = system_registry->wufe_id;

   clock_rec_if = &system_registry->clk_rec->rx_arr[clock_rec_if_id];

   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      if((clock_rec_if->enable == WUFE_TRUE) &&
         (WUFEI_ClockRecRxClockGenRamIsValid(wufe_id, line_id) == WUFE_TRUE))
      {
         WP_U32 new_line_id;

         /*find a new line in the same clock domain to use for the clock generator*/
         for(new_line_id=0; new_line_id<system_registry->cfg.max_line_number; new_line_id++)
         {
            /*find a line connected to the clock interface*/
            if(clock_rec_if->line_id_arr[new_line_id] & WUFEI_CLK_REC_ACTIVE)
            {
               if(new_line_id == line_id)
                  continue;

               /*update the clock generator line*/
               WUFEI_RETURN_IF_ERROR(status, WUFEI_ClockRecRxInterfaceUpdate(system_registry,
                                                                             clock_rec_if_id,
                                                                             new_line_id,
                                                                             line_id));
               clock_rec_if->line_id_arr[line_id] &= ~WUFEI_CLK_REC_OCCUPIED;
               clock_rec_if->line_id_arr[new_line_id] |= WUFEI_CLK_REC_OCCUPIED;

               break;

            }
         }

         /*not found any enabled line*/
         if(clock_rec_if->line_id_arr[line_id] & WUFEI_CLK_REC_OCCUPIED)
            clock_rec_if->enable = WUFE_FALSE;

      }
   }
   else
   {
      /* CAD mode */
      if(clock_rec_if->line_id_arr[line_id] & WUFEI_CLK_REC_OCCUPIED)
         clock_rec_if->enable = WUFE_FALSE;
   }

   clock_rec_if->line_id_arr[line_id] &= ~WUFEI_CLK_REC_USED;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxInterfaceDelete
 *
 * Purpose:
 *
 * Description:
 *     Frees clock recovery id
 *
 * Inputs:
 *     system_registry - pointer to the UFE system registry
 *     clock_rec_if_id - free RX clock recovery value
 *     line_id - line_id to free
 *
 * Outputs:
 *     the clock recovery entry is updated
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      WUFEI_SbiLineConfig function.
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecRxInterfaceDelete(WUFEI_system_registry *system_registry,
                                            WP_U16 clock_rec_if_id,
                                            WP_U16 line_id)
{
   WUFEI_clk_rec_rx *clock_rec_if;
   WUFE_status status;
   WP_U32 wufe_id;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   if (clock_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (clock_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }
   else /* 336 ext diff mode */
   {
      if (clock_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }

   wufe_id = system_registry->wufe_id;

   clock_rec_if = &system_registry->clk_rec->rx_arr[clock_rec_if_id];
   clock_rec_if->line_n--;
   clock_rec_if->line_id_arr[line_id] &= ~WUFEI_CLK_REC_VALID;

   if(clock_rec_if->line_n == 0)
   {
      WUFEI_RETURN_IF_ERROR(status,WUFEI_ClockRecRxDelete(system_registry, line_id));
      clock_rec_if->line_id_arr[line_id] &= ~WUFEI_CLK_REC_OCCUPIED;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxInterfaceUpdate
 *
 * Purpose:
 *
 * Description:
 *     Frees clock recovery id
 *
 * Inputs:
 *     system_registry - pointer to the UFE system registry
 *     clock_rec_if_id - RX clock recovery value
 *     new_line_id - line id to use in clock generator ram
 *
 * Outputs:
 *     the clock recovery entry is updated
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      WUFEI_ClockRecRxInterfaceRelease function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecRxInterfaceUpdate(WUFEI_system_registry *system_registry,
                                                   WP_U16 clock_rec_if_id,
                                                   WP_U16 new_line_id,
                                                   WP_U16 old_line_id)
{
   WUFEI_line_registry* line_registry;
   WUFE_line_transf_type line_transfer_type;
   WUFE_status status;
   WP_U32 wufe_id;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   if (clock_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (clock_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }
   else
   {
      if (clock_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }

   wufe_id = system_registry->wufe_id;


   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineRegistryGet(system_registry,
                                                       new_line_id,
                                                       &line_registry));

   line_transfer_type = WUFEI_LineTransferTypeGet(line_registry);

   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      /*Delete the clock generator old line entry*/
      WUFEI_ClockRecRxClockGenRamDelete(wufe_id, old_line_id);

      /*Create the clock generator new line entry*/
      WUFEI_ClockRecRxClockGenRamCreate(wufe_id,
                                        new_line_id,
                                        clock_rec_if_id,
                                        line_transfer_type);
   }


   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecRxCgenUpdate
 *
 * Purpose:
 *
 * Description:
 *     move RX clock generator from current line to new line
 *
 * Inputs:
 *     system_registry - UFE system registry pointer
 *     clock_rec_if_id - clock recovery id number
 *     current_line_id - current master CGEN line
 *     new_line_id     - this line will become the new master CGEN line
 *
 * Outputs:
 *     Master CGEN is updated
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *      WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID
 *      WUFE_ERR_CLOCK_REC_INVALID_CONFIG
 *
 * Called by:
 *      WUFE_ClockRecRxCgenModify
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecRxCgenUpdate(WUFEI_system_registry *system_registry,
                                       WP_U16 clock_rec_if_id,
                                       WP_U16 current_line_id,
                                       WP_U16 new_line_id)
{
   WUFEI_clk_rec_rx *clock_rec_if;
   WUFE_status status;
   WP_U32 wufe_id;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   /*check the clock recovery interface id range*/
   if (clock_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      if (system_registry->clk_rec->cfg.max_rx_clk_rec_if == 0 ||
          clock_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }
   else
   {
      if (system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if == 0 ||
          clock_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
          system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
         return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
   }

   wufe_id = system_registry->wufe_id;
   clock_rec_if = &system_registry->clk_rec->rx_arr[clock_rec_if_id];

   /* verify CR enable and valid CGEN RAM */
   if((clock_rec_if->enable != WUFE_TRUE) ||
      (WUFEI_ClockRecRxClockGenRamIsValid(wufe_id, current_line_id) != WUFE_TRUE))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFE:ERROR clock recovery isn't enabled\n"));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_CLOCK_REC_INVALID_CONFIG;
   }

   /* update CGEN in new line */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_ClockRecRxInterfaceUpdate(system_registry,
                                                                 clock_rec_if_id,
                                                                 new_line_id,
                                                                 current_line_id));
   clock_rec_if->line_id_arr[current_line_id] &= ~WUFEI_CLK_REC_OCCUPIED;
   clock_rec_if->line_id_arr[new_line_id] |= WUFEI_CLK_REC_OCCUPIED;

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFE_ClockRecRxCgenModify
 *
 * Purpose:   Change master CGEN line.
 *
 *
 * Description:
 *     This function changes the master CGEN on a clock domain by remove old master CGEN
 *     from current line and move it to new line.
 *     New line is given by the user, the current master line is found by the function.
 *
 *
 * Inputs:
 *     new_ufe_line_handel The new UFE Line that should be the CGEN master.
 *
 * Outputs:
 *     None. Will change the current master CGEN on spesific CD.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_CLOCK_REC_LINE_PARAMS
 *     WUFE_ERR_CLOCK_REC_CGEN_LINE_NOT_FOUND
 *     WUFE_ERR_CLOCK_REC_INVALID_CONFIG
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_ClockRecRxCgenModify(WUFE_handle new_ufe_line_handle)
{
   WUFE_status status;
   WP_U32 wufe_id, current_line_id, new_line_id;
   WUFEI_line_registry   *current_line_registry, *new_line_registry;
   WUFEI_system_registry *system_registry;

   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE3)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Invalid FPGA type. WUFE_ClockRecRxCgenModify valid for UFE3 only.\n"));
      return WUFE_ERR_LINE_INVALID_FPGA;
   }

   wufe_id=WUFEI_HANDLE_UFE_ID_V(new_ufe_line_handle);
   new_line_id=WUFEI_HANDLE_INDEX_V(new_ufe_line_handle);

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,
                                  &system_registry,status);

   if (new_line_id >= system_registry->cfg.max_line_number)
   {/* illegal line number */
      WUFE_PRINT_ERR(
         (wufei_log,
          "ERROR: cfg->cad_id(%d) >= sys_reg->cfg.max_line_number( %d )\n",
          new_line_id,
          system_registry->cfg.max_line_number));
      return WUFE_ERR_LINE_ILLEGAL_ID ;
   }

   new_line_registry = &system_registry->line_arr[new_line_id];

   /* verify that RX direction are active (not have to be in differential mode) */
   if(!(new_line_registry->clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_RX))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFE:ERROR line is not Clock Recovery enable in RX direction\n"));

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_CLOCK_REC_LINE_PARAMS;
   }

   /* find the current master CGEN line for new_line CD */
   {
      WP_U16 clock_rec_if_id;
      WUFEI_clk_rec_rx * clock_rec_if;

      clock_rec_if_id = new_line_registry->rx_clock_rec_if_id;

      /* check the clock recovery interface id range */
      if (clock_rec_if_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         if (system_registry->clk_rec->cfg.max_rx_clk_rec_if == 0 ||
             clock_rec_if_id >= system_registry->clk_rec->cfg.max_rx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
      }
      else
      {
         if (system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if == 0 ||
             clock_rec_if_id - WUFE_CLOCK_REC_REGULAR_INSTANCES >=
             system_registry->clk_rec->cfg.max_ext_diff_rx_clk_rec_if)
            return (WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID);
      }

      /* CR_if number that new_line is connected to */
      clock_rec_if = &system_registry->clk_rec->rx_arr[clock_rec_if_id];

      /* find current master */
      for (current_line_id = 0;
           current_line_id < system_registry->cfg.max_line_number;
           current_line_id++)
      {
         if (clock_rec_if->line_id_arr[current_line_id] & WUFEI_CLK_REC_ACTIVE)
         {
            /* check if the line is the master */
            if (WUFEI_ClockRecRxClockGenRamIsValid(wufe_id, current_line_id) == WUFE_TRUE)
            {
               /* found the master */
               break;
            }
         }
      }
      /* Master was not found */
      if (current_line_id > system_registry->cfg.max_line_number)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "WUFE:ERROR no CGEN master line found\n"));
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_CLOCK_REC_CGEN_LINE_NOT_FOUND;
      }
   }

   current_line_registry = &system_registry->line_arr[current_line_id];

   /* verify that lines have the same clock domain */
   if (current_line_registry->rx_clock_rec_if_id != new_line_registry->rx_clock_rec_if_id)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFE:ERROR lines must be in the same clock domain\n"));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_ERR_CLOCK_REC_INVALID_CONFIG;
   }

   /* move RX clock generator from current line to new line */
   WUFEI_IF_ERROR_RETURN_DETACH_W(
      status,
      WUFEI_ClockRecRxCgenUpdate(system_registry,
                                 current_line_registry->rx_clock_rec_if_id,
                                 current_line_registry->line_id,
                                 new_line_registry->line_id),
      wufe_id);


   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecTxMasterSet
 *
 * Purpose:  sign a specified PHY the clock is retrieved from (Master PHY)
 *
 *
 * Description:
 *     This function adds clock recovery master to the system
 *
 * Inputs: ufe_phy_handle - UFE phy handle
 *         direction - the direction of the clock recovery TX/RX
 *         tx_data_unit_size - used in trans. phys only for clock
 *                             recovery master
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *     user function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_ClockRecTxMasterSet(WUFEI_system_registry *system_registry,
                                             WUFEI_line_registry *line_registry,
                                             WUFEI_phy_registry *phy_registry,
                                             WP_U8 tx_data_unit_size)
{
   WUFE_status status, ts_status;
   WP_U16 clk_rec_if;
   WUFEI_clk_rec_tx  *clk_rec_tx_registry;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   clk_rec_if = WUFEI_LineClockRecIfTxGet(line_registry);

   clk_rec_tx_registry = &system_registry->clk_rec->tx_arr[clk_rec_if];

   if(clk_rec_tx_registry->state & WUFEI_CLK_REC_VALID)
   {
      WP_U8 master_new, master_old;
      WUFEI_phy_registry *master_old_registry;
      WP_U16 tx_master_phy;

      tx_master_phy = clk_rec_tx_registry->phy_id;
      WUFEI_RETURN_IF_ERROR(status, WUFEI_PhyRegistryGet(system_registry,
                                                         tx_master_phy,
                                                         &master_old_registry));

      master_new = WUFEI_PhyTxActiveGet(phy_registry);
      master_old = WUFEI_PhyTxActiveGet(master_old_registry);
      if (master_old == WUFEI_PHY_ENABLE || master_new == WUFEI_PHY_ENABLE)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "ERROR WUFEI_ClockRecTxMasterSet() for active PHYS\n "));
         return WUFE_ERR_PHY_DISABLED;
      }

      WUFEI_PhyClockRecStatusSetTxSlave(master_old_registry);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_ClockRecTxDelete(system_registry,tx_master_phy));
   }
   clk_rec_tx_registry->phy_id = WUFEI_PhyIdGet(phy_registry);
   clk_rec_tx_registry->state |= WUFEI_CLK_REC_VALID;
   clk_rec_tx_registry->state &= ~WUFEI_CLK_REC_USED;
   clk_rec_tx_registry->tx_data_unit_size = tx_data_unit_size;

   WUFEI_PhyClockRecStatusSetTxMaster(phy_registry);

   /* may return a warning WUFE_ERR_CLOCK_REC_INVALID_TS_WIDTH */
   ts_status = WUFEI_ClockRecTxCreate(system_registry,
                                      clk_rec_tx_registry->phy_id);

   return ts_status;
}

/*****************************************************************************
 *
 * Function:  WUFE_ClockRecMasterSet
 *
 * Purpose:  sign a specified PHY the clock is retrieved from (Master PHY)
 *
 *
 * Description:
 *     This function adds clock recovery master to the system
 *
 * Inputs: ufe_phy_handle - UFE phy handle
 *         tx_data_unit_size - used in trans. phys only for clock
 *                             recovery master
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *    any user function.
 *
 ****************************************************************************/
WUFE_status WUFE_ClockRecMasterSet(WUFE_handle ufe_phy_handle,
                                   WP_U8 tx_data_unit_size)
{
   WUFE_status status;
   WP_U16 wufe_id, phy_id, line_id, core_id, emphy_port_id;
   WUFEI_system_registry *system_registry;
   WUFEI_phy_registry    *phy_registry;
   WUFEI_line_registry   *line_registry;
   WP_U8 clock_rec_mode;

   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE3)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Invalid FPGA type. WUFE_ClockRecMasterSet valid for UFE3 only.\n"));
      return WUFE_ERR_LINE_INVALID_FPGA;
   }

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

   if (!system_registry->clk_rec)
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, (WUFE_ERR_CLOCK_REC_NOT_ENABLED), wufe_id);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_PhyRegistryGet(system_registry,
                                                       phy_id,
                                                       &phy_registry),
                                  wufe_id);

   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_LineRegistryGet(system_registry,
                                                        line_id,
                                                        &line_registry),
                                  wufe_id);

   clock_rec_mode = WUFEI_LineClockRecModeGet(line_registry);

   /* may return a warning WUFE_ERR_CLOCK_REC_INVALID_TS_WIDTH */
   if(clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX)
      status =  WUFEI_ClockRecTxMasterSet(system_registry,
                                          line_registry,
                                          phy_registry,
                                          tx_data_unit_size);

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecConfigDivider
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 *
 * Inputs:
 *     ufe_id      - UFE id
 *     system_registry  -  UFE system registry
 *     cr_id   - clock recovery interface id
 *     line_transfer_type - line type T1, E1, etc.
 *     enable  - indicates if the clock recovery is enabled for the Line
 *
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_CLOCK_REC_NOT_ENABLED - clock recovery not found
 *
 * Called by:
 *      WUFEI_ClockRecLineConfig  function.
 *
 ****************************************************************************/
WUFE_status WUFEI_ClockRecConfigDivider(WP_U32 wufe_id,
                                        WUFEI_system_registry *system_registry,
                                        WP_U32 cr_id,
                                        WP_U32 line_transfer_type,
                                        WP_U8 enable)
{
   WUFE_status status;
   WUFEI_clk_rec_tx *clock_rec_if;
   WP_U16 e1t1_mode;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   /* check existence of clock recovery module */
   if (system_registry->cfg.system_clock_rec_enable != WUFE_SYSTEM_CLOCK_REC_ENABLE)
   {
      WUFEI_RETURN_IF_ERROR(status,WUFE_ERR_INVALID_CONFIG);
   }
   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   clock_rec_if = &system_registry->clk_rec->tx_arr[cr_id];

   if (enable)
   {
      WUFE_clk_rec_drc_divisor_list *drc;
      WUFE_clk_rec_drc_divisor_list *ext_drc;

      /*The clock devisor should be configured only once per clock domain*/
      if(clock_rec_if->n_active_lines)
      {
         clock_rec_if->n_active_lines ++;
         return WUFE_OK;
      }

      drc = &system_registry->clk_rec->drc;
      ext_drc = &system_registry->clk_rec->ext_drc;
      /* clear clock recovery registers */
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwClockRecDcoInit(wufe_id, cr_id));

      if (cr_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      { /* regular differential mode or adaptive mode */

         if (line_transfer_type == WUFE_SBI_TYPE_E1 ||
             line_transfer_type == WUFE_CAD_TYPE_E1)
         {
            /* write nominal divisor values for E1 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_int,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_rem));

            /* set E1 mode */
            e1t1_mode = 1;

         }
         else if (line_transfer_type == WUFE_SBI_TYPE_T1 ||
                  line_transfer_type == WUFE_CAD_TYPE_T1)
         {
            /* write nominal divisor values for T1 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_int,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_rem));

            /* set T1 mode */
            e1t1_mode = 0;
         }
         else if (line_transfer_type == WUFE_SBI_TYPE_DS3 ||
                  line_transfer_type == WUFE_CAD_TYPE_DS3)
         {
            /* write nominal divisor values for DS3 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_int,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_rem));
            e1t1_mode = 0;

         }
         else if (line_transfer_type == WUFE_SBI_TYPE_E3 ||
                  line_transfer_type == WUFE_CAD_TYPE_E3)
         {
            /* write nominal divisor values for E3 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_int,
                                     drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_rem));
            e1t1_mode = 0;
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "ERROR invalid transfer type (%d)\n ", line_transfer_type));
            return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);
         }

      }
      else
      { /* extended differential mode */

         if(line_transfer_type == WUFE_SBI_TYPE_E1 ||
            line_transfer_type == WUFE_CAD_TYPE_E1)
         {
            /* write nominal divisor values for E1 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_int,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E1].nominal_rem));

            /* set E1 mode */
            e1t1_mode = 1;

         }
         else if(line_transfer_type == WUFE_SBI_TYPE_T1 ||
                 line_transfer_type == WUFE_CAD_TYPE_T1)
         {
            /* write nominal divisor values for T1 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_int,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_T1].nominal_rem));

            /* set T1 mode */
            e1t1_mode = 0;
         }
         else if (line_transfer_type == WUFE_SBI_TYPE_DS3 ||
                  line_transfer_type == WUFE_CAD_TYPE_DS3)
         {
            /* write nominal divisor values for DS3 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_int,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_DS3].nominal_rem));
            e1t1_mode = 0;

         }
         else if (line_transfer_type == WUFE_SBI_TYPE_E3 ||
                  line_transfer_type == WUFE_CAD_TYPE_E3)
         {
            /* write nominal divisor values for E3 */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecDcoDivisorSet(
                                     wufe_id,cr_id,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_int,
                                     ext_drc->divisor[WUFE_CLOCK_REC_REF_CLK_E3].nominal_rem));
            e1t1_mode = 0;
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "ERROR invalid transfer type (%d)\n ", line_transfer_type));
            return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);
         }

      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwClockRecDcoModeSet(
                               wufe_id, cr_id, e1t1_mode,
                               system_registry->clk_rec->cfg.sync_mode,
                               system_registry->clk_rec->cfg.active_mode));

      /* enable clock output */
      /* not needed in WP2/SBI mode (only CAD) but in order to make is simple for WP1/SBI we will
         always enable this bit. */
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwClockRecDcoEnable(wufe_id, cr_id, 1));

      clock_rec_if->n_active_lines ++;
   }
   else
   {
      clock_rec_if->n_active_lines --;

      /*The clock devisor should be configured only once per clock domain*/
      if(clock_rec_if->n_active_lines)
         return WUFE_OK;

      /* disable clock output */
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwClockRecDcoEnable(wufe_id, cr_id, 0));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_ClockRecSystemDisplay
 *
 * Purpose:   Display the global clock recovery parameters
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry  UFE system registry
 *
 * Outputs:
 *
 * Return Value:
 *     none.
 *
 * Called by:
 *     WUFE_SystemDisplay
 *
 ****************************************************************************/
void WUFEI_ClockRecSystemDisplay(WUFEI_system_registry *system_registry)
{
   WUFE_status status = WUFE_OK;

   WUFE_PRINT_DISP((wufei_log,
                    "\n\n-------------  CLOCK RECOVERY SYSTEM DISPLAY -----------\n"));

   if( system_registry == NULL)
   {
      WUFE_PRINT_DISP((wufei_log,"System Registry not defined\n"));
      return;
   }

   if(system_registry->cfg.system_clock_rec_enable == WUFE_SYSTEM_CLOCK_REC_DISABLE)
   {
      WUFE_PRINT_DISP((wufei_log,"System: system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_DISABLE\n"));
      return;
   }

   WUFE_PRINT_DISP((wufei_log,"System: system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_ENABLE\n"));

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      if(system_registry->clk_rec == NULL)
      {
         WUFE_PRINT_DISP((wufei_log,"Clock Recovery Registry not defined\n"));
         return;
      }
      /* Display CR registry for UFE3 only */
      WUFEI_ClockRecRegistryDisplay(system_registry);
   }

   /* CR HW registers display */
   status = WUFEI_HwClockRecDisplay(system_registry);
   if (status != WUFE_OK)
   {
      WUFE_PRINT_DISP((wufei_log, "\nWUFEI_HwClockRecDisplay returns with an error\n"));
      return;
   }
}

/*****************************************************************************
 *
 * Function:  WUFE_ClockRecDisplay
 *
 * Purpose:   Display the timestamp register and the clock generator RAM
 *            entry of a given PHY
 *
 * Description:
 *
 *
 * Inputs:
 *     ufe_phy_handle
 *
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      any user function.
 *
 ****************************************************************************/
WUFE_status WUFE_ClockRecDisplay(WUFE_handle ufe_phy_handle)
{

   WP_U16 phy_id, emphy_port_id;
   WP_U32 wufe_id, entry_id, core_id;
   WUFE_status status;
   WUFEI_system_registry *system_registry;
   WUFEI_phy_registry *phy_registry;
   WP_U32 line_id;
   WUFEI_line_registry *line_registry;
   WP_U32 cr_tx_if_id, cr_rx_if_id;

   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE3)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Invalid FPGA type. WUFE_ClockRecDisplay valid for UFE3 only.\n"));
      return WUFE_ERR_LINE_INVALID_FPGA;
   }

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

   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_LineRegistryGet(system_registry,
                                                        line_id,
                                                        &line_registry),
                                  wufe_id);

   WUFE_PRINT_DISP((wufei_log,
                    "\n\n----------- CLOCK RECOVERY INTERFACE DISPLAY (phy_id = %d) -------\n",
                    phy_id));

   /*Clock recovery system display*/
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_ClockRecRegistryIfDisplay(system_registry,
                                                                  phy_id),
                                  wufe_id);

   /*TX dispay*/
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_ClockRecTimestampDisplay(system_registry,
                                                                 phy_id,
                                                                 WUFE_TX_DIRECTION),
                                  wufe_id);

   /*RX dispay*/
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_ClockRecTimestampDisplay(system_registry,
                                                                 phy_id,
                                                                 WUFE_RX_DIRECTION),
                                  wufe_id);

   /* read the Rx clock generator register according to the line id */
   if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      WP_U16 ram_entry =0;

      /* find the "master" line for this CGEN (the line who controls the V4)*/
      cr_rx_if_id = WUFEI_LineClockRecIfRxGet(line_registry);

      for (line_id = 0; line_id < system_registry->cfg.max_line_number; line_id++)
      { /* extract the cgen from each line */
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_ClockRecRxClockGenRamGetFromLine(system_registry,
                                                                               line_id,
                                                                               &ram_entry,
                                                                               &entry_id),
                                        wufe_id);

         /* if cgen_id != rx_cr_if -> skip*/
         if (WUFEI_HW_CLK_REC_CGENRX_ID_V(ram_entry) != cr_rx_if_id)
            continue;
         if (WUFEI_HW_CLK_REC_CGENRX_VALID_V(ram_entry)
             && WUFEI_HW_CLK_REC_CGENRX_EN_V(ram_entry))
         {
            WUFE_PRINT_DISP((wufei_log,"\nMASTER line for this CGEN is %d (CGEN 0x%04x):\n",
                             line_id, ram_entry));
            WUFE_PRINT_DISP((wufei_log,"    Valid:        %x\n",
                             WUFEI_HW_CLK_REC_CGENRX_VALID_V(ram_entry)));
            WUFE_PRINT_DISP((wufei_log,"    Enable:       %x\n",
                             WUFEI_HW_CLK_REC_CGENRX_EN_V(ram_entry)));
            WUFE_PRINT_DISP((wufei_log,"    last phase:   0x%x\n",
                             WUFEI_HW_CLK_REC_CGENRX_PHASE_V(ram_entry)));
            WUFE_PRINT_DISP((wufei_log,"    CGEN unit number: %d (decimal)\n",
                             WUFEI_HW_CLK_REC_CGENRX_ID_V(ram_entry)));
            break;
         }
      }
   }

   cr_tx_if_id = WUFEI_LineClockRecIfTxGet(line_registry);
   if (cr_tx_if_id != 0xffff)
   {      
      WUFEI_HwClockRecDcoDisplay(wufe_id, cr_tx_if_id);
   }
   
   WUFE_PRINT_DISP((wufei_log
                    ,"-----------------------------------------------------------------\n\n"));

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_ClockRecInfo
 *
 * Purpose:   Initializes the Timestamp RX mashine register
 *            for the specified Line
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry - UFE system registry
 *     phy_id  - specifies the Master PHY
 *     enable  - indicates if the clock recovery is enabled for the Line
 *
 * Outputs:
 *     RX timestamp registers and the Clock generator RAM are initialized
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *           WUFE_ERR_LINE_ILLEGAL_ID - invalid Line handle is passed
 *           WUFE_ERR_ILLEGAL_PHY_ID - invalid PHY handle is passed
 *
 * Called by:
 *      any  function.
 *
 ****************************************************************************/
WUFE_status WUFE_ClockRecInfo(WP_U32 wufe_id,
                              WUFE_handle ufe_handle, /* phy or line */
                              WUFE_clock_rec_info_type info_type,
                              void* clk_rec_info)
{
   if(!clk_rec_info)
      return WUFE_ERR_NULL_POINTER;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (info_type == WUFE_CLOCK_REC_INFO_LINE)
      {
         WUFE_status status;
         WUFEI_system_registry *system_registry;

         /* lock the wufe_id and get the system registry */
         WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_ClockRecLineInfo(system_registry,
                                                               ufe_handle,
                                                               (WUFE_clk_rec_info_line *)clk_rec_info),
                                        wufe_id);

         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      }
      else if (info_type == WUFE_CLOCK_REC_INFO_RAM_LINE)
      {
         WUFE_status status;
         WUFEI_system_registry *system_registry;

         /* lock the wufe_id and get the system registry */
         WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_HwClockRecRAMGet(system_registry,
                                                               ufe_handle,
                                                               (WUFE_clk_rec_info_ram_line *)clk_rec_info),
                                        wufe_id);

         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid INFO type. WUFE_ClockRecInfo for UFE4 supports INFO_LINE only.\n"));
         return (WUFE_ERR_CLOCK_REC_INVALID_INFO_TYPE);
      }
   }
   else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {

      if(info_type == WUFE_CLOCK_REC_INFO_HW)
      {
         WUFE_clk_rec_info_hw *info_hw = clk_rec_info;

         /*Max TX/RX clock recovery interfaces per synthesis*/
         info_hw->max_ext_diff_rx_clk_rec_if = WUFEI_HwClockRecNumOfRxDiffUnitsGet(wufe_id);
         info_hw->max_rx_clk_rec_if = WUFEI_HwClockRecNumOfRxAdapUnitsGet(wufe_id);
         info_hw->max_ext_diff_tx_clk_rec_if = WUFEI_HwClockRecNumOfTxDiffUnitsGet(wufe_id);
         info_hw->max_tx_clk_rec_if = WUFEI_HwClockRecNumOfTxAdapUnitsGet(wufe_id);
#if 0
         printf("max_ext_diff_rx_clk_rec_if = %d\n", info_hw->max_ext_diff_rx_clk_rec_if);
         printf("max_rx_clk_rec_if = %d\n", info_hw->max_rx_clk_rec_if);
         printf("max_ext_diff_tx_clk_rec_if = %d\n", info_hw->max_ext_diff_tx_clk_rec_if);
         printf("max_tx_clk_rec_if = %d\n", info_hw->max_tx_clk_rec_if);
#endif
      }
      else if(info_type == WUFE_CLOCK_REC_INFO_SYSTEM)
      {
         WUFE_status status;
         WUFEI_system_registry *system_registry;

         /* lock the wufe_id and get the system registry */
         WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

         WUFEI_IF_ERROR_RETURN_DETACH_W( status,
                                         WUFEI_ClockRecSystemInfo(wufe_id,
                                                                  system_registry,
                                                                  (WUFE_clk_rec_info_system *)clk_rec_info),
                                         wufe_id);

         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      }
      else if(info_type == WUFE_CLOCK_REC_INFO_PHY)
      {
         WUFE_status status;
         WUFEI_system_registry *system_registry;

         /* lock the wufe_id and get the system registry */
         WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

         WUFEI_IF_ERROR_RETURN_DETACH_W(
            status,
            WUFEI_ClockRecPhyInfo(ufe_handle,
                                  system_registry,
                                  (WUFE_clk_rec_info_phy *)clk_rec_info),
            wufe_id);

         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      }
      else
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid INFO type. WUFE_ClockRecInfo for UFE3 supports INFO_HW, INFO_SYSTEM and INFO_PHY only.\n"));
         return (WUFE_ERR_CLOCK_REC_INVALID_INFO_TYPE);
      }

      return WUFE_OK;

   }
   else
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Invalid FPGA type. WUFE_ClockRecInfo valid for UFE3 or UFE4 only.\n"));
      return WUFE_ERR_LINE_INVALID_FPGA;
   }

   return WUFE_OK;
}

static WUFE_status WUFEI_ClockRecSystemInfo(WP_U32 wufe_id,
                                            WUFEI_system_registry *system_registry,
                                            WUFE_clk_rec_info_system *info_system)
{
   ((void)wufe_id);
   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   /*Clock recovery method Differential/Adaptive*/
   info_system->method = system_registry->clk_rec->cfg.method;
   info_system->max_rx_clk_rec_if = system_registry->clk_rec->cfg.max_rx_clk_rec_if;
   info_system->max_tx_clk_rec_if = system_registry->clk_rec->cfg.max_tx_clk_rec_if;

   /* Clock recovery extended differential */
   info_system->max_ext_diff_rx_clk_rec_if = WUFEI_HwClockRecNumOfRxDiffUnitsGet(wufe_id);
   info_system->max_ext_diff_tx_clk_rec_if = WUFEI_HwClockRecNumOfTxDiffUnitsGet(wufe_id);

   return WUFE_OK;
}

static WUFE_status WUFEI_ClockRecPhyInfo(WUFE_handle ufe_phy_handle,
                                         WUFEI_system_registry *system_registry,
                                         WUFE_clk_rec_info_phy *info_phy)
{
   WP_U16 line_id, phy_id, wufe_id, core_id, emphy_port_id;
   WUFEI_line_registry *line_registry;
   WUFEI_phy_registry *phy_registry;
   WP_U8 line_clock_rec_mode;
   WUFE_status status;
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (WUFEI_HANDLE_TYPE_V(ufe_phy_handle) != WUFE_HANDLE_TYPE_PHY)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR phy handle 0x%08X ", ufe_phy_handle));
      return WUFE_ERR_HANDLE;
   }

   /*********************************/
   /* Get registry, phy             */
   /*********************************/
   wufe_id = WUFEI_HANDLE_UFE_ID_V(ufe_phy_handle);
   core_id = WUFEI_HANDLE_UFE_CORE_ID_V(ufe_phy_handle);
   emphy_port_id =  WUFEI_PHY_HANDLE_TO_PHY_ID(core_id,
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

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   WUFEI_RETURN_IF_ERROR(status,  WUFEI_PhyRegistryGet(system_registry,
                                                       phy_id,
                                                       &phy_registry));
   line_id = WUFEI_PhyLineIdGet(phy_registry);

   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineRegistryGet(system_registry,
                                                       line_id,
                                                       &line_registry));

   line_clock_rec_mode = WUFEI_LineClockRecModeGet(line_registry);

   info_phy->tx.enable = WUFE_DISABLE;

   if (line_clock_rec_mode == WUFE_LINE_CLOCK_REC_DCO_ONLY)
   {
      info_phy->tx.enable = WUFE_ENABLE;
      /*TX phy info*/
      info_phy->tx.if_id = WUFEI_LineClockRecIfTxGet(line_registry);
      info_phy->tx.buffer_size = 0;
   }

   if(line_clock_rec_mode & WUFE_LINE_CLOCK_REC_ENABLE_TX)
   {
      info_phy->tx.enable = WUFE_ENABLE;

      /*TX phy info*/
      info_phy->tx.if_id = WUFEI_LineClockRecIfTxGet(line_registry);

      /*TX Timestamp buffer size value*/
      WUFEI_HwClockRecTxTsBufferSizeRegGet(wufe_id,
                                           info_phy->tx.if_id,
                                           &info_phy->tx.buffer_size);

   }

   return WUFE_OK;
}

static WUFE_status WUFEI_ClockRecLineInfo(WUFEI_system_registry *system_registry,
                                          WUFE_handle ufe_line_handle,
                                          WUFE_clk_rec_info_line *clk_rec_info)
{
   WUFEI_line_registry *line_registry;
   WP_U32 line_id;

   /* check valid line handle */
   if (WUFEI_HANDLE_TYPE_V(ufe_line_handle) != WUFE_HANDLE_TYPE_LINE)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR line handle 0x%08X ", ufe_line_handle));
      return WUFE_ERR_HANDLE;
   }

   /* Extract information from line handle */
   line_id = WUFEI_HANDLE_INDEX_V(ufe_line_handle);
   line_registry = &system_registry->line_arr[line_id];

   /* Get the Clock Recovery interface ID */
   clk_rec_info->clock_rec_if_id =
      (line_registry->aug4 * WUFE_MAX_N_AUG1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU ) + /* stm4 or sts12 */
      (line_registry->aug1 * WUFE_MAX_N_AU3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU) + /* stm1 or sts3 */
      (line_registry->au3 * WUFE_MAX_N_TUG2 * WUFE_MAX_N_TU)  + /* stm0 or sts1 */
      (line_registry->tug2 * WUFE_MAX_N_TU) + /* tug2 or vt_group */
      line_registry->tu; /*   tu or vt */

   /*
    * If we are in dual EMPHY and on the secondary EMPHY port
    * we add 336 to make the numbering contiguous
    */
   if(system_registry->emphy_number == WUFEI_EMPHY_SECOND)
	   clk_rec_info->clock_rec_if_id += WUFEI_SECOND_EMPHY_LINE_OFFSET;

   return WUFE_OK;
}


WUFE_status WUFEI_ClockRecMethodGet(WUFEI_system_registry *system_registry,
                                    WP_U8 *method)
{
#ifdef WUFE_UFE2PLUS
   return WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED;
#endif

   if (!system_registry->clk_rec)
      return (WUFE_ERR_CLOCK_REC_NOT_ENABLED);

   *method = system_registry->clk_rec->cfg.method;

   return WUFE_OK;
}


/***** functions for UFE4 *****/
void WUFEI_LineClockRecClearParams(WUFEI_line_registry *line_registry)
{
   /* Clear not-in-used parameters for UFE4 */
   line_registry->tx_clk_master = WUFE_LINE_CLOCK_SLAVE;              /* 0 */
   line_registry->clock_rec_mode = WUFE_LINE_CLOCK_REC_DISABLE;
   line_registry->clock_translator_id = WUFE_CLOCK_REC_INVALID_VALUE; /* FFFF */
   line_registry->rx_clock_rec_if_id = WUFE_CLOCK_REC_INVALID_VALUE;  /* FFFF */
   line_registry->tx_clock_rec_if_id = WUFE_CLOCK_REC_INVALID_VALUE;  /* FFFF */
}

WUFE_status WUFEI_ClockRecSystemEnable(WUFEI_system_registry *system_registry)
{
   WUFE_status status = WUFE_OK;
   WP_U32 diff_to_socket_ratio_counter_value = WUFEI_SYSTEM_CLOCK_155_52;
   WP_U32 xo_to_socket_ratio_counter_value   = WUFEI_SYSTEM_CLOCK_155_52;
   WP_U32 ratio_counter_value = 0;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (system_registry->cfg.system_clock_rec_enable == WUFE_SYSTEM_CLOCK_REC_ENABLE)
      {
         /* Set CR enable in the UFE HW. All CR logic is enabled (Rx+Tx) */
         WUFEI_RETURN_IF_ERROR(status, WUFEI_ClkRecModeSet(system_registry, WUFE_ENABLE));

         /* Set the Ratio Counter for the TX side */
         /* Calculate system clock:
            bandwidth (OC)   cores   sys_clk (MHz)
                48             2       155.52
                24             2        77.76
                24             1       155.52
              12,6,3           1        77.76    */
         if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
            ratio_counter_value = WUFEI_SYSTEM_CLOCK_155_52;
         else if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC24)
         {
            if (system_registry->hw_config.core_enabled[1])
               ratio_counter_value = WUFEI_SYSTEM_CLOCK_77_76;
            else
               ratio_counter_value = WUFEI_SYSTEM_CLOCK_155_52;
         }
         else
         {
            /* OC12, OC6 and OC3 */
            ratio_counter_value = WUFEI_SYSTEM_CLOCK_77_76;
         }

         /* Convert system clocks to 125usec units (125usec = 1/8000) */
         ratio_counter_value = ratio_counter_value / 8000;
         /* Set ratio counters */
         diff_to_socket_ratio_counter_value = ratio_counter_value * WUFE_DIFF2SCK_RATIO_COUNTER_VALUE;
         xo_to_socket_ratio_counter_value   = ratio_counter_value * WUFE_XO2SCK_RATIO_COUNTER_VALUE;

         WUFEI_HwClkRecRatioCountersSet(system_registry,
                                        diff_to_socket_ratio_counter_value,
                                        xo_to_socket_ratio_counter_value);
      }
      else if (system_registry->cfg.system_clock_rec_enable != WUFE_SYSTEM_CLOCK_REC_DISABLE)
      {
         WUFEI_RETURN_IF_ERROR(status, (WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS));
      }
   }
   return status;
}

WUFE_status WUFEI_ClockRecSystemDisable(WUFEI_system_registry *system_registry)
{
   WUFE_status status = WUFE_OK;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (system_registry->cfg.system_clock_rec_enable == WUFE_SYSTEM_CLOCK_REC_ENABLE)
      {
         /* Clear CR enable in the UFE HW */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClkRecModeSet(system_registry, WUFE_DISABLE));

         /* Clear the Ratio Counter for the TX side */
         WUFEI_HwClkRecRatioCountersSet(system_registry, 0, 0);
      }
      else if (system_registry->cfg.system_clock_rec_enable != WUFE_SYSTEM_CLOCK_REC_DISABLE)
      {
         WUFEI_RETURN_IF_ERROR(status, (WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS));
      }
   }
   return status;
}

/* Function for internal testing and debug */
/* This function sets or clears the TS stub bit in the CR mode register.
   It is used mainly for testing and debug and required a dedicated synthesis. This function
   has no real purpose in a working system and should not be called. */
WUFE_status WUFE_ClockRecTimestampStubSet(WP_U32 wufe_id, WP_U32 enable)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry *system_registry;
   ((void)enable);   
   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      /* lock the wufe_id UFE*/
      WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_HwClkRecTsSet(system_registry, enable),
                                     wufe_id);

      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   }
   return status;
}
/* End of function for internal testing and debug */

WUFE_status WUFEI_ClockRecLineEnable(WP_U32 wufe_id,
                                     WUFEI_system_registry *system_registry,
                                     WP_U32 line_id,
                                     WUFEI_line_registry *line_registry)
{
   WUFE_status status = WUFE_OK;
   WUFE_line_clock_rec_params *cr_line_params = line_registry->clock_rec_line_params;

   /* Enable the line CR */
   if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_ClockRecLineTxModeRamSet(wufe_id,
                                                           line_id,
                                                           (WUFE_line_transf_type) line_registry->transfer_type,
                                                           1));

      /* If needed, enable the DCO in the line */
      if (cr_line_params->clk_rec_physical_clock_enable)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_ClkRecPhysicalClockSet(system_registry, line_id));
      }
   }
   return status;
}

WUFE_status WUFEI_ClockRecLineDisable(WP_U32 wufe_id,
                                      WUFEI_system_registry *system_registry,
                                      WP_U32 line_id,
                                      WUFEI_line_registry *line_registry)
{
   WUFE_status status = WUFE_OK;
   WUFE_line_clock_rec_params *cr_line_params = line_registry->clock_rec_line_params;

   /* Disable the line CR */
   if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_ClockRecLineTxModeRamSet(wufe_id,
                                                                    line_id,
                                                                    (WUFE_line_transf_type) line_registry->transfer_type,
                                                                    0),
                                     wufe_id);

      /* If needed, disable the DCO in the line */
      if (cr_line_params->clk_rec_physical_clock_enable)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_ClkRecPhysicalClockClear(system_registry, line_id),
                                        wufe_id);
      }
   }
   return status;
}

WUFE_status WUFEI_ClockRecLineTxModeRamSet(WP_U16 wufe_id,
                                           WP_U16 line_id,
                                           WUFE_line_transf_type line_transfer_type,
                                           WP_U8 enable)
{
   WUFE_status status = WUFE_OK;
   WP_U8 line_type_e1;
   ((void)wufe_id);
   ((void)line_id);
   ((void)enable);
   if ((line_transfer_type != WUFE_SDH_TYPE_T1) &
       (line_transfer_type != WUFE_SDH_TYPE_E1) &
       (line_transfer_type != WUFE_SONET_TYPE_T1) &
       (line_transfer_type != WUFE_SONET_TYPE_E1))
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR invalid transfer type: E1 or T1 only.\n"));
      return (WUFE_ERR_CLOCK_REC_INVALID_TRANSFER_TYPE);
   }

   if ((line_transfer_type == WUFE_SDH_TYPE_E1) ||
       (line_transfer_type == WUFE_SONET_TYPE_E1))
      line_type_e1 = 1;
   else
      line_type_e1 = 0;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClockRecTxTribModeRamSet(wufe_id, line_id, line_type_e1, enable));
   return status;
}

WUFE_status WUFEI_ClkRecModeSet(WUFEI_system_registry *system_registry, WP_U32 enable)
{
   WUFE_status status = WUFE_OK;
   WP_U32 direction;
   ((void)system_registry);   
   if ((enable != WUFE_DISABLE) & (enable != WUFE_ENABLE))
   {
      return WUFE_ERR_CLOCK_REC_INVALID_CONFIG;
   }

   /* Set both directions */
   direction =   WUFE_FULL_DUPLEX ;

   if (enable)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwClkRecEnable(system_registry, direction));
   }
   else
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwClkRecDisable(system_registry, direction));
   }
   return status;
}

WUFE_status WUFEI_ClkRecPhysicalClockSet(WUFEI_system_registry *system_registry, WP_U32 line_id)
{
   WUFE_status status;
   WP_U32 wufe_id = system_registry->wufe_id;
   WUFE_clk_rec_system_params *cr_params = system_registry->cfg.clk_rec_system_params;
   WP_U32 xo_ref_clk = cr_params->xo_ref_clk;
   WUFEI_line_registry   *line_registry;
   WUFEI_clock_rec_dco_registry *clk_rec_dco_reg;
   WP_U32 dco_id, timing_mode_freq; /* The E1/T1 frequency */
   WP_U32 nominal_int, nominal_rem;




   /* Verify DCO block in HW */
   /* TBD: need the HW programing model. */


   /* Extract the line registry */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));
   /* Get timing mode */
   if(line_registry->transfer_type == WUFE_CAD_TYPE_T1 ||
      line_registry->transfer_type == WUFE_SDH_TYPE_T1 ||
      line_registry->transfer_type == WUFE_SONET_TYPE_T1)
   {
      timing_mode_freq = WUFE_CLOCK_REC_T1_FREQ;
   }
   else if(line_registry->transfer_type == WUFE_CAD_TYPE_E1 ||
           line_registry->transfer_type == WUFE_SDH_TYPE_E1 ||
           line_registry->transfer_type == WUFE_SONET_TYPE_E1)
   {
      timing_mode_freq = WUFE_CLOCK_REC_E1_FREQ;
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log, "WUFE:ERROR transfer type\n"));
      return WUFE_ERR_INVALID_TRANSFER_TYPE;
   }

   /* Get DCO information and check availability */
   dco_id = (line_registry->clock_rec_line_params)->clk_rec_physical_clock_id;
   if (dco_id != 0 && dco_id != 1)
      return WUFE_ERR_CLOCK_REC_INVALID_PHYSICAL_CLOCK_ID;

   clk_rec_dco_reg = &(system_registry->clk_rec_dco_reg[dco_id]);
   if (clk_rec_dco_reg->dco_active)
   {
      WUFE_PRINT_ERR((wufei_log, "WUFE:ERROR DCO already in used.\n"));
      return WUFE_ERR_CLOCK_REC_PHYSICAL_CLOCK_ALREADY_ACTIVE;
   }

   /* calculate the DCO initiger and remainder parts */
   /* int.rem = diff_ref_freq/(2*timing_mode_freq) */
   WUFEI_ClockRec64BitDivisionCalculation(xo_ref_clk, (2*timing_mode_freq), &nominal_int, &nominal_rem);
   nominal_int--;

   /* update HW to work with DCO */
   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwClockRecDcoEnable(wufe_id,dco_id, 1));
   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwClockRecDcoDivisorSet(wufe_id, dco_id,nominal_int, nominal_rem));



   /* Set DCO parameters into the DCO block */
   clk_rec_dco_reg->dco_active = 1;
   clk_rec_dco_reg->divisor.nominal_int = nominal_int;
   clk_rec_dco_reg->divisor.nominal_rem = nominal_rem;

   /* Update line registry to work with DCO */
   (line_registry->clock_rec_line_params)->clk_rec_physical_clock_enable = 1;

   return WUFE_OK;
}

WUFE_status WUFEI_ClkRecPhysicalClockClear(WUFEI_system_registry *system_registry, WP_U32 line_id)
{
   WUFE_status status;
   WP_U32 wufe_id = system_registry->wufe_id;
   WUFEI_line_registry *line_registry;
   WUFEI_clock_rec_dco_registry *clk_rec_dco_reg;
   WP_U32 dco_id;

   /* Extract the line registry */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_LineRegistryGet(system_registry,
                                               line_id,
                                               &line_registry));

   /* Get DCO information and clear it */
   dco_id = (line_registry->clock_rec_line_params)->clk_rec_physical_clock_id;
   if (dco_id != 0 && dco_id != 1)
      return WUFE_ERR_CLOCK_REC_INVALID_PHYSICAL_CLOCK_ID;

   clk_rec_dco_reg = &(system_registry->clk_rec_dco_reg[dco_id]);
   if (clk_rec_dco_reg->dco_active != 0)
   {
      clk_rec_dco_reg->dco_active = 0;
      clk_rec_dco_reg->divisor.nominal_int = 0;
      clk_rec_dco_reg->divisor.nominal_rem = 0;
   }

   /* Update line registry to work without DCO */
   (line_registry->clock_rec_line_params)->clk_rec_physical_clock_enable = 0;
   (line_registry->clock_rec_line_params)->clk_rec_physical_clock_id = 0;

   /* update HW to work without DCO */
   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwClockRecDcoEnable(wufe_id,dco_id, 0));

   return WUFE_OK;
}

static void WUFEI_ClockRec64BitDivisionCalculation(WP_U32 a, WP_U64 b, WP_U32* int_val, WP_U32* rem_val)
{
   WP_U64 tmp = 0;
   WP_U32 mask;

   WP_U32 n_int = 0;
   WP_U32 n_rem = 0;

   volatile WP_U32 i;

   /* Calculate the integer part */
   mask = 0x80000000;
   for(i = 0; i < 32; i++)
   {
      tmp <<= 1;

      if(mask & a)
         tmp++;

      if(tmp >= b)
      {
         n_int |= mask;
         tmp -= b;
      }
      mask >>= 1;
   }

   *int_val = n_int;

   /* Calculate the MSB fractional part */
   mask = 0x80000000;
   for(i = 0; i < 32; i++)
   {
      tmp <<= 1;

      if(tmp >= b)
      {
         n_rem |= mask;
         tmp -= b;
      }
      mask >>= 1;
   }

   *rem_val = n_rem;
}

WUFE_status WUFE_GpioOutputClockSet(WP_U32 wufe_id, WP_U32 gpio_number, WP_U8 gpio_clock, WP_U8 gpio_ref_clk_out)
{
   WUFEI_system_registry *system_registry;
   WUFE_status status = WUFE_OK;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      /* lock the wufe_id UFE*/
      WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

      if ((gpio_number != WUFE_GPIO_0) && (gpio_number != WUFE_GPIO_1))
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid GPIO number.\n"));
         return WUFE_ERR_INVALID_GPIO_CLOCK;
      }
      if ((gpio_clock != WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_0) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_1) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_0) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_1) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_GP_CLK_IN_0) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_GP_CLK_IN_1) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_SYSTEM_CLK) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_XO_PLL_REF_CLK) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_RECV_SDH_LINE_CLK_1) &&
          (gpio_clock != WUFE_GPIO_CLK_OUT_NONE))
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid GPIO output clock.\n"));
         return WUFE_ERR_INVALID_GPIO_CLOCK;
      }
      if ((gpio_ref_clk_out != WUFE_GPIO_REF_CLK_OUTPUT_XO_REF_CLK) &&
          (gpio_ref_clk_out != WUFE_GPIO_REF_CLK_OUTPUT_DIFF_REF_CLK))
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "Invalid GPIO Reference Clock Output Select.\n"));
         return WUFE_ERR_INVALID_GPIO_CLOCK;
      }

      /* Update HW and system registry with GPIO information */
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_UfeGpioOutputClockSet(system_registry,
                                                                 gpio_number,
                                                                 gpio_clock,
                                                                 gpio_ref_clk_out),
                                     wufe_id);

      /* Release wufe_id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   }
   else
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Invalid FPGA type. WUFE_GpioOutputClockSet valid for UFE4 only.\n"));
      return WUFE_ERR_LINE_INVALID_FPGA;
   }

   return status;
}

WUFE_status WUFE_GpioOutputClockRead(WP_U32 wufe_id, WP_U16 *gpio_register)
{
   WUFEI_system_registry *system_registry;
   WUFE_status status = WUFE_OK;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      /* lock the wufe_id UFE*/
      WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

      /* Read GPIO HW register */
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_UfeGpioOutputClockRead(system_registry,
                                                                  gpio_register),
                                     wufe_id);
      /* Release wufe_id */
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

      /* Display the GPIO register */
      WUFEI_UfeGpioOutputClockdisplay(*gpio_register);
   }
   else
   {
      WUFE_PRINT_ERR((wufei_log,
                      "Invalid FPGA type. WUFE_GpioOutputClockRead valid for UFE4 only.\n"));
      return WUFE_ERR_LINE_INVALID_FPGA;
   }

   return status;
}

WUFE_status WUFEI_UfeGpioOutputClockSet(WUFEI_system_registry* system_registry,
                                        WP_U32 gpio_number, WP_U8 gpio_clock, WP_U8 gpio_ref_clk_out)
{
   WP_U32 wufe_id;
   WUFE_status status = WUFE_OK;
   WP_U16 gpio_register;

   wufe_id = system_registry->wufe_id;

   /* Build the GPIO register */
   switch (gpio_clock)
   {
      case WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_0:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_TX_CLK_0 : WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_TX_CLK_0;
         break;
      }
      case WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_1:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_TX_CLK_1 : WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_TX_CLK_1;
         break;
      }
      case WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_0:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_LINE_CLK_0 : WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_LINE_CLK_0;
         break;
      }
      case WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_1:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_LINE_CLK_1 : WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_LINE_CLK_1;
         break;
      }
      case WUFE_GPIO_CLK_OUT_GP_CLK_IN_0:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_GP_CLK_IN_0 : WUFEI_GPIO_CLK_OUT_0_SELECT_GP_CLK_IN_0;
         break;
      }
      case WUFE_GPIO_CLK_OUT_GP_CLK_IN_1:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_GP_CLK_IN_1 : WUFEI_GPIO_CLK_OUT_0_SELECT_GP_CLK_IN_1;
         break;
      }
      case WUFE_GPIO_CLK_OUT_SYSTEM_CLK:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_SYSTEM_CLK : WUFEI_GPIO_CLK_OUT_0_SELECT_SYSTEM_CLK;
         break;
      }
      case WUFE_GPIO_CLK_OUT_XO_PLL_REF_CLK:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_XO_PLL_REF_CLK : WUFEI_GPIO_CLK_OUT_0_SELECT_XO_PLL_REF_CLK;
         break;
      }
      case WUFE_GPIO_CLK_OUT_RECV_SDH_LINE_CLK_1:
      {
         gpio_register = gpio_number ?
            WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_SDH_LINE_CLK_1 : WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_SDH_LINE_CLK_1;
         break;
      }
      default:
      {
         gpio_register =
            WUFEI_GPIO_CLK_OUT_1_SELECT_NONE | WUFEI_GPIO_CLK_OUT_0_SELECT_NONE;
         break;
      }
   }
   switch (gpio_ref_clk_out)
   {
      case WUFE_GPIO_REF_CLK_OUTPUT_XO_REF_CLK:
      {
         gpio_register |= WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_XO_REF_CLK;
         break;
      }
      case WUFE_GPIO_REF_CLK_OUTPUT_DIFF_REF_CLK:
      {
         gpio_register |= WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_DIFF_REF_CLK;
         break;
      }
      default:
      {
         gpio_register |= WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_XO_REF_CLK;
         break;
      }
   }

   /* Update HW with GPIO information */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_BsFpgaGpioClockSelect(wufe_id, gpio_number, gpio_register));
   return status;
}


WUFE_status WUFEI_UfeGpioOutputClockRead(WUFEI_system_registry* system_registry,
                                         WP_U16 *gpio_register)
{
   WP_U32 wufe_id;
   WUFE_status status = WUFE_OK;
   ((void)gpio_register);   
   wufe_id = system_registry->wufe_id;
   /* Read GPIO register from BS logic */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_BsFpgaGpioClockGet(wufe_id, gpio_register));

   return status;
}


void WUFEI_UfeGpioOutputClockdisplay(WP_U16 gpio_register)
{
   WP_U16 tmp_gpio;

   WUFE_PRINT_DISP((wufei_log,"GPIO Output Clock Register state: 0x%x\n",
                    gpio_register));

   /* Display the GPIO register */
   tmp_gpio = gpio_register & 0xff00;
   if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_TX_CLK_0)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_0\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_TX_CLK_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_1\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_LINE_CLK_0)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_0\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_LINE_CLK_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_1\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_GP_CLK_IN_0)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_GP_CLK_IN_0\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_GP_CLK_IN_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_GP_CLK_IN_1\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_SYSTEM_CLK)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_SYSTEM_CLK\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_XO_PLL_REF_CLK)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_XO_PLL_REF_CLK\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_SDH_LINE_CLK_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_RECV_SDH_LINE_CLK_1\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 1 is configured to WUFE_GPIO_CLK_OUT_NONE\n"));

   tmp_gpio = gpio_register & 0x00ff;
   if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_TX_CLK_0)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_0\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_TX_CLK_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_1\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_LINE_CLK_0)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_0\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_LINE_CLK_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_1\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_GP_CLK_IN_0)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_GP_CLK_IN_0\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_GP_CLK_IN_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_GP_CLK_IN_1\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_SYSTEM_CLK)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_SYSTEM_CLK\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_XO_PLL_REF_CLK)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_XO_PLL_REF_CLK\n"));
   else if (tmp_gpio == WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_SDH_LINE_CLK_1)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_RECV_SDH_LINE_CLK_1\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"\tGPIO output clock 0 is configured to WUFE_GPIO_CLK_OUT_NONE\n"));

   tmp_gpio = gpio_register & 0x1000;
   if (tmp_gpio == WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_XO_REF_CLK)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO reference clock output is configured to WUFE_GPIO_REF_CLK_OUTPUT_XO_REF_CLK\n"));
   else if (tmp_gpio == WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_DIFF_REF_CLK)
      WUFE_PRINT_DISP((wufei_log,"\tGPIO reference clock output is configured to WUFE_GPIO_REF_CLK_OUTPUT_DIFF_REF_CLK\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"\tGPIO reference clock output is not configured\n"));

}

WUFE_status WUFEI_UfeGpioClockDefaultSetup(WUFEI_system_registry* system_registry)
{
   WUFE_status status = WUFE_OK;
   WP_U32 wufe_id;
   WP_U16 gpio_register;

   wufe_id = system_registry->wufe_id;
   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      /* Set Default values to GPIO register */
      gpio_register =
    	 WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_LINE_CLK_0 |
         WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_TX_CLK_0 |
         WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_XO_REF_CLK;

      /* Update BS with GPIO register */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_BsFpgaGpioClockSelect(wufe_id, WUFE_GPIO_0, gpio_register));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_BsFpgaGpioClockSelect(wufe_id, WUFE_GPIO_1, gpio_register));
   }

   return status;
}

