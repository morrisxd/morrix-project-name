/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux_winpath3/sources/wpl_time.c
 *
 * Purpose: Winpath's platform dependent timing functions
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
#include "wpl_types.h"
#include <wpl_platform.h>
#include <wpli_platform.h>
#include <wpl_mem_access.h>


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_TIME_THRESHOLD  0x7FFFFFFF /* 01111111 11111111 11111111 11111111 */

#define WPL_TIME_PRESCALAR  (wpl_rif_base[0] + 0x1f80)
#define WPL_TIME_HIGH       (wpl_rif_base[0] + 0x1f84)
#define WPL_TIME_LOW        (wpl_rif_base[0] + 0x1f88)

#define MILLION 1000000

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

extern WP_U32 wpl_rif_base[WINPATH_NUM_MAX];

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

static WP_boolean wpli_is_init = WP_FALSE;
static WP_U32     wpli_time_factor;

/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

#define WPL_TIME_REGISTER_READ(val, reg) WP_MEM_GET((val), *((WP_U32*)WPL_TIME_##reg))

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

WP_U32 WPLI_Delay(WP_U32 interval);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_TimeInit(void)
{
   WP_U32 prescalar;

   /* Get rif base, if required */
   if (wpl_rif_base[0] == (WP_U32) NULL)
   {
      WPLI_BusesRifBaseGet(0, &wpl_rif_base[0]);
   }

   /* Calculate factor to divide by when getting timestamps */
   WPL_TIME_REGISTER_READ(prescalar, PRESCALAR);
   wpli_time_factor = WPL_GetProcessorSpeed() / ((prescalar + 1) * MILLION); /* MILLION to translate seconds --> ms */

   wpli_is_init = WP_TRUE;

   return WPL_OK;
}

WP_U32 WPL_TimestampGet(WP_time* o_time)
{
   WP_U32 high;
   WP_U32 low;
   WP_U64 total;

   /* Verify that the module is initialized */
   if(WP_FALSE == wpli_is_init)
   {
      WPL_TimeInit();
   }
   
   /* Check input */
   if(NULL == o_time)
   {
      printf("PLATFORM error - o_time is a NULL pointer\n");
      return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }

   /* Read registers */
   WPL_TIME_REGISTER_READ(high, HIGH);
   WPL_TIME_REGISTER_READ(low, LOW);
   if(low < WPL_TIME_THRESHOLD)
   {
      WPL_TIME_REGISTER_READ(high, HIGH);
   }
   
   /* Unite high and low bits */
   total = (((WP_U64)high) << 32) + low;
   
   /* Translate to <seconds, microseconds> format */
   total /= wpli_time_factor;
   o_time->sec  = total / MILLION;
   o_time->usec = total % MILLION;

   return WPL_OK;
}

WP_U32 WPL_Delay(WP_U32 interval)
{
   return WPLI_Delay(interval);
}


/*****************************************************************************
 *
 * Function: WPLI_Delay
 *
 * Purpose: Internal function to consume some time
 *
 * Description:  This function repeatedly calls WPL_TimestampGet until the
 *               requested number of microseconds have elapsed.
 *
 * Inputs:
 *    interval   time to consume, specified in microseconds
 *
 * Outputs:
 *    none
 *
 * Return value:
 *    none
 *
 * Called by:
 *    user application; also used in wpu_oc12_pmc5357.c, wpu_oc3_pmc5351.c
 *
 ****************************************************************************/

WP_U32 WPLI_Delay(WP_U32 interval)
{
   WP_U32 microseconds;
   WP_time curr_time = {0}, timeout = {0};

   /* Get current TimeStamp */
   WPL_TimestampGet(&curr_time);

   /* Calculate timeout expiration */
   microseconds = curr_time.usec + interval;
   timeout.sec = curr_time.sec + (microseconds / MILLION);
   timeout.usec = microseconds % MILLION;

   while(WP_TRUE)
   {
      if((curr_time.sec > timeout.sec) ||
         ((curr_time.sec == timeout.sec) && (curr_time.usec >= timeout.usec)))
      {
         return WPL_OK;
      }

      WPL_TimestampGet(&curr_time);
   }
}
