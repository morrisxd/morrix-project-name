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

#ifndef WUFEI_HW_FM_H
#include "wufei_hw_fm.h"
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

static void WUFEI_HwFmIdirectAddrSet(WP_U32 ufeid, WP_U32 coreid, WP_U32 indirect_addr);
static WUFE_status WUFEI_HwFmIdirectDataSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value);
static WUFE_status WUFEI_HwFmIdirectDataGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value);
static WUFE_status WUFEI_HwFmIdirectMemset(WP_U32 ufeid, WP_U32 coreid, WP_U32 addr, WP_U16 value, WP_U32 size);

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmIdirectAddrSet
 *
 * Purpose: Indirect address set for the FM
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
 *     write to FM indirect address register
 *
 * Return Value: none
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwFmIdirectAddrSet(WP_U32 ufeid, WP_U32 coreid, WP_U32 indirect_addr)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_low.value;
   indha_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_high.value;

   WUFEI_REG_SET(indla_ptr, indirect_addr & 0xffff);
   WUFEI_REG_SET(indha_ptr, (indirect_addr >> 16) & 0xffff);

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmIdirectDataSet
 *
 * Purpose: Indirect data set for the FM
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
 *     write to FM indirect data register
 *
 * Return Value: None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwFmIdirectDataSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_cmd.value;

   WUFEI_REG_SET(inddat_ptr, value);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;

         WUFE_PRINT_ERR((wufei_log, "ERROR: Timeout while trying to set data %x in FM indirect register\n", value));
         ind_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", value));
         ind_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_high.value;
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
 * Function:  WUFEI_HwFmIdirectDataGet
 *
 * Purpose: Indirect data get for the FM
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
 *     get the FM indirect data
 *
 * Return Value:None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwFmIdirectDataGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_cmd.value;

   WUFEI_REG_SET(indcmd_ptr, WUFEI_INDIRECT_RD_CMD);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;
         WP_U16 tmp_value;

         WUFE_PRINT_ERR((wufei_log, "ERROR: Timeout while trying to read data from FM indirect register\n"));
         ind_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&tmp_value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", tmp_value));
         ind_ptr = &((WUFEI_hw_fm*)fm_base)->fm_ind_high.value;
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
 * Function:  WUFEI_HwFmIdirectMemset
 *
 * Purpose: Continuos Indirect memory set for the FM
 *
 *
 * Description:
 *
 *      ufeid - the UFE id
 *      addr - start address
 *      value - value to set
 *      size - memset size
 * Inputs:
 *
 *
 * Outputs:
 *     write to FM indirect registers
 *
 * Return Value: None
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwFmIdirectMemset(WP_U32 ufeid, WP_U32 coreid, WP_U32 addr, WP_U16 value, WP_U32 size)
{
   WP_U32 i;
   WUFE_status status;

   WUFEI_HwFmIdirectAddrSet(ufeid, coreid, addr);
   for(i = 0; i<size; i++)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectDataSet(ufeid, coreid, value));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmEnable
 *
 * Purpose: Enables the Fifo Manager
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
 *     write to FM mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_SystemFmEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmEnable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 direction)
{
   WP_U16 mode_value, *mode_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid,coreid);

   mode_ptr = &((WUFEI_hw_fm*)fm_base)->fm_mode_reg.value;
   WUFEI_REG_GET(mode_ptr, &mode_value);

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      mode_value |= WUFEI_FM_RX_MODE_ENABLE_M;
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      mode_value |= WUFEI_FM_TX_MODE_ENABLE_M;
   }

   WUFEI_REG_SET(mode_ptr, mode_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmDisable
 *
 * Purpose: Disables the Fifo Manager
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
 *     write to FM mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_SystemFmDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmDisable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 direction)
{
   WP_U16 mode_value, *mode_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);

   mode_ptr = &((WUFEI_hw_fm*)fm_base)->fm_mode_reg.value;
   WUFEI_REG_GET(mode_ptr, &mode_value);

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      mode_value &= ~WUFEI_FM_RX_MODE_ENABLE_M;
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      mode_value &= ~WUFEI_FM_TX_MODE_ENABLE_M;
   }

   WUFEI_REG_SET(mode_ptr, mode_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmLoopbackEnable
 *
 * Purpose: Enables the Fifo Manager line loopback
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to FM mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    WUFE_SystemCreate/Reconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmLoopbackEnable(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   WP_U16 mode_value, *mode_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid,coreid);

   mode_ptr = &((WUFEI_hw_fm*)fm_base)->fm_mode_reg.value;
   WUFEI_REG_GET(mode_ptr, &mode_value);

   mode_value |= WUFEI_FM_LINE_LOOPBACK_M;

   WUFEI_REG_SET(mode_ptr, mode_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmLoopbackDisable
 *
 * Purpose: Disables the Fifo Manager line loopback
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to FM mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    WUFE_SystemCreate/Reconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmLoopbackDisable(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   WP_U16 mode_value, *mode_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);

   mode_ptr = &((WUFEI_hw_fm*)fm_base)->fm_mode_reg.value;
   WUFEI_REG_GET(mode_ptr, &mode_value);

   mode_value &= ~WUFEI_FM_LINE_LOOPBACK_M;

   WUFEI_REG_SET(mode_ptr, mode_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmLoopbackReconfigure
 *
 * Purpose: Reconfigures the Fifo Manager line loopback
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to FM mode register
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    WUFE_SystemCreate/Reconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmLoopbackReconfigure(WUFEI_system_registry* system_registry, WUFE_fifo_manager *config)
{
   WP_U32 coreid;
   WUFE_status status=0;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         if (config->loopback == WUFE_FM_LOOPBACK_ENABLE)
         {
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwFmLoopbackEnable(system_registry, coreid));
         }
         else
         {
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwFmLoopbackDisable(system_registry, coreid));
         }
      }
   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmSetPriorityTables
 *
 * Purpose: Sets the Fifo Manager's priority tables
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to FM priority tables registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmSetPriorityTables(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   WP_U16 i, priority_value;
   WUFEI_hw_fifo_pt *priority_table_ptr;
   WP_U16 *value_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid,coreid);

   for (i =0; i<WUFEI_MAX_NUM_PRIORITY_TABLE;i++)
   {
      /*Set priority values for Rx*/
      priority_table_ptr = &((WUFEI_hw_fm*)fm_base)->fm_rx_priority_table[i];

      /*Rx low priority*/
      value_ptr = &priority_table_ptr->priority_table_param_low.value;
      priority_value = (WUFEI_FM_PRIORITY_FILL_LVL_0_F(WUFEI_FM_RX_PRIORITY_LVL_0) |
                        WUFEI_FM_PRIORITY_FILL_LVL_1_F(WUFEI_FM_RX_PRIORITY_LVL_1) |
                        WUFEI_FM_PRIORITY_FILL_LVL_2_F(WUFEI_FM_RX_PRIORITY_LVL_2) |
                        WUFEI_FM_PRIORITY_FILL_LVL_3_F(WUFEI_FM_RX_PRIORITY_LVL_3));
      WUFEI_REG_SET(value_ptr, priority_value);

      /*Rx high priority*/
      value_ptr = &priority_table_ptr->priority_table_param_high.value;
      priority_value = (WUFEI_FM_PRIORITY_FILL_LVL_4_F(WUFEI_FM_RX_PRIORITY_LVL_4) |
                        WUFEI_FM_PRIORITY_FILL_LVL_5_F(WUFEI_FM_RX_PRIORITY_LVL_5) |
                        WUFEI_FM_PRIORITY_FILL_LVL_6_F(WUFEI_FM_RX_PRIORITY_LVL_6) |
                        WUFEI_FM_PRIORITY_FILL_LVL_7_F(WUFEI_FM_RX_PRIORITY_LVL_7));
      WUFEI_REG_SET(value_ptr, priority_value);


      if (i==1)
      {/*HDLC Tx Priority tables*/
         /*Set priority values for Tx*/
         priority_table_ptr = &((WUFEI_hw_fm*)fm_base)->fm_tx_priority_table[i];
         
         /*Tx low priority*/
         value_ptr = &priority_table_ptr->priority_table_param_low.value;
         priority_value = (WUFEI_FM_PRIORITY_FILL_LVL_0_F(WUFEI_FM_TX_PRIORITY_1_LVL_0) |
                           WUFEI_FM_PRIORITY_FILL_LVL_1_F(WUFEI_FM_TX_PRIORITY_1_LVL_1) |
                           WUFEI_FM_PRIORITY_FILL_LVL_2_F(WUFEI_FM_TX_PRIORITY_1_LVL_2) |
                           WUFEI_FM_PRIORITY_FILL_LVL_3_F(WUFEI_FM_TX_PRIORITY_1_LVL_3));
         
         WUFEI_REG_SET(value_ptr, priority_value);
         
         /*Tx high priority*/
         value_ptr = &priority_table_ptr->priority_table_param_high.value;
         priority_value = (WUFEI_FM_PRIORITY_FILL_LVL_4_F(WUFEI_FM_TX_PRIORITY_1_LVL_4) |
                           WUFEI_FM_PRIORITY_FILL_LVL_5_F(WUFEI_FM_TX_PRIORITY_1_LVL_5) |
                           WUFEI_FM_PRIORITY_FILL_LVL_6_F(WUFEI_FM_TX_PRIORITY_1_LVL_6) |
                           WUFEI_FM_PRIORITY_FILL_LVL_7_F(WUFEI_FM_TX_PRIORITY_1_LVL_7));
         
         WUFEI_REG_SET(value_ptr, priority_value);
      }
      else
      {/*Not HDLC*/
         /*Set priority values for Tx*/
         priority_table_ptr = &((WUFEI_hw_fm*)fm_base)->fm_tx_priority_table[i];
         
         /*Tx low priority*/
         value_ptr = &priority_table_ptr->priority_table_param_low.value;
         priority_value = (WUFEI_FM_PRIORITY_FILL_LVL_0_F(WUFEI_FM_TX_PRIORITY_0_LVL_0) |
                           WUFEI_FM_PRIORITY_FILL_LVL_1_F(WUFEI_FM_TX_PRIORITY_0_LVL_1) |
                           WUFEI_FM_PRIORITY_FILL_LVL_2_F(WUFEI_FM_TX_PRIORITY_0_LVL_2) |
                           WUFEI_FM_PRIORITY_FILL_LVL_3_F(WUFEI_FM_TX_PRIORITY_0_LVL_3));
         
         WUFEI_REG_SET(value_ptr, priority_value);
         
         /*Tx high priority*/
         value_ptr = &priority_table_ptr->priority_table_param_high.value;
         priority_value = (WUFEI_FM_PRIORITY_FILL_LVL_4_F(WUFEI_FM_TX_PRIORITY_0_LVL_4) |
                           WUFEI_FM_PRIORITY_FILL_LVL_5_F(WUFEI_FM_TX_PRIORITY_0_LVL_5) |
                           WUFEI_FM_PRIORITY_FILL_LVL_6_F(WUFEI_FM_TX_PRIORITY_0_LVL_6) |
                           WUFEI_FM_PRIORITY_FILL_LVL_7_F(WUFEI_FM_TX_PRIORITY_0_LVL_7));
         
         WUFEI_REG_SET(value_ptr, priority_value);
      }
   }
   
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmIndirectSetup
 *
 * Purpose: resets the FM memory and configurations in the indirect
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to FM indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_SystemFmSetup
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmIndirectSetup(WUFEI_system_registry* system_registry, WP_U32 coreid)
{

   WP_U32 id = system_registry->wufe_id;
   WUFE_status status;

   /*********************************/
   /*** clear FM port configuration */
   /*********************************/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwFmIdirectMemset(id, coreid,
                                                 (WP_U32)WUFEI_INDIR_FM_RX_PORT_CONF(0),
                                                 0x0000,
                                                 WUFEI_INDIR_FM_RX_PORT_CONF_SIZE));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwFmIdirectMemset(id, coreid,
                                                 (WP_U32)WUFEI_INDIR_FM_TX_PORT_CONF(0),
                                                 0x0000,
                                                 WUFEI_INDIR_FM_TX_PORT_CONF_SIZE));

   /********************************/
   /*** clear FM port DDR memory   */
   /********************************/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwFmIdirectMemset(id, coreid,
                                                 (WP_U32)WUFEI_INDIR_FM_RX_PORT_DDR_MEM(0),
                                                 0x0000,
                                                 WUFEI_INDIR_FM_RX_PORT_DDR_MEM_SIZE));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwFmIdirectMemset(id, coreid,
                                                 (WP_U32)WUFEI_INDIR_FM_TX_PORT_DDR_MEM(0),
                                                 0x0000,
                                                 WUFEI_INDIR_FM_TX_PORT_DDR_MEM_SIZE));

   /*********************************/
   /*** clear FM port burst memory  */
   /*********************************/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwFmIdirectMemset(id, coreid,
                                                 (WP_U32)WUFEI_INDIR_FM_RX_PORT_BURST_MEM(0),
                                                 0x0000,
                                                 WUFEI_INDIR_FM_RX_PORT_BURST_MEM_SIZE));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwFmIdirectMemset(id, coreid,
                                                 (WP_U32)WUFEI_INDIR_FM_TX_PORT_BURST_MEM(0),
                                                 0x0000,
                                                 WUFEI_INDIR_FM_TX_PORT_BURST_MEM_SIZE));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmAssignRxBankNum
 *
 * Purpose: Assigns the Rx bank number for a specific phy type
 *          according to the registry.
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              The core id of this fm
 *     phy_type            The phy type
 *     num_of_slots        the amount of slots for a T1 or E1
 *
 * Outputs:
 *     finds the relevant bank number
 *
 * Return Value:
 *     Upon successful completion, the bank number is returned.
 *
 * Called by:
 *     WUFEI_HwFmPhyCreate
 *
 ****************************************************************************/
WP_U16 WUFEI_HwFmAssignRxBankNum(WUFEI_system_registry *system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_type,
                                 WP_U32 num_of_slots)
{
   WP_U16 i, bank_num =0;
   WP_U32 min_bank_value;
   WP_U8 max_num_of_banks = system_registry->hw_config.num_of_banks;

   /*Find the most available bank*/
   /*Start with the first bank, and go all over the banks for RX*/
   min_bank_value = system_registry->fm_bank_status[coreid][0];

   for (i=1;i<max_num_of_banks/2;i++)
   {
      if (system_registry->fm_bank_status[coreid][i] < min_bank_value)
      {
         bank_num = i;
         min_bank_value = system_registry->fm_bank_status[coreid][i];
      }
   }

   /*Add the bank value according to the phy type*/
   switch (phy_type)
   {
      case WUFE_SDH_TYPE_T1:
      case WUFE_SONET_TYPE_T1:
      case WUFE_CAD_TYPE_T1:
      case WUFE_SDH_TYPE_E1:
      case WUFE_SONET_TYPE_E1:
      case WUFE_CAD_TYPE_E1:
      case WUFE_DCC_TYPE_MS:
      case WUFE_DCC_TYPE_RS:
         min_bank_value += WUFEI_FM_BANK_DS0_WEIGHT * num_of_slots;
         break;
      case WUFE_SDH_TYPE_E3:
      case WUFE_SONET_TYPE_E3:
         min_bank_value += WUFEI_FM_BANK_E3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_DS3:
      case WUFE_SONET_TYPE_DS3:
         min_bank_value += WUFEI_FM_BANK_DS3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC11:
      case WUFE_SONET_TYPE_VT1_5:
         min_bank_value += WUFEI_FM_BANK_VC11_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC12:
      case WUFE_SONET_TYPE_VT2:
         min_bank_value += WUFEI_FM_BANK_VC12_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC3:
      case WUFE_SONET_TYPE_STS1_SPE:
         min_bank_value += WUFEI_FM_BANK_VC3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC4:
      case WUFE_SONET_TYPE_STS3:
         min_bank_value += WUFEI_FM_BANK_VC4_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC44C:
      case WUFE_SONET_TYPE_STS12:
         min_bank_value += WUFEI_FM_BANK_VC44C_WEIGHT;
         break;
      default:
         return max_num_of_banks;
   }

   /*Save value in registry*/
   system_registry->fm_bank_status[coreid][bank_num] = min_bank_value;

   return bank_num;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmAssignTxBankNum
 *
 * Purpose: Assigns the Tx bank number for a specific phy type
 *          according to the registry.
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              The core id of this fm
 *     phy_type            The phy type
 *     num_of_slots        the amount of slots for a T1 or E1
 *
 * Outputs:
 *     finds the relevant bank number
 *
 * Return Value:
 *     Upon successful completion, the bank number is returned.
 *
 * Called by:
 *     WUFEI_HwFmPhyCreate
 *
 ****************************************************************************/
WP_U16 WUFEI_HwFmAssignTxBankNum(WUFEI_system_registry *system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_type,
                                 WP_U32 num_of_slots)
{
   WP_U16 i, bank_num;
   WP_U32 min_bank_value;
   WP_U8 max_num_of_banks = system_registry->hw_config.num_of_banks;

   /*Find the most available bank*/
   /*Start with the first bank, and go all over the banks for TX*/
   bank_num = max_num_of_banks/2;
   min_bank_value = system_registry->fm_bank_status[coreid][max_num_of_banks/2];

   for (i=max_num_of_banks/2 +1;i<max_num_of_banks;i++)
   {
      if (system_registry->fm_bank_status[coreid][i] < min_bank_value)
      {
         bank_num = i;
         min_bank_value = system_registry->fm_bank_status[coreid][i];
      }
   }

   /*Add the bank value according to the phy type*/
   switch (phy_type)
   {
      case WUFE_SDH_TYPE_T1:
      case WUFE_SONET_TYPE_T1:
      case WUFE_CAD_TYPE_T1:
      case WUFE_SDH_TYPE_E1:
      case WUFE_SONET_TYPE_E1:
      case WUFE_CAD_TYPE_E1:
      case WUFE_DCC_TYPE_MS:
      case WUFE_DCC_TYPE_RS:
         min_bank_value += WUFEI_FM_BANK_DS0_WEIGHT * num_of_slots;
         break;
      case WUFE_SDH_TYPE_E3:
      case WUFE_SONET_TYPE_E3:
         min_bank_value += WUFEI_FM_BANK_E3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_DS3:
      case WUFE_SONET_TYPE_DS3:
         min_bank_value += WUFEI_FM_BANK_DS3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC11:
      case WUFE_SONET_TYPE_VT1_5:
         min_bank_value += WUFEI_FM_BANK_VC11_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC12:
      case WUFE_SONET_TYPE_VT2:
         min_bank_value += WUFEI_FM_BANK_VC12_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC3:
      case WUFE_SONET_TYPE_STS1_SPE:
         min_bank_value += WUFEI_FM_BANK_VC3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC4:
      case WUFE_SONET_TYPE_STS3:
         min_bank_value += WUFEI_FM_BANK_VC4_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC44C:
      case WUFE_SONET_TYPE_STS12:
         min_bank_value += WUFEI_FM_BANK_VC44C_WEIGHT;
         break;

      default:
         return max_num_of_banks;
   }

   /*Save value in registry*/
   system_registry->fm_bank_status[coreid][bank_num] = min_bank_value;

   return bank_num;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmRemoveBankValue
 *
 * Purpose: Removes the value of the specific phy type from the bank registry
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              The core id of this fm
 *     bank_num            The bank to update
 *     phy_type            The phy type
 *     num_of_slots        the amount of slots for a T1 or E1
 *
 * Outputs:
 *     updates the relevant bank number in the registry
 *
 * Return Value:
 *
 * Called by:
 *     WUFEI_HwFmDeletePhy
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmRemoveBankValue(WUFEI_system_registry *system_registry,
                                      WP_U32 coreid,
                                      WP_U32 bank_num,
                                      WP_U32 phy_type,
                                      WP_U32 num_of_slots)
{
   WP_U32 bank_value;

   /*Find the bank value according to the phy type*/
   switch (phy_type)
   {
      case WUFE_SDH_TYPE_T1:
      case WUFE_SONET_TYPE_T1:
      case WUFE_CAD_TYPE_T1:
      case WUFE_SDH_TYPE_E1:
      case WUFE_SONET_TYPE_E1:
      case WUFE_CAD_TYPE_E1:
      case WUFE_DCC_TYPE_MS:
      case WUFE_DCC_TYPE_RS:
         bank_value = WUFEI_FM_BANK_DS0_WEIGHT * num_of_slots;
         break;
      case WUFE_SDH_TYPE_E3:
      case WUFE_SONET_TYPE_E3:
         bank_value = WUFEI_FM_BANK_E3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_DS3:
      case WUFE_SONET_TYPE_DS3:
         bank_value = WUFEI_FM_BANK_DS3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC11:
      case WUFE_SONET_TYPE_VT1_5:
         bank_value = WUFEI_FM_BANK_VC11_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC12:
      case WUFE_SONET_TYPE_VT2:
         bank_value = WUFEI_FM_BANK_VC12_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC3:
      case WUFE_SONET_TYPE_STS1_SPE:
         bank_value = WUFEI_FM_BANK_VC3_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC4:
      case WUFE_SONET_TYPE_STS3:
         bank_value = WUFEI_FM_BANK_VC4_WEIGHT;
         break;
      case WUFE_SDH_TYPE_VC44C:
      case WUFE_SONET_TYPE_STS12:
         bank_value = WUFEI_FM_BANK_VC44C_WEIGHT;
         break;

      default:
         return WUFE_ERR_PHY_INVALID_TYPE;
   }

   /*Remove value from registry*/
   system_registry->fm_bank_status[coreid][bank_num] -= bank_value;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmPhyCreate
 *
 * Purpose: Sets the most avilable bank for a specific phy
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     phy_registry        The registry of the phy to update
 *     phy_type            The type of the phy
 *     direction           The direction in which to set
 *
 * Outputs:
 *     write to FM indirect registers and phy registry
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFE_PhyCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmPhyCreate(WUFEI_system_registry *system_registry,
                                WP_U32 core_id,
                                WUFEI_phy_registry *phy_registry,
                                WP_U32 phy_type,
                                WP_U8 framed,
                                WP_U32 direction)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U32 phy_id = phy_registry->emphy_port_id;
   WP_U16 reg_value, bank_num;
   WP_U32 num_of_slots, max_buffers, priority_table;
   WP_U8 max_num_of_banks = system_registry->hw_config.num_of_banks;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      num_of_slots = phy_registry->cfg.rx_slot_group.n_slots;
      if (framed == WUFE_UNFRAMED)
      {/*In unframed mode the number of slots is 1, */
         /*but we need all the slots of each type, for the calculation of the weight*/
         if (phy_type == WUFE_SDH_TYPE_T1 ||
             phy_type == WUFE_SONET_TYPE_T1 ||
             phy_type == WUFE_CAD_TYPE_T1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_T1;
         else if (phy_type == WUFE_SDH_TYPE_E1 ||
                  phy_type == WUFE_SONET_TYPE_E1 ||
                  phy_type == WUFE_CAD_TYPE_E1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_E1;
         else if (phy_type == WUFE_DCC_TYPE_MS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_MS;
         else if (phy_type == WUFE_DCC_TYPE_RS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_RS;
      }


      /*Find the most available bank for this phy*/
      bank_num = WUFEI_HwFmAssignRxBankNum(system_registry, core_id, phy_type, num_of_slots);
      if (bank_num == max_num_of_banks)
         return WUFE_ERR_PHY_INVALID_TYPE;
      /*Update in the phy registry*/
      phy_registry->rx_bank_number = bank_num;
      /*Update the bank num and phy params in the phy's indirect port configuration register*/
      WUFEI_HwFmIdirectAddrSet(id, core_id, (WP_U32)WUFEI_INDIR_FM_RX_PORT_CONF(phy_id));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectDataGet(id, core_id, &reg_value));
      reg_value |= (WUFEI_FM_RX_PORT_CONF_BANK_NUM_F(bank_num) |
                    WUFEI_FM_RX_PORT_CONF_PRIORITY_F(WUFEI_FM_PORT_CONF_PRIORITY) |
                    WUFEI_FM_RX_PORT_CONF_TRANSMIT_THRESHOLD_F(phy_registry->cfg.rx_fifo_param.transmit_th));
      WUFEI_HwFmIdirectAddrSet(id, core_id, (WP_U32)WUFEI_INDIR_FM_RX_PORT_CONF(phy_id));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectDataSet(id, core_id, reg_value));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      if (phy_registry->cfg.struct_id == WUFE_STRUCT_PHY_HDLC)
      {
#if 0
         if (phy_registry->cfg.tx_fifo_param.wait_type != WUFE_PHY_TX_WAIT_TYPE_D)
         {
            WUFE_PRINT_ERR((wufei_log,"Wait type for HDLC must be WUFE_PHY_TX_WAIT_TYPE_D\n"));
            return WUFE_ERR_PHY_INVALID_TYPE;
         }
#endif
         
         max_buffers = WUFEI_FM_PORT_HDLC_CONF_MAX_BUFFER;
         priority_table = WUFEI_FM_PORT_HDLC_CONF_PRIORITY;
      }
      else
      {
         max_buffers = WUFEI_FM_PORT_CONF_MAX_BUFFER;
         priority_table = WUFEI_FM_PORT_CONF_PRIORITY;
      }
      
      
      num_of_slots = phy_registry->cfg.tx_slot_group.n_slots;
      if (framed == WUFE_UNFRAMED)
      {/*In unframed mode the number of slots is 1, */
         /*but we need all the slots of each type, for the calculation of the weight*/
         if (phy_type == WUFE_SDH_TYPE_T1 ||
             phy_type == WUFE_SONET_TYPE_T1 ||
             phy_type == WUFE_CAD_TYPE_T1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_T1;
         else if (phy_type == WUFE_SDH_TYPE_E1 ||
                  phy_type == WUFE_SONET_TYPE_E1 ||
                  phy_type == WUFE_CAD_TYPE_E1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_E1;
         else if (phy_type == WUFE_DCC_TYPE_MS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_MS;
         else if (phy_type == WUFE_DCC_TYPE_RS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_RS;
      }
      
      /*Find the most available bank for this phy*/
      bank_num = WUFEI_HwFmAssignTxBankNum(system_registry, core_id, phy_type, num_of_slots);
      if (bank_num == max_num_of_banks)
         return WUFE_ERR_PHY_INVALID_TYPE;
      /*Update in the phy registry*/
      phy_registry->tx_bank_number = bank_num;
      /*Update in the phy's indirect port configuration register*/
      WUFEI_HwFmIdirectAddrSet(id, core_id, (WP_U32)WUFEI_INDIR_FM_TX_PORT_CONF(phy_id*2));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectDataGet(id, core_id, &reg_value));
      reg_value |= (WUFEI_FM_TX_PORT_CONF_BANK_NUM_F(bank_num)|
                    WUFEI_FM_TX_PORT_CONF_PRIORITY_F(priority_table)|
                    WUFEI_FM_TX_PORT_CONF_FAST_PORT_F(phy_registry->cfg.tx_fifo_param.fast_mode));
      WUFEI_HwFmIdirectAddrSet(id, core_id, (WP_U32)WUFEI_INDIR_FM_TX_PORT_CONF(phy_id*2));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectDataSet(id, core_id, reg_value));
      reg_value = (WUFEI_FM_TX_PORT_CONF_TRANSMIT_THRESHOLD_F(phy_registry->cfg.tx_fifo_param.transmit_th) |
                   WUFEI_FM_TX_PORT_CONF_MAX_BUF_F(max_buffers) |
                   WUFEI_FM_TX_PORT_CONF_WAIT_TYPE_F(phy_registry->cfg.tx_fifo_param.wait_type));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectDataSet(id, core_id, reg_value));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmDeletePhy
 *
 * Purpose: Deletes the fifo manager entries for a specific phy
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     phy_registry        The registry of the phy to update
 *     phy_type            The type of the phy to delete
 *     direction           The direction which to delete
 *
 * Outputs:
 *     write to FM indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFE_PhyDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmDeletePhy(WUFEI_system_registry *system_registry,
                                WP_U32 coreid,
                                WUFEI_phy_registry   *phy_registry,
                                WP_U32 phy_type,
                                WP_U8 framed,
                                WP_U32 direction)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U32 phy_id = phy_registry->emphy_port_id;
   WP_U32 num_of_slots;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {/*Remove the bank value from the registry*/
      num_of_slots = phy_registry->cfg.rx_slot_group.n_slots;
      if (framed == WUFE_UNFRAMED)
      {/*In unframed mode the number of slots is 1, */
         /*but we need all the slots of each type, for the calculation of the weight*/
         if (phy_type == WUFE_SDH_TYPE_T1 ||
             phy_type == WUFE_SONET_TYPE_T1 ||
             phy_type == WUFE_CAD_TYPE_T1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_T1;
         else if (phy_type == WUFE_SDH_TYPE_E1 ||
                  phy_type == WUFE_SONET_TYPE_E1 ||
                  phy_type == WUFE_CAD_TYPE_E1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_E1;
         else if (phy_type == WUFE_DCC_TYPE_MS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_MS;
         else if (phy_type == WUFE_DCC_TYPE_RS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_RS;
      }

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmRemoveBankValue(system_registry,
                                                      coreid,
                                                      phy_registry->rx_bank_number,
                                                      phy_type,
                                                      num_of_slots));

      /*Clear indirect registers*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectMemset(id, coreid,
                                                    (WP_U32)WUFEI_INDIR_FM_RX_PORT_CONF(phy_id),
                                                    0x0000,
                                                    1));

      /*This RAM is 2 registers per port*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectMemset(id, coreid,
                                                    (WP_U32)WUFEI_INDIR_FM_RX_PORT_DDR_MEM(phy_id),
                                                    0x0000,
                                                    2));

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectMemset(id, coreid,
                                                    (WP_U32)WUFEI_INDIR_FM_RX_PORT_BURST_MEM(phy_id),
                                                    0x0000,
                                                    1));
   }

   if (direction & WUFE_TX_DIRECTION)
   {/*Remove the bank value from the registry*/
      num_of_slots = phy_registry->cfg.tx_slot_group.n_slots;
      if (framed == WUFE_UNFRAMED)
      {/*In unframed mode the number of slots is 1, */
         /*but we need all the slots of each type, for the calculation of the weight*/
         if (phy_type == WUFE_SDH_TYPE_T1 ||
             phy_type == WUFE_SONET_TYPE_T1 ||
             phy_type == WUFE_CAD_TYPE_T1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_T1;
         else if (phy_type == WUFE_SDH_TYPE_E1 ||
                  phy_type == WUFE_SONET_TYPE_E1 ||
                  phy_type == WUFE_CAD_TYPE_E1)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_E1;
         else if (phy_type == WUFE_DCC_TYPE_MS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_MS;
         else if (phy_type == WUFE_DCC_TYPE_RS)
            num_of_slots = WUFEI_FM_NUM_OF_SLOTS_DCC_RS;
      }

      WUFEI_HwFmRemoveBankValue(system_registry, coreid, phy_registry->tx_bank_number, phy_type, num_of_slots);

      /*Clear indirect registers*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectMemset(id, coreid,
                                                    (WP_U32)WUFEI_INDIR_FM_TX_PORT_CONF(phy_id*2),
                                                    0x0000,
                                                    2));
      /*This RAM is 4 registers per port*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectMemset(id, coreid,
                                                    (WP_U32)WUFEI_INDIR_FM_TX_PORT_DDR_MEM(phy_id),
                                                    0x0000,
                                                    4));

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwFmIdirectMemset(id, coreid,
                                                    (WP_U32)WUFEI_INDIR_FM_TX_PORT_BURST_MEM(phy_id),
                                                    0x0000,
                                                    1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmPhyCheckMemPtr
 *
 * Purpose: Checks the pointers in the ddr and burst memory
 *
 * Inputs:
 *     wufe_id             UFE id
 *     core_id             The core id of this fm
 *     emphy_port_id       The id of the phy in this core
 *     direction           The direction in which to set
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmPhyCheckMemPtr(WP_U32 wufe_id,
                                     WP_U32 coreid,
                                     WP_U32 emphy_port_id,
                                     WP_U32 direction)
{
   WP_U16 read_value, write_value;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      /*Check Fifo Manager's Rx DDR Memory for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_DDR_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &write_value));

      if (WUFEI_FM_PORT_DDR_MEM_READ_PTR_V(read_value) != WUFEI_FM_PORT_DDR_MEM_WRITE_PTR_V(write_value))
      {
         return WUFE_ERR_PHY_FIFO_NOT_EMPTY;
      }
      /*Check Fifo Manager's Rx Burst Memory for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_BURST_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      /*Both read and write pointers are on the same reg*/
      if (WUFEI_FM_PORT_BURST_MEM_READ_PTR_V(read_value) != WUFEI_FM_PORT_BURST_MEM_WRITE_PTR_V(read_value))
         {

         return WUFE_ERR_PHY_FIFO_NOT_EMPTY;
         }
   }
   if (direction & WUFE_TX_DIRECTION)
   {
      /*Check Fifo Manager's Tx DDR Memory for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_DDR_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &write_value));

      if (WUFEI_FM_PORT_DDR_MEM_READ_PTR_V(read_value) != WUFEI_FM_PORT_DDR_MEM_WRITE_PTR_V(write_value))
         return WUFE_ERR_PHY_FIFO_NOT_EMPTY;

      /*Check Fifo Manager's Tx Burst Memory for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_BURST_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      /*Both read and write pointers are on the same reg*/
      if (WUFEI_FM_PORT_BURST_MEM_READ_PTR_V(read_value) != WUFEI_FM_PORT_BURST_MEM_WRITE_PTR_V(read_value))
         return WUFE_ERR_PHY_FIFO_NOT_EMPTY;
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmPhyClearPtr
 *
 * Purpose: Clears the pointers in the ddr and burst memory
 *
 * Inputs:
 *     wufe_id             UFE id
 *     core_id             The core id of this fm
 *     emphy_port_id       The id of the phy in this core
 *     direction           The direction in which to set
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyDisable (in case of underrun)
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmPhyClearPtr(WP_U32 wufe_id,
                                  WP_U32 coreid,
                                  WP_U32 emphy_port_id,
                                  WP_U32 direction)
{
   WP_U16 read_value, write_value;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      /*Clear Fifo Manager's Rx DDR Memory pointers for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_DDR_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &write_value));

      read_value &= ~WUFEI_FM_PORT_DDR_MEM_READ_PTR_M;
      write_value &= ~WUFEI_FM_PORT_DDR_MEM_WRITE_PTR_M;

      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_DDR_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataSet(wufe_id, coreid, read_value));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataSet(wufe_id, coreid, write_value));

      /*Clear Fifo Manager's Rx Burst Memory pointers for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_BURST_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      /*Both read and write pointers are on the same reg*/
      read_value &=
         (~WUFEI_FM_PORT_BURST_MEM_READ_PTR_M &
          ~WUFEI_FM_PORT_BURST_MEM_WRITE_PTR_M);
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_BURST_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataSet(wufe_id, coreid, read_value));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /*Clear Fifo Manager's Tx DDR Memory pointers for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_DDR_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &write_value));

      read_value &= ~WUFEI_FM_PORT_DDR_MEM_READ_PTR_M;
      write_value &= ~WUFEI_FM_PORT_DDR_MEM_WRITE_PTR_M;

      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_DDR_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataSet(wufe_id, coreid, read_value));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataSet(wufe_id, coreid, write_value));

      /*Clear Fifo Manager's Rx Burst Memory pointers for this phy */
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_BURST_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &read_value));
      /*Both read and write pointers are on the same reg*/
      read_value &=
         (~WUFEI_FM_PORT_BURST_MEM_READ_PTR_M &
          ~WUFEI_FM_PORT_BURST_MEM_WRITE_PTR_M);
      WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_BURST_MEM(emphy_port_id));
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataSet(wufe_id, coreid, read_value));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmPhyDisplay
 *
 * Purpose: Displays the fm indirect information of a specific phy
 *
 * Inputs:
 *     wufe_id             UFE id
 *     core_id             The core id of this fm
 *     emphy_port_id       The id of the phy in this core
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
WUFE_status WUFEI_HwFmPhyDisplay(WP_U32 wufe_id,
                                 WP_U32 coreid,
                                 WP_U32 emphy_port_id)
{
   WP_U16 fm_value;
   WUFE_status status;

   /*Fifo Manager's Rx port configuration for this phy */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_CONF(emphy_port_id));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Rx Port Configuration for phy %d in core %d is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));

   /*Fifo Manager's Tx port configuration for this phy */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_CONF(emphy_port_id*2));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Tx Port Configuration for phy %d in core %d offset 0 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Tx Port Configuration for phy %d in core %d offset 1 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));

   /*Fifo Manager's Rx DDR Memory for this phy */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_DDR_MEM(emphy_port_id));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Rx DDR Memory for phy %d in core %d offset 0 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Rx DDR Memory for phy %d in core %d offset 1 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));

   /*Fifo Manager's Tx DDR Memory for this phy */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_DDR_MEM(emphy_port_id));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Tx DDR Memory for phy %d in core %d offset 0 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Tx DDR Memory for phy %d in core %d offset 1 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Tx DDR Memory for phy %d in core %d offset 2 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Tx DDR Memory for phy %d in core %d offset 3 is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));

   /*Fifo Manager's Rx Burst Memory for this phy */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_BURST_MEM(emphy_port_id));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Rx Burst Memory for phy %d in core %d is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));

   /*Fifo Manager's Tx Burst Memory for this phy */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_BURST_MEM(emphy_port_id));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));
   WUFE_PRINT_DISP((wufei_log,"FM Tx Burst Memory for phy %d in core %d is %x\n",
                    emphy_port_id,
                    coreid,
                    fm_value));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmIndirectDump
 *
 * Purpose: Displays the entire indirect address space of the fifo manager
 *
 * Inputs:
 *     wufe_id             UFE id
 *     core_id             The core id of this fm
 *
 * Outputs:
 *     Display to screen
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_FmDisplay
 *
 ****************************************************************************/
WUFE_status WUFEI_HwFmIndirectDump(WP_U32 wufe_id,
                                   WP_U32 coreid)
{
   WP_U16 i, fm_value;
   WUFE_status status;

   /*Dump all Fifo Manager's Rx port configuration registers */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_CONF(0));
   WUFE_PRINT_DISP((wufei_log,"\nFM Rx Port Configuration\n"));

   for (i=0; i<WUFEI_INDIR_FM_RX_PORT_CONF_SIZE; i++)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));

      WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_FM_RX_PORT_CONF(0)+i, fm_value));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }

   /*Dump all Fifo Manager's Tx port configuration registers */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_CONF(0));
   WUFE_PRINT_DISP((wufei_log,"\nFM Tx Port Configuration\n"));

   for (i=0; i<WUFEI_INDIR_FM_TX_PORT_CONF_SIZE; i++)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));

      WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_FM_TX_PORT_CONF(0)+i, fm_value));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }

   /*Dump all Fifo Manager's Rx port DDR memory registers */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_DDR_MEM(0));
   WUFE_PRINT_DISP((wufei_log,"\nFM Rx Port DDR Memory\n"));

   for (i=0; i<WUFEI_INDIR_FM_RX_PORT_DDR_MEM_SIZE; i++)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));

      WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_FM_RX_PORT_DDR_MEM(0)+i, fm_value));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }

   /*Dump all Fifo Manager's Tx port DDR memory registers */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_DDR_MEM(0));
   WUFE_PRINT_DISP((wufei_log,"\nFM Tx Port DDR Memory\n"));

   for (i=0; i<WUFEI_INDIR_FM_TX_PORT_DDR_MEM_SIZE; i++)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));

      WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_FM_TX_PORT_DDR_MEM(0)+i, fm_value));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }

   /*Dump all Fifo Manager's Rx port burst memory registers */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_RX_PORT_BURST_MEM(0));
   WUFE_PRINT_DISP((wufei_log,"\nFM Rx Port Burst Memory\n"));

   for (i=0; i<WUFEI_INDIR_FM_RX_PORT_BURST_MEM_SIZE; i++)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));

      WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_FM_RX_PORT_BURST_MEM(0)+i, fm_value));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }

   /*Dump all Fifo Manager's Tx port burst memory registers */
   WUFEI_HwFmIdirectAddrSet(wufe_id, coreid, WUFEI_INDIR_FM_TX_PORT_BURST_MEM(0));
   WUFE_PRINT_DISP((wufei_log,"\nFM Tx Port Burst Memory\n"));

   for (i=0; i<WUFEI_INDIR_FM_TX_PORT_BURST_MEM_SIZE; i++)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwFmIdirectDataGet(wufe_id, coreid, &fm_value));

      WUFE_PRINT_DISP((wufei_log,"0x%04x: 0x%04x\t", WUFEI_INDIR_FM_TX_PORT_BURST_MEM(0)+i, fm_value));
      if(i%4==3)
         WUFE_PRINT_DISP((wufei_log,"\n"));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmModeDisplay
 *
 * Purpose: Displays the Mode register of the Fifo Manager
 *
 * Description:
 *
 * Inputs:
 *     ufe_id              The ufe id
 *     coreid              The core id
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *     WUFE_SystemDisplay
 *
 ****************************************************************************/
void WUFEI_HwFmModeDisplay(WP_U32 ufe_id, WP_U32 coreid)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufe_id,coreid);
   WP_U16 mode_value, *mode_ptr;

   mode_ptr = &((WUFEI_hw_fm*)fm_base)->fm_mode_reg.value;

   WUFEI_REG_GET(mode_ptr, &mode_value);

   WUFE_PRINT_DISP((wufei_log, "The Fifo Manager %d Mode Register: %x\n",coreid,mode_value));

   if (WUFEI_FM_RX_MODE_ENABLE_V(mode_value))
      WUFE_PRINT_DISP((wufei_log,"The Fifo Manager's %d RX direction is enabled\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"The Fifo Manager's %d RX direction is disabled\n",coreid));

   if (WUFEI_FM_TX_MODE_ENABLE_V(mode_value))
      WUFE_PRINT_DISP((wufei_log,"The Fifo Manager's %d TX direction is enabled\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"The Fifo Manager's %d TX direction is disabled\n",coreid));

   if (WUFEI_FM_LINE_LOOPBACK_V(mode_value))
      WUFE_PRINT_DISP((wufei_log,"The Fifo Manager's %d Line Loopback is enabled\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"The Fifo Manager's %d Line Loopback is disabled\n",coreid));

   if (WUFEI_DbgGet(ufe_id) > WUFEI_DBG_LEVEL_0)
   {
      WUFEI_HwFmIndirectDump(ufe_id, coreid);
   }
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmInterruptReadRxDisplay
 *
 * Purpose: Displays the last ports in which an interrupt accurred
 *
 * Inputs:
 *     ufe_id              The ufe id
 *
 * Outputs: display.
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwFmInterruptReadRxDisplay(WP_U32 ufe_id, WP_U32 coreid)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufe_id, coreid);
   WP_U16 int_read_value, *int_read_ptr;
   WP_U16 num_of_reads = MAX_NUM_OF_FM_INTERRUPT_READS;

   int_read_ptr = &((WUFEI_hw_fm*)fm_base)->fm_int_read_rx_fifo.value;

   WUFEI_REG_GET(int_read_ptr, &int_read_value);
   while (WUFEI_FM_INT_READ_VALID_V(int_read_value) && num_of_reads>0)
   {
      WUFE_PRINT_DISP((wufei_log,"Interrupt RX port id %x caused an interrupt of type %x\n",
                       WUFEI_FM_INT_READ_PORT_ID_V(int_read_value),
                       WUFEI_FM_INT_READ_TYPE_V(int_read_value)));

      num_of_reads--;
      WUFEI_REG_GET(int_read_ptr, &int_read_value);
   }
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmInterruptReadTxDisplay
 *
 * Purpose: Displays the last ports in which an interrupt accurred
 *
 * Inputs:
 *     ufe_id              The ufe id
 *
 * Outputs: display.
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwFmInterruptReadTxDisplay(WP_U32 ufe_id, WP_U32 coreid)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufe_id, coreid);
   WP_U16 int_read_value, *int_read_ptr;
   WP_U16 num_of_reads = MAX_NUM_OF_FM_INTERRUPT_READS;

   int_read_ptr = &((WUFEI_hw_fm*)fm_base)->fm_int_read_tx_fifo.value;

   WUFEI_REG_GET(int_read_ptr, &int_read_value);
   while (WUFEI_FM_INT_READ_VALID_V(int_read_value) && num_of_reads>0)
   {
      WUFE_PRINT_DISP((wufei_log,"Interrupt TX port id %x caused an interrupt of type %x\n",
                       WUFEI_FM_INT_READ_PORT_ID_V(int_read_value),
                       WUFEI_FM_INT_READ_TYPE_V(int_read_value)));

      num_of_reads--;
      WUFEI_REG_GET(int_read_ptr, &int_read_value);
   }
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmEventRegSet
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
void WUFEI_HwFmEventRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_fm*)fm_base)->fm_int_reg.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmEventRegGet
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
void WUFEI_HwFmEventRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_fm*)fm_base)->fm_int_reg.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmEventMaskRegSet
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
void WUFEI_HwFmEventMaskRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_fm*)fm_base)->fm_int_msk_reg.value;

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmEventMaskRegGet
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
void WUFEI_HwFmEventMaskRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_fm*)fm_base)->fm_int_msk_reg.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwFmVfpTxSet
 *
 * Purpose:   Set TX request number for very fast port (used in clear channel mode).
 *
 *
 * Description:
 *     This function writes the number of TX requests for clear channel ports.
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwFmVfpTxSet(WP_U32 ufeid, WP_U16 coreid, WP_U32 vfp_index, WP_U32 line_type)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WUFEI_hw_fifo_vfp_tx *vfp_tx_ptr;
   WP_U16* reg_ptr;
   WP_U16 requests_value;
   WP_U16 value;

   if (vfp_index >= 12)
      return;

   vfp_tx_ptr = &(((WUFEI_hw_fm*)fm_base)->fm_tx_vfp[vfp_index]);

   reg_ptr = &(vfp_tx_ptr->fm_tx_vfp_rd_ptr.value);
   WUFEI_REG_SET(reg_ptr, 0);

   reg_ptr = &(vfp_tx_ptr->fm_tx_vfp_wr_ptr.value);
   WUFEI_REG_SET(reg_ptr, 0);
   reg_ptr = &(vfp_tx_ptr->fm_tx_vfp_last_counter.value);
   WUFEI_REG_SET(reg_ptr, 0);

   reg_ptr = &(vfp_tx_ptr->fm_tx_vfp_max_req_stx.value);
   
   if ( (line_type == WUFE_SDH_TYPE_VC44C) || (line_type == WUFE_SONET_TYPE_STS12) )
   {
      requests_value = WUFE_FM_TX_VFP_REQ_CNT_OC12 - 1;
   }
   else if ( (line_type == WUFE_SDH_TYPE_VC4) || 
             (line_type == WUFE_SONET_TYPE_STS3) ||
             (line_type == WUFE_SDH_TYPE_DS3) ||
             (line_type == WUFE_SDH_TYPE_E3) ||
             (line_type == WUFE_SONET_TYPE_DS3) ||
             (line_type == WUFE_SONET_TYPE_E3))
   {
      requests_value = WUFE_FM_TX_VFP_REQ_CNT_OC3 - 1;
   }
   else return;

   value = WUFEI_FM_VFP_TX_MAX_REQ_F(requests_value) |
      WUFEI_FM_VFP_TX_STX_F(0);
   WUFEI_REG_SET(reg_ptr, value);
}

static WP_U16 WUFEI_HwFmVfpStartTxThreshIndexGet(WP_U32 line_type, WP_U32 struct_id)
{
   WP_U16 start_tx_thresh_index = 0;

   if (line_type == WUFE_SDH_TYPE_VC44C ||
       line_type == WUFE_SONET_TYPE_STS12)
   {
      if (struct_id == WUFE_STRUCT_PHY_ATM)
         start_tx_thresh_index = 1;
      if (struct_id == WUFE_STRUCT_PHY_HDLC || struct_id == WUFE_STRUCT_PHY_POS)
         start_tx_thresh_index = 3;
   }
      
   if ((line_type == WUFE_SDH_TYPE_VC4) ||
       (line_type == WUFE_SONET_TYPE_STS3) ||
       (line_type == WUFE_SDH_TYPE_DS3) ||
       (line_type == WUFE_SDH_TYPE_E3) ||
       (line_type == WUFE_SONET_TYPE_DS3) ||
       (line_type == WUFE_SONET_TYPE_E3))
   {
      if (struct_id == WUFE_STRUCT_PHY_ATM)
         start_tx_thresh_index = 0;
      if (struct_id == WUFE_STRUCT_PHY_HDLC || struct_id == WUFE_STRUCT_PHY_POS)
         start_tx_thresh_index = 2;
   }

   return start_tx_thresh_index;
}

void WUFEI_HwFmVfpTxStartThresholdSet(WP_U32 ufeid, WP_U16 coreid,
                                      WP_U32 vfp_index, WP_U32 line_type, WP_U32 struct_id)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WUFEI_hw_fifo_vfp_tx *vfp_tx_ptr;
   WP_U16* reg_ptr;
   WP_U16 value;
   WP_U16 start_tx_thresh_index;

   /* nothing to be done for channelized lines */
   if (!(line_type == WUFE_SDH_TYPE_VC44C || 
         line_type == WUFE_SONET_TYPE_STS12 ||
         line_type == WUFE_SDH_TYPE_VC4 || 
         line_type == WUFE_SONET_TYPE_STS3||
         line_type == WUFE_SDH_TYPE_DS3 ||
         line_type == WUFE_SDH_TYPE_E3 ||
         line_type == WUFE_SONET_TYPE_DS3 ||
         line_type == WUFE_SONET_TYPE_E3))
      return;

   if (vfp_index >= 12)
      return;

   start_tx_thresh_index = WUFEI_HwFmVfpStartTxThreshIndexGet(line_type, struct_id);

   vfp_tx_ptr = &(((WUFEI_hw_fm*)fm_base)->fm_tx_vfp[vfp_index]);
   reg_ptr = &(vfp_tx_ptr->fm_tx_vfp_max_req_stx.value);
   
   WUFEI_REG_GET(reg_ptr, &value);
   value &= ~WUFEI_FM_VFP_TX_STX_M;
   value |= WUFEI_FM_VFP_TX_STX_F(start_tx_thresh_index);
   WUFEI_REG_SET(reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmVfpRxReset
 *
 * Purpose:   Resets the VFP RX registers
 *
 * Description:
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *     vfp_index - index of VFP register block
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwFmVfpRxReset(WP_U32 ufeid, WP_U16 coreid, WP_U32 vfp_index)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WUFEI_hw_fifo_vfp_rx *vfp_rx_ptr;
   WP_U16 *reg_ptr;

   if (vfp_index >= 12)
      return;

   vfp_rx_ptr = &(((WUFEI_hw_fm*)fm_base)->fm_rx_vfp[vfp_index]);

   reg_ptr = &(vfp_rx_ptr->fm_rx_vfp_fifo_mask.value);
   WUFEI_REG_SET(reg_ptr, 0); /* disable the port */

   reg_ptr = &(vfp_rx_ptr->fm_rx_vfp_rd_ptr.value);
   WUFEI_REG_SET(reg_ptr, 0);

   reg_ptr = &(vfp_rx_ptr->fm_rx_vfp_data_wr_ptr.value);
   WUFEI_REG_SET(reg_ptr, 0);

   reg_ptr = &(vfp_rx_ptr->fm_rx_vfp_bd_wr_ptr.value);
   WUFEI_REG_SET(reg_ptr, 0);

   reg_ptr = &(vfp_rx_ptr->fm_rx_vfp_fifo_mask.value);
   WUFEI_REG_SET(reg_ptr, 1); /* enable the port */

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmVfpInterruptInfoGet
 *
 * Purpose:   Reads the VFP RX interrupt info register
 *
 * Description:
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *
 ****************************************************************************/
WP_U16  WUFEI_HwFmVfpInterruptInfoGet(WP_U32 ufeid, WP_U32 coreid)
{
   WP_U32 fm_base = WUFEI_UfeFmBaseAddrGet(ufeid, coreid);
   WP_U16 *reg_ptr;
   WP_U16 value;

   reg_ptr = &(((WUFEI_hw_fm*)fm_base)->fm_rx_vfp_int_info.value);

   WUFEI_REG_GET(reg_ptr, &value);

   return value;
}
