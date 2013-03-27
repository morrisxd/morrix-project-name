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

#ifndef WUFEI_HW_GLOBALS_INT_H
#include "wufei_hw_globals_int.h"
#endif

/* Globals */
WP_CHAR wufei_log[400];
volatile struct i2c_regs *regs;
struct sfp_cmd cmd;

static WP_U32 use_board_support_commands = WUFE_BOARD_SUPPORT_DISABLE;
static void WUFEI_BoardWriteZarlinkPllReg(WP_U32 ufeid, WP_U32 address, WP_U32 data);
static void WUFEI_BoardReadZarlinkPllReg(WP_U32 ufeid, WP_U32 address, WP_U32 *data);

#if 0 /*Will be used in future expansions*/
static void WUFEI_HwBsIdirectAddrSet(WP_U32 ufeid, WP_U32 indirect_addr);
static WUFE_status WUFEI_HwBsIdirectDataSet(WP_U32 ufeid, WP_U16 value);
static WUFE_status WUFEI_HwBsIdirectDataGet(WP_U32 ufeid, WP_U16* value);

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsIdirectAddrSet
 *
 * Purpose: Indirect address set for the BS
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      indirect_addr - indirect address
 * Inputs:
 *
 *
 * Outputs:
 *     write to BS indirect address register
 *
 * Return Value: none
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwBsIdirectAddrSet(WP_U32 ufeid, WP_U32 indirect_addr)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_low.value;
   indha_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_high.value;

   WUFEI_REG_SET(indla_ptr, indirect_addr & 0xffff);
   WUFEI_REG_SET(indha_ptr, (indirect_addr >> 16) & 0xffff);

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsIdirectDataSet
 *
 * Purpose: Indirect data set for the BS
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      value - value to set
 * Inputs:
 *
 *
 * Outputs:
 *     write to BS indirect data register
 *
 * Return Value: None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwBsIdirectDataSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout=WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_cmd.value;

   WUFEI_REG_SET(inddat_ptr, value);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;

         WUFE_PRINT_ERR((wufei_log,"ERROR: Timeout while trying to set data %x in BS indirect register\n", value));
         ind_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", value));
         ind_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_high.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect high address was %x\n", value));

         return WUFE_ERR_INDIRECT_DATA_SET_TIMEOUT;
      }
      WPL_Delay(10);
      timeout--;
      WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsIdirectDataGet
 *
 * Purpose: Indirect data get for the BS
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      value - value to get
 * Inputs:
 *
 *
 * Outputs:
 *     get the BS indirect data
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwBsIdirectDataGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout=WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_cmd.value;

   WUFEI_REG_SET(indcmd_ptr, WUFEI_INDIRECT_RD_CMD);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;
         WP_U16 value;

         WUFE_PRINT_ERR((wufei_log,"ERROR: Timeout while trying to get data from BS indirect register\n"));
         ind_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", value));
         ind_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ind_high.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect high address was %x\n", value));

         return WUFE_ERR_INDIRECT_DATA_GET_TIMEOUT;
      }
      WPL_Delay(10);
      timeout--;
      WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   }
   WUFEI_REG_GET(inddat_ptr, value);
   return WUFE_OK;
}
#endif

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
   WP_U8 core_1_included = WUFEI_UfeCore1IncludedGet(ufeid);

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->bs_lock_indication;
   pll_stat_ptr = &(bs_ptr->value);

   do{
      WUFEI_REG_GET(pll_stat_ptr, &value);
      WPL_Delay(WUFEI_PLL_LOCK_DELAY);
      timeout ++;

      if(timeout > WUFEI_PLL_LOCK_TIMEOUT)
      {
         if (!WUFEI_BS_LOCK_IND_SYS_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: System PLL not locked\n"));
         else if (!WUFEI_BS_LOCK_IND_DDR_IO_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: DDR IO PLL not locked\n"));
         else if (!WUFEI_BS_LOCK_IND_FRAMER_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: framer PLL not locked\n"));
         else if (!WUFEI_BS_LOCK_IND_DDR0_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: DDR 0 PLL not locked\n"));
         else if (core_1_included && !WUFEI_BS_LOCK_IND_DDR1_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: DDR 1 PLL not locked\n"));
         else if (!WUFEI_BS_LOCK_EMPHY_RX_CLK_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: EMPHY Rx CLK not locked\n"));
         else if (!WUFEI_BS_LOCK_EMPHY_TX_CLK_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: EMPHY Tx CLK not locked\n"));
         return WUFE_ERR_HW_PLL_LOCK;
      }

   }
   while(!WUFEI_BS_LOCK_IND_SYS_V(value)                       ||
         !WUFEI_BS_LOCK_IND_DDR_IO_V(value)                    ||
         !WUFEI_BS_LOCK_IND_FRAMER_V(value)                    ||
         !WUFEI_BS_LOCK_IND_DDR0_V(value)                      ||
         (core_1_included && !WUFEI_BS_LOCK_IND_DDR1_V(value)) ||
         !WUFEI_BS_LOCK_EMPHY_RX_CLK_V(value) ||
         !WUFEI_BS_LOCK_EMPHY_TX_CLK_V(value)
        );

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
   WP_U32 attributes;
   WP_U8 connector_index;
   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   attributes= WUFEI_MappingAttribGet(ufeid);
   connector_index = WUFE_ATTRIB_CONNECT_OFFSET_V(attributes);
   if(connector_index > WUFE_ATTRIB_CONNECT_MAX_OFFSET)
	   return WUFE_ERR_INVALID_CONFIG;

   if(connector_index == 0)
      WPX_Ufe4HwReset(0,0);
   else /* For UPI3 special fix was made to WDS3 board to reset through SERIAL2 */
      WPX_Ufe4HwReset(0,1);

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
WUFE_status WUFEI_BsFpgaRelease(WP_U32 ufeid )
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
 * Function:  WUFEI_BsFpgaLedSet
 *
 * Purpose: Turn on the LEDs on the board
 *
 * Description:
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *     Turns on the leds
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_BsFpgaInit
 *
 ****************************************************************************/
static WUFE_status WUFEI_BsFpgaLedSet(WP_U32 ufeid)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *led_ptr;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->bs_gpled;
   led_ptr = &(bs_ptr->value);

   /* Turn on the The LEDs*/
   WUFEI_REG_SET(led_ptr, 0);

   return WUFE_OK;
}
/*****************************************************************************
 *
 * Function:  WUFEI_BoardWriteZarlinkPll1FreeRunFreqOffsetg
 *
 * Purpose: configure the external PLL output clock offset in case of freerun mode
 *
 * Inputs:
 *     ufeid - the UFE id
 *     pll_ppb_offset - control the output offset
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *    WUFEI_HwBsZarlinkPllConfig
 *
 ****************************************************************************/

void WUFEI_BoardWriteZarlinkPll1FreeRunFreqOffset(WP_U32 ufeid, WP_S32 pll_ppb_offset)
{

   WP_S32 max_value = WUFEI_ZLPLL_FREERUN_OFFSET_MAX_PPB;
   WP_S32 min_value = WUFEI_ZLPLL_FREERUN_OFFSET_MIN_PPB;
   WP_U8 reg_add0=0 , reg_val0=0 ;
   WP_U8 reg_add1=0 , reg_val1=0 ;
   WP_U8 reg_add2=0 , reg_val2=0 ;
   WP_U8 reg_add3=0 , reg_val3=0 ;

     /* Cut-off value */
   if(pll_ppb_offset > max_value)
   {
   	pll_ppb_offset = max_value;  /* max value */
   }

   if(pll_ppb_offset < -max_value)
   {
     	pll_ppb_offset = min_value;  /* min value */
   }

   reg_add0= 0x65;
   reg_val0= (WP_U8)pll_ppb_offset;
   reg_add1= 0x66;
   reg_val1= (WP_U8)(pll_ppb_offset >> 8);
   reg_add2= 0x67;
   reg_val2= (WP_U8)(pll_ppb_offset >> 16);
   reg_add3= 0x68;
   reg_val3= (WP_U8)(pll_ppb_offset >> 24);

   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x64, 0x01);  /* access page 1 */
   /* set values */
   WUFEI_BoardWriteZarlinkPllReg(ufeid,reg_add0,reg_val0);
   WUFEI_BoardWriteZarlinkPllReg(ufeid,reg_add1,reg_val1);
   WUFEI_BoardWriteZarlinkPllReg(ufeid,reg_add2,reg_val2);
   WUFEI_BoardWriteZarlinkPllReg(ufeid,reg_add3,reg_val3);
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x64, 0x00);  /* set access back to page 0 */
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwBsZarlinkPllConfig
 *
 * Purpose: Sets the zarlink PLL configuration
 *
 * Inputs:
 *     ufeid - the UFE id
 *     setup_cmd - mode in which to setup the pll.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *    WUFEI_BsFpgaInit
 *
 ****************************************************************************/
WUFE_status WUFEI_HwBsZarlinkPllConfig(WP_U32 ufeid, WP_U32 setup_cmd)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* ext_pll_ptr;
   WP_S32 clock_offset;

   ext_pll_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ext_pll.value;

   WUFEI_REG_SET(ext_pll_ptr, 0x8);
   WPL_Delay(1000);

//   Setting DPLL1 with ref0 input as source
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1f, 0x02); /*Mode select: Manual Freerun Mode. In this mode, automatic reference
                                                       switching is disabled and DPLL1 stays in the free-run mode.*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x51, 0x0f); /*eth_en =0, f_sel =0 for sonet/sdh*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x60, 0xa3);
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x65, 0x00); /*Set all refs to Auto_Frequency detect*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x66, 0x00); /*Set all refs to Auto_Frequency detect*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x61, 0x33); /*select diff0 and diff 1 as output 155 frequency*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x20, 0x00); /*Select ref0 input - In Automatic Normal Mode (see register 0x1F),
                                                       this register indicates the currently selected reference. In Manual
                                                       Normal Mode (see register 0x1F), this register is used to manually select
                                                       the active reference.*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x24, 0xf0); /*ref0 has the highest priority ref1 is disabled*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x25, 0xff); /*ref2 is disabled ref3 is disabled*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x26, 0xff); /*ref4 is disabled ref5 is disabled*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x27, 0xff); /*ref6 is disabled ref7 is disabled*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x16, 0x22); /*out of range limit selection: 100-130 (+/-ppm)*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1e, 0x04); /* Disable freerun offset BITS [3:2] must be set to 01*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1d, 0x00); /* filter BW is 0.1Hz */
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1f, 0x00); /*Manual Normal Mode. In this mode, automatic reference switching is disabled
                                                       and the selected reference is determined by the dpll1_refsel register (0x20).*/
   //Setting p0_clk0 to DPLL2 with ref1 input as source and 8Khz output
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x2a, 0x80); /*Enable DPLL2*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x2c, 0x02); /*Mode select: Manual Freerun Mode. In this mode, automatic reference
   													  switching is disabled and DPLL2 stays in the free-run mode.*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x36, 0xcf); /*selects DPLL2 as P0 Synthesizer source*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x2d, 0x01); /*Select ref1 input - In Automatic Normal Mode (see register 0x2C)
                                                       this register indicates the currently selected reference. In Manual
                                                       Normal Mode (see register 0x2C), this register is used to manually select
                                                       the active reference.*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x31, 0x0f); /*ref0 is disabled ref1 has the highest priority*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x32, 0xff); /*ref2 is disabled ref3 is disabled*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x33, 0xff); /*ref4 is disabled ref5 is disabled*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x34, 0xff); /*ref6 is disabled ref7 is disabled*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x17, 0x22); /*out of range limit selection: 100-130 (+/-ppm)*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x2c, 0x00); /*Manual Normal Mode. In this mode, automatic reference switching is disabled
                                                       and the selected reference is determined by the dpll2_refsel register (0x2d).*/
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x38, 0x01);
   WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x39, 0x00); /*Sets the p0_clk0 to output 8Khz frequency*/


   switch (setup_cmd)
   {
      case WUFE_PLL_CONFIG_APLL_FREERUN_NO_OFFSET:

         WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1e, 0x04); /* Disable freerun offset BITS [3:2] must be set to 01*/
         WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1f, 0x02); /* Set DPLL1 to manual freerun mode */
         break;

      case WUFE_PLL_CONFIG_APLL_FREERUN_WITH_OFFSET:
         WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1e, 0x06); /* enable freerun offset BITS [3:2] must be set to 01*/
         WUFEI_BoardWriteZarlinkPllReg(ufeid, 0x1f, 0x02); /* Set DPLL1 to manual freerun mode */

         clock_offset = 0x04FFFFFF; /* 0x04FFFFFF */

         WUFEI_BoardWriteZarlinkPll1FreeRunFreqOffset(ufeid,clock_offset);
         break;
      case WUFE_PLL_CONFIG_APLL_NORMAL:
      case WUFE_PLL_CONFIG_DEFAULT:
         break;
      default:
         WUFE_PRINT_ERR((wufei_log,"ERROR: Worng configuration for Zarlink PLL! %x\n", setup_cmd));
         return WUFE_ERR_EXT_PLL_WRONG_CONFIG;
   }

   WPL_Delay(1000);
   return WP_OK;
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

   /* set the indication to work with board support */
   use_board_support_commands = WUFE_BOARD_SUPPORT_ENABLE;

   /* FPGA RESET only when host programming is not in use */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaReset(ufeid));

   /*Configure external PLL*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwBsZarlinkPllConfig(ufeid, WUFE_PLL_CONFIG_APLL_NORMAL));

   /* Polling for the PLL status*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaPllStatusGet(ufeid));

   /* Turn on Leds*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFpgaLedSet(ufeid));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsUfeInterruptEnable
 *
 * Purpose: clear bit 0 of the Interrupt mask register to allow
 *          UFE interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: UFE interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeCoreHwInterruptEnable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsUfeInterruptEnable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg,value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value & ~WUFEI_BS_INTERRUPT_MASKU_M);
   return WUFE_OK;
}
/*****************************************************************************
 *
 * Function:  WUFEI_BsFramerInterruptEnable
 *
 * Purpose: clear bit 1 of the Interrupt mask register to allow
 *          FRAMER interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: FRAMER interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_EnableFramerInterrupt
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsFramerInterruptEnable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value & ~WUFEI_BS_INTERRUPT_MASKF_M);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsExtPllInterruptEnable
 *
 * Purpose: clear bit 2 of the Interrupt mask register to allow
 *          ext PLL interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: Ext PLL interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeExtPllHwInterruptEnable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsExtPllInterruptEnable(WP_U32 ufeid)
{
   WP_U32 physical_addr,bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value & ~WUFEI_BS_INTERRUPT_MASK_EXT_PLL_M);
return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsLockLostInterruptEnable
 *
 * Purpose: clear bit 3 of the Interrupt mask register to allow
 *          lock lost interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: lock lost interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeLockLostHwInterruptEnable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsLockLostInterruptEnable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value & ~WUFEI_BS_INTERRUPT_MASK_BS_PLL_M);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsSfpInterruptEnable
 *
 * Purpose: clear bit 4 of the Interrupt mask register to allow
 *          sfp interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: sfp interrupt is enabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeSfpHwInterruptEnable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsSfpInterruptEnable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value & ~WUFEI_BS_INTERRUPT_MASK_SFP_M);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsUfeInterruptDisable
 *
 * Purpose: set bit 0 of the Interrupt mask register to mask
 *          UFE interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: UFE interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeCoreHwInterruptDisable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsUfeInterruptDisable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value | WUFEI_BS_INTERRUPT_MASKU_M);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFramerInterruptDisable
 *
 * Purpose: set bit 1 of the Interrupt mask register to mask
 *          FRAMER interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: FRAMER interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeFramerHwInterruptDisable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsFramerInterruptDisable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value | WUFEI_BS_INTERRUPT_MASKF_M);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsExtPllInterruptDisable
 *
 * Purpose: set bit 2 of the Interrupt mask register to mask
 *          ext PLL interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: Ext PLL interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeExtPllHwInterruptDisable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsExtPllInterruptDisable(WP_U32 ufeid)
{
	WP_U32 physical_addr, bs_base;
    WP_U16* event_reg;
	WP_U16 value = 0;

	physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
	bs_base = physical_addr;
	event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

	WUFEI_REG_GET(event_reg, &value);
	WUFEI_REG_SET(event_reg, value | WUFEI_BS_INTERRUPT_MASK_EXT_PLL_M);
	return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_BsLockLostInterruptDisable
 *
 * Purpose: set bit 3 of the Interrupt mask register to mask
 *          lock lost interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: lock lost interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeLockLostHwInterruptDisable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsLockLostInterruptDisable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value | WUFEI_BS_INTERRUPT_MASK_BS_PLL_M);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeLockLostHwInterruptDisable
 *
 * Purpose: set bit 4 of the Interrupt mask register to mask
 *          sfp interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs: lock lost interrupt is disabled
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by: WUFEI_UfeSfpHwInterruptDisable
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsSfpInterruptDisable(WP_U32 ufeid)
{
   WP_U32 physical_addr, bs_base;
   WP_U16* event_reg;
   WP_U16 value = 0;

   physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   bs_base = physical_addr;
   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg, &value);
   WUFEI_REG_SET(event_reg, value | WUFEI_BS_INTERRUPT_MASK_SFP_M);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsGetUfeInterrupt
 *
 * Purpose: Read bit 0 of the Interrupt event register and check
 *          if there is a UFE interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: WP_TRUE if there was a UFE interrupt, else WP_FALSE
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsUfeInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WP_U32 physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U32 bs_base = physical_addr;
   WP_U16* event_reg;
   WP_U16 value = 0;

   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_reg.value;
   WUFEI_REG_GET(event_reg, &value);

   if(WUFEI_BS_INTERRUPT_INTU_V(value))
      *interrupt_result = WP_TRUE;
   else
      *interrupt_result = WP_FALSE;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsGetFramerInterrupt
 *
 * Purpose: Read bit 1 of the Interrupt event register and check
 *          if there is a FRAMER interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: WP_TRUE if there was a FRAMER interrupt, else WP_FALSE
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsFramerInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WP_U32 physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U32 bs_base = physical_addr;
   WP_U16* event_reg;
   WP_U16 value = 0;

   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_reg.value;
   WUFEI_REG_GET(event_reg, &value);

   if(WUFEI_BS_INTERRUPT_INTF_V(value))
      *interrupt_result = WP_TRUE;
   else
      *interrupt_result = WP_FALSE;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsGetExtPllInterrupt
 *
 * Purpose: Read bit 2 of the Interrupt event register and check
 *          if there is an ext PLL interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: WP_TRUE if there was an ext PLL interrupt, else WP_FALSE
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsUfeExtPllInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WP_U32 physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U32 bs_base = physical_addr;
   WP_U16* event_reg;
   WP_U16 value = 0;

   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_reg.value;
   WUFEI_REG_GET(event_reg, &value);

   if(WUFEI_BS_INTERRUPT_EXT_PLL_V(value))
       *interrupt_result = WP_TRUE;
   else
      *interrupt_result = WP_FALSE;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsGetLockLostInterrupt
 *
 * Purpose: Read bit 3 of the Interrupt event register and check
 *          if there is a lock lost interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: WP_TRUE if there was a lock lost interrupt, else WP_FALSE
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsUfeLockLostInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WP_U32 physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U32 bs_base = physical_addr;
   WP_U16* event_reg;
   WP_U16 value = 0;

   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_reg.value;
   WUFEI_REG_GET(event_reg, &value);

   if(WUFEI_BS_INTERRUPT_BS_PLL_V(value))
       *interrupt_result = WP_TRUE;
   else
      *interrupt_result = WP_FALSE;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsGetSfpInterrupt
 *
 * Purpose: Read bit 4 of the Interrupt event register and check
 *          if there is a sfp interrupt indication
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: WP_TRUE if there was a sfp interrupt, else WP_FALSE
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_BsUfeSfpInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WP_U32 physical_addr = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U32 bs_base = physical_addr;
   WP_U16* event_reg;
   WP_U16 value = 0;

   event_reg = &((WUFEI_hw_bs*)bs_base)->bs_int_reg.value;
   WUFEI_REG_GET(event_reg, &value);

   if(WUFEI_BS_INTERRUPT_SFP_V(value))
       *interrupt_result = WP_TRUE;
   else
      *interrupt_result = WP_FALSE;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsEventRegSet
 *
 * Purpose: Setter for the event register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to set in the register
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsEventRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_int_reg.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsEventRegGet
 *
 * Purpose: Getter for the event register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the recieved value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsEventRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_int_reg.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsEventMaskRegSet
 *
 * Purpose: Setter for the event mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to set in the register
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsEventMaskRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsEventMaskRegGet
 *
 * Purpose: Getter for the event mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the recieved value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsEventMaskRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_int_mask.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsLockLostRegSet
 *
 * Purpose: Setter for the lock lost register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsLockLostRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_lock_lost_indication.value;

   WUFEI_REG_SET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsLockLostRegGet
 *
 * Purpose: Getter for the lock lost register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsLockLostRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_lock_lost_indication.value;

   WUFEI_REG_GET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsLockLostMaskRegSet
 *
 * Purpose: Setter for the lock lost mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsLockLostMaskRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_lock_lost_indication_mask.value;

   WUFEI_REG_SET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsLockLostMaskRegGet
 *
 * Purpose: Getter for the lock lost mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsLockLostMaskRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_lock_lost_indication_mask.value;

   WUFEI_REG_GET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpAbsentRegSet
 *
 * Purpose: Setter for the sfp absent register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpAbsentRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_absent.value;

   WUFEI_REG_SET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpAbsentRegGet
 *
 * Purpose: Getter for the sfp absent register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpAbsentRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_absent.value;

   WUFEI_REG_GET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpAbsentMaskRegSet
 *
 * Purpose: Setter for the sfp absent mask mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpAbsentMaskRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_absent_mask.value;

   WUFEI_REG_SET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpAbsentMaskRegGet
 *
 * Purpose: Getter for the sfp absent mask mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpAbsentMaskRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_absent_mask.value;

   WUFEI_REG_GET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpLosRegSet
 *
 * Purpose: Setter for the sfp los register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpLosRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_los.value;

   WUFEI_REG_SET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpLosRegGet
 *
 * Purpose: Getter for the sfp los register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpLosRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_los.value;

   WUFEI_REG_GET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpLosMaskRegSet
 *
 * Purpose: Setter for the sfp los mask mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpLosMaskRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_los_mask.value;

   WUFEI_REG_SET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwBsSfpLosMaskRegGet
 *
 * Purpose: Getter for the sfp los mask mask register of the board support
 *
 * Inputs:
 *     ufeid - the UFE id
 *     value - value to store the register value in.
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwBsSfpLosMaskRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* reg_ptr;

   reg_ptr = &((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_los_mask.value;

   WUFEI_REG_GET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_BoardWriteZarlinkPllReg
 *
 * Purpose: Sets the zarlink PLL registers
 *
 * Inputs:
 *     ufeid - the UFE id
 *     address - the address of the register
 *     data    - the data to write in the register
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *    WUFEI_HwBsZarlinkPllConfig
 *
 ****************************************************************************/
static void WUFEI_BoardWriteZarlinkPllReg(WP_U32 ufeid, WP_U32 address, WP_U32 data)
{
   WP_U8 current_bit;
   WP_S32 ii;
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* ext_pll_ptr;

   ext_pll_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ext_pll.value;

   WUFEI_REG_SET(ext_pll_ptr, 0x8);
   WPL_Delay(WUFEI_ZARLINK_DELAY);

   WUFEI_REG_SET(ext_pll_ptr, 0xa);

   WPL_Delay(WUFEI_ZARLINK_DELAY);
   for(ii = WUFEI_ZARLINK_PLL_LAST_ADDRESS_BIT; ii >= 0; ii-- )
   {
      current_bit= 0x01 & (address>>ii);
      WUFEI_REG_SET(ext_pll_ptr, current_bit|0x8);
      WUFEI_REG_SET(ext_pll_ptr, current_bit|0xa);
   }

   WPL_Delay(WUFEI_ZARLINK_DELAY);
   for(ii = WUFEI_ZARLINK_PLL_LAST_DATA_BIT; ii >= 0; ii-- )
   {
      current_bit= 0x01 & (data>>ii);
      WUFEI_REG_SET(ext_pll_ptr, current_bit|0x8);
      WUFEI_REG_SET(ext_pll_ptr, current_bit|0xa);
   }
   WPL_Delay(WUFEI_ZARLINK_DELAY);
   WUFEI_REG_SET(ext_pll_ptr, 0xc);
   WPL_Delay(WUFEI_ZARLINK_DELAY);
}

/*****************************************************************************
 *
 * Function:  WUFEI_BoardReadZarlinkPllReg
 *
 * Purpose: Reads zarlink PLL register
 *
 * Inputs:
 *     ufeid - the UFE id
 *     address - the address of the register
 *     data    - the data read from the register
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *    WUFEI_HwBsZarlinkPllConfig
 *
 ****************************************************************************/
static void WUFEI_BoardReadZarlinkPllReg(WP_U32 ufeid, WP_U32 address, WP_U32 *data)
{
   WP_U8 current_bit;
   WP_U16 value;
   WP_S32 ii;
   WP_U32 bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   WP_U16* ext_pll_ptr, *ext_pll_rd_ptr;

   *data = 0;

   ext_pll_ptr = &((WUFEI_hw_bs*)bs_base)->bs_ext_pll.value;
   ext_pll_rd_ptr = &((WUFEI_hw_bs*)bs_base)->bs_hif_zl_rd.value;

   WUFEI_REG_SET(ext_pll_ptr, 0x9);
   WPL_Delay(WUFEI_ZARLINK_DELAY);

   WUFEI_REG_SET(ext_pll_ptr, 0xb);
   WPL_Delay(WUFEI_ZARLINK_DELAY);

   /*
    * Sends the address to the Zarlink PLL bit by bit.
    */
   for(ii = WUFEI_ZARLINK_PLL_LAST_ADDRESS_BIT; ii >= 0; ii-- )
   {
      current_bit= 0x01 & (address>>ii);
      WUFEI_REG_SET(ext_pll_ptr, current_bit|0x8);
      WUFEI_REG_SET(ext_pll_ptr, current_bit|0xa);
   }

   /*
    * Reads the data from the Zarlink PLL bit by bit.
    */
   WPL_Delay(WUFEI_ZARLINK_DELAY);
   for(ii = WUFEI_ZARLINK_PLL_LAST_DATA_BIT; ii >= 0; ii-- )
   {
      WUFEI_REG_SET(ext_pll_ptr, 0x8);
      WUFEI_REG_SET(ext_pll_ptr, 0xa);
      WUFEI_REG_GET(ext_pll_rd_ptr, &value);
      *data = *data | ((value & 0x01)<<ii);
   }
   WPL_Delay(WUFEI_ZARLINK_DELAY);
   WUFEI_REG_SET(ext_pll_ptr, 0x8);
   WPL_Delay(WUFEI_ZARLINK_DELAY);
   WUFEI_REG_SET(ext_pll_ptr, 0xc);
   WPL_Delay(WUFEI_ZARLINK_DELAY);
}

/*****************************************************************************
 *
 * Function:  WUFE_BSReadExtPllReg
 *
 * Purpose: Reads zarlink PLL register
 *
 * Inputs:
 *     address - the address of the register
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/
WP_U32 WUFE_BSReadExtPllReg(WP_U32 address)
{
   WP_U32 data = 0;

   WUFEI_BoardReadZarlinkPllReg(0, address, &data);

   return data;
}

/*****************************************************************************
 *
 * Function:  WUFE_BSSetExtPllFreeRunMode
 *
 * Purpose: Sets zarlink PLL to manual free-run mode
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/
void WUFE_BSSetExtPllFreeRunMode(WP_U32 ufe_id)
{
   WUFEI_BoardWriteZarlinkPllReg(ufe_id, 0x1f, 0x02);
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeExternalPllClockOutConfig
 *
 * Purpose: Set zarlink PLL APLL, p0_clk0, p1_clk0
 *
 * Inputs:
 *     ufeid - the UFE id
 *     address - the address of the register
 *     data    - the data read from the register
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 *
 ****************************************************************************/

WUFE_status WUFE_BsUfeExternalPllClockOutConfig(WUFEI_pll_cfg *pllConfigParam)
{
   WP_U32 upperByte,lowerByte;
   WP_U32 rate = 0, apll_cfg = 0;

   /* p0_clk0 or p1_clk0 */
   if(pllConfigParam->clock_out != apll_clk0)
   {
      if(pllConfigParam->clock_rate % 8)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR - Clock rate is not a multiplication of 8KHz\n"));
         return WUFE_ERR_INVALID_PLL_CLOCK_RATE;
      }
      rate = pllConfigParam->clock_rate / 8;

      lowerByte = (rate & 0xFF);
      upperByte = ((rate >> 8) & 0xFF);

      if(pllConfigParam->clock_out == p1_clk0)
      {
         WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid, 0x4a, lowerByte);
         WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid, 0x4b, upperByte);
      }
      else
      {
         WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid, 0x38, lowerByte);
         WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid, 0x39, upperByte);
      }
   }
   else /* apll_clk0 */
   {
      WUFEI_BoardReadZarlinkPllReg(pllConfigParam->ufeid,0x51,&apll_cfg);

      /* sets bits 4 and 6 to 0
         0 - the default, for SONET/SDH frequencies
         1 - set to 1 for Ethernet frequencies */
      apll_cfg = apll_cfg & 0xaf;

      switch(pllConfigParam->apll_rate)
      {
         case f_125:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg | 0x50));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,1);
            break;

         case f_62_5:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg | 0x50));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,2);
            break;

         case f_77_76:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,2);
            break;

         case f_38_88:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,3);
            break;

         case f_19_44:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,4);
            break;

         case f_50:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg | 0x50));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,5);
            break;

         case f_9_72:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,5);
            break;

         case f_25:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg | 0x50));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,6);
            break;

         case f_12_5:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg | 0x50));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,7);
            break;

         case f_51_84:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,10);
            break;

         case f_25_92:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,11);
            break;

         case f_12_96:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,12);
            break;

         case f_6_48:
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x51,(apll_cfg));
            WUFEI_BoardWriteZarlinkPllReg(pllConfigParam->ufeid,0x52,13);
            break;
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaGpioClockSelect
 *
 * Purpose: Sets the GPIO output clock
 *
 * Description: This function sets the relevant GPIO output clock (0 or 1)
 *              to the value accepted in gpio_clock The function updates only one section
 *              each time - GPIO_0 or GPIO_1.
 *
 * Inputs:
 *     ufeid       - The UFE ID
 *     gpio_select - The GPIO output number. Can be '0' or '1'
 *     gpio_clock  - The clock to set as output from the GPIO.
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_UfeGpioOutputClockSet, WUFEI_UfeGpioClockDefaultSetup
 *
 ****************************************************************************/
WUFE_status WUFEI_BsFpgaGpioClockSelect(WP_U32 ufeid, WP_U32 gpio_select, WP_U16 gpio_clock)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *gpio_ptr, value, gpio_mask;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->bs_clk_out_sel;
   gpio_ptr = &(bs_ptr->value);

   gpio_mask = gpio_select ? 0x00ff : 0xff00;

   /* read-modify-write the GPIO clock select mux register:
      read the register, clear relevant area and write the new data. */
   WUFEI_REG_GET(gpio_ptr, &value);
   value &= gpio_mask;
   value |= gpio_clock;
   WUFEI_REG_SET(gpio_ptr, value);

   return WUFE_OK;
}

WUFE_status WUFEI_BsFpgaGpioClockGet(WP_U32 ufeid, WP_U16 *gpio_register)
{
   WP_U32 bs_base;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *gpio_ptr, value;

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->bs_clk_out_sel;
   gpio_ptr = &(bs_ptr->value);

   /* read the GPIO register */
   WUFEI_REG_GET(gpio_ptr, &value);
   *gpio_register = value;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_BsFpgaPllClockStatusGet
 *
 * Purpose: Checks PLL status for XO and DIFF (and external) lock
 *
 * Description: This function verify that the XO and DIFF PLLs are locked in the board support.
 *
 *
 * Inputs:
 *     ufeid       - The UFE ID
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_SystemConfig
 *
 ****************************************************************************/
WUFE_status WUFEI_BsFpgaPllClockStatusGet(WP_U32 ufeid)
{
   WP_U32 bs_base, timeout = 0;
   WUFEI_reg_space* bs_ptr;
   WP_U16 *pll_stat_ptr, value;
   WP_boolean XO_pll_used = WUFE_TRUE;
   WUFEI_system_registry *system_registry;

   system_registry = WUFEI_SystemRegistryGet(ufeid);
   if(!(system_registry))
   {
	   return WUFE_ERR_INVALID_REGISTRY_STATE;
   }

   if(WP_DISABLE == system_registry->cfg.clk_rec_system_params->xo_ref_clk)
   {
	   XO_pll_used = WUFE_FALSE;
   }

   WUFEI_UFE_UNLOCK(ufeid);

   if (use_board_support_commands == WUFE_BOARD_SUPPORT_DISABLE)
      return WUFE_OK;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);

   bs_ptr = &((WUFEI_hw_bs*)bs_base)->bs_lock_indication;
   pll_stat_ptr = &(bs_ptr->value);

   do{
      WUFEI_REG_GET(pll_stat_ptr, &value);
      WPL_Delay(WUFEI_PLL_LOCK_DELAY);
      timeout ++;

      if(timeout > WUFEI_PLL_LOCK_TIMEOUT)
      {
         if (!WUFEI_BS_LOCK_IND_ADAP_V(value) && XO_pll_used)
            WUFE_PRINT_ERR((wufei_log, "BS: XO PLL not locked\n"));
         else if (!WUFEI_BS_LOCK_IND_DIFF_V(value))
            WUFE_PRINT_ERR((wufei_log, "BS: DIFF PLL not locked\n"));
/*         else if (!WUFEI_BS_LOCK_IND_EXT_PLL_V(value))
           WUFE_PRINT_ERR((wufei_log, "BS: External PLL not locked\n")); */
         return WUFE_ERR_HW_PLL_LOCK;
      }
   }
   while(!WUFEI_BS_LOCK_IND_DIFF_V(value)  ||
         (!WUFEI_BS_LOCK_IND_ADAP_V(value) && (WUFE_FALSE != XO_pll_used)) /* ||
         !WUFEI_BS_LOCK_IND_EXT_PLL_V(value) */
      );

   return WUFE_OK;
}

WP_S32 set_ack (struct sfp_cmd *p_cmd)
{
   WP_S32 ret = OK;

   p_cmd->data = LOW;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return ERR_SCL_ACK;
   p_cmd->data = LOW;
   ret = set_sda(p_cmd);
   if (ret != OK)
      return ERR_SDA_ACK;
   p_cmd->data = HIGH;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return ERR_SCL_ACK;
   p_cmd->data = LOW;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return ERR_SCL_ACK;
   p_cmd->data = HIGH;
   ret = set_sda(p_cmd);
   if (ret != OK)
      return ERR_SDA_ACK;
   return (ret);
}

WP_S32 set_scl(struct sfp_cmd *p_cmd)
{
   WP_U16 temp, regs_local_copy;
   WP_U8 level = p_cmd->data;
   WP_U8 sfp_id = p_cmd->present_list[0];

   if (wait_busy() != OK) {
      WUFE_PRINT_ERR((wufei_log, "ERROR - WAIT BUSY - SCL\n"));
      return (ERR_SCL_BUSY);
   }

   if (level == HIGH)
   {
	  if (p_cmd->present_sfp == 1)
      {

	   	 WUFEI_REG_GET(&regs->scl, &regs_local_copy);
		 regs_local_copy |= (1 << sfp_id);
         WUFEI_REG_SET(&regs->scl, regs_local_copy);
      }
      else
      {
    	 WUFEI_REG_GET(&regs->scl, &regs_local_copy);
         regs_local_copy |= 0xffff;
         WUFEI_REG_SET(&regs->scl, regs_local_copy);
      }
   }
   else
   {
	  if (p_cmd->present_sfp == 1)
      {
		 temp = (1 << sfp_id);
		 WUFEI_REG_GET(&regs->scl, &regs_local_copy);
		 regs_local_copy &= (~temp);
         WUFEI_REG_SET(&regs->scl, regs_local_copy);
      }
      else
         WUFEI_REG_SET(&regs->scl, 0);

   }
   return (OK);
}

WP_S32 set_sda(struct sfp_cmd *p_cmd)
{
   WP_U8 level = p_cmd->data;
   WP_U8 sfp_id = p_cmd->present_list[0];
   WP_U16 temp, regs_local_copy;

   if (wait_busy() != OK) {
      WUFE_PRINT_ERR((wufei_log, "ERROR - WAIT BUSY - SDA\n"));
      return (ERR_SDA_BUSY);
   }

   if (level == HIGH)
   {
      if (p_cmd->present_sfp == 1)
      {
    	  WUFEI_REG_GET(&regs->sda_wr, &regs_local_copy);
    	  regs_local_copy |= (1 << sfp_id);
    	  WUFEI_REG_SET(&regs->sda_wr, regs_local_copy);
      }
      else
      {
    	  WUFEI_REG_GET(&regs->sda_wr, &regs_local_copy);
    	  regs_local_copy |= 0xffff;
    	  WUFEI_REG_SET(&regs->sda_wr, regs_local_copy);
      }
   }
   else
   {
      if (p_cmd->present_sfp == 1)
      {
    	  temp = (1 << sfp_id);
    	  WUFEI_REG_GET(&regs->sda_wr, &regs_local_copy);
    	  regs_local_copy &= (~temp);
    	  WUFEI_REG_SET(&regs->sda_wr, regs_local_copy);
      }
      else
    	  WUFEI_REG_SET(&regs->sda_wr, 0);
   }
   return (OK);
}


WP_U8 get_data(WP_U8 sfp_id)
{
   WP_U16 regs_local_copy;

   if (wait_busy() != OK)	{
    	WUFE_PRINT_ERR((wufei_log, "ERROR - WAIT BUSY\n"));
		return -1;
   }

   WUFEI_REG_GET(&regs->sda_rd, &regs_local_copy);
   return ((regs_local_copy & (1 << sfp_id))!=0);
}

WP_S32 send_byte (struct sfp_cmd *p_cmd)
{
   WP_S32 bit, ret;
   WP_U8 Thebyte = p_cmd->data;

   for (bit=7;bit>=0;bit--)
   {
      if (Thebyte & (1 << bit))
      {
         p_cmd->data = HIGH;
         ret = set_sda (p_cmd);
         if (ret != OK)
            return ERR_SDA_SEND;
      }
      else
      {
         p_cmd->data = LOW;
         ret = set_sda (p_cmd);
         if (ret != OK)
            return ERR_SDA_SEND;
      }

      p_cmd->data = HIGH;
      ret = set_scl (p_cmd);
      if (ret != OK)
         return ERR_SCL_SEND;

      p_cmd->data = LOW;
      ret = set_scl (p_cmd);
      if (ret != OK)
         return ERR_SCL_SEND;
   }

   return (OK);
}

WP_S32 wait_for_ack (struct sfp_cmd *p_cmd)
{
   WP_S32 timeout = 100000, i;
   WP_S32 ret;

   p_cmd->data = HIGH;
   ret = set_sda(p_cmd);
   if (ret != OK)
      return ERR_SDA_ACK;

   p_cmd->data = HIGH;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return ERR_SCL_ACK;

   for (i=0;i<p_cmd->present_sfp;i++)
   {
      while ( get_data(p_cmd->present_list[i]) && --timeout)
         WPL_Delay(1);

      if (!timeout)
      {
         WUFE_PRINT_ERR((wufei_log, "Timeout error in wait_for_ack\n"));
         return (ERR_ACK_TIMEOUT);
      }
      timeout = 100000;
   }

   p_cmd->data = LOW;
   ret = set_scl (p_cmd);
   if (ret != OK)
      return ERR_SCL_ACK;

   return (timeout == 0);
}

WP_S32 read_data(WP_S32 pos, struct sfp_cmd *p_cmd, WP_U8 *buff)
{
   WP_S32 bit, i, ret, idx;

   for (i = 0; i < p_cmd->present_sfp;i++)
   {
      idx = p_cmd->present_list[i] * MAX_BYTES;
      buff[idx + pos] = 0;
   }

   for (bit=7; bit>=0; bit--)
   {
      p_cmd->data = HIGH;
      ret = set_scl (p_cmd);
      if (ret != OK)
         return ERR_SCL_READ;

      for (i = 0; i < p_cmd->present_sfp;i++)
      {
         buff[p_cmd->present_list[i]*MAX_BYTES + pos] |= (get_data(p_cmd->present_list[i])<<bit);
      }

      p_cmd->data = LOW;
      ret = set_scl (p_cmd);
      if (ret != OK)
         return ERR_SCL_READ;
   }

   return OK;
}

WP_S32 wait_busy(void)
{
   WP_S32 timeout = 10000;
   WP_U16 regs_local_copy;

   WUFEI_REG_GET(&regs->busy, &regs_local_copy);
   while (regs_local_copy & 1 && --timeout)
   {
      WPL_Delay(1);
      WUFEI_REG_GET(&regs->busy, &regs_local_copy);
   }

   if (!timeout) {
      WUFE_PRINT_ERR((wufei_log, "ERROR - time out in wait busy\n"));
      return ERROR;
   }

   return (OK);
}

WP_S32 start_sequence(struct sfp_cmd *p_cmd)
{
   WP_S32 ret;

   p_cmd->data = LOW;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return (ERR_START_SEQ);
   p_cmd->data = HIGH;
   ret = set_sda(p_cmd);
   if (ret != OK)
      return (ERR_START_SEQ);
   p_cmd->data = HIGH;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return (ERR_START_SEQ);
   p_cmd->data = LOW;
   ret = set_sda(p_cmd);
   if (ret != OK)
      return (ERR_START_SEQ);
   p_cmd->data = LOW;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return (ERR_START_SEQ);

   return OK;
}

WP_S32 stop_sequence(struct sfp_cmd *p_cmd)
{
   WP_S32 ret = OK;

   p_cmd->data = LOW;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return (ERR_STOP_SEQ);
   p_cmd->data = LOW;
   ret = set_sda(p_cmd);
   if (ret != OK)
      return (ERR_STOP_SEQ);
   p_cmd->data = HIGH;
   ret = set_scl(p_cmd);
   if (ret != OK)
      return (ERR_STOP_SEQ);
   p_cmd->data = HIGH;
   ret = set_sda(p_cmd);
   if (ret != OK)
      return (ERR_STOP_SEQ);

   return OK;
}


WP_S32 no_ack (struct sfp_cmd *p_cmd)
{
   WP_S32 ret;

   p_cmd->data = HIGH;
   ret = set_sda (p_cmd);
   if (ret != OK)
      return ERR_SDA_NOACK;
   p_cmd->data = HIGH;
   ret = set_scl (p_cmd);
   if (ret != OK)
      return ERR_SCL_NOACK;
   p_cmd->data = LOW;
   ret = set_scl (p_cmd);
   if (ret != OK)
      return ERR_SCL_NOACK;

   return OK;
}

WP_S32 get_present_devices(struct sfp_cmd *p_cmd)
{
   WP_S32 i;
   WP_U16 regs_local_copy;

   p_cmd->present_sfp = 0;
   for (i=0;i<MAX_SFP;i++) {
	  WUFEI_REG_GET(&regs->absent, &regs_local_copy);
      if ((regs_local_copy & (1 << i)) == 0) {
         p_cmd->present_list[p_cmd->present_sfp] = i;
         p_cmd->present_sfp++;
      }
   }

   if (!p_cmd->present_sfp) {
      WUFE_PRINT_ERR((wufei_log, "No indentified device \n"));
      return (ERR_DEVICE_DETECT);
   }
   return OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_SfpRead
 *
 * Purpose: Read data from SFP EEPROM
 *
 * Description: This function accesses external UFE SFP devices by emulating
 *              the I2C protocol and reads data from the SFP EEPROM
 *
 * Inputs:
 *     ufeid - The UFE ID
 *     sfp_id - SFP module index, valid values are: UFE412 (0-7), UFE448 (0-15)
 *     n_bytes - number of bytes to read (1-16)
 *     single_sfp_read - flag that indicates read from a single sfp module. If set,
 *                       then sfp_id must be provided. Otherwise, all available SFPs
 *                       are accessed and read bytes are appended in rom_value.
 *     device_addr - EEPROM address
 *     rom_addr - offset within EEPROM to be read
 *     rom_value - pointer to user location where read value is written to.
 *
 * Outputs:
 *     Value is read from EEPROM to user application
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WUFE_status WUFE_SfpRead (WP_U32 ufeid,
                          WP_U8 sfp_id,
                          WP_U8 n_bytes,
                          WP_U8 single_sfp_read,
                          WP_U8 device_addr,
                          WP_U8 rom_addr,
                          WP_U8 *rom_value)
{
   WP_U32 bs_base, sfp_base;
   WP_S32 ret;
   WP_S32 i;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   sfp_base = (WP_U32) &(((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_scl);
   regs = (volatile struct i2c_regs *) WPL_PHY_VIRTUAL(0, sfp_base);

   if (single_sfp_read)
   {
      cmd.present_sfp = 1;
      cmd.present_list[0] = sfp_id;
   }
   else
   {
      ret = get_present_devices(&cmd);
      if (ret)
      {
         WUFE_PRINT_ERR((wufei_log, "ERR_DEVICE_DETECT - Devices detection\n"));
         return (ret);
      }
   }

   /* Dummy stuff */
   ret = start_sequence(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - start_sequence\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   cmd.data = device_addr -1;
   ret = send_byte (&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - send_dev_address\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   ret = wait_for_ack(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - wait_for_ack\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   cmd.data = rom_addr;
   ret = send_byte (&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - send_word_address\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   ret = wait_for_ack(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - wait_for_ack\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }
   /* End of dummy */
   ret = start_sequence(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - start_sequence\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   cmd.data = device_addr ;
   ret = send_byte (&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - send_dev_address\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   ret = wait_for_ack(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - wait_for_ack\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   for(i=0;i<n_bytes;i++) {
      ret = read_data(i,&cmd, rom_value);
      if (ret != OK)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR - read_data\n"));
         return WUFE_SFP_ACCESS_ERROR;
      }

      if (i < (n_bytes - 1))
         ret = set_ack(&cmd);
      else
         ret = no_ack(&cmd);

      if (ret != OK)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR - read_data\n"));
         return WUFE_SFP_ACCESS_ERROR;
      }
   }

   ret = no_ack(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - in no ACK\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   ret = stop_sequence(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - stop_sequence\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_SfpWrite
 *
 * Purpose: Write data to SFP EEPROM
 *
 * Description: This function accesses external UFE SFP devices by emulating
 *              the I2C protocol and writes data to the SFP EEPROM
 *
 * Inputs:
 *     ufeid - The UFE ID
 *     sfp_id - SFP module index, valid values are: UFE412 (0-7), UFE448 (0-15)
 *     n_bytes - number of bytes to write (1-16)
 *     device_addr - EEPROM address
 *     rom_addr - offset within EEPROM to be written
 *     rom_value - pointer to value to be written to EEPROM
 *
 * Outputs:
 *     value is wriiten to EEPROM.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK is returned.
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WUFE_status WUFE_SfpWrite (WP_U32 ufeid, WP_U8 sfp_id, WP_U8 n_bytes, WP_U8 device_addr, WP_U8 rom_addr, WP_U8 *rom_value)
{
   WP_U32 bs_base, sfp_base;
   WP_S32 ret, i;

   bs_base = WUFEI_UfeBsBaseAddrGet(ufeid);
   sfp_base = (WP_U32) &(((WUFEI_hw_bs*)bs_base)->bs_sfp_fo_scl);
   regs = (volatile struct i2c_regs *) WPL_PHY_VIRTUAL(0, sfp_base);

   cmd.present_sfp = 1;
   cmd.sfp_id = sfp_id;

   ret = start_sequence(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - start_sequence\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   cmd.data = device_addr;
   ret = send_byte (&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - send_dev_address\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   ret = wait_for_ack(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - wait_for_ack\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   cmd.data = rom_addr;
   ret = send_byte (&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - send_dev_address\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   ret = wait_for_ack(&cmd);
   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - wait_for_ack\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   for (i=0;i<n_bytes;i++) {
      cmd.data = rom_value[i];
      ret = send_byte (&cmd);
      if (ret != OK)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR - send_dev_address\n"));
         return WUFE_SFP_ACCESS_ERROR;
      }

      ret = wait_for_ack(&cmd);
      if (ret != OK)
      {
         WUFE_PRINT_ERR((wufei_log, "ERROR - wait_for_ack\n"));
         return WUFE_SFP_ACCESS_ERROR;
      }
   }

   ret = stop_sequence(&cmd);

   if (ret != OK)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR - stop_sequence\n"));
      return WUFE_SFP_ACCESS_ERROR;
   }

   WPL_Delay(10000); /* Twr_max=10ms, waiting for WWPROM internal data written. */

   return WUFE_OK;
}

