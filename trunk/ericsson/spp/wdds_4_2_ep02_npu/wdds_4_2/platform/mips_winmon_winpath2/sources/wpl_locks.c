/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_winmon_winpath2/sources/wpl_locks.c
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
#include <winmon.h>
#include <stdio.h>


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_LOCKS_NUM           8

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/


/******************************************************************/
/*                       Global Database                          */
/******************************************************************/


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


/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_LockKeyInit(WP_U32 i_key, WP_U32 *i_lock)
{
   /* In winmon/wp2 we have no threads, so no need for locks */
   return WPL_OK;
}

WP_U32 WPL_Lock(WP_U32 i_key, WP_U32 *i_lock)
{
   /* In winmon/wp2 we have no threads, so no need for locks */
   if (*i_lock)
   {
      printf("%s:%d, error - already locked.\n", __FILE__, __LINE__);
      WPL_Abort();    
   }   
   return WPL_OK;
}

WP_U32 WPL_Unlock(WP_U32 i_key, WP_U32 *i_lock)
{
   /* In winmon/wp2 we have no threads, so no need for locks */
   return WPL_OK;
}

WP_U32 WPL_LockDestroy(WP_U32 i_key, WP_U32 *i_lock)
{
   /* No need to do anything here */
   return WPL_OK;
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

/******************************************************************/
/*                       Internal Functions                       */
/******************************************************************/

