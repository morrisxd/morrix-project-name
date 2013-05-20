/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_winmon_winpath3/sources/wpl_locks.c
 *
 * Purpose: Winpath's platform dependent locking functions
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

#include <wpl_types.h>
#include <wpl_platform.h>
#include <wpli_platform.h>
#include <wpl_mem_access.h>
#include <wpl_sw_semaphore.h>
#include <winmon.h>
#include <stdio.h>


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_LOCKS_NUM           8

#define	WPL_SEM_BASE           (wpl_rif_base[0] + 0x1fe0)

#define WPL_SEM_REGISTER_GET(address, sem)             \
do{                                                    \
   address = ((WP_U32*)(WPL_SEM_BASE + ((sem) * 4)));  \
}while(0)


/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/


/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

extern WP_U32 wpl_rif_base[WINPATH_NUM_MAX];
extern WP_U32 wds_bsp_ver;

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/


/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/* Key macros */

/** Key structure is: */
/** HW    -  SHARED  -  SH. NUM  -  NUM    */
/** 1 bit    1 bit      10 bit      20 bit */

#define WPLI_LOCK_KEY_IS_HW(key)            ((key) >> 31)

#define WPLI_LOCK_KEY_IS_SHARED(key)        (((key) >> 30) & 1)

#define WPLI_LOCK_KEY_SHARED_NUM(key)       (((key) >> 20) & 0x3FF)

#define WPLI_LOCK_KEY_NUM(key)              ((key) & 0xFFFFF)


/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

/* Lock shared between processes/cores */
WP_U32 WPLI_SharedLockInit(WP_U32 i_shared_num, WP_U32 i_private_num);
WP_U32 WPLI_SharedLock(WP_U32 i_shared_num);
WP_U32 WPLI_SharedUnlock(WP_U32 i_shared_num);
WP_U32 WPLI_SharedLockDestroy(WP_U32 i_shared_num, WP_U32 i_private_num);

/* Lock between threads only */
WP_U32 WPLI_PrivateLockInit(WP_U32 i_key, WP_U32 *i_lock);
void WPLI_PrivateLock(WP_U32 *i_lock);
void WPLI_PrivateUnlock(WP_U32 *i_lock);


/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_LockKeyInit(WP_U32 i_key, WP_U32 *i_lock)
{
   /* Verify wpl_rif_base in initialized */
   if (wpl_rif_base[0] == NULL)
   {
      WPLI_BusesRifBaseGet(0, &wpl_rif_base[0]);
   }
#if WPL_BSP_VERSION_CHECK
   if(wds_bsp_ver < WPL_BSP_WINMON_VERSION)
      return WPL_ERR(WPL_ERR_ILLEGAL_BSP_VERSION);
#endif

   /* In winmon there's no shared lock that is not hardware */
   if (WPLI_LOCK_KEY_IS_SHARED(i_key) || WPLI_LOCK_KEY_IS_HW(i_key))
   {
      return WPLI_SharedLockInit(WPLI_LOCK_KEY_SHARED_NUM(i_key), WPLI_LOCK_KEY_NUM(i_key));
   }
   else
   {
      return WPLI_PrivateLockInit(WPLI_LOCK_KEY_NUM(i_key), i_lock);
   }
}

WP_U32 WPL_Lock(WP_U32 i_key, WP_U32 *i_lock)
{
   /* Verify wpl_rif_base in initialized */
   if (wpl_rif_base[0] == (WP_U32)NULL)
   {
      printf("PLATFORM error - wpl_rif_base was not initialized");
      return WPL_ERR(WPL_ERR_INIT);
   }

   /* In winmon there's no shared lock that is not hardware */
   if (WPLI_LOCK_KEY_IS_SHARED(i_key) || WPLI_LOCK_KEY_IS_HW(i_key))
   {
      return WPLI_SharedLock(WPLI_LOCK_KEY_SHARED_NUM(i_key));
   }
   else
   {
      WPLI_PrivateLock(i_lock);
      return WPL_OK;
   }
}

WP_U32 WPL_Unlock(WP_U32 i_key, WP_U32 *i_lock)
{
   /* In winmon there's no shared lock that is not hardware */
   if (WPLI_LOCK_KEY_IS_SHARED(i_key) || WPLI_LOCK_KEY_IS_HW(i_key))
   {
      return WPLI_SharedUnlock(WPLI_LOCK_KEY_SHARED_NUM(i_key));
   }
   else
   {
      WPLI_PrivateUnlock(i_lock);
      return WPL_OK;
   }
}

WP_U32 WPL_LockDestroy(WP_U32 i_key, WP_U32 *i_lock)
{
   if (WPLI_LOCK_KEY_IS_SHARED(i_key))
   {
      return WPLI_SharedLockDestroy(WPLI_LOCK_KEY_SHARED_NUM(i_key), WPLI_LOCK_KEY_NUM(i_key));
   }

   return WPL_OK;
}


/******************************************************************/
/*                       Internal Functions                       */
/******************************************************************/

WP_U32 WPLI_SharedLockInit(WP_U32 i_shared_num, WP_U32 i_private_num)
{
   /* Locking register boots with 0 - so no need to do anything here */
   
   return WPL_OK;
}

WP_U32 WPLI_SharedLock(WP_U32 i_shared_num)
{
   WP_U32 lock_val = 1;
   WP_U32* address = NULL;

   WPL_SEM_REGISTER_GET(address, i_shared_num);
   
   if (i_shared_num >= WPL_LOCKS_NUM)
   {
      printf("PLATFORM error - WPLI_SharedLock - sh. num %u must be <= %u\n", i_shared_num, WPL_LOCKS_NUM);

      return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }

   while (lock_val == 1)
   {
      WP_MEM_GET(lock_val, *address);

// XALYO
{
WP_U32 ttt;
ttt = *(volatile unsigned int *)0xbf101020;
}

   }

   return WPL_OK;
}

WP_U32 WPLI_SharedUnlock(WP_U32 i_shared_num)
{
   WP_U32* address = NULL;
   
   WPL_SEM_REGISTER_GET(address, i_shared_num);

   WP_MEM_SET(*address, 1);

   return WPL_OK;
}

WP_U32 WPLI_SharedLockDestroy(WP_U32 i_shared_num, WP_U32 i_private_num)
{
   /* Simply unlock the HW lock - to set it to 0 */
   WPLI_SharedUnlock(i_shared_num);

   return WPL_OK;
}

WP_U32 WPLI_PrivateLockInit(WP_U32 i_key, WP_U32 *i_lock)
{
   *i_lock = 0;
   return WPL_OK;
}

void WPLI_PrivateLock(WP_U32 *i_lock)
{
   int rc =1;
   
   while(rc)
   {
      rc = wpv_swsem_acquire((int*)i_lock, NO_WAIT);
      if(rc != WPL_OK)
      {
         /*sched_yield(); - needs to be implemented in winmon */
      }
   }
}

void WPLI_PrivateUnlock(WP_U32 *i_lock)
{
   int rc =1;

   rc =  wpv_swsem_release((int*)i_lock);
}


/* Private lock helper functions */

/*
 * test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
inline int wpv_test_and_set_bit(unsigned long nr,
	volatile unsigned long *addr)
{
   unsigned short bit = nr & SZLONG_MASK;
   unsigned long res;
   unsigned long *m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);
   unsigned long temp;

   __asm__ __volatile__(
      "	.set	push					\n"
      "	.set	noreorder				\n"
      "	.set	mips3					\n"
      "1:	ll  %0, %1		        	\n"
      "	or	%2, %0, %3				\n"
      "	sc      %2, %1					\n"
      "	beqz	%2, 2f					\n"
      "	 and	%2, %0, %3				\n"
      "	.subsection 2					\n"
      "2:	b	1b				\n"
      "	 nop						\n"
      "	.previous					\n"
      "	.set	pop					\n"
      : "=&r" (temp), "=m" (*m), "=&r" (res)
      : "r" (1UL << bit), "m" (*m)
      : "memory");

   /*smp_llsc_mb();*/

   return res != 0;
}

inline void wpv_clear(volatile unsigned long *addr)
{
   *addr = 0;   
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*  NOTE: FOR COHERENCY BETWEEN THE CORES SEMAPHORES MUST BE UNCACHED               */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
int wpv_swsem_acquire(int *sem, int wait)
{
   int rc;

   if((wait != WAIT_FOREVER)&&(wait != NO_WAIT))
      return SEM_WAIT_PARAM_ERROR;

   while(1)
   {
      rc = wpv_test_and_set_bit(0, (unsigned long*)sem);
      if(rc == WPL_OK)/*succeeded to acquire the semaphore*/
      { 
         return rc;
      }
      else
      {
         if(wait == NO_WAIT)
         {
            return rc;
         }
      }
   }
   return -1; /*code should not get here*/
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*  NOTE: FOR COHERENCY BETWEEN THE CORES SEMAPHORES MUST BE UNCACHED               */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
int wpv_swsem_release(int *sem)
{
   int rc = WPL_OK;

   wpv_clear((unsigned long*)sem);
   return rc; 
}

WP_U32 WPL_SemInit(WP_SEM_ID *sem_id)
{
     return WPL_ERR(WPL_ERR_UNSUPPORTED_COMMAND);
}


WP_U32 WPL_SemIncrement(WP_SEM_ID *sem_id,WP_U32 incrementCount)
{
     return WPL_ERR(WPL_ERR_UNSUPPORTED_COMMAND);
}


WP_U32 WPL_SemDecrement(WP_SEM_ID *sem_id, WP_U32 decrementCount)
{
     return WPL_ERR(WPL_ERR_UNSUPPORTED_COMMAND);
}


WP_U32 WPL_SemDestroy(WP_SEM_ID *sem_id)
{
     return WPL_ERR(WPL_ERR_UNSUPPORTED_COMMAND);
}
