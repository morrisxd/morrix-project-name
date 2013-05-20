/*****************************************************************************
 * (C) Copyright 2000-2002, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <wp_wddi.h>

void sys_dcache_invalidate_index(void) {
return;
}

void WT_TranslateAsciiToHex(WP_CHAR *Hex, WP_CHAR *Ascii,
                            WP_U32 length)
{
   WP_S32 i, first = 0, second = 0;

   for (i=0;i<length;i++) {
      if (Ascii[2*i] >= '0' && Ascii[2*i] <= '9')
         first = Ascii[2*i] - '0';
      if (Ascii[2*i] >= 'a' && Ascii[2*i] <= 'f')
         first = Ascii[2*i] - 'a' + 10;
      if (Ascii[2*i+1] >= '0' && Ascii[2*i+1] <= '9')
         second = Ascii[2*i+1] - '0';
      if (Ascii[2*i+1] >= 'a' && Ascii[2*i+1] <= 'f')
         second = Ascii[2*i+1] - 'a' + 10;

      Hex[i] = (WP_CHAR)(first * 16 + second);
   }
   return;
}

void WT_Identify(void)
{
   WP_U32 p, w;
   WP_CHAR *first_dps_found = NULL;
   WP_U32 found_error = 0;
   printf("Test Identity Version %s\n", WP_Identity.wddi_version);
   printf("Test Identity Build %s\n", WP_Identity.wddi_build);
   for (p = 0; p < WP_MAX_WINPATHS; p++) {
      for (w = 0; w < WP_WINFARMS; w++) {
         if (!WP_Identity.dps_in_wddi[p][w] ||
             strlen(WP_Identity.dps_in_wddi[p][w]) == 0)
            continue;
         printf("Test Identity DPS WP%d WF%d in wddi %s in use %s\n", p, w,
                WP_Identity.dps_in_wddi[p][w],
                WP_Identity.dps_in_use[p][w]);
         if (!first_dps_found)
            first_dps_found = WP_Identity.dps_in_wddi[p][w];
         if (strcmp(WP_Identity.dps_in_wddi[p][w], first_dps_found) ||
            strcmp(WP_Identity.dps_in_use[p][w], first_dps_found))
         found_error = 1;
      }
   }
   if (found_error) {
      printf("Test Abort DPS not uniform\n");
   }
}
