/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_events.c
 *
 * Purpose: internal functions for UFE events.
 *
 ****************************************************************************/
#include <stdio.h>
#include <string.h>

#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFEI_HW_EVENTS_INT_H
#include "wufei_hw_events_int.h"
#endif

#ifndef WUFEI_HW_EVENTS_H
#include "wufei_hw_events.h"
#endif

#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif

#ifndef WUFEI_HW_GLOBALS_INT_H
#include "wufei_hw_globals_int.h"
#endif

#ifndef WUFEI_HW_GLOBALS_H
#include "wufei_hw_globals.h"
#endif

#ifndef WUFEI_EVENTS_INT_H
#include "wufei_events_int.h"
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

#ifndef WUFEI_HW_EMPHY_INT_H
#include "wufei_hw_emphy_int.h"
#endif

#ifndef WUFEI_HW_CLK_REC_INT_H
#include "wufei_hw_clk_rec_int.h"
#endif

#ifndef WUFEI_HW_BOARD_SUPPORT_INT_H
#include "wufei_board_support_int.h"
#endif

#ifndef WUFEI_HW_BOARD_SUPPORT_H
#include "wufei_board_support.h"
#endif

/*****************************************************************************
 *
 * Function: WUFEI_HwEventReadEmphyEvents
 *
 * Purpose: Reads the unmasked Emphy events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     emphy_events      pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDispatch
 *
 ****************************************************************************/
static void WUFEI_HwEventReadEmphyEvents(WP_U32 ufeid,
                                         WP_U32 coreid,
                                         WUFE_events_emphy* emphy_events)
{
   WP_U16 event_mask_reg, event_reg;
   WP_U16 phy_id;

   WUFEI_HwEmphyEventRegGet(ufeid, coreid, &event_reg);
   WUFEI_HwEmphyEventMaskRegGet(ufeid, coreid, &event_mask_reg);

   emphy_events->event_reg = event_reg & ~event_mask_reg;

   if (emphy_events->event_reg & WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID)
   {
      WUFEI_HwEmphyPhyNotValidGet(ufeid, coreid, &phy_id);
      emphy_events->phy_not_valid_handle = (WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_PHY) |
                                            WUFEI_HANDLE_UFE_ID_F(ufeid) |
                                            WUFEI_HANDLE_UFE_CORE_ID_F(coreid)|
                                            WUFEI_HANDLE_INDEX_F(phy_id) );
   }

   /*clear interrupt register */
   WUFEI_HwEmphyEventRegSet(ufeid, coreid, event_reg);

   return;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventReadBsPllEvents
 *
 * Purpose: Reads the unmasked BS lock lost events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     bs_events      pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDispatch
 *
 ****************************************************************************/
static void WUFEI_HwEventReadBsPllEvents(WP_U32 ufeid,
                                         WUFE_events_bs* bs_events)
{
   WP_U16 event_mask_reg, event_reg;

   WUFEI_HwBsLockLostRegGet(ufeid, &event_reg);
   WUFEI_HwBsLockLostMaskRegGet(ufeid, &event_mask_reg);

   bs_events->pll_lost_lock_event_reg = event_reg & ~event_mask_reg;

   /*clear interrupt register */
   WUFEI_HwBsLockLostRegSet(ufeid, event_reg);

   return;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventReadSfpEvents
 *
 * Purpose: Reads the unmasked sfp events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     bs_events      pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDispatch
 *
 ****************************************************************************/
static void WUFEI_HwEventReadBsSfpEvents(WP_U32 ufeid,
                                         WUFE_events_bs* bs_events)
{
   WP_U16 event_mask_reg, event_reg;

   WUFEI_HwBsSfpAbsentRegGet(ufeid, &event_reg);
   WUFEI_HwBsSfpAbsentMaskRegGet(ufeid, &event_mask_reg);

   bs_events->absent_event_reg = event_reg & ~event_mask_reg;

   /*clear interrupt register */
   WUFEI_HwBsSfpAbsentRegSet(ufeid, event_reg);

   WUFEI_HwBsSfpLosRegGet(ufeid, &event_reg);
   WUFEI_HwBsSfpLosMaskRegGet(ufeid, &event_mask_reg);

   bs_events->los_event_reg = event_reg & ~event_mask_reg;

   /*clear interrupt register */
   WUFEI_HwBsSfpLosRegSet(ufeid, event_reg);

   return;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventReadTdmEvents
 *
 * Purpose: Reads the unmasked Tdm events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     tdm_events      pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDispatch
 *
 ****************************************************************************/
static void WUFEI_HwEventReadTdmEvents(WP_U32 ufeid,
                                       WUFE_events_tdm* tdm_events)
{
   WP_U16 event_mask_reg, event_reg_core0 =0, event_reg_core1 =0;
   WP_U16 global_int_reg = WUFEI_UfeCoreGlobalIntGet(ufeid);

   if (WUFEI_UFE_GLOBAL_INT_TDM0_V(global_int_reg))
   {
      WUFEI_HwTdmEventRegGet(ufeid, WUFE_CORE_ID_0, &event_reg_core0);
      WUFEI_HwTdmEventMaskRegGet(ufeid, WUFE_CORE_ID_0, &event_mask_reg);

      /*Fill the structure with the unmasked events*/
      event_reg_core0 = event_reg_core0 & ~event_mask_reg;

      /*clear interrupt register */
      WUFEI_HwTdmEventRegSet(ufeid, WUFE_CORE_ID_0, event_reg_core0);
   }

   if (WUFEI_UFE_GLOBAL_INT_TDM1_V(global_int_reg))
   {
      WUFEI_HwTdmEventRegGet(ufeid, WUFE_CORE_ID_1, &event_reg_core1);
      WUFEI_HwTdmEventMaskRegGet(ufeid, WUFE_CORE_ID_1, &event_mask_reg);

      /*Fill the structure with the unmasked events*/
      event_reg_core1 = event_reg_core1 & ~event_mask_reg;

      /*clear interrupt register */
      WUFEI_HwTdmEventRegSet(ufeid, WUFE_CORE_ID_1, event_reg_core1);
   }

   tdm_events->event_reg = event_reg_core0 | event_reg_core1;

   return;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventReadMachineEventsPerCore
 *
 * Purpose: Reads the unmasked Machine events of the specified core
 *
 *
 * Description:
 *     Reads the unmasked events from the event register of the specified core.
 *     Clears the event register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     machine_events  pointer to a structure to fill in
 *     event_reg_rx    pointer to event_reg_rx
 *     event_reg_tx    pointer to event_reg_tx
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value: none
 *
 *
 * Called by:WUFEI_HwEventReadMachineEvents
 *
 *
 ****************************************************************************/
static void WUFEI_HwEventReadMachineEventsPerCore(WP_U32 ufeid,
                                                  WP_U32 coreid,
                                                  WUFE_events_machine* machine_events,
                                                  WP_U16* event_reg_rx,
                                                  WP_U16* event_reg_tx)
{
   WP_U16 event_mask_reg, event_reg;

   /** RX **/
   WUFEI_HwMachineEventRegGet(ufeid, coreid, &event_reg, WUFE_RX_DIRECTION);
   WUFEI_HwMachineEventMaskRegGet(ufeid, coreid, &event_mask_reg, WUFE_RX_DIRECTION);

   /*Fill the structure with the unmasked events*/
   *event_reg_rx = event_reg & ~event_mask_reg;

   /*clear interrupt register */
   WUFEI_HwMachineEventRegSet(ufeid, coreid, event_reg, WUFE_RX_DIRECTION);

   if (*event_reg_rx)
      WUFEI_HwMachineRxPortIdGet(ufeid, coreid, &(machine_events->rx_event_phy_num));

   /** TX **/
   WUFEI_HwMachineEventRegGet(ufeid, coreid, &event_reg, WUFE_TX_DIRECTION);
   WUFEI_HwMachineEventMaskRegGet(ufeid, coreid, &event_mask_reg, WUFE_TX_DIRECTION);

   /*Fill the structure with the unmasked events*/
   *event_reg_tx = event_reg & ~event_mask_reg;

   /*clear interrupt register */
   WUFEI_HwMachineEventRegSet(ufeid, coreid, event_reg, WUFE_TX_DIRECTION);

   if (*event_reg_tx)
      WUFEI_HwMachineTxPortIdGet(ufeid, coreid, &(machine_events->tx_event_phy_num));

   return;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventReadMachineEvents
 *
 * Purpose: Calls the function WUFEI_HwEventReadMachineEventsPerCore
 *          with the appropriate core id for reading the unmasked Machine events
 *
 * Description:
 *     if an unmasked machine event rise in the global interrupt register of core 0 then
 *     the function call WUFEI_HwEventReadMachineEventsPerCore with core id 0 for reading
 *     the unmasked Machine events of core 0.
 *     if an unmasked machine event rise in the global interrupt register of core 1
 *     then the function call WUFEI_HwEventReadMachineEventsPerCore
 *     with core id 1 for reading the unmasked Machine events of core 1.
 *
 * Inputs:
 *     ufeid           UFE id
 *     machine_events  pointer to a structure to fill in
 *
 * Outputs:
 *     machine_events
 *
 * Return Value: none
 *
 * Called by:
 *     WUFEI_HwEventDispatch
 *
 ****************************************************************************/
static void WUFEI_HwEventReadMachineEvents(WP_U32 ufeid,
                                           WUFE_events_machine* machine_events)
{
   WP_U16 event_reg_rx_core0 = 0, event_reg_tx_core0 = 0 ;
   WP_U16 event_reg_rx_core1 = 0, event_reg_tx_core1 = 0 ;
   WP_U16 global_int_reg = WUFEI_UfeCoreGlobalIntGet(ufeid);

   if (WUFEI_UFE_GLOBAL_INT_MACHINE0_V(global_int_reg))
      WUFEI_HwEventReadMachineEventsPerCore(ufeid, WUFE_CORE_ID_0, machine_events, &event_reg_rx_core0, &event_reg_tx_core0);

   if (WUFEI_UFE_GLOBAL_INT_MACHINE1_V(global_int_reg))
      WUFEI_HwEventReadMachineEventsPerCore(ufeid, WUFE_CORE_ID_1, machine_events, &event_reg_rx_core1, &event_reg_tx_core1);

   machine_events->rx_event_reg = event_reg_rx_core0 | event_reg_rx_core1;
   machine_events->tx_event_reg = event_reg_tx_core0 | event_reg_tx_core1;

   return;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventReadFmEvents
 *
 * Purpose: Reads the unmasked Fm events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     fm_events      pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDispatch
 *
 ****************************************************************************/
static void WUFEI_HwEventReadFmEvents(WP_U32 ufeid,
                                      WUFE_events_fm* fm_events)
{
   WP_U16 event_mask_reg, event_reg_core0 =0, event_reg_core1 =0;
   WP_U16 global_int_reg = WUFEI_UfeCoreGlobalIntGet(ufeid);

   if (WUFEI_UFE_GLOBAL_INT_FM0_V(global_int_reg))
   {
      WUFEI_HwFmEventRegGet(ufeid, WUFE_CORE_ID_0, &event_reg_core0);
      WUFEI_HwFmEventMaskRegGet(ufeid, WUFE_CORE_ID_0, &event_mask_reg);

      /*Fill the structure with the unmasked events*/
      event_reg_core0 = event_reg_core0 & ~event_mask_reg;

#if 0
      /*Display any info if available on the ports that caused the interrupt*/
      WUFEI_HwFmInterruptReadRxDisplay(ufeid,WUFE_CORE_ID_0);
      WUFEI_HwFmInterruptReadTxDisplay(ufeid,WUFE_CORE_ID_0);
#endif

      /*clear interrupt register */
      WUFEI_HwFmEventRegSet(ufeid, WUFE_CORE_ID_0, event_reg_core0);
      fm_events->info[WUFE_CORE_ID_0] = 0;
      if (event_reg_core0 & WUFE_EVENT_FM_RX_VFP_OVERRUN)
      {
         fm_events->info[WUFE_CORE_ID_0] = WUFEI_HwFmVfpInterruptInfoGet(ufeid, WUFE_CORE_ID_0);
      }
   }

   if (WUFEI_UFE_GLOBAL_INT_FM1_V(global_int_reg))
   {
      WUFEI_HwFmEventRegGet(ufeid, WUFE_CORE_ID_1, &event_reg_core1);
      WUFEI_HwFmEventMaskRegGet(ufeid, WUFE_CORE_ID_1, &event_mask_reg);

      /*Fill the structure with the unmasked events*/
      event_reg_core1 = event_reg_core1 & ~event_mask_reg;

#if 0
      /*Display any info if available on the ports that caused the interrupt*/
      WUFEI_HwFmInterruptReadRxDisplay(ufeid,WUFE_CORE_ID_1);
      WUFEI_HwFmInterruptReadTxDisplay(ufeid,WUFE_CORE_ID_1);
#endif

      /*clear interrupt register */
      WUFEI_HwFmEventRegSet(ufeid, WUFE_CORE_ID_1, event_reg_core1);

      fm_events->info[WUFE_CORE_ID_1] = 0;
      if (event_reg_core1 & WUFE_EVENT_FM_RX_VFP_OVERRUN)
      {
         fm_events->info[WUFE_CORE_ID_1] = WUFEI_HwFmVfpInterruptInfoGet(ufeid, WUFE_CORE_ID_1);
      }

   }

   fm_events->event_reg = event_reg_core0 | event_reg_core1;

   return;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventDispatch
 *
 * Purpose: Reads the unmasked UFE events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     events          pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_UfeReadEvents
 *
 ****************************************************************************/
void WUFEI_HwEventDispatch(WP_U32 ufeid,
                           WUFEI_system_registry* system_registry,
                           WUFE_events *events)
{
   /* read UFE interrupt register and interrupt mask register */
   WP_U16 global_int_reg;
   WP_U16 bs_int_reg;

   /*reads the global interrupt register*/
   global_int_reg = WUFEI_UfeCoreGlobalIntGet(ufeid);

   /* check if tdm core0 or tdm core1 events are set in the global register */
   if (WUFEI_UFE_GLOBAL_INT_TDM0_V(global_int_reg) || WUFEI_UFE_GLOBAL_INT_TDM1_V(global_int_reg))
      WUFEI_HwEventReadTdmEvents(ufeid, &events->tdm_events);

   /* check if machine core0 or machine core1 events are set in the global register */
   if (WUFEI_UFE_GLOBAL_INT_MACHINE0_V(global_int_reg) || WUFEI_UFE_GLOBAL_INT_MACHINE1_V(global_int_reg))
      WUFEI_HwEventReadMachineEvents(ufeid, &events->machine_events);

   /* check if fm core0 or fm core1 events are set in the global register */
   if (WUFEI_UFE_GLOBAL_INT_FM0_V(global_int_reg) || WUFEI_UFE_GLOBAL_INT_FM1_V(global_int_reg))
      WUFEI_HwEventReadFmEvents(ufeid, &events->fm_events);

   /* check if EMPHY events are set in the global register */
   if(system_registry->hw_config.emphy_setup_mode != WUFEI_EMPHY_SETUP_MODE_DUAL)
   {
      /* all EMPHY events are located in the EMPHY core 0 interrupt register */
      if(WUFEI_UFE_GLOBAL_INT_EMPHY0_V(global_int_reg))
         WUFEI_HwEventReadEmphyEvents(ufeid, WUFE_CORE_ID_0,
                                      &events->emphy_events[WUFE_CORE_ID_0]);
   }
   else  /* EMPHY DUAL mode */
   {
      /* in this mode the EMPHY_CORE0 and EMPHY_CORE1 bits should be checked */
      if(WUFEI_UFE_GLOBAL_INT_EMPHY0_V(global_int_reg))
         WUFEI_HwEventReadEmphyEvents(ufeid, WUFE_CORE_ID_0,
                                      &events->emphy_events[WUFE_CORE_ID_0]);
      if(WUFEI_UFE_GLOBAL_INT_EMPHY1_V(global_int_reg))
         WUFEI_HwEventReadEmphyEvents(ufeid, WUFE_CORE_ID_1,
                                      &events->emphy_events[WUFE_CORE_ID_1]);
   }

   /* check if CR events is set in the global register */
   if (WUFEI_UFE_GLOBAL_MASK_INT_CR_V(global_int_reg))
      WUFEI_HwClockRecEventsDispatch(ufeid, &events->cr_events);

   /* clear the interrupt register */
   WUFEI_UfeCoreGlobalIntSet(ufeid, global_int_reg);

   if(!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      /*reads the bs interrupt register*/
      WUFEI_HwBsEventRegGet(ufeid, &bs_int_reg);

      /* check if events are set in the bs register */
      if (WUFEI_BS_INTERRUPT_BS_PLL_V(bs_int_reg))
         WUFEI_HwEventReadBsPllEvents(ufeid, &events->bs_events);

      if (WUFEI_BS_INTERRUPT_SFP_V(bs_int_reg))
         WUFEI_HwEventReadBsSfpEvents(ufeid, &events->bs_events);

      /* clear the interrupt register */
      WUFEI_HwBsEventRegSet(ufeid, bs_int_reg);
   }
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventEmphyDisable
 *
 * Purpose: Disables the Emphy events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     emphy_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventEmphyDisable(WP_U32 ufeid,
                                      WP_U32 coreid,
                                      WUFE_events_emphy* emphy_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwEmphyEventMaskRegGet(ufeid, coreid, &event_mask_reg);

   event_mask_reg = event_mask_reg | emphy_events->event_reg;
   WUFEI_HwEmphyEventMaskRegSet(ufeid, coreid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventBsDisable
 *
 * Purpose: Disables the Bs events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     bs_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventBsDisable(WP_U32 ufeid,
                                   WUFE_events_bs* bs_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwBsLockLostRegGet(ufeid, &event_mask_reg);
   event_mask_reg = event_mask_reg | bs_events->pll_lost_lock_event_reg;
   WUFEI_HwBsLockLostRegSet(ufeid, event_mask_reg);

   WUFEI_HwBsSfpAbsentRegGet(ufeid, &event_mask_reg);
   event_mask_reg = event_mask_reg | bs_events->absent_event_reg;
   WUFEI_HwBsSfpAbsentRegSet(ufeid, event_mask_reg);

   WUFEI_HwBsSfpLosRegGet(ufeid, &event_mask_reg);
   event_mask_reg = event_mask_reg | bs_events->los_event_reg;
   WUFEI_HwBsSfpLosRegSet(ufeid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventTdmDisable
 *
 * Purpose: Disables the Tdm events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     tdm_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventTdmDisable(WP_U32 ufeid,
                                    WP_U32 coreid,
                                    WUFE_events_tdm* tdm_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwTdmEventMaskRegGet(ufeid, coreid, &event_mask_reg);

   event_mask_reg = event_mask_reg | tdm_events->event_reg;
   WUFEI_HwTdmEventMaskRegSet(ufeid, coreid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventMachineDisable
 *
 * Purpose: Disables the Machine events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     machine_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventMachineDisable(WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WUFE_events_machine* machine_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwMachineEventMaskRegGet(ufeid, coreid, &event_mask_reg, WUFE_RX_DIRECTION);

   event_mask_reg = event_mask_reg | machine_events->rx_event_reg;
   WUFEI_HwMachineEventMaskRegSet(ufeid, coreid, event_mask_reg, WUFE_RX_DIRECTION);

   WUFEI_HwMachineEventMaskRegGet(ufeid, coreid, &event_mask_reg, WUFE_TX_DIRECTION);

   event_mask_reg = event_mask_reg | machine_events->tx_event_reg;
   WUFEI_HwMachineEventMaskRegSet(ufeid, coreid, event_mask_reg, WUFE_TX_DIRECTION);

}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventFmDisable
 *
 * Purpose: Disables the Fm events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     fm_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventFmDisable(WP_U32 ufeid,
                                   WP_U32 coreid,
                                   WUFE_events_fm* fm_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwFmEventMaskRegGet(ufeid, coreid, &event_mask_reg);

   event_mask_reg = event_mask_reg | fm_events->event_reg;
   WUFEI_HwFmEventMaskRegSet(ufeid, coreid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwBsEnable
 *
 * Purpose: Enables the Bs events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     bs_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventEnable
 *
 ****************************************************************************/
static void WUFEI_HwEventBsEnable(WP_U32 ufeid,
                                  WUFE_events_bs* bs_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwBsLockLostRegGet(ufeid, &event_mask_reg);
   event_mask_reg = event_mask_reg & ~bs_events->pll_lost_lock_event_reg;
   WUFEI_HwBsLockLostRegSet(ufeid, event_mask_reg);

   WUFEI_HwBsSfpAbsentRegGet(ufeid, &event_mask_reg);
   event_mask_reg = event_mask_reg & ~bs_events->absent_event_reg;
   WUFEI_HwBsSfpAbsentRegSet(ufeid, event_mask_reg);

   WUFEI_HwBsSfpLosRegGet(ufeid, &event_mask_reg);
   event_mask_reg = event_mask_reg & ~bs_events->los_event_reg;
   WUFEI_HwBsSfpLosRegSet(ufeid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventEmphyEnable
 *
 * Purpose: Enables the Emphy events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     emphy_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventEnable
 *
 ****************************************************************************/
static void WUFEI_HwEventEmphyEnable(WP_U32 ufeid,
                                     WP_U32 coreid,
                                     WUFE_events_emphy* emphy_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwEmphyEventMaskRegGet(ufeid, coreid, &event_mask_reg);

   event_mask_reg = event_mask_reg & ~emphy_events->event_reg;
   WUFEI_HwEmphyEventMaskRegSet(ufeid, coreid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventTdmEnable
 *
 * Purpose: Enables the Tdm events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     tdm_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventEnable
 *
 ****************************************************************************/
static void WUFEI_HwEventTdmEnable(WP_U32 ufeid,
                                   WP_U32 coreid,
                                   WUFE_events_tdm* tdm_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwTdmEventMaskRegGet(ufeid, coreid, &event_mask_reg);

   event_mask_reg = event_mask_reg & ~tdm_events->event_reg;
   WUFEI_HwTdmEventMaskRegSet(ufeid, coreid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventMachineEnable
 *
 * Purpose: Enables the Machine events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     machine_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventEnable
 *
 ****************************************************************************/
static void WUFEI_HwEventMachineEnable(WP_U32 ufeid,
                                       WP_U32 coreid,
                                       WUFE_events_machine* machine_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwMachineEventMaskRegGet(ufeid, coreid, &event_mask_reg, WUFE_RX_DIRECTION);

   event_mask_reg = event_mask_reg & ~machine_events->rx_event_reg;
   WUFEI_HwMachineEventMaskRegSet(ufeid, coreid, event_mask_reg, WUFE_RX_DIRECTION);

   WUFEI_HwMachineEventMaskRegGet(ufeid, coreid, &event_mask_reg, WUFE_TX_DIRECTION);

   event_mask_reg = event_mask_reg & ~machine_events->tx_event_reg;
   WUFEI_HwMachineEventMaskRegSet(ufeid, coreid, event_mask_reg, WUFE_TX_DIRECTION);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventFmEnable
 *
 * Purpose: Enables the Fm events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     fm_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventEnable
 *
 ****************************************************************************/
static void WUFEI_HwEventFmEnable(WP_U32 ufeid,
                                  WP_U32 coreid,
                                  WUFE_events_fm* fm_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwFmEventMaskRegGet(ufeid, coreid, &event_mask_reg);

   event_mask_reg = event_mask_reg & ~fm_events->event_reg;
   WUFEI_HwFmEventMaskRegSet(ufeid, coreid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventBsSetup
 *
 * Purpose: Sets default values to the BS events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventEnable
 *
 ****************************************************************************/
static void WUFEI_HwEventBsSetup(WP_U32 ufeid)
{
   WUFEI_HwBsLockLostMaskRegSet(ufeid, 0xffff);
   WUFEI_HwBsSfpAbsentMaskRegSet(ufeid, 0xffff);
   WUFEI_HwBsSfpLosMaskRegSet(ufeid, 0xffff);
   WUFEI_HwBsLockLostRegSet(ufeid, 0xffff);
   WUFEI_HwBsSfpAbsentRegSet(ufeid, 0xffff);
   WUFEI_HwBsSfpLosRegSet(ufeid, 0xffff);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventEmphySetup
 *
 * Purpose: Sets default values to the Emphy events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     emphy_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventSetup
 *
 ****************************************************************************/
static void WUFEI_HwEventEmphySetup(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_HwEmphyEventMaskRegSet(ufeid, coreid, 0xffff);
   WUFEI_HwEmphyEventRegSet(ufeid, coreid, 0xffff);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventTdmSetup
 *
 * Purpose: Sets default values to the Tdm events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     tdm_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventSetup
 *
 ****************************************************************************/
static void WUFEI_HwEventTdmSetup(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_HwTdmEventMaskRegSet(ufeid, coreid, 0xffff);
   WUFEI_HwTdmEventRegSet(ufeid, coreid, 0xffff);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventMachineSetup
 *
 * Purpose: Sets default values to the Machine events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     machine_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventSetup
 *
 ****************************************************************************/
static void WUFEI_HwEventMachineSetup(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_HwMachineEventMaskRegSet(ufeid, coreid, 0xffff, WUFE_RX_DIRECTION);
   WUFEI_HwMachineEventMaskRegSet(ufeid, coreid, 0xffff,WUFE_TX_DIRECTION );
   WUFEI_HwMachineEventRegSet(ufeid, coreid, 0xffff, WUFE_RX_DIRECTION);
   WUFEI_HwMachineEventRegSet(ufeid, coreid, 0xffff, WUFE_TX_DIRECTION);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventFmSetup
 *
 * Purpose: Sets default values to the Fm events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     fm_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventSetup
 *
 ****************************************************************************/
static void WUFEI_HwEventFmSetup(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_HwFmEventMaskRegSet(ufeid, coreid, 0xffff);
   WUFEI_HwFmEventRegSet(ufeid, coreid, 0xffff);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventSetup
 *
 * Purpose: Set the reset values to the UFE event register
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_SystemCoreSetup
 *
 ****************************************************************************/
WUFE_status WUFEI_HwEventSetup(WP_U32 ufeid)
{
   WP_U32 coreid;
   WUFEI_system_registry *system_registry;

   system_registry = WUFEI_SystemRegistryGet(ufeid);
   if(!system_registry)
   {
      return WUFE_ERR_INVALID_REGISTRY_STATE;
   }
   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         WUFEI_HwEventEmphySetup(ufeid, coreid);
         WUFEI_HwEventTdmSetup(ufeid, coreid);
         WUFEI_HwEventMachineSetup(ufeid, coreid);
         WUFEI_HwEventFmSetup(ufeid, coreid);
      }
   }

   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_HwEventBsSetup(ufeid);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventDisable
 *
 * Purpose: Disables the UFE events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     events          disable event mask
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFE_UfeEventDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwEventDisable(WP_U32 ufeid, WUFE_events *events)
{
   WP_U32 coreid;

   for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid ++)
   {
      WUFE_events_emphy* emphy_events = &events->emphy_events[coreid];
      WUFE_events_tdm* tdm_events = &events->tdm_events;
      WUFE_events_machine* machine_events = &events->machine_events;
      WUFE_events_fm* fm_events = &events->fm_events;

      if(emphy_events->event_reg)
         WUFEI_HwEventEmphyDisable(ufeid, coreid, emphy_events);

      if(tdm_events->event_reg)
         WUFEI_HwEventTdmDisable(ufeid, coreid, tdm_events);

      if(machine_events->rx_event_reg || machine_events->tx_event_reg)
         WUFEI_HwEventMachineDisable(ufeid, coreid, machine_events);

      if(fm_events->event_reg)
         WUFEI_HwEventFmDisable(ufeid, coreid, fm_events);
   }

   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFE_events_bs* bs_events = &events->bs_events;
      WUFEI_HwEventBsDisable(ufeid, bs_events);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventEnable
 *
 * Purpose: Enables the UFE events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     events          enable event mask
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFE_UfeEventEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwEventEnable(WP_U32 ufeid, WUFE_events *events)
{
   WP_U32 coreid;

   for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid ++)
   {
      WUFE_events_emphy* emphy_events =  &events->emphy_events[coreid];
      WUFE_events_tdm* tdm_events = &events->tdm_events;
      WUFE_events_machine* machine_events = &events->machine_events;
      WUFE_events_fm* fm_events = &events->fm_events;

      if(emphy_events->event_reg)
         WUFEI_HwEventEmphyEnable(ufeid, coreid, emphy_events);

      if(tdm_events->event_reg)
         WUFEI_HwEventTdmEnable(ufeid, coreid, tdm_events);

      if(machine_events->rx_event_reg || machine_events->tx_event_reg)
         WUFEI_HwEventMachineEnable(ufeid, coreid, machine_events);

      if(fm_events->event_reg)
         WUFEI_HwEventFmEnable(ufeid, coreid, fm_events);
   }

   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFE_events_bs* bs_events = &events->bs_events;
      WUFEI_HwEventBsEnable(ufeid, bs_events);
   }

   return WUFE_OK;
}
