/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_events.c
 *
 * Purpose: API and internal functions for UFE events.
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

#ifndef WUFEI_HW_BOARD_SUPPORT_INT_H
#include "wufei_board_support_int.h"
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

#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif

/************************* API functions ************************************/
/*****************************************************************************
 *
 * Function:  WUFE_UfeReadEvents
 *
 * Purpose:   reads the raised UFE events
 *
 * Description:
 *     This function reads the UFE interrupt registers
 *
 * Inputs:
 *     ufeid - the UFE id
 *     events - the user structure to fill
 *
 * Outputs:
 *     fills the ufe_events structure
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeReadEvents(WP_U32 ufeid,
                               WUFE_events *events)
{
   WUFEI_system_registry *system_registry;

   if (!events)
      return WUFE_ERR_EVENT_INVALID_CONFIG;

   memset(events, 0, sizeof(WUFE_events));

   /* get access to the registry -- leave unlocked */
   system_registry = WUFEI_SystemRegistryGet(ufeid);
   if(!system_registry)
   {
      return WUFE_ERR_INVALID_REGISTRY_STATE;
   }
   WUFEI_HwEventDispatch(ufeid, system_registry, events);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeEventDisable
 *
 * Purpose:   masks the required UFE events
 *
 * Description:
 *     This function masks the required events in the UFE interrupt registers
 *
 * Inputs:
 *     ufeid - the UFE id
 *     ufe_events - the user structure to fill
 *
 * Outputs:
 *     the events are disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeEventDisable(WP_U32 ufeid,  WUFE_events *events)
{
   WUFE_status status;

   if (!events)
      return WUFE_ERR_EVENT_INVALID_CONFIG;

   WUFEI_UFE_LOCK(ufeid);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_HwEventDisable(ufeid,events), ufeid);

   WUFEI_UFE_UNLOCK(ufeid);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeEventEnable
 *
 * Purpose:   enables the required UFE events
 *
 * Description:
 *     This function enables the required events in the UFE interrupt registers
 *
 * Inputs:
 *     ufeid - the UFE id
 *     events - the user structure to fill
 *
 * Outputs:
 *     the events are enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeEventEnable(WP_U32 ufeid, WUFE_events *events)
{
   WUFE_status status;

   if (!events)
      return WUFE_ERR_EVENT_INVALID_CONFIG;

   WUFEI_UFE_LOCK(ufeid);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_HwEventEnable(ufeid, events),ufeid);

   WUFEI_UFE_UNLOCK(ufeid);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeCoreHwInterruptEnable
 *
 * Purpose:   enable UFE interrupt in the Interrupt mask register
 *
 * Description:
 *     This function clears bit 0 in the Interrupt mask register
 *     which is used for UFE interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     UFE interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeCoreHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status;

   status = WUFEI_UfeCoreHwInterruptEnable(ufeid);
   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeFramerHwInterruptEnable
 *
 * Purpose:   enable FRAMER interrupt in the Interrupt mask register
 *
 * Description:
 *     This function clears bit 1 in the Interrupt mask register
 *     which is used for FRAMER interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     FRAMER interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeFramerHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status;

   status = WUFEI_UfeFramerHwInterruptEnable(ufeid);
   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeExtPllHwInterruptEnable
 *
 * Purpose:   enable ext PLL interrupt in the Interrupt mask register
 *
 * Description:
 *     This function clears bit 2 in the Interrupt mask register
 *     which is used for ext PLL interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     EXT PLL interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeExtPllHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status;

   status =  WUFEI_UfeExtPllHwInterruptEnable(ufeid);
   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeLockLostHwInterruptEnable
 *
 * Purpose:   enable lock lost interrupt in the Interrupt mask register
 *
 * Description:
 *     This function clears bit 3 in the Interrupt mask register
 *     which is used for lock lost interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     lock lost interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeLockLostHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status;

   WUFEI_UFE_LOCK(ufeid);
   status =  WUFEI_UfeLockLostHwInterruptEnable(ufeid);
   WUFEI_UFE_UNLOCK(ufeid);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeSfpHwInterruptEnable
 *
 * Purpose:   enable sfp interrupt in the Interrupt mask register
 *
 * Description:
 *     This function clears bit 4 in the Interrupt mask register
 *     which is used for sfp interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     sfp interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeSfpHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status;

   WUFEI_UFE_LOCK(ufeid);
   status =  WUFEI_UfeSfpHwInterruptEnable(ufeid);
   WUFEI_UFE_UNLOCK(ufeid);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeCoreHwInterruptDisable
 *
 * Purpose:   disable UFE interrupt in the Interrupt mask register
 *
 * Description:
 *     This function sets bit 0 in the Interrupt mask register
 *     which is used for UFE interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     UFE interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeCoreHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status;

   status = WUFEI_UfeCoreHwInterruptDisable(ufeid);
   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeFramerHwInterruptDisable
 *
 * Purpose:   Disable FRAMER interrupt in the Interrupt mask register
 *
 * Description:
 *     This function sets bit 1 in the Interrupt mask register
 *     which is used for FRAMER interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     FRAMER interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeFramerHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status;

   WUFEI_UFE_LOCK(ufeid);
   status = WUFEI_UfeFramerHwInterruptDisable(ufeid);
   WUFEI_UFE_UNLOCK(ufeid);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeExtPllHwInterruptDisable
 *
 * Purpose:   disable ext PLL interrupt in the Interrupt mask register
 *
 * Description:
 *     This function sets bit 2 in the Interrupt mask register
 *     which is used for ext PLL interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     EXT PLL interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeExtPllHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status;

   WUFEI_UFE_LOCK(ufeid);
   status =  WUFEI_UfeExtPllHwInterruptDisable(ufeid);
   WUFEI_UFE_UNLOCK(ufeid);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeLockLostHwInterruptDisable
 *
 * Purpose:   disable lock lost interrupt in the Interrupt mask register
 *
 * Description:
 *     This function sets bit 3 in the Interrupt mask register
 *     which is used for lock lost interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     lock lost interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeLockLostHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status;

   WUFEI_UFE_LOCK(ufeid);
   status =  WUFEI_UfeLockLostHwInterruptDisable(ufeid);
   WUFEI_UFE_UNLOCK(ufeid);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeSfpHwInterruptDisable
 *
 * Purpose:   disable sfp interrupt in the Interrupt mask register
 *
 * Description:
 *     This function sets bit 4 in the Interrupt mask register
 *     which is used for sfp interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     sfp interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeSfpHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status;

   WUFEI_UFE_LOCK(ufeid);
   status =  WUFEI_UfeSfpHwInterruptDisable(ufeid);
   WUFEI_UFE_UNLOCK(ufeid);

   return status;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeCoreHwInterruptGet
 *
 * Purpose:   read UFE interrupt in the Interrupt event register
 *
 * Description:
 *     This function read bit 0 in the Interrupt mask register
 *     which is used for UFE interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     UFE interrupt is read
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WP_boolean WUFE_UfeCoreHwInterruptGet(WP_U32 ufeid)
{
   WP_boolean interrupt_result;
   WUFE_status status;

   status = WUFEI_UfeCoreHwInterruptGet(ufeid,&interrupt_result);

   return interrupt_result;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeFramerHwInterruptGet
 *
 * Purpose:   read FRAMER interrupt in the Interrupt event register
 *
 * Description:
 *     This function read bit 1 in the Interrupt event register
 *     which is used for FRAMER interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     FRAMER interrupt is read
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WP_boolean WUFE_UfeFramerHwInterruptGet(WP_U32 ufeid)
{
   extern WP_U32 wufei_ufe_current_status[WUFE_MAX_N_UFE];
   WP_boolean interrupt_result = WP_FALSE;
   WUFE_status status;

   if (wufei_ufe_current_status[ufeid] == 1)
      status = WUFEI_UfeFramerHwInterruptGet(ufeid, &interrupt_result);
   return interrupt_result;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeExtPllHwInterruptGet
 *
 * Purpose:   read ext PLL interrupt in the Interrupt event register
 *
 * Description:
 *     This function read bit 2 in the Interrupt event register
 *     which is used for ext PLL interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     EXT PLL interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WP_boolean WUFE_UfeExtPllHwInterruptGet(WP_U32 ufeid)
{
   WP_boolean interrupt_result;
   WUFE_status status;

   status = WUFEI_UfeExtPllHwInterruptGet(ufeid, &interrupt_result);
   return interrupt_result;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeLockLostHwInterruptGet
 *
 * Purpose:   read lock lost interrupt in the Interrupt event register
 *
 * Description:
 *     This function read bit 3 in the Interrupt event register
 *     which is used for lock lost interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the interrupt value
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WP_boolean WUFE_UfeLockLostHwInterruptGet(WP_U32 ufeid)
{
   WP_boolean interrupt_result;
   WUFE_status status;

   status = WUFEI_UfeLockLostHwInterruptGet(ufeid,&interrupt_result);

   return interrupt_result;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeSfpHwInterruptGet
 *
 * Purpose:   read sfp interrupt in the Interrupt event register
 *
 * Description:
 *     This function read bit 4 in the Interrupt event register
 *     which is used for sfp interrupt
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the interrupt value
 *
 * Called by:
 *     Any function user.
 *
 ****************************************************************************/
WP_boolean WUFE_UfeSfpHwInterruptGet(WP_U32 ufeid)
{
   WP_boolean interrupt_result;
   WUFE_status status;

   status = WUFEI_UfeSfpHwInterruptGet(ufeid,&interrupt_result);

   return interrupt_result;
}


/************************* Internal driver functions ************************/
/*****************************************************************************
 *
 * Function:  WUFEI_EventDisable
 *
 * Purpose:   masks the required UFE events
 *
 * Description:
 *
 * Inputs:
 *     ufeid - the UFE id
 *     coreid - the UFE CORE id
 *     value - the interrupt mask
 *
 * Outputs:
 *     the events are disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_EventDisable(WP_U32 ufeid, WP_U32 coreid, WP_U16 value)
{
   WUFEI_hw_events* ufe_event_reg = WUFEI_EVENT;
   WP_U16 event_mask_reg=0;
   WP_U32 id = WUFEI_ID(ufeid, coreid);

   WUFEI_MEM_16_GET(id, &(ufe_event_reg->event_mask_reg), &event_mask_reg);
   event_mask_reg = event_mask_reg | value;
   WUFEI_MEM_16_SET(id, &(ufe_event_reg->event_mask_reg), event_mask_reg);
}

/*****************************************************************************
 *
 * Function:  WUFEI_EventEnable
 *
 * Purpose:   enables the required UFE events
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *     ufe_events - the user structure to fill
 *
 * Outputs:
 *     ufeid - the UFE id
 *     coreid - the UFE CORE id
 *     value - the interrupt mask
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_EventEnable(WP_U32 ufeid, WP_U32 coreid, WP_U16 value)
{
   WUFEI_hw_events* ufe_event_reg = WUFEI_EVENT;
   WP_U16 event_mask_reg=0;
   WP_U32 id = WUFEI_ID(ufeid, coreid);

   WUFEI_MEM_16_GET(id, &(ufe_event_reg->event_mask_reg), &event_mask_reg);
   event_mask_reg = event_mask_reg & ~value;
   WUFEI_MEM_16_SET(id, &(ufe_event_reg->event_mask_reg), event_mask_reg);
}
