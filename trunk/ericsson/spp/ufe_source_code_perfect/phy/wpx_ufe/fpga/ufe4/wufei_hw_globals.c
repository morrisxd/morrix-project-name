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

#ifndef WUFE_SYSTEM_H
#include "wufe_system.h"
#endif

#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFEI_HW_FM_H
#include "wufei_hw_globals.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_HW_EMPHY_INT_H
#include "wufei_hw_emphy_int.h"
#endif

WP_CHAR wufei_log[400];

extern WP_U8 debug_flag;

WP_U32 WUFEI_UfeGenRegBaseAddrGet(WP_U32 ufeid);
static WP_U16 WUFEI_UfeCoreReleaseIdGet(WP_U32 ufeid);
static WP_U16 WUFEI_UfeCoreSecondaryReleaseIdGet(WP_U32 ufeid);
static WP_U16 WUFEI_UfeCoreConfigGet(WP_U32 ufeid);
WP_U16 WUFEI_UfeCoreSysConfigGet(WP_U32 ufeid);

/*****************************************************************************
 *
 * Function:  WUFEI_UfeTdmBaseAddrGet
 *
 * Purpose:   Getter for the base address of the TDM
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     The base address of the TDM
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_UfeTdmBaseAddrGet(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)base_addr->core[coreid].tdm;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeFmBaseAddrGet
 *
 * Purpose:   Getter for the base address of the Fifo Manager
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     The base address of the Fifo Manager
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_UfeFmBaseAddrGet(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)base_addr->core[coreid].fm;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeMachineBaseAddrGet
 *
 * Purpose:   Getter for the base address of the Machine block
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     The base address of the Machine block
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_UfeMachineBaseAddrGet(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)base_addr->core[coreid].machine;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeBsBaseAddrGet
 *
 * Purpose:   Getter for the base address of the Board Support
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     The base address of the Board Support
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_UfeBsBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)base_addr->bsp;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeGenRegsBaseAddrGet
 *
 * Purpose:   Getter for the base address of the Global Registers
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     The base address of the Global Registers
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_UfeGenRegBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)&(base_addr->globals);
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreReleaseIdGet
 *
 * Purpose: Getter for the primary release id
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the release id
 *
 * Called by:
 *    Any Function
 *
 ****************************************************************************/
static WP_U16  WUFEI_UfeCoreReleaseIdGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* release_id_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   release_id_ptr = &gen_regs_base->release_id.value;
   WUFEI_REG_GET(release_id_ptr, &val);

   return val;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreSecondaryReleaseIdGet
 *
 * Purpose: Getter for the secondary release id
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the release id
 *
 * Called by:
 *    Any Function
 *
 ****************************************************************************/
static WP_U16  WUFEI_UfeCoreSecondaryReleaseIdGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* release_id_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   release_id_ptr = &gen_regs_base->secondary_release_id.value;
   WUFEI_REG_GET(release_id_ptr, &val);

   return val;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreBuildNumGet
 *
 * Purpose: Getter for the build number
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the build number
 *
 * Called by:
 *    Any Function
 *
 ****************************************************************************/
WP_U16 WUFEI_UfeCoreBuildNumGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* build_num_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   build_num_ptr = &gen_regs_base->build_number.value;
   WUFEI_REG_GET(build_num_ptr, &val);

   return val;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreSysConfigGet
 *
 * Purpose: Getter for the system config global register
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the register
 *
 * Called by:
 *    Any Function
 *
 ****************************************************************************/
WP_U16 WUFEI_UfeCoreSysConfigGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* sys_config_reg_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   sys_config_reg_ptr = &gen_regs_base->sys_config.value;
   WUFEI_REG_GET(sys_config_reg_ptr, &val);

   return val;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreConfigGet
 *
 * Purpose: Getter for the core config global register
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the register
 *
 * Called by:
 *    Any Function
 *
 ****************************************************************************/
static WP_U16 WUFEI_UfeCoreConfigGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* core_config_reg_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   core_config_reg_ptr = &gen_regs_base->core_config.value;
   WUFEI_REG_GET(core_config_reg_ptr, &val);

   return val;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeFramerTypeGet
 *
 * Purpose: Getter for the framer type
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value: the framer type: either external or internal framer
 *
 * Called by:
 *    WUFE_SystemFramerTypeGet
 *
 ****************************************************************************/
WUFE_framer_type WUFEI_UfeFramerTypeGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* core_config_reg_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   core_config_reg_ptr = &gen_regs_base->sys_config.value;
   WUFEI_REG_GET(core_config_reg_ptr, &val);

   return WUFEI_UFE_FRAMER_MODE_V(val);
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCore1IncludedGet
 *
 * Purpose: Getter to determine if core 1 is included
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *    WUFEI_BsFpgaPllStatusGet
 *
 ****************************************************************************/
WP_U8 WUFEI_UfeCore1IncludedGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* core_config_reg_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   core_config_reg_ptr = &gen_regs_base->core_config.value;
   WUFEI_REG_GET(core_config_reg_ptr, &val);

   return WUFEI_UFE_CORE_1_INCLUDED_V(val);
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreGlobalIntEnable
 *
 * Purpose: Enables the global interrupts
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    Any Function
 *
 ****************************************************************************/
WUFE_status WUFEI_UfeCoreGlobalIntEnable(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_mask_int_reg_ptr;
   WP_U16 value;
   ((void)coreid);   
   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_mask_int_reg_ptr = &gen_regs_base->global_mask_int.value;

   value = ~(WUFEI_UFE_GLOBAL_MASK_INT_TDM0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_TDM1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_MACHINE0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_MACHINE1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_FM0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_FM1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_EMPHY0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_EMPHY1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_CR_M);
   WUFEI_REG_SET(global_mask_int_reg_ptr, value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreGlobalIntDisable
 *
 * Purpose: Disables the global interrupts
 *
 * Inputs:
 *     ufeid - the UFE id
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *    Any Function
 *
 ****************************************************************************/
WUFE_status WUFEI_UfeCoreGlobalIntDisable(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_mask_int_reg_ptr;
   WP_U16 value = 0;
   ((void)coreid);   
   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_mask_int_reg_ptr = &gen_regs_base->global_mask_int.value;

   value |= (WUFEI_UFE_GLOBAL_MASK_INT_TDM0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_TDM1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_MACHINE0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_MACHINE1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_FM0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_FM1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_EMPHY0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_EMPHY1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_CR_M);

   WUFEI_REG_SET(global_mask_int_reg_ptr, value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreGlobalIntGet
 *
 * Purpose:   Getter for the event register
 *
 * Description:
 *     This function gets the event register of the globals
 *
 * Inputs:
 *     ufeid - the ufe id.
 *
 * Return Value:
 *     the register
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U16 WUFEI_UfeCoreGlobalIntGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_int_reg_ptr;
   WP_U16 value;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_reg_ptr = &gen_regs_base->global_int.value;

   WUFEI_REG_GET(global_int_reg_ptr, &value);

   return value;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreGlobalIntMaskGet
 *
 * Purpose:   Getter for the event mask register
 *
 * Description:
 *     This function gets the event mask register of the globals
 *
 * Inputs:
 *     ufeid - the ufe id.
 *
 * Return Value:
 *     the register
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U16 WUFEI_UfeCoreGlobalIntMaskGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_mask_int_reg_ptr;
   WP_U16 value;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_mask_int_reg_ptr = &gen_regs_base->global_mask_int.value;

   WUFEI_REG_GET(global_mask_int_reg_ptr, &value);

   return value;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreGlobalIntSet
 *
 * Purpose:   Setter for the event register
 *
 * Description:
 *     This function sets a value in the event register of the globals
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     value - the value to set.
 *
 * Return Value:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_UfeCoreGlobalIntSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_int_reg_ptr;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_reg_ptr = &gen_regs_base->global_int.value;

   /* clear interrupt register */
   WUFEI_REG_SET(global_int_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreGlobalIntMaskSet
 *
 * Purpose:   Setter for the event mask register
 *
 * Description:
 *     This function sets a value in the event mask register of the globals
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     value - the value to set.
 *
 * Return Value:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_UfeCoreGlobalIntMaskSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_mask_int_reg_ptr;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_mask_int_reg_ptr = &gen_regs_base->global_mask_int.value;

   /* clear interrupt register */
   WUFEI_REG_SET(global_mask_int_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreDdrStateGet
 *
 * Purpose:   Getter for the Ddr state of a specific core
 *
 * Description:
 *     This function returns the status of a specific ddr.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     coreid - the id of the core.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     WUFEI_SystemCoreEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_UfeCoreDdrStateGet(WP_U32 ufeid, WP_U32 coreid)
{
   WP_U16 value, timeout =0;
   WP_U8 ddr_done;
   
   value = WUFEI_UfeCoreConfigGet(ufeid);

   if (coreid == WUFE_CORE_ID_0)
      ddr_done = WUFEI_UFE_CORE_DDR_0_INIT_DONE_V(value);
   else
      ddr_done = WUFEI_UFE_CORE_DDR_1_INIT_DONE_V(value);

   while (!ddr_done)
   {
      if (timeout == WUFEI_DDR_INIT_TIMEOUT)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "WUFE_UfeCoreDdrStateGet Ddr core_id %d has timed out\n",
                         coreid));
         return WUFE_ERR_DDR_NOT_READY;
      }
      value = WUFEI_UfeCoreConfigGet(ufeid);
      
      WPL_Delay(10);
      if (coreid == WUFE_CORE_ID_0)
         ddr_done = WUFEI_UFE_CORE_DDR_0_INIT_DONE_V(value);
      else
         ddr_done = WUFEI_UFE_CORE_DDR_1_INIT_DONE_V(value);
      timeout++;
   }
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwFmEventMaskRegSet
 *
 * Purpose:   Setter for the Max Buffer Size
 *
 * Description:
 *     This function sets the Max buffer size in the global registers
 *
 * Inputs:
 *     ufeid      the ufe id.
 *     max_buf    the value to set in the register
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     WUFEI_UfeCoreHwConfigSetup.
 *
 ****************************************************************************/
WUFE_status WUFEI_UfeCoreMaxBufSizeSet(WP_U32 ufeid, WUFE_max_buff_size max_buf)
{
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *core_cfg_reg_ptr;
   WP_U16 reg_value;
   
   if (max_buf != WUFE_MAX_BUF_SIZE_64)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFEI_UfeCoreMaxBufSizeSet Max Buffer Size not 64 is not supported\n"));
      return WUFE_ERR_MAX_BUFF_SIZE_NOT_IMPLEMENTED;
   }
   
   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   core_cfg_reg_ptr = &gen_regs_base->core_config.value;

   WUFEI_REG_GET(core_cfg_reg_ptr, &reg_value);
   reg_value |= WUFEI_UFE_MAX_BUF_SIZE_F(max_buf);
   WUFEI_REG_SET(core_cfg_reg_ptr, reg_value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreHwConfigSetup
 *
 * Purpose:   Sets up the global registers
 *
 * Description:
 *     This function sets the registry with the global register values
 *
 * Inputs:
 *     hw_config  pointer to registry structure to fill
 *     ufeid      the ufe id.
 *     max_buf    the maximum buffer size to set in the globals
 *
 * Return Value:
 *     NONE
 *
 * Called by:
 *     WUFE_SystemConfig.
 *
 ****************************************************************************/
WUFE_status WUFEI_UfeCoreHwConfigSetup(WUFEI_system_hw_config *hw_config, WP_U32 ufeid, WUFE_max_buff_size max_buf)
{
   WP_U16 value, coreid;
   WUFE_status status;
   WUFE_device_type device_type;
   WP_U16 state_machine, bandwidth;

   memset(hw_config, 0, sizeof(WUFEI_system_hw_config));

   /* read release id */
   hw_config->release_id = WUFEI_UfeCoreReleaseIdGet(ufeid);
   hw_config->secondary_release_id = WUFEI_UfeCoreSecondaryReleaseIdGet(ufeid);

   /* read general configuration register*/
   value = WUFEI_UfeCoreSysConfigGet(ufeid);

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_DeviceTypeGet(ufeid,&device_type));
      bandwidth = WUFEI_UFE_BANDWIDTH_V(value);

      if ((WUFE_DEVICE_TYPE_UFE448_T24 == device_type) ||
          (WUFE_DEVICE_TYPE_UFE448_M24 == device_type))
      {
         bandwidth = WUFEI_GLOBAL_CFG_BANDWIDTH_OC24;
      }
      else if ((WUFE_DEVICE_TYPE_UFE448_T48 == device_type) ||
               (WUFE_DEVICE_TYPE_UFE448_M48 == device_type))
      {
         if ((WUFEI_GLOBAL_CFG_BANDWIDTH_OC24 == bandwidth) ||
             (WUFEI_GLOBAL_CFG_BANDWIDTH_OC12 == bandwidth) ||
             (WUFEI_GLOBAL_CFG_BANDWIDTH_OC3 == bandwidth))
            return WUFE_ERR_BANDWIDTH_NOT_SUPPORTED;
         else
            bandwidth = WUFEI_GLOBAL_CFG_BANDWIDTH_OC48;
         
      }
      else if ((WUFE_DEVICE_TYPE_UFE412_T1 == device_type) ||
               (WUFE_DEVICE_TYPE_UFE412_M1 == device_type))
      {
         bandwidth = WUFEI_GLOBAL_CFG_BANDWIDTH_OC3;
      }
      else if ((WUFE_DEVICE_TYPE_UFE412_T2 == device_type) ||
               (WUFE_DEVICE_TYPE_UFE412_M2 == device_type))
      {
         if (WUFEI_GLOBAL_CFG_BANDWIDTH_OC3 == bandwidth)
            return WUFE_ERR_BANDWIDTH_NOT_SUPPORTED;
         else
            bandwidth = WUFEI_GLOBAL_CFG_BANDWIDTH_OC6;
      }
      else if ((WUFE_DEVICE_TYPE_UFE412_T4 == device_type) ||
               (WUFE_DEVICE_TYPE_UFE412_M4 == device_type))
      {
         if ((WUFEI_GLOBAL_CFG_BANDWIDTH_OC3 == bandwidth) || 
             (WUFEI_GLOBAL_CFG_BANDWIDTH_OC6 == bandwidth))
            return WUFE_ERR_BANDWIDTH_NOT_SUPPORTED;
         else
            bandwidth = WUFEI_GLOBAL_CFG_BANDWIDTH_OC12;         
      }
      hw_config->bandwidth = bandwidth;
   }
   else
      hw_config->bandwidth = WUFEI_UFE_BANDWIDTH_V(value);
   hw_config->framer_type = WUFEI_UFE_FRAMER_MODE_V(value);
   hw_config->emphy_setup_mode = WUFEI_UFE_EMPHY_SETUP_MODE_V(value);
   if (WUFEI_UFE_FM_NUM_OF_BANKS_V(value))
      hw_config->num_of_banks = WUFEI_FM_NUM_OF_BANKS_HIGH;
   else
      hw_config->num_of_banks = WUFEI_FM_NUM_OF_BANKS_LOW;

   
   
   /* read core configuration register*/
   value = WUFEI_UfeCoreConfigGet(ufeid);
   for(coreid=WUFE_CORE_ID_0; coreid<WUFE_MAX_N_CORE; coreid++)
   {  
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {
         WUFEI_RETURN_IF_ERROR(status, WUFEI_DeviceTypeGet(ufeid,&device_type)); 
         state_machine = WUFEI_GEN_CFG_SYS_REG_MACHINE_V(value);

         /*Same state machine for both cores*/
         if ((WUFE_DEVICE_TYPE_UFE412_T1 == device_type)  ||
             (WUFE_DEVICE_TYPE_UFE412_T2 == device_type)  || 
             (WUFE_DEVICE_TYPE_UFE412_T4 == device_type)  ||
             (WUFE_DEVICE_TYPE_UFE448_T24 == device_type) || 
             (WUFE_DEVICE_TYPE_UFE448_T48 == device_type))
         {
            /*HDLC machine is always present for DCC purposes */
            state_machine &= ~WUFEI_GEN_CFG_SYS_REG_MACHINE_ATM;
         }
         hw_config->state_machine[coreid] = state_machine;
      }
      else
         hw_config->state_machine[coreid] = WUFEI_GEN_CFG_SYS_REG_MACHINE_V(value);
   }
   /*Core 0 is always enabled*/
   hw_config->core_enabled[WUFE_CORE_ID_0] = WUFE_ENABLE;
   hw_config->core_enabled[WUFE_CORE_ID_1] = WUFEI_UFE_CORE_1_INCLUDED_V(value);

   WUFEI_RETURN_IF_ERROR(status, 
                         WUFEI_UfeCoreMaxBufSizeSet(ufeid, max_buf));
   hw_config->max_buf_size = WUFEI_UFE_MAX_BUF_SIZE_V(value);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreHwConfigDisplay
 *
 * Purpose: Displays the Global Registers information
 *
 * Inputs:
 *     hw_config           Pointer to the registry containing the info
 *     ufeid               The ufe id
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *     WUFE_SystemDisplay
 *
 ****************************************************************************/
void WUFEI_UfeCoreHwConfigDisplay(WUFEI_system_hw_config *hw_config, WP_U32 ufeid)
{
   WUFE_PRINT_DISP((wufei_log,"hw release_id                      0x%04x\n",
                    hw_config->release_id));
   WUFE_PRINT_DISP((wufei_log,"hw secondary_release_id            0x%04x\n",
                    hw_config->secondary_release_id));

   if (hw_config->bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC3)
      WUFE_PRINT_DISP((wufei_log,"bandwidth is OC3                0x%04x\n",
                       hw_config->bandwidth));
   else if (hw_config->bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC6)
      WUFE_PRINT_DISP((wufei_log,"bandwidth is OC6               0x%04x\n",
                       hw_config->bandwidth));
   else if (hw_config->bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC12)
      WUFE_PRINT_DISP((wufei_log,"bandwidth is OC12               0x%04x\n",
                       hw_config->bandwidth));
   else if (hw_config->bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC24)
      WUFE_PRINT_DISP((wufei_log,"bandwidth is 2xOC12             0x%04x\n",
                       hw_config->bandwidth));
   else if (hw_config->bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
      WUFE_PRINT_DISP((wufei_log,"bandwidth is 4xOC12             0x%04x\n",
                       hw_config->bandwidth));
   
   if (hw_config->emphy_setup_mode == WUFEI_EMPHY_SETUP_MODE_DUAL)
      WUFE_PRINT_DISP((wufei_log,"emphy_setup_mode is Dual        0x%04x\n",
                       hw_config->emphy_setup_mode));
   else if (hw_config->emphy_setup_mode == WUFEI_EMPHY_SETUP_MODE_MUX)
   {
      WUFEI_hw_emphy *emphy_regs_base = (WUFEI_hw_emphy *)WUFEI_UfeEmphyRegBaseAddrGet(ufeid);
      WP_U16* core1_offset_ptr = &emphy_regs_base->core1_port_id_offset.value;
      WP_U16 value;
      
      WUFE_PRINT_DISP((wufei_log,"emphy_setup_mode is Mux         0x%04x\n",
                       hw_config->emphy_setup_mode));
   
      WUFEI_REG_GET(core1_offset_ptr,&value);
      WUFE_PRINT_DISP((wufei_log,"emphy_port id offset for core 1:         0x%04x\n",
                       WUFEI_EMPHY_PORT_ID_OFFSET_V(value)));
   }
   
   else if (hw_config->emphy_setup_mode == WUFEI_EMPHY_SETUP_MODE_SINGLE)
      WUFE_PRINT_DISP((wufei_log,"emphy_setup_mode is Single      0x%04x\n",
                       hw_config->emphy_setup_mode));

   WUFE_PRINT_DISP((wufei_log,"The number of banks in the FM is %d\n",
                    hw_config->num_of_banks));

   if (hw_config->state_machine[WUFE_CORE_ID_0] & WUFEI_GEN_CFG_SYS_REG_MACHINE_TRANS)
      WUFE_PRINT_DISP((wufei_log,"Transparent Machine is included\n"));
   if (hw_config->state_machine[WUFE_CORE_ID_0] & WUFEI_GEN_CFG_SYS_REG_MACHINE_HDLC)
      WUFE_PRINT_DISP((wufei_log,"HDLC  Machine is included\n"));
   if (hw_config->state_machine[WUFE_CORE_ID_0] & WUFEI_GEN_CFG_SYS_REG_MACHINE_ATM)
      WUFE_PRINT_DISP((wufei_log,"ATM Machine is included\n"));
   
   WUFE_PRINT_DISP((wufei_log,"Core 0 is enabled\n"));
   if (hw_config->core_enabled[WUFE_CORE_ID_1])
      WUFE_PRINT_DISP((wufei_log,"Core 1 is enabled\n"));
   else
      WUFE_PRINT_DISP((wufei_log,"Core 1 is disabled\n"));

   if (hw_config->framer_type == WUFE_FRAMER_TYPE_EXTERNAL)
      WUFE_PRINT_DISP((wufei_log,"UFE is using external framer\n"));
   else if (hw_config->framer_type == WUFE_FRAMER_TYPE_INTERNAL)
      WUFE_PRINT_DISP((wufei_log,"UFE is using internal framer\n"));
   else 
      WUFE_PRINT_DISP((wufei_log,"UFE framer is unused\n"));
}

WUFE_status WUFEI_UfeCorePhyIdBlockCreate(WUFEI_system_registry* system_registry)
{
   WP_U16* phy_id_pool;
   WP_U32 coreid;
   WP_U32 max_n_phys;

   if(!system_registry->phy_id_block)
   {
      system_registry->phy_id_block =
         (WUFEI_phy_id_block *)WUFEI_MALLOC(sizeof(WUFEI_phy_id_block));
      if (!system_registry->phy_id_block)
         return WUFE_ERR_MALLOC;
      memset(system_registry->phy_id_block, 0, sizeof(WUFEI_phy_id_block));
   }

   system_registry->phy_id_block->max_n_phys = system_registry->max_phy_per_core;
   max_n_phys = system_registry->phy_id_block->max_n_phys;

   for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      phy_id_pool = (WP_U16 *)WUFEI_MALLOC(sizeof(WP_U16)* max_n_phys);
      if(!phy_id_pool)
         return WUFE_ERR_MALLOC;

      memset(phy_id_pool, 0xff, sizeof(WP_U16)* max_n_phys);

      system_registry->phy_id_block->phy_id_pool[coreid] = phy_id_pool;
   }

   return WUFE_OK;
}

WUFE_status WUFEI_UfeCorePhyIdBlockDelete(WUFEI_system_registry* system_registry)
{
   if (system_registry->phy_id_block)
   {
      WP_U32 coreid;

      for(coreid = WUFE_CORE_ID_0; coreid < WUFE_MAX_N_CORE; coreid++)
      {
         if(system_registry->phy_id_block->phy_id_pool[coreid])
         {
            WUFEI_FREE(system_registry->phy_id_block->phy_id_pool[coreid]);
            system_registry->phy_id_block->phy_id_pool[coreid] = 0;
         }
      }
      system_registry->phy_id_block->max_n_phys = 0;

      WUFEI_FREE(system_registry->phy_id_block);
      system_registry->phy_id_block = 0;
   }

   return WUFE_OK;

}

WUFE_status WUFEI_UfeCorePhyIdBlockSet(WUFEI_system_registry* system_registry,
                                       WP_U32 coreid, WP_U16 phy_id, WP_U16* emphy_port_id)
{
   WP_U32 entry;
   WP_U16* phy_id_pool;

   if(!system_registry->phy_id_block)
       return WUFE_ERR_INVALID_CONFIG;

   if(!system_registry->phy_id_block->phy_id_pool[coreid])
       return WUFE_ERR_INVALID_CONFIG;

   phy_id_pool = system_registry->phy_id_block->phy_id_pool[coreid];
   for(entry = 0; entry<system_registry->phy_id_block->max_n_phys; entry++)
   {
      if(phy_id_pool[entry] == WUFEI_PHY_NOT_CONNECTED)
      {
         phy_id_pool[entry] = phy_id;
         *emphy_port_id = entry;
         return WUFE_OK;
      }
   }

   return WUFE_ERR_NO_FREE_PHY;
}

WUFE_status WUFEI_UfeCorePhyIdBlockGet(WUFEI_system_registry* system_registry,
                                       WP_U32 coreid, WP_U16* phy_id, WP_U16 emphy_port_id)
{
  WP_U16* phy_id_pool;

  if(!system_registry->phy_id_block)
     return WUFE_ERR_INVALID_CONFIG;

  if(!system_registry->phy_id_block->phy_id_pool[coreid])
     return WUFE_ERR_INVALID_CONFIG;

  if(emphy_port_id >= system_registry->phy_id_block->max_n_phys)
     return WUFE_ERR_PHY_ASSIGNED_ERR;

  phy_id_pool = system_registry->phy_id_block->phy_id_pool[coreid];
  *phy_id = phy_id_pool[emphy_port_id];

  return WUFE_OK;
}

WUFE_status WUFEI_UfeCorePhyIdBlockPut(WUFEI_system_registry* system_registry,
                                       WP_U32 coreid, WP_U32 emphy_port_id)
{
   WP_U16* phy_id_pool;

   if(!system_registry->phy_id_block)
       return WUFE_ERR_INVALID_CONFIG;

  if(!system_registry->phy_id_block->phy_id_pool[coreid])
       return WUFE_ERR_INVALID_CONFIG;

  if(emphy_port_id >= system_registry->phy_id_block->max_n_phys)
     return WUFE_ERR_PHY_ASSIGNED_ERR;

  phy_id_pool = system_registry->phy_id_block->phy_id_pool[coreid];
  phy_id_pool[emphy_port_id] = WUFEI_PHY_NOT_CONNECTED;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreMaxPhyGet
 *
 * Purpose:   Getter for the maximum phys per core
 *
 * Inputs:
 *     system_registry -   The registry of the system
 *
 * Return Value:
 *     The max phys per core
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_UfeCoreMaxPhyGet(WUFEI_system_registry* system_registry)
{
   return system_registry->max_phy_per_core;
}

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreRegistrySetup
 *
 * Purpose:   Sets the port register offset and max phy num per core
 *
 * Inputs:
 *     system_registry -   The registry of the system
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WUFE_status WUFEI_UfeCoreRegistrySetup(WUFEI_system_registry* system_registry)
{
   if(system_registry->hw_config.emphy_setup_mode == WUFEI_EMPHY_SETUP_MODE_MUX)
   {
      if (system_registry->hw_config.bandwidth == WUFEI_GLOBAL_CFG_BANDWIDTH_OC48)
         system_registry->core1_phyid_offset = WUFEI_EMPHY_PORT_ID_OFFSET;
      else
         return WUFE_ERR_EMPHY_CORE1_OFFSET_CONFIG;
   }

   system_registry->max_phy_per_core = WUFEI_MAX_N_PHY_PER_CORE;

   return WUFE_OK;
}
