/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_winmon_winpath2/sources/wpl_time.c
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

#include "wpl_types.h"
#include <wpl_platform.h>
#include <wpl_board.h>
#include <wpl_mem_access.h>
#include <wpli_time.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_TIME_THRESHOLD  0x7FFFFFFF /* 01111111 11111111 11111111 11111111 */

#define MILLION             1000000
#define WPLI_MEGAHERZ       1000000

#define WPLI_CLOCK_BASE 0xfc00

/* ------------------------ CLOCK ----------------------- */
#define WPLI_DIV_REG_NUM 0x6

#define WPLI_CLOCK(base) ((wpli_clock *) ((base) + WPLI_CLOCK_BASE))

#define WPI_REG_GET(dest, reg) WP_MEM_GET(dest, reg)

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

typedef struct wpli_clock {
      WP_U32 ck_cfg;
      WP_U32 ck_pllcfg;
      WP_U32 ck_dllcfg;
      WP_U32 ck_pltt;
      WP_U32 pad1[12];
      WP_U32 ck_divint[WPLI_DIV_REG_NUM];
      WP_U32 pad2[2];
      WP_U32 ck_divdat[WPLI_DIV_REG_NUM];
      WP_U32 pad3[2];
      WP_U32 ck_divpar[WPLI_DIV_REG_NUM];
      WP_U32 pad4[2];
      WP_U32 ck_divreg[WPLI_DIV_REG_NUM];
      WP_U32 pad5[2];
      WP_U32 ck_divhos[WPLI_DIV_REG_NUM];
      WP_U32 pad6[2];
      WP_U32 ck_divsyn[WPLI_DIV_REG_NUM];
      WP_U32 pad7[2];
      WP_U32 ck_divgen1[WPLI_DIV_REG_NUM];
      WP_U32 pad8[2];
      WP_U32 ck_divgen2[WPLI_DIV_REG_NUM];
      WP_U32 pad9[2];
      WP_U32 ck_divgen3[WPLI_DIV_REG_NUM];
      WP_U32 pad10[2];
      WP_U32 ck_divgen4[WPLI_DIV_REG_NUM];
      WP_U32 pad11[2];
      WP_U32 ck_divgen5[WPLI_DIV_REG_NUM];
      WP_U32 pad12[2];
      WP_U32 ck_divgen6[WPLI_DIV_REG_NUM];
      WP_U32 pad13[2];
      WP_U32 ck_divgen7[WPLI_DIV_REG_NUM];
      WP_U32 pad14[2];
      WP_U32 ck_divgen8[WPLI_DIV_REG_NUM];
} wpli_clock;

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

static WP_boolean    wpli_is_init = WP_FALSE;
static WP_U32        wpli_time_factor;
static WP_U32        wpl_rif_base = 0;
static WPL_ta_block* wpl_ta_block;
WP_U32 wpli_speed = 0;

/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

#define WPL_TIME_REGISTER_READ(val, reg) WP_MEM_GET((val), *((WP_U32*)WPL_TIME_##reg))

#define WPL_TA(base) ((WPL_ta_block *) ((base) + WPL_TA_BASE))

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

extern void internal_clock(WP_U32 *i, WP_U32 *f);
WP_U32 WPLI_Delay(WP_U32 interval);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

/* This module is not implemented for WP2 winmon */
WP_U32 WPL_TimeInit(void)
{
   WP_U32 prescaler;

   /* Get rif base, if required */
   if (wpl_rif_base == 0)
   {
      WPLI_BusesRifBaseGet(&wpl_rif_base);
   }

   wpl_ta_block = WPL_TA(wpl_rif_base);
   WP_MEM_GET(prescaler, wpl_ta_block->ts_prescaler);

   /* Calculate factor to divide by when getting timestamps */
   wpli_time_factor = (WPL_INTERNAL_CLOCK_FREQ_INT * MILLION + WPL_INTERNAL_CLOCK_FREQ_FRAC) / ((prescaler + 1) * MILLION);

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
   WP_MEM_GET(high, wpl_ta_block->ts_timer[0]);
   WP_MEM_GET(low, wpl_ta_block->ts_timer[1]);
   if (low < WPL_TIME_THRESHOLD)
   {
      WP_MEM_GET(high, wpl_ta_block->ts_timer[0]);
   }

   /* Unite high and low bits */
   total = (((WP_U64)high) << 32) + low;

   /* Translate to <seconds, microseconds> format */
   total /= wpli_time_factor;
   o_time->sec  = total / MILLION;
   o_time->usec = total % MILLION;

   return WPL_OK;
}

static WP_U32 WPLI_ClockDiv(WP_U32 *div)
{
  /* The process of configuring the divsor is as follows:
   * (1) Take the msb nibble of each byte (8 bytes total)
   *     the last meaningful nibble is the one with the lsb
   *     of its byte set.
   * (2) Count the number of transitions from '0' to '1' (but not from '1' to '0')
   * (3) Divide the number of PLL phases with the number of transitions
   *
   * Value returned is multiplied by 2 to add one more digit of accuracy.
   * Input is an array of adjucent clock divider registers
   */
   WP_U8 state[WPLI_DIV_REG_NUM][sizeof(WP_U32)];
   WP_U8 cur;
   WP_U32 last_div = WPLI_DIV_REG_NUM, last_byte = sizeof(WP_U32);
   WP_U32 byte_i,j, div_i;
   WP_U32 trans = 0;

   for (div_i = 0; div_i < last_div; div_i++)
      for (byte_i = 0; byte_i < last_byte; byte_i++)
      {
         cur = (div[div_i]>>(24 - 8*byte_i)) & 0xff;
         if (cur & 0x1) /* is it the last byte */
         {
            last_byte = byte_i;
            last_div  = div_i;
         }
         state[div_i][byte_i] = cur & 0xf0;
      }

   cur = state[last_div][last_byte] & 0x10;   /* Get init bit */

   for (div_i = 0; div_i <= last_div; div_i++)
      for (byte_i = 0; byte_i < 4; byte_i++)
      {
         WP_U8 mask = 0x80;

         for ( j = 0; j < 4; j++)
         {
            if (cur == 0 && (state[div_i][byte_i] & mask))
               trans++;
            cur = state[div_i][byte_i]&mask;
            mask >>= 1;
         }
         if( div_i == last_div && byte_i == last_byte)
            break;
      }

   /*the number of bits divided by the number of trans */
   return 4*(last_div * sizeof(WP_U32) + last_byte+1)/trans;
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
 *     wpli_speed
 *
 * Called by:
 *     WDDI
 *
 ****************************************************************************/

WP_U32 WPL_GetProcessorSpeed(void)
{
   if (wpli_speed == 0)
   {
      WP_U32 pll_config, sysClockDivConfig[WPLI_DIV_REG_NUM], sysclockdivisor;
      WP_U32 pfd, pod, prd, poc, sysclock;
      wpli_clock *clock;
      WP_S32 i;

      clock = WPLI_CLOCK(WPL_RegBaseGet(0));

      WPI_REG_GET(pll_config, clock->ck_pllcfg);
      for (i = 0; i < WPLI_DIV_REG_NUM; i++)
      {
         WPI_REG_GET(sysClockDivConfig[i], clock->ck_divint[i]);
      }

      sysclockdivisor = WPLI_ClockDiv(sysClockDivConfig);

      sysclock = WPL_EXTERNAL_CLOCK;
      pfd   = (pll_config & 0x000fff);     /* bits 0-11 */
      pod   = (pll_config & 0x007000)>>12; /* bits 12-14 */
      prd   = (pll_config & 0x3f0000)>>16; /* bits 16-21 */
      poc   = (pll_config & 0xc00000)>>22; /* bits 22-23 */

      sysclock /= (prd+1);
      sysclock /= (pod+1);
      sysclock *= (pfd+1);
      sysclock /= (poc == 1 ? 2 : (poc == 3 ? 4 : 1)); /* calc phdiv */

      wpli_speed = 2*sysclock / sysclockdivisor;
   }

   return wpli_speed;
}


/*****************************************************************************
 *
 * Function: WPL_Delay
 *
 * Purpose: Internal function to consume some time
 *
 * Description:  This function repeatedly calls WPI_TimeDelta() until the
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

WP_U32 WPL_Delay(WP_U32 interval)
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

   return WPL_OK;
}

void usleep(WP_U32 period)
{
   WPL_Delay(period);
}

