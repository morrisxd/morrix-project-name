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

static WUFE_status WUFEI_HwEmphyRxPrioritySetup(WUFEI_system_registry* system_registry,
                                                WP_U32 coreid);

static WUFE_status WUFEI_HwEmphyTxPrioritySetup(WUFEI_system_registry* system_registry,
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
static WP_U32 WUFEI_UfeEmphyRegBaseAddrGet(WP_U32 ufeid)
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
   WUFEI_hw_emphy_n_direct *emphy_direct_ptr;
   WP_U16* event_reg_ptr;

   emphy_direct_ptr =  &emphy_regs_base->emphy_direct_reg[coreid];
   event_reg_ptr = &emphy_direct_ptr->event_phy_nv.value;
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
   WUFEI_hw_emphy_n_direct *emphy_direct_ptr;
   WP_U16* event_reg_ptr;

   emphy_direct_ptr =  &emphy_regs_base->emphy_direct_reg[coreid];
   event_reg_ptr = &emphy_direct_ptr->event_reg.value;
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
   WUFEI_hw_emphy_n_direct *emphy_direct_ptr;
   WP_U16* event_reg_ptr;

   emphy_direct_ptr = &emphy_regs_base->emphy_direct_reg[coreid];
   event_reg_ptr = &emphy_direct_ptr->event_reg.value;
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
   WUFEI_hw_emphy_n_direct *emphy_direct_ptr;
   WP_U16* event_reg_ptr;

   emphy_direct_ptr = &emphy_regs_base->emphy_direct_reg[coreid];
   event_reg_ptr = &emphy_direct_ptr->event_mask_reg.value;
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
   WUFEI_hw_emphy_n_direct *emphy_direct_ptr;
   WP_U16* event_reg_ptr;

   emphy_direct_ptr = &emphy_regs_base->emphy_direct_reg[coreid];
   event_reg_ptr = &emphy_direct_ptr->event_mask_reg.value;
   WUFEI_REG_GET(event_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyRxPrioritySet
 *
 * Purpose:   Helper to Initialize the Emphy Rx Priority
 *
 *
 * Description:
 *     This function intializes the rx priority tables
 *
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *     descriptor - the priority num
 *     value - the value to set in the register
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     WUFEI_HwEmphyRxPrioritySetup.
 *
 ****************************************************************************/
static void WUFEI_HwEmphyRxPrioritySet(WP_U32 ufeid, WP_U32 coreid,
                                       WP_U32 descriptor, WP_U8 value)
{
   /* provide read modify write for 16 bit access */
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_n_direct* emphy_direct;
   WP_U16* rx_priority_desc_ptr;
   WP_U16 tmp;

   emphy_direct = &emphy_regs_base->emphy_direct_reg[coreid];
   rx_priority_desc_ptr = &emphy_direct->rx_priority_desc[descriptor/2].value;

   WUFEI_REG_GET(rx_priority_desc_ptr, &tmp);

   if(descriptor % 2)
      tmp = (tmp & 0xff00) | value;
   else
      tmp = (tmp & 0xff) | (value << 8);

   WUFEI_REG_SET(rx_priority_desc_ptr, tmp);

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

   /*Prepare core1 port id offset register only in mux mode*/
   if (WUFEI_SystemHwConfigEmphyModeGet(system_registry) == WUFEI_EMPHY_SETUP_MODE_MUX &&
       coreid == WUFE_CORE_ID_1)
   {
      WUFEI_HwEmphyCore1PortIdOffsetRegSetup(system_registry);
   }

   /*Mode register is prepared in one of two options: Either in Dual mode for both Cores,
    Or in Mux mode for core0 only */
   if ((WUFEI_SystemHwConfigEmphyModeGet(system_registry) == WUFEI_EMPHY_SETUP_MODE_DUAL) ||
       ((WUFEI_SystemHwConfigEmphyModeGet(system_registry) == WUFEI_EMPHY_SETUP_MODE_MUX) &&
        (coreid == WUFE_CORE_ID_0)))
   {
      /* Prepare mode register*/
      WUFEI_HwEmphyModeRegSetup(system_registry,
                                coreid);
   }

   /* Prepare Rx priority table for EMPHY*/
   WUFEI_HwEmphyRxPrioritySetup(system_registry,
                                coreid);

   /* Prepare Tx priority table for EMPHY*/
   WUFEI_HwEmphyTxPrioritySetup(system_registry,
                                coreid);

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
   WUFEI_hw_emphy_n_direct *emphy_direct_ptr;
   WP_U16* mode_reg_ptr,value;

   if((system_registry->cfg.emphy.tx_parity_check>1) ||
      (system_registry->cfg.emphy.tx_parity_mode >1) ||
      (system_registry->cfg.emphy.rx_parity_mode >1))
      return WUFE_ERR_ILLEGAL_EMPHY_PARITY;

   value =
      (WUFEI_EMPHY_TX_PARITY_CHECK_F(system_registry->cfg.emphy.tx_parity_check) |
       WUFEI_EMPHY_TX_PARITY_EVEN_F (system_registry->cfg.emphy.tx_parity_mode)  |
       WUFEI_EMPHY_RX_PARITY_EVEN_F (system_registry->cfg.emphy.rx_parity_mode));

   emphy_direct_ptr = &emphy_regs_base->emphy_direct_reg[coreid];
   mode_reg_ptr = &emphy_direct_ptr->mode.value;
   WUFEI_REG_SET(mode_reg_ptr, value);

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyRxPrioritySetup
 *
 * Purpose:   configure the EMPHY rx priority tables
 *
 *
 * Description:
 *     This function intializes the rx priority registers of EMPHY.
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *     coreid  - the id of the core.
 *
 * Outputs:
 *     write to the rx priority registers.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFEI_HwEmphySetup.
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwEmphyRxPrioritySetup(WUFEI_system_registry* system_registry,
                                                WP_U32 coreid)
{
   WP_U32 descriptor, i, ufeid = system_registry->wufe_id;
   WP_U8 value;

   /* Prepare Rx priority table */
   for (descriptor=0;descriptor<WUFE_N_PRI_DESCRIPTOR;descriptor++)
   {
      value=0;

      for(i=0;i<WUFE_PRI_DESCRIPTOR_LENGTH;i++)
      {
         /* check this is monotonic rising function
          * all values are between 0 and 3
          * check there is difference between the first and last value.
          */
         if (system_registry->cfg.emphy.rx_pri_table[descriptor][i] > 3)
         {
            return WUFE_ERR_ILLEGAL_EMPHY_PRI_RX_VALUE;
         }
         if(i !=0)
         {
            if (system_registry->cfg.emphy.rx_pri_table[descriptor][i]<
                system_registry->cfg.emphy.rx_pri_table[descriptor][i-1])
            {/* check increasing */
               WUFE_PRINT_ERR((wufei_log,"WUFE: RX PRI %d: (val %d) <  %d: (val %d)\n",
                               i,  system_registry->cfg.emphy.rx_pri_table[descriptor][i],
                               i-1,system_registry->cfg.emphy.rx_pri_table[descriptor][i-1]));
               return WUFE_ERR_ILLEGAL_EMPHY_PRI_RX_VALUE;
            }
         }
         if (i == (WUFE_PRI_DESCRIPTOR_LENGTH-1))
         {
            if (system_registry->cfg.emphy.rx_pri_table[descriptor][0] ==
                system_registry->cfg.emphy.rx_pri_table[descriptor][i])
            {/* check the last is really bigger than the first */
               WUFE_PRINT_ERR((wufei_log,"WUFE: RX PRI %d: (val %d) ==  %d: (val %d)\n",
                               0, system_registry->cfg.emphy.rx_pri_table[descriptor][0],
                               i, system_registry->cfg.emphy.rx_pri_table[descriptor][i]));
               return WUFE_ERR_ILLEGAL_EMPHY_PRI_RX_VALUE;
            }
         }

         value +=
            (system_registry->cfg.emphy.rx_pri_table[descriptor][i] << (
               i * WUFE_SYS_EMPHY_ENTRY_SIZE));

      }

      WUFEI_HwEmphyRxPrioritySet(ufeid, coreid, descriptor, value);

   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwEmphyTxPrioritySetup
 *
 * Purpose:   configure the EMPHY Tx priority tables
 *
 *
 * Description:
 *     This function intializes the Tx priority registers of EMPHY.
 *
 * Inputs:
 *     system_registry - The registry entry created for this UFE.
 *     coreid  - the id of the core.
 *
 * Outputs:
 *     write to the Tx priority registers.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 *
 * Called by:
 *     WUFEI_HwEmphySetup.
 *
 ****************************************************************************/
static WUFE_status WUFEI_HwEmphyTxPrioritySetup(WUFEI_system_registry* system_registry,
                                                WP_U32 coreid)
{
   WP_U32 descriptor, i;
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
   WUFEI_hw_emphy_n_direct *emphy_direct_ptr;
   WP_U16* tx_priority_ptr;
   WP_U16 value;


   for (descriptor = 0; descriptor < WUFE_N_PRI_DESCRIPTOR; descriptor++)
    {

      value=0;

      for(i=0;i<WUFE_PRI_DESCRIPTOR_LENGTH;i++)
      {

         if (system_registry->cfg.emphy.tx_pri_table[descriptor][i] > 3)
         {
            return WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE;
         }
         if (system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i] > 3)
         {
            return WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_INFRAME_VALUE;
         }

         /* check this is monotonic rising function
          * all values are between 0 and 3
          * check there is difference between the first and last value.
          */
         if(i !=0)
         {
            if (system_registry->cfg.emphy.tx_pri_table[descriptor][i]<
                system_registry->cfg.emphy.tx_pri_table[descriptor][i-1])
            {/* check increaseing */
               WUFE_PRINT_ERR((wufei_log,"WUFE: TX PRI %d: (val %d) <  %d: (val %d)\n",
                               i,  system_registry->cfg.emphy.tx_pri_table[descriptor][i],
                               i-1,system_registry->cfg.emphy.tx_pri_table[descriptor][i-1]));
               return WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE;
            }
            if (system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i]<
                system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i-1])
            {/* check increaseing */
               WUFE_PRINT_ERR(
                  (wufei_log,"WUFE: TX in frame PRI %d: (val %d) <  %d: (val %d)\n",
                   i,  system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i],
                   i-1,system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i-1]));
               return WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE;
            }

         }
         if (i == (WUFE_PRI_DESCRIPTOR_LENGTH-1))
         {
            if (system_registry->cfg.emphy.tx_pri_table[descriptor][0] ==
                system_registry->cfg.emphy.tx_pri_table[descriptor][i])
            {/* check the last is really bigger than the first */
               WUFE_PRINT_ERR((wufei_log,"WUFE: TX PRI %d: (val %d) ==  %d: (val %d)\n",
                                0, system_registry->cfg.emphy.tx_pri_table[descriptor][0],
                                i, system_registry->cfg.emphy.tx_pri_table[descriptor][i]));
                return WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE;
            }
            if (system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][0] ==
                system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i])
            {/* check the last is really bigger than the first */
               WUFE_PRINT_ERR(
                   (wufei_log,"WUFE: TX in frame  PRI %d: (val %d) ==  %d: (val %d)\n",
                    0, system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][0],
                    i, system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i]));
                return WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE;
            }
         }

         if (system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i] <
             system_registry->cfg.emphy.tx_pri_table[descriptor][i])
         {/* check the last is really bigger than the first */
            WUFE_PRINT_ERR(
               (wufei_log,"WUFE: TX PRI inframe  %d: (val %d) <  %d: (val %d)\n",
                i, system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i],
                i, system_registry->cfg.emphy.tx_pri_table[descriptor][i]));
            return WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE;
         }

         value +=
            (system_registry->cfg.emphy.tx_pri_table[descriptor][i] << (
               i*WUFE_SYS_EMPHY_ENTRY_TX_SIZE));
         value +=
            (system_registry->cfg.emphy.tx_inframe_pri_table[descriptor][i] << (
               i * WUFE_SYS_EMPHY_ENTRY_TX_SIZE + WUFE_SYS_EMPHY_ENTRY_SIZE));

      }

      emphy_direct_ptr = &emphy_regs_base->emphy_direct_reg[coreid];
      tx_priority_ptr = &emphy_direct_ptr->tx_priority_desc[descriptor].value;
      WUFEI_REG_SET(tx_priority_ptr,value);

   }
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
   WUFEI_hw_emphy_n_direct * emphy_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);

   if((WUFEI_SystemHwConfigEmphyModeGet(system_registry) == WUFEI_EMPHY_SETUP_MODE_MUX) &&
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

   emphy_ptr = &emphy_regs_base->emphy_direct_reg[coreid];

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
   WUFEI_hw_emphy_n_direct *emphy_ptr;
   WP_U32 ufeid = system_registry->wufe_id;
   WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);

   if((WUFEI_SystemHwConfigEmphyModeGet(system_registry) == WUFEI_EMPHY_SETUP_MODE_MUX) &&
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

   emphy_ptr = &emphy_regs_base->emphy_direct_reg[coreid];

   /* read-modify-write enable bits in the emphy mode register*/
   WUFEI_REG_BIT_CLEAR(&emphy_ptr->mode.value, enb_mask);


   return WUFE_OK;
}
