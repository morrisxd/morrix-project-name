/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 * (C) Copyright Wintegra 2001-2008.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/


/*****************************************************************************
 *
 * Module:
 * Purpose:  This function is used by some interworking examples.
 *           It should not be replicated.
 *           Its use is not required by WDDI.

 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 *****************************************************************************/
#ifndef _APP_CONVERT_H
#define _APP_CONVERT_H

#include "wp_wddi.h"

void App_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_S32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {

          if ( Ascii[2*i] >= '0' &&  Ascii[2*i] <= '9')
            first =  Ascii[2*i] - '0';

          if ( Ascii[2*i] >= 'a' &&  Ascii[2*i] <= 'f')
             first =  Ascii[2*i] - 'a' + 10;

          if ( Ascii[2*i] >= 'A' &&  Ascii[2*i] <= 'F')
             first =  Ascii[2*i] - 'A' + 10;

          if ( Ascii[2*i+1] >= '0' &&  Ascii[2*i+1] <= '9')
            second =  Ascii[2*i+1] - '0';
          if ( Ascii[2*i+1] >= 'a' &&  Ascii[2*i+1] <= 'f')
            second =  Ascii[2*i+1] - 'a' + 10;
          if ( Ascii[2*i+1] >= 'A' &&  Ascii[2*i+1] <= 'F')
            second =  Ascii[2*i+1] - 'A' + 10;

          Hex[i]=(WP_CHAR )(first * 16 + second);
     }
    return;
}


void App_TranslateHexToAscii(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length)
{
   WP_S32 i;
   WP_U8 first=0,second=0;
   
   for (i=0;i<length;i++)
   {
      first = Hex[i]%16;
      second = Hex[i]>>4;
      
      if ( first <= 9)
      {
         Ascii[2*i +1]= first + '0';
      }
      else
      {
         Ascii[2*i +1] = first + 'a' -10;
      }
      
      if ( second <= 9)
      {
         Ascii[2*i]= second + '0';
      }
      else
      {
         Ascii[2*i] = second + 'a' -10;
      }
   }
   /* Terminate with null */
   Ascii[2 * i] = 0;
   return;
}

#endif
