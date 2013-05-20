#ifndef _WPL_INTERRUPTS_INTERFACE_H__
#define _WPL_INTERRUPTS_INTERFACE_H__

#include "wpl_types.h"

/* General notes: For a multiple-WinPath system, interrupt priorities on
 different WinPaths should match.  For example, a priority of 2 on a
 WinPath is higher than priority 3 on any WinPath and less than
 priority 1 on any WinPath.  Masking interrupts at a given priority
 level must be done on all WinPaths, not just the one interrupting.

 On entry to an interrupt handler, interrupts at or below the current
 priority are masked. */

/* The interrupt type which the WDDI needs to connect to. */
typedef enum
{
   WPL_IntOverrunBspQueue = 0,
   WPL_IntServiceBspQueue,
   WPL_IntOverrunQueues,
   WPL_IntServiceQueues,
   /* The following are used
    only in OsEnet KEEPALIVE mode */
   WPL_ServiceEvents,
   WPL_OverrunEvents,
   /* The following are used only
    when WEB_WINPATHS > 1 */
   WPL_MultiOverrunQueues,
   WPL_MultiServiceQueues,
   WPL_BusError,
   WPL_SgmiiAn,
   WPL_Eint3Ufe4,
   WPL_SerdesInterrupt,
   /* Value to indicate global state of interrupts */
   WPL_IntGlobalState,
   WPL_IntVectorNum
} WPL_IntVector;

/* The type of an interrupt handler.  wpid specifies the WinPath #.  */
typedef void WPL_IntHandler(WP_U32 wpid);
typedef void WPL_IntHandlerCallback(WP_U32 wpid, WP_U32 event_bits);

#define WPL_WINPATH(x) (x)

/* The function to connect a handler to an interrupt vector. */
void WPL_IntConnect(WP_U32 wpid,
                    WPL_IntVector vector,
                    WPL_IntHandler *function,
                    WPL_IntHandlerCallback *wddi_callback);

/* A function to enable specific interrupt sources */
WP_U32 WPL_IntEnable(WP_U32 wpid, WPL_IntVector vector);

/* A function to disable specific interrupt sources;
 * should only be called if WPL_IntEnable was called for that source
 */
WP_U32 WPL_IntDisable(WP_U32 wpid, WPL_IntVector vector);

#endif /*_WPL_LOCKS_H__*/
