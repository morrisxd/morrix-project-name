/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#include <stdio.h>
#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif

#ifndef WUFEI_BOARD_SUPPORT_H
#include "wufei_board_support.h"
#endif

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFEI_MEMORY_ACCESS_INT_H
#include "wufei_memory_access_int.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

WP_CHAR wufei_log[400];

static WP_U32 use_board_support_commands = WUFE_BOARD_SUPPORT_DISABLE;

extern WP_U8 debug_flag;

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaReleaseIdGet
 *
 * Purpose: reads the release id
 *
 * Description:
 *     Reads the release id from the board support register
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_BsFpgaInit
 *
 ****************************************************************************/
static WUFE_status WUFEI_BsFpgaReleaseIdGet(WP_U32 ufeid)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *release_ptr;
   WP_U16 value;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->release_id;
   release_ptr = &(bs_ptr->value);

   WUFEI_REG_GET(release_ptr, &value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaPllStatusGet
 *
 * Purpose: checks the PLL status
 *
 * Description:
 *     check the PLL lock after the board support reset
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HW_PLL_LOCK
 *
 * Called by:
 *     WUFEI_BsFpgaInit
 *
 ****************************************************************************/
static WUFE_status WUFEI_BsFpgaPllStatusGet(WP_U32 ufeid)
{
   WP_U32 bs_base, timeout = 0;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *pll_stat_ptr;
   WP_U16 value;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->status[0];
   pll_stat_ptr = &(bs_ptr->value);

   do{

      WUFEI_REG_GET(pll_stat_ptr, &value);
      WPL_Delay(WUFEI_PLL_LOCK_DELAY);
      timeout ++;

      if(timeout > WUFEI_PLL_LOCK_TIMEOUT)
      {
         WUFE_PRINT_ERR((wufei_log, "BS did not step out of PLL lock\n"));
         return WUFE_ERR_HW_PLL_LOCK;
      }

   }
   while(!WUFEI_BS_STATUS_0_LOCK_SYS_V(value) ||
         !WUFEI_BS_STATUS_0_LOCK_ZBT_V(value) ||
         !WUFEI_BS_STATUS_0_LOCK_SBI_V(value) ||
         !WUFEI_BS_STATUS_0_LOCK_HMVIP_V(value));

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaReset
 *
 * Purpose: resets the board suppport
 *
 * Description:
 *    performs the board support reset
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *      WUFE_ERR_HW_RESET
 *
 * Called by:
 *     WUFEI_BsFpgaInit
 *
 ****************************************************************************/

WUFE_status WUFEI_BsFpgaReset(WP_U32 ufeid)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *reset_ptr;
   WP_U16 value;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->reset;
   reset_ptr = &(bs_ptr->value);

   WUFEI_REG_GET(reset_ptr, &value);
   if(WUFEI_BS_RESET_SW_V(value))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFEI_BsFpgaReset: invalid reset value(0x%4X)\n",
                      value));
      return WUFE_ERR_HW_RESET;
   }

   value |= WUFEI_BS_RESET_SW_F(1);

   /* Reset the BS*/
   WUFEI_REG_SET(reset_ptr, value);

   WUFEI_REG_GET(reset_ptr, &value);
   if(!WUFEI_BS_RESET_SW_V(value))
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFEI_BsFpgaReset: FPGA did not enter reset\n"));
      return WUFE_ERR_HW_RESET;
   }

   WPL_Delay (8000);

   value &= ~WUFEI_BS_RESET_SW_M;
   WUFEI_REG_SET(reset_ptr, value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaRelease
 *
 * Purpose: resets the board suppport
 *
 * Description:
 *    performs the board support reset
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_UfeRelease
 *
 ****************************************************************************/
WUFE_status WUFEI_BsFpgaRelease(WP_U32 ufeid)
{
   WUFE_status status;

   /*Board support release*/
   if(use_board_support_commands == WUFE_BOARD_SUPPORT_ENABLE)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaReset(ufeid));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaGlobalCfgSet
 *
 * Purpose: configures the global configuration register 1,5,6,7,8
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_BsFpgaInit
 *
 ****************************************************************************/
static WUFE_status WUFEI_BsFpgaGlobalCfgSet(WP_U32 ufeid)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *global_cfg_ptr;
   WP_U16 i, reg_n, value = 0;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   /*Global configuration #1*/
   bs_ptr = &((WUFEI_hw_bs*)bs_base)->global_cfg[1];
   global_cfg_ptr = &(bs_ptr->value);

   value = (WUFEI_BS_GLOBAL_CFG_1_SAC1FP_F(0)|
            WUFEI_BS_GLOBAL_CFG_1_SDC1FP_F(0)|
            WUFEI_BS_GLOBAL_CFG_1_CMVFPB_MD_F(0));

   WUFEI_REG_SET(global_cfg_ptr, value);

   /*Global configuration #5 - C1FP\A1FP delay*/
   bs_ptr = &((WUFEI_hw_bs*)bs_base)->global_cfg[5];
   global_cfg_ptr = &(bs_ptr->value);

   value = WUFEI_BS_GLOBAL_CFG_5_DC1FP_DELAY_F(WUFEI_BS_DC1FP_DELAY_NONE) |
           WUFEI_BS_GLOBAL_CFG_5_AC1FP_DELAY_F(WUFEI_BS_AC1FP_DELAY_NONE);

   WUFEI_REG_SET(global_cfg_ptr, value);

   /*Global configuration #6,7,8 - default*/
   /* 0-11 JAT number, '1101' - DS3, '1100' - E3 */
   for(reg_n=0; reg_n < WUFEI_BS_GLOBAL_CFG_TICLK_SEL_N; reg_n++)
   {
      value = 0;
      for(i=0; i < 4; i++)
         value |= WUFEI_BS_GLOBAL_CFG_TICLK_SEL_F(WUFEI_BS_TICLK_SEL_DS3, i);

      bs_ptr = &((WUFEI_hw_bs*)bs_base)->global_cfg[reg_n+6];
      global_cfg_ptr = &(bs_ptr->value);
      WUFEI_REG_SET(global_cfg_ptr, value);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_Ac1fpDelaySet
 *
 * Purpose: configures AC1FP delayin global register 5
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *     delay - the dealy to put in the register
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwSbiGmrSetup
 *
 ****************************************************************************/
WUFE_status WUFEI_Ac1fpDelaySet(WP_U32 ufeid, WP_U16 delay)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *global_cfg_ptr;
   WP_U16 value;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_ptr = &((WUFEI_hw_bs*)bs_base)->global_cfg[5];
   global_cfg_ptr = &(bs_ptr->value);

   WUFEI_REG_GET(global_cfg_ptr, &value);
   value&=~WUFEI_BS_GLOBAL_CFG_5_AC1FP_DELAY_M;
   value|= WUFEI_BS_GLOBAL_CFG_5_AC1FP_DELAY_F(delay);
   WUFEI_REG_SET(global_cfg_ptr, value);

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_BsTiclkInSpeConnect
 *
 * Purpose: configures the global configuration register 0
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *     spe - values [0...2]
 *     ja - jitter attenuator values [0...2]
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *      Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_BsTiclkInSpeConnect(WP_U32 ufeid, WP_U16 spe, WP_U8 ja)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *global_cfg_ptr;
   WP_U16 entry_n, reg_n, value = 0;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   if( ja >= WUFEI_BS_MAX_N_JAT_OUTPUT ||
       spe >= WUFEI_MAX_N_SPE * WUFEI_MAX_N_STM1 * WUFEI_MAX_N_CORE)
      return WUFE_ERR_INVALID_CONFIG;

   /* calculate the global cfg register number */
   reg_n = spe/4;
   entry_n = spe%4;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_ptr = &((WUFEI_hw_bs*)bs_base)->global_cfg[reg_n+6];
   global_cfg_ptr = &(bs_ptr->value);

   /*Global configuration #6,7,8 - default*/
   WUFEI_REG_GET(global_cfg_ptr, &value);

   value &= ~WUFEI_BS_GLOBAL_CFG_TICLK_SEL_M(entry_n);
   value |= WUFEI_BS_GLOBAL_CFG_TICLK_SEL_F(WUFEI_BS_JAT_OUTPUT_N(ja), entry_n);

   WUFEI_REG_SET(global_cfg_ptr, value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsLineCadSetSyncType
 *
 * Purpose: configures the E1/T1 transfer mode
 *
 * Description:
 *        writes to the Board Support transfer_mode register E1 or T1 mode
 *
 * Inputs:
 *     ufeid - the UFE id
 *     transfer_mode - E1 ot T1
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_BsFpgaInit
 *
 ****************************************************************************/
WUFE_status WUFEI_BsLineCadSetSyncType(WP_U32 ufeid,
                                       WUFE_line_transf_type type,
                                       WP_U32 line_id,
                                       WP_U32 sys_line_interface)
{
   WUFEI_hw_bs* bs_base;
   WUFEI_reg_space* transfer_mode_ptr;
   WP_U16 *reg_ptr;
   WP_U16 value;
   ((void)type);   
   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   if (sys_line_interface == WUFE_INTERFACE_CAD_E1_T1)
   {

      if (line_id > WUFE_MAX_N_CAD_LINE_E1T1)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Error: invlid line_id (%d)\n", line_id));
         return WUFE_ERR_INVALID_CONFIG;
      }

      bs_base = (WUFEI_hw_bs*)WUFEI_UfeBsBaseAddrGet(ufeid);
      transfer_mode_ptr = &(bs_base->global_cfg[2]);
      reg_ptr = &(transfer_mode_ptr->value);

      /* value should be set to '0' */
      value = ~WUFEI_BS_GLOBAL_CFG_2_DATA_SAMP_0_M & ~WUFEI_BS_GLOBAL_CFG_2_FP_SAMP_0_M &
         ~WUFEI_BS_GLOBAL_CFG_2_FP_DATA_0_M & ~WUFEI_BS_GLOBAL_CFG_2_DATA_SAMP_1_M &
         ~WUFEI_BS_GLOBAL_CFG_2_FP_SAMP_1_M & ~WUFEI_BS_GLOBAL_CFG_2_FP_DATA_1_M;

      WUFEI_REG_SET(reg_ptr, value);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsLineCadClockRecTypeSelect
 *
 * Purpose: configures the clock mode in the CAD mux in the BS logic
 *
 * Description:
 *        writes to the Board Support CR_mux2:1 register ACR/DCR or extended DCR mode
 *
 * Inputs:
 *     ufeid - the UFE id
 *     cd_id - clock domain ID
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_LineCadClockRecConfig
 *
 ****************************************************************************/
WUFE_status WUFEI_BsLineCadClockRecTypeSelect(WP_U32 ufeid,
                                              WP_U32 sys_line_interface,
                                              WP_U32 line_id,
                                              WP_U32 cd_id)
{
   WUFEI_hw_bs* bs_base;
   WUFEI_reg_space *mux0_ptr, *mux1_ptr, *mux2_ptr, *mux3_ptr;
   WP_U16 *reg0_ptr, *reg1_ptr, *reg2_ptr, *reg3_ptr;
   WP_U16 value0, value1, value2, value3;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   if (sys_line_interface == WUFE_INTERFACE_CAD_E1_T1)
   {

      if (line_id > WUFE_MAX_N_CAD_LINE_E1T1)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Error: invlid line_id (%d)\n", line_id));
         return WUFE_ERR_INVALID_CONFIG;
      }

      bs_base = (WUFEI_hw_bs*)WUFEI_UfeBsBaseAddrGet(ufeid);
      mux0_ptr = &(bs_base->global_cfg[3]);
      mux1_ptr = &(bs_base->global_cfg[4]);
      mux2_ptr = &(bs_base->global_cfg[12]); /* core1 registers */
      mux3_ptr = &(bs_base->global_cfg[13]); /* core1 registers */
      reg0_ptr = &(mux0_ptr->value);
      reg1_ptr = &(mux1_ptr->value);
      reg2_ptr = &(mux2_ptr->value);
      reg3_ptr = &(mux3_ptr->value);

      WUFEI_REG_GET(reg0_ptr, &value0);
      WUFEI_REG_GET(reg1_ptr, &value1);
      WUFEI_REG_GET(reg2_ptr, &value2);
      WUFEI_REG_GET(reg3_ptr, &value3);
      
      if (cd_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         if (cd_id < WUFE_CLK_REC_UNITS_IN_CORE)
         {
            if (cd_id < WUFE_CLK_REC_UNITS_IN_REG)
               value0 &= ~WUFEI_BS_GLOBAL_CFG_3_MUXSEL_M(cd_id);
            else
               value1 &= ~WUFEI_BS_GLOBAL_CFG_4_MUXSEL_M((cd_id-WUFE_CLK_REC_UNITS_IN_REG));
         }
         else /* core1 registers */
         {
            if (cd_id - WUFE_CLK_REC_UNITS_IN_CORE < WUFE_CLK_REC_UNITS_IN_REG)
               value2 &= ~WUFEI_BS_GLOBAL_CFG_C_MUXSEL_M((cd_id - WUFE_CLK_REC_UNITS_IN_CORE));
            else
               value3 &= ~WUFEI_BS_GLOBAL_CFG_D_MUXSEL_M((cd_id - WUFE_CLK_REC_UNITS_IN_CORE - WUFE_CLK_REC_UNITS_IN_REG));
         }
      }
      else /* cd_id >=64 - extended differential mode */
      {
         cd_id = cd_id - WUFE_CLOCK_REC_REGULAR_INSTANCES;
         if (cd_id < WUFE_CLK_REC_UNITS_IN_CORE)
         {
            if (cd_id < WUFE_CLK_REC_UNITS_IN_REG)
               value0 |= WUFEI_BS_GLOBAL_CFG_3_MUXSEL_M(cd_id);
            else
               value1 |= WUFEI_BS_GLOBAL_CFG_4_MUXSEL_M((cd_id-WUFE_CLK_REC_UNITS_IN_REG));
         }
         else /* core1 registers */
         {
            if (cd_id - WUFE_CLK_REC_UNITS_IN_CORE < WUFE_CLK_REC_UNITS_IN_REG)
               value2 |= WUFEI_BS_GLOBAL_CFG_C_MUXSEL_M((cd_id - WUFE_CLK_REC_UNITS_IN_CORE));
            else
               value3 |= WUFEI_BS_GLOBAL_CFG_D_MUXSEL_M((cd_id - WUFE_CLK_REC_UNITS_IN_CORE - WUFE_CLK_REC_UNITS_IN_REG));
         }
      }
      
      WUFEI_REG_SET(reg0_ptr, value0);
      WUFEI_REG_SET(reg1_ptr, value1);
      WUFEI_REG_SET(reg2_ptr, value2);
      WUFEI_REG_SET(reg3_ptr, value3);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsLineCadClockRecEnable
 *
 * Purpose: Enable CR module in Board Support
 *
 * Description:
 *        writes to the Board Support cr_active register: '1' CR enable '0' CR disable
 *
 * Inputs:
 *     ufeid - the UFE id
 *     sys_line_interface - system IF (CAD for E1/T1)
 *     line_id - the line ID
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_LineEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_BsLineCadClockRecEnable(WP_U32 ufeid,
                                          WP_U32 sys_line_interface,
                                          WP_U32 line_id)
{
   WUFEI_hw_bs* bs_base;
   WUFEI_reg_space* clock_rec_active_ptr;
   WP_U16 *reg_ptr;
   WP_U16 value;
   

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   if (sys_line_interface == WUFE_INTERFACE_CAD_E1_T1)
   {

      if (line_id > WUFE_MAX_N_CAD_LINE_E1T1)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Error: invlid line_id (%d)\n", line_id));
         return WUFE_ERR_INVALID_CONFIG;
      }

      bs_base = (WUFEI_hw_bs*)WUFEI_UfeBsBaseAddrGet(ufeid);
      if (line_id < WUFE_CLK_REC_UNITS_IN_CORE)
      {
         /* core0 registers */
         if (line_id < WUFE_CLK_REC_UNITS_IN_REG)
         {
            clock_rec_active_ptr = &(bs_base->cr_active[0]);
            reg_ptr = &(clock_rec_active_ptr->value);

            WUFEI_REG_GET(reg_ptr, &value);
            value |= (0x1 << line_id);
            WUFEI_REG_SET(reg_ptr, value);
         }
         else
         {
            WP_U32 id = line_id - WUFE_CLK_REC_UNITS_IN_REG;

            clock_rec_active_ptr = &(bs_base->cr_active[1]);
            reg_ptr = &(clock_rec_active_ptr->value);

            WUFEI_REG_GET(reg_ptr, &value);
            value |= (0x1 << id);
            WUFEI_REG_SET(reg_ptr, value);
         }
      }
      else
      {
         /* core1 registers */
         if (line_id - WUFE_CLK_REC_UNITS_IN_CORE < WUFE_CLK_REC_UNITS_IN_REG)
         {
            clock_rec_active_ptr = &(bs_base->cr_active[2]);
            reg_ptr = &(clock_rec_active_ptr->value);

            WUFEI_REG_GET(reg_ptr, &value);
            value |= (0x1 << (line_id - WUFE_CLK_REC_UNITS_IN_CORE));
            WUFEI_REG_SET(reg_ptr, value);
         }
         else
         {
            WP_U32 id = line_id - WUFE_CLK_REC_UNITS_IN_CORE - WUFE_CLK_REC_UNITS_IN_REG;

            clock_rec_active_ptr = &(bs_base->cr_active[3]);
            reg_ptr = &(clock_rec_active_ptr->value);

            WUFEI_REG_GET(reg_ptr, &value);
            value |= (0x1 << id);
            WUFEI_REG_SET(reg_ptr, value);
         }
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsLineCadClockRecDisable
 *
 * Purpose: Disbale CR module in Board Support
 *
 * Description:
 *        writes to the Board Support cr_active register: '1' CR enable '0' CR disable
 *
 * Inputs:
 *     ufeid - the UFE id
 *     sys_line_interface - system IF (CAD for E1/T1)
 *     line_id - the line ID
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_LineDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_BsLineCadClockRecDisable(WP_U32 ufeid,
                                           WP_U32 sys_line_interface,
                                           WP_U32 line_id)
{
   WUFEI_hw_bs* bs_base;
   WUFEI_reg_space* clock_rec_active_ptr;
   WP_U16 *reg_ptr;
   WP_U16 value;
   

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   if (sys_line_interface == WUFE_INTERFACE_CAD_E1_T1)
   {

      if (line_id > WUFE_MAX_N_CAD_LINE_E1T1)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "Error: invlid line_id (%d)\n", line_id));
         return WUFE_ERR_INVALID_CONFIG;
      }

      bs_base = (WUFEI_hw_bs*)WUFEI_UfeBsBaseAddrGet(ufeid);
      if (line_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         /* regular mode */
         if (line_id < WUFE_CLK_REC_UNITS_IN_REG)
         {
            clock_rec_active_ptr = &(bs_base->cr_active[0]);
            reg_ptr = &(clock_rec_active_ptr->value);
         
            WUFEI_REG_GET(reg_ptr, &value);
            value &= ~(0x1 << line_id);
            WUFEI_REG_SET(reg_ptr, value);
         }
         else
         {
            WP_U32 id = line_id - WUFE_CLK_REC_UNITS_IN_REG;
         
            clock_rec_active_ptr = &(bs_base->cr_active[1]);
            reg_ptr = &(clock_rec_active_ptr->value);
         
            WUFEI_REG_GET(reg_ptr, &value);
            value &= ~(0x1 << id);
            WUFEI_REG_SET(reg_ptr, value);
         }
      }
      else
      {
         /* extended mode */
         line_id = line_id - WUFE_CLOCK_REC_REGULAR_INSTANCES;
         if (line_id < WUFE_CLK_REC_UNITS_IN_REG)
         {
            clock_rec_active_ptr = &(bs_base->cr_active[2]);
            reg_ptr = &(clock_rec_active_ptr->value);
         
            WUFEI_REG_GET(reg_ptr, &value);
            value &= ~(0x1 << line_id);
            WUFEI_REG_SET(reg_ptr, value);
         }
         else
         {
            WP_U32 id = line_id - WUFE_CLK_REC_UNITS_IN_REG;
         
            clock_rec_active_ptr = &(bs_base->cr_active[3]);
            reg_ptr = &(clock_rec_active_ptr->value);
         
            WUFEI_REG_GET(reg_ptr, &value);
            value &= ~(0x1 << id);
            WUFEI_REG_SET(reg_ptr, value);
         }
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaInit
 *
 * Purpose: configures the board support according to the passed attributes
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_UfeInit
 *
 ****************************************************************************/
WUFE_status WUFEI_BsFpgaInit(WP_U32 ufeid)
{
   WUFE_status status;
   WP_U32 ufe2_full_model;

   /* check that UFE FPGA includes board support */
   ufe2_full_model = WUFEI_UfeCoreHwModelGet(ufeid);

   if (ufe2_full_model == WUFEI_GEN_CFG_SYS_REG_HM_CORE_ONLY)
      return WUFE_ERR_HW_CONFIG_NOT_MATCH;

   /* set the indication to work with board support */
   use_board_support_commands = WUFE_BOARD_SUPPORT_ENABLE;

   WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaReleaseIdGet(ufeid));

   WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaGlobalCfgSet(ufeid));

   /* FPGA RESET */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaReset(ufeid));

   /* Polling for the PLL status set*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaPllStatusGet(ufeid));

   return WUFE_OK;
}
