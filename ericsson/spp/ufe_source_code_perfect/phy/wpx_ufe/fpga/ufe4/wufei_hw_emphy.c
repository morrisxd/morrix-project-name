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

#ifndef WUFEI_HW_EMPHY_H
#include "wufei_hw_emphy.h"
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

#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif

/*internal functions*/
static WUFE_status WUFEI_HwEmphyCore1PortIdOffsetRegSetup(WUFEI_system_registry* system_registry);

static WUFE_status WUFEI_HwEmphyModeRegSetup(WUFEI_system_registry* system_registry,
                                             WP_U32 coreid);
static WUFE_status WUFEI_HwEmphyPortTableThresholdSetup(WUFEI_system_registry* system_registry,
                                                        WP_U32 coreid);

/*****************************************************************************
 *
 * Function:  WUFEI_UfeEmphyRegBaseAddrGet
 *
 * Purpose:   returns the base adress of the emphy registers
 *
 *
 * Description:
 *     This function returns the base adress of the emphy registers
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *
 * Return Value:
 *     the base adress of the EMPHY.
 *
 * Called by:
 *     Any function (actually by most functions in this file).
 *
 ****************************************************************************/
WP_U32 WUFEI_UfeEmphyRegBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)&(base_addr->emphy);
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyPhyNotValidGet
 *
 * Purpose:   gets the non valid interrupt register
 *
 *
 * Description:
 *     This function puts the non valid interrupt register in the value parameter.
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *     value - the pointer that contains the value to put in the result.
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwEmphyPhyNotValidGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value)
{
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_mode *emphy_mode_ptr;
   WP_U16* event_reg_ptr;

   emphy_mode_ptr =  &emphy_regs_base->emphy_mode_reg[coreid];
   event_reg_ptr = &emphy_mode_ptr->event_phy_non_valid.value;
   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyEventRegSet
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
void WUFEI_HwEmphyEventRegSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value)
{
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_event *emphy_event_ptr;
   WP_U16* event_reg_ptr;

   emphy_event_ptr =  &emphy_regs_base->emphy_event_reg[coreid];
   event_reg_ptr = &emphy_event_ptr->event_reg.value;
   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyEventRegGet
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
void WUFEI_HwEmphyEventRegGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value)
{
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_event *emphy_event_ptr;
   WP_U16* event_reg_ptr;

   emphy_event_ptr = &emphy_regs_base->emphy_event_reg[coreid];
   event_reg_ptr = &emphy_event_ptr->event_reg.value;
   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyEventMaskRegSet
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
void WUFEI_HwEmphyEventMaskRegSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value)
{
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_event *emphy_event_ptr;
   WP_U16* event_reg_ptr;

   emphy_event_ptr = &emphy_regs_base->emphy_event_reg[coreid];
   event_reg_ptr = &emphy_event_ptr->event_mask_reg.value;
   WUFEI_REG_SET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyEventMaskRegGet
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
void WUFEI_HwEmphyEventMaskRegGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value)
{
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_event *emphy_event_ptr;
   WP_U16* event_reg_ptr;

   emphy_event_ptr = &emphy_regs_base->emphy_event_reg[coreid];
   event_reg_ptr = &emphy_event_ptr->event_mask_reg.value;
   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphySetup
 *
 * Purpose:   configure the UFE EMPHY HW Registers
 *
 *
 * Description:
 *     This function intializes HW registers of EMPHY module
 *
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *     coreid - The id of the core.
 *
 * Outputs:
 *     write to external HW EMPHY registers.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFEI_SystemEmphySetup.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwEmphySetup(WUFEI_system_registry* system_registry, WP_U32 coreid)
{

	WUFE_status status;

   /*Prepare core1 port id offset register only in mux mode*/
   if (WUFEI_SystemHwConfigEmphyModeGet(system_registry) == WUFEI_EMPHY_SETUP_MODE_MUX &&
       coreid == WUFE_CORE_ID_1)
   {
	   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwEmphyCore1PortIdOffsetRegSetup(system_registry));
   }

   /*Mode and threshold registers are prepared in one of two options: Either in Dual mode for both Cores,
    Or for core0 only in other modes */
   if ((WUFEI_SystemHwConfigEmphyModeGet(system_registry) == WUFEI_EMPHY_SETUP_MODE_DUAL) ||
       (coreid == WUFE_CORE_ID_0))
   {

	   /* Prepare mode and threshold registers*/
	   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwEmphyModeRegSetup(system_registry, coreid));
   }

   WUFEI_RETURN_IF_ERROR(status,WUFEI_HwEmphyPortTableThresholdSetup(system_registry,
                                        coreid));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyCore1PortIdOffsetRegSetup
 *
 * Purpose:   configure the Core1 offset EMPHY Register
 *
 *
 * Description:
 *     This function intializes the core1 offset HW register of EMPHY.
 *     It sets the offset value to the max phys if it is lower than 4k
 *     or else we set  the highest configuration.
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *
 *
 * Outputs:
 *     write to the core1 offset register.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFEI_HwEmphySetup.
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwEmphyCore1PortIdOffsetRegSetup(WUFEI_system_registry* system_registry)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WP_U16* core1_offset_ptr = &emphy_regs_base->core1_port_id_offset.value;
   WP_U16 value;

   value = WUFEI_EMPHY_PORT_ID_OFFSET_F(system_registry->core1_phyid_offset);
   WUFEI_REG_SET(core1_offset_ptr,value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyPortTableThresholdSetup
 *
 * Purpose:   configure the EMPHY port table threshold Register
 *
 *
 * Description:
 *     This function intializes the port table threshold register of EMPHY.
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *     coreid  - the id of the core.
 *
 * Outputs:
 *     write to the port table threshold register.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_HwEmphySetup.
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwEmphyPortTableThresholdSetup(WUFEI_system_registry* system_registry,
                                                        WP_U32 coreid)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_mode *emphy_mode_ptr;
   WP_U16* port_table_reg_ptr;

   emphy_mode_ptr = &emphy_regs_base->emphy_mode_reg[coreid];
   port_table_reg_ptr = &emphy_mode_ptr->ports_table_threshold.value;
   WUFEI_REG_SET(port_table_reg_ptr, WUFEI_EMPHY_PORT_TABLE_THERSHOLD);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyModeRegSetup
 *
 * Purpose:   configure the EMPHY general mode Register
 *
 *
 * Description:
 *     This function intializes the general mode register of EMPHY.
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *     coreid  - the id of the core.
 *
 * Outputs:
 *     write to the general mode register.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFEI_HwEmphySetup.
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwEmphyModeRegSetup(WUFEI_system_registry* system_registry,
                                             WP_U32 coreid)
{
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_mode *emphy_mode_ptr;
   WP_U16* mode_reg_ptr,value;


   if((system_registry->cfg.emphy.tx_parity_check>1) ||
      (system_registry->cfg.emphy.tx_parity_mode >1) ||
      (system_registry->cfg.emphy.rx_parity_mode >1))
      return WUFE_ERR_ILLEGAL_EMPHY_PARITY;


   if (system_registry->cfg.emphy.tx_parity_check != WUFE_TX_PARITY_IGNORE)
   {

	   return WUFE_ERR_PARITY_CHECK_NOT_SUPPORTED;
   }

   value =
      (WUFEI_EMPHY_TX_PARITY_CHECK_F(system_registry->cfg.emphy.tx_parity_check) |
       WUFEI_EMPHY_TX_PARITY_EVEN_F (system_registry->cfg.emphy.tx_parity_mode)  |
       WUFEI_EMPHY_RX_PARITY_EVEN_F (system_registry->cfg.emphy.rx_parity_mode));

   emphy_mode_ptr = &emphy_regs_base->emphy_mode_reg[coreid];
   mode_reg_ptr = &emphy_mode_ptr->mode.value;
   WUFEI_REG_SET(mode_reg_ptr, value);

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyEnable
 *
 * Purpose:   Enables the UFE EMPHY HW
 *
 *
 * Description:
 *     This function enables the EMPHY of the UFE system.
 *     The enable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *     coreid     The id of the core.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_SystemCoreEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwEmphyEnable(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 direction)
{
   WP_U16 enb_mask;
   WUFEI_hw_emphy_mode * emphy_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);

   if((WUFEI_SystemHwConfigEmphyModeGet(system_registry) != WUFEI_EMPHY_SETUP_MODE_DUAL) &&
      (coreid == WUFE_CORE_ID_1))
      return WUFE_OK;

   if (direction == WUFE_FULL_DUPLEX)
   {
      enb_mask=(WP_U16)(WUFEI_EMPHY_RX_MODE_ENABLE_M |
                        WUFEI_EMPHY_TX_MODE_ENABLE_M);
   }
   else if (direction == WUFE_RX_DIRECTION)
   {
      enb_mask=(WP_U16)WUFEI_EMPHY_RX_MODE_ENABLE_M;
   }
   else if (direction ==WUFE_TX_DIRECTION)
   {
      enb_mask=(WP_U16)WUFEI_EMPHY_TX_MODE_ENABLE_M;
   }
   else
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   emphy_ptr = &emphy_regs_base->emphy_mode_reg[coreid];

   /* read-modify-write on emphy mode register setup*/
   WUFEI_REG_BIT_SET(&emphy_ptr->mode.value, enb_mask);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyDisable
 *
 * Purpose:   Disables the UFE EMPHY HW
 *
 *
 * Descriptnnnnnion:
 *     This function disables the EMPHY of the UFE system.
 *     The disable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *     coreid     The id of the core.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_SystemCoreDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_HwEmphyDisable(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 direction)
{
   WP_U16 enb_mask;
   WUFEI_hw_emphy_mode *emphy_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);

   if((WUFEI_SystemHwConfigEmphyModeGet(system_registry) != WUFEI_EMPHY_SETUP_MODE_DUAL) &&
      (coreid == WUFE_CORE_ID_1))
      return WUFE_OK;

   if (direction == WUFE_FULL_DUPLEX)
   {
      enb_mask=~(WP_U16)(WUFEI_EMPHY_RX_MODE_ENABLE_M |
                         WUFEI_EMPHY_TX_MODE_ENABLE_M);
   }
   else if (direction ==WUFE_RX_DIRECTION)
   {
      enb_mask=~(WP_U16)(WUFEI_EMPHY_RX_MODE_ENABLE_M);
   }
   else if (direction ==WUFE_TX_DIRECTION)
   {
      enb_mask=~(WP_U16)(WUFEI_EMPHY_TX_MODE_ENABLE_M);
   }
   else
   {
      return WUFE_ERR_INVALID_DIRECTION;
   }

   emphy_ptr = &emphy_regs_base->emphy_mode_reg[coreid];

   /* read-modify-write enable bits in the emphy mode register*/
   WUFEI_REG_BIT_CLEAR(&emphy_ptr->mode.value, enb_mask);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyDisplay
 *
 * Purpose: Displays the Emphy mode register
 *
 * Inputs:
 *     ufe_id              UFE id
 *     coreid              core id
 * Outputs:
 *     Display to screen
 *
 * Return Value:
 *
 * Called by:
 *     WUFE_SystemDisplay
 *
 ****************************************************************************/
void WUFEI_HwEmphyDisplay(WP_U32 ufe_id, WP_U32 coreid)
{
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufe_id);
   WUFEI_hw_emphy_mode *emphy_mode_ptr;
   WP_U16* mode_reg_ptr,value;

   emphy_mode_ptr = &emphy_regs_base->emphy_mode_reg[coreid];
   mode_reg_ptr = &emphy_mode_ptr->mode.value;
   WUFEI_REG_GET(mode_reg_ptr, &value);

   WUFE_PRINT_DISP((wufei_log,"The EMPHY[%d] Mode Register: 0x%x\n",coreid,value));

   if (WUFEI_EMPHY_RX_MODE_ENABLE_V(value))
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] RX direction is enabled\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] RX direction is disabled\n",coreid));

   if (WUFEI_EMPHY_TX_MODE_ENABLE_V(value))
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] TX direction is enabled\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] TX direction is disabled\n",coreid));

   if (WUFEI_EMPHY_RX_PARITY_EVEN_V(value))
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] RX parity is odd\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] RX parity is even\n",coreid));

   if (WUFEI_EMPHY_TX_PARITY_EVEN_V(value))
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] TX parity is odd\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] TX parity is even\n",coreid));

   if (WUFEI_EMPHY_TX_PARITY_CHECK_V(value))
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] TX checks parity\n",coreid));
   else
      WUFE_PRINT_DISP((wufei_log,"EMPHY[%d] TX does not check parity\n",coreid));
}
