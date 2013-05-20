/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux_winpath3/sources/wpl_platform.c
 *
 * Purpose: Winpath's platform dependent functions
 *
 * Functions contained in this file:
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal Functions
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal Functions (static)
 * |  -------------------------+----------------------------------------------
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

/******************************************************************/
/*                       Includes                                 */
/******************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <wpl_types.h>
#include <wpl_platform.h>
#include <wpl_board.h>


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

/***************************************************
 * Return information about a memory bank queried by index.
 * If bank is available to WDDI, return wpi_vbank_valid and set "line".
 * If bank is unavailable to WDDI, return wpi_vbank_invisible.
 * If index is too large, return wpi_vbank_invalid.
 ****************************************************/

#define WPL_PAGE_SIZE 4096


/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/


/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

static void *host_allocation = NULL;
static void *host_start;
static void *host_offset;

WP_U32            wpl_rif_base[WINPATH_NUM_MAX] = { 0 };
wpi_veneer_memory wpl_bus_mappings[WPI_BUS_LAST] = { { 0 } };

static wpi_veneer_memory wpl_fixed_bus_mappings[WPL_NUM_BUSES] = {
      {0, WP_BUS_PARAM,    2, WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_PARAM_RANGE),
         0x10000000, 0x00400000,  0, 0, 0x10000000 },
      {0, WP_BUS_PACKET,   0, WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_PACKET_RANGE),
         0x50000000,  0x00400000, 0, 0, 0x50000000  },
      {0, WP_BUS_INTERNAL, 0, WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_INTERNAL_RANGE),
         0x1f000000, 0x280000,    0, 0, 0x1f000000 },
      {0, WP_BUS_HOST,     1, WPL_SHARED_MEMORY_F(1) + WPL_PHYS_RANGE_F(WPL_BOARD_HOST_RANGE),
         0x30000,  (WPL_PAGES * WPL_PAGE_SIZE - 0x30000), 0, 0, 0x30000  },
      {0, WP_BUS_WMM5,     0, 0x0,  0x0,        0x0,       0, 0, 0x0        },
      {0, WPI_BUS_RIF,     0, WPL_SHARED_MEMORY_F(0) + WPL_PHYS_RANGE_F(WPL_BOARD_RIF_RANGE),
         0x00400000, 0x40000,   0, 0, 0x00400000 }
};


/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

/***************************************************
 * Return number of WinPaths in system.
 ****************************************************/
WP_U32 WPL_GetWinPaths(void)
{
   return WPL_NUM_WINPATHS;
}

enum wpi_vbank_result WPL_GetVbank(WP_U8 index, wpi_veneer_memory *line)
{
   /* index too large */
   if (index >= WPL_NUM_BUSES)
      return wpi_vbank_invalid;

   /* Reorder the line numbering to conform to WDDI assumption */
   /* of Host, param, packet, internal...                      */
   switch (index) {
   case 0:
      memcpy(line, &wpl_bus_mappings[WP_BUS_HOST], sizeof(wpi_veneer_memory));
      break;
   case 1:
   case 2:
   case 3:
      memcpy(line, &wpl_bus_mappings[index - 1], sizeof(wpi_veneer_memory));
      break;
   case 4:
      memcpy(line, &wpl_bus_mappings[WPI_BUS_RIF], sizeof(wpi_veneer_memory));
      break;
   default:
      return wpi_vbank_invalid;
   }

   return wpi_vbank_valid;
}

/*****************************************************************************
 *
 * Function:  WPL_BspServiceMask
 *
 * Purpose:  Supply WDDI with queue service mask for this target
 *
 ****************************************************************************/

WP_U32 WPL_BspServiceMask(void)
{
   return 0;
}

/*****************************************************************************
 *
 * Function:  WPL_BspOverrunMask
 *
 * Purpose:  Supply WDDI with queue overrun mask for this target
 *
 ****************************************************************************/

WP_U32 WPL_BspOverrunMask(void)
{
   return 0;
}

/*****************************************************************************
 *
 * DUMMY CACHE MANAGEMENT ROUTINE
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * Function: WPL_CacheFlush
 *
 * Purpose: Dummy WDDI / BSP interface function
 *
 ****************************************************************************/

void WPL_CacheFlush(WP_U8 *address, WP_U32 nBytes)
{
}

/*****************************************************************************
 *
 * DUMMY BSP BUS ASSIGNMENT ROUTINE
 *
 ****************************************************************************/

enum wpi_vbank_result WPL_GetBspBusAssignment(WP_U32 whichBusType,
                                              WP_U8 *bus,
                                              WP_U8 *bank)
{
   return wpi_vbank_invalid;
}


/*****************************************************************************
 *
 * Function: WPL_OsService
 *
 * Purpose: Indicate non-use of ENET1 to WDDI
 *
 ****************************************************************************/

WP_U32 WPL_OsService(WPL_OsServiceType action, void *holder)
{
   if (action == WPL_OsServiceGetDriverInfo)
   {
      *(WPL_os_enet_usage *) holder = WPL_os_enet_unused;
      return WPL_OK;
   }

   return WPL_ERR(WPL_ERR_INVALID_PARAM);
}

WP_U32 WPI_WinSimCmdInit(void);
WP_U32 WPL_DriverInit(void)
{
   WP_U32 i, n_veneer_memory_items;
   WP_boolean rif_bus_found = WP_FALSE;
   WP_U32 host_base;

   n_veneer_memory_items = WPL_NUM_BUSES;

   memcpy(wpl_bus_mappings, wpl_fixed_bus_mappings, sizeof(wpl_fixed_bus_mappings));

   /* first time allocate host memory */
   for (i=0; i<n_veneer_memory_items; i++)
   {
      if (wpl_bus_mappings[i].bus == WP_BUS_HOST)
      {
         /* First-time allocation of shared memory on host */
         if (!host_allocation)
         {
            WP_U32 host_size = wpl_bus_mappings[i].size + wpl_bus_mappings[i].start;
            host_allocation = (void*)WPL_MALLOC(host_size);
            host_base = (WP_U32) host_allocation;
            host_base = (host_base + WPL_PAGE_SIZE - 1) & ~(WPL_PAGE_SIZE - 1);
            host_start = (WP_U8 *) host_base;
            host_offset = (WP_U8 *) host_start - wpl_bus_mappings[i].start;
         }
         break;
      }
   }

   /* second round update cached/uncached offsets and
    * set Winpath RIF base */
   for (i=0; i<n_veneer_memory_items; i++)
   {
      wpl_bus_mappings[i].cached_offset   = host_offset;
      wpl_bus_mappings[i].uncached_offset = host_offset;

      if (wpl_bus_mappings[i].bus == WPI_BUS_RIF)
      {
         WP_U32 wpid = wpl_bus_mappings[i].wpid;

         wpl_rif_base[wpid] = (wpl_bus_mappings[i].start +
                     (WP_U32)wpl_bus_mappings[i].uncached_offset);
         rif_bus_found = WP_TRUE;
      }
   }

   if (!rif_bus_found)
      return WPL_ERR(WPL_ERR_RIF_BUS_UNINITIALIZED);

   return WPI_WinSimCmdInit();
}

WP_U8* WPL_RegBaseGet(WP_U8 i_wpid)
{
   return (WP_U8*) wpl_rif_base[i_wpid];
}

WP_U8* WPL_RegOffsetGet(WP_U8 i_wpid)
{
   if (i_wpid == 0)
   {
      return (WP_U8*) wpl_bus_mappings[WPI_BUS_RIF].uncached_offset;
   }

   return (WP_U8*) -1;
}



WP_U32 WPL_EventInit(void)
{
   return WPL_OK;
}

void WPI_WinSimCmdRelease(void);

void WPL_DriverRelease(void)
{
   WPI_WinSimCmdRelease();
}

/*****************************************************************************
 *
 * Function: WPL_ReportCatastrophicErrors
 *
 * Purpose: Report details about catastrophic errors and then stall
 *
 * Inputs:
 *    syir     Current value of interrupt controller register SYIR
 *
 * Outputs:
 *    None
 *
 * Return value:
 *    None
 *
 * Called by:
 *    WPI_SimulateInterrupts
 *
 ****************************************************************************/

void WPL_ReportCatastrophicErrors(WP_U32 syir)
{
   printf("Either bus monitor error or OCAMF has occurred!\n");
}

/*****************************************************************************
 *
 * Function: WPL_BrgAvailability
 *
 * Purpose:  Determine whether or not BRG9, BRG10 are in use by the BSP
 *           or are available to WDDI
 *
 * Description:  This function reports on the availability of BRGs 9 and 10
 *           on the designated WinPath for use by WDDI.  If the BRG is available,
 *           a non-zero value is reported; if the BRG is not available (that is,
 *           the BSP is using it), zero is reported.
 *
 * Inputs:
 *    wpid   WinPath id
 *
 * Outputs:
 *    brg9_is_available   0 if BRG9 is used by the BSP
 *    brg10_is_available  0 if BRG10 is used by the BSP
 *
 * Returns:
 *    N/A
 *
 * Called by:
 *   WP_SysInit
 *
 ****************************************************************************/

void WPL_BrgAvailability(WP_U32 wpid,
                         WP_U32 *brg9_is_available,
                         WP_U32 *brg10_is_available)
{
   *brg9_is_available = 1;
   *brg10_is_available = 1;
}

/*****************************************************************************
 *
 * Function: WPL_Abort
 *
 * Purpose: Cease execution if the registry was previously locked
 *
 * Inputs:
 *     None
 *
 * Outputs:
 *     None
 *
 * Return Value:
 *     None
 *
 * Called by:
 *     WPI_WDDI_ATTACH() macro if registry already locked
 *
 ****************************************************************************/

void WPL_Abort(void)
{
   exit(1);
}

WP_U32 WPL_ThreadInit(WP_THREAD_ID *thread_id,
                       void *(*thread_function)(void *),void *thread_function_arg)
{
     return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
}

WP_U32 WPL_ThreadDestroy(WP_THREAD_ID *thread_id)
{
     return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
}

WP_U32 WPL_ThreadSelf(WP_THREAD_ID *thread_id)
{
   *thread_id = 0xdeadbeef;
   return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
}
