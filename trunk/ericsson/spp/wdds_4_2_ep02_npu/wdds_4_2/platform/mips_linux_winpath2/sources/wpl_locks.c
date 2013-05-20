/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux_winpath2/sources/wpl_locks.c
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
#include <pthread.h> 
#include <fcntl.h>
#include <errno.h>
#include <sys/sem.h>
#include <wpl_sw_semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_FLOCK_FILENAME_SIZE 40
#define WPL_FLOCK_FILE          "/var/tmp/flock"
#define WPL_LOCKS_NUM           8


/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

typedef union semun {
        long val;
        struct semid_ds *buf;
        unsigned short int *array;
        struct seminfo *__buf;
} SEMUN;

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/


/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

static WP_S32 wpl_flocks[WPL_LOCKS_NUM] = {0};
static WP_U32 wpl_threads_locks[WPL_LOCKS_NUM] = {0};

struct flock  wpl_flock_struct;


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
   /* HW locks not support in WP2, so we don't check that bit here */

   if (WPLI_LOCK_KEY_IS_SHARED(i_key))
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
   if (WPLI_LOCK_KEY_IS_SHARED(i_key))
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
   if (WPLI_LOCK_KEY_IS_SHARED(i_key))
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
   WP_CHAR lock_name[WPL_FLOCK_FILENAME_SIZE];

   if (WPL_LOCKS_NUM <= i_shared_num)
   {
      perror("Lock value invalid");
      return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }

   /* Initialize threads lock */
   WPL_LockKeyInit(i_private_num, &wpl_threads_locks[i_shared_num]);
      
   /* Initialize processes lock */
   wpl_flock_struct.l_type   = F_WRLCK;
   wpl_flock_struct.l_whence = SEEK_SET;
   wpl_flock_struct.l_start  = 0;
   wpl_flock_struct.l_len    = 0;
   wpl_flock_struct.l_pid    = getpid();

   sprintf(lock_name, "%s.%u", WPL_FLOCK_FILE, i_shared_num);
   wpl_flocks[i_shared_num] = open(lock_name, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR);
   if (wpl_flocks[i_shared_num] == -1)
   {
      perror("Lock file could not be opened");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }
   
   return WPL_OK;
}

WP_U32 WPLI_SharedLock(WP_U32 i_shared_num)
{
   wpl_flock_struct.l_type   = F_WRLCK;

   while (fcntl(wpl_flocks[i_shared_num], F_SETLKW, &wpl_flock_struct) == -1)
   {
      switch (errno) {
         case EBADF:
            perror("Lock file error: bad file name, or file not open");
            return WPL_ERR(WPL_ERR_PLATFORM);
         case EINTR:
            /* took some signal while waiting */
            perror("Lock file warning: got a signal while waiting for lock");
            break;
         case EINVAL:
            perror("Lock file error: operation is invalid.");
            return WPL_ERR(WPL_ERR_PLATFORM);
         case ENOLCK:
            perror("Lock file error: The kernel ran out of memory for allocating lock records.");
            return WPL_ERR(WPL_ERR_PLATFORM);
         case EWOULDBLOCK:
            perror("Lock file error: LOCK_NB was selected on open");
            break;
         default:
            printf("Lock file error: got unexpected errno %u\n", errno);
      }
   }
   
   WPLI_PrivateLock(&wpl_threads_locks[i_shared_num]);
   
   return WPL_OK;
}

WP_U32 WPLI_SharedUnlock(WP_U32 i_shared_num)
{
   WPLI_PrivateUnlock(&wpl_threads_locks[i_shared_num]);

   wpl_flock_struct.l_type   = F_UNLCK;
   
   while (fcntl(wpl_flocks[i_shared_num], F_SETLK, &wpl_flock_struct) == -1)
   {
      switch (errno) {
         case EBADF:
            perror("Lock file error: bad file name, or file not open");
            return WPL_ERR(WPL_ERR_PLATFORM);
         case EINTR:
            /* took some signal while waiting */
            break;
         case EINVAL:
            perror("Lock file error: operation is invalid.");
            return WPL_ERR(WPL_ERR_PLATFORM);
         case ENOLCK:
            perror("Lock file error: The kernel ran out of memory for allocating lock records.");
            return WPL_ERR(WPL_ERR_PLATFORM);
         case EWOULDBLOCK:
            perror("Lock file error: LOCK_NB was selected on open");
            break;
         default:
            printf("Lock file error: got unexpected errno %u\n", errno);
      }
   }
   
   return WPL_OK;
}

WP_U32 WPLI_SharedLockDestroy(WP_U32 i_shared_num, WP_U32 i_private_num)
{
   wpl_flocks[i_shared_num] = close(wpl_flocks[i_shared_num]);
   if (wpl_flocks[i_shared_num] == -1)
   {
      perror("Lock file could not be closed");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }

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
   int rc =1;
   
   while(rc)
   {
      rc = wpv_swsem_acquire((int*)i_lock, NO_WAIT);
      if(rc != WPL_OK)
      {
         WPL_Wait(10);
      }
   }
}

void WPLI_PrivateUnlock(WP_U32 *i_lock)
{
   int rc =1;

   rc =  wpv_swsem_release((int*)i_lock);
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
      switch (errno) {
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
static inline int wpv_test_and_set_bit(unsigned long nr,
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
