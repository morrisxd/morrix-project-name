/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_winmon_winpath2/sources/wpl_platform.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <winmon.h>

#include <wpl_types.h>
#include <wpl_platform.h>
#include <wpl_board.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPI_MIPS_UNCACHED_OFFSET 0xa0000000
#define WPI_MIPS_CACHED_OFFSET   0x80000000

#define HAS_IMPLICIT_ADDRESS(a) ((WP_U32)(a) < 0x20000000)

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

#define WPI_FREQ_AUTODETECT 1

#ifndef NO_AUTO_MEMMAP
#define WINPATH2_NO_BUSES 4
extern WP_S32 get_bus_address(WP_S32 c);
extern WP_S32 get_bus_size(WP_S32 c);
extern WP_S32 is_bus_access(WP_S32 c);
#endif



/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

wpi_veneer_memory wpl_bus_mappings[WPI_BUS_LAST] = { { 0 } };
WP_U32            wpl_rif_base[WINPATH_NUM_MAX] = { 0 };

WP_U8 wpl_board_app_banks [WPI_BUS_LIMIT] = {
   APP_BANK_PARAM,
   APP_BANK_PACKET,
   APP_BANK_INTERNAL,
   APP_BANK_HOST
};


/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

WP_U32 WPLI_BusesMapPrint(wpi_veneer_memory i_bus_mapping[]);

WP_U32 win_get_characteristics(WP_U32 bank)
{
   if (bank == 3)
      return 0x81;
   else
      return bank + 2;
}
/* TMP win_get_characteristics - revert to a BSP function */

static void local_exit(WP_U32 i);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/


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

WP_U32 WPL_Wait(WP_U32 wait_length)
{
   if (wait_length == 0)
      wait_length = 1;
/*    sleep(wait_length);*/
   return WPL_OK;
}

WP_U32 WPL_BusesMapGet(wpi_veneer_memory* o_bus_mapping)
{
   WP_U32 i;

   if (o_bus_mapping == NULL)
   {
      return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }

   for (i = 0; i < WINPATH_NUM_BUSES; ++i)
   {
      wpl_bus_mappings[i].bus = i;
      wpl_bus_mappings[i].start = get_bus_address(i + 1);
      wpl_bus_mappings[i].size = get_bus_size(i + 1);
      if (HAS_IMPLICIT_ADDRESS(wpl_bus_mappings[i].start))
      {
         wpl_bus_mappings[i].cached_offset = (WP_U8 *) WPI_MIPS_UNCACHED_OFFSET;
         wpl_bus_mappings[i].uncached_offset = (WP_U8 *) WPI_MIPS_UNCACHED_OFFSET;
      }
      else 
      {
         wpl_bus_mappings[i].cached_offset = (WP_U8 *) 0;
         wpl_bus_mappings[i].uncached_offset = (WP_U8 *) 0;
      }
      wpl_bus_mappings[i].actual_start = wpl_bus_mappings[i].start;
      wpl_bus_mappings[i].characteristics = win_get_characteristics(i);
      wpl_bus_mappings[i].bank = wpl_board_app_banks[i];

      /* Workaround for address 0xffffffff related problems */
      if ((wpl_bus_mappings[i].size) && (wpl_bus_mappings[i].start + wpl_bus_mappings[i].size == 0xffffffff))
      {
         wpl_bus_mappings[i].size -= 16;
      }
   }

   wpl_bus_mappings[WP_BUS_HOST].start = WPL_HOST_BUS_ADDR;
   wpl_bus_mappings[WP_BUS_HOST].size  = WPL_HOST_BUS_SIZE;
   wpl_bus_mappings[WP_BUS_HOST].actual_start = wpl_bus_mappings[WP_BUS_HOST].start;

   /* Workaround for address 0xffffffff related problems */
   if ((wpl_bus_mappings[WP_BUS_HOST].size) &&
       (wpl_bus_mappings[WP_BUS_HOST].start + wpl_bus_mappings[WP_BUS_HOST].size == 0xffffffff))
   {
      wpl_bus_mappings[WP_BUS_HOST].size -= 16;
   }

   /* get RIF base */
   wpl_rif_base[0] = WPL_INTERNAL_BUS_RIF_BASE;

   if (HAS_IMPLICIT_ADDRESS(wpl_rif_base[0]))
   {
      wpl_rif_base[0] += WPI_MIPS_UNCACHED_OFFSET;
   }

   /* Copy the results to the out param */
   memcpy(o_bus_mapping, wpl_bus_mappings, sizeof(wpi_veneer_memory)
         * WPI_BUS_LAST);

   return WPL_OK;
}

WP_U32 WPL_BusesMapSet(wpi_veneer_memory i_bus_mapping[])
{
   return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
}

WP_U32 WPL_BusesVadrsSet(WP_U8* i_buses_vadrs[])
{
   return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
}

WP_U32 WPL_BusesNeedRemap()
{
   return WPL_OK;
}

static void local_exit(WP_U32 i)
{
   exit(i);
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
   local_exit(1);
}

/***************************************************
 * Return information about a memory bank queried by index.
 * If bank is available to WDDI, return wpi_vbank_valid and set "line".
 * If bank is unavailable to WDDI, return wpi_vbank_invisible.
 * If index is too large, return wpi_vbank_invalid.
 ****************************************************/

/*****************************************************************************
 *
 * Function: WPL_GetVbank
 *
 * Purpose: Return each line of memory_data as requested by WDDI
 *
 * Inputs:
 *     index  the row to be returned
 *     line   address into which to store the data
 *
 * Outputs:
 *     line   contents of row/index from memory_data
 *
 * Return Value:
 *     wpi_vbank_valid     data in line filled in
 *     wpi_vbank_invalid   no more lines in table
 *
 * Called by:
 *     WDDI
 *
 ****************************************************************************/

enum wpi_vbank_result WPL_GetVbank(WP_U8 index, wpi_veneer_memory *line)
{
   /* index too large */
   if (index >= WPI_BUS_LIMIT)
      return wpi_vbank_invalid;

   /* Reorder the line numbering to conform to WDDI assumption */
   /* of Host, param, packet, internal...                      */
   if (index == 0){
      memcpy(line, &wpl_bus_mappings[3], sizeof(wpi_veneer_memory));
   }
   else
   {
      memcpy(line, &wpl_bus_mappings[index - 1], sizeof(wpi_veneer_memory));
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

void WPL_CacheFlush(WP_U8 *address,
                    WP_U32 nBytes)
{
}

/*****************************************************************************
 *
 * Function: WPL_GetBspBusAssignment
 *
 * Purpose: Dummy WDDI / BSP interface function
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

   return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
}

WP_U32 WPL_DriverInit(void)
{
   /* Call the platform function to get bus information from the kernel */
   WP_U32 status = WPL_BusesMapGet(wpl_bus_mappings);
   if (status != WPL_OK)
   {
      return status;
   }

   WinMon_enable_interrupts();

   return WPL_OK;
}

WP_U8* WPL_RegBaseGet(WP_U8 i_wpid)
{
   return (WP_U8*) wpl_rif_base[i_wpid];
}

WP_U8* WPL_RegOffsetGet(WP_U8 i_wpid)
{
   if (i_wpid == 0)
   {
      return (WP_U8*) wpl_bus_mappings[WP_BUS_INTERNAL].uncached_offset;
   }

   return (WP_U8*) -1;
}

WP_U32 WPL_EventInit(void)
{
   return WPL_OK;
}

void WPL_DriverRelease(void)
{
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
   printf("-------- UNRECOVERABLE INTERRUPTS 0x%08x ---------\n", syir);

   WPL_Abort();
}

WP_U32 WPLI_BusesRifBaseGet(WP_U32* o_rif_base)
{
   if (wpl_rif_base[0] == NULL)
   {
      wpl_rif_base[0] = WPL_INTERNAL_BUS_RIF_BASE;
      if (HAS_IMPLICIT_ADDRESS(wpl_rif_base[0]))
      {
         wpl_rif_base[0] += (WP_U32) WPI_MIPS_UNCACHED_OFFSET;
      }
   }

   *o_rif_base = wpl_rif_base[0];

   return WPL_OK;
}

WP_U32 WPLI_BusesMapPrint(wpi_veneer_memory i_bus_mapping[])
{
   WP_U32 ii;

   for (ii = 0; ii < WPI_BUS_LAST; ii++)
   {
      printf(
            "%u: bus %u bank %u ch 0x%x start 0x%x (actual 0x%x) size 0x%x ch 0x%x unch 0x%x \n",
            ii, i_bus_mapping[ii].bus, i_bus_mapping[ii].bank,
            i_bus_mapping[ii].characteristics, i_bus_mapping[ii].start,
            i_bus_mapping[ii].actual_start, i_bus_mapping[ii].size,
            (WP_U32) i_bus_mapping[ii].cached_offset,
            (WP_U32) i_bus_mapping[ii].uncached_offset);
   }

   return 0;
}
