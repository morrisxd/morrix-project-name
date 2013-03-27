/*****************************************************************************
 * (C) Copyright 2000-2010, Wintegra. All rights reserved.
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

#ifndef WUFEI_LINE_H
#include "wufei_line.h"
#endif

#ifndef WUFEI_HW_TDM_H
#include "wufei_hw_tdm.h"
#endif

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif

#ifndef WUFEI_LINE_INT_H
#include "wufei_line_int.h"
#endif

#ifndef WUFEI_BOARD_SUPPORT_INT_H
#include "wufei_board_support_int.h"
#endif

#ifndef WUFEI_BOARD_SUPPORT_H
#include "wufei_board_support.h"
#endif

#ifndef WUFEI_HW_GLOBALS_INT_H
#include "wufei_hw_globals_int.h"
#endif

#ifndef WUFEI_HW_GLOBALS_H
#include "wufei_hw_globals.h"
#endif

static void WUFEI_HwTdmIdirectAddrSet(WP_U32 ufeid, WP_U32 coreid, WP_U32 indirect_addr);
static WUFE_status WUFEI_HwTdmIdirectDataSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value);
static WUFE_status WUFEI_HwTdmIdirectDataGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value);
static WUFE_status WUFEI_HwTdmIdirectMemset(WP_U32 ufeid, WP_U32 coreid, WP_U32 addr, WP_U16 value, WP_U32 size);

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmIdirectAddrSet
 *
 * Purpose: Indirect address set for the TDM
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
 *     write to TDM indirect address register
 *
 * Return Value: none
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwTdmIdirectAddrSet(WP_U32 ufeid, WP_U32 coreid, WP_U32 indirect_addr)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_low.value;
   indha_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_high.value;

   WUFEI_REG_SET(indla_ptr, indirect_addr & 0xffff);
   WUFEI_REG_SET(indha_ptr, (indirect_addr >> 16) & 0xffff);

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmIdirectDataSet
 *
 * Purpose: Indirect data set for the TDM
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
 *     write to TDM indirect data register
 *
 * Return Value: None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwTdmIdirectDataSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_cmd.value;

   WUFEI_REG_SET(inddat_ptr, value);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;

         WUFE_PRINT_ERR((wufei_log,"ERROR: Timeout while trying to set data %x in TDM indirect register\n", value));
         ind_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", value));
         ind_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_high.value;
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
 * Function:  WUFEI_HwTdmIdirectDataGet
 *
 * Purpose: Indirect data get for the TDM
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
 *     get the TDM indirect data
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwTdmIdirectDataGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_cmd.value;

   WUFEI_REG_SET(indcmd_ptr, WUFEI_INDIRECT_RD_CMD);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;
         WP_U16 tmp_value;

         WUFE_PRINT_ERR((wufei_log,"ERROR: Timeout while trying to get data from TDM indirect register\n"));
         ind_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&tmp_value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", tmp_value));
         ind_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_ind_high.value;
         WUFEI_REG_GET(ind_ptr ,&tmp_value);
         WUFE_PRINT_ERR((wufei_log,"Indirect high address was %x\n", tmp_value));

         return WUFE_ERR_INDIRECT_DATA_GET_TIMEOUT;
      }
      WPL_Delay(10);
      timeout--;
      WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   }
   WUFEI_REG_GET(inddat_ptr, value);
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmIdirectMemset
 *
 * Purpose: Continuos Indirect memory set for the TDM
 *
 *
 * Inputs:
 *      ufeid - the UFE id
 *      addr - start address
 *      value - value to set
 *      size - memset size
 *
 * Outputs:
 *     write to TDM indirect registers
 *
 * Return Value: None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwTdmIdirectMemset(WP_U32 ufeid, WP_U32 coreid, WP_U32 addr, WP_U16 value, WP_U32 size)
{
   WP_U32 i;
   WUFE_status status;

   WUFEI_HwTdmIdirectAddrSet(ufeid, coreid, addr);
   for(i = 0; i<size; i++)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmIdirectDataSet(ufeid, coreid, value));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmEnable
 *
 * Purpose: Enables the TDM
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     direction           direction to enable
 *
 * Outputs:
 *     write to TDM mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_SystemTdmEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmEnable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 direction)
{
   WP_U16 mode_value, *mode_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);

   mode_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_mode.value;
   WUFEI_REG_GET(mode_ptr, &mode_value);

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      mode_value |= WUFEI_TDM_MODE_RX_EN_M;
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      mode_value |= WUFEI_TDM_MODE_TX_EN_M;
   }

   WUFEI_REG_SET(mode_ptr, mode_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmDisable
 *
 * Purpose: Disables the TDM
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     direction           direction to disable
 *
 * Outputs:
 *     write to TDM mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_SystemTdmDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmDisable(WUFEI_system_registry* system_registry, WP_U32 coreid,WP_U32 direction)
{
   WP_U16 mode_value, *mode_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);

   mode_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_mode.value;
   WUFEI_REG_GET(mode_ptr, &mode_value);

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      mode_value &= ~WUFEI_TDM_MODE_RX_EN_M;
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      mode_value &= ~WUFEI_TDM_MODE_TX_EN_M;
   }
   WUFEI_REG_SET(mode_ptr, mode_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmIdleCasSet
 *
 * Purpose: Sets the TDM idle cas and data
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to TDM Idle register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    WUFE_SystemCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmIdleCasSet(WUFEI_system_registry* system_registry, WP_U8 idle_data, WP_U8 idle_cas, WP_U32 coreid)
{
   WP_U16 idle_value, *idle_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);

   idle_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_idle.value;
   WUFEI_REG_GET(idle_ptr, &idle_value);

   idle_value = (WUFEI_TDM_IDLE_DATA_PATTERN_F(idle_data) |
                  WUFEI_TDM_IDLE_CAS_PATTERN_F(idle_cas));

   WUFEI_REG_SET(idle_ptr, idle_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmLoopbackSet
 *
 * Purpose: Sets the TDM loopback
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to TDM loopback register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    WUFE_LineLoopbackConfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmLoopbackSet(WUFEI_system_registry* system_registry,
                                   WP_U8 enable,
                                   WP_U8 framed,
                                   WP_U8 cas_enable,
                                   WP_U8 lpbk_type,
                                   WP_U32 coreid)
{
   WP_U16 lb_value, *lb_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);

   lb_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_sys_lb.value;

   if (enable)
   {
      lb_value = (WUFEI_TDM_SYSTEM_LB_EN_M |
                  WUFEI_TDM_SYSTEM_LB_UNFRM_F(!framed));

      if (lpbk_type == WUFEI_TDM_LOOPBACK_E1)
         lb_value |= WUFEI_TDM_SYSTEM_LB_E1_T1_M;
      if (lpbk_type == WUFEI_TDM_LOOPBACK_OC3)
         lb_value |= WUFEI_TDM_SYSTEM_LB_OC3_M;
      if (lpbk_type == WUFEI_TDM_LOOPBACK_OC12)
         lb_value |= WUFEI_TDM_SYSTEM_LB_OC12_M;
      if (lpbk_type == WUFEI_TDM_LOOPBACK_VC3)
         lb_value |= WUFEI_TDM_SYSTEM_LB_VC3_M;

      if (cas_enable == WUFE_CAS_ENABLE)
         lb_value |= WUFEI_TDM_SYSTEM_LB_CAS_M;
   }
   else
      lb_value = 0;

   WUFEI_REG_SET(lb_ptr, lb_value);

   return WUFE_OK;
}

WUFE_status WUFEI_HwTdmPdhLoopbackGetIndex(WUFE_line_pdh_loopback_type type,
										  WUFEI_system_registry* system_registry,
										  WP_U16* loopback_index)
{

	WP_U16 i;
	WUFE_status status=WUFE_OK;

	for(i=0;i<WUFEI_MAX_PDH_LOOPBACK;i++)
	{
		if(type == WUFE_LINE_LOOPBACK_PDH_TYPE_LINE)
		{
			if(system_registry->pdh_line_loopback[i]==WUFEI_MAX_OC48_LINES)
			{
				*loopback_index = i;
				return status;
			}
		}
		else if(type == WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM)
		{
			if(system_registry->pdh_system_loopback[i]==WUFEI_MAX_OC48_LINES)
			{
				*loopback_index=i;
				return status;
			}
		}
	}

	*loopback_index=WUFEI_MAX_PDH_LOOPBACK;
	return WUFE_ERR_LINE_PDH_LOOPBACK_MAX_NUM_OF_LOOPBACKS;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmPdhLoopbackSet
 *
 * Purpose: Sets the PDH loopback
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to TDM loopback register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    WUFE_LineLoopbackConfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmPdhLoopbackSet(WUFEI_system_registry* system_registry,
                                   WP_U8 lpbk_index,
                                   WUFE_line_pdh_loopback_type lpbk_type,
                                   WP_U32 coreid,
                                   WP_U8 enable)
{
   WP_U16 lb_value, *lb_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);

   if(enable)
   {
	   if(lpbk_type == WUFE_LINE_LOOPBACK_PDH_TYPE_LINE)
	   {
		   lb_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_pdh_line_lb.value;
	   }
	   else/* WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM*/
	   {
		   lb_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_pdh_system_lb.value;
	   }
	   WUFEI_REG_GET(lb_ptr,&lb_value);
	   /*set the bit of the loopback index */
	   lb_value |= (0x1 << lpbk_index);
	   WUFEI_REG_SET(lb_ptr, lb_value);
   }
   else
   {
	   if(lpbk_type == WUFE_LINE_LOOPBACK_PDH_TYPE_LINE)
	   {
		   lb_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_pdh_line_lb.value;
	   }
	   else /*WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM*/
	   {
		   lb_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_pdh_system_lb.value;
	   }

	   WUFEI_REG_GET(lb_ptr,&lb_value);

	   /*clear the bit of the loopback index */
	   lb_value &= ~(0x1 << lpbk_index);

	   WUFEI_REG_SET(lb_ptr, lb_value);

   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmIndirectSetup
 *
 * Purpose: resets the TDM regular and mode tables
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to TDM indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemTdmSetup
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmIndirectSetup(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   WP_U32 id = system_registry->wufe_id;
   WUFE_status status;
   WP_U32 bandwidth = system_registry->hw_config.bandwidth;
   WP_U32 i, offset;
   WP_U16 value, *mode_ptr;
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(id, coreid);

   /***********************/
   /*** Set TDM Parity ****/
   /***********************/
   if((system_registry->cfg.tdm_parity.tx_parity_mode >1) ||
      (system_registry->cfg.tdm_parity.rx_parity_mode >1))
      return WUFE_ERR_ILLEGAL_TDM_PARITY;

   value =
      (WUFEI_TDM_MODE_TX_PARITY_F(system_registry->cfg.tdm_parity.tx_parity_mode)  |
       WUFEI_TDM_MODE_RX_PARITY_F(system_registry->cfg.tdm_parity.rx_parity_mode));

   mode_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_mode.value;
   WUFEI_REG_SET(mode_ptr, value);

   /*********************************/
   /*** clear TDM Regular Tables ****/
   /*********************************/
   if ((bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC3) ||
       (bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC6) || 
       (bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12))
   {
      for(i=0;i<3;i++)
      {/*The HW divides the RAM into 6 individual sections, in OC12 there are only 3, seperated: 0,2,4*/
         offset = i*(WUFEI_INDIR_TDM_RX_REG_TABLE_SIZE/3);

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwTdmIdirectMemset(id, coreid,
                                                        (WP_U32)WUFEI_INDIR_TDM_RX_REG_TABLE(offset),
                                                        0x0000,
                                                        WUFEI_INDIR_TDM_RX_REG_TABLE_SIZE/6));
      }
   }
   else
   {/*OC24 or OC48*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmIdirectMemset(id, coreid,
                                                     (WP_U32)WUFEI_INDIR_TDM_RX_REG_TABLE(0),
                                                     0x0000,
                                                     WUFEI_INDIR_TDM_RX_REG_TABLE_SIZE));

   }

   /*******************************/
   /*** clear TDM Mode Tables   ***/
   /*******************************/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwTdmIdirectMemset(id, coreid,
                                                  (WP_U32)WUFEI_INDIR_TDM_RX_MODE_TABLE(0),
                                                  0x0000,
                                                  WUFEI_INDIR_TDM_RX_MODE_TABLE_SIZE));


   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmLineConfig
 *
 * Purpose: sets the TDM mode table for a specific line
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to TDM indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_LineCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmLineConfig(WUFEI_system_registry* system_registry,
                                  WUFEI_line_registry *line_registry,
                                  WP_U32 tdm_entry)
{
   WP_U32 id = system_registry->wufe_id;
   WUFE_status status;
   WP_U32 reg_value,coreid,transfer_type,framing_type,cas_enable,vfp_index;



   coreid = line_registry->ufe_core_id;
   transfer_type = line_registry->transfer_type;
   framing_type = line_registry->framed;
   cas_enable = line_registry->cas_enable;
   vfp_index = (WUFEI_MAX_N_AUG1 * line_registry->au3) + line_registry->aug1;


   /*In CAS mode set multiframe*/
   if (cas_enable == WUFE_CAS_ENABLE)
      framing_type = WUFEI_TDM_MODE_RAM_LINE_MODE_F(WUFEI_TDM_MODE_MULTI_FRAMED);

   reg_value = WUFEI_TDM_MODE_RAM_LINE_MODE_F(framing_type);
   if ( (transfer_type == WUFE_SDH_TYPE_VC44C) ||
        (transfer_type == WUFE_SDH_TYPE_VC4) ||
        (transfer_type == WUFE_SONET_TYPE_STS12) ||
        (transfer_type == WUFE_SONET_TYPE_STS3) ||
        (transfer_type == WUFE_SDH_TYPE_DS3) ||
        (transfer_type == WUFE_SONET_TYPE_DS3) ||
        (transfer_type == WUFE_SDH_TYPE_E3) ||
        (transfer_type == WUFE_SONET_TYPE_E3))
   {
      reg_value = WUFEI_TDM_MODE_RAM_LINE_MODE_F(WUFEI_TDM_MODE_RAM_TRIBUTARY_TYPE_CLEAR_CHANNEL)|
	WUFEI_TDM_MODE_RAM_CC_INDEX_F(vfp_index);
   }

   /*UFE Line is not direction based, so configuration is for both directions*/
   /*****************************/
   /*** Set TDM Mode Tables   ***/
   /*****************************/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwTdmIdirectMemset(id, coreid,
                                                  (WP_U32)WUFEI_INDIR_TDM_RX_MODE_TABLE(tdm_entry),
                                                  reg_value,
                                                  1));


   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmLineDelete
 *
 * Purpose: clears the TDM mode table for a specific line
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to TDM indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_LineDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmLineDelete(WUFEI_system_registry* system_registry,
                                  WP_U32 coreid,
                                  WP_U32 tdm_entry)
{
   WP_U32 id = system_registry->wufe_id;
   WUFE_status status;

   /*******************************/
   /*** clear TDM Mode Tables   ***/
   /*******************************/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwTdmIdirectMemset(id, coreid,
                                                  (WP_U32)WUFEI_INDIR_TDM_RX_MODE_TABLE(tdm_entry),
                                                  0x0000,
                                                  1));


   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmSlotsSet
 *
 * Purpose: Sets the tdm regular table for a specific phy
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     tdm_entry           The entry of the phy
 *     direction           The direction
 *
 * Outputs:
 *     write to TDM indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFE_PhyCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmSlotsSet(WP_U32 wufe_id,
                                WP_U32 core_id,
                                WP_U32 emphy_port_id,
                                WP_U32 direction,
                                WUFE_SlotGroup *slot_group,
                                WUFEI_line_registry *line_registry)
{
   WP_U16 min_slot_id = 0xffff;
   WP_U16 tdm_value, offset, i;
   WUFE_status status;

   /*Find the first slot in the group*/
   for (i=0; i<slot_group->n_slots;i++)
      if( slot_group->slot_arr[i]<min_slot_id)
         min_slot_id=slot_group->slot_arr[i];

   for (i=0; i<slot_group->n_slots; i++)
   {
      /*calculate entry of the specific slot*/
      if (line_registry->line_mode == WUFE_STRUCT_LINE_DCC)
      {
         /*Entry is calculated according to special channel id designated to DCC*/
         offset = 
            WUFEI_TDM_CH_ID_DS0_F(emphy_port_id - WUFE_MAX_OC48_LINES) |
            WUFEI_TDM_CH_ID_TUG2_F(WUFEI_TDM_TUG2_DCC);
      }
      else
      {
      /*Entry is calculated according to channel_id in the HW*/
      /*Bits 0:4 are the slot_id. Bits 5:6 are the tu*/
      /*Bits 7:9 are the tug2. Bits 10:11 are the aug1*/
      /*Bit 12 is aug4. Bits 13:14 are the au3.*/
      offset =
         WUFEI_TDM_CH_ID_DS0_F(slot_group->slot_arr[i])   |
         WUFEI_TDM_CH_ID_TU_F(line_registry->tu)    |
         WUFEI_TDM_CH_ID_TUG2_F(line_registry->tug2)  |
         WUFEI_TDM_CH_ID_AUG1_F(line_registry->aug1) |
         WUFEI_TDM_CH_ID_AUG4_F((line_registry->aug4 %2)) |
         WUFEI_TDM_CH_ID_AU3_F(line_registry->au3);
      }

      if (WUFEI_DbgGet(wufe_id) > WUFEI_DBG_LEVEL_0)
         printf("TDM channel id is %x\n",offset);
      
      /*Enable the slot*/
      tdm_value = WUFEI_TDM_ENTRY_PHY_ID_F(emphy_port_id);
      /*If it is the first slot, mark in the entry*/
      if(min_slot_id == slot_group->slot_arr[i])
         tdm_value |= WUFEI_TDM_ENTRY_FIRST_F(1);

      /*Set the indirect entry*/
      if (direction == WUFEI_LINE_TDM_RX)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwTdmIdirectMemset(wufe_id, core_id,
                                                        (WP_U32)WUFEI_INDIR_TDM_RX_REG_TABLE(offset),
                                                        tdm_value,
                                                        1));
      }


      /*Update the registry*/
      line_registry->tdm_ram_mng[0].
         tdm_slots[direction][slot_group->slot_arr[i]] = emphy_port_id;
   }

   return WUFE_OK;
}





WUFE_status WUFEI_HwTdmActiveEnableSet(WP_U32 wufe_id,
                                       WP_U32 core_id,
                                       WP_U32 emphy_port_id,
                                       WUFE_SlotGroup *slot_group,
                                       WUFEI_line_registry *line_registry)
{
   WP_U16 tdm_value, offset, i;
   WUFE_status status;
   for (i=0; i<slot_group->n_slots; i++)
   {   
      /*calculate entry of the specific slot*/
      if (line_registry->line_mode == WUFE_STRUCT_LINE_DCC)
      {
         /*Entry is calculated according to special channel id designated to DCC*/
         offset = 
            WUFEI_TDM_CH_ID_DS0_F(emphy_port_id - WUFE_MAX_OC48_LINES) |
            WUFEI_TDM_CH_ID_TUG2_F(WUFEI_TDM_TUG2_DCC);
      }
      else
      {
         offset =
            WUFEI_TDM_CH_ID_DS0_F(slot_group->slot_arr[i])   |
            WUFEI_TDM_CH_ID_TU_F(line_registry->tu)    |
            WUFEI_TDM_CH_ID_TUG2_F(line_registry->tug2)  |
            WUFEI_TDM_CH_ID_AUG1_F(line_registry->aug1) |
            WUFEI_TDM_CH_ID_AUG4_F((line_registry->aug4 %2)) |
            WUFEI_TDM_CH_ID_AU3_F(line_registry->au3);
      }
      
      /*Enable the slot*/
      WUFEI_HwTdmIdirectAddrSet(wufe_id, core_id, (WP_U32)WUFEI_INDIR_TDM_RX_REG_TABLE(offset));

      WUFEI_RETURN_IF_ERROR(status, 
                            WUFEI_HwTdmIdirectDataGet(wufe_id, core_id, &tdm_value));

      tdm_value |=  WUFEI_TDM_ENTRY_ACTIVE_F(WUFE_ENABLE);
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwTdmIdirectMemset(wufe_id, core_id,
                                                     (WP_U32)WUFEI_INDIR_TDM_RX_REG_TABLE(offset),
                                                     tdm_value,
                                                     1));
   
   }
   

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmSlotsClear
 *
 * Purpose: Clears the tdm regular table for a specific phy
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     tdm_entry           The entry of the phy
 *     direction           The direction
 *
 * Outputs:
 *     write to TDM indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFE_PhyDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmSlotsClear(WP_U32 ufeid,
                                  WP_U32 core_id,
                                  WP_U32 emphy_port_id,
                                  WP_U32 direction,
                                  WUFE_SlotGroup *slot_group,
                                  WUFEI_line_registry *line_registry)
{
   WP_U16 offset, i;
   WUFE_status status;

   if (direction != WUFEI_LINE_TDM_RX && direction != WUFEI_LINE_TDM_TX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

  
   for (i=0; i<slot_group->n_slots; i++)
   {
      /*calculate entry of the specific slot*/
      if (line_registry->line_mode == WUFE_STRUCT_LINE_DCC)
      {
         /*Entry is calculated according to special channel id designated to DCC*/
         offset = 
            WUFEI_TDM_CH_ID_DS0_F(emphy_port_id - WUFE_MAX_OC48_LINES) |
            WUFEI_TDM_CH_ID_TUG2_F(WUFEI_TDM_TUG2_DCC);
      }
      else
      {
      /*Entry is calculated according to channel_id in the HW*/
      /*Bits 0:4 are the slot_id. Bits 5:6 are the tu*/
      /*Bits 7:9 are the tug2. Bits 10:11 are the aug1*/
      /*Bit 12 is aug4. Bits 13:14 are the au3.*/
      offset =
         WUFEI_TDM_CH_ID_DS0_F(slot_group->slot_arr[i])   |
         WUFEI_TDM_CH_ID_TU_F(line_registry->tu)    |
         WUFEI_TDM_CH_ID_TUG2_F(line_registry->tug2)  |
         WUFEI_TDM_CH_ID_AUG1_F(line_registry->aug1) |
         WUFEI_TDM_CH_ID_AUG4_F((line_registry->aug4 %2)) |
         WUFEI_TDM_CH_ID_AU3_F(line_registry->au3);
      }

      /*Set the indirect entry*/
      if (direction == WUFEI_LINE_TDM_RX)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwTdmIdirectMemset(ufeid, core_id,
                                                        (WP_U32)WUFEI_INDIR_TDM_RX_REG_TABLE(offset),
                                                        0x0000,
                                                        1));
      }


     /*Update the registry*/
      line_registry->tdm_ram_mng[0].
         tdm_slots[direction][slot_group->slot_arr[i]] = WUFEI_NOT_VALID_SLOT;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmSlotsDisplay
 *
 * Purpose: Displays the slots of a specific phy
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     wufe_id             UFE id
 *     core_id             The core id of this tdm
 *     emphy_port_id       The id of the phy in this core
 *     direction           The direction of slots to display
 *     slot_group          The array of the slots to display
 *
 * Outputs:
 *     Display to screen
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyDisplay
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmSlotsDisplay(WP_U32 wufe_id,
                                    WP_U32 core_id,
                                    WP_U32 emphy_port_id,
                                    WP_U32 direction,
                                    WUFE_SlotGroup *slot_group,
                                    WUFEI_line_registry *line_registry)
{
   WP_U16 i, tdm_entry;
   WP_U32 offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if(slot_group->n_slots>WUFE_N_SLOTS_E1)
   {
       return WUFE_ERR_INVALID_CONFIG;
   }

   for (i=0; i<slot_group->n_slots; i++)
   {
      /*calculate entry of the specific slot*/
      if (line_registry->line_mode == WUFE_STRUCT_LINE_DCC)
      {
         /*Entry is calculated according to special channel id designated to DCC*/
         offset = 
            WUFEI_TDM_CH_ID_DS0_F(emphy_port_id - WUFE_MAX_OC48_LINES) |
            WUFEI_TDM_CH_ID_TUG2_F(WUFEI_TDM_TUG2_DCC);
      }
      else
      {
      /*Entry is calculated according to channel_id in the HW*/
      /*Bits 0:4 are the slot_id. Bits 5:6 are the tu*/
      /*Bits 7:9 are the tug2. Bits 10:11 are the aug1*/
      /*Bit 12 is aug4. Bits 13:14 are the au3.*/
      offset =
         WUFEI_TDM_CH_ID_DS0_F(slot_group->slot_arr[i])   |
         WUFEI_TDM_CH_ID_TU_F(line_registry->tu)    |
         WUFEI_TDM_CH_ID_TUG2_F(line_registry->tug2)  |
         WUFEI_TDM_CH_ID_AUG1_F(line_registry->aug1) |
         WUFEI_TDM_CH_ID_AUG4_F((line_registry->aug4 %2)) |
         WUFEI_TDM_CH_ID_AU3_F(line_registry->au3);
      }

      if (direction == WUFEI_LINE_TDM_RX)
      {
         WUFEI_HwTdmIdirectAddrSet(wufe_id, core_id, WUFEI_INDIR_TDM_RX_REG_TABLE(offset));
      }

      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwTdmIdirectDataGet(wufe_id, core_id, &tdm_entry));

      WUFE_PRINT_DISP((wufei_log,"%2d: 0x%04x\t", slot_group->slot_arr[i], tdm_entry));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }

   return WUFE_OK;
}


WUFE_status WUFEI_HwTdmPdhLoopbackConfig(WP_U32 wufe_id,
                                       	 WP_U32 core_id,
                                       	 WP_U32 loopback_index,
                                       	 WP_U16 tdm_entry,
                                       	 WUFE_line_pdh_loopback_type type)
{
   WP_U16 tdm_value=0;
   WUFE_status status;
   /*set the indirect address */
   WUFEI_HwTdmIdirectAddrSet(wufe_id, core_id, (WP_U32)WUFEI_INDIR_TDM_RX_MODE_TABLE(tdm_entry));
   /*read the value of the mode register*/
   WUFEI_RETURN_IF_ERROR(status,
		   WUFEI_HwTdmIdirectDataGet(wufe_id, core_id, &tdm_value));

   switch(type)
   {
   case WUFE_LINE_LOOPBACK_PDH_TYPE_LINE:
	   /*set the line loopback bit and set the index of the loopback*/
	   tdm_value |=  (WUFEI_TDM_MODE_RAM_PDH_LINE_LB_EN_M |
			   WUFEI_TDM_MODE_RAM_PDH_INDEX_F(loopback_index));
	   break;
   case WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM:
	   /*set the system loopback bit and set the index of the loopback*/
	   tdm_value |=  (WUFEI_TDM_MODE_RAM_PDH_SYS_LB_EN_M |
			   	   	  WUFEI_TDM_MODE_RAM_PDH_INDEX_F(loopback_index));
	   break;
   case WUFE_LINE_LOOPBACK_PDH_TYPE_NONE:
	   /*clear the system and line loopbacks bits and set the index of the loopback to zero*/
	   tdm_value &=  ~(WUFEI_TDM_MODE_RAM_PDH_LINE_LB_EN_M |
			   	   	   WUFEI_TDM_MODE_RAM_PDH_SYS_LB_EN_M);
	   tdm_value &= ~(WUFEI_TDM_MODE_RAM_PDH_INDEX_F(0xf));
	   break;
   }


   WUFEI_RETURN_IF_ERROR(status,
		   WUFEI_HwTdmIdirectMemset(wufe_id, core_id,
				   (WP_U32)WUFEI_INDIR_TDM_RX_MODE_TABLE(tdm_entry),
				   tdm_value,
				   1));

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmLineDisplay
 *
 * Purpose: Displays the tdm mode table indirect entry for a specific line
 *
 * Inputs:
 *     wufe_id             UFE id
 *     core_id             The core id of this tdm
 *
 * Outputs:
 *     Display to screen
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_LineDisplay
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmLineDisplay(WP_U32 wufe_id,
                                   WP_U32 core_id,
                                   WP_U16 tdm_entry)
{
   WUFE_status status;

   /*TDM Rx Mode table */
   WUFEI_HwTdmIdirectAddrSet(wufe_id, core_id, WUFEI_INDIR_TDM_RX_MODE_TABLE(tdm_entry));
   WUFE_PRINT_DISP((wufei_log," TDM Rx mode table:\n"));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwTdmIdirectDataGet(wufe_id, core_id, &tdm_entry));
   WUFE_PRINT_DISP((wufei_log," 0x%04x: 0x%04x\n", WUFEI_INDIR_TDM_RX_MODE_TABLE(tdm_entry), tdm_entry));


   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmIndirectDump
 *
 * Purpose: Displays the entire indirect address space of the tdm
 *
 * Inputs:
 *     wufe_id             UFE id
 *     core_id             The core id of this tdm
 *
 * Outputs:
 *     Display to screen
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_TdmDisplay
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTdmIndirectDump(WP_U32 wufe_id,
                                    WP_U32 core_id,
                                    WUFEI_system_registry* system_registry)
{
   WP_U16 i, j, offset, tdm_entry;
   WUFE_status status;
   WP_U32 bandwidth = system_registry->hw_config.bandwidth;

   if ((bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC3) ||
       (bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC6) || 
       (bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12))
   {
      for(j=0;j<3;j++)
      {/*The HW divides the RAM into 6 individual sections, in OC12 there are only 3, seperated: 0,2,4*/
         offset = j*(WUFEI_INDIR_TDM_RX_REG_TABLE_SIZE/3);

         /*Dump all TDM Rx regular tables */
         WUFEI_HwTdmIdirectAddrSet(wufe_id, core_id, WUFEI_INDIR_TDM_RX_REG_TABLE(offset));
         WUFE_PRINT_DISP((wufei_log,"TDM Rx regular tables\n"));

         for (i=0; i<WUFEI_INDIR_TDM_RX_REG_TABLE_SIZE/6; i++)
         {
            WUFEI_RETURN_IF_ERROR(status, WUFEI_HwTdmIdirectDataGet(wufe_id, core_id, &tdm_entry));

            WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_TDM_RX_REG_TABLE(i+offset), tdm_entry));
            if(i%4==3)
               WUFE_PRINT_DISP((wufei_log,"\n"));
         }

      }
   }
   else
   {/*OC24 or OC48*/
      /*Dump all TDM Rx regular tables */
      WUFEI_HwTdmIdirectAddrSet(wufe_id, core_id, WUFEI_INDIR_TDM_RX_REG_TABLE(0));
      WUFE_PRINT_DISP((wufei_log,"TDM Rx regular tables\n"));

      for (i=0; i<WUFEI_INDIR_TDM_RX_REG_TABLE_SIZE; i++)
      {
         WUFEI_RETURN_IF_ERROR(status, WUFEI_HwTdmIdirectDataGet(wufe_id, core_id, &tdm_entry));

         WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_TDM_RX_REG_TABLE(i), tdm_entry));
         if(i%4==3)
            WUFE_PRINT_DISP((wufei_log,"\n"));
      }

   }

   /*Dump all TDM Rx Mode tables */
   WUFEI_HwTdmIdirectAddrSet(wufe_id, core_id, WUFEI_INDIR_TDM_RX_MODE_TABLE(0));
   WUFE_PRINT_DISP((wufei_log,"TDM Rx mode tables\n"));

   for (i=0; i<WUFEI_INDIR_TDM_RX_MODE_TABLE_SIZE; i++)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwTdmIdirectDataGet(wufe_id, core_id, &tdm_entry));

      WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_TDM_RX_MODE_TABLE(i), tdm_entry));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }


   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmDisplay
 *
 * Purpose: Displays the Tdm mode and idle registers
 *
 * Inputs:
 *     ufe_id              UFE id
 *     core_id             The core id of this tdm
 *
 * Outputs:
 *     Display to screen
 *
 * Return Value:
 *
 * Called by:
 *     WUFE_SystemDisplay
 *
 ****************************************************************************/
void WUFEI_HwTdmDisplay(WP_U32 ufe_id, WP_U32 coreid)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufe_id,coreid);
   WP_U16 mode_value, *mode_ptr;
   WP_U16 loopback_value, *loopback_ptr;

   mode_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_mode.value;

   WUFEI_REG_GET(mode_ptr, &mode_value);

   WUFE_PRINT_DISP((wufei_log,"The TDM %d Mode Register: 0x%x\n",coreid,mode_value));

   if (WUFEI_TDM_MODE_RX_EN_V(mode_value))
      WUFE_PRINT_DISP((wufei_log,"TDM's %d RX direction is enabled\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"TDM's %d RX direction is disabled\n",coreid));

   if (WUFEI_TDM_MODE_TX_EN_V(mode_value))
      WUFE_PRINT_DISP((wufei_log,"TDM's %d TX direction is enabled\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"TDM's %d TX direction is disabled\n",coreid));

   mode_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_idle.value;
   WUFEI_REG_GET(mode_ptr, &mode_value);

   WUFE_PRINT_DISP((wufei_log,"The TDM %d Idle Register: 0x%x\n",coreid,mode_value));

   WUFE_PRINT_DISP((wufei_log,"The Idle CAS %d is: %x\n",coreid,WUFEI_TDM_IDLE_CAS_PATTERN_V(mode_value)));
   WUFE_PRINT_DISP((wufei_log,"The Idle DATA %d is: %x\n",coreid,WUFEI_TDM_IDLE_DATA_PATTERN_V(mode_value)));

   
   loopback_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_sys_lb.value;

   WUFEI_REG_GET(loopback_ptr, &loopback_value);

   WUFE_PRINT_DISP((wufei_log,"TDM system loopback register: 0x%x\n",loopback_value));

   if (WUFEI_TDM_SYS_LB_EN_V(loopback_value))
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback is enabled\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback is disabled\n"));

   if (WUFEI_TDM_SYS_LB_UNFRM_V(loopback_value))
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback in unframed mode\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback in framed mode\n"));

   if (WUFEI_TDM_SYS_LB_E1_T1N_V(loopback_value))
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback in E1 mode\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback in T1 mode\n"));

   if (WUFEI_TDM_SYS_LB_MF_MODE_V(loopback_value))
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback in multiframed mode\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"TDM system loopback not in multiframed mode\n"));


}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmEventRegSet
 *
 * Purpose:   Setter for the event register
 *
 *
 * Description:
 *     This function sets the event register to the value passed to the function.
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *     value - the value to set in the register
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwTdmEventRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_interrupt.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmEventRegGet
 *
 * Purpose:   Getter for the event register
 *
 *
 * Description:
 *     This function gets the event register into the value passed to the function.
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *     value - the value to set in the register
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwTdmEventRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_interrupt.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmEventMaskRegSet
 *
 * Purpose:   Setter for the mask event register
 *
 *
 * Description:
 *     This function sets the mask event register to the value passed to the function.
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *     value - the value to set in the register
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwTdmEventMaskRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_interrupt_mask.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTdmEventMaskRegGet
 *
 * Purpose:   Getter for the mask event register
 *
 *
 * Description:
 *     This function gets the mask event register into the value passed to the function.
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *     value - the value to get the register into.
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwTdmEventMaskRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value)
{
   WP_U32 tdm_base = WUFEI_UfeTdmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_tdm*)tdm_base)->tdm_interrupt_mask.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}
