/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_registry.c
 *
 * Purpose: the UFE EMPHY macro and structures
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif
#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif
#ifndef WUFEI_INT_H
#include "wufei_int.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif
#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif
#ifndef WUFEI_BOARD_DATA_H
#include "wufei_board_data.h"
#endif
#ifndef WUFEI_BOARD_SUPPORT_INT_H
#include "wpi_board_support_int.h"
#endif
#ifndef WUFEI_BOARD_SUPPORT_H
#include "wufei_board_support.h"
#endif
#include "include/core/utility/wpi_services.h"

#define WUFE_UFE_LOG_SIZE 800
extern wufei_mem_space_block ufe_mem_space_block[];
extern WP_U32 wufei_ufe_current_status;

WP_CHAR wufei_log[WUFE_UFE_LOG_SIZE];

/* the definition of the UFE Adress space is located in file wufei_hw_config.h */

wpi_match_mask save_mask[WUFE_MAX_N_UFE]={{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

static WUFEI_registry ufe_registry[WUFE_MAX_N_UFE] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

#define WUFEI_ILLIGAL_VALUE 0xFFFFFFFF

WP_boolean WUFEI_CheckBoardSupport(WP_U32 ufeid)
{
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      return WP_TRUE;
   }
   else
   {
      return WP_FALSE;
   }
}


WUFE_status WUFEI_UfeCoreHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsUfeInterruptEnable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeFramerHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsFramerInterruptEnable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeExtPllHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid,&system_registry,status);
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsExtPllInterruptEnable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeLockLostHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if(!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsLockLostInterruptEnable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeSfpHwInterruptEnable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsSfpInterruptEnable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeCoreHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);

   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsUfeInterruptDisable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeFramerHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsFramerInterruptDisable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeExtPllHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsExtPllInterruptDisable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeLockLostHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsLockLostInterruptDisable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeSfpHwInterruptDisable(WP_U32 ufeid)
{
   WUFE_status status = WUFE_OK;
   WUFEI_system_registry* system_registry;

   WUFEI_REGISTY_GET_WRITE_ATTACH(ufeid, &system_registry,status);
   if(!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsSfpInterruptDisable(ufeid),ufeid);
   }
   WUFEI_REGISTY_GET_WRITE_DETACH(ufeid);
   return status;
}

WUFE_status WUFEI_UfeCoreHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WUFE_status status = WUFE_OK;
   ((void)interrupt_result);   
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_RETURN_IF_ERROR(status,WUFEI_BsUfeInterruptGet(ufeid, interrupt_result));
   }
   return status;
}

WUFE_status WUFEI_UfeFramerHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WUFE_status status = WUFE_OK;
   ((void)interrupt_result);   
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_BsFramerInterruptGet(ufeid, interrupt_result));
   }
   return status;
}

WUFE_status WUFEI_UfeExtPllHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WUFE_status status = WUFE_OK;
   ((void)interrupt_result);   
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_BsUfeExtPllInterruptGet(ufeid,interrupt_result));
   }
   return status;
}

WUFE_status WUFEI_UfeLockLostHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WUFE_status status = WUFE_OK;
   ((void)interrupt_result);   
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_BsUfeLockLostInterruptGet(ufeid,interrupt_result));
   }
   return status;
}

WUFE_status WUFEI_UfeSfpHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result)
{
   WUFE_status status = WUFE_OK;
   ((void)interrupt_result);   
   if (!(WUFEI_MappingAttribGet(ufeid) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS))
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_BsUfeSfpInterruptGet(ufeid,interrupt_result));
   }
   return status;
}

WUFE_status WUFEI_MappingAllocate(WP_U32 ufeid)
{
   if(!(ufe_registry[ufeid].mapping_block))
   {
      ufe_registry[ufeid].mapping_block =
         (WUFEI_mapping_block*)WUFEI_MALLOC(sizeof(WUFEI_mapping_block));

      if (!(ufe_registry[ufeid].mapping_block))
      {
         WUFE_PRINT_ERR((wufei_log,
                         "WUFEI_MappingAllocate:mapping_block malloc failed ufeid %d\n",
                         ufeid));
         return WUFE_ERR_MALLOC;
      }
      memset(ufe_registry[ufeid].mapping_block, 0, sizeof(WUFEI_mapping_block));
   }
   
   ufe_registry[ufeid].device_type = WUFE_DEVICE_TYPE_NONE;

   return WUFE_OK;
}

WUFE_status WUFEI_MappingInit(WP_U32 ufeid, WP_U32 attributes)
{
   WP_U32 offset, ufe_base;

   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_MappingInit: invalid ufeid %d\n", ufeid));
      return WUFE_ERR_INVALID_WUFE_ID;
   }
   if(!(ufe_registry[ufeid].mapping_block))
   {
     WUFE_PRINT_ERR((wufei_log,
                     "WUFEI_MappingUfeBaseSet: mapping_block not allocated, ufeid %d\n",
                     ufeid));
     return WUFE_ERR_MAPPING_NOT_INITIALIZED;
   }

   offset = WUFE_ATTRIB_CONNECT_OFFSET_V(attributes);
   /* If UFE is already initialized it means that we are in dual EMPHY mode
      and the base address of the second UFE is taken from offset 0 and not
      from offset ufeid */
   if (wufei_ufe_current_status)
      ufe_base = ufe_mem_space_block[offset].ufe_bases[0].ufe_fpga_base;
   else
   ufe_base = ufe_mem_space_block[offset].ufe_bases[ufeid].ufe_fpga_base;

   ufe_registry[ufeid].mapping_block->ufe_base = ufe_base;
   ufe_registry[ufeid].mapping_block->attributes = attributes;

   return WUFE_OK;
}

WP_U32 WUFEI_MappingUfeBaseGet(WP_U32 ufeid)
{
   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_MappingUfeBaseGet: invalid ufeid %d\n", ufeid));
      return WUFEI_ILLIGAL_VALUE;
   }
   if(!(ufe_registry[ufeid].mapping_block))
   {
     WUFE_PRINT_ERR((wufei_log,
                     "WUFEI_MappingUfeBaseGet: mapping_block not allocated, ufeid %d\n",
                     ufeid));
     return WUFEI_ILLIGAL_VALUE;
   }

   return ufe_registry[ufeid].mapping_block->ufe_base;
}

WP_U32 WUFEI_MappingAttribGet(WP_U32 ufeid)
{
   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_MappingAttribGet: invalid ufeid %d\n", ufeid));
      return WUFEI_ILLIGAL_VALUE;
   }
   if(!(ufe_registry[ufeid].mapping_block))
   {
     WUFE_PRINT_ERR((wufei_log,
                     "WUFEI_MappingAttribGet: mapping_block not allocated, ufeid %d\n",
                     ufeid));
     return WUFEI_ILLIGAL_VALUE;
   }

   return ufe_registry[ufeid].mapping_block->attributes;
}

WUFE_status WUFEI_MappingDelete(WP_U32 ufeid)
{
   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_UfeRegistryMappingGet:ufeid %d\n", ufeid));
      return WUFE_ERR_INVALID_WUFE_ID;
   }

   if(ufe_registry[ufeid].mapping_block)
   {
      WUFEI_FREE(ufe_registry[ufeid].mapping_block);
      ufe_registry[ufeid].mapping_block = NULL;
   }

   return WUFE_OK;
}

WUFE_status WUFEI_SystemeRegistrySet(WP_U32 wufe_id, WUFEI_system_registry* system_registry)
{
   if (wufe_id >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "WUFEI_UfeRegistrySystemSet:invalid wufe_id %d\n",wufe_id ));
      return WUFE_ERR_INVALID_WUFE_ID;
   }
   if (!ufe_registry[wufe_id].system_registry)
   {/*  if there is no registry saved and the init lock is set */
      ufe_registry[wufe_id].system_registry = system_registry;
      return WUFE_OK;
   }
   else if(ufe_registry[wufe_id].system_registry)
   {/*  if there is registry saved and the command is clear the value */
      ufe_registry[wufe_id].system_registry= 0;
      return WUFE_OK;
   }
   else
   {/* try to overwrite the UFE registery
       or try write without getting the init key*/
      WUFE_PRINT_ERR((wufei_log,
                      "WUFEI_UfeRegistrySystemSet:invalid commit"));
      return WUFE_ERR_INVALID_REGISTRY_COMMIT_ACTION;
   }
}

void WUFEI_ChipSelectWaitSet(WP_U32 wufe_id, WP_U8 connector_index, WP_U32 attributes)
{
   WP_U32 *if_addr,mask, *general_turn_off_register_addr, value=0;

   /* Save current match/mask, install UFE values. */
   save_mask[wufe_id].match_address = wpi_board_data.match_mask->ufe[connector_index].match_address;

   /* Get the virtual address of the match/mask register pair. */
   if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, wpi_board_data.match_mask->ufe[connector_index].match_address);

   /* Save the current match/mask values. */
   WPI_REG_GET(save_mask[wufe_id].match_value, if_addr[0]);
   WPI_REG_GET(save_mask[wufe_id].mask_value, if_addr[1]);

   /* Install the desired match/mask values. */
   if (wpi_board_data.match_mask->ufe[connector_index].mask_value != save_mask[wufe_id].mask_value ||
       wpi_board_data.match_mask->ufe[connector_index].match_value != save_mask[wufe_id].match_value)
   {
      WPI_REG_SET(if_addr[1], wpi_board_data.match_mask->ufe[connector_index].mask_value);
      WPI_REG_SET(if_addr[0], wpi_board_data.match_mask->ufe[connector_index].match_value);
   }

   if (~(attributes) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS)
   {
      if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
      {

    	  /* Configure the turn off time of  ufe412  chip select to 4 clock cycles*/
         mask=WUFEI_TURNOFF_MASK;
         mask<<=(WUFEI_UFE412_CHIP_SELECT*4);
         mask=~mask;
    	 general_turn_off_register_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, WUFEI_SIU4X_GEN_TOFF_ADDRESS);
         WPI_REG_GET(value, *general_turn_off_register_addr);
         value &= mask ;
         value |= (WUFEI_SIU4X_GEN_TOFF_VALUE<<(WUFEI_UFE412_CHIP_SELECT*4));
         WPI_REG_SET(*general_turn_off_register_addr, value);
      }
   }

   WPL_Delay(100);
}

void WUFEI_ChipSelectWaitRestore(WP_U32 wufe_id)
{
   /* reload the previous match/mask, before UFE values. */
   WP_U32 *if_addr;

   /* in WP3 - do nothing */
   if(save_mask[wufe_id].match_address == 0xffffffff)
      return;

   /* Get the virtual address of the match/mask register pair. */
   if_addr = (WP_U32 *) WPL_RIF_VIRTUAL(0, save_mask[wufe_id].match_address);

   /* Install the desired match/mask values. */
   WPI_REG_SET(if_addr[1], save_mask[wufe_id].mask_value);
   WPI_REG_SET(if_addr[0], save_mask[wufe_id].match_value);

   WPL_Delay(100);
}

void WUFEI_UfeRegistryLock(WP_U32 wufe_id)
{
   ((void)wufe_id);
   WPI_WDDI_ATTACH();
}

void WUFEI_UfeRegistryUnLock(WP_U32 wufe_id)
{
   ((void)wufe_id);
   WPI_WDDI_DETACH();
}


WUFEI_system_registry* WUFEI_SystemRegistryGet(WP_U32 wufe_id)
{

   if (wufe_id >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_UfeRegistrySystemInitGet:wufe_id %d\n", wufe_id));
      return NULL;
   }

   return  ufe_registry[wufe_id].system_registry;

}

WUFE_status WUFEI_SystemRegistryGetAndLock(WP_U32 wufe_id,
                                           WUFEI_system_registry** system_registry)
{
#if !WUFE_DEBUG_ENABLE_WUFE_CALL_API
   WUFEI_UFE_LOCK(wufe_id);
#endif
   *system_registry = WUFEI_SystemRegistryGet(wufe_id);
   if(!(*system_registry))
   {
      WUFEI_UFE_UNLOCK(wufe_id);
      return WUFE_ERR_INVALID_REGISTRY_STATE;
   }
   return WUFE_OK;

}

void WUFEI_RegDbgSet(WP_U32 command, WP_U32 ufe_id)
{
   if (command == WUFEI_DBG_LEVEL_0)
   {
      ufe_registry[ufe_id].dbg_mode=WUFEI_DBG_LEVEL_0;
   }
   else if(command == WUFEI_DBG_LEVEL_1)
   {
      ufe_registry[ufe_id].dbg_mode=WUFEI_DBG_LEVEL_1;
   }
   else
      ufe_registry[ufe_id].dbg_mode=WUFEI_DBG_LEVEL_MAX;
}

WP_U8 WUFEI_DbgGet(WP_U32 ufe_id)
{
   return ufe_registry[ufe_id].dbg_mode;
}

WUFE_status WUFEI_SystemRegistryUfeEmphyNumSet(WP_U32 wufe_id)
{
	WUFE_status status = WUFE_OK;
	WUFEI_system_registry* system_registry;
	WP_U32 port_index, attributes;
//	WP_upi_emphy_info portInfo;
//	wp_emphy_number emphy_creation_order = 0;



	attributes = WUFEI_MappingAttribGet(wufe_id);

	if(WUFEI_ILLIGAL_VALUE == attributes)
	{
		return WUFE_ERR_MAPPING_ATTRIB;
	}

	port_index = WUFE_ATTRIB_CONNECT_OFFSET_V(attributes);
	if(port_index > WUFE_ATTRIB_CONNECT_MAX_OFFSET)
		return WUFE_ERR_INVALID_CONFIG;
#if 0
	status = WP_PortInfoGet(WUFEI_DEFAULT_WINPATH_ID, port_index, WP_PORT_UPI_EMPHY_COMMAND, &portInfo);
#endif
	if(WP_OK != status)
	{
	   return WUFE_PORT_INFO_GET_ERR;
	}
#if 0
	emphy_creation_order = WP_EMPHY_FIRST/*portInfo.emphy_port_creation_order*/;
#endif
	system_registry = WUFEI_SystemRegistryGet(wufe_id);

	if(!(system_registry))
	{
		return WUFE_ERR_INVALID_REGISTRY_STATE;
	}
#if 0
	if(emphy_creation_order == WP_EMPHY_FIRST)
	{
		system_registry->emphy_number = WUFEI_EMPHY_FIRST;
	}
	else if(emphy_creation_order == WP_EMPHY_SECOND)
	{
		system_registry->emphy_number = WUFEI_EMPHY_SECOND;
	}
	else
	{
	   return WUFE_ERR_INVALID_CONFIG;
	}
#endif
        system_registry->emphy_number = WUFEI_EMPHY_FIRST;
	return status;
}

WUFE_status WUFEI_DeviceTypeSet(WP_U32 ufeid, WUFE_device_type device_type)
{
   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_DeviceTypeSet: invalid ufeid %d\n", ufeid));
      return WUFE_ERR_INVALID_WUFE_ID;
   }

   ufe_registry[ufeid].device_type  = device_type;
   
   return WUFE_OK;
}

WUFE_status WUFEI_DeviceTypeGet(WP_U32 ufeid, WUFE_device_type *device_type)
{

   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_DeviceTypeGet: invalid ufeid %d\n", ufeid));
      return WUFE_ERR_INVALID_WUFE_ID;
   }

   if (WUFE_DEVICE_TYPE_NONE  == ufe_registry[ufeid].device_type)
   {
      WUFE_PRINT_ERR((wufei_log,
                      "UFEID %d -- WUFEI_DeviceTypeGet: "
                      "must call the WUFE_DeviceTypeSet API before WUFE_SystemConfig API\n", ufeid));
      return WUFE_ERR_DEVICE_TYPE_NOT_SET;
   }

   *device_type = ufe_registry[ufeid].device_type;

   return WUFE_OK;
   
}


WUFE_status WUFEI_UfeSynBuildTimeGet(WP_U32 ufeid, WP_U32 *syn_build_time)
{
   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_DeviceTypeGet: invalid ufeid %d\n", ufeid));
      return WUFE_ERR_INVALID_WUFE_ID;
   }

   *syn_build_time = ufe_registry[ufeid].build_time;

   return WUFE_OK;   
}

WUFE_status WUFEI_UfeSynBuildTimeSet(WP_U32 ufeid, WP_U32 syn_build_time)
{
   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFEI_DeviceTypeGet: invalid ufeid %d\n", ufeid));
      return WUFE_ERR_INVALID_WUFE_ID;
   }   

   ufe_registry[ufeid].build_time = syn_build_time;

   return WUFE_OK;
}
