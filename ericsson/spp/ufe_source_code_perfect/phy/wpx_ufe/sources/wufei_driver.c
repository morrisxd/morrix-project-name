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
#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif
#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif
#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif
#ifndef WUFEI_BOARD_SUPPORT_INT_H
#include "wufei_board_support_int.h"
#endif
#ifndef WUFEI_BOARD_DATA_H
#include "wufei_board_data.h"
#endif
#ifndef WUFEI_HW_CORE_INT_H
#include "wufei_hw_core_int.h"
#endif
#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif
#ifndef WUFEI_FIRMWARE_H
#include "wufei_firmware.h"
#endif
#ifndef _WPL_PLATFORM_H__
#include <wpl_platform.h>
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif
#ifndef WUFEI_HW_GLOBALS_H
#include "wufei_hw_globals.h"
#endif

extern wufei_mem_space_block ufe_mem_space_block[];

WP_CHAR wufei_log[400];
static WP_U8 chip_select_enabled = 0;

static WUFE_status WUFEI_MappingCreate(WP_U32 ufeid, WP_U32 attributes);

static WUFE_status WUFEI_CheckWpEmphyPinmux(WP_U32 connector_index, WP_U32 wp_id, WP_boolean  is16bit);

extern WP_U32 WUFEI_UfeGenRegBaseAddrGet(WP_U32 ufeid);

extern WP_U16 WUFEI_UfeCoreSysConfigGet(WP_U32 ufeid);

//Added by Jonathan, to fix link error
extern WP_U8 WPX_Ufe412CpldInterruptMaskGet(WP_U32 wpid);

/*****************************************************************************
 *
 * Function:  WUFEI_InitializeFirmware
 *
 * Purpose:   Setting the configuration through the host
 *
 * Description:
 *     This function initializes the FPGA configuration
 *     It should be called in order to program the FPGA through the host
 *     Writing 1 byte at a time is necessary for using bitstream encryption
 *
 * Inputs:
 *     ufeid - the UFE id
 *     connector_index  - the serial connector in use on the WDS
 *     attributes - the attributes register
 *
 * Outputs:
 *
 *
 * Return Value:
 *
 * Called by:
 *     WUFE_UfeInit
 *
 ****************************************************************************/
static WUFE_status WUFEI_InitializeFirmware(WP_U32 ufeid, WP_U32 attributes)
{
   WP_U32 i, offset;
   WP_U32 physical_addr;

   offset = WUFE_ATTRIB_CONNECT_OFFSET_V(attributes);
   physical_addr = ufe_mem_space_block[offset].ufe_bases[ufeid].ufe_fpga_base;

   for ( i=0 ; i< ((sizeof(rawData))/2) ; i++)
   {
      WUFEI_REG_SET(physical_addr, (rawData[i] & 0xff00) | ((rawData[i] & 0xff00) >> 8));
      WUFEI_REG_SET(physical_addr, (rawData[i] & 0x00ff) | ((rawData[i] & 0x00ff) << 8));

   }

   /* Delay before HW reset to the FPGA */
   WPL_Delay(2000000);

   return WUFE_OK;
}
/*****************************************************************************
 *
 * Function:  WUFE_UfeInitializeFirmware
 *
 * Purpose:
 *
 * Description:
 *     This function is used to initialize the firmware through the host
 *
 * Inputs:
 *     config - ufe initialization configuration
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *
 * Called by:
 *     User function.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeInitializeFirmware(WUFE_init_config* config)
{
   WUFE_status status = WUFE_OK;
   WP_U32 ufeid = config->ufe_id;
   WP_U8 connector_index = 0;
   static WP_U32 wpid = 0;
   WP_U8 fpga_mask;
#ifdef WUFEI_UFE448
   WP_status wp_status;
#endif  
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      return WUFE_ERR_WUFE_UFEINITIALIZEFIRMWARE_SUPPORT_ONLY_UFE4;
   }
   
   connector_index = WUFE_ATTRIB_CONNECT_OFFSET_V(config->attributes);
   if(connector_index > WUFE_ATTRIB_CONNECT_MAX_OFFSET)
      return WUFE_ERR_INVALID_CONFIG;

   fpga_mask = WPX_Ufe412CpldInterruptMaskGet(wpid);
   WPX_Ufe412CpldInterruptMaskSet(wpid, 0x7f/*WPX_FPGA_INTR_ALL_MASKED*/);

   WUFEI_ChipSelectWaitSet(ufeid, connector_index, config->attributes);

   /* Toggle the program_b signal in order to start the
      configuration programming through the host         */
   WPX_FpgaFirmwareInitializationStart(wpid, connector_index);

#ifdef WUFEI_UFE448
   wp_status = WPX_UFE448InitBRead(0, connector_index);
   if (wp_status != WP_OK)
   {
      WUFE_PRINT_ERR((wufei_log,"Init_b signal has not been asserted\n"));
      return WUFE_ERR_INIT_B_SIG_NOT_ASSERTED;
   }
#else /*UFE412 */
   /* Delay till the init_b signal is asserted */
   WPL_Delay(5000);
#endif

   WUFEI_RETURN_IF_ERROR(status, WUFEI_InitializeFirmware(ufeid, config->attributes));

#ifdef WUFEI_UFE448   
   wp_status = WPX_Ufe448FirmwareVerify(config->ufe_id, connector_index);
   if (wp_status != WP_OK)
   {
      WUFE_PRINT_ERR((wufei_log,"Conf_done signal has not been asserted\n"));
      return WUFE_ERR_CONF_DONE_SIG_NOT_ASSERTED;
   }
#else /*UFE412 */
   /* Delay till the conf_done signal is asserted */
   WPL_Delay(5000);
#endif
   
   if (fpga_mask != 0x7f/*WPX_FPGA_INTR_ALL_MASKED*/)
      WPX_Ufe412CpldInterruptMaskSet(wpid, fpga_mask);
   
   return status;
}


/*****************************************************************************
 *
 * Function:  WUFE_UfeInit
 *
 * Purpose:
 *
 * Description:
 *     This function initializes the ufe core and the board support
 *     It should be called once for each mapping ufe <-> core
 *
 * Inputs:
 *     config - ufe initialization configuration
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *            WUFE_ERR_INVALID_CONFIG
 *            WUFE_ERR_INVALID_REGISTRY_STATE
 * Called by:
 *     User function.
 *
 ****************************************************************************/

WP_U32 wufei_ufe_current_status[WUFE_MAX_N_UFE] = { 0, 0, 0, 0 };

WUFE_status WUFE_UfeInit(WUFE_init_config* config)
{
   WUFE_status status;
   WP_U32 ufeid, wpid = WUFEI_DEFAULT_WINPATH_ID, bandwidth;
   WP_U8 connector_index = 0,upi_status;
   WP_U16 value;

   if(!config)
   {
      WUFE_PRINT_ERR((wufei_log, "WUFE_UfeInit: invalid mapping_config\n"));
      return WUFE_ERR_INVALID_CONFIG;
   }

   ufeid = config->ufe_id;

   if (ufeid >= WUFE_MAX_N_UFE)
   {
      WUFE_PRINT_ERR((wufei_log, "WUFE_UfeInit:error ufeid %d\n", ufeid));
      return WUFE_ERR_INVALID_CONFIG;
   }

   connector_index = WUFE_ATTRIB_CONNECT_OFFSET_V(config->attributes);
   if(connector_index > WUFE_ATTRIB_CONNECT_MAX_OFFSET)
      return WUFE_ERR_INVALID_CONFIG;

   if (~(config->attributes) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS)
      chip_select_enabled = 1;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      status= WP_PortStateGet(connector_index, wpid, &upi_status);
      if (status != WP_OK)
      {
         WUFE_PRINT_ERR((wufei_log, "WUFE_UfeInit:error ufeid %d, WP_PortStateGet returned error\n", ufeid));
         return WUFE_ERR_INVALID_CONFIG;
      }
      if(upi_status == WP_DISABLE){
         WUFE_PRINT_ERR((wufei_log, "WUFE_UfeInit:error ufeid %d on wpid %d, WUFE_UfeInit must be called after the upi port is enabled\n", ufeid, wpid));
         return WUFE_ERR_UFE_INIT_BEFORE_UPI_ENABLE;
      }
   }

   /*sets the UFE lock */
   WUFEI_UFE_LOCK(ufeid);

   /*sets the initial UFE chip select*/
   if(chip_select_enabled)
   {
      WUFEI_ChipSelectWaitSet(ufeid, connector_index, config->attributes);
   }

   /* allocate memory for mapping block within the UFE registry and
    * set the UFE base address and attributes there */
   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_MappingCreate(ufeid, config->attributes),
                                  ufeid);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeSynBuildTimeSet(ufeid, WUFE_SYNTHESIS_BUILD_TIME),
                                  ufeid);

   /*Board support Initialization */
   if (~(config->attributes) & WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFEI_BsFpgaInit(ufeid), ufeid);
   }

   /* read general configuration register*/
   value = WUFEI_UfeCoreSysConfigGet(ufeid);

   bandwidth = WUFEI_UFE_BANDWIDTH_V(value);
   if ((WUFEI_GLOBAL_CFG_BANDWIDTH_OC24 == bandwidth) || 
       (WUFEI_GLOBAL_CFG_BANDWIDTH_OC48 == bandwidth))
   {
      WUFEI_RETURN_IF_ERROR(status, 
                            WUFEI_CheckWpEmphyPinmux(connector_index , wpid, WP_FALSE));
   }
   else
   {
      WUFEI_RETURN_IF_ERROR(status, 
                            WUFEI_CheckWpEmphyPinmux(connector_index , wpid, WP_TRUE));
   }

   wufei_ufe_current_status[ufeid] = 1;

   WUFEI_UFE_UNLOCK(ufeid);

   return WUFE_OK;
}

WUFE_status WUFE_GetCurrentStatus(WP_U32 ufeid, WP_U32 *status)
{
   WUFEI_UFE_LOCK(ufeid);
   *status = wufei_ufe_current_status[ufeid];
   WUFEI_UFE_UNLOCK(ufeid);

   return WUFE_OK;
}

static WUFE_status WUFEI_MappingCreate(WP_U32 ufeid, WP_U32 attributes)
{
   WUFE_status status;

   /* Allocate memory for ufe mapping block, only first enterence to UfeInit function*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_MappingAllocate(ufeid));

   /*Initiates the UFE base address*/
   WUFEI_RETURN_IF_ERROR(status, WUFEI_MappingInit(ufeid, attributes));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeRelease
 *
 * Purpose:
 *
 * Description:
 *     This function releases the ufe structures
 *     It should be called once for each mapping ufe <-> core
 * Inputs:
 *     ufeid  - ufe identifier
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *            WUFE_ERR_INVALID_CONFIG
 *
 * Called by:
 *     User function.
 *
 ****************************************************************************/
WUFE_status WUFE_UfeRelease(WP_U32 ufeid)
{
   WUFE_status status;

   /*lock the wufe_id UFE*/
   WUFEI_UFE_LOCK(ufeid);

   wufei_ufe_current_status[ufeid] = 0;

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_SystemDisable(ufeid,
                                                      WUFE_SYS_EMPHY,
                                                      WUFE_FULL_DUPLEX),
                                  ufeid);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_SystemDelete(ufeid), ufeid);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_BsFpgaRelease(ufeid), ufeid);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_MappingDelete(ufeid), ufeid);

   /*restores the initial UFE chip select*/
   if(chip_select_enabled)
      WUFEI_ChipSelectWaitRestore(ufeid);

   WUFEI_UFE_UNLOCK(ufeid);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_UfeControl
 *
 * Purpose: set the debug mode
 *
 * Description: this function sets the debug mode for ufe.
 *
 * Inputs:
 *       command - the mode to set the debug to.
 *       ufe_id - the ufe id.
 *
 *
 * Return Value:
 *     None.
 *
 * Called by:
 *     User function.
 *
 ****************************************************************************/
void WUFE_UfeControl(WP_U32 command, WP_U32 ufe_id)
{
   WUFEI_RegDbgSet(command,ufe_id);
}

static WUFE_status WUFEI_CheckWpEmphyPinmux(WP_U32 connector_index, WP_U32 wpid, WP_boolean  is16bit)
{   
#ifdef WP_HW_WINPATH3
   WP_status wp_status;
#endif
   ((void)connector_index);
   ((void)wpid);
   ((void)is16bit); 
#ifdef WP_HW_WINPATH3   
   printf("WUFEI_CheckWpEmphyPinmux: connector_index %d, wpid %d, is16bit %d\n", connector_index, wpid, is16bit);
   
   wp_status = WPX_EmphyPinmuxCheck(connector_index, wpid, is16bit);
   if (WP_OK != wp_status)
   {
      WUFE_PRINT_ERR((wufei_log,"Pinmux configuration is not valid\n"));
      return WUFE_ERR_INVALID_EMPHY_WP_PINMUX_CFG;
   }    
#endif

  return WUFE_OK; 
}

