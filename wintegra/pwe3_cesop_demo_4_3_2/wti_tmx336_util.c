/*****************************************************************************
 * (C) Copyright 2000-2004, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wti_ufe_util.c
 * UFE2 demo utility functions for PWE3 CES application.
 *  Demonstrates the use of UFE2 API.
 *
 ****************************************************************************/
#include <string.h>

#ifdef __WT_PMC__
#ifdef __WT_UFE3__

#include "wti_tmx336_util.h"

static WPX_tmx336_cfg tmx336_cfg;
extern WP_U32 upi_index;

static void WT_Tmx336TerminateOnError(WP_handle handle, WP_CHAR *s,WP_U32 id,WP_U32 LineNum)
{
   if(handle != WUFE_OK)
   {
      printf("Test Abort %s %d %s 0x%x Line:%d\n", s,id,
             WUFE_error_name[(handle&0x03ffffff)], handle, LineNum);

      exit(1);

   }
#if WT_TMX336_DBG_MODE > 0
   else
   {
      printf("Test info %s %d 0x%x Line:%d\n", s,id, handle, LineNum);
   }
#endif
}

/**********************************************************************************
 **********************************************************************************
 ************           External Chip Init configuratipn     *********************
 **********************************************************************************
 *********************************************************************************/

/* mode 0-internal, 
        1-loopt; 
type 0-t1 unframed, 
     1-t1 framed, 
     2-e1 unframed, 
     3-e1 framed 
     4-ds3 unframed, 
     5-ds3 framed, 
     6-e3 unframed, 
     7-e3 framed
     8-t1 esf (for t1 cas)
     9-e1 cas  */    

int WT_Tmx336SbiSetup(int mode, int type)
{
   WP_U32 spe, stm1, link;
   WP_U32 status;

   tmx336_cfg.oc_n = WPX_TMX336_OC_12; /*WPX_TMX336_OC_3;*/
   tmx336_cfg.line_if = WPX_TMX336_LINE_IF_OPTICAL;
   tmx336_cfg.system_if = WPX_TMX336_SYS_IF_SBI;
   tmx336_cfg.lpbck_type = WPX_TMX336_LPBCK_MAPPER_NOLOOP;

   for(stm1 = 0; stm1<4; stm1++)
   {
      WPX_tmx336_stm1_cfg *stm1_cfg = &(tmx336_cfg.stm1_cfg[stm1]);

      stm1_cfg->is_idle = TMX336_FALSE;
      stm1_cfg->payload = WPX_TMX336_PAYLOAD_AU3; /*WPX_TMX336_PAYLOAD_AU4;*/

      if (mode & 0x1 )/*WTI_CESOP_REGRESSION_TEST*/
         stm1_cfg->timing_mode = WPX_TMX336_TIMING_MODE_INTERNAL;
      else
      {
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)

         /* --------------------------------------------------------------------------------------- */
         /* In Current UFE3 (no ext PLL) must work in internal mode in CR tests                     */
         /* In CR tests with testing equipment - the testing equipment should be in looptiming mode */
         /* NOTE !!!!  This define can be removed when NEW UFE3 card with EXT PLL will be available */
         /* --------------------------------------------------------------------------------------- */

         stm1_cfg->timing_mode = WPX_TMX336_TIMING_MODE_INTERNAL;
#else
         stm1_cfg->timing_mode = WPX_TMX336_TIMING_MODE_LOOPT;
#endif
      }

      stm1_cfg->c2_octet = WPX_TMX336_C2_FLOAT_VT;
      stm1_cfg->h1_ss_bits = WPX_TMX336_H1_SS_SDH;
      stm1_cfg->lpbck_type = WPX_TMX336_LPBCK_SONET_NOLOOP;

      for (spe = 0; spe<3; spe++)
      {
         WPX_tmx336_spe_cfg *spe_cfg = &stm1_cfg->spe_cfg[spe];
         spe_cfg->is_idle = TMX336_FALSE;
         spe_cfg->trib_mapping = WPX_TMX336_TRIB_MAP_BITASYNC;
         if (type < 2 || (type==8))   /* t1 framed/unframed, or t1-esf (usef for t1 cas) */
         {
            spe_cfg->mapper = WPX_TMX336_MAPPER_T1E1;
            spe_cfg->trib_type = WPX_TMX336_TRIB_TYPE_TU11_T1;
            spe_cfg->frmr = WPX_TMX336_FRMR_MODE_NA;
            spe_cfg->ds3_timing = WPX_TMX336_DS3_CLK_NA;
         }
         else if (type < 4 || (type==9)) /* e1 framed/unframed, or e1-cas */
         {
            spe_cfg->mapper = WPX_TMX336_MAPPER_T1E1;
            spe_cfg->trib_type = WPX_TMX336_TRIB_TYPE_TU12_E1;
            spe_cfg->frmr = WPX_TMX336_FRMR_MODE_NA;
            spe_cfg->ds3_timing = WPX_TMX336_DS3_CLK_NA;
         }
         else if (type < 6)
         {
            spe_cfg->mapper = WPX_TMX336_MAPPER_DS3;
            spe_cfg->trib_type = WPX_TMX336_TRIB_TYPE_NA;
            if (type == 4) /*DS3 unframed*/
               spe_cfg->frmr = WPX_TMX336_FRMR_MODE_UNCHAN;
            else /*DS3 framed*/
               spe_cfg->frmr = WPX_TMX336_DS3_FRMR_MODE_CBIT;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
            spe_cfg->ds3_timing = WPX_TMX336_DS3_CLK_TICLK;
#else
            spe_cfg->ds3_timing = WPX_TMX336_DS3_CLK_RECOVER;
#endif

         }
         else if (type < 8)
         {
            spe_cfg->mapper = WPX_TMX336_MAPPER_DS3;
            spe_cfg->trib_type = WPX_TMX336_TRIB_TYPE_NA;
            if (type == 6) /*E3 unframed*/
               spe_cfg->frmr = WPX_TMX336_FRMR_MODE_UNCHAN;
            else /*E3 framed*/
               spe_cfg->frmr = WPX_TMX336_E3_FRMR_MODE_G751;/*G832 not supported over SBI*/

            spe_cfg->ds3_timing = WPX_TMX336_DS3_CLK_RECOVER;
         }
         else
         {
            printf("WT_Tmx336SbiSetup: invalid type %d\n", type);
            return -1;
         }

         for(link = 0; link<28; link ++)
         {
            spe_cfg->lpbck_type[link] = WPX_TMX336_LPBCK_E1T1_NOLOOP;
            if (type == 0)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_T1_UNFRM;
            else if (type == 1)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_T1_SF;
            else if (type == 2)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_E1_UNFRM;
            else if (type == 3)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_E1_STD;
            else if (type == 4)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_DS3_UNFRM;
            else if (type == 5)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_DS3_FRM;
            else if (type == 6)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_E3_UNFRM;
            else if (type == 7)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_E3_FRM;
            else if (type == 8)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_T1_ESF;   
            else if (type == 9)
               spe_cfg->op_mode[link] = WPX_TMX336_OPMODE_E1_CAS; 
            else
            {
               printf("WT_Tmx336SbiSetup: invalid type %d\n", type);
               return -1;
            }
            if (mode == 0x1 )
               spe_cfg->e1t1_timing[link] = WPX_TMX336_E1T1_CLK_CTCLK;
            else if(mode == 0x0)
               spe_cfg->e1t1_timing[link] = WPX_TMX336_E1T1_CLK_INGRESS;
            else
               spe_cfg->e1t1_timing[link] = WPX_TMX336_E1T1_CLK_EGRESS;
         }
      }
   }

   status = WPX_PmcTmx336Init(0,upi_index,&tmx336_cfg);
   WT_Tmx336TerminateOnError(status, "WPX_PmcTmx336Setup" ,0,__LINE__);
/*   status = WPX_PmcTmx336RegisterDump(0, 0);*/
   return 0;
}

#endif /*__WT_PMC__*/
#endif /*__WT_UFE3__*/
