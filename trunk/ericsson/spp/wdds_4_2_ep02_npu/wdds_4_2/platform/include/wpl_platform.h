/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef _WPL_PLATFORM_H_
#define _WPL_PLATFORM_H_

/******************************************************************/
/*                       Includes                                 */
/******************************************************************/

#include "wpl_types.h"

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#ifndef WPL_DEBUG_ENABLE
#define WPL_DEBUG_ENABLE 0
#endif

#if WPL_DEBUG_ENABLE
#define WPLI_LOG(toprint)     \
do {                          \
   printf toprint ;           \
} while (0)
#else
#define WPLI_LOG(toprint)
#endif

#define WPL_ERR(err_id) ((err_id == WPL_OK) ? WPL_OK : (0xFC000000 + (err_id)))

#define WPL_IS_ERROR(status) (((status) & 0xFFFF0000) == 0xFC000000)

enum WPL_error_types
{
   WPL_OK = 0,
   /* Define first error as a real WDDI error so wddi would catch these */
   WPL_ERR_GENERIC = 0x20,
   WPL_ERR_UNSUPPORTED_COMMAND,
   WPL_ERR_RIF_BUS_UNINITIALIZED,
   WPL_ERR_UNIMPLEMENTED,
   WPL_ERR_INIT,
   WPL_ERR_GNU_LINUX_APPLICATION_FAILURE,
   WPL_ERR_WINPATH_UNINITIALIZED,
   WPL_ERR_MEMORY_ALLOCATION,
   WPL_ERR_PLATFORM,
   WPL_ERR_PREVIOUS_WMM_HOST_CMD_FAILED,
   WPL_ERR_WMM_HOST_CMD_FAILED,
   WPL_ERR_INVALID_PARAM,
   WPL_ERR_MMAP_FAILED,
   WPL_ERR_ADDRESS_ALIGNMENT,
   WPL_ERR_INT_NOT_SUPPORTED,
   WPL_ERR_SEM_INIT_FAILED,
   WPL_ERR_SEM_INCREMENT_FAILED,
   WPL_ERR_SEM_DECREMENT_FAILED,
   WPL_ERR_SEM_DESTROY_FAILED,
   WPL_ERR_ILLEGAL_BSP_VERSION,
   WPL_ERR_NUM
};

#ifndef WPL_IOCTL_WMM_HOST_CMD
#define WPL_IOCTL_WMM_HOST_CMD 0
#endif

#ifndef WPL_IOCTL_SUPPRESS_DUP_SIGNAL
#define WPL_IOCTL_SUPPRESS_DUP_SIGNAL 1
#endif

#ifndef WPL_IOCTL_READ_PHYS_MEM_LAYOUT
#define WPL_IOCTL_READ_PHYS_MEM_LAYOUT 2
#endif

#ifndef WINPATH_NUM_BUSES
#define WINPATH_NUM_BUSES 4
#endif

#ifndef WINPATH_NUM_MAX
#define WINPATH_NUM_MAX  4
#endif

#define WPL_VB_BSP_EVENT_QUEUE          0x80000000
#define WPL_VB_BSP_FIFO_QUEUE           0x40000000

/*****************************************************************************
 *
 * Macros to compress into and extract from a single WP_U8 the
 * is_shared_memory indicator and the physical memory range.
 * This keeps the definition of wpi_veneer_memory 32-bit aligned.
 *
 ****************************************************************************/

#define WPL_FIELD(v,s) (((WP_U32) (v)) << (s))
#define WPL_VALUE(f,s,w) ((((WP_U32) (f)) >> (s)) & ((1 << (w)) - 1))
#define WPL_MASK(s,w) ((((WP_U32) ((1 << (w)) - 1)) << (s)))

#define WPL_SHARED_MEMORY_S   7
#define WPL_SHARED_MEMORY_W   1
#define WPL_SHARED_MEMORY_F(v)   WPL_FIELD(v, WPL_SHARED_MEMORY_S)
#define WPL_SHARED_MEMORY_M      WPL_MASK(WPL_SHARED_MEMORY_S, WPL_SHARED_MEMORY_W)
#define WPL_SHARED_MEMORY_V(f)   WPL_VALUE(f, WPL_SHARED_MEMORY_S, WPL_SHARED_MEMORY_W)

#define WPL_IS_SHARED_MEMORY(v) WPL_SHARED_MEMORY_V(v)

/***************************************************************************
 * The remaining code defines structures and interfaces used by the veneer.
 * The structures are WPI_xxx types, indicating that they are defined by
 * Wintegra and may not be changed without losing compatibility with Wintegra
 * code.  The functions have WPV_xxx names, indicating that they are
 * platform-dependent code which is provided as sample code by Wintegra.
 ****************************************************************************/

#define WPI_HEAP_METHOD 1
#define WPI_NO_FREE_METHOD 2
#define WPI_MEGAHERZ 1000000

#define WPL_PHYS_RANGE_S      0
#define WPL_PHYS_RANGE_W      6
#define WPL_PHYS_RANGE_F(v) WPL_FIELD(v, WPL_PHYS_RANGE_S)
#define WPL_PHYS_RANGE_V(f) \
   WPL_VALUE(f, WPL_PHYS_RANGE_S, WPL_PHYS_RANGE_W)

#define WPL_RANGE_INDEX(v) WPL_PHYS_RANGE_V(v)

#ifndef NULL
#define NULL 0
#endif

#include "wpli_platform.h"
#include "wpl_alloc.h"
#include "wpl_os_enet.h"
#include "wpl_interrupts.h"
#include "wpl_wmm.h"
#include "wpl_board.h"

#include "wpi_veneer_memory.h"

typedef void * (*WPL_start_function)(void *);

/***************************************************************************
 * Define the memory management functions used for WDDI structures that
 * are not used by WinComm.  These can be in cacheable memory.
 ****************************************************************************/

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

/******************************************************************/
/*                       External database                        */
/******************************************************************/

/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/** Key structure is: */
/** HW    -  SHARED  -  SH. NUM  -  NUM    */
/** 1 bit    1 bit      10 bit      20 bit */

#define WPL_LOCK_KEY_CREATE(hw, shared, shnum, num)  (((hw) << 31) | ((shared) << 30) | ((shnum) << 20) | (num))

/******************************************************************/
/*                       Functions                                */
/******************************************************************/

WP_U32 WPL_ThreadInit(WP_THREAD_ID *thread_id,
                      void *(*thread_function)(void *),
                      void *thread_function_arg);

WP_U32 WPL_ThreadDestroy(WP_THREAD_ID *thread_id);

WP_U32 WPL_ThreadSelf(WP_THREAD_ID *thread_id);

WP_U32 WPL_Wait(WP_U32 wait_length);


WP_U32 WPL_BusesMapGet(wpi_veneer_memory o_bus_mapping[]);

WP_U32 WPL_BusesUnmap(void);


WP_U32 WPL_BusesMapSet(wpi_veneer_memory i_bus_mapping[]);

WP_U32 WPL_BusesVadrsSet(WP_U8* i_buses_vadrs[]);

WP_U32 WPL_BusesNeedRemap(void);

WP_U32 WPL_LockKeyInit(WP_U32 i_key, WP_U32 *i_lock);

WP_U32 WPL_Lock(WP_U32 i_key, WP_U32 *i_lock);

WP_U32 WPL_Unlock(WP_U32 i_key, WP_U32 *i_lock);

WP_U32 WPL_LockDestroy(WP_U32 i_key, WP_U32 *i_lock);

enum wpi_vbank_result WPL_GetVbank(WP_U8 index, wpi_veneer_memory *line);

WP_U32 WPL_BspServiceMask(void);

WP_U32 WPL_BspOverrunMask(void);

enum wpi_vbank_result WPL_GetBspBusAssignment(WP_U32 whichBusType, WP_U8 *bus,
                                              WP_U8 *bank);

WP_U32 WPL_OsService(WPL_OsServiceType action, void *holder);

WP_U32 WPL_DriverInit(void);

WP_U8* WPL_RegBaseGet(WP_U8 i_wpid);

WP_U8* WPL_RegOffsetGet(WP_U8 i_wpid);

WP_U32 WPL_EventInit(void);

void WPL_DriverRelease(void);

void WPL_CacheFlush(WP_U8 *address, WP_U32 nBytes);

void WPL_BrgAvailability(WP_U32 wpid, WP_U32 *brg9_is_available,
                         WP_U32 *brg10_is_available);

void WPL_ReportCatastrophicErrors(WP_U32 syir);

void WPL_Abort(void);

WP_U32 WPL_WmmHostCmd(WP_U32 wpid, WP_U8 bustype, WP_U32 *hcr_cmd,
                      WP_U32 *hcr_reg, WP_U32 *host_address);

void WPL_FifoInitialize(void *wmm);

WP_U32 WPL_GetWinPaths(void);

WP_U32 WPL_Delay(WP_U32 interval);

WP_U32 WPL_GetProcessorSpeed(void);

WP_U32 WPL_TimeInit(void);

WP_U32 WPL_TimestampGet(WP_time* o_time);
#endif
