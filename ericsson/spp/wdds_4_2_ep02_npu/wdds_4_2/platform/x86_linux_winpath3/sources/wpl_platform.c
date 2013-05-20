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

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/sem.h>

#include "wpl_types.h"
#include <wpl_platform.h>
#include <wpli_platform.h>
#include <wpl_interrupts.h>
#include <pthread.h> 
#include <wpl_sw_semaphore.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/
#define WPL_BUSES_MAP_DEBUG 0

/* We should have reduced by 1 because of WP_BUS_WMM5 and increase by 1 because of the PHYS "bus" */
#define WPL_NUM_BUSES WPI_BUS_LAST

#define WPL_SIGNAL_RT_SER                35 /*SIGRTMIN+3*/
#define WPL_SIGNAL_RT_OV         (WPL_SIGNAL_RT_SER + 1)
#define WPL_SIGNAL_RT_BUSERR     (WPL_SIGNAL_RT_SER + 2)
#define WPL_SIGNAL_RT_SERDES             44
#define WPL_SIGNAL_RT_SGMII_AN           45
#define WPL_SIGNAL_RT_UFE4_FRAMER        46

// XALYO
//#define MULTICORE           1
#define MULTICORE           0

#define WPL_BUS_NAME_LEN    24

#define WPL_ACCEPT_INTERRUPTS 1

#if (WPL_ACCEPT_INTERRUPTS == 1)
#define WPL_OPEN_FLAG O_RDWR
#else
#define WPL_OPEN_FLAG O_RDWR | O_NOFOLLOW
#endif


/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

static struct sigaction siginfo;

struct wpl_bus_phys_memory_layout
{
   WP_U32 bus;
   WP_U32 active;
   WP_U32 phys_base;
   WP_U32 phys_size;
   WP_U32 application_base;
   WP_U32 application_reserved_size;
};

typedef struct
{
   unsigned int wp_bus;
   unsigned int wp_bus_active;
   unsigned int wp_phys_bank_base_addr;
   unsigned int wp_phys_bank_size;
   unsigned int wp_application_bank_base_addr;
   unsigned int wp_application_bank_size;
} wds_winpath_mem_bank_info_t;

typedef struct
{
   unsigned int wp_id;
   unsigned int wp_clock_freq_siu[MAX_SIU_NO];
   unsigned int wp_clock_freq_mips;
   unsigned int wds_winpath_no_of_banks;
   wds_winpath_mem_bank_info_t wp_bank_info[MAX_WP_BANKS];
} wds_winpath_info_t;

typedef struct
{
   unsigned int wds_winpath_no; /* Number of WinPath devices on board */
   unsigned int wds_clock_freq_external; /* Board external clock ferquency */
   wds_winpath_info_t wds_winpath_info[WINPATH_NUM_MAX];
   unsigned int wds_chip;
   unsigned int wds_winmon_ver;
} wds_board_info_t;

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

WP_S32                   fd[WPL_NUM_BANKS];
struct wpl_bus_phys_memory_layout winpath_mem_map[WPL_NUM_BUSES];
static wpi_veneer_memory wpl_bus_mappings[WINPATH_NUM_MAX][WPL_NUM_BUSES] = {{{ 0 }}}; /* Adding 1 for PHYS "bus" */
static WP_boolean        wpl_buses_mapped = WP_FALSE;
static WP_boolean        wpl_buses_need_to_unmap = WP_FALSE;
static WP_U8*            wpl_buses_vadrs[WINPATH_NUM_MAX][WPL_NUM_BUSES] = {{ 0 }};
static WP_U32            wpl_first_time = 1;
static WP_U8             wpl_num_banks = 0;
WP_U32                   wds_bsp_ver   = 0;
WP_U8                    wpl_num_wp    = 0;
WP_U32                   clock_freq_internal = 0;
WP_U32                   wpl_rif_base[WINPATH_NUM_MAX] = { 0 };
wds_board_info_t         board_gen_info;


/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/
WP_U32 WPLI_BoardInfoPrint(wds_board_info_t *board_info)
{
   WP_U32 wp, ii;

   printf("winpathes num %u cf_ext %u\n", board_info->wds_winpath_no, board_info->wds_clock_freq_external);

   for (wp = 0; wp < board_info->wds_winpath_no; wp++)
   {
      printf("\nwp %u: wpid %u cl_mips %u no banks %u\n",
             wp, wp,
             board_info->wds_winpath_info[wp].wp_clock_freq_mips,
             board_info->wds_winpath_info[wp].wds_winpath_no_of_banks);

      for(ii=0; ii<MAX_SIU_NO; ii++)
         printf("siu %u clk_freq %u   ", ii,
                board_info->wds_winpath_info[wp].wp_clock_freq_siu[ii]);

      for(ii=0; ii<board_info->wds_winpath_info[wp].wds_winpath_no_of_banks; ii++)
         printf("\n bank %u bus %u active %u phys_addr %08x phys_size %08x app_addr %08x app_size %08x ",
                ii, board_info->wds_winpath_info[wp].wp_bank_info[ii].wp_bus,
                board_info->wds_winpath_info[wp].wp_bank_info[ii].wp_bus_active,
                board_info->wds_winpath_info[wp].wp_bank_info[ii].wp_phys_bank_base_addr,
                board_info->wds_winpath_info[wp].wp_bank_info[ii].wp_phys_bank_size,
                board_info->wds_winpath_info[wp].wp_bank_info[ii].wp_application_bank_base_addr,
                board_info->wds_winpath_info[wp].wp_bank_info[ii].wp_application_bank_size);
   }
   printf("\n\n");
   
   return 0;
}

static WP_U32 win_get_characteristics(WP_U32 wp, WP_U32 bank)
{
   if (bank == 3 &&wp ==0)//host bus in WP #1
      return (WPL_SHARED_MEMORY_M | 1); 
   else
      return (wp* 10) + (bank + 2);
}
/* TMP win_get_characteristics - revert to a BSP function */

WP_U32 WPLI_BusesActualUnmap(void);
WP_U32 WPLI_IsSedUsed(void);

/* Debug functions */
WP_U32 WPLI_BusesMapPrint(wpi_veneer_memory i_bus_mapping[]);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_GetProcessorSpeed()
{
   return clock_freq_internal;
}

WP_U32 WPL_GetWinPaths(void)
{
   static wpi_veneer_memory bus_mappings[WINPATH_NUM_MAX][WPI_BUS_LAST] = {{{0}}};
   if(wpl_num_wp == 0)
   {/* if wpl_num_wp is not initialized (for example in winutil), initialize it */
      WPL_BusesMapGet(&(bus_mappings[0][0]));
   }
   return wpl_num_wp;
}

WP_U32 WPL_BusesMapGet(wpi_veneer_memory o_bus_mapping[])
{
   WP_U32 i, size_to_map;
   WP_U32 bus, wp, fd_num;
   WP_U32 *vadr;

   if (wpl_buses_mapped == WP_TRUE)
   {
      memcpy(o_bus_mapping, wpl_bus_mappings, sizeof(wpi_veneer_memory)
            * WPL_NUM_BUSES * wpl_num_wp); /* add multi winpath support (WPL_NUM_BANKS) */

      return WPL_OK;
   }

   if (wpl_buses_need_to_unmap == WP_TRUE)
   {
      WPLI_BusesActualUnmap();
   }

   if (((fd[0]  = open("/dev/wp_bank1",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[1]  = open("/dev/wp_bank2",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[2]  = open("/dev/wp_bank3",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[3]  = open("/dev/wp_bank4",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[4]  = open("/dev/wp_bank5",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[5]  = open("/dev/wp_bank6",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[6]  = open("/dev/wp_bank7",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[7]  = open("/dev/wp_bank8",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[8]  = open("/dev/wp_bank9",  WPL_OPEN_FLAG)) < 0) ||
       ((fd[9]  = open("/dev/wp_bank10", WPL_OPEN_FLAG)) < 0) ||
       ((fd[10] = open("/dev/wp_bank11", WPL_OPEN_FLAG)) < 0) ||
       ((fd[11] = open("/dev/wp_bank12", WPL_OPEN_FLAG)) < 0) ||
       ((fd[12] = open("/dev/wp_bank13", WPL_OPEN_FLAG)) < 0) ||
       ((fd[13] = open("/dev/wp_bank14", WPL_OPEN_FLAG)) < 0) ||
       ((fd[14] = open("/dev/wp_bank15", WPL_OPEN_FLAG)) < 0) ||
       ((fd[15] = open("/dev/wp_bank16", WPL_OPEN_FLAG)) < 0) ||
       ((fd[16] = open("/dev/wp_bank17", WPL_OPEN_FLAG)) < 0) ||
       ((fd[17] = open("/dev/wp_bank18", WPL_OPEN_FLAG)) < 0) ||
       ((fd[18] = open("/dev/wp_bank19", WPL_OPEN_FLAG)) < 0) ||
       ((fd[19] = open("/dev/wp_bank20", WPL_OPEN_FLAG)) < 0) ||
       ((fd[20] = open("/dev/wp_bank21", WPL_OPEN_FLAG)) < 0) ||
       ((fd[21] = open("/dev/wp_bank22", WPL_OPEN_FLAG)) < 0) ||
       ((fd[22] = open("/dev/wp_bank23", WPL_OPEN_FLAG)) < 0) ||
       ((fd[23] = open("/dev/wp_bank24", WPL_OPEN_FLAG)) < 0))
   {
      printf("WPL_BusesMapGet: Couldn't open wp_bankXX\n");
      perror("open");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }

   if (ioctl(fd[WP_BUS_INTERNAL], WPL_IOCTL_READ_BOARD_GEN_CONF, &board_gen_info) < 0)/* assumed that internal bus on wp0 is always there... */
   {
      return WPL_ERR(WPL_ERR_PLATFORM);
   }
/*   WPLI_BoardInfoPrint(&board_gen_info); */

   /* get the BSP version */
   wds_bsp_ver = board_gen_info.wds_winmon_ver;


   /* Get the number of active WPs */
   wpl_num_wp = board_gen_info.wds_winpath_no;

   clock_freq_internal
         = board_gen_info.wds_winpath_info[0].wp_clock_freq_siu[WP_BUS_INTERNAL];

   /* Init the internal bus struct bus id */
   for (wp = 0; wp < wpl_num_wp; wp++)
   {
      for (i = 0; i < WPL_NUM_BUSES; i++)
         wpl_bus_mappings[wp][i].bus = i;
   }

   /****************************************/

   /* Copy bus info to veneer_memory struct */
   for (wp = 0; wp < wpl_num_wp; wp++)
   {
      for (i = 0; i < MAX_WP_BANKS; i++) /* Reducing 1 for WMM5 */
      {
         bus = board_gen_info.wds_winpath_info[wp].wp_bank_info[i].wp_bus - 1;
         wpl_bus_mappings[wp][bus].wpid = wp;
         if (!(board_gen_info.wds_winpath_info[wp].wp_bank_info[i].wp_bus_active))
         {
            wpl_bus_mappings[wp][bus].bus = WP_BUS_NONE;
            continue;
         }
         
         switch (bus)
         {
            case WP_BUS_PARAM:
            case WP_BUS_PACKET:
            case WP_BUS_INTERNAL:
            case WP_BUS_HOST:
            case WPI_BUS_RIF:
            case WPI_BUS_PHYS:
               /* Numbering of the buses start at 1 */
               wpl_bus_mappings[wp][bus].bus = bus;
               wpl_bus_mappings[wp][bus].actual_start
                  = board_gen_info.wds_winpath_info[wp].wp_bank_info[i].wp_application_bank_base_addr;
               wpl_bus_mappings[wp][bus].start
                  = board_gen_info.wds_winpath_info[wp].wp_bank_info[i].wp_application_bank_base_addr;
               wpl_bus_mappings[wp][bus].size
                  = board_gen_info.wds_winpath_info[wp].wp_bank_info[i].wp_application_bank_size;
               wpl_bus_mappings[wp][bus].characteristics = win_get_characteristics(wp, bus);

               /* Workaround for address 0xffffffff related problems */
               if ((wpl_bus_mappings[wp][i].size) && (wpl_bus_mappings[wp][i].start + wpl_bus_mappings[wp][i].size == 0xffffffff))
               {
                  wpl_bus_mappings[wp][i].size -= 16;
               }
               
               wpl_num_banks++;
               break;
            case WP_BUS_WMM5:
               wpl_bus_mappings[wp][WP_BUS_WMM5].bus = bus;
               wpl_bus_mappings[wp][WP_BUS_WMM5].actual_start = 0;
               wpl_bus_mappings[wp][WP_BUS_WMM5].start = 0;
               wpl_bus_mappings[wp][WP_BUS_WMM5].size = 0;
               wpl_bus_mappings[wp][WP_BUS_WMM5].characteristics
                  = win_get_characteristics(wp, bus);
               
               /* Workaround for address 0xffffffff related problems */
               if ((wpl_bus_mappings[wp][i].size) && (wpl_bus_mappings[wp][i].start + wpl_bus_mappings[wp][i].size == 0xffffffff))
               {
                  wpl_bus_mappings[wp][i].size -= 16;
               }
               
               wpl_num_banks++;
               break;
            default:
               /* set the bus to an illegal number - so we don't mmap it */
               printf("WPL_BusesMapGet: got unknown bus id from kernel %d\n", bus);
               wpl_bus_mappings[wp][bus].bus = WP_BUS_NONE;
         }

         /* Make sure no bus starts at physical address 0x00000000 */
         if (wpl_bus_mappings[wp][bus].start == 0 && wpl_bus_mappings[wp][bus].size != 0)
         {
            wpl_bus_mappings[wp][bus].start += 0x10;
            wpl_bus_mappings[wp][bus].size -= 0x10;               
         }            
      }
   }
   
   for (wp = 0; wp < wpl_num_wp; wp++)
   {
      for (i = 0; i < WPL_NUM_BUSES; i++)
      {
         bus = wpl_bus_mappings[wp][i].bus;
         
         if ((bus != WP_BUS_PARAM) && (bus != WP_BUS_PACKET) && (bus
            != WP_BUS_INTERNAL) && (bus != WP_BUS_HOST) && (bus != WPI_BUS_RIF)
             && (bus != WPI_BUS_PHYS))
         {
            /* We don't mmap other buses */
            continue;
         }
         
         size_to_map = wpl_bus_mappings[wp][bus].size;
         
         fd_num = bus + (wp*WPI_BUS_LAST); /* to match linux mmap */
         
#if (MULTICORE)
         /* Increase size_to_map to the next MEG multiply */
         if (size_to_map % (1024*1024) != 0)
         {
            size_to_map += ((1024*1024) - (size_to_map % (1024*1024)));
         }
#endif

         if (wpl_buses_vadrs[wp][bus] != NULL)
         {
            vadr = mmap(wpl_buses_vadrs[wp][bus], size_to_map, PROT_WRITE, MAP_SHARED
                        | MAP_FIXED, fd[fd_num], 0);
         }
         else
         {
#if (MULTICORE)
            WP_U8* request_vadr = (WP_U8*)wpl_bus_mappings[wp][bus].start;
            
            vadr = mmap(request_vadr, size_to_map, PROT_WRITE, MAP_SHARED, fd[fd_num], 0);
            
            if ((((WP_U32)vadr) % size_to_map != 0) && (bus > 0))
            {
               munmap(vadr, size_to_map);
               
               request_vadr = wpl_buses_vadrs[wp][bus - 1];
               request_vadr += (size_to_map - ((WP_U32)request_vadr % size_to_map));
               if ((WP_U32)request_vadr < wpl_bus_mappings[wp][bus].start)
               {
                  request_vadr = (WP_U8*)wpl_bus_mappings[wp][bus].start;
                  request_vadr += (size_to_map - ((WP_U32)request_vadr % size_to_map));
               }
               
               vadr = mmap(request_vadr, size_to_map, PROT_WRITE, MAP_SHARED, fd[fd_num], 0);
            }
            
            while (((WP_U32)vadr % size_to_map != 0) && ((WP_U32)request_vadr < 0x80000000))
            {
               munmap(vadr, size_to_map);
               
               request_vadr += wpl_bus_mappings[wp][bus].size;
               
               vadr = mmap(request_vadr, size_to_map, PROT_WRITE, MAP_SHARED, fd[fd_num], 0);
            }
            
            if ((WP_U32)vadr % size_to_map != 0)
            {
               printf("WPL_BusesMapGet: cannot allocate aligned virtual address: bus %u, vadr 0x%x size 0x%x\n",
                      bus, (WP_U32)vadr, wpl_bus_mappings[wp][bus].size);
               return WPL_ERR(WPL_ERR_ADDRESS_ALIGNMENT);
            }
#else
            vadr = mmap(0, size_to_map, PROT_WRITE, MAP_SHARED, fd[fd_num], 0);
#endif
         }
         
         if (vadr == MAP_FAILED)
         {
            return WPL_ERR(WPL_ERR_MMAP_FAILED);
         }
         
         wpl_buses_vadrs[wp][bus] = (WP_U8*) vadr;

         wpl_bus_mappings[wp][bus].uncached_offset = 
            (WP_U8 *) ((WP_U32) vadr- wpl_bus_mappings[wp][bus].start);

         wpl_bus_mappings[wp][bus].cached_offset = 
            wpl_bus_mappings[wp][bus].uncached_offset;

#ifdef WPL_PRIVATE_HEAP_ALLOCATION
         if (bus == WPL_HEAP_BUS)
         {
            wpl_bus_mappings[bus].size -= WPL_HEAP_SIZE;
         }
#endif
      }
   }
   
   for (wp = 0; wp < wpl_num_wp; wp++)
   {
      wpl_rif_base[wp] = (WP_U32) (wpl_bus_mappings[wp][WPI_BUS_RIF].start
                                  + wpl_bus_mappings[wp][WPI_BUS_RIF].uncached_offset);
   }
   
   /* The start and size should not be altered after the first time. */
   wpl_buses_mapped = WP_TRUE;

   memcpy(o_bus_mapping, wpl_bus_mappings, sizeof(wpi_veneer_memory)
          * WPI_BUS_LAST * wpl_num_wp);

#if WPL_BUSES_MAP_DEBUG
   for(wp = 0; wp < wpl_num_wp; wp++)
   {
      printf("WP %d BusesMap\n", wp);
      WPLI_BusesMapPrint(wpl_bus_mappings[wp]);
   }
#endif

   return WPL_OK;
}

WP_U32 WPL_BusesMapSet(wpi_veneer_memory i_bus_mapping[])
{
   /* Set the static bus mapping to the new given values */
   memcpy(wpl_bus_mappings[0], i_bus_mapping, sizeof(wpi_veneer_memory)
         * WPI_BUS_LAST);

   /* Make sure the flag is set */
   wpl_buses_mapped = WP_TRUE;

   return WPL_OK;
}

WP_U32 WPLI_BusesVadrsSetWpid(WP_U8 i_wpid, WP_U8* i_buses_vadrs[])
{
   WP_U32 ii;

   /* Copy the virtual addresses */
   for (ii = 0; ii < WPI_BUS_LAST; ii++)
   {
      wpl_buses_vadrs[i_wpid][ii] = i_buses_vadrs[ii];
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

enum wpi_vbank_result WPL_GetVbank(WP_U8 index, wpi_veneer_memory *line)
{
   WP_U32 wpid = index / WPI_BUS_LAST;
   WP_U32 wpid_index = index % WPI_BUS_LAST;

   /* index too large */
   if (wpid_index >= WPI_BUS_LIMIT)
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
      line->size = WPL_HOST_SIZE;
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

WP_U32 WPLI_BusesActualUnmap()
{
   WP_U32 i, size_to_unmap, wp;

   for (wp = 0; wp < WINPATH_NUM_MAX; wp++)
   {
      for (i = 0; i < WPI_BUS_LAST; i++)
      {
         size_to_unmap = wpl_bus_mappings[wp][i].size;
#ifdef WPL_PRIVATE_HEAP_ALLOCATION
         if (i == WPL_HEAP_BUS) size_to_unmap += WPL_HEAP_SIZE;
#endif      

#if (MULTICORE)
         /* Increase size_to_map to the next MEG multiply */
         if (size_to_unmap % (1024*1024) != 0)
         {
            size_to_unmap += ((1024*1024) - (size_to_unmap % (1024*1024)));
         }
#endif
         
         if ((wpl_buses_vadrs[wp][i] != NULL) && (i != WP_BUS_WMM5))
         {
            if (munmap(wpl_buses_vadrs[wp][i], size_to_unmap) == -1)
            {
               perror("PLATFORM munmap");
               return WPL_ERR(WPL_ERR_MMAP_FAILED);
            }
            
            wpl_buses_vadrs[wp][i] = NULL;
         }
      }
   }

   wpl_buses_need_to_unmap = WP_FALSE;
   for (i=0; i<24; i++)
      close(fd[i]);

   return WPL_OK;
}

WP_U32 WPL_BusesUnmap()
{
   /* Let the next time WPL_BusesMapGet is called unmap the buses */
   wpl_buses_need_to_unmap = WP_TRUE;

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
                      void *(*thread_function)(void *),
                      void *thread_function_arg)
{
   pthread_t thread;
   int iret;

   /* This creates a thread and stores it's id in thread */
   iret = pthread_create(&thread, NULL, thread_function, thread_function_arg);

   *thread_id = thread;

   if (iret == 0)
      return 0;
   else
      return 1;
}

WP_U32 WPL_ThreadDestroy(WP_THREAD_ID *thread_id)
{
   int status;
   status = pthread_cancel(*thread_id);

   if (status == 0)
      return 0;
   else
      return 1;
}

WP_U32 WPL_ThreadSelf(WP_THREAD_ID *thread_id)
{
   pthread_t thread;
   thread = pthread_self();
   *thread_id = (WP_THREAD_ID) thread;
   return 0;
}

WP_U32 WPL_Wait(WP_U32 wait_length)
{
   if (wait_length == 0)
      wait_length = 1;
   usleep(wait_length);

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

   sigaction(WPL_SIGNAL_RT_SGMII_AN, NULL, &siginfo);
   siginfo.sa_handler = SIG_DFL;
   siginfo.sa_flags = 0;
   sigaction(WPL_SIGNAL_RT_SGMII_AN, &siginfo, NULL);

   sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, NULL, &siginfo);
   siginfo.sa_handler = SIG_DFL;
   siginfo.sa_flags = 0;
   sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, &siginfo, NULL);

   sigaction(WPL_SIGNAL_RT_SERDES, NULL, &siginfo);
   siginfo.sa_handler = SIG_DFL;
   siginfo.sa_flags = 0;
   sigaction(WPL_SIGNAL_RT_SERDES, &siginfo, NULL);

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
   wpi_veneer_memory bus_mapping[WINPATH_NUM_MAX][WPI_BUS_LAST];
   WP_U32 status;

   status = WPLI_IntConnect();
   if (status != WPL_OK) {
      return status;
   }

   if ((fd[WP_BUS_INTERNAL] = open("/dev/wp_bank3", WPL_OPEN_FLAG)) < 0)
   {
      perror("open");
      return WPL_ERR(WPL_ERR_INIT);
   }

   if (ioctl(fd[WP_BUS_INTERNAL], WPL_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL, &value) < 0)
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);

   /* Call the platform function to get bus information from the kernel */
   if (WPL_BusesMapGet(&(bus_mapping[0][0])) != WPL_OK)
   {
      return WPL_ERR(WPL_ERR_INIT);
   }

   /* The start and size should not be altered after the first time. */
   wpl_first_time = 0;

   return WPL_OK;
}

WP_U8* WPL_RegBaseGet(WP_U8 i_wpid)
{
   if (i_wpid < wpl_num_wp)
   {
      WP_U32 result;
      WPLI_BusesRifBaseGet(i_wpid, &result);
      return (WP_U8*) result;
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

   return WPL_ERR(WPL_ERR_UNIMPLEMENTED);
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
 * Function: WPL_abort
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
   if (i_wpid < wpl_num_wp)
   {
      return (WP_U8*) wpl_bus_mappings[i_wpid][WPI_BUS_PHYS].uncached_offset;
   }

   return (WP_U8*) -1;
}

WP_U32 WPLI_BusesRifBaseGet(WP_U8 i_wpid, WP_U32* o_rif_base)
{
   if (wpl_rif_base[i_wpid] == (WP_U32) NULL)
   {
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
            "%u: wpid %u bus %u bank %u characteristics 0x%x start 0x%x (actual 0x%x) size 0x%x cached 0x%x uncached 0x%x \n",
            ii, i_bus_mapping[ii].wpid, i_bus_mapping[ii].bus, i_bus_mapping[ii].bank,
            i_bus_mapping[ii].characteristics, i_bus_mapping[ii].start,
            i_bus_mapping[ii].actual_start, i_bus_mapping[ii].size,
            (WP_U32) i_bus_mapping[ii].cached_offset,
            (WP_U32) i_bus_mapping[ii].uncached_offset);
   }
   printf("\n");
   
   return 0;
}

WP_U32 WPLI_IsSedUsed(void)
{
   WP_U32 value = system("/sbin/ifconfig | grep WP-SED> /dev/null");
   return ((value == 0) ? 1 : 0);
}

#if 1 // XALYO
#include "wpl_mem_access.h"
void *__xsmemcpy(void *dest, void *src, WP_S32 n) {
    int i;
    unsigned char val, *__src, *__dst;

    __src = src;
    __dst = dest;

    for(i=0; i<n; i++) {
        WP_MEM_BYTE_GET(val, __src[i]);
        WP_MEM_BYTE_SET(__dst[i], val);
    }

    return dest;
}

void *__xsmemset(void *s, WP_S32 c, WP_S32 n) {
    int i;
    unsigned char *__s;

    __s = s;

    for(i=0; i<n; i++) {
        WP_MEM_BYTE_SET(__s[i], c);
    }

    return s;
}

extern WP_U32 ntohl(WP_U32 x);
extern WP_U16 ntohs(WP_U16 x);
extern WP_U32 htonl(WP_U32 x);
extern WP_U16 htons(WP_U16 x);

#include <sys/mman.h>
#define __xsWP_MEM_PRINTno

void __xsWP_MEM_SET(volatile WP_U32 *dest, WP_U32 source) {
    WP_U32 tmp;
    tmp = htonl(source);
    *dest = tmp;
    msync((void *)dest, 4, MS_SYNC);
#ifdef __xsWP_MEM_PRINT
    printf("Writing %08x to %p\n", source, dest);
#endif
    return;
}

void __xsWP_MEM_SHORT_SET(volatile WP_U16 *dest, WP_U16 source) {
    WP_U16 tmp;
    tmp = htons(source);
    *dest = tmp;
    msync((void *)dest, 2, MS_SYNC);
#ifdef __xsWP_MEM_PRINT
    printf("Writing %04x to %p\n", source, dest);
#endif
    return;
}

void __xsWP_MEM_BYTE_SET(volatile WP_U8 *dest, WP_U8 source) {
    *dest = source;
    msync((void *)dest, 1, MS_SYNC);
#ifdef __xsWP_MEM_PRINT
    printf("Writing %02x to %p\n", source, dest);
#endif
    return;
}
void __xsWP_MEM_GET(WP_U32 *dest, volatile WP_U32 *source) {
    WP_U32 tmp;
#ifdef __xsWP_MEM_PRINT
   printf("Reading from %p\n", source);
#endif
    tmp = *source;
    *dest = htonl(tmp);
#ifdef __xsWP_MEM_PRINT
    printf("Reading %08x from %p\n", *dest, source);
#endif
    return;
}

void __xsWP_MEM_SHORT_GET(WP_U16 *dest, volatile WP_U16 *source){
    WP_U16 tmp;
    tmp = *source;
    *dest = htons(tmp);
#ifdef __xsWP_MEM_PRINT
    printf("Reading %04x from %p\n", *dest, source);
#endif
    return;
}

void __xsWP_MEM_BYTE_GET(WP_U8 *dest, volatile WP_U8 *source){
    *dest = *source;
#ifdef __xsWP_MEM_PRINT
    printf("Reading %02x from %p\n", *dest, source);
#endif
    return;
}

#endif // XALYO

