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

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_HW_CLK_REC_INT_H
#include "wufei_hw_clk_rec_int.h"
#endif

WP_CHAR wufei_log[400];

extern WP_U8 debug_flag;

static WP_U32 WUFEI_UfeGenRegBaseAddrGet(WP_U32 ufeid);
static WP_U16 WUFEI_UfeCoreReleaseIdGet(WP_U32 ufeid);
static WP_U16 WUFEI_UfeCoreSecondaryReleaseIdGet(WP_U32 ufeid);
static WP_U16 WUFEI_UfeCoreConfigGet(WP_U32 ufeid, WP_U32 coreid);
WP_U16 WUFEI_UfeCoreSysConfigGet(WP_U32 ufeid);
static WP_U16 WUFEI_UfeLiuConfigGet(WP_U32 ufeid);

/*****************************************************************************
 *
 * Function:  WUFEI_UfeCoreHwModelGet
 *
 * Purpose:
 *
 * Description:
 *     This function initializes the ufe_core address arrays
 *
 * Inputs:
 *     ufeid
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *            WUFE_ERR_INVALID_CONFIG
 *
 * Called by:
 *
 *   WUFEI_BsFpgaInit
 ****************************************************************************/
WP_U32 WUFEI_UfeCoreHwModelGet(WP_U32 ufeid)
{
   ((void)ufeid);   
   return 0;/*WUFE_INTERFACE_CAD_HMVIP*/;
}

WP_U32 WUFEI_UfeCoreBaseAddrGet(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   if(coreid == WUFE_CORE_ID_0)
      return (WP_U32)base_addr->core0;
   else if(coreid == WUFE_CORE_ID_1)
      return (WP_U32)base_addr->core1;
   else
      return 0;
}

WP_U32 WUFEI_UfeLiuBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)base_addr->liu;
}

WP_U32 WUFEI_UfeBsBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)base_addr->bsp;
}

WP_U32 WUFEI_UfeClkRecBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)&(base_addr->clk_rec);
}

static WP_U32 WUFEI_UfeGenRegBaseAddrGet(WP_U32 ufeid)
{
   WUFEI_hw_ufe* base_addr = (WUFEI_hw_ufe*)WUFEI_MappingUfeBaseGet(ufeid);

   return (WP_U32)&(base_addr->gen_regs);
}

static WP_U16  WUFEI_UfeCoreReleaseIdGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs_release* release_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* release_id_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);

   release_ptr = &gen_regs_base->release;

   release_id_ptr = &((release_ptr->release_id).value);

   debug_flag = 0 ;
   WUFEI_REG_GET(release_id_ptr, &val);
   if(debug_flag)
      printf("------------ addr 0x%08x,release_id 0x%04x\n",(WP_U32)release_id_ptr,val);

   return val;
}

static WP_U16  WUFEI_UfeCoreSecondaryReleaseIdGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs_release* release_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* release_id_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);

   release_ptr = &gen_regs_base->release;

   release_id_ptr = &((release_ptr->secondary_release_id).value);

   debug_flag = 0 ;
   WUFEI_REG_GET(release_id_ptr, &val);
   if(debug_flag)
      printf("------------ addr 0x%08x,secondary_release_id 0x%04x\n",(WP_U32)release_id_ptr,val);

   return val;
}


WP_U16  WUFEI_UfeCoreBuildNumGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs_release* release_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* build_num_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);

   release_ptr = &gen_regs_base->release;

   build_num_ptr = &((release_ptr->build_number).value);

   debug_flag = 0 ;
   WUFEI_REG_GET(build_num_ptr, &val);
   if(debug_flag)
      printf("------------ addr 0x%08x,build_number 0x%04x\n",(WP_U32)build_num_ptr,val);

   return val;
}


WP_U16 WUFEI_UfeCoreSysConfigGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs_sys_config* sys_config_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* sys_config_reg_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   sys_config_ptr = &gen_regs_base->sys_config;
   sys_config_reg_ptr = &((sys_config_ptr->sys_config).value);

   WUFEI_REG_GET(sys_config_reg_ptr, &val);
   if(debug_flag)
      printf("------------ addr 0x%08x, sys_config 0x%04x\n",(WP_U32)sys_config_reg_ptr, val);

   return val;
}


static WP_U16 WUFEI_UfeLiuConfigGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs_liu_config* liu_config_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* liu_config_reg_ptr;
   WP_U16 val;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   liu_config_ptr = &gen_regs_base->liu;
   liu_config_reg_ptr = &((liu_config_ptr->liu_config).value);

   WUFEI_REG_GET(liu_config_reg_ptr, &val);
   if(debug_flag)
      printf("------------ addr 0x%08x, liu_config 0x%04x\n",(WP_U32)liu_config_reg_ptr, val);

   return val;
}


static WP_U16 WUFEI_UfeCoreConfigGet(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_gen_regs_core_config* core_config_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16* core_config_reg_ptr;
   WP_U16 val;

   debug_flag = 0;
   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   if(coreid == WUFE_CORE_ID_0)
      core_config_ptr = &gen_regs_base->core0;
   else if(coreid == WUFE_CORE_ID_1)
      core_config_ptr = &gen_regs_base->core1;
   else
      return 0;

   core_config_reg_ptr = &((core_config_ptr->core_config).value);
   WUFEI_REG_GET(core_config_reg_ptr, &val);
   debug_flag = 0;

   return val;
}

WUFE_status WUFEI_UfeCoreGlobalIntEnable(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_gen_regs_global_int *global_int_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_mask_int_reg_ptr;
   WP_U16 value = 0;
   ((void)coreid);   
   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_ptr = &gen_regs_base->interrupt;

   global_mask_int_reg_ptr = &((global_int_ptr->global_mask_int).value);

   value &= ~(WUFEI_UFE_GLOBAL_MASK_INT_CORE0_M |
              WUFEI_UFE_GLOBAL_MASK_INT_CORE1_M |
              WUFEI_UFE_GLOBAL_MASK_INT_EMPHY_CORE0_M |
              WUFEI_UFE_GLOBAL_MASK_INT_EMPHY_CORE1_M |
              WUFEI_UFE_GLOBAL_MASK_INT_LIU_M |
              WUFEI_UFE_GLOBAL_MASK_INT_CR_M);
   WUFEI_REG_SET(global_mask_int_reg_ptr, value);

   return WUFE_OK;

}

WUFE_status WUFEI_UfeCoreGlobalIntDisable(WP_U32 ufeid, WP_U32 coreid)
{
   WUFEI_hw_gen_regs_global_int *global_int_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_mask_int_reg_ptr;
   WP_U16 value = 0;
   ((void)coreid);   
   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_ptr = &gen_regs_base->interrupt;

   global_mask_int_reg_ptr = &((global_int_ptr->global_mask_int).value);

   value |= (WUFEI_UFE_GLOBAL_MASK_INT_CORE0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_CORE1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_EMPHY_CORE0_M |
             WUFEI_UFE_GLOBAL_MASK_INT_EMPHY_CORE1_M |
             WUFEI_UFE_GLOBAL_MASK_INT_LIU_M |
             WUFEI_UFE_GLOBAL_MASK_INT_CR_M);

   WUFEI_REG_SET(global_mask_int_reg_ptr, value);

   return WUFE_OK;

}

WP_U16 WUFEI_UfeCoreGlobalIntGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs_global_int *global_int_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_int_reg_ptr;
   WP_U16 value;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_ptr = &gen_regs_base->interrupt;

   global_int_reg_ptr = &((global_int_ptr->global_int).value);

   /* read interrupt register */
   WUFEI_REG_GET(global_int_reg_ptr, &value);

   return value;
}

WP_U16 WUFEI_UfeCoreGlobalIntMaskGet(WP_U32 ufeid)
{
   WUFEI_hw_gen_regs_global_int *global_int_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_int_reg_ptr;
   WP_U16 value;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_ptr = &gen_regs_base->interrupt;

   global_int_reg_ptr = &((global_int_ptr->global_mask_int).value);

   /* read interrupt register */
   WUFEI_REG_GET(global_int_reg_ptr, &value);

   return value;
}

void WUFEI_UfeCoreGlobalIntSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_gen_regs_global_int *global_int_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_int_reg_ptr;

   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_ptr = &gen_regs_base->interrupt;

   global_int_reg_ptr = &((global_int_ptr->global_int).value);

   /* clear interrupt register */
   WUFEI_REG_SET(global_int_reg_ptr, value);
}

void WUFEI_UfeCoreGlobalIntMaskSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_gen_regs_global_int *global_int_ptr;
   WUFEI_hw_gen_regs *gen_regs_base;
   WP_U16 *global_int_reg_ptr;
   ((void)value);   
   gen_regs_base = (WUFEI_hw_gen_regs*)WUFEI_UfeGenRegBaseAddrGet(ufeid);
   global_int_ptr = &gen_regs_base->interrupt;

   global_int_reg_ptr = &((global_int_ptr->global_mask_int).value);

   /* clear interrupt register */
   WUFEI_REG_SET(global_int_reg_ptr, 0);
}


WP_U32 WUFEI_UfeCoreLiuModeGet(WP_U32 ufeid)
{
   WP_U16 value;

   value = WUFEI_UfeCoreSysConfigGet(ufeid);

   return  WUFEI_UFE_LIU_MODE_V(value);

}

WP_U32 WUFEI_UfeCoreBandwidthGet(WP_U32 ufeid)
{
   WP_U16 value;

   value = WUFEI_UfeCoreSysConfigGet(ufeid);

   return  WUFEI_UFE_BANDWIDTH_V(value);

}


WUFE_status WUFEI_UfeCoreHwConfigSetup(WUFEI_system_hw_config *hw_config, WP_U32 ufeid, WP_U16 dummy)
{
   WP_U16 value, coreid;
   ((void)dummy);
   
   memset(hw_config, 0, sizeof(WUFEI_system_hw_config));

   /* read release id */
   hw_config->release_id = WUFEI_UfeCoreReleaseIdGet(ufeid);
   hw_config->secondary_release_id = WUFEI_UfeCoreSecondaryReleaseIdGet(ufeid);

   hw_config->full_ufe_model = 0; /* irrelevant for ufe3 always full model*/

   /* read general configuration register - LIU mode/Emphy mode/CR enabled*/
   value = WUFEI_UfeCoreSysConfigGet(ufeid);

   hw_config->bandwidth = WUFEI_UFE_BANDWIDTH_V(value);
   hw_config->line_interface = WUFEI_UFE_LIU_MODE_V(value);
   hw_config->emphy_setup_mode = WUFEI_UFE_EMPHY_SETUP_MODE_V(value);

   if((WUFEI_UFE_CLK_REC_RX_ENABLE_V(value)) || (WUFEI_UFE_CLK_REC_TX_ENABLE_V(value)))
   {
      WP_U16 RxEnable=0, TxEnable=0;
      WUFE_status status;
      if (WUFEI_UFE_CLK_REC_RX_ENABLE_V(value))
         RxEnable = 1;
      if (WUFEI_UFE_CLK_REC_TX_ENABLE_V(value))
         TxEnable = 1;

      WUFEI_RETURN_IF_ERROR(status,
                            WUFEI_HwClockRecHwConfigSetup(ufeid, &hw_config->clk_rec_hw_config,
                                                          RxEnable, TxEnable));
   }

   for(coreid=WUFE_CORE_ID_0; coreid<WUFE_MAX_N_CORE; coreid++ )
   {
      value = WUFEI_UfeCoreConfigGet(ufeid, coreid);
      hw_config->state_machine[coreid] = WUFEI_GEN_CFG_SYS_REG_MACHINE_V(value);
      hw_config->core_enabled[coreid] = WUFEI_UFE_CORE_ENABLED_V(value);

      /*ZBT internal memory configuration check*/
      if(WUFEI_UFE_CORE_ZBT_SIZE_V(value) == 0)
         return WUFE_ERR_HW_CONFIG_FIFO_MEMORY_NOT_MATCH;

      /*ZBT size calculate*/
      if(WUFEI_UFE_CORE_ZBT_SIZE_V(value) == 1)
         hw_config->zbt_size[coreid] = WUFEI_HW_MINIMUM_FIFO_MEM_SIZE;
      else hw_config->zbt_size[coreid] =
              WUFEI_HW_MINIMUM_FIFO_MEM_SIZE*(1 << WUFEI_UFE_CORE_ZBT_SIZE_V(value));

      hw_config->zbt_internal[coreid] = WUFEI_UFE_CORE_INT_ZBT_V(value);
      if (hw_config->zbt_internal[coreid] &&  (hw_config->zbt_size[coreid] != WUFEI_HW_MINIMUM_FIFO_MEM_SIZE))
         return WUFE_ERR_HW_CONFIG_INTERNAL_ZBT_MUST_BE_128K;
      hw_config->tdm_sys_loop[coreid] = WUFEI_UFE_TDM_SYS_LOOP_INC_V(value);
      hw_config->tdm_line_loop[coreid] = WUFEI_UFE_TDM_LINE_LOOP_INC_V(value);
   }

   if(hw_config->zbt_size[WUFE_CORE_ID_0] != hw_config->zbt_size[WUFE_CORE_ID_1])
      return WUFE_ERR_HW_CONFIG_FIFO_MEMORY_SIZE;

   return WUFE_OK;
}

WUFE_status WUFEI_UfeCoreHwConfigDelete(WUFEI_system_hw_config *hw_config)
{
   return (WUFEI_HwClockRecHwConfigDelete(hw_config->clk_rec_hw_config));
}

void WUFEI_UfeCoreHwConfigDisplay(WUFEI_system_hw_config *hw_config, WP_U32 ufeid)
{
   WP_U16 coreid;
   WP_U16 value;

   WUFE_PRINT_DISP((wufei_log,"release_id                      0x%04x\n",
                    hw_config->release_id));
   WUFE_PRINT_DISP((wufei_log,"secondary_release_id            0x%04x\n",
                    hw_config->secondary_release_id));
   WUFE_PRINT_DISP((wufei_log,"full_ufe_model                  0x%04x\n",
                    hw_config->full_ufe_model));
   WUFE_PRINT_DISP((wufei_log,"line_interface                  0x%04x\n",
                    hw_config->line_interface));
   WUFE_PRINT_DISP((wufei_log,"bandwidth                       0x%04x\n",
                    hw_config->bandwidth));
   WUFE_PRINT_DISP((wufei_log,"emphy_setup_mode                0x%04x\n",
                    hw_config->emphy_setup_mode));

   value = WUFEI_UfeLiuConfigGet(ufeid);
   if (hw_config->line_interface == WUFEI_GEN_CFG_SYS_REG_LIFM_SBI)
   {
      WUFE_PRINT_DISP((wufei_log,"SBI Mode                        0x%04x\n",
                       WUFEI_UFE_SBI_MODE_V(value)));
   }
   else
   {
      WUFE_PRINT_DISP((wufei_log,"LIU Mode Core1 0x%04x\n",
                       WUFEI_UFE_LIU_MODE_CORE1_V(value)));

      WUFE_PRINT_DISP((wufei_log,"Number of instantiated units Core1 0x%04x\n",
                       WUFEI_UFE_CAD_UNIT_NUM_CORE1_V(value)));

      WUFE_PRINT_DISP((wufei_log,"LIU Mode Core1 0x%04x\n",
                       WUFEI_UFE_LIU_MODE_CORE1_V(value)));

      WUFE_PRINT_DISP((wufei_log,"Number of instantiated units Core1 0x%04x\n",
                       WUFEI_UFE_CAD_UNIT_NUM_CORE1_V(value)));
   }

   for(coreid=WUFE_CORE_ID_0; coreid<WUFE_MAX_N_CORE; coreid++ )
   {
       WUFE_PRINT_DISP((wufei_log,"hw_config->state_machine[%d]     0x%04x\n",
                        coreid, hw_config->state_machine[coreid]));
       WUFE_PRINT_DISP((wufei_log,"hw_config->core_enabled[%d]      0x%04x\n",
                        coreid, hw_config->core_enabled[coreid]));
       WUFE_PRINT_DISP((wufei_log,"hw_config->zbt_size[%d]          0x%04x\n",
                        coreid, hw_config->zbt_size[coreid]));
       WUFE_PRINT_DISP((wufei_log,"hw_config->zbt_internal[%d]      0x%04x\n",
                        coreid, hw_config->zbt_internal[coreid]));
       WUFE_PRINT_DISP((wufei_log,"hw_config->tdm_sys_loop[%d]      0x%04x\n",
                        coreid, hw_config->tdm_sys_loop[coreid]));
       WUFE_PRINT_DISP((wufei_log,"hw_config->tdm_line_loop[%d]     0x%04x\n",
                        coreid, hw_config->tdm_line_loop[coreid]));
   }
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
   WP_U16* phy_id_pool = system_registry->phy_id_block->phy_id_pool[coreid];

   if(!system_registry->phy_id_block)
       return WUFE_ERR_INVALID_CONFIG;

   if(!system_registry->phy_id_block->phy_id_pool[coreid])
       return WUFE_ERR_INVALID_CONFIG;

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
  WP_U16* phy_id_pool = system_registry->phy_id_block->phy_id_pool[coreid];

  if(!system_registry->phy_id_block)
     return WUFE_ERR_INVALID_CONFIG;

  if(!system_registry->phy_id_block->phy_id_pool[coreid])
     return WUFE_ERR_INVALID_CONFIG;

  if(emphy_port_id >= system_registry->phy_id_block->max_n_phys)
     return WUFE_ERR_PHY_ASSIGNED_ERR;

  *phy_id = phy_id_pool[emphy_port_id];

  return WUFE_OK;
}

WUFE_status WUFEI_UfeCorePhyIdBlockPut(WUFEI_system_registry* system_registry,
                                       WP_U32 coreid, WP_U32 emphy_port_id)
{
   WP_U16* phy_id_pool = system_registry->phy_id_block->phy_id_pool[coreid];

   if(!system_registry->phy_id_block)
       return WUFE_ERR_INVALID_CONFIG;

  if(!system_registry->phy_id_block->phy_id_pool[coreid])
       return WUFE_ERR_INVALID_CONFIG;

  if(emphy_port_id >= system_registry->phy_id_block->max_n_phys)
     return WUFE_ERR_PHY_ASSIGNED_ERR;

   phy_id_pool[emphy_port_id] = WUFEI_PHY_NOT_CONNECTED;

   return WUFE_OK;
}

WP_U32 WUFEI_UfeCoreMaxPhyGet(WUFEI_system_registry* system_registry)
{
   return system_registry->max_phy_per_core;
}

WUFE_status WUFEI_UfeCoreRegistrySetup(WUFEI_system_registry* system_registry)
{

   WP_U32 rx_fbp_size, tx_fbp_size, max_n_phys_per_core;
   WP_U32 coreid;

   /*assumes the EMPHY is already initialized*/
   if(system_registry->hw_config.emphy_setup_mode == WUFEI_EMPHY_SETUP_MODE_MUX)
   {
      WP_U16 max_phy_num,value;

      max_phy_num = system_registry->cfg.max_phy_number;

      /*if the max phy number is lower than the highest configurable option,
        we set the offset to max_phy_num (rounded up to the power of 2)
        else, we set it to the highest configurable option*/
      if (max_phy_num < WUFEI_EMPHY_PORT_ID_OFFSET_MAX)
         /*if it is an exact power of two we don't need to round it up*/
         if ((max_phy_num & (max_phy_num - 1)) == 0)
            value= 1 << WPI_Log2(max_phy_num);
         else value= 1 << (WPI_Log2(max_phy_num) + 1);
      else value = WUFEI_EMPHY_PORT_ID_OFFSET_MAX;

     system_registry->core1_phyid_offset = value;

      if(system_registry->core1_phyid_offset == 0)
         return WUFE_ERR_EMPHY_CORE1_OFFSET_CONFIG;
   }

   if(system_registry->cfg.max_phy_number < WUFEI_MAX_N_PHY/2)
      max_n_phys_per_core = system_registry->cfg.max_phy_number;
   else
      max_n_phys_per_core = WUFEI_MAX_N_PHY/2;

   /*TSPT/RSPT & emphy_port_id*/
   system_registry->max_phy_per_core = max_n_phys_per_core;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if (system_registry->cfg.max_phy_number >= max_n_phys_per_core)
      {
         rx_fbp_size = system_registry->cfg.fifo_manager.rx_fbp_size;
         tx_fbp_size = system_registry->cfg.fifo_manager.tx_fbp_size;
         system_registry->rx_fbp_size[coreid] =
            rx_fbp_size*max_n_phys_per_core/system_registry->cfg.max_phy_number;
         system_registry->tx_fbp_size[coreid] =
            tx_fbp_size*max_n_phys_per_core/system_registry->cfg.max_phy_number;
      }
      else
      {
         system_registry->rx_fbp_size[coreid] = system_registry->cfg.fifo_manager.rx_fbp_size;
         system_registry->tx_fbp_size[coreid] = system_registry->cfg.fifo_manager.tx_fbp_size;
      }
   }

   return WUFE_OK;
}
