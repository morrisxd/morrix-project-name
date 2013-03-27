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

#ifndef WUFEI_HW_CLK_REC_H
#include "wufei_hw_clk_rec.h"
#endif

#ifndef WUFEI_HW_CLK_REC_INT_H
#include "wufei_hw_clk_rec_int.h"
#endif

#ifndef WUFEI_LINE_H
#include "wufei_line.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

static WP_U32 WUFEI_UfeClockRecBaseAddrGet(WP_U32 ufeid);
static void WUFEI_HwClkRecIndirectAddrSet(WP_U32 ufeid, WP_U32 indirect_addr);
static WUFE_status WUFEI_HwClkRecIndirectDataSet(WP_U32 ufeid, WP_U16 value);
static WUFE_status WUFEI_HwClkRecIndirectDataGet(WP_U32 ufeid, WP_U16* value);

/*****************************************************************************
 *
 * Function:  WUFEI_UfeClockRecBaseAddrGet
 *
 * Purpose:   Getter for the base address of the Clock Recovery Registers
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     The base address of the Clock Recovery Registers
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WP_U32 WUFEI_UfeClockRecBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)&(base_addr->clk_rec);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecIndirectAddrSet
 *
 * Purpose: Indirect address set for the Clock Recovery Block
 *
 *
 * Description:
 *
 * Inputs:
 *      ufeid - the UFE id
 *      indirect_addr - indirect address
 *
 * Outputs:
 *     write to CR indirect address register
 *
 * Return Value: none
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwClkRecIndirectAddrSet(WP_U32 ufeid, WP_U32 indirect_addr)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_add_low.value;
   indha_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_add_high.value;

   WUFEI_REG_SET(indla_ptr, indirect_addr & 0xffff);
   WUFEI_REG_SET(indha_ptr, (indirect_addr >> 16) & 0xffff);

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecIndirectDataSet
 *
 * Purpose: Indirect data set for the clock Recovery Block
 *
 *
 * Description:
 *
 * Inputs:
 *      ufeid - the UFE id
 *      value - value to set
 *
 * Outputs:
 *     write to CR indirect data register
 *
 * Return Value: None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwClkRecIndirectDataSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_cmd.value;

   WUFEI_REG_SET(inddat_ptr, value);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0) 
      {
         WP_U16 *ind_ptr;

         WUFE_PRINT_ERR((wufei_log,"ERROR: Timeout while trying to set data %x in Clock Recovery indirect register\n", value));
         ind_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_add_low.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", value));
         ind_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_add_high.value;
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
 * Function:  WUFEI_HwClkRecIndirectDataGet
 *
 * Purpose: Indirect data get for the clock Recovery Block
 *
 *
 * Description:
 *
 * Inputs:
 *      ufeid - the UFE id
 *      value - value to get
 *
 * Outputs:
 *     get the CR indirect data register
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwClkRecIndirectDataGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_cmd.value;

   WUFEI_REG_SET(indcmd_ptr, WUFEI_INDIRECT_RD_CMD);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value); 
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0) 
      {
         WP_U16 *ind_ptr;
         WP_U16 tmp_value;

         WUFE_PRINT_ERR((wufei_log,"ERROR: Timeout while trying to get data from Clock Recovery indirect register\n"));
         ind_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_add_low.value;
         WUFEI_REG_GET(ind_ptr ,&tmp_value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", tmp_value));
         ind_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_ind_add_high.value;
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
 * Function:  WUFEI_HwClkRecEventsRegSet
 *
 * Purpose: Clear CR interrupt register
 *
 * Description:
 *
 * Inputs:
 *     ufeid               the UFE id
 *     value               required clear bits
 *
 * Outputs:
 *     Clear bits in the CR interrupt registrer
 *
 * Return Value:None
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClkRecEventsRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *events_reg_ptr;

   events_reg_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_interrupt.value;
   WUFEI_REG_SET(events_reg_ptr, value);
}

/*****************************************************************************
 * Function:  WUFEI_HwClkRecEventsRegGet
 *
 * Purpose: read CR interrupt register
 *
 * Description:
 *
 * Inputs:
 *     ufeid               the UFE id
 *     value               the interrupt register value
 *
 * Outputs:
 *     Read bits in the CR interrupt registrer
 *
 * Return Value:None
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClkRecEventsRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *events_reg_ptr;

   events_reg_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_interrupt.value;
   WUFEI_REG_GET(events_reg_ptr, value);
}

/*****************************************************************************
 * Function:  WUFEI_HwClkRecEventsMaskRegSet
 *
 * Purpose: Set CR interrupt mask register
 *
 * Description:
 *
 * Inputs:
 *     ufeid               the UFE id
 *     value               required mask bits
 *
 * Outputs:
 *     Set bits in the CR interrupt mask registrer
 *
 * Return Value:None
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClkRecEventsMaskRegSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *events_reg_mask_ptr;

   events_reg_mask_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_interrupt_mask.value;
   WUFEI_REG_SET(events_reg_mask_ptr, value);
}

/*****************************************************************************
 * Function:  WUFEI_HwClkRecEventsMaskRegGet
 *
 * Purpose: Read CR interrupt mask register
 *
 * Description:
 *
 * Inputs:
 *     ufeid               the UFE id
 *     value               mask register status
 *
 * Outputs:
 *     Read bits in the CR interrupt mask registrer
 *
 * Return Value:None
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClkRecEventsMaskRegGet(WP_U32 ufeid, WP_U16* value)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *events_reg_mask_ptr;

   events_reg_mask_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_interrupt_mask.value;
   WUFEI_REG_GET(events_reg_mask_ptr, value);
}

/*****************************************************************************
 * Function:  WUFEI_HwClkRecTxStuffLossTribIdRegGet
 *
 * Purpose: Read RX stuffing loss register.
 *
 * Description:
 *
 * Inputs:
 *     ufeid               the UFE id
 *     idx                 The lane index (0-laneA, 1-laneB)
 *     value               Tributery ID with stuffing loss.
 *
 * Outputs:
 *     The higher tributery ID from lanes A or B that has Stuffing Loss event.
 *
 * Return Value:None
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClkRecTxStuffLossTribIdRegGet(WP_U32 ufeid, WP_U32 idx, WP_U16* value)
{
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 *tx_stuff_loss_trib_id;

   tx_stuff_loss_trib_id = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_tx_stf_loss_trib_id[idx].trib_id.value;
   WUFEI_REG_GET(tx_stuff_loss_trib_id, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecEnable
 *
 * Purpose: Enables the Clock Recovery in the HW
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     direction           direction to enable
 *
 *
 * Outputs:
 *     Set the enable bits in the required direction
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClkRecEnable(WUFEI_system_registry* system_registry, WP_U32 direction)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 cr_mode_value, *cr_mode_ptr;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_CLOCK_REC_INVALID_DIRECTION;
   }

   cr_mode_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_mode_reg.value;
   WUFEI_REG_GET(cr_mode_ptr, &cr_mode_value);

   if (direction & WUFE_RX_DIRECTION)
   {
      cr_mode_value |= WUFEI_CLK_REC_MODE_RX_EN_M;
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      cr_mode_value |= WUFEI_CLK_REC_MODE_TX_EN_M;
   }

   WUFEI_REG_SET(cr_mode_ptr, cr_mode_value);

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecDisable
 *
 * Purpose: Disables the Clock Recovery in the HW
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     direction           direction to disable
 *
 *
 * Outputs:
 *     Unset the enable bits in the required direction
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClkRecDisable(WUFEI_system_registry* system_registry, WP_U32 direction)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 cr_mode_value, *cr_mode_ptr;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_CLOCK_REC_INVALID_DIRECTION;
   }

   cr_mode_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_mode_reg.value;
   WUFEI_REG_GET(cr_mode_ptr, &cr_mode_value);

   if (direction & WUFE_RX_DIRECTION)
   {
      cr_mode_value &= ~WUFEI_CLK_REC_MODE_RX_EN_M;
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      cr_mode_value &= ~WUFEI_CLK_REC_MODE_TX_EN_M;
   }

   WUFEI_REG_SET(cr_mode_ptr, cr_mode_value);

   return WUFE_OK;

}

/* Function for internal testing and debug */
/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecTsSet
 *
 * Purpose: Internal testing and debugging function. set/clear the stubbing
 *          bit in the CR mode register in the UFE HW
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     enable              enable / disable command
 *
 *
 * Outputs:
 *     set or clear the stub enable bit in the CR mode register in the UFE HW
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClkRecTsSet(WUFEI_system_registry* system_registry, WP_U32 enable)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 cr_mode_value, *cr_mode_ptr;

   cr_mode_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_mode_reg.value;
   WUFEI_REG_GET(cr_mode_ptr, &cr_mode_value);

   if (enable)
   {
      cr_mode_value |= WUFEI_CLK_REC_TS_STUB_EN_M;
   }
   else
   {
      cr_mode_value &= ~WUFEI_CLK_REC_TS_STUB_EN_M;
   }

   WUFEI_REG_SET(cr_mode_ptr, cr_mode_value);

   return WUFE_OK;
}
/* end of Function for internal testing and debug */

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecDisplay
 *
 * Purpose: Display the Clock Recovery HW registers and indirect addresses
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 *
 * Outputs:
 *     
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecDisplay(WUFEI_system_registry *system_registry)
{
   WUFE_status status = WUFE_OK;
   WP_U32 wufe_id = system_registry->wufe_id;
   WUFEI_line_registry   *line_registry;
   WUFE_line_clock_rec_params *cr_line_params;
   WP_U32 line_id;

   /* Display mode register */
   WUFEI_HwClkRecModeDisplay(system_registry);

   /* Display Ratio Counter register */
   WUFEI_HwClkRecRatioCounterDisplay(system_registry);

   /* Display the mode RAM */
   for (line_id = 0; line_id < system_registry->cfg.max_line_number; line_id++)
   {
      /* Extract the line registry and check for CR enable */
      line_registry = &system_registry->line_arr[line_id];
      cr_line_params = line_registry->clock_rec_line_params;
      if (cr_line_params)
      {
         /* Check only lines that CR params are exist */
         if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
         {
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwClockRecTxLineModeRamDisplay(wufe_id, line_id));
         }
      }
   }

   /* Display the Stuff indication RAM */
   for (line_id = 0; line_id < system_registry->cfg.max_line_number; line_id++)
   {
      /* Extract the line registry and check for CR enable */
      line_registry =&system_registry->line_arr[line_id];
      cr_line_params = line_registry->clock_rec_line_params;
      if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwClockRecTxLineStuffIndicationRamDisplay(wufe_id, line_id));
      }
   }

   /* Display the Gap Count RAM */
   for (line_id = 0; line_id < system_registry->cfg.max_line_number; line_id++)
   {
      /* Extract the line registry and check for CR enable */
      line_registry =&system_registry->line_arr[line_id];
      cr_line_params = line_registry->clock_rec_line_params;
      if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwClockRecTxLineGapCountRamDisplay(wufe_id, line_id));
      }
   }

   /* Display the Fractional Gap Count RAM */
   for (line_id = 0; line_id < system_registry->cfg.max_line_number; line_id++)
   {
      /* Extract the line registry and check for CR enable */
      line_registry =&system_registry->line_arr[line_id];
      cr_line_params = line_registry->clock_rec_line_params;
      if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwClockRecTxLineFracGapCountRamDisplay(wufe_id, line_id));
      }
   }

   /* Display the Delta Gap RAM */
   for (line_id = 0; line_id < system_registry->cfg.max_line_number; line_id++)
   {
      /* Extract the line registry and check for CR enable */
      line_registry =&system_registry->line_arr[line_id];
      cr_line_params = line_registry->clock_rec_line_params;
      if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwClockRecTxLineDeltaGapRamDisplay(wufe_id, line_id));
      }
   }

   WUFE_PRINT_DISP((wufei_log, "\n"));

   return status;

}




/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecRAMGet
 *
 * Purpose: Gets the Clock Recovery indirect addresses data
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 *
 * Outputs:
 *     
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecRAMGet(WUFEI_system_registry *system_registry,
                                   WUFE_handle ufe_line_handle,
                                   WUFE_clk_rec_info_ram_line *clk_rec_info_ram_line)
{
   WUFE_status status = WUFE_OK;
   WP_U32 wufe_id = system_registry->wufe_id;
   WUFEI_line_registry   *line_registry;
   WUFE_line_clock_rec_params *cr_line_params;
   WP_U32 line_id;  

   /* check valid line handle */
   if (WUFEI_HANDLE_TYPE_V(ufe_line_handle) != WUFE_HANDLE_TYPE_LINE)
   {
      WUFE_PRINT_ERR((wufei_log, "ERROR line handle 0x%08X ", ufe_line_handle));
      return WUFE_ERR_HANDLE;
   }

   /* Extract the line registry and check for CR enable */
   line_id = WUFEI_HANDLE_INDEX_V(ufe_line_handle);
   line_registry =&system_registry->line_arr[line_id];
   cr_line_params = line_registry->clock_rec_line_params;

   if (cr_line_params->tx_clk_rec_enable == WUFE_LINE_TX_CLOCK_REC_ENABLE)
   {
      WP_U16 indirect_reg_value;

      /* Get the Stuff indication RAM */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwClockRecTxTribStfIndRamRead(wufe_id,
                                                                   line_id,
                                                                   &indirect_reg_value));

         clk_rec_info_ram_line->stuffing_ind = WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_V(indirect_reg_value);
         

         /* Get the Gap Count RAM */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwClockRecTxTribGapCountRamRead(wufe_id,
                                                                     line_id,
                                                                     &indirect_reg_value));

         clk_rec_info_ram_line->current_gap_count = WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_V(indirect_reg_value);
         clk_rec_info_ram_line->max_gap_vector =  WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_V(indirect_reg_value);

      /* Get the Fractional Gap Count RAM */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwClockRecTxTribGapFracCountRamRead(wufe_id,
                                                                         line_id,
                                                                         &(clk_rec_info_ram_line->current_gap_fraction)));
        /* Get the Delta Gap RAM */
        WUFEI_RETURN_IF_ERROR(status,
                              WUFEI_HwClockRecTxTribDeltaGapRamRead(wufe_id,
                                                                    line_id,
                                                                    &(clk_rec_info_ram_line->delta_gap)));
   }

   return status;
}




/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecModeDisplay
 *
 * Purpose: Read and display the Clock Recovery mode register from the HW
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClkRecModeDisplay(WUFEI_system_registry* system_registry)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 cr_mode_reg, *cr_mode_ptr;
   WP_U32 cnt = 0;

   cr_mode_ptr = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_mode_reg.value;
   WUFEI_REG_GET(cr_mode_ptr, &cr_mode_reg);

   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Mode Register      0x%x\n", cr_mode_reg));

   if (cr_mode_reg && WUFEI_CLK_REC_MODE_RX_EN_M)
   {
      WUFE_PRINT_DISP((wufei_log, "\tClock Recovery Enable in RX direction\n"));
      cnt++;
   }
   if (cr_mode_reg && WUFEI_CLK_REC_MODE_TX_EN_M)
   {
      WUFE_PRINT_DISP((wufei_log, "\tClock Recovery Enable in TX direction\n"));
      cnt++;
   }
   if (!cnt)
      WUFE_PRINT_DISP((wufei_log, "\tClock Recovery disable\n"));
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecRatioCountersSet
 *
 * Purpose: Set Clock Ratio System Counters in the Clock Recovery block
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     diff_sck_value      Initial value for the Differential-to-Socket Ratio Counter
 *     xo_sck_value        Initial value for the Osillator-to-Socket Ratio Counter
 *                         Osillator is usualy the adaptive clock
 *
 * Outputs:
 *     Set the relevant values in the HW Ratio Counters registers
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwClkRecRatioCountersSet(WUFEI_system_registry* system_registry,
                                    WP_U32 diff_sck_value, WP_U32 xo_sck_value)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 init_val_high, init_val_low;
   WP_U16 *sys_cnt_init_h, *sys_cnt_init_l;

   /* Set Differential to socket ratio registers */
   sys_cnt_init_h = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_diff_sck_ratio_init_h.value;
   sys_cnt_init_l = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_diff_sck_ratio_init_l.value;

   init_val_low = diff_sck_value & 0xffff;
   init_val_high = (diff_sck_value >> 16);

   WUFEI_REG_SET(sys_cnt_init_h, init_val_high);
   WUFEI_REG_SET(sys_cnt_init_l, init_val_low);

   /* Set Oscillator to socket ratio registers */
   sys_cnt_init_h = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_xo_sck_ratio_init_h.value;
   sys_cnt_init_l = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_xo_sck_ratio_init_l.value;

   init_val_low = xo_sck_value & 0xffff;
   init_val_high = (xo_sck_value >> 16);

   WUFEI_REG_SET(sys_cnt_init_h, init_val_high);
   WUFEI_REG_SET(sys_cnt_init_l, init_val_low);

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecRatioCountersGet
 *
 * Purpose: Get Clock Ratio System Counters from the Clock Recovery block
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     diff_sck_value      Initial value for the Differential to socket Ratio Counter
 *     xo_sck_value        Initial value for the oscillator (adaptive) to socket Ratio Counter
 *
 *
 * Outputs:
 *     Get the value from the HW Ratio Counters registers
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WUFEI_HwClkRecRatioCountersGet(WUFEI_system_registry* system_registry,
                                    WP_U32 *diff_sck_value, WP_U32 *xo_sck_value)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 cr_base = WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WP_U16 init_val_high, init_val_low;
   WP_U16 *sys_cnt_init_h, *sys_cnt_init_l;

   /* Read Differential to socket ratio registers */
   sys_cnt_init_h = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_diff_sck_ratio_init_h.value;
   sys_cnt_init_l = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_diff_sck_ratio_init_l.value;

   WUFEI_REG_GET(sys_cnt_init_h, &init_val_high);
   WUFEI_REG_GET(sys_cnt_init_l, &init_val_low);

   *diff_sck_value = (init_val_high & 0xffff) << 16;
   *diff_sck_value |= init_val_low & 0xffff;

   /* Read Oscillator to socket ratio registers */
   sys_cnt_init_h = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_xo_sck_ratio_init_h.value;
   sys_cnt_init_l = &((WUFEI_hw_clk_rec*)cr_base)->clk_rec_xo_sck_ratio_init_l.value;

   WUFEI_REG_GET(sys_cnt_init_h, &init_val_high);
   WUFEI_REG_GET(sys_cnt_init_l, &init_val_low);

   *xo_sck_value = (init_val_high & 0xffff) << 16;
   *xo_sck_value |= init_val_low & 0xffff;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClkRecRatioCounterDisplay
 *
 * Purpose: Display the Ratio Counter register read from the HW
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClkRecRatioCounterDisplay(WUFEI_system_registry* system_registry)
{
   WP_U32 diff2socket_ratio_counter, xo2socket_ratio_counter;

   WUFEI_HwClkRecRatioCountersGet(system_registry, &diff2socket_ratio_counter, &xo2socket_ratio_counter);
   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Differential-to-Socket Ratio Register     0x%x\n",
                    diff2socket_ratio_counter));
   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Oscillator-to-Socket Ratio Register       0x%x\n",
                    xo2socket_ratio_counter));
}

/*****************************************************************************
 *
 * Function: WUFEI_HwClockRecDcoDivisorSet
 *
 * Purpose: Sets the clock devisor integer and remainder parts
 *
 * Description:
 *
 *      ufeid -  UFE id
 *      dco_id - DCO id
 *      integer - initial integer part
 *      remainder - initial remainder part
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *      WUFE_ERR_MALLOC
 *
 * Called by:
 *      WUFEI_ClkRecPhysicalClockSet() func.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecDcoDivisorSet(WP_U32 ufeid, WP_U32 dco_id,
                                          WP_U16 integer, WP_U32 remainder)
{
   WUFEI_hw_clk_rec *cr_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *integer_ptr, *remainder_msb_ptr, *remainder_lsb_ptr;

   dco_ptr = &cr_base->dco[dco_id];
   integer_ptr = &(dco_ptr->integer).value;
   remainder_msb_ptr = &(dco_ptr->remainder_msb).value;
   remainder_lsb_ptr = &(dco_ptr->remainder_lsb).value;

   WUFEI_REG_SET(integer_ptr, integer);
   WUFEI_REG_SET(remainder_msb_ptr, (WP_U16)(remainder>>16));
   WUFEI_REG_SET(remainder_lsb_ptr,(WP_U16)remainder);

   return WUFE_OK;
}



/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecDcoEnable
 *
 * Purpose: Toggles the enable bit in the DCO mode register
 *
 * Description:
 *
 *      ufeid -  UFE id
 *      dco_id - DCO id
 *      enable
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *      WUFE_ERR_MALLOC
 *
 * Called by:
 *      WUFEI_ClockRecConfigDivider() func.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecDcoEnable(WP_U32 ufeid, WP_U32 dco_id, WP_U8 enable)
{
  WUFEI_hw_clk_rec *cr_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *mode_ptr, value;

   dco_ptr = &cr_base->dco[dco_id];
   mode_ptr = &(dco_ptr->mode).value;
   
   WUFEI_REG_GET(mode_ptr, &value);
   
     /* Set/clear Enable bit */
   if(enable)
      value |= WUFEI_HW_CLK_REC_DCO_MODE_EN_M;
   else
      value &= ~WUFEI_HW_CLK_REC_DCO_MODE_EN_M;

   WUFEI_REG_SET(mode_ptr, value);

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecDcoSet
 *
 * Purpose: Toggles the enable bit in the DCO mode register and set the E1/T1 mode.
 *
 * Description:
 *
 *      ufeid -  UFE id
 *      dco_id - DCO id
 *      enable
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *      WUFE_ERR_MALLOC
 *
 * Called by:
 *      WUFEI_ClkRecPhysicalClockSet() func.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecDcoSet(WP_U32 ufeid, WP_U32 dco_id, WP_U32 e1_indication, WP_U8 enable)
{
   WUFEI_hw_clk_rec *cr_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClockRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *mode_ptr, value;

   dco_ptr = &cr_base->dco[dco_id];
   mode_ptr = &(dco_ptr->mode).value;

   WUFEI_REG_GET(mode_ptr, &value);

   /* Set E1/T1 indication */
   if (e1_indication)
      value |= WUFEI_HW_CLK_REC_DCO_MODE_E1T1_M;
   else
      value &= ~WUFEI_HW_CLK_REC_DCO_MODE_E1T1_M;

   /* Set/clear Enable bit */
   if(enable)
      value |= WUFEI_HW_CLK_REC_DCO_MODE_EN_M;
   else
      value &= ~WUFEI_HW_CLK_REC_DCO_MODE_EN_M;

   WUFEI_REG_SET(mode_ptr, value);

   return WUFE_OK;
}


WUFE_status WUFEI_HwClockRecTxLineModeRamDisplay(WP_U32 wufe_id, WP_U32 line_id)
{
   WUFE_status status = WUFE_OK;
   WP_U16 trib_mode;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClockRecTxTribModeRamGet(wufe_id,
                                                          line_id,
                                                          &trib_mode));
   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Line %4d: Mode RAM            0x%x\n", line_id, trib_mode));
   return status;
}

WUFE_status WUFEI_HwClockRecTxLineStuffIndicationRamDisplay(WP_U32 wufe_id, WP_U32 line_id)
{
   WUFE_status status = WUFE_OK;
   WP_U16 trib_stf_ind;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClockRecTxTribStfIndRamRead(wufe_id,
                                                             line_id,
                                                             &trib_stf_ind));
   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Line %4d: Stuff Indication RAM       0x%x\n", line_id, trib_stf_ind));
   return status;
}

WUFE_status WUFEI_HwClockRecTxLineGapCountRamDisplay(WP_U32 wufe_id, WP_U32 line_id)
{
   WUFE_status status = WUFE_OK;
   WP_U16 trib_gap_count;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClockRecTxTribGapCountRamRead(wufe_id,
                                                               line_id,
                                                               &trib_gap_count));

   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Line %4d: Gap Counter RAM            0x%x\n", line_id, trib_gap_count));
   return status;
}

WUFE_status WUFEI_HwClockRecTxLineFracGapCountRamDisplay(WP_U32 wufe_id, WP_U32 line_id)
{
   WUFE_status status = WUFE_OK;
   WP_U32 trib_gap_frac_count;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClockRecTxTribGapFracCountRamRead(wufe_id,
                                                                   line_id,
                                                                   &trib_gap_frac_count));

   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Line %4d: Fraction Gap Counter RAM   0x%x\n", line_id, trib_gap_frac_count));
   return status;
}

WUFE_status WUFEI_HwClockRecTxLineDeltaGapRamDisplay(WP_U32 wufe_id, WP_U32 line_id)
{
   WUFE_status status = WUFE_OK;
   WP_U32 trib_delta_gap;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClockRecTxTribDeltaGapRamRead(wufe_id,
                                                               line_id,
                                                               &trib_delta_gap));

   WUFE_PRINT_DISP((wufei_log,
                    "Clock Recovery Line %4d: Delta Gap RAM              0x%x\n", line_id, trib_delta_gap));
   return status;
}


WUFE_status WUFEI_HwClockRecTxTribModeRamSet(WP_U16 wufe_id,
                                             WP_U16 line_id,
                                             WP_U8 line_type_e1,
                                             WP_U8 enable)
{
   WUFE_status status = WUFE_OK;
   WP_U16 trib_mode_ram;
   WUFEI_hw_clk_rec_trib_mode_ram *trib_mode_ram_ptr;

   trib_mode_ram_ptr = WUFEI_INDIRECT_TRIB_MODE_RAM(line_id);
   trib_mode_ram = WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_F(enable) |
      WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_F(line_type_e1);

   WUFEI_HwClkRecIndirectAddrSet(wufe_id, (WP_U32)&(trib_mode_ram_ptr->trib_mode));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataSet(wufe_id, trib_mode_ram));

   return status;
}

WUFE_status WUFEI_HwClockRecTxTribModeRamGet(WP_U16 wufe_id,
                                             WP_U16 line_id,
                                             WP_U16 *trib_mode)
{
   WUFE_status status = WUFE_OK;
   WUFEI_hw_clk_rec_trib_mode_ram *trib_mode_ram_ptr;

   trib_mode_ram_ptr = WUFEI_INDIRECT_TRIB_MODE_RAM(line_id);

   WUFEI_HwClkRecIndirectAddrSet(wufe_id, (WP_U32)&(trib_mode_ram_ptr->trib_mode));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataGet(wufe_id, trib_mode));

   return status;
}

WUFE_status WUFEI_HwClockRecTxTribStfIndRamRead(WP_U16 wufe_id,
                                                WP_U16 line_id,
                                                WP_U16 *trib_stf_ind_ram)
{
   WUFE_status status = WUFE_OK;
   WUFEI_hw_clk_rec_trib_stf_ind_ram *trib_stf_ind_ram_ptr;

   trib_stf_ind_ram_ptr = WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM(line_id);

   WUFEI_HwClkRecIndirectAddrSet(wufe_id, (WP_U32)&(trib_stf_ind_ram_ptr->trib_stf_ind));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataGet(wufe_id, trib_stf_ind_ram));

   return status;
}

WUFE_status WUFEI_HwClockRecTxTribGapCountRamRead(WP_U16 wufe_id,
                                                  WP_U16 line_id,
                                                  WP_U16 *trib_gap_count_ram)
{
   WUFE_status status = WUFE_OK;
   WUFEI_hw_clk_rec_trib_gap_count_ram *trib_gap_count_ram_ptr;

   trib_gap_count_ram_ptr = WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM(line_id);
   
   WUFEI_HwClkRecIndirectAddrSet(wufe_id, (WP_U32)&(trib_gap_count_ram_ptr->trib_gap_cnt));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataGet(wufe_id, trib_gap_count_ram));

   return status;
}

WUFE_status WUFEI_HwClockRecTxTribGapFracCountRamRead(WP_U16 wufe_id,
                                                      WP_U16 line_id,
                                                      WP_U32 *trib_gap_frac_count_ram)
{
   WUFE_status status = WUFE_OK;
   WP_U16 gap_frac_l, gap_frac_h;
   WUFEI_hw_clk_rec_trib_gap_frac_count_ram *trib_gap_frac_count_ram_ptr;

   trib_gap_frac_count_ram_ptr = WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM(line_id);


   WUFEI_HwClkRecIndirectAddrSet(wufe_id, (WP_U32)&(trib_gap_frac_count_ram_ptr->trib_gap_frac_cnt_h));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataGet(wufe_id, &gap_frac_h));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataGet(wufe_id, &gap_frac_l));

   *trib_gap_frac_count_ram = (gap_frac_h << 16) | gap_frac_l;

   return status;
}

WUFE_status WUFEI_HwClockRecTxTribDeltaGapRamRead(WP_U16 wufe_id,
                                                  WP_U16 line_id,
                                                  WP_U32 *trib_delta_gap_ram)
{
   WUFE_status status = WUFE_OK;
   WP_U16 delta_gap_l, delta_gap_h;
   WUFEI_hw_clk_rec_trib_delta_gap_ram *trib_delta_gap_ram_ptr;

   trib_delta_gap_ram_ptr = WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM(line_id);

   WUFEI_HwClkRecIndirectAddrSet(wufe_id, (WP_U32)&(trib_delta_gap_ram_ptr->trib_delta_gap_h));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataGet(wufe_id, &delta_gap_h));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwClkRecIndirectDataGet(wufe_id, &delta_gap_l));
   /* update delta_gap as returning value */
   *trib_delta_gap_ram = (delta_gap_h << 16) | delta_gap_l;
   
   return status;
}

void WUFEI_HwClockRecEventsDispatch(WP_U32 wufe_id, WUFE_events_clock_recovery *events)
{
   WP_U16 int_reg, mask_reg, value;

   /* Get CR interrupt register */
   WUFEI_HwClkRecEventsRegGet(wufe_id, &int_reg);
   /* Get CR interrupt mask register */
   WUFEI_HwClkRecEventsMaskRegGet(wufe_id, &mask_reg);

   events->event_reg = int_reg & ~mask_reg;

   /* mask the lane_b interrupt */
   events->event_reg &= ~WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_M;

   /* Check for Stuffing loss indication */
   if (WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_V(int_reg))
   {
      WUFEI_HwClkRecTxStuffLossTribIdRegGet(wufe_id, 0, &value);
      events->stuffing_loss_indication_line_id = value;
   }
   /* Check for Stuffing loss indication */
   else if (WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_V(int_reg))
   {
      WUFEI_HwClkRecTxStuffLossTribIdRegGet(wufe_id, 1, &value);
      events->stuffing_loss_indication_line_id = value + WUFEI_MAX_OC24_LINES;
   }

   /* Clear CR interrupt register */
   WUFEI_HwClkRecEventsRegSet(wufe_id, int_reg);
}
