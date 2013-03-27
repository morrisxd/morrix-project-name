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

#ifndef WUFEI_HW_SBI_H
#include "wufei_hw_sbi.h"
#endif

#ifndef WUFEI_HW_CAD_H
#include "wufei_hw_cad.h"
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

#ifndef WUFEI_HW_CORE_INT_H
#include "wufei_hw_core_int.h"
#endif

#ifndef WUFEI_EVENTS_INT_H
#include "wufei_events_int.h"
#endif

#ifndef WUFEI_HW_EMPHY_INT_H
#include "wufei_hw_emphy_int.h"
#endif

#ifndef WUFEI_HW_SBI_INT_H
#include "wufei_hw_sbi_int.h"
#endif

#ifndef WUFEI_HW_CAD_INT_H
#include "wufei_hw_cad_int.h"
#endif

#ifndef WUFEI_HW_CLK_REC_INT_H
#include "wufei_hw_clk_rec_int.h"
#endif

#ifndef WUFEI_HW_CLK_TRANSLATOR_INT_H
#include "wufei_hw_clk_translator_int.h"
#endif

static void WUFEI_SbiClockTranslatorEvents(WP_U32 ufeid, WUFE_events_clock_recovery *cr_events);

/*****************************************************************************
 *
 * Function:  WUFEI_HwEventReadCoreEvents
 *
 * Purpose: Reads the unmasked CORE events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     core_events     pointer to a structure to fill in
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
static void WUFEI_HwEventReadCoreEvents(WP_U32 ufeid, WP_U32 coreid,
                                        WUFE_events_core *core_events)
{

   WUFEI_hw_events* ufe_event_reg = WUFEI_EVENT;
   WP_U16 event_mask_reg=0, event_reg=0, phy_id=0;
   WP_U32 id  = WUFEI_ID(ufeid, coreid);

   WUFEI_MEM_16_GET(id, &(ufe_event_reg->event_reg), &event_reg);
   WUFEI_MEM_16_GET(id, &(ufe_event_reg->event_mask_reg), &event_mask_reg);

   core_events->event_reg = event_reg & ~event_mask_reg;

   if (core_events->event_reg & WUFE_EVENT_CORE_EGRESS_PHY_UNDERRUN)
   {
      WUFEI_MEM_16_GET(id, &(ufe_event_reg->event_phy_underrun_tx), &phy_id);
      core_events->tx_phy_underrun_handle = (WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_PHY)|
                                             WUFEI_HANDLE_UFE_ID_F(ufeid) |
                                             WUFEI_HANDLE_UFE_CORE_ID_F(coreid)|
                                             WUFEI_HANDLE_INDEX_F(phy_id) );
   }

   if (core_events->event_reg & WUFE_EVENT_CORE_INGRESS_PHY_CR_UNDERRUN)
   {
      WUFEI_MEM_16_GET(id, &(ufe_event_reg->event_phy_underrun_rx), &phy_id);
      core_events->rx_phy_underrun_handle = (WUFEI_HANDLE_TYPE_F(WUFE_HANDLE_TYPE_PHY) |
                                             WUFEI_HANDLE_UFE_ID_F(ufeid) |
                                             WUFEI_HANDLE_UFE_CORE_ID_F(WUFE_CORE_ID_0)|
                                             WUFEI_HANDLE_INDEX_F(phy_id) );

   }

   /*clear interrupt register */
   WUFEI_MEM_16_SET(id, &(ufe_event_reg->event_reg), event_reg);

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEventMapSbiEvents
 *
 * Purpose: Maps the SBI events from Hw register to user struct or vice versa.
 *
 *
 * Description:
 *     Gets the HW event register as parameter (masked or unmasked).
 *     Maps HW events to the way they appear in the API.
 *
 * Inputs:
 *     ufeid           UFE id
 *     hw_event_reg    pointer to SBI event register as it appears in HW.
 *     sbi_events      pointer to a structure to fill in
 *     hw_to_api       determines whether HW events should be mapped to API structures
 *                     or vice versa
 * Outputs:
 *     maps between HW and API events format.
 *
 * Return Value:
 *
 * Called by:
 *     WUFEI_HwEventReadSbiEvents
 *
 ****************************************************************************/
static void WUFEI_HwEventMapSbiEvents(WP_U16 *hw_event_reg,
                                      WUFE_events_sbi* sbi_events,
                                      WP_U32 hw_to_api)
{
   if (hw_to_api == WUFE_TRUE)
   {
      sbi_events->event_reg =
         ((((*hw_event_reg & WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE0)
            >> WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE0_S) <<
           WUFEI_EVENT_SBI_V5_SYNC_LOSS_CORE0_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE1)
            >> WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE1_S) <<
           WUFEI_EVENT_SBI_V5_SYNC_LOSS_CORE1_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE0)
            >> WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE0_S) <<
           WUFEI_EVENT_SBI_RX_SYNC_LOSS_CORE0_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE1)
            >> WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE1_S) <<
           WUFEI_EVENT_SBI_RX_SYNC_LOSS_CORE1_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE0)
            >> WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE0_S) <<
           WUFEI_EVENT_SBI_PARITY_ERROR_CORE0_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE1)
            >> WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE1_S) <<
           WUFEI_EVENT_SBI_PARITY_ERROR_CORE1_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0)
            >> WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0_S) <<
           WUFEI_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1)
            >> WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1_S) <<
           WUFEI_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE0)
            >> WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE0_S) <<
           WUFEI_EVENT_SBI_RX_ALM_INDICATOR_CORE0_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE1)
            >> WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE1_S) <<
           WUFEI_EVENT_SBI_RX_ALM_INDICATOR_CORE1_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0)
            >> WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0_S) <<
           WUFEI_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1)
            >> WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1_S) <<
           WUFEI_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0)
            >> WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0_S) <<
           WUFEI_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1)
            >> WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1_S) <<
           WUFEI_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1_S) |
          (((*hw_event_reg & WUFEI_HW_EVENT_SBI_TX_AC1FP_SYNC_LOSS)
            >> WUFEI_HW_EVENT_SBI_TX_AC1FP_SYNC_LOSS_S) <<
           WUFEI_EVENT_SBI_TX_AC1FP_SYNC_LOSS_S));
   }
   else
   {
      *hw_event_reg =
         ((((sbi_events->event_reg & WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE0)
            >> WUFEI_EVENT_SBI_V5_SYNC_LOSS_CORE0_S) <<
           WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE0_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE1)
            >> WUFEI_EVENT_SBI_V5_SYNC_LOSS_CORE1_S) <<
           WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE1_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE0)
            >> WUFEI_EVENT_SBI_RX_SYNC_LOSS_CORE0_S) <<
           WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE0_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE1)
            >> WUFEI_EVENT_SBI_RX_SYNC_LOSS_CORE1_S) <<
           WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE1_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_PARITY_ERROR_CORE0)
            >> WUFEI_EVENT_SBI_PARITY_ERROR_CORE0_S) <<
           WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE0_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_PARITY_ERROR_CORE1)
            >> WUFEI_EVENT_SBI_PARITY_ERROR_CORE1_S) <<
           WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE1_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0)
            >> WUFEI_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0_S) <<
           WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1)
            >> WUFEI_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1_S) <<
           WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE0)
            >> WUFEI_EVENT_SBI_RX_ALM_INDICATOR_CORE0_S) <<
           WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE0_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE1)
            >> WUFEI_EVENT_SBI_RX_ALM_INDICATOR_CORE1_S) <<
           WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE1_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0)
            >> WUFEI_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0_S) <<
           WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1)
            >> WUFEI_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1_S) <<
           WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0)
            >> WUFEI_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0_S) <<
           WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1)
            >> WUFEI_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1_S) <<
           WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1_S) |
          (((sbi_events->event_reg & WUFE_EVENT_SBI_TX_AC1FP_SYNC_LOSS)
            >> WUFEI_EVENT_SBI_TX_AC1FP_SYNC_LOSS_S) <<
           WUFEI_HW_EVENT_SBI_TX_AC1FP_SYNC_LOSS_S));
   }

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEventReadSbiEvents
 *
 * Purpose: Reads the unmasked SBI events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     sbi_events     pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventReadLiuEvents
 *
 ****************************************************************************/
static void WUFEI_HwEventReadSbiEvents(WP_U32 ufeid,
                                       WUFE_events_sbi* sbi_events)
{
   WP_U16 event_mask_reg, event_reg, masked_event_reg;

   /* get event register from HW */
   WUFEI_HwSbiEventRegGet(ufeid, &event_reg);

   /* get mask register */
   WUFEI_HwSbiEventMaskRegGet(ufeid, &event_mask_reg);

   /* mask the event register */
   masked_event_reg = event_reg & ~event_mask_reg;

   /* map the raised events to the API structures */
   WUFEI_HwEventMapSbiEvents(&masked_event_reg, sbi_events, WUFE_TRUE);

   /* clear event register */
   WUFEI_HwSbiEventRegSet(ufeid, event_reg);

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEventReadCadEvents
 *
 * Purpose: Reads the unmasked CAD events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid          UFE id
 *     coreid         the core id
 *     liu_events     pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventReadLiuEvents
 *
 ****************************************************************************/
static void WUFEI_HwEventReadCadEvents(WP_U32 ufeid,
                                       WP_U32 coreid,
                                       WUFE_events_liu* liu_events)
{
   WP_U16 event_reg_rx, event_reg_tx;
   WP_U32 i, j, cad_num, shifter, events_per_core, cads_per_reg;
   WUFE_events_cad* cad_events;

   events_per_core=WUFE_CAD_EVENTS/2;
   cads_per_reg = events_per_core/WUFEI_CAD_INT_NUM;

   for (i = events_per_core*coreid; i<events_per_core*(coreid+1); i+=cads_per_reg)
   {

      /* get event registers from HW */
      event_reg_rx = WUFEI_HwCadIntGet(ufeid, WUFEI_CAD_RX, i);
      event_reg_tx = WUFEI_HwCadIntGet(ufeid, WUFEI_CAD_TX, i);

      for (j=0;j<cads_per_reg;j++)
      {
         cad_num=i+j;
         /* map the raised events to the API structures */
         cad_events = &liu_events->cad_events[cad_num];
         shifter = 2*j;
         if (event_reg_rx & (WUFE_EVENT_CAD_RX_OVERRUN<<shifter))
            cad_events->rx_event_reg |= WUFE_EVENT_CAD_RX_OVERRUN;
         if (event_reg_rx & (WUFE_EVENT_CAD_RX_SYNC_LOSS<<shifter))
            cad_events->rx_event_reg |= WUFE_EVENT_CAD_RX_SYNC_LOSS;
         if (event_reg_tx & (WUFE_EVENT_CAD_TX_UNDERRUN<<shifter))
            cad_events->tx_event_reg |= WUFE_EVENT_CAD_TX_UNDERRUN;
         if (event_reg_tx & (WUFE_EVENT_CAD_TX_SYNC_LOSS<<shifter))
            cad_events->tx_event_reg |= WUFE_EVENT_CAD_TX_SYNC_LOSS;
      }

      /* clear event register */
      WUFEI_HwCadIntSet(ufeid, WUFEI_CAD_RX, i, event_reg_rx);
      WUFEI_HwCadIntSet(ufeid, WUFEI_CAD_TX, i, event_reg_tx);

   }

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEventReadLiuEvents
 *
 * Purpose: Reads the unmasked LIU events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
 *     liu_events      pointer to a structure to fill in
 *     sbi_flag        indicates whether it is sbi or cad.
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
static void WUFEI_HwEventReadLiuEvents(WP_U32 ufeid,
                                       WUFE_events_liu* liu_events,
                                       WP_U16 sbi_flag)
{
   if (sbi_flag == WUFE_TRUE)
   {
      WUFE_events_sbi* sbi_events = &liu_events->sbi_events[WUFEI_SBI_ID_0];

      WUFEI_HwEventReadSbiEvents(ufeid, sbi_events);
   }
   else /*CAD Events*/
   {
      WP_U16 global_cad_int_reg;

      /*reads the global interrupt register for cad events*/
      global_cad_int_reg = WUFEI_HwCadGlobalEventGet(ufeid);

      /* check if core0 or core1 events are set in the global register */
      if (WUFEI_CAD_GLOBAL_INT_RX_CORE0_V(global_cad_int_reg) |
          WUFEI_CAD_GLOBAL_INT_TX_CORE0_V(global_cad_int_reg))
      {
         WUFEI_HwEventReadCadEvents(ufeid, WUFE_CORE_ID_0, liu_events);
      }
      if (WUFEI_CAD_GLOBAL_INT_RX_CORE1_V(global_cad_int_reg) |
          WUFEI_CAD_GLOBAL_INT_TX_CORE1_V(global_cad_int_reg))
      {
         WUFEI_HwEventReadCadEvents(ufeid, WUFE_CORE_ID_1, liu_events);
      }
   }

   return;
}

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
 * Function:  WUFEI_HwEventReadCrEvents
 *
 * Purpose: Reads the unmasked Clock Recovery events
 *
 *
 * Description:
 *     Reads the unmasked events from the event register. Clears the event
 *     register afterwards
 *
 * Inputs:
 *     ufeid           UFE id
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
 *     WUFEI_HwEventDispatch
 *
 ****************************************************************************/
static void WUFEI_HwEventReadCrEvents(WP_U32 ufeid,
                                      WUFE_events* events)
{
   WP_U16 event_mask_reg, event_reg, raised_events;

   WUFEI_HwClockRecEventRegGet(ufeid, &event_reg);
   WUFEI_HwClockRecEventMaskRegGet(ufeid, &event_mask_reg);

   raised_events = event_reg & ~event_mask_reg;

   /* copy raised_event to user structure */
   events->cr_events.event_reg = raised_events;

   if (raised_events & WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW)
   {
      WUFEI_SbiClockTranslatorEvents(ufeid, &events->cr_events);
   }

   /* clear interrupt register */
   WUFEI_HwClockRecEventRegSet(ufeid, event_reg);

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
   /* read UFE2 interrupt register and interrupt mask register */
   WP_U16 global_int_reg,sbi_flag;

   /*reads the global interrupt register*/
   global_int_reg = WUFEI_UfeCoreGlobalIntGet(ufeid);

   /* check if core0 or core1 events are set in the global register */
   if (WUFEI_UFE_GLOBAL_INT_CORE0_V(global_int_reg))
      WUFEI_HwEventReadCoreEvents(ufeid, WUFE_CORE_ID_0, &events->core_events[WUFE_CORE_ID_0]);

   if (WUFEI_UFE_GLOBAL_INT_CORE1_V(global_int_reg))
      WUFEI_HwEventReadCoreEvents(ufeid, WUFE_CORE_ID_1, &events->core_events[WUFE_CORE_ID_1]);

   /* check if EMPHY events are set in the global register */
   if(system_registry->hw_config.emphy_setup_mode == WUFEI_EMPHY_SETUP_MODE_MUX)
   {
      /* all EMPHY events are located in the EMPHY core 0 interrupt register */
      if(WUFEI_UFE_GLOBAL_INT_EMPHY_CORE0_V(global_int_reg))
         WUFEI_HwEventReadEmphyEvents(ufeid, WUFE_CORE_ID_0,
                                      &events->emphy_events[WUFE_CORE_ID_0]);
   }
   else  /* EMPHY DUAL mode */
   {
      /* in this mode the EMPHY_CORE0 and EMPHY_CORE1 bits should be checked */
      if(WUFEI_UFE_GLOBAL_INT_EMPHY_CORE0_V(global_int_reg))
         WUFEI_HwEventReadEmphyEvents(ufeid, WUFE_CORE_ID_0,
                                      &events->emphy_events[WUFE_CORE_ID_0]);
      if(WUFEI_UFE_GLOBAL_INT_EMPHY_CORE1_V(global_int_reg))
         WUFEI_HwEventReadEmphyEvents(ufeid, WUFE_CORE_ID_1,
                                      &events->emphy_events[WUFE_CORE_ID_1]);
   }

   sbi_flag = WUFEI_SystemHwConfigLiuSbi(system_registry);

   /* check if LIU events are set in the global register */
   if (WUFEI_UFE_GLOBAL_INT_LIU_V(global_int_reg))
      WUFEI_HwEventReadLiuEvents(ufeid, &events->liu_events,sbi_flag);

   /* check if CR events are set in the global register */
   if (WUFEI_UFE_GLOBAL_INT_CR_V(global_int_reg))
      WUFEI_HwEventReadCrEvents(ufeid, events);

   /* clear the interrupt register */
   WUFEI_UfeCoreGlobalIntSet(ufeid, global_int_reg);
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
 * Function: WUFEI_HwEventSbiDisable
 *
 * Purpose: Disables the SBI events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     sbi_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventLiuDisable
 *
 ****************************************************************************/
void WUFEI_HwEventSbiDisable(WP_U32 ufeid, WP_U16 sbi_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwSbiEventMaskRegGet(ufeid, &event_mask_reg);

   event_mask_reg = event_mask_reg | sbi_events;
   WUFEI_HwSbiEventMaskRegSet(ufeid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventCoreDisable
 *
 * Purpose: Disables the SBI events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     sbi_events    pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventCoreDisable(WP_U32 ufeid,
                                     WP_U32 coreid,
                                     WUFE_events_core* core_events)
{
   WUFEI_EventDisable(ufeid, coreid, core_events->event_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventCrDisable
 *
 * Purpose: Disables the CR events according to a given mask
 *
 * Description:
 *
 * Inputs:
 *     ufeid     UFE id
 *     events    event mask that should be disabled
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventCrDisable(WP_U32 ufeid,
                                   WP_U16 cr_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwClockRecEventMaskRegGet(ufeid, &event_mask_reg);

   event_mask_reg = event_mask_reg | cr_events;

   if (cr_events & WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW)
   {
      WP_U16 ct_event_mask_arr[WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS];
      WP_U32 i;

      for (i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
         ct_event_mask_arr[i] = 0xffff;

      WUFEI_HwSbiClockTranslatorEventMaskSet(ufeid, ct_event_mask_arr);
   }

   WUFEI_HwClockRecEventMaskRegSet(ufeid, event_mask_reg);
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
 * Function: WUFEI_HwEventSbiEnable
 *
 * Purpose: Enables the SBI events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     sbi_events      pointer to a structure to fill in
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
void WUFEI_HwEventSbiEnable(WP_U32 ufeid, WP_U16 sbi_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwSbiEventMaskRegGet(ufeid, &event_mask_reg);

   event_mask_reg = event_mask_reg & ~sbi_events;

   WUFEI_HwSbiEventMaskRegSet(ufeid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventSbiSetup
 *
 * Purpose: Sets default values to the SBI events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     sbi_events      pointer to a structure to fill in
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
static void WUFEI_HwEventSbiSetup(WP_U32 ufeid)
{
   WUFEI_HwSbiEventMaskRegSet(ufeid, 0xffff);
   WUFEI_HwSbiEventRegSet(ufeid, 0xffff);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventCadSetup
 *
 * Purpose: Sets default values to the CAD events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     sbi_events      pointer to a structure to fill in
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
static void WUFEI_HwEventCadSetup(WP_U32 ufeid, WP_U32 coreid)
{
   WP_U32 i, events_per_core, cads_per_reg;

   WUFEI_HwCadGlobalEventSet(ufeid, 0xffff);
   WUFEI_HwCadGlobalEventMaskSet(ufeid, 0xffff);

   events_per_core=WUFE_CAD_EVENTS/2;
   cads_per_reg = events_per_core/WUFEI_CAD_INT_NUM;

   for (i = events_per_core*coreid; i<events_per_core*(coreid+1); i+=cads_per_reg)
   {
      WUFEI_HwCadIntSet(ufeid, WUFEI_CAD_RX, i, 0xffff);
      WUFEI_HwCadIntSet(ufeid, WUFEI_CAD_TX, i, 0xffff);
   }

}


/*****************************************************************************
 *
 * Function: WUFEI_HwEventCoreEnable
 *
 * Purpose: Enables the CORE events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     core_events     pointer to a structure to fill in
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
static void WUFEI_HwEventCoreEnable(WP_U32 ufeid,
                                    WP_U32 coreid,
                                    WUFE_events_core* core_events)
{
   WUFEI_EventEnable(ufeid, coreid, core_events->event_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventCoreSetup
 *
 * Purpose: Initializes the CORE events
 *
 * Description:
 *
 * Inputs:
 *     ufeid           UFE id
 *
 * Outputs:
 *
 * Return Value:
 *     none
 *
 * Called by:
 *    WUFEI_HwEventSetup
 *
 ****************************************************************************/
static void WUFEI_HwEventCoreSetup(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_events* ufe_event_reg = WUFEI_EVENT;
   WP_U32 id = WUFEI_ID(ufeid, coreid);

   /* write initial value for UFE3 event mask register */
   WUFEI_MEM_16_SET(id, &(ufe_event_reg->event_mask_reg), 0xffff);
   WUFEI_MEM_16_SET(id, &(ufe_event_reg->event_reg), 0xffff);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventLiuDisable
 *
 * Purpose: Disables the LIU events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     liu_events      pointer to a structure to fill in
 *
 * Outputs:
 *     reads the event register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *    WUFEI_HwEventDisable
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwEventLiuDisable(WP_U32 ufeid, WUFE_events_liu* liu_events)
{
   WUFE_events_sbi* sbi_events = &liu_events->sbi_events[WUFEI_SBI_ID_0];

   if (sbi_events->event_reg)
   {
      WP_U16 sbi_hw_event_reg;

      WUFEI_HwEventMapSbiEvents(&sbi_hw_event_reg,
                                sbi_events,
                                WUFE_FALSE);

      WUFEI_HwEventSbiDisable(ufeid, sbi_hw_event_reg);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventLiuEnable
 *
 * Purpose: Enables the LIU events according to a given mask
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid           UFE id
 *     coreid          Core id
 *     liu_events      pointer to a structure to fill in
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
static WUFE_status WUFEI_HwEventLiuEnable(WP_U32 ufeid, WUFE_events_liu* liu_events)
{
   WUFE_events_sbi* sbi_events = &liu_events->sbi_events[WUFEI_SBI_ID_0];

   if (sbi_events->event_reg)
   {
      WP_U16 sbi_hw_event_reg;

      WUFEI_HwEventMapSbiEvents(&sbi_hw_event_reg,
                                sbi_events,
                                WUFE_FALSE);

      WUFEI_HwEventSbiEnable(ufeid, sbi_hw_event_reg);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventCrEnable
 *
 * Purpose: Enables the CR events according to a given mask
 *
 * Description:
 *
 * Inputs:
 *     ufeid     UFE id
 *     events    event mask that should be disabled
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwEventDisable
 *
 ****************************************************************************/
static void WUFEI_HwEventCrEnable(WP_U32 ufeid,
                                  WP_U16 cr_events)
{
   WP_U16 event_mask_reg;

   WUFEI_HwClockRecEventMaskRegGet(ufeid, &event_mask_reg);

   event_mask_reg = event_mask_reg & ~cr_events;

   if (cr_events & WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW)
   {
      WP_U16 ct_event_mask_arr[WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS];
      WP_U32 i;

      for (i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
         ct_event_mask_arr[i] = 0;

      WUFEI_HwSbiClockTranslatorEventMaskSet(ufeid, ct_event_mask_arr);
   }

   WUFEI_HwClockRecEventMaskRegSet(ufeid, event_mask_reg);
}

/*****************************************************************************
 *
 * Function: WUFEI_HwEventCrSetup
 *
 * Purpose: Sets default values to the CR events
 *
 * Description:
 *
 * Inputs:
 *     ufeid       UFE id
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
static void WUFEI_HwEventCrSetup(WP_U32 ufeid)
{
   WP_U16 ct_event_arr[WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS];
   WP_U16 ct_event_mask_arr[WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS];
   WP_U32 i;

   for (i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
   {
      ct_event_arr[i] = 0xffff;
      ct_event_mask_arr[i] = 0xffff;
   }

   WUFEI_HwSbiClockTranslatorEventGet(ufeid, ct_event_arr);
   WUFEI_HwSbiClockTranslatorEventMaskGet(ufeid, ct_event_mask_arr);

   WUFEI_HwClockRecEventRegSet(ufeid, 0xffff);
   WUFEI_HwClockRecEventMaskRegSet(ufeid, 0xffff);
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
         WUFEI_HwEventCoreSetup(ufeid, coreid);
         WUFEI_HwEventEmphySetup(ufeid, coreid);
      }
   }

   if (WUFEI_SystemHwConfigLiuSbi(system_registry) == WUFE_TRUE)
      WUFEI_HwEventSbiSetup(ufeid);
   else
   {
      for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
      {
         if(system_registry->hw_config.core_enabled[coreid])
         {
            WUFEI_HwEventCadSetup(ufeid,coreid);
         }
      }
   }

   WUFEI_HwEventCrSetup(ufeid);

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
   WUFE_events_liu* liu_events = &events->liu_events;

   for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid ++)
   {
      WUFE_events_core* core_events =  &events->core_events[coreid];
      WUFE_events_emphy* emphy_events = &events->emphy_events[coreid];

      if(core_events->event_reg)
         WUFEI_HwEventCoreDisable(ufeid, coreid, core_events);

      if(emphy_events->event_reg)
         WUFEI_HwEventEmphyDisable(ufeid, coreid, emphy_events);
   }

   if (events->cr_events.event_reg & WUFE_EVENT_CR_CT_COUNTER_OVERFLOW)
   {
      events->cr_events.event_reg &= ~WUFE_EVENT_CR_CT_COUNTER_OVERFLOW;
      WUFEI_HwEventCrDisable(ufeid, WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW);
   }

   WUFEI_HwEventLiuDisable(ufeid, liu_events);

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
   WUFE_events_liu* liu_events = &events->liu_events;

   for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid ++)
   {
      WUFE_events_core* core_events =  &events->core_events[coreid];
      WUFE_events_emphy* emphy_events =  &events->emphy_events[coreid];

      if(core_events->event_reg )
         WUFEI_HwEventCoreEnable(ufeid, coreid, core_events);

      if(emphy_events->event_reg)
         WUFEI_HwEventEmphyEnable(ufeid, coreid, emphy_events);
   }

   if (events->cr_events.event_reg & WUFE_EVENT_CR_CT_COUNTER_OVERFLOW)
   {
      events->cr_events.event_reg &= ~WUFE_EVENT_CR_CT_COUNTER_OVERFLOW;
      WUFEI_HwEventCrEnable(ufeid, WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW);
   }

   WUFEI_HwEventLiuEnable(ufeid, liu_events);

   return WUFE_OK;
}

static void WUFEI_SbiClockTranslatorEvents(WP_U32 ufeid, WUFE_events_clock_recovery *cr_events)
{
   WP_U16 ct_event_arr[WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS];
   WP_U16 ct_event_mask_arr[WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS];
   WP_U32 i, sbi_ct_id;

   /* clear all SBI CT events */
   memset(cr_events->ct_events, 0, sizeof(WP_U8)* WUFE_MAX_N_CLOCK_TRANSLATORS);

   /* read the interrupt register */
   WUFEI_HwSbiClockTranslatorEventGet(ufeid, ct_event_arr);
   WUFEI_HwSbiClockTranslatorEventMaskGet(ufeid, ct_event_mask_arr);

   for (i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
   {
      WP_U16 int_reg = ct_event_arr[i] & ~ct_event_mask_arr[i];

      sbi_ct_id = i*16;
      while(int_reg)
      {
         if (sbi_ct_id > WUFE_MAX_N_CLOCK_TRANSLATORS)
            break;

         if (int_reg & 0x1)
            cr_events->ct_events[sbi_ct_id] = 1;

         sbi_ct_id++;
         int_reg >>= 1;
      }
   }

   /* clean the interrupt register */
   WUFEI_HwSbiClockTranslatorEventSet(ufeid, ct_event_arr);
}
