
/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif

#ifndef WUFEI_HW_CLK_REC_H
#include "wufei_hw_clk_rec.h"
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

extern WP_U8 debug_flag;

static void WUFEI_HwClockRecTsGenConfigRegGet(WP_U32 ufeid, WP_U16* value);


/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecInclkMuxSelConfig
 *
 * Purpose: Writes the clock recovery mux registers for E3/DS3 lines
 *
 * Description:
 *
 *      ufeid - UFE id
 *      speid - SPE id
 *      ds3e3_mode - DS3/E3 line select
 *      framed_mode - framed
 *      inclk_div - WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_0
 *                  WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_32
 *                  WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_1074
 *                  WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_1398
 *      inclk_sel - selects the in clock source
 *                  '1' -  TX serial clock from Jitter Attenuator
 *                  '0' -  TX serial clock from DCO
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecInclkMuxSelConfig(WP_U32 ufeid, WP_U32 speid, WP_U8 ds3e3_mode,
                                              WP_U8 framed_mode, WP_U8 inclk_div, WP_U8 inclk_sel)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_mux *mux_ptr = &clk_rec_base->mux;
   WP_U16* reg_ptr, value;

   reg_ptr = &(mux_ptr->clk_mux[speid]).value;

   value =
      WUFEI_HW_CLK_REC_MUX_MODE_F(ds3e3_mode)|
      WUFEI_HW_CLK_REC_MUX_MODE_FRAMED_F(framed_mode)|
      WUFEI_HW_CLK_REC_MUX_INCLK_SEL_F(inclk_sel);

   WUFEI_REG_SET(reg_ptr, value);

   reg_ptr = &(mux_ptr->dco_mux[speid]).value;

   value = WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_F(inclk_div);

   WUFEI_REG_SET(reg_ptr, value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecEventRegSet
 *
 * Purpose: Writes the clock recovery event registers
 *
 * Description:
 *
 *      ufeid - UFE id
 *      value - value
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClockRecEventRegSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &(clk_rec_base->event_reg).value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecEventRegGet
 *
 * Purpose: Reads the clock recovery event registers
 *
 * Description:
 *
 *      ufeid - UFE id
 *      value - value
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *     WUFEI_HwEventReadCoreEvents()
 ****************************************************************************/
void WUFEI_HwClockRecEventRegGet(WP_U32 ufeid, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->event_reg.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecEventMaskRegSet
 *
 * Purpose: Writes the clock recovery event mask registers
 *
 * Description:
 *
 *      ufeid - UFE id
 *      value - value
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *    WUFEI_HwEventReadCoreEvents()
 ****************************************************************************/
void WUFEI_HwClockRecEventMaskRegSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16* event_mask_reg_ptr;

   event_mask_reg_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->event_mask_reg.value;

   WUFEI_REG_SET(event_mask_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecEventMaskRegGet
 *
 * Purpose: Reads the clock recovery event mask registers
 *
 * Description:
 *
 *      ufeid - UFE id
 *      value - value
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *   WUFEI_HwEventReadCoreEvents()
 ****************************************************************************/
void WUFEI_HwClockRecEventMaskRegGet(WP_U32 ufeid, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16* event_mask_reg_ptr;

   event_mask_reg_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->event_mask_reg.value;

   WUFEI_REG_GET(event_mask_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecGeneralCfgGet
 *
 * Purpose: Reads the general configuration register
 *
 *
 * Description:
 *
 *      ufeid -  UFE id
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *     WUFEI_HwClockRecHwConfigSetup()
 *
 ****************************************************************************/
static WP_U16 WUFEI_HwClockRecGeneralCfgGet(WP_U32 ufeid)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_general_cfg* general_cfg_ptr;
   WP_U16* cr_blocks_ptr, value;

   general_cfg_ptr = &clk_rec_base->general_cfg;
   cr_blocks_ptr = &general_cfg_ptr->blocks.value;
   WUFEI_REG_GET(cr_blocks_ptr, &value);

   return value;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecIdirectAddrSet
 *
 * Purpose: Indirect address set for the Clock recovery internal RAM
 *
 *
 * Description:
 *
 *      ufeid -  UFE id
 *      indirect_addr - indirect address
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClockRecIdirectAddrSet(WP_U32 ufeid, void* indirect_addr)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->indla.value;
   indha_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->indha.value;

   WUFEI_REG_INDIRECT_ADDRESS_SET((WP_U32)indha_ptr/WUFEI_ADDR_DRV_TO_HW,
                                  (WP_U32)indla_ptr/WUFEI_ADDR_DRV_TO_HW,
                                  (WP_U32)indirect_addr*WUFEI_INDRCT_ADDR_ABS_TO_DRV,
                                  0, 0);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecIdirectDataSet
 *
 * Purpose: Indirect address set for the SBI RAMs
 *
 * Description:
 *
 *      ufeid - UFE id
 *      value - value to set
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClockRecIdirectDataSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;

   inddat_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->inddat.value;

   WUFEI_REG_SET(inddat_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecIdirectDataGet
 *
 * Purpose: Indirect address set for the SBI RAMs
 *
 * Description:
 *
 *      ufeid - UFE id
 *      value - value to get
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClockRecIdirectDataGet(WP_U32 ufeid, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;

   inddat_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->inddat.value;

   WUFEI_REG_GET(inddat_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecIdirectMemset
 *
 * Purpose: Indirect address set for the SBI RAMs
 *
 * Description:
 *
 *      ufeid - UFE id
 *      addr - start address
 *      value - value to get
 *      size - memset size
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClockRecIdirectMemset(WP_U32 ufeid, void* addr, WP_U16 value, WP_U32 size)
{
   WP_U32 i;

   WUFEI_HwClockRecIdirectAddrSet(ufeid, addr);
   for(i = 0; i<size; i+=2)
   {
      WUFEI_HwClockRecIdirectDataSet(ufeid, value);
   }
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecNumOfRxDiffUnitsGet
 *
 * Purpose: 
 *
 * Description:
 *
 *      ufeid - UFE id
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 ****************************************************************************/
WP_U32 WUFEI_HwClockRecNumOfRxDiffUnitsGet(WP_U32 ufeid)
{
   WUFEI_hw_clk_rec *clk_rec_base;
   WUFEI_hw_clk_rec_general_cfg* general_cfg_ptr;
   WP_U16* units_rx_ext_diff_ptr;
   WP_U16 value;

   clk_rec_base = (WUFEI_hw_clk_rec*)(WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   general_cfg_ptr = &clk_rec_base->general_cfg;
   units_rx_ext_diff_ptr = &((general_cfg_ptr->units_rx_diff).value);
   WUFEI_REG_GET(units_rx_ext_diff_ptr, &value);

   return WUFEI_HW_CLK_REC_TIMESTAMP_UNITS_N_RX_V(value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecNumOfRxAdapUnitsGet
 *
 * Purpose: 
 *
 * Description:
 *
 *      ufeid - UFE id
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 ****************************************************************************/
WP_U32 WUFEI_HwClockRecNumOfRxAdapUnitsGet(WP_U32 ufeid)
{
   WUFEI_hw_clk_rec *clk_rec_base;
   WUFEI_hw_clk_rec_general_cfg* general_cfg_ptr;
   WP_U16* units_rx_ptr;
   WP_U16 value;

   clk_rec_base = (WUFEI_hw_clk_rec*)(WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   general_cfg_ptr = &clk_rec_base->general_cfg;
   units_rx_ptr = &((general_cfg_ptr->units_rx_adap).value);
   WUFEI_REG_GET(units_rx_ptr, &value);

   return WUFEI_HW_CLK_REC_TIMESTAMP_UNITS_N_RX_V(value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecNumOfTxDiffUnitsGet
 *
 * Purpose: 
 *
 * Description:
 *
 *      ufeid - UFE id
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 ****************************************************************************/
WP_U32 WUFEI_HwClockRecNumOfTxDiffUnitsGet(WP_U32 ufeid)
{
   WUFEI_hw_clk_rec *clk_rec_base;
   WUFEI_hw_clk_rec_general_cfg* general_cfg_ptr;
   WP_U16* units_tx_ext_diff_ptr;
   WP_U16 value;

   clk_rec_base = (WUFEI_hw_clk_rec*)(WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   general_cfg_ptr = &clk_rec_base->general_cfg;
   units_tx_ext_diff_ptr = &((general_cfg_ptr->units_tx_diff).value);
   WUFEI_REG_GET(units_tx_ext_diff_ptr, &value);

   return WUFEI_HW_CLK_REC_TIMESTAMP_UNITS_N_RX_V(value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecNumOfTxAdapUnitsGet
 *
 * Purpose: 
 *
 * Description:
 *
 *      ufeid - UFE id
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *
 ****************************************************************************/
WP_U32 WUFEI_HwClockRecNumOfTxAdapUnitsGet(WP_U32 ufeid)
{
   WUFEI_hw_clk_rec *clk_rec_base;
   WUFEI_hw_clk_rec_general_cfg* general_cfg_ptr;
   WP_U16* units_tx_ptr;
   WP_U16 value;

   clk_rec_base = (WUFEI_hw_clk_rec*)(WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   general_cfg_ptr = &clk_rec_base->general_cfg;
   units_tx_ptr = &((general_cfg_ptr->units_tx_adap).value);
   WUFEI_REG_GET(units_tx_ptr, &value);

   return WUFEI_HW_CLK_REC_TIMESTAMP_UNITS_N_RX_V(value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecMethodSet
 *
 * Purpose: Sets the UFE clock recovery adaptive/differential method
 *
 * Description:
 *
 *      ufeid - UFE id
 *      method
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecSystemSetup()
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecMethodSet(WP_U32 ufeid, WP_U8 method)
{
   WUFEI_hw_clk_rec *clk_rec_base;
   WUFEI_hw_clk_rec_timestamp* timestamp_ptr;
   WP_U16* ts_gen_config_ptr;
   WP_U16 value;

   clk_rec_base = (WUFEI_hw_clk_rec*)(WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   timestamp_ptr = &clk_rec_base->timestamp;
   ts_gen_config_ptr = &((timestamp_ptr->ts_gen_config).value);
   WUFEI_REG_GET(ts_gen_config_ptr, &value);

   /* clear field for clock recovery method */
   value &= ~WUFEI_HW_CLK_REC_METHOD_M;
   if( method == WUFE_CLOCK_REC_METHOD_ADAPTIVE)
      value |= WUFEI_HW_CLK_REC_METHOD_F(WUFEI_HW_CLK_REC_METHOD_ADAPTIVE);
   else if( method == WUFE_CLOCK_REC_METHOD_DIFFERENTIAL)
      value |= WUFEI_HW_CLK_REC_METHOD_F(WUFEI_HW_CLK_REC_METHOD_DIFFERENTIAL);
   else
      return WUFE_ERR_CLOCK_REC_METHOD_INVALID;

   WUFEI_REG_SET(ts_gen_config_ptr, value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecTsGenConfigRegGet
 *
 * Purpose: Reads the timestamp general configuration register.
 *          The registor provides the Adaptive/Differential method indication
 *
 * Description:
 *
 *      ufeid - UFE id
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_ClockRecDisplay()
 *
 ****************************************************************************/
static void WUFEI_HwClockRecTsGenConfigRegGet(WP_U32 ufeid, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base;
   WUFEI_hw_clk_rec_timestamp* timestamp_ptr;
   WP_U16* ts_gen_config_ptr;

   clk_rec_base = (WUFEI_hw_clk_rec*)(WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   timestamp_ptr = &clk_rec_base->timestamp;
   ts_gen_config_ptr = &((timestamp_ptr->ts_gen_config).value);
   WUFEI_REG_GET(ts_gen_config_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecRxTsModeRegSet
 *
 * Purpose: Writes the RX timestamp mode to the clock recovery RX timestamp
 *          configuration register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecRxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecRxTsModeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_rx *timestamp_rx_ptr;
   WP_U16* rx_timestamp_mode_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_rx_ptr = &timestamp_ptr->rx[timestamp_id];

   rx_timestamp_mode_ptr = &(timestamp_rx_ptr->mode).value;

   WUFEI_REG_SET(rx_timestamp_mode_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecRxTsModeRegGet
 *
 * Purpose: Reads the RX timestamp mode from the clock recovery RX timestamp
 *          configuration register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecRxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecRxTsModeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_rx *timestamp_rx_ptr;
   WP_U16* rx_timestamp_mode_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_rx_ptr = &timestamp_ptr->rx[timestamp_id];

   rx_timestamp_mode_ptr = &(timestamp_rx_ptr->mode).value;

   WUFEI_REG_GET(rx_timestamp_mode_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecTxTsModeRegSet
 *
 * Purpose: Writes the TX timestamp mode to the clock recovery TX timestamp
 *          configuration register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecTxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecTxTsModeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_tx *timestamp_tx_ptr;
   WP_U16* tx_timestamp_mode_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_tx_ptr = &timestamp_ptr->tx[timestamp_id];

   tx_timestamp_mode_ptr = &(timestamp_tx_ptr->mode).value;

   WUFEI_REG_SET(tx_timestamp_mode_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecTxTsModeRegGet
 *
 * Purpose: Reads the TX timestamp mode from the clock recovery TX timestamp
 *          configuration register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecTxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecTxTsModeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_tx *timestamp_tx_ptr;
   WP_U16* tx_timestamp_mode_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_tx_ptr = &timestamp_ptr->tx[timestamp_id];

   tx_timestamp_mode_ptr = &(timestamp_tx_ptr->mode).value;

   WUFEI_REG_GET(tx_timestamp_mode_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecTxTsBufferSizeRegSet
 *
 * Purpose: Writes the TX bufer_size value to the clock recovery TX timestamp
 *          configuration register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecTxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecTxTsBufferSizeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_tx *timestamp_tx_ptr;
   WP_U16* tx_timestamp_buffer_size_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_tx_ptr = &timestamp_ptr->tx[timestamp_id];

   tx_timestamp_buffer_size_ptr = &(timestamp_tx_ptr->buffer_size).value;

   WUFEI_REG_SET(tx_timestamp_buffer_size_ptr, WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_F(value));
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecTxTimestampRegGet
 *
 * Purpose: Reads the TX timestamp value from the clock recovery TX timestamp
 *          register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecTxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecTxTimestampRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U32* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_tx *timestamp_tx_ptr;
   WP_U16 *tx_timestamp_lsb_ptr, *tx_timestamp_msb_ptr;
   WP_U16 timestamp_lsb, timestamp_msb;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_tx_ptr = &timestamp_ptr->tx[timestamp_id];

   tx_timestamp_lsb_ptr = &(timestamp_tx_ptr->timestamp_lsb).value;
   tx_timestamp_msb_ptr = &(timestamp_tx_ptr->timestamp_msb).value;

   WUFEI_REG_GET(tx_timestamp_lsb_ptr, &timestamp_lsb);
   WUFEI_REG_GET(tx_timestamp_msb_ptr, &timestamp_msb);

   *value = (WP_U32)(timestamp_msb << 16);
   *value |= (WP_U32)timestamp_lsb;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecRxTimestampRegGet
 *
 * Purpose: Reads the RX timestamp value from the clock recovery TX timestamp
 *          register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecRxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecRxTimestampRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U32* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_rx *timestamp_rx_ptr;
   WP_U16 *rx_timestamp_lsb_ptr, *rx_timestamp_msb_ptr;
   WP_U16 timestamp_lsb, timestamp_msb;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_rx_ptr = &timestamp_ptr->rx[timestamp_id];

   rx_timestamp_lsb_ptr = &(timestamp_rx_ptr->timestamp_lsb).value;
   rx_timestamp_msb_ptr = &(timestamp_rx_ptr->timestamp_msb).value;

   WUFEI_REG_GET(rx_timestamp_lsb_ptr, &timestamp_lsb);
   WUFEI_REG_GET(rx_timestamp_msb_ptr, &timestamp_msb);

   *value = (WP_U32)(timestamp_msb << 16);
   *value |= (WP_U32)timestamp_lsb;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecTxTsBufferSizeRegGet
 *
 * Purpose: Reads the TX bufer_size value from the clock recovery TX timestamp
 *          configuration register
 *
 * Description:
 *
 *      ufeid - UFE id
 *      timestamp_id - clock recovery unit id (timestamp)
 *      value
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_ClockRecTxCreate() etc
 *
 ****************************************************************************/
void WUFEI_HwClockRecTxTsBufferSizeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_timestamp_tx *timestamp_tx_ptr;
   WP_U16* tx_timestamp_buffer_size_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   timestamp_tx_ptr = &timestamp_ptr->tx[timestamp_id];

   tx_timestamp_buffer_size_ptr = &(timestamp_tx_ptr->buffer_size).value;

   WUFEI_REG_GET(tx_timestamp_buffer_size_ptr, value);
   *value = WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_V(*value);
}

void WUFEI_HwClockRecRxTsBufferSizeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_ts_rx_ext_diff_buff_size *ts_buf_size_ptr;
   WP_U16* rx_timestamp_buffer_size_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   ts_buf_size_ptr = &timestamp_ptr->rx_buff_size[timestamp_id];

   rx_timestamp_buffer_size_ptr = &(ts_buf_size_ptr->ts_rx_ext_diff_buf_size).value;
   if (value == 0x0)
      WUFEI_REG_SET(rx_timestamp_buffer_size_ptr,
                    (~WUFEI_HW_CLK_REC_RX_TS_EN_M | (WUFEI_HW_CLK_REC_RX_BUFFER_SIZE_F(value))));
   else
      WUFEI_REG_SET(rx_timestamp_buffer_size_ptr,
                    (WUFEI_HW_CLK_REC_RX_TS_EN_M | (WUFEI_HW_CLK_REC_RX_BUFFER_SIZE_F(value))));
}

void WUFEI_HwClockRecRxTsBufferSizeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_timestamp *timestamp_ptr;
   WUFEI_hw_clk_rec_ts_rx_ext_diff_buff_size *ts_buf_size_ptr;
   WP_U16* rx_timestamp_buffer_size_ptr;

   timestamp_ptr = &clk_rec_base->timestamp;
   ts_buf_size_ptr = &timestamp_ptr->rx_buff_size[timestamp_id];

   rx_timestamp_buffer_size_ptr = &(ts_buf_size_ptr->ts_rx_ext_diff_buf_size).value;
   WUFEI_REG_GET(rx_timestamp_buffer_size_ptr, value);
}

void WUFEI_HwClockRecRxTsBufferEnable(WP_U32 ufeid, WP_U32 timestamp_id)
{
   WP_U16 buf_value;

   WUFEI_HwClockRecRxTsBufferSizeRegGet(ufeid, timestamp_id, &buf_value);
   buf_value |= WUFEI_HW_CLK_REC_RX_TS_EN_M;
   WUFEI_HwClockRecRxTsBufferSizeRegSet(ufeid, timestamp_id, buf_value);
}

void WUFEI_HwClockRecRxTsBufferDisable(WP_U32 ufeid, WP_U32 timestamp_id)
{
   WP_U16 buf_value;

   WUFEI_HwClockRecRxTsBufferSizeRegGet(ufeid, timestamp_id, &buf_value);
   buf_value &= ~WUFEI_HW_CLK_REC_RX_TS_EN_M;
   WUFEI_HwClockRecRxTsBufferSizeRegSet(ufeid, timestamp_id, buf_value);
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
 *      WUFEI_ClockRecConfigDivider() func.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecDcoDivisorSet(WP_U32 ufeid, WP_U32 dco_id,
                                          WP_U16 integer, WP_U32 remainder)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *integer_ptr, *remainder_msb_ptr, *remainder_lsb_ptr;

   dco_ptr = &clk_rec_base->dco[dco_id];
   integer_ptr = &(dco_ptr->integer).value;
   remainder_msb_ptr = &(dco_ptr->remainder_msb).value;
   remainder_lsb_ptr = &(dco_ptr->remainder_lsb).value;

   WUFEI_REG_SET(integer_ptr, integer);
   WUFEI_REG_SET(remainder_msb_ptr, (WP_U16)(remainder>>16));
   WUFEI_REG_SET(remainder_lsb_ptr,(WP_U16)remainder);

   return WUFE_OK;
}

void WUFEI_HwClockRecDcoDisplay(WP_U32 ufeid, WP_U32 dco_id)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *integer_ptr, *remainder_msb_ptr, *remainder_lsb_ptr;
   WP_U16 value;
   WP_U16 *mode_ptr;

   dco_ptr = &clk_rec_base->dco[dco_id];
   integer_ptr = &(dco_ptr->integer).value;
   remainder_msb_ptr = &(dco_ptr->remainder_msb).value;
   remainder_lsb_ptr = &(dco_ptr->remainder_lsb).value;

   WUFE_PRINT_DISP((wufei_log,
                    "\n\n-------------  CLOCK RECOVERY DCO DISPLAY (dco_id = %d) -----------\n",
                    dco_id));
   WUFEI_REG_GET(integer_ptr, &value);
   WUFE_PRINT_DISP((wufei_log, "Integer:      0x%04x\n", value));

   WUFEI_REG_GET(remainder_msb_ptr, &value);
   WUFE_PRINT_DISP((wufei_log, "Reminder MSB: 0x%04x\n", value));

   WUFEI_REG_GET(remainder_lsb_ptr, &value);
   WUFE_PRINT_DISP((wufei_log, "Reminder LSB: 0x%04x\n", value));

   mode_ptr = &(dco_ptr->mode).value;
   WUFEI_REG_GET(mode_ptr, &value);
   WUFE_PRINT_DISP((wufei_log, "Mode:         0x%04x\n", value));
   WUFE_PRINT_DISP((wufei_log, "\tE1T1 mode:   %d\n", WUFEI_HW_CLK_REC_DCO_MODE_E1T1_V(value)));
   WUFE_PRINT_DISP((wufei_log, "\tSYNC mode:   %d\n", WUFEI_HW_CLK_REC_DCO_MODE_SYNC_V(value)));
   WUFE_PRINT_DISP((wufei_log, "\tActive mode: %d\n", WUFEI_HW_CLK_REC_DCO_MODE_ACTIVE_V(value)));
   WUFE_PRINT_DISP((wufei_log, "\tEnable:      %d\n", WUFEI_HW_CLK_REC_DCO_MODE_EN_V(value)));
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecDcoModeSet
 *
 * Purpose: Sets the clock devisor mode E1T1 mode, active sync, pos/neg
 *
 * Description:
 *
 *      ufeid -  UFE id
 *      dco_id - DCO id
 *      e1t1_mode - E1T1 mode
 *      sync_mode - Pos/Neg sync mode
 *      active_mode - active High/Low sync mode
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
WUFE_status WUFEI_HwClockRecDcoModeSet(WP_U32 ufeid, WP_U32 dco_id,
                                       WP_U16 e1t1_mode, WP_U16 sync_mode, WP_U16 active_mode)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *mode_ptr, value;

   dco_ptr = &clk_rec_base->dco[dco_id];
   mode_ptr = &(dco_ptr->mode).value;

   value =
      WUFEI_HW_CLK_REC_DCO_MODE_E1T1_F(e1t1_mode)|
      WUFEI_HW_CLK_REC_DCO_MODE_SYNC_F(sync_mode)|
      WUFEI_HW_CLK_REC_DCO_MODE_ACTIVE_F(active_mode);

   WUFEI_REG_SET(mode_ptr,value);

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecDcoInit
 *
 * Purpose: Resets the DCO registers
 *
 * Description:
 *
 *      ufeid -  UFE id
 *      dco_id - DCO id
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
WUFE_status WUFEI_HwClockRecDcoInit(WP_U32 ufeid, WP_U32 dco_id)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *ptr;

   dco_ptr = &clk_rec_base->dco[dco_id];
   ptr = &(dco_ptr->mode).value;
   WUFEI_REG_SET(ptr, 0);
   ptr = &(dco_ptr->integer).value;
   WUFEI_REG_SET(ptr, 0);
   ptr = &(dco_ptr->remainder_msb).value;
   WUFEI_REG_SET(ptr, 0);
   ptr = &(dco_ptr->remainder_lsb).value;
   WUFEI_REG_SET(ptr, 0);

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
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_dco* dco_ptr;
   WP_U16 *mode_ptr, value;

   dco_ptr = &clk_rec_base->dco[dco_id];
   mode_ptr = &(dco_ptr->mode).value;

   WUFEI_REG_GET(mode_ptr, &value);

   if(enable)
      value |= WUFEI_HW_CLK_REC_DCO_MODE_EN_M;
   else
      value &= ~WUFEI_HW_CLK_REC_DCO_MODE_EN_M;

   WUFEI_REG_SET(mode_ptr, value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecHwConfigSetup
 *
 * Purpose: Allocates the HW clock recovery structure in the system_registry
 *          and initiates its parameters
 *
 * Description:
 *
 *      ufeid - UFE id
 *      clk_rec_hw_config - HW configuration structure
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
 *      WUFEI_UfeCoreHwConfigSetup() func.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecHwConfigSetup(WP_U32 ufeid, WUFEI_clk_rec_hw_config **hw_config,
                                          WP_U32 RxEnable, WP_U32 TxEnable)
{
   WUFEI_clk_rec_hw_config *clk_rec_hw_config;
   WP_U16 rx_ext_diff_units, rx_units, tx_ext_diff_units, tx_units, value;

   clk_rec_hw_config = (WUFEI_clk_rec_hw_config *)WUFEI_MALLOC(sizeof(WUFEI_clk_rec_hw_config));
   if(!clk_rec_hw_config)
      return WUFE_ERR_MALLOC;

   memset(clk_rec_hw_config, 0, sizeof(WUFEI_clk_rec_hw_config));

   value = WUFEI_HwClockRecGeneralCfgGet(ufeid);
   if (TxEnable)
   {
      (clk_rec_hw_config->clk_translator).mode = WUFEI_HW_CLK_REC_CLK_TRANSLATOR_EN_V(value);
      (clk_rec_hw_config->dco).mode = WUFEI_HW_CLK_REC_DCO_EN_V(value);
      (clk_rec_hw_config->clk_rec_tx).mode = WUFEI_HW_CLK_REC_TIMESTAMP_TX_EN_V(value);
   }
   if (RxEnable)
   {
      (clk_rec_hw_config->cgen).mode = WUFEI_HW_CLK_REC_CGEN_EN_V(value);
      (clk_rec_hw_config->clk_rec_rx).mode = WUFEI_HW_CLK_REC_TIMESTAMP_RX_EN_V(value);
   }

   rx_ext_diff_units = WUFEI_HwClockRecNumOfRxDiffUnitsGet(ufeid);
   rx_units = WUFEI_HwClockRecNumOfRxAdapUnitsGet(ufeid);
   tx_ext_diff_units = WUFEI_HwClockRecNumOfTxDiffUnitsGet(ufeid);
   tx_units = WUFEI_HwClockRecNumOfTxAdapUnitsGet(ufeid);
   
   if (TxEnable)
   {
      if((clk_rec_hw_config->clk_translator).mode)
      {
         (clk_rec_hw_config->clk_translator).n_units = tx_units;
         (clk_rec_hw_config->clk_translator).n_ext_diff_units = tx_ext_diff_units;
      }
      if((clk_rec_hw_config->dco).mode)
      {
         (clk_rec_hw_config->dco).n_units = tx_units;
         (clk_rec_hw_config->dco).n_ext_diff_units = tx_ext_diff_units;
      }
      
      if((clk_rec_hw_config->clk_rec_tx).mode)
      {
         (clk_rec_hw_config->clk_rec_tx).n_units = tx_units;
         (clk_rec_hw_config->clk_rec_tx).n_ext_diff_units = tx_ext_diff_units;
      }
   }
   if (RxEnable)
   {
      if((clk_rec_hw_config->cgen).mode)
      {
         (clk_rec_hw_config->cgen).n_units = rx_units;
         (clk_rec_hw_config->cgen).n_ext_diff_units = rx_ext_diff_units;
      }
      if((clk_rec_hw_config->clk_rec_rx).mode)
      {
         (clk_rec_hw_config->clk_rec_rx).n_units = rx_units;
         (clk_rec_hw_config->clk_rec_rx).n_ext_diff_units = rx_ext_diff_units;
      }
   }

   *hw_config = clk_rec_hw_config;

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecHwConfigDelete
 *
 * Purpose: Release memory allocated for clock recovery hw config
 *
 * Description:
 *      clk_rec_hw_config - HW configuration structure
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned, an error code otherwise.
 *
 * Called by:
 *      WUFEI_UfeCoreHwConfigDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_HwClockRecHwConfigDelete(WUFEI_clk_rec_hw_config *clk_rec_hw_config)
{
   if (clk_rec_hw_config)
      WUFEI_FREE(clk_rec_hw_config);

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecHwConfigDisplay
 *
 * Purpose: Display the clock recovery HW configuration
 *
 * Description:
 *
 *      ufeid - UFE id
 *      clk_rec_hw_config - HW configuration structure
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *      None
 *
 * Called by:
 *      WUFEI_UfeCoreHwConfigDisplay() func.
 *
 ****************************************************************************/
void WUFEI_HwClockRecHwConfigDisplay(WP_U32 ufeid, WUFEI_clk_rec_hw_config *clk_rec_hw_config)
{
   ((void)ufeid);   
   WUFE_PRINT_DISP((wufei_log,
                    "Clock Translator:  mode 0x%04x   n_units  0x%04x  n_ext_diff_units 0x%04x\n",
                    (clk_rec_hw_config->clk_translator).mode,
                    (clk_rec_hw_config->clk_translator).n_units,
                    (clk_rec_hw_config->clk_translator).n_ext_diff_units));
   WUFE_PRINT_DISP((wufei_log,"DCO:     mode 0x%04x   n_units  0x%04x  n_ext_diff_units 0x%04x\n",
                    (clk_rec_hw_config->dco).mode,
                    (clk_rec_hw_config->dco).n_units,
                    (clk_rec_hw_config->dco).n_ext_diff_units));
   WUFE_PRINT_DISP((wufei_log,"CGEN:    mode 0x%04x   n_units  0x%04x  n_ext_diff_units 0x%04x\n",
                    (clk_rec_hw_config->cgen).mode,
                    (clk_rec_hw_config->cgen).n_units,
                    (clk_rec_hw_config->cgen).n_ext_diff_units));
   WUFE_PRINT_DISP((wufei_log,"CR TX:  mode 0x%04x   n_units  0x%04x  n_ext_diff_units 0x%04x\n",
                    (clk_rec_hw_config->clk_rec_tx).mode,
                    (clk_rec_hw_config->clk_rec_tx).n_units,
                    (clk_rec_hw_config->clk_rec_tx).n_ext_diff_units));
   WUFE_PRINT_DISP((wufei_log,"CR RX:  mode 0x%04x   n_units  0x%04x  n_ext_diff_units 0x%04x\n",
                    (clk_rec_hw_config->clk_rec_rx).mode,
                    (clk_rec_hw_config->clk_rec_rx).n_units,
                    (clk_rec_hw_config->clk_rec_rx).n_ext_diff_units));

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecInclkMuxSelDisplay
 *
 * Purpose: Display the clock recovery mux configuration
 *
 * Description:
 *
 *      ufeid - UFE id
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *      None
 *
 * Called by:
 *      WUFEI_UfeCoreHwConfigDisplay() func.
 *
 ****************************************************************************/
void WUFEI_HwClockRecInclkMuxSelDisplay(WP_U32 ufeid)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_mux *mux_ptr = &clk_rec_base->mux;
   WP_U16* reg_ptr, value, div_value, speid;

   for(speid = 0; speid < WUFEI_HW_CLK_REC_MAX_N_CLK_MUX; speid ++)
   {
      reg_ptr = &(mux_ptr->clk_mux[speid]).value;
      WUFEI_REG_GET(reg_ptr, &value);

      reg_ptr = &(mux_ptr->dco_mux[speid]).value;
      WUFEI_REG_GET(reg_ptr, &div_value);

      WUFE_PRINT_DISP((wufei_log,"SPE %d: clk_mux 0x%x, dco_mux 0x%x ",
                       speid, value, div_value));

      if( WUFEI_HW_CLK_REC_MUX_INCLK_SEL_V(value))
      {
         WUFE_PRINT_DISP((wufei_log,"TX serial clock from Jitter Attenuator, "));
         if(WUFEI_HW_CLK_REC_MUX_MODE_V(value))
            WUFE_PRINT_DISP((wufei_log,"E3, "));
         else
            WUFE_PRINT_DISP((wufei_log,"DS3, "));
         if(WUFEI_HW_CLK_REC_MUX_MODE_FRAMED_V(value))
            WUFE_PRINT_DISP((wufei_log,"framed\n"));
         else
            WUFE_PRINT_DISP((wufei_log,"unframed\n"));
      }
      else
         WUFE_PRINT_DISP((wufei_log,"TX serial clock from DCO\n"));

   }
}

void WUFEI_HwClockRecRegsDisplay(WUFEI_system_registry *system_registry)
{
   WP_U16 value;
   WP_U16 ufeid = system_registry->wufe_id;

   WUFEI_HwClockRecTsGenConfigRegGet(ufeid, &value);
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery General Config Register: 0x%04x\n", value));
   WUFE_PRINT_DISP((wufei_log,"\tClock Recovery method: %s\n",
                    (WUFEI_HW_CLK_REC_METHOD_V(value) == 1) ? "DIFFERENTIAL" : "ADAPTIVE"));

   value = WUFEI_HwClockRecGeneralCfgGet(ufeid);
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery BLOCKS Register:         0x%04x\n", value));

   value = WUFEI_HwClockRecNumOfTxAdapUnitsGet(ufeid);
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery TX_UNITS Register:       0x%04x\n",value));

   value = WUFEI_HwClockRecNumOfTxDiffUnitsGet(ufeid);
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery TX_EXT_DIFF_UNITS Register:       0x%04x\n",value));

   value = WUFEI_HwClockRecNumOfRxAdapUnitsGet(ufeid);
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery RX_UNITS Register:       0x%04x\n",value));

   value = WUFEI_HwClockRecNumOfRxDiffUnitsGet(ufeid);
   WUFE_PRINT_DISP((wufei_log,"Clock Recovery RX_EXT_DIFF_UNITS Register:       0x%04x\n",value));
}

WUFE_status WUFEI_HwClockRecDisplay(WUFEI_system_registry *system_registry)
{
   /* Display HW clock recovery information */
   WUFE_PRINT_DISP((wufei_log,
                    "\n\n------ Clock Recovery FPGA Configuration ------\n"));
   WUFEI_HwClockRecRegsDisplay(system_registry);

   /* Display CR HW information saved within the UFE registry */
   if(system_registry->hw_config.clk_rec_hw_config)
   {
      WUFE_PRINT_DISP((wufei_log,
                       "\n\n------ Clock Recovery HW Configuration read by driver ------\n"));
      WUFEI_HwClockRecHwConfigDisplay(system_registry->wufe_id,
                                      system_registry->hw_config.clk_rec_hw_config);
   }
   else
      WUFE_PRINT_DISP((wufei_log,"\nClock Recovery HW Config Registry not defined\n"));

   return WUFE_OK;
}

