/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/winsim_winpath3/sources/wpl_time.c
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

#include <stdlib.h>
#include <sys/time.h>

#include "wpl_types.h"
#include <wpl_platform.h>
#include <wpl_board.h>


/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_TimeInit(void)
{
   return WPL_OK;
}

WP_U32 WPL_TimestampGet(WP_time* o_time)
{
   struct timeval tv;

   gettimeofday(&tv, NULL);
   o_time->sec  = tv.tv_sec;
   o_time->usec = tv.tv_usec;

   return WPL_OK;
}

/*****************************************************************************
 *
 * Function: WPL_GetProcessorSpeed
 *
 * Purpose: Return processor speed in Hz
 *
 * Description:
 *     Calculate processor speed from the pllConfig value
 *
 * Inputs:
 *     None
 *
 * Outputs:
 *     None
 *
 * Return Value:
 *     wpi_speed
 *
 * Called by:
 *     WDDI
 *
 ****************************************************************************/

WP_U32 WPL_GetProcessorSpeed(void)
{
   return WPL_PROCESSOR_SPEED;
}

/*******************************************************************
 * Function to delay
 * For Winsim, use usleep()
 *******************************************************************/
void usleep(WP_U32);
WP_U32 WPL_Delay(WP_U32 microseconds)
{
   usleep(microseconds);
   return WPL_OK;
}

WP_U32 WPL_Wait(WP_U32 wait_length)
{
   if (wait_length == 0)
      wait_length = 1;
   usleep(wait_length);
   return WPL_OK;
}
