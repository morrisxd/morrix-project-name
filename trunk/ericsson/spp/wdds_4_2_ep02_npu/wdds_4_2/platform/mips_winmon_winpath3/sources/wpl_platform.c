/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_winmon_winpath3/sources/wpl_platform.c
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
#include <mt_api.h>

#include <wpl_types.h>
#include <wpl_platform.h>
#include "wpl_os_enet.h"

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/
#define WPL_BUSES_MAP_DEBUG 0
#define WPL_NUM_BUSES WPI_BUS_LAST

#define WPI_MIPS_UNCACHED_OFFSET 0xa0000000
#define WPI_MIPS_CACHED_OFFSET   0x80000000

#define HAS_IMPLICIT_ADDRESS(a) ((WP_U32)(a) < 0x20000000)

#define WPL_MEGABYTE (1024*1024)

#define WPL_CACHE_DISABLE 0 /* See winmon's cache_mmu.h */

#define WPL_FIELD(v,s) (((WP_U32) (v)) << (s))
#define WPL_VALUE(f,s,w) ((((WP_U32) (f)) >> (s)) & ((1 << (w)) - 1))

#define WPL_SHARED_MEMORY_S   7
#define WPL_SHARED_MEMORY_W   1
#define WPL_SHARED_MEMORY_F(v) WPL_FIELD(v, WPL_SHARED_MEMORY_S)
#define WPL_SHARED_MEMORY_V(f) \
   WPL_VALUE(f, WPL_SHARED_MEMORY_S, WPL_SHARED_MEMORY_W)

#define WPL_IS_SHARED_MEMORY(v) WPL_SHARED_MEMORY_V(v)

#define WPL_PHYS_RANGE_S      0
#define WPL_PHYS_RANGE_W      6
#define WPL_PHYS_RANGE_F(v) WPL_FIELD(v, WPL_PHYS_RANGE_S)
#define WPL_PHYS_RANGE_V(f) \
   WPL_VALUE(f, WPL_PHYS_RANGE_S, WPL_PHYS_RANGE_W)

#define WPL_RANGE_INDEX(v) WPL_PHYS_RANGE_V(v)

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/


typedef struct  {
  unsigned int wp_bus;
  char         wds_bank_name[24];
  unsigned int wp_bus_active;
  unsigned int wp_phys_bank_base_addr;
  unsigned int wp_phys_bank_size;
  unsigned int wp_application_bank_base_addr;
  unsigned int wp_application_bank_size;
} wds_winpath_mem_bank_t;

typedef struct {
  unsigned int wp_clock_freq_siu[MAX_SIU_NO];
  unsigned int wp_clock_freq_mips;
  unsigned int wds_winpath_no_of_banks;
  wds_winpath_mem_bank_t wds_winpath_mem_banks[MAX_WP_BANKS];
} wds_per_winpath_t;

typedef struct  {
  unsigned int wds_winpath_no;            // Number of WinPath devices on board
  unsigned int wds_clock_freq_external;   // Board external clock ferquency
  unsigned int wds_winpath_phys_rif_base; // uncached virtual address of WP RIF base 
  wds_per_winpath_t wds_wp[WINPATH_NUM_MAX];
  unsigned int wds_chip;
  unsigned int wds_winmon_ver;     
  unsigned int wp_address_offset[WINPATH_NUM_MAX];
} wds_board_info_t;

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

extern WP_S32 get_bus_address(WP_S32 c);
extern WP_S32 get_bus_size(WP_S32 c);
extern WP_S32 is_bus_access(WP_S32 c);
extern unsigned int host_uncached_base;
extern unsigned int end;

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

static wpi_veneer_memory wpl_bus_mappings[WINPATH_NUM_MAX][WPL_NUM_BUSES] = {{{ 0 }}}; /* Adding 1 for PHYS "bus" */
static WP_boolean        wpl_buses_mapped = WP_FALSE;
static WP_U8*            wpl_buses_vadrs[WINPATH_NUM_MAX][WPL_NUM_BUSES] = {{ 0 }};
static WP_U8             wpl_num_banks = 0;
WP_U8                    wpl_num_wp    = 0;
WP_U32                   wds_bsp_ver   = 0;
WP_U32                   wpl_rif_base[WINPATH_NUM_MAX] = { 0 };


/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

static WP_U32 win_get_characteristics(WP_U32 wp, WP_U32 bank)
{

   if ( (wp == 0) && (get_bus_size(WP_BUS_HOST + 1)==0) )
   {
      /* No "host" bus. Parameter bus is the shared bus (SPO). */
      if (bank == WP_BUS_PARAM)
         return (WPL_SHARED_MEMORY_M | 2);
   }
   else
   {
      /* Host bus exists */
      if (bank == WP_BUS_HOST && wp ==0)
         return (WPL_SHARED_MEMORY_M | 1);
   }
   /* Any other case, return a unique vbank number to each bus */
   return (wp* 10) + (bank + 2);
}

void WPLI_exit(WP_U32 i);

WP_U32 WPL_linker_end(void);

WP_U32 WPLI_BusesRifBaseGet(WP_U8 i_wpid, WP_U32* o_rif_base);

WP_U32 WPLI_BusesMapPrint(wpi_veneer_memory i_bus_mapping[]);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_ThreadInit(WP_THREAD_ID *thread_id,
                      void *(*thread_function)(void *),
                      void *thread_function_arg)
{
   static WP_boolean is_multithreading_enabled = WP_FALSE;

   if (is_multithreading_enabled == WP_FALSE)
   {
      enable_multithreading();
      is_multithreading_enabled = WP_TRUE;
   }

   win_pthread_create(thread_id, 0, thread_function, thread_function_arg);

   return WPL_OK;
}

WP_U32 WPL_ThreadDestroy(WP_THREAD_ID *thread_id)
{
   /* needs to be implemented in winmon */

   return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
}

WP_U32 WPL_ThreadSelf(WP_THREAD_ID *thread_id)
{
   *thread_id = win_pthread_self() + 1;
   return WPL_OK;
}

WP_U32 WPL_Wait(WP_U32 wait_length)
{
   if (wait_length == 0)
      wait_length = 1;
   /*    sleep(wait_length);*/
   return WPL_OK;
}

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

WP_U32 WPL_GetWinPaths(void)
{
   if(wpl_num_wp != 0)
      return wpl_num_wp;
   else 
      return 1;
}

WP_U32 WPL_BusesMapGet(wpi_veneer_memory o_bus_mapping[])
{
   WP_U32 i, bus, wp;
   WP_U8 host_bus_exist_wp0=1;
   wds_board_info_t *pWds;
   wds_board_info_t wds;

   if (o_bus_mapping == NULL)
   {
      return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }
   /* If WPL_BusesMapSet was called no need to call get_bus_address again */
   if (wpl_buses_mapped == WP_FALSE)
   {
      pWds = (wds_board_info_t*) Winmon_get_multi_wp_desc();

      memcpy(&wds, pWds, sizeof(wds));
      wpl_num_wp    = wds.wds_winpath_no;

      /* check the BSP version */
      wds_bsp_ver = wds.wds_winmon_ver;

#if WPL_BSP_VERSION_CHECK
      if(wds_bsp_ver < WPL_BSP_WINMON_VERSION)
         return WPL_ERR(WPL_ERR_ILLEGAL_BSP_VERSION);
#endif

      if(wds_bsp_ver < WPL_BSP_MULTI_WINPATH_VERSION)
      {
         for (wp = 0; wp < WINPATH_NUM_MAX; wp++)
         {
            wds.wp_address_offset[wp] = wp * 0x40000000;
         }
      }

      for (wp = 0; wp < WINPATH_NUM_MAX; wp++)
      {
         wpl_rif_base[wp] = -1;
      }

      /* Get buses addresses and sizes */
      for(wp = 0; wp < wpl_num_wp; wp++)
      {
         for (i = 0; i < MAX_WP_BANKS; i++) /* Reducing 1 for WMM5 */
         {
            bus = wds.wds_wp[wp].wds_winpath_mem_banks[i].wp_bus - 1;
            wpl_bus_mappings[wp][bus].wpid = wp;
            //board_gen_info.wds_winpath_info[wp].wp_bank_info[i].wp_bus - 1;
            switch (bus)
            {
            case WPI_BUS_RIF:
            case WP_BUS_PARAM:
            case WP_BUS_PACKET:
            case WP_BUS_INTERNAL:
               wpl_bus_mappings[wp][bus].bus = bus;
               wpl_bus_mappings[wp][bus].start = wds.wds_wp[wp].wds_winpath_mem_banks[bus].wp_phys_bank_base_addr; //get_bus_address(i + 1);
               wpl_bus_mappings[wp][bus].size = wds.wds_wp[wp].wds_winpath_mem_banks[bus].wp_phys_bank_size; //get_bus_size(i + 1);
               wpl_bus_mappings[wp][bus].characteristics = win_get_characteristics(wp, bus);


               /* Workaround for address 0xffffffff related problems */
               if ((wpl_bus_mappings[wp][bus].size) && (wpl_bus_mappings[wp][bus].start + wpl_bus_mappings[wp][bus].size == 0xffffffff))
               {
                  wpl_bus_mappings[wp][bus].size -= 16;
               }

               wpl_num_banks++;
               break;

            case WP_BUS_HOST:
               if (wp == 0 && wds.wds_wp[wp].wds_winpath_mem_banks[WP_BUS_HOST].wp_phys_bank_size == 0)
               {
                  host_bus_exist_wp0=0;
                  break;
               }

               wpl_bus_mappings[wp][bus].bus = bus;

               /* Start host memory directly after program_end which includes also heap memory */
               if (wp == 0)
               {                  
                  wpl_bus_mappings[wp][bus].start = wds.wds_wp[wp].wds_winpath_mem_banks[bus].wp_phys_bank_base_addr +
                     wds.wds_wp[wp].wds_winpath_mem_banks[bus].wp_phys_bank_size - WPL_HOST_SIZE;
                  wpl_bus_mappings[wp][bus].size = WPL_HOST_SIZE;
               }
               else
               {
                  /* for the non master WPs, the host bus can start at the beginning of the bus */
                  wpl_bus_mappings[wp][bus].start = wds.wds_wp[wp].wds_winpath_mem_banks[bus].wp_phys_bank_base_addr;
                  wpl_bus_mappings[wp][bus].size = wds.wds_wp[wp].wds_winpath_mem_banks[bus].wp_phys_bank_size;
               }

               wpl_bus_mappings[wp][bus].characteristics = win_get_characteristics(wp, bus);
                  
               /* Workaround for address 0xffffffff related problems */
               if ((wpl_bus_mappings[wp][bus].size) && (wpl_bus_mappings[wp][bus].start + wpl_bus_mappings[wp][bus].size == 0xffffffff))
               {
                  wpl_bus_mappings[wp][bus].size -= 16;
               }

               wpl_num_banks++;
               break;

            case WP_BUS_WMM5:
            case WPI_BUS_PHYS:
               wpl_bus_mappings[wp][bus].bus = bus;

               wpl_num_banks++;
               break;
            }

            /* Make sure no bus starts at physical address 0x00000000 */
            if (wpl_bus_mappings[wp][bus].start == 0 && wpl_bus_mappings[wp][bus].size != 0)
            {
               wpl_bus_mappings[wp][bus].start += 0x10;
               wpl_bus_mappings[wp][bus].size -= 0x10;               
            }            

            /* If the virtual address is set, set the offset to reflect it */
            if (wpl_buses_vadrs[wp][bus] != 0)
            {
               wpl_bus_mappings[wp][bus].cached_offset = (WP_U8 *) (wpl_buses_vadrs[wp][bus]
                     - wpl_bus_mappings[wp][bus].start);
               wpl_bus_mappings[wp][bus].uncached_offset = (WP_U8 *) (wpl_buses_vadrs[wp][bus]
                     - wpl_bus_mappings[wp][bus].start);
            }
            else if (HAS_IMPLICIT_ADDRESS(wpl_bus_mappings[wp][bus].start) && wp == 0)
            {
               wpl_bus_mappings[wp][bus].cached_offset
                     = (WP_U8 *) (WPI_MIPS_UNCACHED_OFFSET + wds.wp_address_offset[wp] );
               wpl_bus_mappings[wp][bus].uncached_offset
                     = (WP_U8 *) (WPI_MIPS_UNCACHED_OFFSET + wds.wp_address_offset[wp] );
            }
            else
            {
               wpl_bus_mappings[wp][bus].cached_offset = (WP_U8 *) (wds.wp_address_offset[wp]);
               wpl_bus_mappings[wp][bus].uncached_offset = (WP_U8 *) (wds.wp_address_offset[wp]);
            }
            wpl_bus_mappings[wp][bus].actual_start = wpl_bus_mappings[wp][bus].start;
         }
      }
   }

   
   /* Checking if PARAM bus is the only bus that exist - SPO */
   if (host_bus_exist_wp0==0)
   {
      wpl_bus_mappings[0][WP_BUS_PARAM].bus = WP_BUS_PARAM;
      wpl_bus_mappings[0][WP_BUS_HOST].bus = WP_BUS_HOST;
      wpl_bus_mappings[0][WP_BUS_PARAM].start = get_bus_address(WP_BUS_PARAM + 1) + get_bus_size(WP_BUS_PARAM + 1) - WPL_HOST_SIZE;
      wpl_bus_mappings[0][WP_BUS_PARAM].size  = WPL_HOST_SIZE;
      wpl_bus_mappings[0][WP_BUS_PARAM].characteristics = win_get_characteristics(wp, WP_BUS_PARAM);
      wpl_bus_mappings[0][WP_BUS_HOST].start = get_bus_address(WP_BUS_HOST + 1);
      wpl_bus_mappings[0][WP_BUS_HOST].size  = get_bus_size(WP_BUS_HOST + 1);
      wpl_bus_mappings[0][WP_BUS_HOST].characteristics = win_get_characteristics(wp, WP_BUS_HOST);
   }

   /* Copy the results to the out param */
   memcpy(o_bus_mapping, wpl_bus_mappings, sizeof(wpi_veneer_memory) * WPI_BUS_LAST * wpl_num_wp);

   /* get RIF base */
   for (wp = 0; wp < wpl_num_wp; wp++)
   {
      wpl_rif_base[wp] = wpl_bus_mappings[wp][WPI_BUS_RIF].start;
      wpl_rif_base[wp] += (WP_U32) wpl_bus_mappings[wp][WPI_BUS_RIF].uncached_offset;
   }
   
   wpl_buses_mapped = WP_TRUE;

#if WPL_BUSES_MAP_DEBUG
   for(wp = 0; wp < wpl_num_wp; wp++)
   {
      printf("WP %d BusesMap\n", wp);
      WPLI_BusesMapPrint(wpl_bus_mappings[wp]);
   }
#endif
   return WPL_OK;
}

WP_U32 WPL_BusesUnmap()
{
   /* Make sure the flag is not set */
   wpl_buses_mapped = WP_FALSE;
   return WPL_OK;
}

WP_U32 WPL_BusesMapSet( wpi_veneer_memory i_bus_mapping[])
{
   /* Set the static bus mapping to the new given values */
   memcpy(wpl_bus_mappings[0], i_bus_mapping, sizeof(wpi_veneer_memory)
         * WINPATH_NUM_BUSES);

   /* Make sure the flag is set */
   wpl_buses_mapped = WP_TRUE;

   return WPL_OK;
}

WP_U32 WPLI_BusesVadrsSetWpid(WP_U8 i_wpid, WP_U8* i_buses_vadrs[])
{
   WP_U32 ii;
   WP_U32 size_left, size_to_alloc;
   WP_U32 next_phys;
   WP_U32 next_vadrs;

   WP_U32 winmon_ret;

   WinMon_invalidate_mmu(1);

   /* Add the virtual range for each bus */
   for (ii = 0; ii < WPI_BUS_LAST; ii++)
   {
      wpl_buses_vadrs[i_wpid][ii] = i_buses_vadrs[ii];

      size_left = wpl_bus_mappings[i_wpid][ii].size;
      next_phys = wpl_bus_mappings[i_wpid][ii].actual_start;
      next_vadrs = (WP_U32) i_buses_vadrs[ii];

      while (size_left > 0)
      {
         /* Find the biggest size block from the possible 0x1/4/10/40/100 */
         /* That is <= the req. size, and allocate it                     */
         if (size_left >= 0x100 * WPL_MEGABYTE)
         {
            size_to_alloc = 0x100;
         }
         else if (size_left >= 0x40 * WPL_MEGABYTE)
         {
            size_to_alloc = 0x40;
         }
         else if (size_left >= 0x10 * WPL_MEGABYTE)
         {
            size_to_alloc = 0x10;
         }
         else if (size_left >= 0x4 * WPL_MEGABYTE)
         {
            size_to_alloc = 0x4;
         }
         else if (size_left >= 0x1 * WPL_MEGABYTE)
         {
            size_to_alloc = 0x1;
         }
         else
         {
            /* No smaller chunk - use 1 meg */
            size_to_alloc = 0x1;
         }

         winmon_ret
               = WinMon_set_mmu_map(next_phys, next_vadrs, size_to_alloc * WPL_MEGABYTE, WPL_CACHE_DISABLE);

         if (winmon_ret != 0)
         {
            printf("PLATFORM: error mapping mmu to MiniMon: %u\n", winmon_ret);
            return WPL_ERR(WPL_ERR_MMAP_FAILED);
         }

         /* Update the alloc vars */
         if (size_left > (size_to_alloc * WPL_MEGABYTE))
         {
            size_left -= (size_to_alloc * WPL_MEGABYTE);

            next_phys += size_to_alloc * WPL_MEGABYTE;
            next_vadrs += size_to_alloc * WPL_MEGABYTE;

            size_to_alloc = 0;
         }
         else
         {
            /* We're finished */
            size_left = 0;
         }
      }

   }

   /* Refresh the mmu to consider the new mappings */
   winmon_ret = WinMon_init_mmu();
   if (winmon_ret != 0)
   {
      printf("PLATFORM: error invalidating mmu in MiniMon\n");
      return WPL_ERR(WPL_ERR_MMAP_FAILED);
   }

   wpl_buses_mapped = WP_FALSE;

   return WPL_OK;
}

WP_U32 WPL_BusesVadrsSet(WP_U8* i_buses_vadrs[])
{
   return WPLI_BusesVadrsSetWpid(WPL_WINPATH(0), i_buses_vadrs);
}

WP_U32 WPL_BusesNeedRemap()
{
   wpl_buses_mapped = WP_FALSE;

   return WPL_OK;
}

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
   WP_U32 wpid = index / WPI_BUS_LAST;
   WP_U32 wpid_index = index % WPI_BUS_LAST;

   /* index too large */
   if (wpid_index  >= WPI_BUS_LIMIT)
   {
      if (wpl_num_wp-1 == wpid)
         return wpi_vbank_invalid;
      else
         return wpi_vbank_invisible;
   }
   
   /* Reorder the line numbering to conform to WDDI assumption */
   /* of Host, param, packet, internal...                      */
   if (wpid_index == 0)
   {
      memcpy(line, &wpl_bus_mappings[wpid][WP_BUS_HOST], sizeof(wpi_veneer_memory));
   }
   else
   {
      memcpy(line, &wpl_bus_mappings[wpid][wpid_index - 1], sizeof(wpi_veneer_memory));
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
 * Function: WPL_GetBspBusAssignment
 *
 * Purpose: Dummy WDDI / BSP interface function
 *
 ****************************************************************************/

enum wpi_vbank_result WPL_GetBspBusAssignment(WP_U32 whichBusType, WP_U8 *bus,
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

WP_U32 WPL_DriverInit(void)
{
   wpi_veneer_memory bus_mapping[WINPATH_NUM_MAX][WPI_BUS_LAST];

   /* Call the platform function to get bus information from the kernel */
   WP_U32 status = WPL_BusesMapGet((wpi_veneer_memory *)bus_mapping);
   if (status != WPL_OK)
   {
      return status;
   }

   WinMon_enable_interrupts();

   return WPL_OK;
}

WP_U8* WPL_RegBaseGet(WP_U8 i_wpid)
{
   if (i_wpid < wpl_num_wp)
   {
      WP_U32 result;
      WPLI_BusesRifBaseGet(i_wpid, &result);
      return (WP_U8*) (WP_U8*)result;
   }

   return (WP_U8*) -1;
}

WP_U8* WPL_RegOffsetGet(WP_U8 i_wpid)
{
   if (i_wpid < wpl_num_wp)
   {
      return (WP_U8*) wpl_bus_mappings[i_wpid][WPI_BUS_RIF].uncached_offset;
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

void WPL_CacheFlush(WP_U8 *address, WP_U32 nBytes)
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

void WPL_BrgAvailability(WP_U32 wpid, WP_U32 *brg9_is_available,
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
   WPLI_exit(1);
}

/******************************************************************/
/*                       Internal Functions                       */
/******************************************************************/

WP_U8* WPLI_PhysOffsetGet(WP_U8 i_wpid)
{
   if (i_wpid < wpl_num_wp)
   {
      return (WP_U8*) wpl_bus_mappings[i_wpid][WPI_BUS_PHYS].uncached_offset;
   }

   return (WP_U8*) -1;
}

/******************************************************************/
/*                       Static functions                         */
/******************************************************************/

/* coverity[+kill] */
void WPLI_exit(WP_U32 i)
{
   exit(i);
}

WP_U32 WPL_linker_end(void)
{
   return (WP_U32) &end;
}

WP_U32 WPLI_BusesRifBaseGet(WP_U8 i_wpid, WP_U32* o_rif_base)
{
   if (wpl_rif_base[i_wpid] == (WP_U32) NULL)
   {
/*      wpl_rif_base[i_wpid] = get_bus_address(WPI_BUS_RIF + 1);
      if (HAS_IMPLICIT_ADDRESS(wpl_rif_base[0]))
      {
         wpl_rif_base[i_wpid] += (WP_U32) WPI_MIPS_UNCACHED_OFFSET;
      }
*/
      wpi_veneer_memory bus_mapping[WINPATH_NUM_MAX][WPI_BUS_LAST];

      WPL_BusesMapGet(&(bus_mapping[0][0]));
   }
   *o_rif_base = wpl_rif_base[i_wpid];

   return WPL_OK;
}

WP_U32 WPLI_BusesMapPrint(wpi_veneer_memory i_bus_mapping[])
{
   WP_U32 ii;

   for (ii = 0; ii < WPI_BUS_LAST; ii++)
   {
      printf(
            "%u: bus %u bank %u characteristics 0x%x start 0x%x (actual 0x%x) size 0x%x cached 0x%x uncached 0x%x \n",
            ii, i_bus_mapping[ii].bus, i_bus_mapping[ii].bank,
            i_bus_mapping[ii].characteristics, i_bus_mapping[ii].start,
            i_bus_mapping[ii].actual_start, i_bus_mapping[ii].size,
            (WP_U32) i_bus_mapping[ii].cached_offset,
            (WP_U32) i_bus_mapping[ii].uncached_offset);
   }

   return 0;
}
