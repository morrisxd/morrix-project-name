/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux_winpath3/sources/wpl_locks.c
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

#include <sys/file.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include "wpl_types.h"
#include <wpl_platform.h>
#include <wpli_platform.h>
#include <wpl_mem_access.h>
#include <pthread.h> 
#include <fcntl.h>
#include <errno.h>
#include <sys/sem.h>
#include <wpl_sw_semaphore.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>

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

typedef union semun
{
   long val;
   struct semid_ds *buf;
   unsigned short int *array;
   struct seminfo *__buf;
} SEMUN;

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

extern WP_U32 wpl_rif_base[WINPATH_NUM_MAX];

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

#define WPLI_LOCK_KEY_IS_HW(key)                      ((key) >> 31)

#define WPLI_LOCK_KEY_IS_SHARED(key)                  (((key) >> 30) & 1)

#define WPLI_LOCK_KEY_SHARED_NUM(key)                 (((key) >> 20) & 0x3FF)

#define WPLI_LOCK_KEY_NUM(key)                        ((key) & 0xFFFFF)

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
void WPLI_PrivateLockDestroy(WP_U32 *i_lock);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_LockKeyInit(WP_U32 i_key, WP_U32 *i_lock)
{
   /* Verify wpl_rif_base is initialized */
   if (wpl_rif_base[0] == (WP_U32) NULL)
   {
      WPLI_BusesRifBaseGet(0, &wpl_rif_base[0]);
   }

   /* Shared locks in linux should be HW only (for efficiency purposes) */
   if (WPLI_LOCK_KEY_IS_HW(i_key) || WPLI_LOCK_KEY_IS_SHARED(i_key))
   {
      return WPLI_SharedLockInit(WPLI_LOCK_KEY_SHARED_NUM(i_key),
            WPLI_LOCK_KEY_NUM(i_key));
   }
   else
   {
      return WPLI_PrivateLockInit(WPLI_LOCK_KEY_NUM(i_key), i_lock);
   }
}

WP_U32 WPL_Lock(WP_U32 i_key, WP_U32 *i_lock)
{
   /* Verify wpl_rif_base in initialized */
   if (wpl_rif_base[0] == (WP_U32) NULL)
   {
      printf("PLATFORM error - wpl_rif_base was not initialized");
      return WPL_ERR(WPL_ERR_INIT);
   }

   /* Shared locks in linux should be HW only (for efficiency purposes) */
   if (WPLI_LOCK_KEY_IS_HW(i_key) || WPLI_LOCK_KEY_IS_SHARED(i_key))
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
   /* Shared locks in linux should be HW only (for efficiency purposes) */
   if (WPLI_LOCK_KEY_IS_HW(i_key) || WPLI_LOCK_KEY_IS_SHARED(i_key))
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
   /* HW locks not support in WP2, so we don't check that bit here */

   if (WPLI_LOCK_KEY_IS_SHARED(i_key))
   {
      return WPLI_SharedLockDestroy(WPLI_LOCK_KEY_SHARED_NUM(i_key), WPLI_LOCK_KEY_NUM(i_key));
   }
   else
   {
      WPLI_PrivateLockDestroy(i_lock);
   }

   return WPL_OK;
}

/******************************************************************/
/*                       Internal Functions                       */
/******************************************************************/

WP_U32 WPLI_SharedLockInit(WP_U32 i_shared_num, WP_U32 i_private_num)
{
   /* Locking register starts with 0 - so no need to do anything here */

   return WPL_OK;
}

WP_U32 WPLI_SharedLock(WP_U32 i_shared_num)
{
   WP_U32 lock_val = 1;
   WP_U32* address = NULL;

   WPL_SEM_REGISTER_GET(address, i_shared_num);

   if (i_shared_num >= WPL_LOCKS_NUM)
   {
      printf("PLATFORM error - WPLI_SharedLock - sh. num %u must be <= %u\n",
            i_shared_num, WPL_LOCKS_NUM);

      return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }

   WP_MEM_GET(lock_val, *address);

   while (lock_val == 1)
   {
      WPL_Wait(10);      

      WP_MEM_GET(lock_val, *address);
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
#if (USE_LINUX_SEMAPHORE)
   SEMUN arg;
   arg.val = 0;
   int semid;
   /* Give this process read and write permissions on the semaphore */
   semid = semget(i_key, 1, IPC_CREAT | 0700);
   semctl(semid, 0, SETVAL, arg);
   *i_lock = semid;
#else
   *i_lock = 0;
#endif

   return WPL_OK;
}

#if (!USE_LINUX_SEMAPHORE)
void WPLI_PrivateLock(WP_U32 *i_lock)
{
   int rc = 1;

   while (rc)
   {
      rc = wpv_swsem_acquire((int*) i_lock, NO_WAIT);
      if (rc != WPL_OK)
      {
         sched_yield();
      }
   }
}

void WPLI_PrivateUnlock(WP_U32 *i_lock)
{
   int rc = 1;

   rc = wpv_swsem_release((int*) i_lock);
}
#endif /* #if (!USE_LINUX_SEMAPHORE) */

#if (USE_LINUX_SEMAPHORE)
void WPLI_PrivateLock(WP_U32 *i_lock)
{
   struct sembuf sop[2];
   int semid;

   /* First operation waits for semval to be 0 */
   sop[0].sem_num = 0;
   sop[0].sem_op = 0;
   sop[0].sem_flg = 0;

   /* Second operation sets semval to 1 */
   sop[1].sem_num = 0;
   sop[1].sem_op = 1;
   sop[1].sem_flg = 0;

   semid = *i_lock;
   while (semop(semid, sop, 2) == -1)
   switch (errno)
   {
      case EINTR:
      /* took some signal while waiting */
      break;
      default:
      printf("PLATFORM: got unexpected errno %u\n", errno);
   }
}

void WPLI_PrivateUnlock(WP_U32 *i_lock)
{
   struct sembuf sop;
   int semid;

   sop.sem_num = 0;
   sop.sem_op = -1;
   sop.sem_flg = IPC_NOWAIT;

   semid = *i_lock;
   semop(semid, &sop, 1);
}
#endif /* #if (USE_LINUX_SEMAPHORE) */

void WPLI_PrivateLockDestroy(WP_U32 *i_lock)
{
#if (USE_LINUX_SEMAPHORE)
   int semid;
   semid = *i_lock;

   semctl(semid, 0, IPC_RMID, 0);
#endif

   *i_lock = 0;
}

#if (!USE_LINUX_SEMAPHORE)
/*
 * test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static inline int wpv_test_and_set_bit(unsigned long nr, volatile unsigned long *addr)
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

static inline void wpv_clear(volatile unsigned long *addr)
{
   *addr = 0;
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*  NOTE: FOR COHERENCY BETWEEN THE CORES SEMAPHORES MUST BE UNCACHED               */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
int wpv_swsem_acquire(int *sem, int wait)
{
   int rc;

   if ((wait != WAIT_FOREVER) && (wait != NO_WAIT))
      return SEM_WAIT_PARAM_ERROR;

   while (1)
   {
      rc = wpv_test_and_set_bit(0, (unsigned long*) sem);
      if (rc == WPL_OK)/*succeeded to acquire the semaphore*/
      {
         return rc;
      }
      else
      {
         if (wait == NO_WAIT)
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

   wpv_clear((unsigned long*) sem);
   return rc;
}
#endif /* #if (!USE_LINUX_SEMAPHORE) */

WP_U32 WPL_SemInit(WP_SEM_ID *sem_id)
{
   SEMUN arg;
   arg.val = 0;
   int id;

   /* This creates the semaphore and gives  read and write permissions on the same */
   id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0700);

   /* This initialises the semaphore to 0 */
   if (id != -1)
   {
      semctl(id, 0, SETVAL, arg);
      *sem_id = id;
      return WPL_OK;
   }
   else
      return WPL_ERR(WPL_ERR_SEM_INIT_FAILED);
}

WP_U32 WPL_SemIncrement(WP_SEM_ID *sem_id, WP_U32 incrementCount)
{
   struct sembuf sop;
   int id;

   id = *sem_id;

   /* This populates the structure 'sop' with values such that the
    semaphore is incremented by 'incrementCount' */
   sop.sem_num = 0;
   sop.sem_op = incrementCount;
   sop.sem_flg = 0;

   /* This loop increments the semaphore and waits till the increment is performed */
   while (semop(id, &sop, 1) == -1)
      switch (errno)
      {
      case EINTR:
         /* took some signal while waiting */
         break;
      default:
         return WPL_ERR(WPL_ERR_SEM_INCREMENT_FAILED);
         break;
      }

   return WPL_OK;
}

WP_U32 WPL_SemDecrement(WP_SEM_ID *sem_id, WP_U32 decrementCount)
{
   struct sembuf sop;
   int id;

   id = *sem_id;

   /* This populates the structure 'sop' with values such that
    the semaphore is decremented by 'decrementCount' */
   sop.sem_num = 0;
   sop.sem_op = -decrementCount;
   sop.sem_flg = 0;

   /* This loop decrements the semaphore and waits till the decrement is performed */
   while (semop(id, &sop, 1) == -1)
      switch (errno)
      {
      case EINTR:
         /* took some signal while waiting */
         break;

      default:
         return WPL_ERR(WPL_ERR_SEM_DECREMENT_FAILED);
         break;
      }

   return WPL_OK;
}

WP_U32 WPL_SemDestroy(WP_SEM_ID *sem_id)
{
   SEMUN arg;
   int id, ret;
   id = *sem_id;

   /* This call destroys the semaphore.The second parameter (semnum) and
    the last parameter (arg)have no meaning */
   ret = semctl(id, 0, IPC_RMID, arg);

   if (ret != -1)
      return WPL_OK;
   else
      return WPL_ERR(WPL_ERR_SEM_DESTROY_FAILED);
}
