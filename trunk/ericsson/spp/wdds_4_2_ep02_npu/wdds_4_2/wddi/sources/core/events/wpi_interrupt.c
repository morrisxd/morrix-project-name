/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpi_interrupt.c
 *
 * Purpose: Interrupt handling functions
 *

 * Functions contained in this file:
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal WDDI Functions
 * |  -------------------------+----------------------------------------------
 * |  WPI_IntServiceWrapper    | Respond to queue service interrupts
 * |  WPI_IntOverrunWrapper    | Respond to queue overrun interrupts
 * |  WPI_ReportEventOverrun   | Report overruns to application
 * |  WPI_SimulateInterrupts   | Poll for events; process like interrupts
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal WDDI Functions (static)
 * |  -------------------------+----------------------------------------------
 * |  WPI_IntEventService      | Service valid events in event queues
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

#include <stdio.h>

#include "include/core/registry/wpi_types.h"
#include "include/core/registry/wpi_wddi_typedefs.h"

#ifndef WPI_UTILS_H
#include "include/core/utility/wpi_utils.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_REG_INT_H
#include "include/core/hardware/wpi_reg_int.h"
#endif
#ifndef WPI_HW_INT_H
#include "include/core/hardware/wpi_hw_int.h"
#endif
#ifndef WPI_DPS_EVENT_H
#include "include/core/event/wpi_dps_event.h"
#endif
#ifndef WPI_HW_H
#include "include/core/hardware/wpi_hw.h"
#endif
#ifndef WPI_EVENT_H
#include "include/core/event/wpi_event.h"
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif

#include "wpu_bus_mon.h"
#include "wpu_trs.h"
#include "wpu_wver.h"
#include "wpu_tb.h"
#include "wpu_ta_error.h"

/* Prototypes for static functions */

#include "include/core/wpi_memory_class.h"
#include "include/core/wpi_interrupt_queue_class.h"
#include "include/core/wpi_system_class.h"

void WPI_IntEventService(WP_U32 wpid, WP_U32 event_bits);

#if defined(INTERRUPT_MODEL_FLAT)

/*****************************************************************************
 *
 * Function: WPI_IntEventService
 *
 * Purpose: Service valid events in event queues
 *
 * Description:
 *     This function reads event entries in selected event queues, handing the
 *     valid events to WPI_IntDispatchEvent for processing.  For systems
 *     providing interrupt service, this function is called at interrupt time.
 *     This function services only the event queues which can contain
 *     interrupting events.
 *
 * Inputs:
 *     wpid            Index of WinPath whose event queues are to be serviced
 *     event_bits      Value of SEIR masked to select only service events
 *
 * Outputs:
 *
 * Return Value:
 *
 * Global variables:
 *     wpi_registry
 *
 * Called by:
 *     WPI_IntServiceWrapper, WPI_SimulateInterrupts
 *
 ****************************************************************************/

void WPI_IntEventService(WP_U32 wpid,
                         WP_U32 event_bits)
{
   wpi_irq *queue;
   wpi_memory *memory;
   WP_U32 i;
   WP_U32 vbank;
   WP_U8 *host_offset;
   volatile wpi_dps_event *host_event;
   WP_U32 event, eventx;
   WPI_PTR current, last, marker;
   WP_U32 queue_mask;

   /* This function must not call WPI_INTERNAL_FUNCTION() */
   /* Get pointer to interrupt queue registry information */
   queue = WPI_RegistryInterruptQueue(wpid);
   if (queue == 0)
      return;

   /* Find the vbank for the IRQ on this WinPath */

   memory = WPI_RegistryMemory(wpid);
   vbank = WPI_MemoryVbankOfVirtualBus(memory, WP_VB_INTQUEUE_DEBUG);
   if (vbank == 0)
      return;

   host_offset = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_INTQUEUE_DEBUG);

   /* Loop through the event queues in priority order. */
   /* Queue mask has been adjusted so that we do not see
    * events in IRQT0 if the BSP owns ENET1.  Polled queues
    * will not set a bit in SEIR.
    */
   queue_mask = WPI_HwInterruptQueueHostGet();
   for (i = 0; i < WP_IRQ_TABLES; queue++, i++, queue_mask <<= 1)
   {
      /* Don't look at queues with no events */
      if ((event_bits & queue_mask) == 0)
         continue;
      
      /* copy current to local variable */
      current = queue->current;
      last = queue->end;
      /* loop: at exit, current must be WPI_PTR of first V=0 bit */
      host_event = (wpi_dps_event *) WPI_HOST_PTR(current, host_offset);

       /* 1) if there were a previous overrun on this queue, we need to
         re-synchronize the read and write pointers on the queue by
         searching for the first valid event, if any.
         2) if the current write pointer(in the hardware irq) is not equal to the
         current read pointer,we need to search for valid events.
         It occurs when there is overrun and can still be a problem if the pointers match
         but there are still valid events(this is a very rare case which is not handled).
      */
      WPI_MEM_GET(event, host_event->event);
      if (WPI_DPS_EVENT_V_V(event) == 0 && queue->queue_pending_overrun_recovery)
      {
         marker = current;

         do {
            ++host_event;

            /* Increment current to find the next event */
            current += sizeof(wpi_dps_event);
            if (current > last)
            {
               current = queue->start;
               host_event = (wpi_dps_event *) WPI_HOST_PTR(current, host_offset);
            }

            WPI_MEM_GET(event, host_event->event);

         } while ((WPI_DPS_EVENT_V_V(event) == 0) && (current != marker));
      }
      WP_S32 loopnum =500;/*rizh*/
      while (loopnum-->0)
      {
         /* Get the word containing the V bit */
         WPI_MEM_GET(event, host_event->event);

         if (WPI_DPS_EVENT_V_V(event) == 0)
            break;
         /* Read the second word */
         WPI_MEM_GET(eventx, host_event->eventx);
         /* Mask all but the I bit of the event queue entry.
          * Increment the event pointer.  Fix at the end of the loop if it
          * needs to be wrapped. */
         WPI_MEM_SET(host_event++->event, event & WPI_DPS_EVENT_I_M);

         /* Dispatch the current event */
         WPI_IntDispatchEvent(wpid, event, eventx, WPI_QUEUE_INTERRUPTING);

         /* Increment current to find the next event */
         current += sizeof(wpi_dps_event);
         if (current > last)
         {
            current = queue->start;
            host_event = (wpi_dps_event *) WPI_HOST_PTR(current, host_offset);
         }
      }

      /* restore current from local variable */
      queue->current = current;
      /* Done with this queue.  Go on to the others, if any. */
   }

}

#if defined(WPI_OS_INT_SERVICE)

/*****************************************************************************
 *
 * Function: WPI_IntServiceWrapper
 *
 * Purpose: Called from BSP interrupt handler, responsible for acknowledging
 *          service queue interrupts.
 *
 * Description:
 *     This function is registered with the BSP to service the service events.
 *     It acknowledges the outstanding service interrupts, and calls
 *     WPI_IntEventService to process the event queues with events currently
 *     reported.  It finishes by restoring the interrupt level which was in
 *     effect prior to the call to this function.  This function is part of
 *     BSP interrupt processing.
 *
 * Inputs:
 *     wpid            Index of WinPath which interrupted the processor
 *
 * Outputs:
 *     WinPath service interrupts acknowledged (cleared from SEIR)
 *     WinPath interrupt level restored on exit.
 *
 * Return Value:
 *
 * Global variables:
 *     wpi_registry
 *
 * Called by:
 *     BSP interrupt handler
 *
 ****************************************************************************/

void WPI_IntServiceWrapper(WP_U32 wpid)
{
   WP_U32 queue_event;
   /* This function must not call WPI_INTERNAL_FUNCTION() */
   queue_event = WPI_HwInterruptQueueEventGet(wpid);

   /* Call the function telling it what the SEIR was.
    * That is where the events are actually analyzed and dispatched. */
   WPI_IntEventService(wpid, queue_event);
}
#endif

/*****************************************************************************
 *
 * Function: WPI_ReportEventOverrun
 *
 * Purpose: Report overruns to application, if it registered for this
 *          information.
 *
 * Description:
 *
 * Inputs:
 *     wpid            WinPath reporting the overflow event
 *     event_bits      SEIR value masked to select overflow queues
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/

void WPI_ReportEventOverrun(WP_U32 wpid, WP_U32 event_bits)
{
   wpi_event_callback *callback;
   wpi_irq *queue;
   WP_U32 queue_mask;
   WP_U32 j;

   WPI_INTERNAL_FUNCTION();
   /* This function must not call WPI_INTERNAL_FUNCTION() */
   callback = WPI_RegistryGlobalLockEventCallback(WP_EVENT_QUEUE_OVERRUN);
   /* Get pointer to interrupt queue registry information */
   queue = WPI_RegistryInterruptQueue(wpid);
   if (queue == 0)
      return;

   /* Loop through the event queues in priority order. */
   /* Queue mask has been adjusted so that we do not see
    * events for IRQT0 if the BSP owns ENET1.
    */
   queue_mask = WPI_HwInterruptQueueOverrunHostGet();
   for (j = 0; j < WP_IRQ_TABLES; ++j, ++queue, queue_mask <<= 1)
   {
      /* Don't look at queues with no events */
      if ((event_bits & queue_mask) == 0)
         continue;
      WPI_HwInterruptQueueTreatOverrun(wpid, j, queue);

      if (callback)
         WPI_EVENT_CALLBACK(callback, wpid, j, ++queue->queue_overrun_count);
   }
}

#if defined(WPI_OS_INT_SERVICE)

/*****************************************************************************
 *
 * Function: WPI_IntOverrunWrapper
 *
 * Purpose: Called from BSP interrupt handler, responsible for acknowledging
 *          queue overrun interrupts.
 *
 * Description:
 *     This function is registered with the BSP to service the queue overflow
 *     events.  It acknowledges the outstanding overrun interrupts, and calls
 *     WPI_ReportEventOverrun for each queue signalling overrun.  It finishes
 *     by restoring the interrupt level which was in effect prior to the call
 *     to this function.  This function is part of BSP interrupt processing.
 *
 * Inputs:
 *     wpid            Index of WinPath which interrupted the processor
 *
 * Outputs:
 *     WinPath overrun interrupts acknowledged (cleared from SEIR)
 *     WinPath interrupt level restored on exit.
 *
 * Return Value:
 *
 * Global variables:
 *     wpi_registry
 *
 * Called by:
 *     BSP interrupt handler
 *
 ****************************************************************************/

void WPI_IntOverrunWrapper(WP_U32 wpid)
{
   WP_U32 event_overrun;

   WPI_INTERNAL_FUNCTION();
   /* This function must not call WPI_INTERNAL_FUNCTION() */
   event_overrun = WPI_HwInterruptQueueOverrunGet(wpid);

   /* Call the function telling it what the SEIR was.
    * That is where the overruns are reported to the application. */
   WPI_ReportEventOverrun(wpid, event_overrun);
}
#endif

#endif /* INTERRUPT_MODEL_FLAT */

#ifndef WPI_OS_INT_SERVICE

/*****************************************************************************
 *
 * Function: WPI_SimulateInterrupts
 *
 * Purpose: Provide interrupt event distribution on polled-only targets
 *
 * Description:
 *     This function processes event queues which have valid, interrupting
 *     events and calls WPI_IntEventOverflow or WPI_IntEventService to
 *     process them.  It uses the value in the interrupt controller SEIR
 *     register to determine which queues to process.  WinPath 0 is assumed.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *
 * Global variables:
 *     wpi_registry; the SEIR in the WinPath interrupt controller
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/

void WPI_SimulateInterrupts(void)
{
   WP_U32 event_oflow;
   WP_U32 event_service;
   WP_U32 catastrophic_errors;
   WP_U32 event_system;
   WPI_INTERNAL_FUNCTION();
 
   WPI_HwInterruptSystemEventsGet(WP_WINPATH(0), &catastrophic_errors, &event_system);
   if (catastrophic_errors)
   {
      /* Report catastrophic error has occurred. */
       WPL_ReportCatastrophicErrors(event_system);

       while (1) ;
   }

   WPI_HwInterruptQueueEventsGet(WP_WINPATH(0), &event_oflow, &event_service);
   if (event_oflow)
      WPI_ReportEventOverrun(WP_WINPATH(0), event_oflow);
   if (event_service)
      WPI_IntEventService(WP_WINPATH(0), event_service);
}

#else

void WPI_SimulateInterrupts(void)
{
   /* Since applications may call this function, make sure it is defined on
    * all targets.
    */
}

#endif
