/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux_winpath2/sources/wpl_platform.c
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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/sem.h>

#include "wpl_types.h"
#include <wpl_platform.h>
#include <wpl_sw_semaphore.h>
#include <wpl_board.h>


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WINPATH_NUM_BUSES 4

#define WPL_SIGNAL_RT_SER 35 /*SIGRTMIN*/
#define WPL_SIGNAL_RT_OV (WPL_SIGNAL_RT_SER + 1)
#define WPL_SIGNAL_RT_BUSERR (WPL_SIGNAL_RT_SER + 2)

#define WPL_ACCEPT_INTERRUPTS 0

#if (WPL_ACCEPT_INTERRUPTS == 1)
#define WPL_OPEN_FLAG O_RDWR
#else
#define WPL_OPEN_FLAG O_RDWR | O_NOFOLLOW
#endif

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

static struct sigaction siginfo;

struct WPL_bus_phys_memory_layout {
      WP_U32 bus;
      WP_U32 active;
      WP_U32 phys_base;
      WP_U32 phys_size;
      WP_U32 use_this_base;
      WP_U32 use_this_size;
      WP_U32 mmap_size;
};

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/


/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

WP_S32                            fd[WP_BUS_NONE] = { 0 };
struct WPL_bus_phys_memory_layout winpath_mem_map[WINPATH_NUM_BUSES];/*4 buses on the Winpath2*/
static wpi_veneer_memory          wpl_bus_mappings[WINPATH_NUM_BUSES] = {{0}};
static WP_boolean                 wpl_buses_mapped   = WP_FALSE;
static WP_U8*                     wpl_buses_vadrs[WINPATH_NUM_BUSES] = {0};
static WP_U32                     wpl_mmap_size[WINPATH_NUM_BUSES] = {0};
static WP_U8*                     wpl_app_phys_address = 0;

WP_U32        wpl_rif_base[WINPATH_NUM_MAX] = { 0 };
static WP_U32 wpl_first_time = 1;

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

WP_U32 win_get_characteristics(WP_U32 bank)
{
   if (bank == 3)
      return 0x81;
   else
      return bank + 2;
}
/* TMP win_get_characteristics - revert to a BSP function */

WP_U32 WPLI_BusesActualUnmap(void);
WP_U32 WPLI_IsSedUsed(void);

/* Debug functions */
WP_U32 WPLI_BusesMapPrint(wpi_veneer_memory i_bus_mapping[]);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_BusesMapGet(wpi_veneer_memory o_bus_mapping[])
{
   WP_U32 *vadr;
   WP_U32 i;
   /* If WPL_BusesMapSet was called no need to call ioctl again */
   if (wpl_buses_mapped == WP_TRUE)
   {
      memcpy(o_bus_mapping, wpl_bus_mappings, sizeof(wpi_veneer_memory) * WINPATH_NUM_BUSES);
      
      return WPL_OK;
   }

   if (((fd[0] = open("/dev/wp_siu1", WPL_OPEN_FLAG))<0) ||
       ((fd[1] = open("/dev/wp_siu2", WPL_OPEN_FLAG))<0) ||
       ((fd[2] = open("/dev/wp_siu3", WPL_OPEN_FLAG))<0) ||
       ((fd[3] = open("/dev/wp_siu4", WPL_OPEN_FLAG))<0))
   {
      perror("open");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }

   if (ioctl(fd[WP_BUS_INTERNAL-1], WPL_IOCTL_READ_PHYS_MEM_LAYOUT, winpath_mem_map) < 0)
   {
      return WPL_ERR(WPL_ERR_PLATFORM);
   }
   
   /* Copy bus info to veneer_memory struct */
   for(i = 0; i < WINPATH_NUM_BUSES; i++)
   {
      wpl_bus_mappings[i].bus           = winpath_mem_map[i].bus - 1; /*Numbering of the buses start at 1*/
      wpl_bus_mappings[i].actual_start  = winpath_mem_map[i].use_this_base;
      wpl_bus_mappings[i].start         = winpath_mem_map[i].use_this_base;
      wpl_bus_mappings[i].size          = winpath_mem_map[i].use_this_size;
      wpl_bus_mappings[i].characteristics = win_get_characteristics(i);
      wpl_bus_mappings[i].bank          = wpl_board_app_banks[i];
      wpl_mmap_size[i]                  = winpath_mem_map[i].mmap_size;

      /* Workaround for address 0xffffffff related problems */
      if ((wpl_bus_mappings[i].size) && (wpl_bus_mappings[i].start + wpl_bus_mappings[i].size == 0xffffffff))
      {
         wpl_bus_mappings[i].size -= 16;
      }

      if (WP_BUS_HOST == i)
      {
         wpl_app_phys_address = (WP_U8*)winpath_mem_map[i].phys_size;
      }
   }

   for (i = 0; i < WINPATH_NUM_BUSES ; i++)
   {
      WP_U32 size_to_map;
      if (wpl_mmap_size[i] == 0xffffffff)/*0xffffffff is an invalid value that should be ignored*/
      {
         size_to_map = wpl_bus_mappings[i].size;
      }
      else
      {
         size_to_map = wpl_mmap_size[i];
      }

      if (wpl_buses_vadrs[i] != NULL)
      {
         vadr = mmap(wpl_buses_vadrs[i], size_to_map, PROT_WRITE, MAP_SHARED | MAP_FIXED, fd[i], 0);
      }
      else
      {
         vadr = mmap(wpl_buses_vadrs[i], size_to_map, PROT_WRITE, MAP_SHARED, fd[i], 0);
      }

      if (vadr == MAP_FAILED)
      {
         perror("mmap");
         return WPL_ERR(WPL_ERR_MMAP_FAILED);
      }

      wpl_buses_vadrs[i] = (WP_U8*) vadr;
      wpl_bus_mappings[i].uncached_offset = (WP_U8 *)((WP_U32)vadr - wpl_bus_mappings[i].start);
      wpl_bus_mappings[i].cached_offset = wpl_bus_mappings[i].uncached_offset;
   }

   wpl_rif_base[0] = wpl_bus_mappings[WP_BUS_INTERNAL].actual_start + WPL_INTERNAL_BUS_RIF_BASE;
   wpl_rif_base[0] += (WP_U32) wpl_bus_mappings[WP_BUS_INTERNAL].uncached_offset;

   /* The start and size should not be altered after the first time. */
   wpl_buses_mapped = WP_TRUE;

   memcpy(o_bus_mapping, wpl_bus_mappings, sizeof(wpi_veneer_memory) * WINPATH_NUM_BUSES);

   return WPL_OK;
}

WP_U32 WPL_BusesMapSet(wpi_veneer_memory i_bus_mapping[])
{
   /* Set the static bus mapping to the new given values */
   memcpy(wpl_bus_mappings, i_bus_mapping, sizeof(wpi_veneer_memory) * WINPATH_NUM_BUSES);

   /* Make sure the flag is set */
   wpl_buses_mapped = WP_TRUE;

   return WPL_OK;
}

WP_U32 WPL_BusesVadrsSet(WP_U8* i_buses_vadrs[])
{
   WP_U32 ii;

   /* Copy the virtual addresses */
   for (ii = 0; ii < WINPATH_NUM_BUSES; ii++)
   {
      wpl_buses_vadrs[ii] = i_buses_vadrs[ii];
   }

   wpl_buses_mapped = WP_FALSE;

   return WPL_OK;
}

WP_U32 WPL_BusesNeedRemap()
{
   wpl_buses_mapped = WP_FALSE;

   return WPL_OK;
}

enum wpi_vbank_result WPL_GetVbank(WP_U8 index, wpi_veneer_memory *line)
{
   /* index too large */
   if (index >= WPI_BUS_LIMIT)
      return wpi_vbank_invalid;

   /* Reorder the line numbering to conform to WDDI assumption */
   /* of Host, param, packet, internal...                      */
   if (index == 0)
   {
      memcpy(line, &wpl_bus_mappings[3], sizeof(wpi_veneer_memory));
      line->size = WPL_HOST_SIZE;
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

WP_U32 WPL_BusesUnmap()
{
   WP_U32 i, size_to_unmap;

   for (i = 0; i < WINPATH_NUM_BUSES; i++)
   {
      if (wpl_mmap_size[i] == 0xffffffff)/*0xffffffff is an invalid value that should be ignored*/
      {
         size_to_unmap = wpl_bus_mappings[i].size;
      }
      else
      {
         size_to_unmap = wpl_mmap_size[i];
      }

      if (munmap(wpl_buses_vadrs[i], size_to_unmap) == -1)
      {
         perror("munmap");
         return WPL_ERR(WPL_ERR_MMAP_FAILED);
      }
   }

   /* Make sure the flag is not set */
   wpl_buses_mapped = WP_FALSE;

   return WPL_OK;
}

/******************************************************************/
/** @ingroup Platform
 * \brief Creates a thread
 *
 * Creates a thread using POSIX pthread_create
 * \return \arg \a WPL_OK on success
 */
/******************************************************************/
WP_U32 WPL_ThreadInit(WP_THREAD_ID *thread_id,
                      void *(*thread_function)(void *),void *thread_function_arg)
{
   pthread_t thread;
   int iret;

   /* This creates a thread and stores it's id in thread */
   iret = pthread_create(&thread,NULL,thread_function,thread_function_arg);

   *thread_id = thread;

   if(iret == 0)
      return 0;
   else
      return 1;
}


WP_U32 WPL_ThreadDestroy(WP_THREAD_ID *thread_id)
{
   int status;
   status = pthread_cancel(*thread_id);

   if(status == 0)
      return 0;
   else
      return 1;
}

WP_U32 WPL_ThreadSelf(WP_THREAD_ID *thread_id)
{
   pthread_t thread;
   thread = pthread_self();
   *thread_id = (WP_THREAD_ID)thread;
   return 0;
}

WP_U32 WPL_Wait(WP_U32 wait_length)
{
   if (wait_length == 0)
      wait_length = 1;
   sleep(wait_length);
   return 0;
}

void WPL_DriverRelease(void)
{
   sigaction(WPL_SIGNAL_RT_SER, NULL, &siginfo);
   siginfo.sa_handler = SIG_DFL;
   siginfo.sa_flags = 0;
   sigaction(WPL_SIGNAL_RT_SER, &siginfo, NULL);

   sigaction(WPL_SIGNAL_RT_OV, NULL, &siginfo);
   siginfo.sa_handler = SIG_DFL;
   siginfo.sa_flags = 0;
   sigaction(WPL_SIGNAL_RT_OV, &siginfo, NULL);

   if ((close(fd[0]) < 0) || (close(fd[1]) < 0) || (close(fd[2]) < 0)
         || (close(fd[3]) < 0))
   {
      perror("close");
   }
   else
   {
      if (WPL_BusesUnmap() != WPL_OK)
      {
         perror("munmap");
      }
   }
}

WP_U32 WPL_EventInit(void)
{
   if (WPLI_IsSedUsed())
   {
      WPLI_LOG(("SED in USE\n"));
   }
   return WPL_OK;
}

WP_U32 WPL_DriverInit(void)
{
   WP_U32 value = 1;
   wpi_veneer_memory bus_mapping[WPI_BUS_LAST];
   WP_U32 status;
   WP_S32 local_fd;

   status = WPLI_IntConnect();
   if (status != WPL_OK) {
      return status;
   }

   if ((local_fd = open("/dev/wp_siu1", WPL_OPEN_FLAG)) < 0)
   {
      perror("open");
      return WPL_ERR(WPL_ERR_INIT);
   }

   if (ioctl(local_fd,
         WPL_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL, &value) < 0)
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);

   close(local_fd);

   /* Call the platform function to get bus information from the kernel */
   if (WPL_BusesMapGet(bus_mapping) != WPL_OK)
   {
      return WPL_ERR(WPL_ERR_INIT);
   }

   if (WPLI_IsSedUsed() && wpl_first_time)
   {
      WP_VU32 *sed_map = NULL;

      /* the memory consumption info is located at the end of the internal bus
         memory mapping (relevant only in SED mode) */
      sed_map = (WP_VU32 *) (wpl_bus_mappings[WP_BUS_INTERNAL].actual_start
            + WPL_INT_MEM_RAM_OFFSET
            + wpl_bus_mappings[WP_BUS_INTERNAL].uncached_offset
            + wpl_bus_mappings[WP_BUS_INTERNAL].size);
      wpl_bus_mappings[WP_BUS_HOST].size = wpl_bus_mappings[WP_BUS_HOST].size
            - (((WP_U32)wpl_app_phys_address) - *(--sed_map)); /* Host bus     */
      wpl_bus_mappings[WP_BUS_INTERNAL].size = *(--sed_map); /* Inetrnal bus */
      wpl_bus_mappings[WP_BUS_PACKET].size   = *(--sed_map); /* Packet bus   */
      wpl_bus_mappings[WP_BUS_PARAM].size    = *(--sed_map); /* Param bus    */
   }

   /* The start and size should not be altered after the first time. */
   wpl_first_time = 0;

   return WPL_OK;
}

WP_U8* WPL_RegBaseGet(WP_U8 i_wpid)
{
   if (wpl_rif_base[i_wpid] == 0) {
      WPLI_BusesRifBaseGet(&wpl_rif_base[i_wpid]);
   }

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

/*****************************************************************************
 *
 * Function: WPL_OsService
 *
 * Purpose: Indicate non-use of ENET1 to WDDI
 *
 ****************************************************************************/

WP_U32 WPL_OsService(WPL_OsServiceType action, void *holder)
{
   WP_U32 suppress_dup_signal;

   if (action == WPL_OsServiceGetDriverInfo)
   {
      if (WPLI_IsSedUsed())
         *(WPL_os_enet_usage *) holder = WPL_os_enet_keepalive;
      else
         *(WPL_os_enet_usage *) holder = WPL_os_enet_unused;
      return WPL_OK;
   }
   else if (action == WPL_OsServiceDupIntrSignalAllow)
   { /* Allow duplicate signals */
      if (wpl_first_time)
         return WPL_ERR(WPL_ERR_WINPATH_UNINITIALIZED);
      else
      {
         suppress_dup_signal = WP_FALSE; /* Do not suppress */
         if (ioctl(fd[WP_BUS_INTERNAL], WPL_IOCTL_SUPPRESS_DUP_SIGNAL,
               &suppress_dup_signal) < 0)
            return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);

         return WPL_OK;
      }
   }
   else if (action == WPL_OsServiceDupIntrSignalSuppress)
   { /* Suppress duplicate signals */
      if (wpl_first_time)
         return WPL_ERR(WPL_ERR_WINPATH_UNINITIALIZED);
      else
      {
         suppress_dup_signal = WP_TRUE; /* Suppress */
         if (ioctl(fd[WP_BUS_INTERNAL], WPL_IOCTL_SUPPRESS_DUP_SIGNAL,
               &suppress_dup_signal) < 0)
            return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);

         return WPL_OK;
      }
   }

   return WPL_ERR(WPL_ERR_INVALID_PARAM);
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

   switch (whichBusType)
   {
   case WPL_VB_BSP_EVENT_QUEUE:
      *bus = WP_BUS_INTERNAL;
      break;
   case WPL_VB_BSP_FIFO_QUEUE:
      *bus = WP_BUS_PARAM;
      break;
   default:
      return wpi_vbank_invalid;

   }
   return wpi_vbank_valid;
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
   exit(1);
}

/******************************************************************/
/*                       Internal Functions                       */
/******************************************************************/

WP_U8* WPLI_PhysOffsetGet(WP_U8 i_wpid)
{
   if (i_wpid == 0)
   {
      return (WP_U8*) wpl_bus_mappings[WP_BUS_INTERNAL].uncached_offset;
   }

   return (WP_U8*) -1;
}

WP_U32 WPLI_BusesRifBaseGet(WP_U32* o_rif_base)
{
   if (wpl_rif_base[0] == 0)
   {
      WPL_BusesMapGet(wpl_bus_mappings);
   }

   *o_rif_base = wpl_rif_base[0];

   return WPL_OK;
}

WP_U32 WPLI_BusesMapPrint(wpi_veneer_memory i_bus_mapping[])
{
   WP_U32 ii;
   
   for (ii = 0; ii < WINPATH_NUM_BUSES; ii++)
   {
      printf("%u: wpid %u bus %u bank %u ch 0x%x start 0x%x (actual 0x%x) size 0x%x ch 0x%x unch 0x%x mmap 0x%x\n",
             ii, i_bus_mapping[ii].wpid, i_bus_mapping[ii].bus, i_bus_mapping[ii].bank, 
             i_bus_mapping[ii].characteristics, i_bus_mapping[ii].start, i_bus_mapping[ii].actual_start, i_bus_mapping[ii].size, 
             (WP_U32)i_bus_mapping[ii].cached_offset, (WP_U32)i_bus_mapping[ii].uncached_offset, wpl_mmap_size[ii]);
   }

   return 0;
}

WP_U32 WPLI_IsSedUsed(void)
{
   WP_U32 value = system("/sbin/ifconfig | grep WP-SED> /dev/null");
   return ((value == 0) ? 1 : 0);
}
