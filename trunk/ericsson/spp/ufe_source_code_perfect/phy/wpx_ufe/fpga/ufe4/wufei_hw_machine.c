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

#ifndef WUFEI_HW_MACHINE_H
#include "wufei_hw_machine.h"
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

static void WUFEI_HwMachineIdirectAddrSet(WP_U32 ufeid, WP_U32 coreid, WP_U32 indirect_addr);
static WUFE_status WUFEI_HwMachineIdirectDataSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value);
static WUFE_status WUFEI_HwMachineIdirectDataGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value);
static WUFE_status WUFEI_HwMachineIdirectMemset(WP_U32 ufeid, WP_U32 coreid, WP_U32 addr, WP_U16 value, WP_U32 size);

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineIdirectAddrSet
 *
 * Purpose: Indirect address set for all MACHINE
 *
 * Description:
 *
 *      Setting the indirect address registers
 *
 * Inputs:
 *
 *      ufeid - the UFE id
 *      coreid - the core id (0/1)
 *      indirect_addr - indirect address to approach
 *
 *
 * Outputs:
 *     write to MACHINE indirect address register
 *
 * Return Value: none
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static void WUFEI_HwMachineIdirectAddrSet(WP_U32 ufeid, WP_U32 coreid, WP_U32 indirect_addr)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_low.value;
   indha_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_high.value;

   WUFEI_REG_SET(indla_ptr, indirect_addr & 0xffff);
   WUFEI_REG_SET(indha_ptr, (indirect_addr >> 16) & 0xffff);

   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineIdirectDataSet
 *
 * Purpose: Indirect data set for the MACHINE
 *
 *
 * Description:
 *
 *      Setting the indirect data register
 *
 * Inputs:
 *
 *      ufeid - the UFE id
 *      coreid - the core id (0/1)
 *      value - value to set
 *
 * Outputs:
 *     write to MACHINE indirect data register
 *
 * Return Value: Status of the DataSet
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwMachineIdirectDataSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_cmd.value;

   WUFEI_REG_SET(inddat_ptr, value);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;

         WUFE_PRINT_ERR((wufei_log, "ERROR: Timeout while trying to set data %x in Machine indirect register\n", value));
         ind_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", value));
         ind_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_high.value;
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
 * Function:  WUFEI_HwMachineIdirectDataGet
 *
 * Purpose: Indirect data get for the MACHINE
 *
 *
 * Description:
 *
 *      get the indirect data
 *
 * Inputs:
 *
 *      ufeid - the UFE id
 *      coreid - the core id (0/1)
 *      value - value to get
 *
 * Outputs:
 *     get the MACHINE indirect data
 *
 * Return Value: Status of the DataGet
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwMachineIdirectDataGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16 *inddat_ptr;
   WP_U16 *indcmd_ptr;
   WP_U32 timeout = WUFEI_INDIRECT_TIMEOUT;
   WP_U16 cmd_value;

   inddat_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_data.value;
   indcmd_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_cmd.value;

   WUFEI_REG_SET(indcmd_ptr, WUFEI_INDIRECT_RD_CMD);
   WUFEI_REG_GET(indcmd_ptr ,&cmd_value);
   while (!(cmd_value & WUFEI_INDIRECT_BUSY_WAIT))
   {
      if (timeout==0)
      {
         WP_U16 *ind_ptr;
         WP_U16 tmp_value;

         WUFE_PRINT_ERR((wufei_log,"ERROR: Timeout while trying to read data from Machine indirect register\n"));
         ind_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_low.value;
         WUFEI_REG_GET(ind_ptr ,&tmp_value);
         WUFE_PRINT_ERR((wufei_log,"Indirect low address was %x\n", tmp_value));
         ind_ptr = &((WUFEI_hw_machine*)machine_base)->machine_ind_high.value;
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
 * Function:  WUFEI_HwMachineIdirectMemset
 *
 * Purpose: Continuos Indirect memory set for the MACHINE
 *
 *
 * Description:
 *
 *      Indirect address set for several sequential registers
 *
 * Inputs:
 *
 *      ufeid - the UFE id
 *      addr - start address
 *      value - value to set
 *      size - memset size
 *
 * Outputs:
 *     write to MACHINE indirect registers
 *
 * Return Value: Status of the IdirectMemset
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwMachineIdirectMemset(WP_U32 ufeid, WP_U32 coreid, WP_U32 addr, WP_U16 value, WP_U32 size)
{
   WP_U32 i;
   WUFE_status status;

   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, addr);
   for(i = 0; i<size; i++)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(ufeid, coreid, value));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCesPhyEnable
 *
 * Purpose: Enables the Ces Machine for a specific phy
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 * Outputs:
 *     write to Ces indirect RCPT/TCPT
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwCesPhyEnable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 phy_id, WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   /*Calculate phy entry in the parameter tables*/
   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_CES_RX_MODE_EN_M;

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_CES_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCesPhyDisable
 *
 * Purpose: Disables the Ces Machine for a specific phy
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     write to Ces indirect RCPT/TCPT
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwCesPhyDisable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 phy_id, WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_CES_RX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_CES_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCesConfigureSlots
 *
 * Purpose: Configures the slot group for a specific phy
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 * Outputs:
 *     write to CES machine indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_HwCesPhyCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwCesConfigureSlots(WP_U32 ufeid,
                                      WP_U32 coreid,
                                      WUFE_SlotGroup *slot_group,
                                      WP_U32 phy_id,
                                      WP_U32 direction)
{
   WP_U16 i, slot_id;
   WP_U32 slots_mask = 0;
   WUFE_status status;
   WP_U16 offset ;

   if (phy_id != CES_MAC_NOT_RECONFIGURE)
   {/*In case of reconfigure*/
      WP_U16 mode;

      if (direction & WUFE_RX_DIRECTION)
      {
         offset = (phy_id * MAC_NUM_REGS_PER_PT) + CES_MAC_NSPP_OFFSET_REGS;
         WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset));
         mode = WUFEI_CES_RX_MODE_TYPE_F(WUFEI_PHY_MODE_TRANS) |
            WUFEI_CES_RX_MODE_NSPP_F(slot_group->n_slots -1);
      }
      else if (direction & WUFE_TX_DIRECTION)
      {
         offset = (phy_id * MAC_NUM_REGS_PER_PT) + CES_MAC_NSPP_OFFSET_REGS;
         WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset));
         mode = WUFEI_CES_TX_MODE_TYPE_F(WUFEI_PHY_MODE_TRANS) |
            WUFEI_CES_TX_MODE_NSPP_F(slot_group->n_slots -1);
      }
      else
      {
         return WUFE_ERR_INVALID_DIRECTION;
      }
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(ufeid, coreid, mode));
   }


   if (phy_id != CES_MAC_NOT_RECONFIGURE)
   {/*In case of reconfigure*/

      if (direction & WUFE_RX_DIRECTION)
      {
         offset = (phy_id * MAC_NUM_REGS_PER_PT) + CES_MAC_SLOTS_REG_OFFSET_RX;
         WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset));
      }
      else if (direction & WUFE_TX_DIRECTION)
      {
         offset = (phy_id * MAC_NUM_REGS_PER_PT) +CES_MAC_SLOTS_REG_OFFSET_TX;
         WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset));
      }
      else
      {
         return WUFE_ERR_INVALID_DIRECTION;
      }
   }

   for (i=0; i< slot_group->n_slots;i++)
   {
      /*The slots are arranged in reversed order of bits in the two registers*/
      /*So they are calculted backwards. i.e. slots 31 and 15 will be in bits 0 and 16*/
      slot_id = slot_group->slot_arr[i];
      slots_mask |= 1 << (WUFEI_MAX_NUM_OF_SLOTS -1 - slot_id);
   }
   /*Write to the indirect registers*/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataSet(ufeid, coreid, (slots_mask >> 16) & 0xffff));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataSet(ufeid, coreid, slots_mask & 0xffff));



   return WUFE_OK;
}
/*****************************************************************************
 *
 * Function:  WUFEI_HwCesPhyUnderrunGet
 *
 * Purpose: Getter for the CES underrun state
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyDisable
 *
 ****************************************************************************/
WP_U8 WUFEI_HwCesPhyUnderrunGet(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 phy_id)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 value, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_CES_UNDERRUN_OFFSET;

   WUFEI_HwMachineIdirectAddrSet(id, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(id, coreid, &value));

   return WUFEI_CES_TX_UNDERRUN_V(value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCesPhyCreate
 *
 * Purpose: Initializes the CES Machine tables for a specific phy
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     slot_group          An array that hold all the information regarding the phy's timeslots
 *     data_unit_size      For setting the data unit size of the phy
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     write to CES machine indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwCesPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_SlotGroup *slot_group,
                                 WP_U16 data_unit_size,
                                 WP_U32 direction)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 mode, offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   offset = phy_id * MAC_NUM_REGS_PER_PT;

   if (direction & WUFE_RX_DIRECTION)
   {
      /* Set RSPT */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         0x0000,
                                                         1));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0030));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwCesConfigureSlots(id, coreid, slot_group, CES_MAC_NOT_RECONFIGURE, WUFE_RX_DIRECTION));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));

      /* Set RCPT */
      mode = WUFEI_CES_RX_MODE_TYPE_F(WUFEI_PHY_MODE_TRANS) |
         WUFEI_CES_RX_MODE_NSPP_F(slot_group->n_slots -1);
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));

      mode = WUFEI_CES_RX_MODE_DATA_U_SIZE_F(data_unit_size -1);
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Set TSPT */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         0x0000,
                                                         3));

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwCesConfigureSlots(id, coreid, slot_group, CES_MAC_NOT_RECONFIGURE, WUFE_TX_DIRECTION));


      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      /* Set TCPT */
      mode = WUFEI_CES_TX_MODE_TYPE_F(WUFEI_PHY_MODE_TRANS) |
         WUFEI_CES_TX_MODE_NSPP_F(slot_group->n_slots -1);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
   }

   return WUFE_OK;
}
/*****************************************************************************
 *
 * Function:  WUFEI_HwCesCasConfig
 *
 * Purpose: Sets the cas table of a specific line
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     lineid              The line id is required for calculating the offset
 *
 * Outputs:
 *     write to CES machine indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_LineCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwCesCasConfig(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 line_id)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 offset;
   WUFE_status status;

   offset = line_id * CES_MAC_CAS_TABLE_OFFSET;

   /* Set Rx CAS Table*/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(id, coreid,
                                                      (WP_U32)WUFEI_INDIR_CES_MAC_RX_CAS_TABLE(offset),
                                                      CES_CAS_TABLE_INITIAL_VALUE,
                                                      CES_MAC_NUM_REGS_PER_CAS_TABLE));

   /* Set Tx CAS Table*/
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(id, coreid,
                                                      (WP_U32)WUFEI_INDIR_CES_MAC_TX_CAS_TABLE(offset),
                                                      CES_CAS_TABLE_INITIAL_VALUE,
                                                      CES_MAC_NUM_REGS_PER_CAS_TABLE));

   return WUFE_OK;
}

void WUFEI_HwCesCasDisplay(WUFEI_system_registry* system_registry,
                           WP_U32 coreid,
                           WP_U32 line_id)
{
   WP_U32 i, id = system_registry->wufe_id;
   WP_U16 offset, value;

   WUFE_PRINT_DISP((wufei_log, " cas rx table: \n"));
   offset = (line_id * CES_MAC_CAS_TABLE_OFFSET);
   WUFEI_HwMachineIdirectAddrSet(id, coreid, (WP_U32) WUFEI_INDIR_CES_MAC_RX_CAS_TABLE(offset));
   for(i=0;i<CES_MAC_NUM_REGS_PER_CAS_TABLE;i++)
   {
      WUFEI_HwMachineIdirectDataGet(id, coreid, &value);
      WUFE_PRINT_DISP((wufei_log, "%x",value));
   }
   WUFE_PRINT_DISP((wufei_log, "\n"));
   WUFE_PRINT_DISP((wufei_log, " cas tx table for line_id %d: \n",line_id));
   WUFEI_HwMachineIdirectAddrSet(id, coreid, (WP_U32) WUFEI_INDIR_CES_MAC_TX_CAS_TABLE(offset));
   for(i=0;i<CES_MAC_NUM_REGS_PER_CAS_TABLE;i++)
   {
     WUFEI_HwMachineIdirectDataGet(id, coreid, &value);
     WUFE_PRINT_DISP((wufei_log, "%x",value));
   }

   WUFE_PRINT_DISP((wufei_log, "\n"));
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwHdlcPhyEnable
 *
 * Purpose: Enables the Hdlc Machine for a specific phy
 *
 * Description: Setting the enable bit in the RPT table
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     Setting the enable field in the Hdlc indirect TSPT/RSPT table
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwHdlcPhyEnable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 phy_id, WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   /*Calculate phy entry in the parameter tables*/
   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_HDLC_RX_MODE_EN_M;

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_HDLC_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwHdlcPhyDisable
 *
 * Purpose: Disable the Hdlc Machine for a specific phy
 *
 * Description: Clearing the enable bit in the RPT table
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     Clearing the enable field in the Hdlc indirect TSPT/RSPT table
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwHdlcPhyDisable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 phy_id, WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_HDLC_RX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_HDLC_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwHdlcPhyUnderrunGet
 *
 * Purpose: Check for the HDLC underrun state.
 *          In case ther are underruns, some of the RAM's
 *          of the FM should be cleared
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *
 * Outputs:
 *     The value of the underrun bit in the TSPT table ('1' when in underrun state)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyDisable
 *
 ****************************************************************************/
WP_U8 WUFEI_HwHdlcPhyUnderrunGet(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 phy_id)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 value, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_HDLC_UNDERRUN_OFFSET;

   WUFEI_HwMachineIdirectAddrSet(id, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(id, coreid, &value));

   return WUFEI_HDLC_TX_UNDERRUN_V(value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwHdlcMinFlagsReconfig
 *
 * Purpose: Reconfigure the HDLC Machine minimum flags between frames parameter
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     min_flags            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the TCPT of the HDLC phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwHdlcMinFlagsReconfig(WP_U32 ufeid,
                                         WP_U32 coreid,
                                         WP_U32 phy_id,
                                         WP_U32 min_flags)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read TCPT and modify according to new CRC size*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_HDLC_TX_NOF_M;
   mode |= WUFEI_HDLC_TX_NOF_F(min_flags);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwHdlcNoFlagsReconfig
 *
 * Purpose: Reconfigure the HDLC Machine no flags between frames parameter
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     no_flags            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the TCPT of the HDLC phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwHdlcNoFlagsReconfig(WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 no_flags)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read TCPT and modify according to new CRC size*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_HDLC_TX_IDLE_M;
   mode |= WUFEI_HDLC_TX_IDLE_F(no_flags);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwHdlcCrcReconfig
 *
 * Purpose: Reconfigure the HDLC Machine CRC size parameter
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *     crc_size            The new CRC Size to be configured.
 *
 * Outputs:
 *     Changes in the RCPT or TCPT of the HDLC phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwHdlcCrcReconfig(WP_U32 ufeid,
                                    WP_U32 coreid,
                                    WP_U32 phy_id,
                                    WP_U32 direction,
                                    WP_U32 crc_size)
{
   WP_U16 mode, offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   if (direction & WUFE_RX_DIRECTION)
   {
      /* Read RCPT and modify according to new CRC size*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_HDLC_RX_FCS_M;
      mode |= WUFEI_HDLC_RX_FCS_F(crc_size);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Read TCPT and modify according to new CRC size*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_HDLC_TX_FCS_M;
      mode |= WUFEI_HDLC_TX_FCS_F(crc_size);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwHdlcPhyCreate
 *
 * Purpose: Initializes the HDLC Machine tables for a specific phy
 *
 *
 * Description:
 *
 * Inputs:
       system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     ufe_phy_cfg         Struct with all the defined parameters of the phy
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     Initializing the registers of the RSPT/RCPT and TSPT/TCPT for the HDLC phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwHdlcPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_phy *ufe_phy_cfg,
                                 WP_U32 direction)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 mode, offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   offset = phy_id * MAC_NUM_REGS_PER_PT;

   if (direction & WUFE_RX_DIRECTION)
   {
      /* Set RSPT */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         0x0000,
                                                         MAC_NUM_REGS_PER_SPT));
      /* Set RCPT */

      mode = WUFEI_HDLC_RX_MODE_TYPE_F(WUFEI_PHY_MODE_HDLC_BIT)             |
             WUFEI_HDLC_RX_FCS_F(ufe_phy_cfg->type.hdlc.rx_frame_crc_size);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Set TSPT */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         0x0000,
                                                         MAC_NUM_REGS_PER_SPT));
      /* Set TCPT */

      mode = WUFEI_HDLC_TX_MODE_TYPE_F(WUFEI_PHY_MODE_HDLC_BIT)                          |
             WUFEI_HDLC_TX_FCS_F(ufe_phy_cfg->type.hdlc.tx_frame_crc_size)               |
             WUFEI_HDLC_TX_IDLE_F(ufe_phy_cfg->type.hdlc.tx_no_flag_between_frames)      |
             WUFEI_HDLC_TX_NOF_F(ufe_phy_cfg->type.hdlc.tx_min_flag_num-1);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
   }

   return WUFE_OK;
}



/*****************************************************************************
 *
 * Function:  WUFEI_HwAtmPhyEnable
 *
 * Purpose: Enables the ATM Machine for a specific phy
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
 *     write to ATM indirect RCPT/TCPT
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmPhyEnable(WUFEI_system_registry* system_registry,
                              WP_U32 coreid,
                              WP_U32 phy_id,
                              WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   /*Calculate phy entry in the parameter tables*/
   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {/*Write to the enable bit in the offset of the phy in the receive parameter table*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_ATM_RX_MODE_EN_M;

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {/*Write to the enable bit in the offset of the phy in the transmit parameter table*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_ATM_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwAtmPhyDisable
 *
 * Purpose: Disables the ATM Machine for a specific phy
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
 *     write to ATM indirect RCPT/TCPT
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmPhyDisable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 phy_id, WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {/*Write to the enable bit in the offset of the phy in the receive parameter table*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_ATM_RX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {/*Write to the enable bit in the offset of the phy in the transmit parameter table*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_ATM_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwAtmStateGet
 *
 * Purpose: Getter for the ATM Machine state
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyStateGet
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmPhyStateGet(WUFEI_system_registry* system_registry,
                                   WP_U32 coreid,
                                   WP_U32 phy_id,
                                   WUFE_phy_state* sync_state)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 value, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ATM_SYNC_STATE_OFFSET;
   /*Setting the address to the offset of this phy's sync state location*/
   WUFEI_HwMachineIdirectAddrSet(id, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(id, coreid, &value));
   /*Getting the value of the sync state*/
   *sync_state = WUFEI_ATM_RX_SYNC_STATE_V(value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwAtmUnderrunGet
 *
 * Purpose: Getter for the ATM underrun state
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyDisable
 *
 ****************************************************************************/
WP_U8 WUFEI_HwAtmPhyUnderrunGet(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 phy_id)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 value, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ATM_UNDERRUN_OFFSET;
   /*Setting the address to the offset of this phy's underrun location*/
   WUFEI_HwMachineIdirectAddrSet(id, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(id, coreid, &value));
    /*Getting the value if there was or wasn't an underrun*/
   return WUFEI_ATM_TX_UNDERRUN_V(value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwAtmPhyCreate
 *
 * Purpose: Initializes the ATM Machine tables for a specific phy
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *
 * Outputs:
 *     write to ATM machine indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_phy *ufe_phy_cfg,
                                 WP_U32 direction)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 mode, offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }
   /*calculate location of this phy's tables*/
   offset = (phy_id * MAC_NUM_REGS_PER_PT);

   if (direction & WUFE_RX_DIRECTION)
   {
      /* Set RSPT - intialized to zero*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         0x0000,
                                                         8));

      /* Set RCPT - according to the user's configurations*/
      mode =
         WUFEI_ATM_RX_MODE_TYPE_F(WUFEI_PHY_MODE_ATM)|
         WUFEI_ATM_RX_DESCRAMBLER_F(ufe_phy_cfg->type.atm.rx_descrambler_mode)|
         WUFEI_ATM_RX_COSET_F(ufe_phy_cfg->type.atm.rx_coset_mode)|
         WUFEI_ATM_RX_IDLE_CELL_F(ufe_phy_cfg->type.atm.rx_idle_cell_discard)|
         WUFEI_ATM_RX_UCD_F(ufe_phy_cfg->type.atm.rx_unassigned_cell_discard)|
         WUFEI_ATM_RX_HECD_F(ufe_phy_cfg->type.atm.rx_hec_error_discard)|
         WUFEI_ATM_RX_HEN_F(ufe_phy_cfg->type.atm.rx_hec_correction_mode);

      if (ufe_phy_cfg->type.atm.rx_cell_delineation == WUFE_PHY_ATM_RX_CELL_DELINEATION_NIBBLE_RESOLUTION)
         mode |= WUFEI_ATM_RX_CDM_F(WUFEI_HW_ATM_RX_CELL_DELINEATION_NIBBLE_RESOLUTION);
      else
         mode |= WUFEI_ATM_RX_CDM_F(WUFEI_HW_ATM_RX_CELL_DELINEATION_BYTE_RESOLUTION);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));

      mode =
         WUFEI_ATM_RX_DELTA_F(ufe_phy_cfg->type.atm.rx_delta-1) |
         WUFEI_ATM_RX_ALPHA_F(ufe_phy_cfg->type.atm.rx_alpha-1);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Set TSPT - intialized to zero*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         0x0000,
                                                         4));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id,
                                                          coreid,
                                                          ufe_phy_cfg->type.atm.tx_idle_unassigned_cell_mode));

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));

      /* Set TCPT - according to the user's configurations*/
      mode =
         WUFEI_ATM_TX_MODE_TYPE_F(WUFEI_PHY_MODE_ATM)|
         WUFEI_ATM_TX_SCRAMBLER_F(ufe_phy_cfg->type.atm.tx_scrambler_mode) |
         WUFEI_ATM_TX_COSET_F(ufe_phy_cfg->type.atm.tx_coset_mode) |
         WUFEI_ATM_TX_CLP_F(ufe_phy_cfg->type.atm.tx_idle_unassigned_cell_mode);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
   }

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwAtmDescramblerReconfig
 *
 * Purpose: Reconfigure the ATM Machine scrambling
 *
 *
 * Description:
 *
 * Inputs:
 *     id                            The UFE id
 *     coreid                        Which core (0/1) does the phy belong to
 *     phyid                         The id of the phy to reconfigure
 *     direction                     The direction in which the phy should be enabled:
 *                                      WUFE_RX_DIRECTION = 1,
 *                                      WUFE_TX_DIRECTION = 2,
 *                                      WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *     scrambler_enable            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT or TCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmScramblingReconfig(WP_U32 ufeid,
                                         WP_U32 coreid,
                                         WP_U32 phy_id,
                                         WP_U32 direction,
                                         WP_U32 scrambler_enable)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }
   if (direction & WUFE_RX_DIRECTION)
   {
      /* Read RCPT and modify according to new Rx descrambler configureation*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_ATM_RX_DESCRAMBLER_M;
      mode |= WUFEI_ATM_RX_DESCRAMBLER_F(scrambler_enable);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Read TCPT and modify according to new Tx scrambler configuration*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_ATM_TX_SCRAMBLER_M;
      mode |= WUFEI_ATM_TX_SCRAMBLER_F(scrambler_enable);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwAtmCosetReconfig
 *
 * Purpose: Reconfigure the ATM Machine coset
 *
 *
 * Description:
 *
 * Inputs:
 *     id                            The UFE id
 *     coreid                        Which core (0/1) does the phy belong to
 *     phyid                         The id of the phy to reconfigure
 *     direction                     The direction in which the phy should be enabled:
 *                                      WUFE_RX_DIRECTION = 1,
 *                                      WUFE_TX_DIRECTION = 2,
 *                                      WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *     coset_enable            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT or TCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/

WUFE_status WUFEI_HwAtmCosetReconfig(WP_U32 ufeid,
                                         WP_U32 coreid,
                                         WP_U32 phy_id,
                                         WP_U32 direction,
                                         WP_U32 coset_enable)
{
   WP_U16 mode, offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

      /* Read RCPT and modify according to new Rx coset configuration*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_ATM_RX_COSET_M;
      mode |= WUFEI_ATM_RX_COSET_F(coset_enable);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

      /* Read TCPT and modify according to the new Tx coset configuration*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_ATM_TX_COSET_M;
      mode |= WUFEI_ATM_TX_COSET_F(coset_enable);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwAtmIdleRxCellModeReconfig
 *
 * Purpose: Reconfigure the ATM Machine mode of idle cells
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     idle_cell_enable            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmIdleCellModeReconfig(WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 idle_cell_enable)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read RCPT and modify according to idle cell mode configuration*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_RX_IDLE_CELL_M;
   mode |= WUFEI_ATM_RX_IDLE_CELL_F(idle_cell_enable);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwAtmUnassignedCellModeReconfig
 *
 * Purpose: Reconfigure the ATM Machine mode of idle cells
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     unassigned_cell_enable            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmUnassignedCellModeReconfig(WP_U32 ufeid,
                                                  WP_U32 coreid,
                                                  WP_U32 phy_id,
                                                  WP_U32 unassigned_cell_enable)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read RCPT and modify according to unasssigned cell mode configuration*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_RX_UCD_M;
   mode |= WUFEI_ATM_RX_UCD_F(unassigned_cell_enable);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwAtmHecErrorDiscardModeReconfig
 *
 * Purpose: Reconfigure the ATM Machine mode of idle cells
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     hec_error_discard            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmHecErrorDiscardModeReconfig(WP_U32 ufeid,
                                                   WP_U32 coreid,
                                                   WP_U32 phy_id,
                                                   WP_U32 hec_error_discard)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read RCPT and modify according to idle cell mode configuration*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_RX_HECD_M;
   mode |= WUFEI_ATM_RX_HECD_F(hec_error_discard);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwAtmHecErrorCorrectionModeReconfig
 *
 * Purpose: Reconfigure the ATM Machine mode of idle cells
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     hec_error_correction            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmHecErrorCorrectionModeReconfig (WP_U32 ufeid,
                                                       WP_U32 coreid,
                                                       WP_U32 phy_id,
                                                       WP_U32 hec_error_correction)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read RCPT and modify according to idle cell mode configuration*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_RX_HEN_M;
   mode |= WUFEI_ATM_RX_HEN_F(hec_error_correction);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwAtmCellDelineationModeReconfig
 *
 * Purpose: Reconfigure the ATM Machine mode of idle cells
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     cell_delineation            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmCellDelineationModeReconfig (WP_U32 ufeid,
                                                    WP_U32 coreid,
                                                    WP_U32 phy_id,
                                                    WP_U32 cell_delineation)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read RCPT and modify according to the rx alpha parameter*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_RX_CDM_M;
   mode |= WUFEI_ATM_RX_CDM_F(cell_delineation);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwAtmRxAlphaReconfig
 *
 * Purpose: Reconfigure the ATM Machine mode of idle cells
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     rx_alpha            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmRxAlphaReconfig (WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 rx_alpha)
{
   WP_U16 mode, offset;
   WUFE_status status;

   /* we add 1 to the offset because the alpha parameter is in the second reg of the rcpt */
   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT + 1;

   /* Read RCPT and modify according to cell delineation configuration*/
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_RX_ALPHA_M;
   mode |= WUFEI_ATM_RX_ALPHA_F(rx_alpha);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function: WUFEI_HwAtmRxDeltaReconfig
 *
 * Purpose: Reconfigure the ATM Machine Rx delta parameter
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     rx_delta            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmRxDeltaReconfig (WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 rx_delta)
{
   WP_U16 mode, offset;
   WUFE_status status;

   /* we add 1 to the offset because the alpha parameter is in the second reg of the rcpt */
   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT + 1;

   /* Read RCPT and modify according to cell rx delta parameter */
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_RX_DELTA_M;
   mode |= WUFEI_ATM_RX_DELTA_F(rx_delta);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function: WUFEI_HwAtmIdleUnassignedCellModeReconfig
 *
 * Purpose: Reconfigure the ATM Machine
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     idle_unassinged_mode            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT of the ATM phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwAtmIdleUnassignedCellModeReconfig (WP_U32 ufeid,
                                                       WP_U32 coreid,
                                                       WP_U32 phy_id,
                                                       WP_U32 idle_unassinged_mode)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read RCPT and modify according to cell rx delta parameter */
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_ATM_TX_CLP_M;
   mode |= WUFEI_ATM_TX_CLP_F(idle_unassinged_mode);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwPhyDelete
 *
 * Purpose: Deletes the Machine tables for a specific phy
 *
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     write to machine indirect registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyDelete
 *
 ****************************************************************************/
WUFE_status WUFEI_HwPhyDelete(WUFEI_system_registry* system_registry,
                              WP_U32 coreid,
                              WP_U32 phy_id,
                              WP_U32 direction)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   offset = phy_id * MAC_NUM_REGS_PER_PT;

   if (direction & WUFE_RX_DIRECTION)
   {
      /* Set RSPT and RCPT*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         0x0000,
                                                         MAC_NUM_ACTUAL_REGS_PER_PT));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Set TSPT and TCPT*/
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         0x0000,
                                                         MAC_NUM_ACTUAL_REGS_PER_PT));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwRptDisplay
 *
 * Purpose: Displays the RSPT and RCPT for a specific phy
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyDisplay
 *
 ****************************************************************************/
WUFE_status WUFEI_HwRptDisplay(WUFEI_system_registry* system_registry,
                               WP_U32 coreid,
                               WP_U32 phy_id)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 offset, rcpt_reg, i;
   WP_U16 rpt_i_value[MAC_NUM_ACTUAL_REGS_PER_PT];
   WUFE_status status;

   offset = phy_id * MAC_NUM_REGS_PER_PT;

   WUFEI_HwMachineIdirectAddrSet(id, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));

   for(i=0; i<MAC_NUM_ACTUAL_REGS_PER_PT ; i++)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(id, coreid, &rpt_i_value[i]));
   }

   WUFE_PRINT_DISP((wufei_log, "\nRSPT:0x%x \n", phy_id));
   for(i=0; i<MAC_NUM_ACTUAL_REGS_PER_PT ; i++)
   {
      if (i==MAC_NUM_ACTUAL_REGS_PER_PT-2)
         WUFE_PRINT_DISP((wufei_log, "\nRCPT:0x%x \n", phy_id));
      WUFE_PRINT_DISP((wufei_log, "0x%02x: 0x%04X\n",i, rpt_i_value[i]));
   }

   rcpt_reg = rpt_i_value[MAC_NUM_ACTUAL_REGS_PER_PT-2];

   if (WUFEI_CES_RX_MODE_TYPE_V(rcpt_reg) == WUFEI_PHY_MODE_HDLC_BIT)
   {
      WUFE_PRINT_DISP((wufei_log, "The RCPT mode type is HDLC bit resolution in Rx direction\n"));
      if (WUFEI_HDLC_RX_MODE_EN_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The phy is enabled in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The phy is disabled in Rx direction\n"));
      if (WUFEI_HDLC_RX_FCS_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "FCS mode is 32bit in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "FCS mode is 16bit in Rx direction\n"));
   }
   else if (WUFEI_CES_RX_MODE_TYPE_V(rcpt_reg) == WUFEI_PHY_MODE_POS)
      WUFE_PRINT_DISP((wufei_log, "The RCPT mode type is POS (HDLC byte resolution)\n"));
   else if (WUFEI_ATM_RX_MODE_TYPE_V(rcpt_reg) == WUFEI_PHY_MODE_ATM)
   {
      WUFE_PRINT_DISP((wufei_log, "The RCPT mode type is ATM in Rx direction\n"));
      if (WUFEI_ATM_RX_MODE_EN_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The phy is enabled in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The phy is disabled in Rx direction\n"));

      if (WUFEI_ATM_RX_DESCRAMBLER_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The Receiver descrambler is on in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The Receiver descrambler is off in Rx direction\n"));

      if (WUFEI_ATM_RX_COSET_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The COSET is added to the HEC before comparison in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The COSET is not added to the HEC before comparison in Rx direction\n"));

      if (WUFEI_ATM_RX_IDLE_CELL_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "Idle cells are discarded in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "Idle cells are not discarded in Rx direction\n"));

      if (WUFEI_ATM_RX_UCD_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "Unsigned cells are discarded  in Rx direction in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "Unsigned cells are not discarded in Rx direction in Rx direction\n"));

      if (WUFEI_ATM_RX_HECD_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "HEC error cells are discarded in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "HEC error cells are not discarded in Rx direction\n"));

      if (WUFEI_ATM_RX_HEN_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "Single bit HEC error correction is enabled in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "Single bit HEC error correction is disabled in Rx direction\n"));

      if (WUFEI_ATM_RX_CDM_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "Cell delineation mode is Nibble resolution in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "Cell delineation mode is Byte resolution in Rx direction\n"));

      rcpt_reg = rpt_i_value[MAC_NUM_ACTUAL_REGS_PER_PT-1];
      WUFE_PRINT_DISP((wufei_log, "The Delta is %d in Rx direction\n", (WUFEI_ATM_RX_DELTA_V(rcpt_reg) +1)));
      WUFE_PRINT_DISP((wufei_log, "The Alpha is %d in Rx direction\n", (WUFEI_ATM_RX_ALPHA_V(rcpt_reg) +1)));
   }
   else if (WUFEI_CES_RX_MODE_TYPE_V(rcpt_reg) == WUFEI_PHY_MODE_TRANS)
   {
      WUFE_PRINT_DISP((wufei_log, "The RCPT mode type is TRANS in Rx direction\n"));
      WUFE_PRINT_DISP((wufei_log, "NSPP is:  %x\n", WUFEI_CES_RX_MODE_NSPP_V(rcpt_reg)));
      if (WUFEI_CES_RX_MODE_EN_V(rcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The phy is enabled in Rx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The phy is disabled in Rx direction\n"));

      rcpt_reg = rpt_i_value[MAC_NUM_ACTUAL_REGS_PER_PT-1];
      WUFE_PRINT_DISP((wufei_log, "Data unit size is:  %x in Rx direction\n", WUFEI_CES_RX_MODE_DATA_U_SIZE_V(rcpt_reg)));
   }
   else if (WUFEI_CES_RX_MODE_TYPE_V(rcpt_reg) == WUFEI_PHY_MODE_GFP)
      WUFE_PRINT_DISP((wufei_log, "The RCPT mode type is GFP in Rx direction\n"));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwTptDisplay
 *
 * Purpose: Displays the TSPT and TCPT for a specific phy
 *
 * Description:
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyDisplay
 *
 ****************************************************************************/
WUFE_status WUFEI_HwTptDisplay(WUFEI_system_registry* system_registry,
                               WP_U32 coreid,
                               WP_U32 phy_id)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 offset, tcpt_reg, i;
   WP_U16 tpt_i_value[MAC_NUM_ACTUAL_REGS_PER_PT];
   WUFE_status status;

   offset = phy_id * MAC_NUM_REGS_PER_PT;

   WUFEI_HwMachineIdirectAddrSet(id, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));

   for(i=0; i<MAC_NUM_ACTUAL_REGS_PER_PT ; i++)
   {
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(id, coreid, &tpt_i_value[i]));
   }

   WUFE_PRINT_DISP((wufei_log, "\nTSPT :0x%x \n", phy_id));
   for(i=0; i<MAC_NUM_ACTUAL_REGS_PER_PT ; i++)
   {
      if (i==MAC_NUM_ACTUAL_REGS_PER_PT-2)
         WUFE_PRINT_DISP((wufei_log, "\nTCPT:0x%x \n", phy_id));
      WUFE_PRINT_DISP((wufei_log, "0x%02x: 0x%04X\n",i, tpt_i_value[i]));
   }
   tcpt_reg = tpt_i_value[MAC_NUM_ACTUAL_REGS_PER_PT-2];
   WUFE_PRINT_DISP((wufei_log, "NSPP is:  %x\n", WUFEI_CES_TX_MODE_NSPP_V(tcpt_reg)));
   if (WUFEI_CES_TX_MODE_EN_V(tcpt_reg))
      WUFE_PRINT_DISP((wufei_log, "The TCPT enable bit is on\n"));
   else
      WUFE_PRINT_DISP((wufei_log, "The TCPT enable bit is off\n"));

   if (WUFEI_CES_TX_MODE_TYPE_V(tcpt_reg) == WUFEI_PHY_MODE_HDLC_BIT)
   {
      WUFE_PRINT_DISP((wufei_log, "The TCPT mode type is HDLC bit resolution\n"));
      if (WUFEI_HDLC_TX_MODE_EN_V(tcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The phy is enabled in Tx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The phy is disabled in Tx direction\n"));
      if (WUFEI_HDLC_TX_FCS_V(tcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "FCS mode is 32bit in Tx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "FCS mode is 16bit in Tx direction\n"));
      if (WUFEI_HDLC_TX_IDLE_V(tcpt_reg))
      {
         WUFE_PRINT_DISP((wufei_log, "flags are transmitted between tests in Tx direction\n"));
         WUFE_PRINT_DISP((wufei_log, "minimum number of flags between frames is %d\n",WUFEI_HDLC_TX_NOF_V(tcpt_reg)));
      }
      else
         WUFE_PRINT_DISP((wufei_log, "only close and open flags are transmitted and ones between them  in Tx direction\n"));
   }
   else if (WUFEI_CES_RX_MODE_TYPE_V(tcpt_reg) == WUFEI_PHY_MODE_POS)
      WUFE_PRINT_DISP((wufei_log, "The TCPT mode type is POS (HDLC byte resolution)\n"));
   else if (WUFEI_ATM_TX_MODE_TYPE_V(tcpt_reg) == WUFEI_PHY_MODE_ATM)
   {
      WUFE_PRINT_DISP((wufei_log, "The TCPT mode type is ATM\n"));
      if (WUFEI_ATM_TX_MODE_EN_V(tcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The phy is enabled in Tx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The phy is disabled in Tx direction\n"));

      if (WUFEI_ATM_TX_SCRAMBLER_V(tcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "The scrambler is on in Tx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "The scrambler is off in Tx direction\n"));

      if (WUFEI_ATM_TX_COSET_V(tcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "COSET is added to the generated HEC in Tx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "COSET is not added to the generated HEC in Tx direction\n"));

      if (WUFEI_ATM_TX_CLP_V(tcpt_reg))
         WUFE_PRINT_DISP((wufei_log, "CLP bit in ATM cell header is 1 in Tx direction\n"));
      else
         WUFE_PRINT_DISP((wufei_log, "CLP bit in ATM cell header is 0 in Tx direction\n"));

   }
   else if (WUFEI_CES_TX_MODE_TYPE_V(tcpt_reg) == WUFEI_PHY_MODE_TRANS)
      WUFE_PRINT_DISP((wufei_log, "The TCPT mode type is TRANS\n"));
   else if (WUFEI_CES_TX_MODE_TYPE_V(tcpt_reg) == WUFEI_PHY_MODE_GFP)
      WUFE_PRINT_DISP((wufei_log, "The TCPT mode type is GFP\n"));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineEventRegSet
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
void WUFEI_HwMachineEventRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value, WP_U16 direction)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr=0;

   if (direction & WUFE_RX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_rx_interrupt.value;
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_tx_interrupt.value;
   }

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineEventRegGet
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
 *     value - the value to set the register into it.
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwMachineEventRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value, WP_U16 direction)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr =0;

   if (direction & WUFE_RX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_rx_interrupt.value;
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_tx_interrupt.value;
   }

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineEventMaskRegSet
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
void WUFEI_HwMachineEventMaskRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value, WP_U16 direction)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr =0;

   if (direction & WUFE_RX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_rx_interrupt_mask.value;
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_tx_interrupt_mask.value;
   }

   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineEventMaskRegGet
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
void WUFEI_HwMachineEventMaskRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value, WP_U16 direction)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr =0;

   if (direction & WUFE_RX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_rx_interrupt_mask.value;
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_tx_interrupt_mask.value;
   }

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineRxPortIdGet
 *
 * Purpose:   Getter for the phy id register of the rx interrupts
 *
 *
 * Description:
 *     This function gets the phy id of the event into the value passed to the function.
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
void WUFEI_HwMachineRxPortIdGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_rx_int_port_id.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwMachineTxPortIdGet
 *
 * Purpose:   Getter for the phy id register of the tx interrupts
 *
 *
 * Description:
 *     This function gets the phy id of the event into the value passed to the function.
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
void WUFEI_HwMachineTxPortIdGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16* event_reg_ptr;

   event_reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_tx_int_port_id.value;

   WUFEI_REG_GET(event_reg_ptr, value);
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwPosCrcReconfig
 *
 * Purpose: Reconfigure the POS Machine CRC size parameter
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *     crc_size            The new CRC Size to be configured.
 *
 * Outputs:
 *     Changes in the RCPT or TCPT of the POS phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwPosCrcReconfig(WP_U32 ufeid,
                                   WP_U32 coreid,
                                   WP_U32 phy_id,
                                   WP_U32 direction,
                                   WP_U32 crc_size)
{
   WP_U16 mode, offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   if (direction & WUFE_RX_DIRECTION)
   {
      /* Read RCPT and modify according to new CRC size*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_POS_RX_FCS_M;
      mode |= WUFEI_POS_RX_FCS_F(crc_size);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Read TCPT and modify according to new CRC size*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_POS_TX_FCS_M;
      mode |= WUFEI_POS_TX_FCS_F(crc_size);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwPosDescramblerReconfig
 *
 * Purpose: Reconfigure the POS Machine scrambling
 *
 *
 * Description:
 *
 * Inputs:
 *     id                          The UFE id
 *     coreid                      Which core (0/1) does the phy belong to
 *     phyid                       The id of the phy to reconfigure
 *     direction                   The direction in which the phy should be enabled:
 *                                      WUFE_RX_DIRECTION = 1,
 *                                      WUFE_TX_DIRECTION = 2,
 *                                      WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *     scrambler_enable            The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the RCPT or TCPT of the POS phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/

WUFE_status WUFEI_HwPosScramblingReconfig(WP_U32 ufeid,
                                         WP_U32 coreid,
                                         WP_U32 phy_id,
                                         WP_U32 direction,
                                         WP_U32 scrambler_enable)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }
   if (direction & WUFE_RX_DIRECTION)
   {
      /* Read RCPT and modify according to new Rx descrambler configureation*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_POS_RX_DEN_M;
      mode |= WUFEI_POS_RX_DEN_F(scrambler_enable);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Read TCPT and modify according to new Tx scrambler configuration*/
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

      mode &= ~WUFEI_POS_TX_SEN_M;
      mode |= WUFEI_POS_TX_SEN_F(scrambler_enable);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         mode,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwPosMinFlagsReconfig
 *
 * Purpose: Reconfigure the POS Machine minimum flags between frames parameter
 *
 *
 * Description:
 *
 * Inputs:
 *     id                  The UFE id
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to reconfigure
 *     min_flags           The new parameter to be configured.
 *
 * Outputs:
 *     Changes in the TCPT of the POS phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFE_PhyReconfigure
 *
 ****************************************************************************/
WUFE_status WUFEI_HwPosMinFlagsReconfig(WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 min_flags)
{
   WP_U16 mode, offset;
   WUFE_status status;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_NUM_REGS_PER_SPT;

   /* Read TCPT and modify according to new MIN FLAGS value */
   WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &mode));

   mode &= ~WUFEI_POS_TX_NOF_INTER_FLAGS_M;
   mode |= WUFEI_POS_TX_NOF_INTER_FLAGS_F(min_flags);

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                      (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                      mode,
                                                      1));

   return WUFE_OK;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwPosPhyCreate
 *
 * Purpose: Initializes the POS Machine tables for a specific phy
 *
 *
 * Description:
 *
 * Inputs:
       system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     ufe_phy_cfg         Struct with all the defined parameters of the phy
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     Initializing the registers of the RSPT/RCPT and TSPT/TCPT for the HDLC phy
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_PhyCreate
 *
 ****************************************************************************/
WUFE_status WUFEI_HwPosPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_phy *ufe_phy_cfg,
                                 WP_U32 direction)
{
   WP_U32 id = system_registry->wufe_id;
   WP_U16 mode, offset;
   WUFE_status status;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   offset = phy_id * MAC_NUM_REGS_PER_PT;

   if (direction & WUFE_RX_DIRECTION)
   {
      /* Set RSPT */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         0x0000,
                                                         6));
      WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwMachineIdirectDataSet(id, coreid, 0xffff));
      WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwMachineIdirectDataSet(id, coreid, 0xffff));
      /* Set RCPT */

      mode = WUFEI_POS_RX_MODE_TYPE_F(WUFEI_PHY_MODE_POS)             |
         WUFEI_POS_RX_FCS_F(ufe_phy_cfg->type.pos.rx_frame_crc_size) |
         WUFEI_POS_RX_DEN_F(ufe_phy_cfg->type.pos.rx_descrambler_mode);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
   }

   if (direction & WUFE_TX_DIRECTION)
   {
      /* Set TSPT */
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(id, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         0xffff,
                                                         2));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));

      /* Set TCPT */

      mode = WUFEI_POS_TX_MODE_TYPE_F(WUFEI_PHY_MODE_POS)                  |
             WUFEI_POS_TX_FCS_F(ufe_phy_cfg->type.pos.tx_frame_crc_size)  |
             WUFEI_POS_TX_SEN_F(ufe_phy_cfg->type.pos.tx_scrambler_mode)  |
             WUFEI_POS_TX_NOF_INTER_FLAGS_F(ufe_phy_cfg->type.pos.tx_min_flag_num-1);

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, mode));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataSet(id, coreid, 0x0000));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwPosPhyEnable
 *
 * Purpose: Enables the POS Machine for a specific phy
 *
 * Description: Setting the enable bit in the RPT table
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     Setting the enable field in the Hdlc indirect TSPT/RSPT table
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwPosPhyEnable(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   /*Calculate phy entry in the parameter tables*/
   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_POS_RX_MODE_EN_M;

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value |= WUFEI_POS_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwPosPhyDisable
 *
 * Purpose: Disable the POS Machine for a specific phy
 *
 * Description: Clearing the enable bit in the RPT table
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     coreid              Which core (0/1) does the phy belong to
 *     phyid               The id of the phy to enable
 *     direction           The direction in which the phy should be enabled:
 *                           WUFE_RX_DIRECTION = 1,
 *                           WUFE_TX_DIRECTION = 2,
 *                           WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
 *
 * Outputs:
 *     Clearing the enable field in the Hdlc indirect TSPT/RSPT table
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_PhyDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwPosPhyDisable(WUFEI_system_registry* system_registry,
                                  WP_U32 coreid,
                                  WP_U32 phy_id,
                                  WP_U32 direction)
{
   WUFE_status status;
   WP_U16 reg_value, offset;
   WP_U32 ufeid = system_registry->wufe_id;

   offset = (phy_id * MAC_NUM_REGS_PER_PT) + MAC_ENB_REG_OFFSET;

   if (direction > WUFE_FULL_DUPLEX)
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   if (direction & WUFE_RX_DIRECTION)
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_RPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_POS_RX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_RPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   if (direction & WUFE_TX_DIRECTION )
   {
      WUFEI_HwMachineIdirectAddrSet(ufeid, coreid, WUFEI_INDIR_MAC_TPT_TABLE(offset));
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectDataGet(ufeid, coreid, &reg_value));
      reg_value &= ~WUFEI_POS_TX_MODE_EN_M;
      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwMachineIdirectMemset(ufeid, coreid,
                                                         (WP_U32)WUFEI_INDIR_MAC_TPT_TABLE(offset),
                                                         reg_value,
                                                         1));
   }

   return WUFE_OK;
}

static void WUFEI_HwMachineSetTxThreshold(WP_U32 ufeid, WP_U16 coreid)
{
   WP_U32 machine_base = WUFEI_UfeMachineBaseAddrGet(ufeid, coreid);
   WP_U16* reg_ptr;
   WP_U32 i;
   WP_U16 value[WUFEI_HW_MACHINE_TX_START_THRESH_OPTIONS] = {WUFEI_HW_MACHINE_TX_START_THRESH_ATM_OC3,
                                                             WUFEI_HW_MACHINE_TX_START_THRESH_ATM_OC12,
                                                             WUFEI_HW_MACHINE_TX_START_THRESH_POS_OC3,
                                                             WUFEI_HW_MACHINE_TX_START_THRESH_POS_OC12};

   for (i=0; i<WUFEI_HW_MACHINE_TX_START_THRESH_OPTIONS; i++)
   {
      reg_ptr = &((WUFEI_hw_machine*)machine_base)->machine_tx_start_tx_threshold[i].value;
      WUFEI_REG_SET(reg_ptr, value[i]);
   }
}

WUFE_status WUFEI_HwMachineSetup(WP_U32 ufeid, WP_U16 coreid)
{
   WUFEI_HwMachineSetTxThreshold(ufeid, coreid);

   return WUFE_OK;
}
