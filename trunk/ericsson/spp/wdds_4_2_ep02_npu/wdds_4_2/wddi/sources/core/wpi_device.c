/*************************************************************
 * (C) Copyright 2004-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * File: wpi_device.c
 *
 * Purpose:  Lower-layer WDDI routines for creation and management of
 *           AAL0 channels and services
 *
 * Functions contained in this file:
 * +----------------------------+---------------------------------------------
 * |   Function name            | Description
 * +----------------------------+---------------------------------------------
 * |   API Functions
 * |   -------------------------+---------------------------------------------
 * |   WP_DeviceAlloc           | User-level function to create a new device
 * |   WP_DeviceCreate          | User-level function to create a new device
 * |   WP_DeviceEnable          | User-level function to enable a device
 * |   WP_DeviceDisable         | User-level function to disable a device
 * |   WP_DeviceStatistics      | User-level function to get device statistics
 * |   WP_DeviceFlush           | User-level function to flush TQ of a
 * |                            |   previously disabled device.
 * |   WP_G999StreamCreate      | User-level function to create a new G999 stream
 * |   WP_G999StreamDelete      | User-level function to delete G999 stream
 * |   WP_G999StreamTxEnable    | User-level function to enable G999 stream
 * |   WP_G999StreamTxDisable   | User-level function to disable G999 stream
 * |   WP_G999StreamStatistics  | User-level function to get G999 stream statistics
 * |   WP_G999StreamstatisticsReset  | User-level function to clear G999 stream statistics
 * |   -------------------------+---------------------------------------------
 * |   Internal WDDI Functions
 * |   -------------------------+---------------------------------------------
 * |   WPI_DeviceCommit         | Commit a device configuration to WinComm
 * |   WPI_GetDeviceFlowmode    | Returns the flowmode of device to caller
 * |   WPI_GetDevicePhy         | Returns the phy number to caller
 * |   WPI_DeviceCreateOverEnet |
 * |   WPI_DeviceCreateOverTdi  |
 * |   WPI_DeviceCreateOverUpi  |
 * +----------------------------+---------------------------------------------
 * |   WPI_VirtualDeviceCreate  | Creates a new virtual device
 * |   WPI_VirtualDeviceEnable  | Enables  virtual device
 * |   WPI_VirtualDeviceDisable | Disables a virtual device
 * |   WPI_VirtualDeviceDelete  | Deletes a virtual device
 * +----------------------------+---------------------------------------------
 * |   WPI_G999StreamCreate     | Creates a new G999 stream
 * +----------------------------+---------------------------------------------
 ****************************************************************************/

#include <stdio.h>
#include <string.h>


#ifndef WP_CONFIG_H
#include "api/wp_config.h"
#endif

#ifndef WPI_TYPES_H
#include "include/core/registry/wpi_types.h"
#endif
#ifndef WPI_WDDI_TYPEDEFS_H
#include "include/core/registry/wpi_wddi_typedefs.h"
#endif
#ifndef WPI_UTILS_H
#include "include/core/utility/wpi_utils.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"
#endif

#ifndef WPI_SERVICES_H
#include "include/core/utility/wpi_services.h"
#endif
#ifndef WPI_MEMORY_INT_H
#include "include/core/memory/wpi_memory_int.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_STATIC_H
#include "include/core/registry/wpi_static.h"
#endif
#ifndef WPI_DEVICE_PACKET_H
#include "include/layer_2/packet/wpi_device_packet.h"
#endif
#ifndef WPI_DEVICE_TDI_H
#include "include/layer_1/tdi/wpi_device_tdi.h"
#endif
#ifndef WPI_PORTS_INT_H
#include "include/core/wpi_ports_int.h"
#endif
#ifndef WPI_PORT_UPI_H
#include "include/layer_1/upi/wpi_port_upi.h"
#endif
#ifndef WPI_PORT_UPI_INT_H
#include "include/layer_1/upi/wpi_port_upi_int.h"
#endif
#ifndef WPI_PORT_ENET_INT_H
#include "include/layer_1/enet/wpi_port_enet_int.h"
#endif
#ifndef WPI_DEVICE_PACKET_INT_H
#include "include/layer_2/packet/wpi_device_packet_int.h"
#endif
#ifndef WPI_DEVICE_ATM_INT_H
#include "include/layer_2/atm/wpi_device_atm_int.h"
#endif
#ifndef WPI_DEVICE_HSPI_INT_H
#include "include/layer_2/packet/hspi/wpi_device_hspi_int.h"
#endif
#ifndef WPI_DEVICE_ENET_INT_H
#include "include/layer_2/packet/enet/wpi_device_enet_int.h"
#endif
#ifndef WPI_DEVICE_UPI_H
#include "include/layer_1/upi/wpi_device_upi.h"
#endif
#ifndef WPI_PORT_ENET_H
#include "include/layer_1/enet/wpi_port_enet.h"
#endif
#ifndef WPI_HSPI_H
#include "include/layer_2/packet/hspi/wpi_hspi.h"
#endif
#ifndef WPI_MC_H
#include "include/layer_2/packet/wpi_mc.h"
#endif
#ifndef WPI_DEVICE_ATM_H
#include "include/layer_2/atm/wpi_device_atm.h"
#endif
#ifndef WPI_ATM_H
#include "include/layer_2/atm/wpi_atm.h"
#endif
#ifndef WPI_PACKET_H
#include "include/layer_2/packet/wpi_packet.h"
#endif
#ifndef WPI_DEVICE_ENET_H
#include "include/layer_2/packet/enet/wpi_device_enet.h"
#endif
#ifndef WPI_DEVICE_HDLC_INT_H
#include "include/layer_2/packet/hdlc/wpi_device_hdlc_int.h"
#endif
#ifndef WPI_DEVICE_FR_INT_H
#include "include/layer_2/packet/fr/wpi_device_fr_int.h"
#endif
#ifndef WPI_PORT_GROUP_INT_H
#include "include/core/wpi_port_group_int.h"
#endif
#ifndef WPI_MC_INT_H
#include "include/layer_2/packet/wpi_mc_int.h"
#endif
#ifndef WPI_ENET_MULTI_CLASS_CLASS_H
#include "include/layer_1/enet/wpi_enet_multi_class_class.h"
#endif
#ifndef WPI_HS_ENET
#include "include/layer_2/packet/hs_enet/wpi_hs_enet.h"
#endif
#ifndef WPI_VENEER_H
#include "veneer/wpi_veneer.h"
#endif
#if _WP_MODULE_DUMP_ENABLE_
#ifndef WPI_DUMP_H
#include "include/core/wpi_dump.h"
#endif
#endif
#ifndef WPI_DISPLAY_H
#include "include/core/wpi_display.h"
#endif
#ifndef WPI_DPS_FEATURES_H
#include "include/core/features/wpi_dps_features.h"
#endif
#ifndef WPI_MFR_H
#include "include/layer_2/packet/wpi_mfr.h"
#endif
#ifndef WPI_MLPPP_H
#include "include/layer_2/packet/wpi_mlppp.h"
#endif
#ifndef WPI_IMA_H
#include "include/layer_2/atm/ima/wpi_ima.h"
#endif
#ifndef WPI_CAPABILITY_H
#include "include/core/features/wpi_capability.h"
#endif
#ifndef WPI_EMPHY_H
#include "include/layer_1/upi/wpi_upi_emphy.h"
#endif
#ifndef WPI_EMPHY_TRANS_INT_H
#include "include/layer_2/trans/wpi_emphy_trans.h"
#endif
#ifndef WPI_UPI_PER_PHY_ATM_IMA_H
#include "include/layer_1/upi/wpi_upi_per_phy_atm_ima.h"
#endif
#ifndef WPI_UPI_PER_PHY_POS_ATM_H
#include "include/layer_1/upi/wpi_upi_per_phy_pos_atm.h"
#endif
#ifndef WPI_IW_GENERIC_H
#include "include/iw/iwgp/wpi_iw_generic.h"
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif
#ifndef WPI_TDI_HW_H
#include "include/layer_1/tdi/wpi_tdi_hw.h"
#endif
#ifndef WPI_PORT_TDI_INT_H
#include "include/layer_1/tdi/wpi_port_tdi_int.h"
#endif
#ifndef WPI_HSPI_INT_H
#include "include/layer_2/packet/hspi/wpi_hspi_int.h"
#endif

#include "include/layer_2/packet/hspi/wpi_hspi_global_class.h"
#include "include/core/wpi_port_group_class.h"
#include "include/core/wpi_port_group_private.h"
#include "include/layer_2/atm/wpi_atm_class.h"
#include "include/core/wpi_tpt_class.h"
#include "include/core/wpi_system_class.h"
#include "include/layer_2/atm/gbond/wpi_gbond_class.h"
#include "include/core/hardware/wpi_hw.h"

#if _WP_MODULE_DUMP_ENABLE_
/* atm */
static WP_CHAR *wpi_atm_ext_phy_stats_string[] = {
   "WP_ATM_EXT_PHY_STATS_DISABLE",
   "WP_ATM_EXT_PHY_STATS_ENABLE"
};

/* packet */
static WP_CHAR *wpi_pkt_intmode_string[] = {
   "WP_PKT_INTMODE_DISABLE",
   "WP_PKT_INTMODE_ENABLE"
};

static WP_CHAR *wpi_pkt_stat_string[] = {
   "WP_PKT_STAT_DISABLE",
   "WP_PKT_STAT_ENABLE"
};

/* enet */
static WP_CHAR *wpi_enet_address_mode_string[] = {
   "WP_ENET_ADDR_END",
   "WP_ENET_ADDR_ACCEPTALL"
};

static WP_CHAR *wpi_enet_lookup_mode_string[] = {
   "WP_ENET_MACLOOKUP_TABLE",
   "WP_ENET_MACLOOKUP_CAM",
   "WP_ENET_MAC_HASH_LOOKUP"
};

static WP_CHAR *wpi_enet_filter_string[] = {
   "WP_ENET_FILTER_DISCARD",
   "WP_ENET_FILTER_PROCESS",
   "WP_ENET_FILTER_HOSTTERM"

};

/* hdlc */
static WP_CHAR *wpi_hdlc_crc_string[] = {
   "WP_HDLC_CRC16",
   "WP_HDLC_CRC32"
};

#if 0
/* tdi */
static WP_CHAR *wpi_tdi_idle_string[] = {
   "WP_TDI_IDLEMODE_IDLE",
   "WP_TDI_IDLEMODE_FLAG"
};
#endif

/* tdi mc */
static WP_CHAR *wpi_tdi_mch_idle_string[] = {
   "WP_TDI_MCH_IDLEMODE_FLAG",
   "WP_TDI_MCH_IDLEMODE_IDLE"
};

static WP_CHAR *wpi_tq_mode_string[] = {
   "WP_TQ_SLOW_MODE",
   "WP_TQ_FAST_MODE"
};

static WP_CHAR *wpi_iw_background_string[] = {
   "WP_IW_BKGND_NOT_USED",
   "WP_IW_BKGND_USED"
};
#endif

/* Prototypes of virtual devices API */
static WP_status WPI_VirtualDeviceCreate(WP_handle device,
                                         WP_device virtualdevtype,
                                         void *config);
static WP_status WPI_VirtualDeviceEnable(WP_handle device,
                                         WP_U32 direction);
static WP_status WPI_VirtualDeviceDisable(WP_handle device,
                                          WP_U32 direction);
static WP_status WPI_VirtualDeviceDelete(WP_handle device);

/*****************************************************************************
 *
 * Function:   WPI_DeviceCommit
 *
 * Purpose:    Commit to DPS structures and WinComm registers all
 *             configuration date related to WinPath devices
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     wpid             WinPath ID
 *     is_sys_commit    Indication for whether we reached here
 *                      from SysCommit or from another location
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully, or an error code otherwise
 *
 * Called by:
 *     WP_SysCommit  (wpi_wddi.c)
 *
 ****************************************************************************/

WP_status WPI_DeviceCommit(WP_U32 wpid)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   WP_U32 index;
   WP_S32 tdi_wa_enabled = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   /*********************
    * upi devices  *
    *********************/
   status = WPI_DeviceCommitUpi(wpid, resources);
   WPI_BREAK_NOT_OK(status, error_return);

   /*************************
    * tdi atm devices  *
    *************************/
   status = WPI_DeviceCommitTdiAtm(wpid);
   WPI_BREAK_NOT_OK(status, error_return);

   /********************************
    * tdi hdlc/frame relay devices *
    ********************************/
   for (index = WPI_tdi_hdlc_base; index <= WPI_tdi_hdlc_last; index++)
   {
      dev_entry = &resources->entry[index];
      /* if not initialized, skip device */
      if (!WPI_INITIALIZED(dev_entry->state))
         continue;
      status = WPI_DeviceCommitHdlcTdi(wpid, resources,
                                       (index - WPI_tdi_hdlc_base),
                                       WPI_RES_DEV_PKT(dev_entry));
      WPI_BREAK_NOT_OK(status, error_return);
   }

   /**************************
    * enet devices           *
    **************************/

   /********************/
   /* PHYSICAL DEVICES */
   /********************/
   for (index = WPI_enet_device_base; index <= WPI_enet_device_last; index++)
   {
      status = WPI_EnetDeviceCommit(wpid,
                                    resources,
                                    index,
                                    WP_FALSE);
      WPI_BREAK_NOT_OK(status, error_return);
   }

   /**************************************/
   /* VIRTUAL DEVICES (MULTI-CLASS)) */
   /**************************************/

   /* Sets tx status 2 register for all virtual
    * devices (i.e. user defined bits of tx cfu port) */
   status = WPI_MultiClassDeviceCommit(wpid);
   WPI_BREAK_NOT_OK(status, error_return);

   status = WPI_HwLagDeviceCommit(resources);
   WPI_BREAK_NOT_OK(status, error_return);

   /* Transparent devices */
   for (index = WPI_tdi_trans_base; index <= WPI_tdi_trans_last; index++)
   {
      status = WPI_DeviceCommitTdiTrans(wpid, index);
      WPI_BREAK_NOT_OK(status, error_return);
   }

   
   /*TDI_workaround*/
  WPI_RETURN_IF_ERROR(status, WPI_CapabilityRead(wpid,
                                                 WPI_CAP_TDI_WA,
                                                 &tdi_wa_enabled));
   if(tdi_wa_enabled)
   {
      status = WPI_TdiLookupTableCreate(wpid);
      WPI_BREAK_NOT_OK(status, error_return);      
   }
   status = WP_OK;

  error_return:
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceCreateOverUpi
 *
 * Purpose:
 *             handle the creation of devices over UPI ports
 *
 * Description:
 *             Create a device over the UPI port according to the
 *             devtype parameter and other configuration structures.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *             WP_handle - valid device handle we were able to create one.
 *
 * Called by:
 *     WP_DeviceCreate  (wpi_device.c)
 *
 ****************************************************************************/
WP_handle WPI_DeviceCreateOverUpi(WP_handle port,
                                  WP_U32 phy,
                                  WP_device devtype,
                                  void *config,
                                  wpi_resource_entry *port_entry,
                                  WP_boolean *emphy_trans_device,
                                  WP_boolean *per_phy_device,
                                  wpi_resource_entry **device_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_upi *res_upi = NULL;
   WP_U16 max_upi_phy = 0;
   wpi_resource_block *resources = NULL;
   WP_U32 dev_index = 0;
   WP_U32 port_index;
   WP_U32 wpid;
   WP_handle handle;
   WP_U32 emphy_phy=0,emphy_dev=0;
   WP_U16 emphy_virtual_phy=0xffff;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   port_index = WPI_HANDLE_INDEX(port);
   wpid = WPI_HANDLE_WPID(port);
   resources = &wpi_registry_ptr->pwp_resources[wpid];
   res_upi = WPI_RES_UPI(port_entry);

   /* If mutli clav was selected in the port,
    * check that the clav marcro was used
    * Note that we check only the tx_mclav value since the tx and rx
    * values should be consistent
    * (this is checked in the wp_portcreate()  */
   if (res_upi->hw.tx_mclav == WPI_UPI_HW_MCLAV_ENABLE)
   {
      status =  WPI_HwUpiMclavPhyGet(&phy);
      WPI_RETURN_IF_ERROR_STATUS(status);
   }
   else if(!res_upi->uses_emphy)
   {
      if (phy >> WP_CLAV_OFFSET != 0)
      {
         return WPI_ERROR(WP_ERR_PHY);
      }

   }

   /* Set the max_phy_index in the device entry */
   if (phy > res_upi->max_phy_index)
      res_upi->max_phy_index = phy;

   /* Set the num_of_phys in the device entry
    * For WP_UPI_POS_ATM_PACKET (per-phy) this field is handle in WPI_HwDeviceCreatePerPhyPosAtm */
   if (res_upi->mode != WP_UPI_POS_ATM_PACKET)
   {
      res_upi->num_of_phys++;
   }

   /*************************************************************
    *
    * Permit calling of specific function based on port type and
    * device type.  If this switch is used, then the function
    * should completely generate the handle and return.  It is
    * not permitted to continue to the rest of the code.  Right
    * now, this method is used for per-phy ATM/IMA.
    *************************************************************/
   switch(res_upi->mode)
   {
      case WP_UPI_UTOPIA_ATM_IMA:
      {
         *per_phy_device = WP_TRUE;
         return WPI_DeviceCreatePerPhyAtmIma(port, phy, devtype, config, WP_TRUE);
      }
      case WP_UPI_POS_ATM_PACKET:
      case WP_UPI_POS_ATM_PACKET_MIB:
      {
         *per_phy_device = WP_TRUE;
         return WPI_HwDeviceCreatePerPhyPosAtm(port, phy, devtype, config, WP_TRUE);
      }
   }

   if (!WPI_CHECK_UPI_DEVICE(devtype))
      return WPI_ERROR(WP_ERR_DEVICE);

   if (!res_upi->uses_emphy)
   {
      switch (devtype)
      {
         case WP_DEVICE_ATM:
         case WP_DEVICE_HDLC:
         case WP_DEVICE_HSPOS:
            WPI_HwUpiMaxPhyNumGet(&max_upi_phy);

            if(phy >= max_upi_phy)
               return WPI_ERROR(WP_ERR_PHY);
            dev_index = WPI_INDEX_UPI_PHY(port_index, phy);
            break;
         case WP_DEVICE_IMA:
         case WP_DEVICE_GBOND:
         case WP_DEVICE_FR:
            WPI_RETURN_IF_ERROR(status,
                                WPI_Limits( wpid, WPI_LIMITS_UPI_PHY, &max_upi_phy));
            if(phy >= max_upi_phy - 1)/* either 63 or 127 is reserved */
               return WPI_ERROR(WP_ERR_PHY);
            dev_index = WPI_INDEX_UPI_PHY(port_index, phy);
            break;
         default:
            return WPI_ERROR(WP_ERR_DEVICE);
      }
   }
   else
   {
      /* under EMPHY port */
      wpi_emphy_block *emphy_block =
         wpi_registry_ptr->pwp_block[wpid].emphy_block;

      WPI_EmphyPhyAndDeviceGet(phy, &emphy_phy, &emphy_dev);
      if (!WPI_CHECK_EMPHY_DEVICE(devtype))
         return WPI_ERROR(WP_ERR_DEVICE);

      switch (devtype)
      {
         case WP_DEVICE_ATM:
         case WP_DEVICE_IMA:
         case WP_DEVICE_GBOND:
            /* check that the phy number <336 */
            WPI_GET_EMY_INDEX(emphy_virtual_phy,emphy_block->atm_dev);
            dev_index = WPI_EMY_CALC_ATM_DEV_INDEX(emphy_virtual_phy);
            break;
         case WP_DEVICE_HDLC:
         case WP_DEVICE_FR:
         case WP_DEVICE_HSPOS:
            /* check that the phy number < 8064 */
            WPI_GET_EMY_INDEX(emphy_virtual_phy, emphy_block->pkt_dev);
            dev_index = WPI_EMY_CALC_FR_DEV_INDEX(emphy_virtual_phy);
            break;
         case WP_DEVICE_TRANS:
            /* emphy_virtual_phy will be calculated later */
            emphy_virtual_phy = 0;
            dev_index = emphy_phy;
            *emphy_trans_device = WP_TRUE;
            break;
         default:
            return WPI_ERROR(WP_ERR_DEVICE);
      }

      /* check if there is free dev */
      if (emphy_virtual_phy == WPI_NO_FREE_EMY_DEV)
         return WPI_ERROR(WP_ERR_DEVICE);

      phy = 0;
   }

/***********************************************************/
/***********************************************************/
/***********************************************************/

   if (! (*emphy_trans_device) )
   {
      /* dev_index passed in here is the index of the device entry in the
         registry */

      WPI_ACCESS_RES((*device_entry), resources, dev_index);
      if ((*device_entry) == NULL)
         /* Means we were not able to access this device entry */
         return WPI_ERROR(WP_ERR_WDDI);

      /* if the state DEVICE_CREATED is enabled, then reject the
         call because the devicecreate has already been called for
         this device */
      if (WPI_DEVICE_STATE((*device_entry)) & WPI_STATE_DEVICE_CREATED)
         return WPI_ERROR(WP_ERR_DUPLICATE);
   }

/***********************************************************/
/***********************************************************/
/***********************************************************/

   switch (devtype)
   {
      case WP_DEVICE_IMA:
      case WP_DEVICE_GBOND:
      case WP_DEVICE_ATM:
      {
         WP_device_upi_atm *upi_config;
         wpi_res_device_atm_upi *dev_atm_upi;
         wpi_atm *atm;
         if (res_upi->uses_hspi)
            return WPI_ERROR(WP_ERR_UPI_PORT_CONFIG_AND_DEVICE_INCOMPATIBLE);
         atm = WPI_RegistryAtm(wpid);

         /* ATM entry should be initialized */
         if (atm == 0)
            return WPI_ERROR(WP_ERR_CONFIG);

         if (WPI_DEVICE_STATE((*device_entry)) != WPI_STATE_INITIALIZED)
         {
            dev_atm_upi =
               (wpi_res_device_atm_upi *)
               WPI_WDDI_MALLOC(sizeof(wpi_res_device_atm_upi));
            if (dev_atm_upi == NULL)
               return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
            /* clear the table */
            memset(dev_atm_upi, 0, sizeof(wpi_res_device_atm_upi));
            (*device_entry)->resource = dev_atm_upi;
            (*device_entry)->state = WPI_STATE_INITIALIZED;
            dev_atm_upi->atm.device.next = NULL;
         }
         else
            dev_atm_upi =
               (wpi_res_device_atm_upi *) (*device_entry)->resource;
         upi_config = (WP_device_upi_atm *) config;
         /*reassembly timeout initializing*/
         dev_atm_upi->atm.aal5_timeout_list.head = 0;
         dev_atm_upi->atm.aal5_timeout_list.tail = 0;
         dev_atm_upi->atm.ras_timeout = upi_config->rx_ras_timeout;
         dev_atm_upi->atm.ras_last_sampling = 0;
         dev_atm_upi->atm.atm_ufe_clear_channel = 0;

         if (res_upi->uses_emphy)
         {
            WP_device_emphy_atm *param = (WP_device_emphy_atm*)config;

            status = WPI_EmphyAtmDeviceParam(resources,param);
            WPI_RETURN_IF_ERROR_STATUS(status);

            /* identify UFE clear channel device */
            if (res_upi->mode == WP_UPI_UFE4)
            {
               if (param->tx_shaping_mode & WP_EMPHY_ATM_FAST)
               {
                  if (devtype != WP_DEVICE_IMA)
                  {
                     dev_atm_upi->atm.atm_ufe_clear_channel = 1;
                     /* clear the fast bit to merge to the existed solution */
                     param->tx_shaping_mode &= ~WP_EMPHY_ATM_FAST;
                     if (param->tx_shaping_mode == WP_ATM_DEV_SHAPING_HIERARCHICAL)
                        return WPI_ERROR(WP_ERR_UNSUPPORTED);
                  }
                  /* the fast flag can't be set for IMA device */
                  else return WPI_ERROR(WP_ERR_CONFIG);
               }
            }

            dev_atm_upi->atm.device.emphy_param =
               WPI_RES_EMY_HIER_SHAPING_F(param->tx_shaping_mode) |
               WPI_RES_EMY_PHY_NUM_F(emphy_phy)|
               WPI_RES_EMY_DEV_NUM_F(emphy_dev)|
               WPI_RES_EMY_ATM_UNI_NNI_F(param->uni_nni_mode)|
               WPI_RES_EMY_ENABLE_F(1);

            if(param->idle_format != WP_UNUSED)
               dev_atm_upi->atm.device.emphy_param |=
                  WPI_RES_EMY_ATM_IDLE_FORMAT_F(param->idle_format);

            dev_atm_upi->atm.device.emphy_virtual_phy = emphy_virtual_phy;

            /* Alternate PHY ID - used for reporting IMA state/status */
            dev_atm_upi->atm.device.line_id = param->line_id;
         }
         else if (upi_config->tx_shaping_mode & WP_EMPHY_ATM_FAST)
         {
            /* protection that this flag can't be set for non-UFE */
            return WPI_ERROR(WP_ERR_CONFIG);
         }

         if ((devtype != WP_DEVICE_IMA) && (dev_atm_upi->atm.atm_ufe_clear_channel == 0))
         {
            /* Allocate TPT */
            wpi_tpt *res_tpt = NULL;
            res_tpt = WPI_RegistryTpt(wpid);
            WPI_TptAllocateUpi(res_tpt, port_index - WPI_upi_base, phy);
         }

         (*device_entry)->resource = dev_atm_upi;

         handle = WPI_DeviceCreateUpiAtm(port, phy,
                                         upi_config, dev_atm_upi,
                                         res_upi, devtype);
         WPI_RETURN_IF_ERROR_STATUS(handle);

         if (res_upi->uses_emphy)
         {
            if (WPI_REGISTRY_USE() == WP_TRUE)
            {
               status = WPI_EmphyDeviceCreate(wpid,&dev_atm_upi->atm.device);
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
         }
         break;
      }
      case WP_DEVICE_HDLC:
      case WP_DEVICE_FR:
      {
         wpi_res_device_pkt *dev_upi;

         if (res_upi->uses_hspi)
            return WPI_ERROR(WP_ERR_UPI_PORT_CONFIG_AND_DEVICE_INCOMPATIBLE);

         /* if port EMPHY set EMPHY device parameters */
         if (res_upi->uses_emphy)
         {
            handle =
               WPI_DeviceCreatePos(port, phy, emphy_virtual_phy,
                                   devtype, config, res_upi,
                                   emphy_virtual_phy+WPI_emy_pkt_dev_base,
                                   (*device_entry),
                                   &dev_upi);
            WPI_RETURN_IF_ERROR_STATUS(handle);

            dev_upi->device.emphy_param = (WP_U32) (
               WPI_RES_EMY_PHY_NUM_F(emphy_phy)|
               WPI_RES_EMY_DEV_NUM_F(emphy_dev)|
               WPI_RES_EMY_ENABLE_F(1));
            dev_upi->device.emphy_virtual_phy = emphy_virtual_phy;
            /*If ufe2/ufe3 mode is enabled, crc removal is required
              because the crc is not extracted by the hardware*/
            if(res_upi->uses_emphy == WPI_UPI_EMPHY_MODE_UFE2 ||
               res_upi->uses_emphy == WPI_UPI_EMPHY_MODE_UFE3 ||
               res_upi->uses_emphy == WPI_UPI_EMPHY_MODE_UFE4)/*rizh*/
               dev_upi->crc_rem = 1;

            dev_upi->device.device_handle = handle;
            if (WPI_REGISTRY_USE() == WP_TRUE) /* after SysCommit */
            {
               status = WPI_EmphyDeviceCreate(wpid, &dev_upi->device);
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
         }
         else
         {
            handle =
               WPI_DeviceCreatePos(port, phy, phy, devtype, config, res_upi,
                                   dev_index, (*device_entry), &dev_upi);
            WPI_RETURN_IF_ERROR_STATUS(handle);
         }

         /* Need only deal with transmit queue allocation if
            we're in multi-flow mode */
         if ((dev_upi->flowmode == WPI_DEVICE_PKT_FLOWMODE_MULTI) &&
             (dev_upi->device.tx_bit_rate != 0))
         {
            WP_U8 *tx_tq_types;

            if (!res_upi->uses_emphy)
            {
               /* Allocate TPT */
               wpi_tpt *res_tpt = NULL;
               res_tpt = WPI_RegistryTpt(wpid);
               WPI_TptAllocateUpi(res_tpt, port_index - WPI_upi_base, phy);
            }

            if (devtype == WP_DEVICE_HDLC)
            {
               tx_tq_types = ((WP_device_upi_pos *) config)->tx_tq_types;
            }
            else
            {
               /* WP_DEVICE_FR */
               tx_tq_types = ((WP_device_upi_fr *) config)->tx_tq_types;
            }

            WPI_HwUpiMultiFlowDemandModeSet(res_upi, devtype);

            status = WPI_PktTqTypeCopy(tx_tq_types, dev_upi);
            WPI_RETURN_IF_ERROR_STATUS(status);
         }

         break;
      }
      case WP_DEVICE_TRANS:
      {
         handle = WPI_EmphyTransDeviceCreate(port,
                                             emphy_phy,
                                             emphy_dev,
                                             res_upi,
                                             config);
         WPI_RETURN_IF_ERROR_STATUS(handle);
         break;
      }
      case WP_DEVICE_HSPOS:
      {
         WP_device_upi_hspos *upi_config;
         wpi_res_device_hspi_pos *dev_hspos;

         if (res_upi->mode != WP_UPI_POS_HIGHSPEED)
            return WPI_ERROR(WP_ERR_UPI_PORT_CONFIG_AND_DEVICE_INCOMPATIBLE);

         upi_config = (WP_device_upi_hspos *) config;

         dev_hspos =
            (wpi_res_device_hspi_pos *)
            WPI_WDDI_MALLOC(sizeof(wpi_res_device_hspi_pos));
         (*device_entry)->resource = dev_hspos;
         if (dev_hspos == NULL)
               return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
         (*device_entry)->state = WPI_STATE_INITIALIZED;
         /* clear the table */
         memset( dev_hspos, 0, sizeof(wpi_res_device_hspi_pos));
         handle = WPI_DeviceCreateUpiHspi(port,
                                          phy,
                                          upi_config,
                                          (wpi_res_device_hspi *)
                                          dev_hspos, res_upi,
                                          WPI_pg_hspi_hspos1,
                                          WP_DEVICE_HSPOS);
         WPI_RETURN_IF_ERROR_STATUS(handle);
         break;
      }
      default:
         return WPI_ERROR(WP_ERR_NODE);
   }
   return handle;

} /* WPI_DeviceCreateInitUpi */

WP_U32 WPI_CheckDeviceCreationOverTdi(wpi_resource_entry *device_entry,wpi_resource_block *resources ,WP_U32 dev_index)
{
   WPI_ACCESS_RES(device_entry, resources, dev_index);
   if ((device_entry) != NULL)
   {
      if ((WPI_DEVICE_STATE(device_entry)) & WPI_STATE_DEVICE_CREATED) 
         return WP_ERR_DIFFERENT_PROTOCOLS_OVER_TDI_PORT;
      else
         return WP_OK;
   }
   else
   {
      /* Means we were not able to access this device entry */
      return WP_ERR_WDDI;
   }
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceCreateOverTdi
 *
 * Purpose:
 *             handle the creation of devices over TDI ports
 *
 * Description:
 *             Create a device over the TDI port according to the
 *             devtype parameter and other configuration structures.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *             WP_handle - valid device handle we were able to create one.
 *
 * Called by:
 *     WP_DeviceCreate  (wpi_device.c)
 *
 ****************************************************************************/
WP_handle WPI_DeviceCreateOverTdi(WP_handle port,
                                  WP_U32 phy,
                                  WP_device devtype,
                                  void *config,
                                  wpi_resource_entry *port_entry,
                                  wpi_resource_entry **device_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 dev_index = 0;
   WP_U32 port_index;
   WP_U32 wpid;
   WP_handle handle;
   wpi_resource_block *resources = NULL;
   WP_S32 tdi_wa_enabled = 0;   
   WP_U8 atm_bypass_lkp = 0;
   wpi_res_device_atm_tdi *dev_atm_tdi = NULL;
   wpi_resource_entry *device_entry_2=NULL,*device_entry_3=NULL;	 
   WP_U32 dev_index_2 = 0;
   WP_U32 dev_index_3 = 0;	 
   WP_U32 error = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   port_index = WPI_HANDLE_INDEX(port);
   wpid = WPI_HANDLE_WPID(port);
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   /* In the case of TDM, the specific dev_index value will
   depend on the type of node being created */
   if (!WPI_CHECK_TDI_DEVICE(devtype))
      return WPI_ERROR(WP_ERR_NODE);

 /*Retrieve all the possible device indices */
   switch (devtype)
   {
      case WP_DEVICE_IMA:
      case WP_DEVICE_ATM:
         if (!WPI_DpsFeatureCheckStatus(wpid,
                                    WPI_FEATUREMODE_PORT_HANDLE,
                                    port,
                                    WPI_DPS_MODULE_ATMTC_ENABLE))
         {
            return WPI_ERROR(WP_ERR_DPS_FEATURE_UNAVAILABLE);
         }
         dev_index = WPI_INDEX_TDI_ATM(port_index);
         dev_index_2 = WPI_INDEX_TDI_HDLC(port_index);
         dev_index_3 = WPI_INDEX_TDI_TRANS(port_index);
         break;

      case WP_DEVICE_HDLC:
      case WP_DEVICE_FR:
         dev_index = WPI_INDEX_TDI_HDLC(port_index);
         dev_index_2 = WPI_INDEX_TDI_ATM(port_index);
         dev_index_3 = WPI_INDEX_TDI_TRANS(port_index);
         break;

      case WP_DEVICE_TRANS:
         dev_index = WPI_INDEX_TDI_TRANS(port_index);
         dev_index_2 = WPI_INDEX_TDI_HDLC(port_index);
         dev_index_3 = WPI_INDEX_TDI_ATM(port_index);
         break;
         
      default:
         return WPI_ERROR(WP_ERR_NODE);
   }

/***********************************************************/
/***********************************************************/
/***********************************************************/

   WPI_ACCESS_RES((*device_entry), resources, dev_index);
   if ((*device_entry) == NULL)
   /* Means we were not able to access this device entry */
      return WPI_ERROR(WP_ERR_WDDI);

   /* if the state DEVICE_CREATED is enabled, then reject the
   call because the devicecreate has already been called for
   this device */
   if (WPI_DEVICE_STATE((*device_entry)) & WPI_STATE_DEVICE_CREATED)
      return WPI_ERROR(WP_ERR_DUPLICATE);

   /*check if the device of this type is already created.
   If created then return an error indicating that Different device types over the same TDI port can not be created*/
   error = WPI_CheckDeviceCreationOverTdi(device_entry_2,resources,dev_index_2);
   if(error != WP_OK)
      return WPI_ERROR(error);

   error = WPI_CheckDeviceCreationOverTdi(device_entry_3,resources,dev_index_3);
   if(error != WP_OK)
      return WPI_ERROR(error);

/***********************************************************/
/***********************************************************/
/***********************************************************/


   switch (devtype)
   {
      case WP_DEVICE_IMA:
      case WP_DEVICE_ATM:
      {
         WP_device_tdm_atm *tdm_config = (WP_device_tdm_atm *) config;
         wpi_atm *atm;

         atm = WPI_RegistryAtm(wpid);

         /* ATM entry should be initialized */
         if (atm == 0)
            return WPI_ERROR(WP_ERR_CONFIG);

         if (WPI_DEVICE_STATE((*device_entry)) != WPI_STATE_INITIALIZED) {
            dev_atm_tdi =
               (wpi_res_device_atm_tdi *)
               WPI_WDDI_MALLOC(sizeof(wpi_res_device_atm_tdi));
            if (dev_atm_tdi == NULL)
               return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
            /* clear the table */
            memset(dev_atm_tdi, 0, sizeof(wpi_res_device_atm_tdi));
            (*device_entry)->resource = dev_atm_tdi;
            dev_atm_tdi->next = NULL;
         }
         else
            dev_atm_tdi =
               (wpi_res_device_atm_tdi *) (*device_entry)->resource;

         /*reassembly timeout initializing*/
         dev_atm_tdi->atm.aal5_timeout_list.head = 0;
         dev_atm_tdi->atm.aal5_timeout_list.tail = 0;
         dev_atm_tdi->atm.ras_timeout = tdm_config->rx_ras_timeout;
         dev_atm_tdi->atm.ras_last_sampling = 0;

         /* Create the device (ATM or IMA) */
         handle = WPI_DeviceCreateTdiAtm(wpid, port, tdm_config,
                                         dev_atm_tdi,
                                         WPI_RES_TDI(port_entry),
                                         devtype);

         if (WP_ERROR_P(handle))
         {
            return handle;
         }
         if (devtype == WP_DEVICE_ATM)
         {
            /* Allocate TPT */
            wpi_tpt *res_tpt = NULL;
            res_tpt = WPI_RegistryTpt(wpid);
            WPI_TptAllocateTdi(res_tpt, WP_DEVICE_ATM, port_index - WPI_tdi_base);
         }

         break;
      }
      case WP_DEVICE_HDLC:
      case WP_DEVICE_FR:
      {
         wpi_res_device_pkt *dev_hdlc_tdi;

         handle =
            WPI_DeviceCreateTdiHdlc(wpid, port, devtype, config,
                                    dev_index, WPI_RES_TDI(port_entry),
                                    (*device_entry));
         if (WP_ERROR_P(handle))
         {
            return handle;
         }	 
         dev_hdlc_tdi = (wpi_res_device_pkt *) (*device_entry)->resource;
         /* Need only deal with transmit queue allocation if we're
            in multi-flow mode */
         if (dev_hdlc_tdi->flowmode == WPI_DEVICE_PKT_FLOWMODE_MULTI)
         {
            WP_U8 *tx_tq_types;
            wpi_tpt *res_tpt = NULL;
            res_tpt = WPI_RegistryTpt(wpid);
            /* Allocate TPT */
            WPI_TptAllocateTdi(res_tpt, WP_DEVICE_HDLC, port_index - WPI_tdi_base);

            if (devtype == WP_DEVICE_HDLC)
            {
               tx_tq_types =
                  ((WP_device_tdm_hdlc *) config)->tx_tq_types;
            }
            else
            {
               /* WP_DEVICE_FR */
               tx_tq_types =
                  ((WP_device_tdm_fr *) config)->tx_tq_types;
            }

            status = WPI_PktTqTypeCopy(tx_tq_types,
                                       dev_hdlc_tdi);
            WPI_RETURN_IF_ERROR_STATUS(status);
         }
         break;
      }
      case WP_DEVICE_TRANS:
      {
         /* Initialize the transparent TDI device */
         handle =
            WPI_DeviceCreateTdiTrans(wpid, port,
                                     (WP_device_tdm_trans *)config,
                                     dev_index, (*device_entry),
                                     WPI_RES_TDI(port_entry));
         WPI_RETURN_IF_ERROR_STATUS(handle);
         break;
      }
      default:
         return WPI_ERROR(WP_ERR_NODE);
   }
   
   /*TDI_workaround*/
   WPI_RETURN_IF_ERROR(status, WPI_CapabilityRead(wpid,
                                                  WPI_CAP_TDI_WA,
                                                  &tdi_wa_enabled));   
   if ((tdi_wa_enabled ==1) && (WPI_REGISTRY_USE() == WP_TRUE))
   {
      if ((devtype == WP_DEVICE_ATM))
         atm_bypass_lkp = (dev_atm_tdi->atm.bypass_lkp);
      else
         atm_bypass_lkp = 0;

      status = WPI_TdiLookupTableUpdate(wpid,
                                        devtype,
                                        (port_index - WPI_tdi_base),
                                        atm_bypass_lkp,
                                        port_index);   
      WPI_BREAK_NOT_OK(status, error_return);      
   }

   return handle;

  error_return:
   return status;   
} /* WPI_DeviceCreateInitTdi */

/*****************************************************************************
 *
 * Function:   WPI_DeviceCreateOverEnet
 *
 * Purpose:
 *             handle the creation of devices over Ethernet ports
 *
 * Description:
 *             Create a device over the Ethernet port according to the
 *             devtype parameter and other configuration structures.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *             WP_handle - valid device handle we were able to create one.
 *
 * Called by:
 *     WP_DeviceCreate  (wpi_device.c)
 *
 ****************************************************************************/
WP_handle WPI_DeviceCreateOverEnet(WP_handle port,
                                   WP_U32 phy,
                                   WP_device devtype,
                                   void *config,
                                   wpi_resource_entry *port_entry,
                                   wpi_resource_entry **device_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 dev_index = 0;
   WP_U32 port_index;
   WP_U32 wpid;
   WP_handle handle;
   wpi_resource_block *resources = NULL;
   WP_boolean create_swap_enet_device = WP_FALSE;
   WP_device_enet *enet_config = NULL;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if ( (devtype != WP_DEVICE_ENET) &&
        (devtype != WP_DEVICE_ENET_G999) )
   {
      return WPI_ERROR(WP_ERR_DEVICE);
   }

   port_index = WPI_HANDLE_INDEX(port);
   wpid = WPI_HANDLE_WPID(port);
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   if (((wpi_res_enet*)WPI_RES_ENET(port_entry))->flowmode == WP_FLOWMODE_MULTI)
   {
      dev_index = WPI_INDEX_ENET(port_index) + phy * WPI_HwEnetMaxNumGet();
   }
   else if (((wpi_res_enet*)WPI_RES_ENET(port_entry))->flowmode == WP_FLOWMODE_FAST ||
            ((wpi_res_enet*)WPI_RES_ENET(port_entry))->flowmode ==
            WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE)
   {
      dev_index = WPI_HwEnetDeviceIndexGet(port_index, phy);
   }

/***********************************************************/
/***********************************************************/
/***********************************************************/

   WPI_ACCESS_RES((*device_entry), resources, dev_index);
   if ((*device_entry) == NULL)
      /* Means we were not able to access this device entry */
      return WPI_ERROR(WP_ERR_WDDI);

   /* if the state DEVICE_CREATED is enabled, then reject the
      call because the devicecreate has already been called for
      this device */
   if (WPI_DEVICE_STATE((*device_entry)) & WPI_STATE_DEVICE_CREATED)
   {
      if ( (((wpi_res_enet*)WPI_RES_ENET(port_entry))->enable_g999 != WP_ENABLE) ||
           (devtype != WP_DEVICE_ENET) )
      {
         /* An Enet device is allowed to be created over an enet port
          * which has G999 physical device, in order to allow swap
          * capability between the 2 device types. Otherwise, return error */
         return WPI_ERROR(WP_ERR_DUPLICATE);
      }
      else
      {
         /* A G999 device has been created over this port,
          * check if an ENET device has also been created for
          * ENET<->G999 swap mode. */
         wpi_res_device_hspi_enet *dev_enet = NULL;
         dev_enet = WPI_RES_DEV_HSPI_ENET((*device_entry));
         if (dev_enet->swap_enet_device_created)
            return WPI_ERROR(WP_ERR_DUPLICATE);
         create_swap_enet_device = WP_TRUE;
      }
   }
/***********************************************************/
/***********************************************************/
/***********************************************************/

   enet_config = (WP_device_enet *)config;
   if (create_swap_enet_device == WP_FALSE)
   {
      wpi_res_device_hspi_enet *dev_enet =
         (wpi_res_device_hspi_enet *)
         WPI_WDDI_MALLOC(sizeof(wpi_res_device_hspi_enet));

      (*device_entry)->resource = dev_enet;
      /* mark this resource as initialized so that it can be
       * accessed by the registry functions */
      (*device_entry)->state = WPI_STATE_INITIALIZED;

      if (dev_enet == NULL)
      {
         return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
      }

      memset(dev_enet, 0, sizeof(wpi_res_device_hspi_enet));

      dev_enet->swap_enet_device_created = WP_FALSE;

      /* Creates an ENET / G999 device from scratch */
      handle = WPI_EnetDeviceCreate(wpid, port, phy,
                                    enet_config, dev_enet,
                                    WPI_RES_ENET(port_entry), devtype);
      WPI_RETURN_IF_ERROR_STATUS(handle);
   }
   else //swap_g999_with_enet_mode == WP_TRUE)
   {
      wpi_res_device_hspi_enet *dev_enet = NULL;
      dev_enet = WPI_RES_DEV_HSPI_ENET((*device_entry));
      /* Merges G999 with ENET parameters towards a possible swap */
      handle = WPI_EnetSwapG999DeviceCreate(wpid,
                                            port,
                                            phy,
                                            enet_config,
                                            dev_enet,
                                            devtype);
      WPI_RETURN_IF_ERROR_STATUS(handle);
      dev_enet->swap_enet_device_created = WP_TRUE;
   }
   return handle;
} /* WPI_DeviceCreateOverEnet */

/*****************************************************************************
 *
 * Function:   WPI_MultiClassDeviceCommit
 *
 * Purpose:    Commit Multi Class Device settings
 *            (by using WPI_EnetMultiClassDeviceCommit)
 *
 * Description:  Allocates resources and initialization for a previously
 *               created Multi class Ethernet Device at system commit.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK
 *
 * Called by:
 *     WPI_DeviceCommit (wpi_devices.c)
 ****************************************************************************/
WP_status WPI_MultiClassDeviceCommit(WP_U32 wpid)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_enet_multi_class *res_enet_multi_class = NULL;

   WPI_REGISTRY_OBJECT_GET(lock_status,
                        WPI_RegistryEnetMultiClassGet,
                        WPI_REG_LOCKED,
                        &res_enet_multi_class,
                        wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (res_enet_multi_class != NULL)
   {
      wpi_res_enet_multi_class_port *enet_multi_class_port_entry;
      wpi_res_enet_multi_class_device *res_class;
      WP_U32 port_index = 0;
      WP_U32 class_index = 0;

      for(port_index = (WP_PORT_ENET1 - WPI_enet_base); port_index <= (WP_PORT_XGI2  - WPI_enet_base) ; port_index++)
      {
         /* Go over all port indices in the multi_class registry */
         enet_multi_class_port_entry = res_enet_multi_class->port[port_index];

         if (!enet_multi_class_port_entry)
            continue;

         for(class_index = enet_multi_class_port_entry->start_index;
             class_index <= enet_multi_class_port_entry->end_index;
             class_index++)
            /* Go over all the virtual devices (classes) in multi_class registry */
         {
            res_class = enet_multi_class_port_entry->multi_class_dev[class_index];

            status = WPI_EnetMultiClassDeviceCommit(wpid,
                                                    res_enet_multi_class,
                                                    port_index,
                                                    class_index,
                                                    WP_FALSE);
            WPI_RETURN_IF_ERROR_STATUS(status);
         }
      }
   }
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryEnetMultiClassRelease,
                               WPI_REG_UNLOCKED,
                               &res_enet_multi_class);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);


   return WP_OK;
}

/************************************************************************
 *
 * Function:  WP_DeviceCreate
 *
 * Purpose: Creates a WDDI Device object with user supplied configuration
 *          configuration information.
 *
 * Description: The function accepts user supplied configuration information
 * for a specific device type and then creates the device. The
 * device information is entered into the registry. This function
 * performs checks to see if the combination of port and device are valid.
 * Like WP_PortCreate, this function calls a device specific routine.
 * Note: The registry structure for the device structure is allocated in each
 * device specific * DeviceCreate function. This function just passes pointer
 * to the enclosing registry entry.
 *
 * Inputs:
 *  (A Handle an already created port)
 *           -- OR --
 *  (In case of creation of a virtual device, a main physical device handle,
 *   with which this virtual device is associated.)
 *
 * A phy value if the device is a UPI port
 * A configuration for a specific device for a port. See wp_wddi.h for
 * structure definitions.
 * A Handle to an already created port
 * A phy value if the device is a UPI port
 *
 * Outputs:
 * A device entry in the registry filled with this user supplied
 * configuration.
 *
 * Return Value:
 *              WP_handle - valid device handle we were able to create one.
 *              WP_ERR_HANDLE - port handle passed in is not valid
 *              WP_ERR_PHY - phy number passed in is not valid for UPI device
 *              WP_ERR_DEVICE - wrong type of device being created for a port
 *              WP_ERR_PORT - wrong type of port corresponding to node
 *              WP_ERR_CONFIG - invalid or null config argument
 *              WP_ERR_INCORRECT_REGISTRY_STATE
 *
 * Re-entrancy:
 *  This function is responsible for freeing the locks on both the device
 *  and port registry entries.
 *
 **************************************************************************/

WP_handle WP_DeviceCreate(WP_handle port,
                          WP_U32 phy,
                          WP_device devtype,
                          void *config)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *device_entry = NULL;
   wpi_resource_entry *port_entry;
   wpi_res_device* dev = NULL;
   WP_handle handle;
   WP_U32 port_type, port_index;
   WP_U32 wpid;
   WP_U32 dps_feature = 0;
   WP_boolean emphy_trans_device = WP_FALSE;
   WP_boolean per_phy_device = WP_FALSE;

   wpid = WPI_HANDLE_WPID(port);

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
   if (WPI_REGISTRY_INITIALIZED() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   port_index = WPI_HANDLE_INDEX(port);

   /* Obtain port entry from registry. Any returns after this should
      unlock the registry entry and return.  */
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   /* Call port specific routine to write values into device structure
      in the registry.  Each of these routines will access the port
      specific information in the registry to create the device. First
      check if port is initialized or not.  */
   WPI_ACCESS_RES(port_entry, resources, port_index);
   if (port_entry == NULL)
      WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_WDDI);
   /* If virtual device of Multi Channel HDLC/FR types should be created
    * call the specific internal WPI_VirtualDeviceCreate routine and return.
    * No need to continue with the rest of the code.
    */
   if (devtype == WP_DEVICE_ENET_MULTI_CLASS ||
       devtype == WP_DEVICE_ENET_MULTI_CLASS_INTSW ||
       devtype == WP_DEVICE_MC_HDLC ||
       devtype == WP_DEVICE_MC_FR ||
       WPI_HANDLE_MFR_P(port))
   {
      /* In case of virtual device, port is a actually a device handle.
       * because a virtual device is created over a device (not over a port). */
      WP_handle device_handle = port;

      handle = WPI_VirtualDeviceCreate(device_handle, devtype, config);
      WPI_WDDI_DETACH();
      return handle;
   }

   /* continue with the regular device */
   /* Check DPS feature availability */
   switch (devtype)
   {
      case WP_DEVICE_HDLC:
         dps_feature = WPI_DPS_MODULE_HDLC_PPP_ENABLE;
         break;
      case WP_DEVICE_IMA:
         dps_feature = WPI_DPS_MODULE_IMA_ENABLE;
         break;
      case WP_DEVICE_ATM:
         dps_feature = WPI_DPS_MODULE_ADDRLOOKUP_ENABLE;
         break;
      case WP_DEVICE_HSPOS:
         dps_feature = WPI_HwHspiType(WP_FALSE);
         break;
      case WP_DEVICE_GENET:
         /* No longer supported */
         WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_NOT_SUPPORTED_ANYMORE);
         break;
      case WP_DEVICE_FR:
         dps_feature = WPI_DPS_MODULE_FR_ENABLE;
         break;
      case WP_DEVICE_ENET:
      case WP_DEVICE_ENET_G999:
         dps_feature = WPI_HwEthernetDpsType(((wpi_res_enet*)WPI_RES_ENET(port_entry))->flowmode);
         break;
      case WP_DEVICE_GBOND:
         dps_feature = WPI_DPS_MODULE_GBOND_ENABLE;
         break;
      case WP_DEVICE_IW_HOST:
         dps_feature = WPI_DPS_MODULE_INTERWORKING_ENABLE;
         break;
      case WP_DEVICE_TRANS:
         dps_feature = 0;
         break;
      default:
         WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_CONFIG);
   }

   if (dps_feature != 0)
   {
      if (!WPI_DpsFeatureCheckStatus(wpid,
                                     WPI_FEATUREMODE_PORT_HANDLE,
                                     port,
                                     dps_feature))
      {
         WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_DPS_FEATURE_UNAVAILABLE);
      }
   }

   /* Check if handle is a port handle */
   if (WPI_HANDLE_TYPE(port) != WPI_handle_port)
      WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_HANDLE);

   port_type = WPI_HANDLE_SUBTYPE(port);

   if (port_type == WPI_PORT_IW_HOST)
   {
      /* Nothing needs to be created or entered in the registry for this;
       * only an identifier is supplied. */
      handle = WPI_MAKE_HANDLE(WPI_handle_device,
                               WPI_pg_pkt_iw_host, wpid, 0);

      WPI_WDDI_DETACH();
      return handle;
   }

   if (!WPI_INITIALIZED(WPI_PORT_STATE(port_entry)))
   {
      /* cannot create device without port */
      WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_PORT);
   }

   /* Other device specific configuration errors will be caught later */
   if (config == NULL)
      WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_CONFIG);

   /* Validate node type depending on the type of port passed in.
    * i.e we can't create hdlc device on a upi port
    * If OK, set the variable dev_index to the registry index value of the
    * device that will be created.
    */

   /*************************************************************
    *
    * Permit calling of specific function based on port type and
    * device type.
    *
    *************************************************************/
   switch (port_type)
   {
      case WPI_PORT_UPI:
      {
         handle = WPI_DeviceCreateOverUpi(port,
                                          phy,
                                          devtype,
                                          config,
                                          port_entry,
                                          &emphy_trans_device,
                                          &per_phy_device,
                                          &device_entry);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(handle);
         break;
      }
      case WPI_PORT_TDI:
      {
         handle = WPI_DeviceCreateOverTdi(port,
                                          phy,
                                          devtype,
                                          config,
                                          port_entry,
                                          &device_entry);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(handle);
         break;
      }
      case WPI_PORT_ENET:
      {
         handle = WPI_DeviceCreateOverEnet(port,
                                           phy,
                                           devtype,
                                           config,
                                           port_entry,
                                           &device_entry);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(handle);
         break;
      }

      default:
         /* Unrecognized port type */
         WPI_BREAK(status, WP_DeviceCreateErrExit, WP_ERR_PORT);
   }
   if ( (emphy_trans_device == WP_FALSE) &&
        (per_phy_device == WP_FALSE) )
   {
      wpi_res_enet_multi_class *res_enet_multi_class = NULL;
      device_entry->state = WPI_STATE_INITIALIZED | WPI_STATE_DEVICE_CREATED;
      if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(handle))
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryEnetMultiClassGet,
                                 WPI_REG_LOCKED,
                                 &res_enet_multi_class,
                                 wpid);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
         dev = WPI_MULTI_CLASS_DEV_BYHANDLE(res_enet_multi_class,
                                            handle);
      }
      else
      {
         WPI_REGISTRY_DEVICE_GET_BYHANDLE(lock_status,
                                          &dev,
                                          handle);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
      }
      dev->rx_priority = WP_IW_RX_BIND_PRIORITY_DISABLED;
      dev->device_handle = handle;
   }



   WPI_WDDI_DETACH();
   return handle;

 WP_DeviceCreateErrExit:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_DeviceEnable
 *
 * Purpose:    User called function to enable a WinPath device
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     device         Handle of the device to be enabled
 *
 *     direction      Specifies whether device is to be enabled to handle
 *                    Rx traffic, Tx traffic, or both
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully, or the list of errors below
 *         WP_ERR_HANDLE
 *         WP_ERR_DEVICE
 *         WP_ERR_PORT
 *         WP_ERR_INCORRECT_REGISTRY_STATE
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_DeviceEnable(WP_handle device,
                          WP_U32 direction)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   WP_U32 wpid;
   wpi_res_device *dev;

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   /* If the device is a virtual device, call the WP_VirtualDeviceEnable
    * function and return. No need to process further in this code. */
   if (WPI_HANDLE_TYPE(device) == WPI_handle_virtual_device)
   {
      status = WPI_VirtualDeviceEnable(device, direction);
      goto WP_DeviceEnableEnd;
   }

   /* Check if handle is a Device Handle */
   if (WPI_HANDLE_TYPE(device) != WPI_handle_device)
      WPI_BREAK(status, WP_DeviceEnableEnd, WP_ERR_HANDLE);

   /* If this device is for the IW host virtual port, there's nothing to do.
      Just return. */
   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_pkt_iw_host)
   {
      status = WP_OK;
      goto WP_DeviceEnableEnd;
   }

   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_emphy_trans)
   {
      status = WPI_EmphyTransDeviceEnable(device, direction);
      goto WP_DeviceEnableEnd;
   }

   wpid = WPI_HANDLE_WPID(device);
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   dev_entry = &resources->entry[WPI_HANDLE_INDEX(device)];

   if (!WPI_INITIALIZED(dev_entry->state))
      WPI_BREAK(status, WP_DeviceEnableEnd, WP_ERR_DEVICE);

   dev = WPI_RES_DEV(dev_entry);

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_IwDeviceEnableDefaultFlowAggregationCheck(device));

   if (!WPI_RES_EMY_ENABLE_V(dev->emphy_param))
   {
      WP_boolean tod_available = WP_TRUE;

      if (dev->devtype == WP_DEVICE_ENET ||
          dev->devtype == WP_DEVICE_ENET_G999 ||
          dev->devtype == WP_DEVICE_HSPOS)
      {
         wpi_res_device_pkt *dev_pkt = (wpi_res_device_pkt *)dev;

         tod_available = WPI_DeviceTodIsAvailable(dev);

         if (dev->devtype == WP_DEVICE_ENET)
         {
            wpi_res_device_hspi_enet *hs_enet_device = WPI_RES_DEV_HSPI_ENET(dev_entry);
            if (hs_enet_device->enet_multi_class_used)
            {
               tod_available = WP_FALSE;
            }
         }

         if((dev_pkt->flowmode == WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL) && !tod_available)
         {
            /* if the device is in Hierarchical flowmode, and the L1/mask wasnt created yet
               the CFU port will be enaled during the first L1 group enable */
            dev_pkt->device_enable_pending = WP_TRUE;
         }
      }

      /* if the TOD source is available enable the CFU-Tx port now */
      if(tod_available)
      {
         /* enable the CFU Tx port for the appropriate serials (WP3 only) */
         WPI_DETACH_RETURN_IF_ERROR(status,
                                    WPI_SaDeviceStateSet(wpid,dev,WP_DIRECTION_TX,WP_ENABLE));
      }
   }

   WPI_DETACH_RETURN_IF_ERROR(status,
                              WPI_SaDeviceStateSet(wpid,dev,WP_DIRECTION_RX,WP_ENABLE));

   if (dev->functions && dev->functions->device_enable)
   {
      /*
       * WPI_EnetDeviceEnable
       * WPI_DeviceEnableTdi
       * WPI_DeviceEnableUpi
       */
      WPI_DETACH_RETURN_IF_ERROR(status,
                          dev->functions->device_enable(wpid, dev_entry, direction));
   }
   else
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_PORT);
   }

   if ((status == WP_OK) && (direction == WP_DIRECTION_DUPLEX ||
                             direction == WP_DIRECTION_RX))
      dev_entry->state |= WPI_STATE_ACTIVE;

  WP_DeviceEnableEnd:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_DeviceDisable
 *
 * Purpose:    User called function to enable a WinPath device
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     device         Handle of the device to be enabled
 *
 *     direction      Specifies whether device is to be enabled to handle
 *                    Rx traffic, Tx traffic, or both
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully or an error code listed here
 *         WP_ERR_HANDLE
 *         WP_ERR_PORT
 *         WP_ERR_DEVICE
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_DeviceDisable(WP_handle device,
                           WP_U32 direction)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   WP_U32 wpid;
   wpi_res_device *dev;

   /* If this device is for the IW host virtual port, there's nothing to do.
      Just return. */
   if (WPI_HANDLE_TYPE(device) == WPI_handle_device &&
       WPI_HANDLE_SUBTYPE(device) == WPI_pg_pkt_iw_host)
      return WP_OK;

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   /* If the device is a virtual device, call the WP_VirtualDeviceDisable
    * function and return. No need to process further in this code. */
   if (WPI_HANDLE_TYPE(device) == WPI_handle_virtual_device)
   {
      status = WPI_VirtualDeviceDisable(device, direction);
      WPI_WDDI_DETACH();
      return status;
   }

   /* Check if handle is a Device Handle */
   if (WPI_HANDLE_TYPE(device) != WPI_handle_device)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_emphy_trans)
   {
      status = WPI_EmphyTransDeviceDisable(device, direction);
      WPI_WDDI_DETACH();
      return status;
   }

   wpid = WPI_HANDLE_WPID(device);
   resources = &wpi_registry_ptr->pwp_resources[wpid];
   dev_entry = &resources->entry[WPI_HANDLE_INDEX(device)];

   if (!WPI_INITIALIZED(dev_entry->state))
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_DEVICE);
   }

   dev = WPI_RES_DEV(dev_entry);

   /* disable the CFU Tx ports for the appropriate serials (WP3 only) */
   if (!WPI_RES_EMY_ENABLE_V(dev->emphy_param))
   {
      if ((direction == WP_DIRECTION_TX) || (direction == WP_DIRECTION_DUPLEX))
      {
         WPI_DETACH_RETURN_IF_ERROR(status,
                                    WPI_SaDeviceStateSet(wpid,dev, WP_DIRECTION_TX, WP_DISABLE));
      }
   }
   
   if (dev->functions && dev->functions->device_disable)
      status = dev->functions->device_disable(wpid, dev_entry, direction);
   else
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_PORT);
   }   

   /* disable the CFU Rx ports for the appropriate serials (WP3 only) */
   if (!WPI_RES_EMY_ENABLE_V(dev->emphy_param))
   {
      if ((direction == WP_DIRECTION_RX) || (direction == WP_DIRECTION_DUPLEX))
      {
         WPI_DETACH_RETURN_IF_ERROR(status,
                                    WPI_SaDeviceStateSet(wpid,dev, WP_DIRECTION_RX, WP_DISABLE));
      }
   }

   if ((status == WP_OK) && (direction == WP_DIRECTION_DUPLEX ||
                             direction == WP_DIRECTION_RX))
      dev_entry->state &= ~WPI_STATE_ACTIVE;

   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_DeviceDelete
 *
 * Purpose:    User called function to delete a WinPath device
 *
 * Description:
 *
 * Inputs:
 *     device         Handle of the device to be deleted
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully or an error code otherwise.
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_DeviceDelete(WP_handle device)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   wpi_resource_block *resources;
   wpi_resource_entry *device_entry;
   wpi_res_device *dev_res;
   WP_U32 dev_index;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 wpid;

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   /* If the device is a virtual device, call the WP_VirtualDeviceDelete
    * function and return. No need to process further in this code. */
   if (WPI_HANDLE_TYPE(device) == WPI_handle_virtual_device)
   {
      status = WPI_VirtualDeviceDelete(device);
      goto device_delete_end;
   }

   if (WPI_HANDLE_TYPE(device) != WPI_handle_device)
      WPI_BREAK(status, device_delete_end, WP_ERR_HANDLE);

   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_emphy_trans)
   {
      status = WPI_EmphyTransDeviceDelete(device);
      goto device_delete_end;
   }

   dev_index = WPI_HANDLE_INDEX(device);
   wpid = WPI_HANDLE_WPID(device);
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   WPI_ACCESS_RES(device_entry, resources, dev_index);
   if (device_entry == NULL)
      WPI_BREAK(status, device_delete_end, WP_ERR_HANDLE);

   if (!WPI_INITIALIZED(device_entry->state))
      WPI_BREAK(status, device_delete_end, WP_ERR_DEVICE);

   dev_res = (wpi_res_device *) device_entry->resource;

   /* checks if there are any channels assign to the device, which
    * are not deleted */
   if (dev_res->ch_counter >0)
      WPI_BREAK(status, device_delete_end, WP_ERR_CHANNEL_NOT_DELETED);

   if (WPI_RES_EMY_ENABLE_V(dev_res->emphy_param))
   {
      status = WPI_EmphyDeviceDelete(wpid,dev_res);
      if (WP_ERROR_P(status))
         goto device_delete_end;
   }
   else if ((dev_res->functions) && (dev_res->functions->device_delete))
   {
      status = dev_res->functions->device_delete(device);
      WPI_BREAK_IF_ERROR(status, device_delete_end);
   }
   else
      WPI_BREAK(status, device_delete_end, WP_ERR_UNIMPLEMENTED);

   device_entry->state &= ~WPI_STATE_DEVICE_CREATED;

  device_delete_end:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_DeviceStatistics
 *
 * Purpose:    Gather and return statistics for a specified device
 *
 * Description:
 *     Shows the device statistics.
 *
 * Inputs:
 *
 *     device - Handle of the device
 *
 * Outputs:
 *
 *     stats - pointer to the stats table the user allocated to return the
 *             stats
 *
 * Return Value:
 *     WP_OK is function completes successfully, or an error code otherwise
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_DeviceStatistics(WP_handle device, void *stats)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 wpid, dev_index;
   wpi_res_device *dev = NULL;

   /* Check if device handle is valid */
   if ((WPI_HANDLE_TYPE(device) != WPI_handle_device) &&
       (WPI_HANDLE_TYPE(device) != WPI_handle_virtual_device) &&
       (WPI_HANDLE_TYPE(device) != WPI_handle_ima) &&
       (WPI_HANDLE_TYPE(device) != WPI_handle_gbond_group))
      return WPI_ERROR(WP_ERR_HANDLE);

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   wpid = WPI_HANDLE_WPID(device);
   dev_index = WPI_HANDLE_INDEX(device);

   if (WPI_HANDLE_DEVICE_P(device))
   {
      switch (WPI_HANDLE_SUBTYPE(device))
      {
      case WPI_pg_atm_upi1:
      case WPI_pg_atm_upi2:
      case WPI_pg_atm_upi3:
      case WPI_pg_atm_upi4:
         status = WPI_UpiAtmStatistics(device, stats);
         break;
      case WPI_pg_atm_tdi:
         status = WPI_TdiAtmStatistics(device, stats);
         break;
      case WPI_pg_emphy_trans:
         status = WPI_EmphyTransDeviceStatistics(device, stats);
         break;
      case WPI_pg_pkt_upi1:
      case WPI_pg_pkt_upi2:
      case WPI_pg_pkt_upi3:
      case WPI_pg_pkt_upi4:
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryDeviceGet,
                                 WPI_REG_LOCKED,
                                 &dev,
                                 wpid,
                                 dev_index);
         if (!dev)
            WPI_BREAK(status, WP_DeviceStatisticsEnd, WP_ERR_WDDI);
         if (dev->devtype == WP_DEVICE_FR)
            status = WPI_UpiFrStatistics(device, stats);
         else if (dev->devtype == WP_DEVICE_ATM ||
                  dev->devtype == WP_DEVICE_GBOND)
            status = WPI_UpiAtmStatistics(device, stats);
         else
            status = WPI_UpiHdlcStatistics(device, stats);
         break;
      case WPI_pg_pkt_tdi:
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryDeviceGet,
                                 WPI_REG_LOCKED,
                                 &dev,
                                 wpid,
                                 dev_index);
         if (!dev)
            WPI_BREAK(status, WP_DeviceStatisticsEnd, WP_ERR_WDDI);
         if (dev->devtype == WP_DEVICE_FR)
            status = WPI_TdiFrStatistics(device, stats);
         else
            status = WPI_TdiHdlcStatistics(device, stats);
         break;
      case WPI_pg_pkt_enet1:
      case WPI_pg_pkt_enet2:
      case WPI_pg_pkt_enet3:
      case WPI_pg_pkt_enet4:
      case WPI_pg_pkt_enet_g999:
         status = WPI_EnetStatistics(device, stats);
         break;
      case WPI_pg_hspi_hspos1:
      case WPI_pg_hspi_hspos2:
      case WPI_pg_hspi_hspos3:
      case WPI_pg_hspi_hspos4:
         status = WPI_UpiHsposStatistics(device, stats);
         break;
      case WPI_pg_tdi_trans:
         status = WPI_TdiTransStatistics(device, stats);
         break;
      default:
         status = WPI_ERROR(WP_ERR_DEVICE);
      }
   }
   else if (WPI_HANDLE_VIRTUAL_DEVICE_P(device)) /* virtual device */
   {
      switch (WPI_HANDLE_SUBTYPE(device))
      {
      case WP_DEVICE_MC_HDLC:
      case WP_DEVICE_MC_FR:
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryDeviceGet,
                                 WPI_REG_LOCKED,
                                 &dev,
                                 wpid,
                                 dev_index);
         if (!dev)
            WPI_BREAK(status, WP_DeviceStatisticsEnd, WP_ERR_WDDI);
         if (dev->devtype == WP_DEVICE_MC_FR)
            status = WPI_TdiFrStatistics(device, stats);
         else
            status = WPI_TdiHdlcStatistics(device, stats);
         break;
      }
      case WP_DEVICE_MFR_FR: /* MFR */
      {
         wpi_resource_entry *dev_entry = WPI_RES_ENTRY(wpid, dev_index);
         if (!WPI_INITIALIZED(dev_entry->state))
            WPI_BREAK(status, WP_DeviceStatisticsEnd, WP_ERR_DEVICE);
         status = WPI_MfrDeviceStatistics(dev_entry, stats);
         break;
      }
      case WP_DEVICE_ENET_MULTI_CLASS:
      case WP_DEVICE_ENET_MULTI_CLASS_INTSW:
      {
         status = WPI_EnetMultiClassStatistics(device,
                                       stats,
                                       WP_FALSE /* g999_stats */);
         break;
      }

      default:
         status = WPI_ERROR(WP_ERR_DEVICE);
      }
   }
   else
      status = WPI_ERROR(WP_ERR_DEVICE);

 WP_DeviceStatisticsEnd:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_DeviceStatisticsReset
 *
 * Purpose:    Clear statistics for a specified device
 *
 * Description:
 *     Clears device statistics.
 *
 * Inputs:
 *
 *     device - Handle of the device
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully, or an error code otherwise
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_DeviceStatisticsReset(WP_handle device,void *stats)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 wpid, dev_index;
   wpi_res_device *dev;
   WP_U32 pg;

   /* Check if device handle is valid */
   if ((WPI_HANDLE_TYPE(device) != WPI_handle_device) &&
       (WPI_HANDLE_TYPE(device) != WPI_handle_virtual_device) &&
       (WPI_HANDLE_TYPE(device) != WPI_handle_ima) &&
       (WPI_HANDLE_TYPE(device) != WPI_handle_gbond_group))
      return WPI_ERROR(WP_ERR_HANDLE);

   if(stats == NULL)
      return WPI_ERROR(WP_ERR_STATISTICS);

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   wpid = WPI_HANDLE_WPID(device);
   dev_index = WPI_HANDLE_INDEX(device);
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(device))
   {
      pg = WPI_pg_pkt_enet_shared; /* WPI_pg_pkt_enet1 */
   }
   else
   {
      pg = WPI_HANDLE_SUBTYPE(device);
   }
   switch (pg)
   {
      case WPI_pg_atm_upi1:
      case WPI_pg_atm_upi2:
      case WPI_pg_atm_upi3:
      case WPI_pg_atm_upi4:
         status = WPI_UpiAtmStatisticsReset(device,stats);
         break;
      case WPI_pg_pkt_upi1:
      case WPI_pg_pkt_upi2:
      case WPI_pg_pkt_upi3:
      case WPI_pg_pkt_upi4:
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryDeviceGet,
                                 WPI_REG_LOCKED,
                                 &dev,
                                 wpid,
                                 dev_index);
         if (!dev)
         {
            status = WPI_ERROR(WP_ERR_WDDI);
            WPI_BREAK(status, WpiErrRet, WP_ERR_WDDI);
         }
         if (dev->devtype == WP_DEVICE_ATM ||
             dev->devtype == WP_DEVICE_GBOND)
            status = WPI_UpiAtmStatisticsReset(device,stats);
         else if (dev->devtype == WP_DEVICE_FR)
            status = WPI_UpiFrStatisticsReset(device, stats);
         else
            status = WPI_UpiHdlcStatisticsReset(device, stats);
         break;
      case WPI_pg_atm_tdi:
         status = WPI_TdiAtmStatisticsReset(device,stats);
         break;

      case WPI_pg_pkt_tdi:
         status = WPI_TdiHdlcStatisticsReset(device,stats);
         break;
      case WPI_pg_hspi_hspos1:
      case WPI_pg_hspi_hspos2:
      case WPI_pg_hspi_hspos3:
      case WPI_pg_hspi_hspos4:
         status = WPI_ResetUpiHsposStatistics(device,stats);
         break;
      case WPI_pg_pkt_enet1:
      case WPI_pg_pkt_enet2:
      case WPI_pg_pkt_enet3:
      case WPI_pg_pkt_enet4:
      case WPI_pg_pkt_enet_g999:
         status = WPI_EnetStatisticsReset(device,stats);
         break;
      default:
         status = WPI_ERROR(WP_ERR_DEVICE);
   }

 WpiErrRet:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_GetDeviceFlowmode
 *
 * Purpose:
 *             Returns the flow mode of the device
 *
 * Description:
 *             This function returns the flow mode of the device to the
 *             caller.
 *
 * Inputs:
 *             device  handle of device
 *
 * Outputs:
 *
 * Return Value:
 *             Flow mode of the device in registry
 *
 * Called by:
 *     WPI_ChannelAllocate  (wpi_devices.c)
 *
 ****************************************************************************/

WP_U8 WPI_GetDeviceFlowmode(WP_handle device)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   wpi_res_device_pkt *dev_pkt;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return 0;
   dev_entry =
      WPI_RES_ENTRY(WPI_HANDLE_WPID(device), WPI_HANDLE_INDEX(device));
   res_dev = WPI_RES_DEV(dev_entry);
   if (res_dev->devtype == WP_DEVICE_ATM ||
       res_dev->devtype == WP_DEVICE_GBOND)
      return WPI_DEVICE_PKT_FLOWMODE_INVALID;
   else
   {
      dev_pkt = WPI_RES_DEV_PKT(dev_entry);
      return (dev_pkt->flowmode);
   }
}

/*****************************************************************************
 *
 * Function:   WPI_GetDevicePhy
 *
 * Purpose:
 *             Returns the phy number the device belongs to
 *
 * Description:
 *             This function returns the phy number of the device to the
 *             caller.
 *
 * Inputs:
 *             device  handle of device
 *
 * Outputs:
 *
 * Return Value:
 *             Phy number of the device
 *
 * Called by:
 *     WPI_ChannelAllocate  (wpi_devices.c)
 *
 ****************************************************************************/

WP_U32 WPI_GetDevicePhy(WP_handle device)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return 0;

   if (WPI_HANDLE_SUBTYPE(device) != WPI_pg_emphy_trans)
   {
      wpi_resource_entry *dev_entry;
      wpi_res_device *dev;

      dev_entry =
         WPI_RES_ENTRY(WPI_HANDLE_WPID(device), WPI_HANDLE_INDEX(device));
      dev = WPI_RES_DEV(dev_entry);
      if (WPI_RES_EMY_ENABLE_V(dev->emphy_param) == WPI_UPI_EMPHY_ENABLED)
         return (dev->emphy_virtual_phy);
      else
         return (dev->phy_number);
   }
   else
   {
      return (WPI_RegistryEmphyTransDeviceGetPhy(device));
   }
}

/********************* Virtual Device Functions ****************************/

/*****************************************************************************
 *
 * Function: WPI_VirtualDeviceCreate
 *
 * Purpose: Create a new virtual device
 *
 * Description:
 *     The function accepts user supplied configuration information
 *     for a specific virtual device type and then creates the device. The
 *     device information is entered into the registry and the memory
 *     is allocated for this device. This function checks user-supplied
 *     parameters.
 *
 * Inputs:
 *     device          Handle of the main (normal) device
 *     virtualdevtype  Type of this virtual device
 *     config          User-supplied configuration parameters
 *
 * Outputs:
 *     The new virtual device is allocated and initialized.
 *
 * Return Value:
 *     Handle of this new virtual device on success, an error code otherwise.
 *
 * Called by:
 *     WP_DeviceCreate
 *
 ****************************************************************************/
static WP_handle WPI_VirtualDeviceCreate(WP_handle device,
                                         WP_device virtualdevtype,
                                         void *config)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_handle handle;
   WP_U32 wpid = WPI_HANDLE_WPID(device);

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if (WPI_HANDLE_TYPE(device) != WPI_handle_device)
      return WPI_ERROR(WP_ERR_HANDLE);

   /* Check if specific configuration exist */
   if (config == NULL)
      return WPI_ERROR(WP_ERR_CONFIG);

   if (virtualdevtype == WP_DEVICE_MC_HDLC ||
       virtualdevtype == WP_DEVICE_MC_FR)
   {
      WPI_RETURN_ERROR_IF_FALSE(
         WPI_DpsFeatureCheckStatus(wpid,
                                   WPI_FEATUREMODE_PORT_HANDLE,
                                   0,
                                   WPI_DPS_MODULE_MULTI_CHANNEL_HDLC_ENABLE),
         WP_ERR_DPS_FEATURE_UNAVAILABLE);

      if (virtualdevtype == WP_DEVICE_MC_FR)
      {
         WPI_RETURN_ERROR_IF_FALSE(
            WPI_DpsFeatureCheckStatus(wpid,
                                      WPI_FEATUREMODE_PORT_HANDLE,
                                      0,
                                      WPI_DPS_MODULE_FR_ENABLE),
            WP_ERR_DPS_FEATURE_UNAVAILABLE);
      }
      else
      {
         WPI_RETURN_ERROR_IF_FALSE(
            WPI_DpsFeatureCheckStatus(wpid,
                                      WPI_FEATUREMODE_PORT_HANDLE,
                                      0,
                                      WPI_DPS_MODULE_HDLC_PPP_ENABLE),
            WP_ERR_DPS_FEATURE_UNAVAILABLE);
      }

      handle = WPI_MchDeviceCreate(device, virtualdevtype, config);
   }
   else if (WPI_HANDLE_MFR_P(device))
   {
      handle = WPI_MfrDeviceCreate(device, config);
   }
   else
   {
      /* WP_DEVICE_ENET_MULTI_CLASS */
      /* WP_DEVICE_ENET_MULTI_CLASS_INTSW */
      wpi_res_device *dev_res;
      WPI_REGISTRY_DEVICE_GET_BYHANDLE(lock_status,
                                       &dev_res,
                                       device);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      if (dev_res->functions && dev_res->functions->virtual_device_create)
      {
         /* WPI_EnetMultiClassDeviceCreate */
         handle = dev_res->functions->virtual_device_create(device, virtualdevtype, config);
      }
      else
         handle = WPI_ERROR(WP_ERR_WDDI);
   }

   return handle;
}

/*****************************************************************************
 *
 * Function:   WPI_VirtualDeviceEnable
 *
 * Purpose:    User called function to enable a WinPath virtual device
 *
 * Description:
 *    This functions makes the virtual device active for data processing.
 *    It checks the type of this virtual device and calls the device
 *    specific routine that enables the device.
 *
 * Inputs:
 *     device         Handle of the device to be enabled
 *     direction      Specifies whether device is to be enabled to handle
 *                    Rx traffic, Tx traffic, or both
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully, an error code otherwise.
 *
 * Called by:
 *     WP_DeviceEnable, WP_G999StreamEnable
 *
 ****************************************************************************/
static WP_status WPI_VirtualDeviceEnable(WP_handle device,
                                         WP_U32 direction)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_entry *dev_entry;
   WP_U32 wpid;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* This mode is added if case this function will be converted to global */
   if (WPI_HANDLE_TYPE(device) != WPI_handle_virtual_device)
      return WPI_ERROR(WP_ERR_DEVICE);

   /**********************/
   /* Multi Class Device */
   /**********************/
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(device))
   {
      if (direction != WP_DIRECTION_TX)
      {
         return WPI_ERROR(WP_ERR_DIRECTION);
      }
      status = WPI_EnetMultiClassDeviceEnable(device, direction);
      return status;
   }

   /*************************/
   /* Other virtual Devices */
   /*************************/
   wpid = WPI_HANDLE_WPID(device);
   dev_entry = WPI_RES_ENTRY(wpid, WPI_HANDLE_INDEX(device));

   if (!WPI_INITIALIZED(dev_entry->state))
      return WPI_ERROR(WP_ERR_DEVICE);

   if (!(dev_entry->state & WPI_STATE_ACTIVE))
   {
      switch (WPI_HANDLE_SUBTYPE(device))
      {
         case WP_DEVICE_MC_HDLC:
         case WP_DEVICE_MC_FR:
            status = WPI_MchDeviceEnable(wpid, WPI_RES_DEV_MCH(dev_entry),
                                         direction);
            break;
         case WP_DEVICE_MFR_FR:
            status = WPI_MfrDeviceEnable(dev_entry, direction);
            break;
         default:
            status = WPI_ERROR(WP_ERR_DEVICE);
      }

      if (status == WP_OK)
      {
         /* Convert state of this virtual device to ACTIVE */
         dev_entry->state |= WPI_STATE_ACTIVE;
      }
   }
   else
      status = WPI_ERROR(WP_ERR_DEVICE_ALREADY_ACTIVE);

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_VirtualDeviceDisable
 *
 * Purpose:    User called function to disable a WinPath virtual device
 *
 * Description:
 *    This functions makes the virtual device not active for data processing.
 *    It checks the type of this virtual device and calls the device
 *    specific routine that disables the device.
 *
 * Inputs:
 *     device         Handle of the device to be disabled
 *     direction      Specifies whether device is to be disabled the
 *                    Rx traffic, Tx traffic, or both
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully, an error code otherwise.
 *
 * Called by:
 *     WP_DeviceDisable, WP_G999StreamDisable
 *
 ****************************************************************************/
static WP_status WPI_VirtualDeviceDisable(WP_handle device,
                                          WP_U32 direction)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_entry *dev_entry;
   WP_U32 wpid;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* This mode is added if case this function will be converted to global */
   if (WPI_HANDLE_TYPE(device) != WPI_handle_virtual_device)
      return WPI_ERROR(WP_ERR_DEVICE);

   /**********************/
   /* Multi Class Device */
   /**********************/
   if (WPI_HANDLE_SUBTYPE(device) == WP_DEVICE_ENET_MULTI_CLASS)
   {
      if (direction != WP_DIRECTION_TX)
      {
         return WPI_ERROR(WP_ERR_DIRECTION);
      }
      status = WPI_EnetMultiClassDeviceDisable(device, direction);
      return status;
   }

   /*************************/
   /* Other virtual Devices */
   /*************************/
   wpid = WPI_HANDLE_WPID(device);
   dev_entry = WPI_RES_ENTRY(wpid, WPI_HANDLE_INDEX(device));
   if (!WPI_INITIALIZED(dev_entry->state))
      return WPI_ERROR(WP_ERR_DEVICE);

   if (dev_entry->state & WPI_STATE_ACTIVE)
   {
      switch (WPI_HANDLE_SUBTYPE(device))
      {
         case WP_DEVICE_MC_HDLC:
         case WP_DEVICE_MC_FR:
            status = WPI_MchDeviceDisable(wpid, WPI_RES_DEV_MCH(dev_entry),
                                          direction);
            break;
         case WP_DEVICE_MFR_FR:
            status = WPI_MfrDeviceDisable(dev_entry, direction);
            break;
          default:
            status = WPI_ERROR(WP_ERR_DEVICE);
      }

      if (status == WP_OK)
      {
         /* Convert state of this virtual device to NOT ACTIVE */
         dev_entry->state &= ~WPI_STATE_ACTIVE;
      }
   }
   else
      status = WPI_ERROR(WP_ERR_DEVICE_ALREADY_DISABLED);

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_VirtualDeviceDelete
 *
 * Purpose:    Internal function to delete a WinPath virtual device
 *
 * Description:
 *    This functions checks that the device is disable and calls the device
 *    specific routine that removes or clears all resources allocated by this
 *    device.
 *
 * Inputs:
 *     device         Handle of the device to be disabled
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully, an error code otherwise.
 *
 * Called by:
 *     WP_DeviceDelete
 *
 ****************************************************************************/
static WP_status WPI_VirtualDeviceDelete(WP_handle device)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_entry *dev_entry;
   wpi_res_device *dev_res;
   WP_U32 wpid;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* This mode is added if case this function will be converted to global */
   if (WPI_HANDLE_TYPE(device) != WPI_handle_virtual_device)
      return WPI_ERROR(WP_ERR_DEVICE);

   wpid = WPI_HANDLE_WPID(device);

   /**********************/
   /* Multi Class Device */
   /**********************/
   if (WPI_HANDLE_SUBTYPE(device) == WP_DEVICE_ENET_MULTI_CLASS)
   {
      status = WPI_EnetMultiClassDeviceDelete(device);
      return status;
   }

   /*************************/
   /* Other virtual Devices */
   /*************************/

   dev_entry = WPI_RES_ENTRY(wpid, WPI_HANDLE_INDEX(device));
   if (!WPI_INITIALIZED(dev_entry->state))
      return WPI_ERROR(WP_ERR_DEVICE);

   dev_res = (wpi_res_device *) dev_entry->resource;
   if (dev_res->ch_counter > 0)
      return WPI_ERROR(WP_ERR_CHANNEL_NOT_DELETED);

   if ((dev_entry->state & WPI_STATE_ACTIVE) == 0)
   {
      /* the device is disabled */
      switch (WPI_HANDLE_SUBTYPE(device))
      {
         case WP_DEVICE_MC_HDLC:
         case WP_DEVICE_MC_FR:
            status = WPI_MchDeviceDelete(wpid, dev_entry,
                                         WPI_RES_DEV_MCH(dev_entry));
            break;
         case WP_DEVICE_MFR_FR:
            status = WPI_MfrDeviceDelete(dev_entry);
            break;
         default:
            status = WPI_ERROR(WP_ERR_DEVICE);
            break;
      }
   }
   else
      status = WPI_ERROR(WP_ERR_DEVICE_NOT_DISABLED);

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_VirtualDeviceModify
 *
 * Purpose:    Internal function to modify a WinPath virtual device
 *
 * Description:
 *    This functions checks that the device is disable and modifies parameters
 *    specified in the command word for the specific virtual device.
 *
 * Inputs:
 *     device         Handle of the device to be disabled
 *     command        Modification command word
 *     config         User-defined configuration parameters
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK is function completes successfully, an error code otherwise.
 *
 * Called by:
 *     WP_DeviceDelete
 *
 ****************************************************************************/
static WP_status WPI_VirtualDeviceModify(WP_handle device,
                                         WP_U32 command,
                                         void *config)
{
   WP_status status;

   WPI_INTERNAL_FUNCTION();

   if (WPI_REGISTRY_USE() == WP_FALSE)
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);

   /* This mode is added if case this function will be converted to global */
   if (WPI_HANDLE_TYPE(device) != WPI_handle_virtual_device)
      return WPI_ERROR(WP_ERR_DEVICE);

   /* the device is disabled */
   switch (WPI_HANDLE_SUBTYPE(device))
   {
   case WP_DEVICE_MC_HDLC:
   case WP_DEVICE_MC_FR:
      status = WPI_MchDeviceModify(device, command, config);
      break;
   case WP_DEVICE_MFR_FR:
      status = WPI_MfrDeviceModify(device, command, config);
      break;
   case WP_DEVICE_ENET_MULTI_CLASS:
   case WP_DEVICE_ENET_MULTI_CLASS_INTSW:
      status = WPI_EnetMultiClassDeviceModify(device, command, config);
      break;
   default:
      status = WPI_ERROR(WP_ERR_DEVICE);
   }

   return status;
}

/************************************************************************
 *
 * Function:  WP_DeviceModify
 *
 * Purpose: Modifies a device's parameters
 *
 * Description: This function modifies parameters for a device already
 *    created.  The device must be disabled.
 *    Currently, the function does not change device parameters that would
 *    require pre-allocated resources (TBD)
 *
 * Inputs:
 *        device   device handle
 *        command  command field
 *        config   configuration structure
 *
 * Outputs:
 *    None - Internal registry and hw registers modified
 *
 * Return Value:
 *    Returns WP_OK if successful, or the following error code:
 *        WP_ERR_INCORRECT_REGISTRY_STATE
 *        WP_ERR_DEVICE
 *        WP_ERR_WDDI
 *
 * Re-entrancy:
 *  This function is responsible for freeing the locks on both the device
 *  and port registry entries.
 *
 **************************************************************************/
WP_status WP_DeviceModify (WP_handle device, WP_U32 command,
                           void *config)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *device_entry;

   WP_U32 dev_index;
   WP_U32 wpid;
   wpi_res_device *dev_res;

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_HANDLE_TYPE(device) == WPI_handle_virtual_device)
   {
      status = WPI_VirtualDeviceModify(device, command, config);
      WPI_WDDI_DETACH();
      return status;
   }

   /* IMA Group Device modification */
   if (WPI_HANDLE_TYPE(device) == WPI_handle_ima)
   {
      status = WPI_ImaGroupDeviceModify(device, command,
               (WP_ima_group *)config);
      WPI_WDDI_DETACH();
      return status;
   }

   if (WPI_HANDLE_TYPE(device) != WPI_handle_device)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_emphy_trans)
   {
      status = WPI_EmphyTransDeviceModify(device, command, config);
      WPI_WDDI_DETACH();
      return status;
   }

   dev_index = WPI_HANDLE_INDEX(device);
   wpid = WPI_HANDLE_WPID(device);
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   /* dev_index passed in here is the index of the device entry in the
      registry */
   WPI_ACCESS_RES(device_entry, resources, dev_index);

   if (device_entry == NULL)
      WPI_BREAK(status, WP_DeviceModifyEnd, WP_ERR_WDDI);

   /* Check that device was initialized. */
   if (!WPI_INITIALIZED(device_entry->state))
      WPI_BREAK(status, WP_DeviceModifyEnd, WP_ERR_DEVICE);

   dev_res = WPI_RES_DEV(device_entry);

   if (WPI_RES_EMY_ENABLE_V(dev_res->emphy_param))
   {
      status = WPI_EmphyDeviceModify(device_entry,command,dev_res->devtype,config );
      WPI_BREAK_NOT_OK(status, WP_DeviceModifyEnd);
   }
   dev_res->device_handle = device;
   /* run the modify functions according to the function pointer */
   /* WPI_EnetDeviceModify */
   if ((dev_res->functions) && (dev_res->functions->device_modify))
      status = dev_res->functions->device_modify(wpid, resources, device_entry,
                                                 dev_index, command, config);

   WP_DeviceModifyEnd:
   WPI_WDDI_DETACH();
   return status;
}

WP_handle WP_DeviceAlloc(WP_handle port,
                         WP_U32 phy,
                         WP_device devtype,
                         void *config)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *device_entry, *port_entry;
   wpi_res_upi *res_upi = NULL;
   wpi_tpt *res_tpt;
   WP_handle handle = 0;
   WP_U32 dev_index;
   WP_U32 port_type, port_index;
   WP_U32 wpid = WPI_HANDLE_WPID( port);
   WP_U16 max_upi_phy;

   WPI_WDDI_ATTACH();
   /* If virtual device of Multi Channel HDLC/FR types should be created
    * call the specific internal WPI_VirtualDeviceCreate routine and return.
    * No need to continue with the rest of the code.
    */
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (devtype == WP_DEVICE_MC_HDLC || devtype == WP_DEVICE_MC_FR ||
       WPI_HANDLE_MFR_P(port))
   {
      WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_NOT_IMPLEMENTED);
   }

   /* Check if handle is a port handle */
   if ((WPI_HANDLE_TYPE(port) != WPI_handle_port_alloc) &&
       (WPI_HANDLE_TYPE(port) != WPI_handle_port))
      WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_HANDLE);

   port_type = WPI_HANDLE_SUBTYPE(port);

   wpid = WPI_HANDLE_WPID(port);
   resources = &wpi_registry_ptr->pwp_resources[wpid];
   res_tpt = WPI_RegistryTpt(wpid);

   port_index = WPI_HANDLE_INDEX(port);

   if (port_type == WPI_PORT_IW_HOST)
   {
      /* no need for prealllocation for iw_host */
      WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_HANDLE);
   }

   /* Other device specific configuration errors will be caught later */
   if (config == NULL)
      WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_CONFIG);

   /* Obtain port entry from registry.  */
   WPI_ACCESS_RES(port_entry, resources, port_index);

   if (port_entry == NULL)
   {
      WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_WDDI);
   }
   else if (WPI_PORT_STATE(port_entry) != WPI_STATE_INITIALIZED)
   {
      /* return as cannot create device without port */
      WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_PORT);
   }

   /* Validate node type depending on the type of port passed in.
      i.e we can't create hdlc device on a upi port
      If OK, set the variable dev_index to the registry index value of the
      device that will be created.
    */
   switch (port_type)
   {
      case WPI_PORT_UPI:
         if (!WPI_CHECK_UPI_DEVICE(devtype))
            WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_DEVICE);
         res_upi = WPI_RES_UPI(port_entry);
         /* If mutli clav was selected in the port,
          * check that the clav marcro was used
          * Note that we check only the tx_mclav value since the tx and rx
          * values should be consistent
          * (this is checked in the wp_portcreate()  */
         if (res_upi->hw.tx_mclav == WPI_UPI_HW_MCLAV_ENABLE)
         {
            WPI_DETACH_RETURN_IF_ERROR(status, WPI_HwUpiMclavPhyGet(&phy));
         }
         else if(!res_upi->uses_emphy)
         {
            if (phy >> WP_CLAV_OFFSET != 0)
            {
               WPI_WDDI_DETACH();
               return WPI_ERROR(WP_ERR_PHY);
            }
         }

         res_upi = WPI_RES_UPI(port_entry);

         switch (devtype)
         {
               case WP_DEVICE_ATM:
               case WP_DEVICE_HDLC:
               case WP_DEVICE_HSPOS:
                  WPI_HwUpiMaxPhyNumGet(&max_upi_phy);
                  if(phy >= max_upi_phy)
                     WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_PHY);
                  dev_index = WPI_INDEX_UPI_PHY(port_index, phy);
                  break;
               case WP_DEVICE_IMA:
               case WP_DEVICE_GBOND:
               case WP_DEVICE_FR:
                  WPI_DETACH_RETURN_IF_ERROR(status,
                                             WPI_Limits( wpid, WPI_LIMITS_UPI_PHY, &max_upi_phy));
                  if(phy >= max_upi_phy - 1)/* either 63 or 127 is reserved */
                     WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_PHY);
                  dev_index = WPI_INDEX_UPI_PHY(port_index, phy);
                  break;
               case WP_DEVICE_GENET:
                  WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_NOT_SUPPORTED_ANYMORE);
               default:
                  WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_DEVICE);
         }
         break;

      case WPI_PORT_TDI:
      {
         if (!WPI_CHECK_TDI_DEVICE(devtype))
            WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_DEVICE);
         switch (devtype)
         {
            case WP_DEVICE_IMA:
            case WP_DEVICE_ATM:
               dev_index = WPI_INDEX_TDI_ATM(port_index);
               break;
            case WP_DEVICE_HDLC:
            case WP_DEVICE_FR:
               dev_index = WPI_INDEX_TDI_HDLC(port_index);
               break;
            case WP_DEVICE_TRANS:
               dev_index = WPI_INDEX_TDI_TRANS(port_index);
               break;
            default:
               WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_DEVICE);
         }
         break;
      }
      case WPI_PORT_ENET:
      {
         WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_UNIMPLEMENTED);
      }
      default:
      {
         /* Unrecognized port type */
         WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_PORT);
      }
   }

   /* Call port specific routine to write values into device structure
      in the registry.  Each of these routines will access the port
      specific information in the registry to create the device. First
      check if port is initialized or not.  */

   /* dev_index passed in here is the index of the device entry in the
      registry */
   WPI_ACCESS_RES(device_entry, resources, dev_index);

   if (device_entry == NULL)
   {
      /* Means we were not able to obtain the lock on this device entry */
      WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_INUSE);
   }

   /*************************************************************
    *
    * Permit calling of specific function based on port type and
    * device type.  If this switch is used, then the function
    * should completely generate the handle and return.  It is
    * not permitted to continue to the rest of the code.  Right
    * now, this method is used for per-phy ATM/IMA.
    *************************************************************/
   switch (port_type) {
      case WPI_PORT_UPI: {
         wpi_res_upi *res_upi = WPI_RES_UPI(port_entry);

         /* If mutli clav was selected in the port,
          * check that the clav marcro was used
          * Note that we check only the tx_mclav value since the tx
          * and rx values should be consistent
          * (this is checked in the wp_portcreate()
          */
         if (res_upi->hw.tx_mclav == WPI_UPI_HW_MCLAV_ENABLE)
         {
            status =  WPI_HwUpiMclavPhyGet(&phy);
            WPI_BREAK_NOT_OK(status, WP_DeviceAllocErrExit);
         }
         else
         {
            if (phy >> WP_CLAV_OFFSET != 0)
            {
               WPI_WDDI_DETACH();
               return WPI_ERROR(WP_ERR_PHY);
            }
         }

         /* Set the max_phy_index in the device entry */
         if (phy > res_upi->max_phy_index)
            res_upi->max_phy_index = phy;

         switch(res_upi->mode) {
            case WP_UPI_UTOPIA_ATM_IMA:
               handle = WPI_DeviceCreatePerPhyAtmIma(port, phy, devtype, config, WP_FALSE);
               WPI_WDDI_DETACH();
               return handle;
            case WP_UPI_POS_ATM_PACKET:
            case WP_UPI_POS_ATM_PACKET_MIB:
               handle = WPI_HwDeviceCreatePerPhyPosAtm(port, phy, devtype, config, WP_FALSE);
               WPI_WDDI_DETACH();
               return handle;
         }
      }
      break;
   }

   switch (port_type)
   {
      case WPI_PORT_UPI:
      {
         wpi_res_upi *res_upi;

         res_upi = WPI_RES_UPI(port_entry);
         switch (devtype)
         {
            case WP_DEVICE_ATM:
            {
               WP_device_upi_atm *upi_config = (WP_device_upi_atm *) config;
               wpi_res_device_atm_upi *dev_atm_upi;
               wpi_atm *atm;

               atm = WPI_RegistryAtm(wpid);

               /* ATM entry should be initialized */
               if (atm == 0)
                  WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_CONFIG);

               if (WPI_DEVICE_STATE(device_entry) != WPI_STATE_INITIALIZED) {
                  dev_atm_upi =
                     (wpi_res_device_atm_upi *)
                     WPI_WDDI_MALLOC(sizeof(wpi_res_device_atm_upi));
                  if (dev_atm_upi == NULL)
                     WPI_BREAK(handle, WP_DeviceAllocErrExit,
                               WP_ERR_MEMORY_ALLOCATION);
                  /* clear the table */
                  memset(dev_atm_upi, 0, sizeof(wpi_res_device_atm_upi));
                  device_entry->resource = dev_atm_upi;
                  dev_atm_upi->atm.device.next = NULL;
               }
               else
                  dev_atm_upi = (wpi_res_device_atm_upi *)
                     device_entry->resource;
               handle = WPI_DeviceAllocUpiAtm(port, phy,
                                              upi_config, dev_atm_upi,
                                              res_upi);

               /* Allocate TPT */
               WPI_TptAllocateUpi(res_tpt, port_index - WPI_upi_base, phy);
               break;
            }

            default:
               WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_UNIMPLEMENTED);
         }
         break;
      }

      case WPI_PORT_TDI:
      {
         switch (devtype)
         {
            case WP_DEVICE_IMA:
            case WP_DEVICE_ATM:
            {
               wpi_res_device_atm_tdi *dev_atm_tdi;
               WP_device_tdm_atm *tdm_config = (WP_device_tdm_atm *) config;
               wpi_atm *atm;

               atm = WPI_RegistryAtm(wpid);

               /* ATM entry should be initialized */
               if (atm == 0)
                  WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_CONFIG);

               if (!WPI_INITIALIZED(device_entry->state)) {
                  dev_atm_tdi =
                     (wpi_res_device_atm_tdi *)
                     WPI_WDDI_MALLOC(sizeof(wpi_res_device_atm_tdi));
                  if (dev_atm_tdi == NULL)
                     WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_WDDI);
                  memset (dev_atm_tdi, 0, sizeof(wpi_res_device_atm_tdi));
                  dev_atm_tdi->next = NULL;
                  device_entry->resource = dev_atm_tdi;
               }
               else
                  dev_atm_tdi = (wpi_res_device_atm_tdi *)
                     device_entry->resource;

               /* Create either an ATM device or an IMA device */
               handle = WPI_DeviceAllocTdiAtm(wpid, port, tdm_config,
                                              dev_atm_tdi,
                                              WPI_RES_TDI(port_entry),
                                              devtype);

               if (devtype == WP_DEVICE_ATM)
               {
                  /* Allocate TPT */
                  WPI_TptAllocateTdi(res_tpt, WP_DEVICE_ATM, port_index - WPI_tdi_base);
               }
               break;
            }
            case WP_DEVICE_HDLC:
            case WP_DEVICE_FR:
            {
               handle = WPI_DeviceAllocTdiHdlc(wpid, port, devtype,
                                               config, dev_index,
                                               WPI_RES_TDI(port_entry),
                                               device_entry);

               /* Allocate TPT */
               WPI_TptAllocateTdi(res_tpt, WP_DEVICE_HDLC, port_index - WPI_tdi_base);
               break;
            }
            case WP_DEVICE_TRANS:
            {
               WP_device_tdm_trans *trans_config =
                  (WP_device_tdm_trans *)config;
               handle = WPI_DeviceAllocTdiTrans(wpid, port, trans_config,
                                                dev_index, device_entry,
                                                WPI_RES_TDI(port_entry));
               break;
            }
            default:
               handle = WPI_ERROR(WP_ERR_DEVICE);
         }
         break;
      }
      case WPI_PORT_ENET:
      {
         WPI_BREAK(handle, WP_DeviceAllocErrExit, WP_ERR_UNIMPLEMENTED);
      }
      default:
         handle = WPI_ERROR(WP_ERR_PORT);
   }

   if (!WPI_INITIALIZED(device_entry->state))
      device_entry->state = WPI_STATE_INITIALIZED;

WP_DeviceAllocErrExit:
   WPI_WDDI_DETACH();

   return handle;
}

/************************************************************************
 *
 * Function:  WP_DeviceFlush
 *
 * Purpose: Flushes transmit queue of a previously disabled device
 *
 * Description: This function flushes traffic descriptors (or in the
 *    case of HSPI, buffers) from the transmit queues of a device
 *    of which the transmit portion is disabled or in an inoperative
 *    state.
 *
 * Inputs:
 *        device   device handle
 *
 * Outputs:
 *    None - Transmit queues are flushed
 *
 * Return Value:
 *    Returns WP_OK if successful, or the following error code:
 *        WP_ERR_INCORRECT_REGISTRY_STATE
 *        WP_ERR_HANDLE
 *        WP_ERR_DEVICE
 *
 * Re-entrancy:
 *  This function is responsible for freeing the locks on both the device
 *  and port registry entries.
 *
 **************************************************************************/
WP_status WP_DeviceFlush(WP_handle device)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   WP_U32 wpid;
   WP_U32 pg, port, port_index, dev_index;
   WP_U32 hwq_num;
   WP_U32 swq_num = 0;
   wpi_tpt *tpt;

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   /* Check if device handle is valid */
   if ((!WPI_HANDLE_DEVICE_P(device)) &&
       (!WPI_HANDLE_VIRTUAL_DEVICE_P(device)) &&
       (!WPI_HANDLE_IMA_P(device)) &&
       (!WPI_HANDLE_GBOND_GROUP_P(device)))
      WPI_BREAK(status, WP_DeviceFlushEnd, WP_ERR_HANDLE);

   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_emphy_trans)
      WPI_BREAK(status, WP_DeviceFlushEnd, WP_ERR_DEVICE);

   wpid = WPI_HANDLE_WPID(device);
   resources = &wpi_registry_ptr->pwp_resources[wpid];
   tpt = WPI_RegistryTpt(wpid);

   dev_index = WPI_HANDLE_INDEX(device);
   dev_entry = &resources->entry[WPI_HANDLE_INDEX(device)];

   if (!WPI_INITIALIZED(dev_entry->state))
      WPI_BREAK(status, WP_DeviceFlushEnd, WP_ERR_DEVICE);

   res_dev = WPI_RES_DEV(dev_entry);

   if (!WPI_HANDLE_VIRTUAL_DEVICE_P(device))
   {
      pg = WPI_HANDLE_SUBTYPE(device);
      switch (pg)
      {
         /* ATM devices */
         case WPI_pg_atm_upi1:
         case WPI_pg_atm_upi2:
         case WPI_pg_atm_upi3:
         case WPI_pg_atm_upi4:
         case WPI_pg_atm_tdi:
         {
            wpi_res_device_atm *res_dev_atm;

            res_dev_atm = (wpi_res_device_atm *) res_dev;

            /* The device.port may be a port handle or an IMA device handle */
            port = res_dev_atm->device.port;
            port_index = WPI_HANDLE_INDEX(port);
            hwq_num = WPI_UPI_FIFO_START +
               ((port_index - WPI_upi_base) * WPI_UPI_FIFO_N) +
               (res_dev->phy_number * WPI_UPI_PHY_FIFO_N);
            swq_num = WPI_TptIndexAtm(wpid, tpt,
                                      port_index,
                                      res_dev->phy_number,
                                      res_dev->emphy_param,
                                      res_dev->emphy_virtual_phy);

            /* Flush queues */
            if (res_dev_atm->hierarchical_shaping
                != WP_ATM_DEV_SHAPING_HIERARCHICAL)
            {
               if (res_dev->tqmode == WP_TQ_FAST_MODE)
               {
                  status = WPI_HwUpiFifoFlush(wpid, res_dev, hwq_num, pg);
                  WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
               }
               else
                  WPI_AsuSwTqFlush(wpid, res_dev_atm,
                                   swq_num, WP_FALSE, 0);
            }
            else
            {
               WPI_HierTqFlush(wpid, res_dev_atm, swq_num);
            }
            break;
         }
         case WPI_pg_pkt_upi1:
         case WPI_pg_pkt_upi2:
         case WPI_pg_pkt_upi3:
         case WPI_pg_pkt_upi4:
         case WPI_pg_pkt_tdi:
         {
            /* Get HW queue and SW queue numbers */
            if ((res_dev->devtype == WP_DEVICE_MLPPP) ||
                (WPI_RES_EMY_ENABLE_V(res_dev->emphy_param)))
               port_index = dev_index;
            else
               port_index = WPI_HANDLE_INDEX(res_dev->port);
            hwq_num = WPI_UPI_FIFO_START +
               ((port_index - WPI_upi_base) * WPI_UPI_FIFO_N) +
               (res_dev->phy_number * WPI_UPI_PHY_FIFO_N);
            swq_num = WPI_TptIndexPkt(wpid, tpt,
                                      port_index,
                                      res_dev->phy_number,
                                      res_dev->emphy_virtual_phy);

            /* Flush queues */
            if (res_dev->tqmode == WP_TQ_FAST_MODE)
            {
               status = WPI_HwUpiFifoFlush(wpid, res_dev, hwq_num, pg);
               WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
            }
            else
            {
               switch (res_dev->devtype)
               {
                  case WP_DEVICE_ATM:
                  case WP_DEVICE_GBOND:
                  {
                     wpi_res_device_atm *dev_atm = (wpi_res_device_atm *) res_dev;
                     WPI_PsuSwTqFlush(wpid, swq_num, dev_atm->tx_rate_limit);
                     status = WPI_PktGftdFlush(wpid, resources, pg, res_dev);
                     WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
                     break;
                  }
                  case WP_DEVICE_HDLC:
                  {
                     wpi_res_device_pkt_hdlc *dev_hdlc = (wpi_res_device_pkt_hdlc *) res_dev;
#if _WP_MODULE_EFM_BONDING_ENABLE_
                     if (dev_hdlc->efm_bonding_enable)
                     {
                        if ((res_dev->functions) && (res_dev->functions->device_flush))
                           status =
                              res_dev->functions->device_flush(dev_hdlc->efm_bonding_link_handle);
                        else
                           status = WPI_ERROR(WP_ERR_WDDI);
                     }
                     else
#endif
                     {
                        WPI_PsuSwTqFlush(wpid, swq_num, dev_hdlc->tx_rate_limit);
                        status = WPI_PktGftdFlush(wpid, resources, pg, res_dev);
                        WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
                     }
                     break;
                  }
                  default:
                     WPI_PsuSwTqFlush(wpid, swq_num, 0);
                     status = WPI_PktGftdFlush(wpid, resources, pg, res_dev);
                     WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
               }
            }
            break;
         }
         case WPI_pg_pkt_enet1:
         case WPI_pg_pkt_enet2:
         case WPI_pg_pkt_enet3:
         case WPI_pg_pkt_enet4:
         {
            wpi_res_device_hspi_enet *hs_enet_device = WPI_RES_DEV_HSPI_ENET(dev_entry);
            wpi_hspi_global *res_hspi = WPI_RegistryHspiGlobal(wpid);

            /* check if HS Enet device */
            if (res_hspi->hs_enet_used == WP_TRUE)
            {
               if (hs_enet_device->HsEnetDev == WPI_USE_HS_ENET)
               {
                  status = WPI_HspiBufferFlush(wpid, hs_enet_device->hspi.tx_fifo_start,
                                               WPI_ENET_FIFO_N);
                  WPI_BREAK_NOT_OK(status, WP_DeviceFlushEnd);
                  break;
               }
            }
            port_index = WPI_HANDLE_INDEX(res_dev->port);
            status = WPI_HwEnetFifoStartGet(port_index, &hwq_num);
            WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
            status = WPI_PsuHwTqFlush(wpid, hwq_num);
            WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
            break;
         }
         case WPI_pg_hspi_hspos1:
         case WPI_pg_hspi_hspos2:
         case WPI_pg_hspi_hspos3:
         case WPI_pg_hspi_hspos4:
         {
            /* Flush buffers from transmit queues */
            wpi_res_device_hspi *res_dev_hspi;
            wpi_res_upi *res_upi;

            res_dev_hspi = (wpi_res_device_hspi *) res_dev;
            port = res_dev_hspi->pkt.device.port;
            port_index = WPI_HANDLE_INDEX(port);
            res_upi = WPI_RES_UPI(WPI_RES_ENTRY(wpid, port_index));

            status = WPI_HspiBufferFlush(wpid, res_dev_hspi->tx_fifo_start,
                                         res_upi->tx_priority_class_mode);
            WPI_BREAK_NOT_OK(status, WP_DeviceFlushEnd);
            break;
         }
         case WPI_pg_tdi_trans:
         {
            /* No flush required. */
            break;
         }
         default:
            WPI_BREAK(status, WP_DeviceFlushEnd, WP_ERR_DEVICE);
      }
   }
   else
   {
      /* Flush virtual devices */
      pg = WPI_pg_pkt_tdi;
      switch (WPI_HANDLE_SUBTYPE(device))
      {
         case WP_DEVICE_MC_HDLC:
         case WP_DEVICE_MC_FR:
         {
            swq_num = WPI_TptIndexPkt(wpid, tpt,
                                      WPI_mch_dev_base,
                                      res_dev->phy_number,
                                      0);
            WPI_PsuSwTqFlush(wpid, swq_num, 0);
            status = WPI_PktGftdFlush(wpid, resources, pg, res_dev);
            WPI_BREAK_IF_ERROR(status, WP_DeviceFlushEnd);
            break;
         }
         case WP_DEVICE_MFR_FR: /* MFR */
         {
            swq_num = WPI_TptIndexPkt(wpid, tpt,
                                      WPI_mfr,
                                      res_dev->phy_number,
                                      0);
            WPI_PsuSwTqFlush(wpid, swq_num, 0);
            break;
         }
         default:
            WPI_BREAK(status, WP_DeviceFlushEnd, WP_ERR_DEVICE);
      }
   }

   status = WP_OK;

 WP_DeviceFlushEnd:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceDisplay
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/
void WPI_DeviceDisplay(WP_U32 level, WP_handle handle)
{
#if _WP_MODULE_DUMP_ENABLE_
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 wpid, type;
   wpi_resource_block *resources;
   wpi_resource_entry *entry;
   wpi_res_device *res_dev;
   WP_U32 port;

   WP_U32 dev_index, devtype;
   WP_U32 i;
   WP_CHAR name[64];
   WP_device res_device_type;
   WP_boolean multi_class_physical_device = WP_FALSE;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return;
   WPI_UintDisplay (level, "Device handle", handle);

   wpid = WPI_HANDLE_WPID(handle);
   type = WPI_HANDLE_TYPE(handle);
   dev_index = WPI_HANDLE_INDEX(handle);
   devtype = WPI_HANDLE_SUBTYPE(handle);
   if (type != WPI_handle_device)
      return;

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_ACCESS_RES(entry, resources, dev_index);
   res_dev = WPI_RES_DEV(entry);

   if (entry == NULL || !WPI_INITIALIZED(entry->state))
      return;
   res_device_type = res_dev->devtype;
   if (res_device_type == WP_DEVICE_ENET)
   {
      wpi_res_device_hspi_enet *res_dev_enet = NULL;
      res_dev_enet = WPI_RES_DEV_HSPI_ENET(entry);
      if (res_dev_enet->enet_multi_class_used)
      {
         multi_class_physical_device = WP_TRUE;
         if (res_dev_enet->enet_g999_used)
         {
            res_device_type = WP_DEVICE_ENET_G999;
         }
      }
   }
   /* if this flag is set, only FMU information is displayed */
   if (!WPI_DISPLAY_CONFIG(fmu_only))
   {
      if (devtype == WPI_pg_emphy_trans)
      {
         WPI_EmphyTransDeviceDisplay(level, handle);
         return;
      }

      WPI_Dump_enum_WP_DEVICE (level + 1, "Device type", res_device_type);
      port = WPI_HANDLE_INDEX(res_dev->port);
      if (WPI_DISPLAY_CONFIG(raw_dps_structures) == 0) {
         WPI_Dump_enum_WP_PORT_NUMBERS (level + 1, "Port type", port);
         WPI_IntDisplay (level + 1, "Phy number", res_dev->phy_number);
      }

      switch (devtype) {
         case WPI_pg_atm_upi1:
         case WPI_pg_atm_upi2:
         case WPI_pg_atm_upi3:
         case WPI_pg_atm_upi4:
         {
            wpi_res_device_atm_upi *dev;
            wpi_res_upi *res_upi;

            dev = WPI_RES_DEV_ATM_UPI(entry);

            res_upi = (wpi_res_upi *)
               &wpi_registry_ptr->pwp_resources[wpid].entry[port - WP_PORT_UPI1 + WPI_upi_base];
            if (!res_upi)
               break;

            if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
               WPI_Dump_wpi_res_device_atm_upi (level + 1, level + 3,
                                                "Atm Upi res", dev);
            }
            else {
               wpi_res_device_atm *dev_atm;
               WP_U32 n_entries;

               dev_atm = &dev->atm;
               WPI_StringDisplay (level + 1, "Tx configuration", "");
               WPI_DeviceTxDisplay (level + 2, &dev_atm->device);

               if (res_upi->hw.tx_ms_mode == WPI_UPI_HW_SLAVE)
                  WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.tx_last_phy);
               WPI_Dump_enum_WP_ATM_DEVICE_SHAPING_MODES (level + 2,
                                                          "Shaping mode",
                                                          dev_atm->hierarchical_shaping);
               n_entries = dev_atm->hier_shaping_table_n_entries;
               WPI_IntDisplay (level + 2, "Shaping table size",
                               n_entries);
               for (i = 0; i < n_entries; i++) {
                  sprintf (name, "Shaping table %d", i);
                  WPI_IntDisplay (level + 3, name,
                                  dev_atm->hier_shaping_grp_table[i]);
               }

               WPI_StringDisplay (level + 1, "Rx configuration", "");
               WPI_DeviceRxDisplay (level + 2, &dev_atm->device);
               if (res_upi->hw.rx_ms_mode == WPI_UPI_HW_SLAVE)
                  WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.rx_last_phy);
               WPI_AtmVpVcBlockDisplay (level + 2, dev_atm);
               WPI_IntDisplay (level + 2, "RAS timeout", dev_atm->ras_timeout);
            }
         }
         break;

         case WPI_pg_atm_tdi:
         {
            wpi_res_device_atm_tdi *dev;
            wpi_res_tdi *res_tdi;

            dev = WPI_RES_DEV_ATM_TDI(entry);

            res_tdi = WPI_RES_TDI(&wpi_registry_ptr->pwp_resources[wpid].entry[port]);

            if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
               WPI_Dump_wpi_res_device_atm_tdi (level + 1, level + 3,
                                                "Atm Tdi res", dev);
            }
            else {
               wpi_res_device_atm *dev_atm;
               WP_U32 n_entries;
               WP_U32 uni, hash, vp;

               dev_atm = &dev->atm;
               if (res_tdi) {

                  WPI_TmrSlotDisplay (level + 1, res_tdi, WPI_TDI_TMR_PRSL_ATM);
                  WPI_Dump_enum_wp_tdm_scramble(level + 1, "Scramble mode",
                                                res_tdi->hw.atm_scramble);
                  WPI_Dump_enum_wp_tdm_cosetmode(level + 1, "Coset mode",
                                                 res_tdi->hw.atm_coset);
                  WPI_Dump_enum_wp_tdm_idlemode(level + 1, "Idle mode",
                                                res_tdi->hw.atm_idle_discard);
                  WPI_IntDisplay(level + 1, "Alpha", res_tdi->hw.atm_alpha + 1);
                  WPI_IntDisplay(level + 1, "Delta", res_tdi->hw.atm_delta + 1);
               }
               WPI_StringDisplay (level + 1, "Atm extended phy statistics",
                                  wpi_atm_ext_phy_stats_string[
                                     dev_atm->atm_ext_phy_stats]);

               WPI_StringDisplay (level + 1, "Tx configuration", "");
               WPI_DeviceTxDisplay (level + 2, &dev_atm->device);

               WPI_Dump_enum_WP_ATM_DEVICE_SHAPING_MODES (level + 2,
                                                          "Shaping mode",
                                                          dev_atm->hierarchical_shaping);
               n_entries = dev_atm->hier_shaping_table_n_entries;
               WPI_IntDisplay (level + 2, "Shaping table size",
                               n_entries);
               for (i = 0; i < n_entries; i++) {
                  sprintf (name, "Shaping table %d", i);
                  WPI_IntDisplay (level + 3, name,
                                  dev_atm->hier_shaping_grp_table[i]);
               }

               WPI_StringDisplay (level + 1, "Rx configuration", "");
               WPI_DeviceRxDisplay (level + 2, &dev_atm->device);

               uni = (dev_atm->unimode == WPI_ATM_LOOKUP_UNI);
               vp = (dev_atm->vplkp == WPI_ATM_VPLKP_ENABLE);
               hash = (dev_atm->lookupmode == WP_ATM_LOOKUP_HASH);

               WPI_StringDisplay (level + 2, "Lookup mode",
                                  WPI_AtmLookupString(0, uni, hash, vp));
               WPI_AtmVpVcBlockDisplay (level + 2, dev_atm);
               WPI_IntDisplay (level + 2, "RAS timeout", dev_atm->ras_timeout);
            }
         }
         break;
         case WPI_pg_pkt_upi1:
         case WPI_pg_pkt_upi2:
         case WPI_pg_pkt_upi3:
         case WPI_pg_pkt_upi4:
         {
            wpi_res_upi *res_upi;

            res_upi = (wpi_res_upi *)
               &wpi_registry_ptr->pwp_resources[wpid].entry[port - WP_PORT_UPI1 + WPI_upi_base];

            switch(res_device_type) {
               case WP_DEVICE_HDLC:
               {
                  wpi_res_device_pkt_hdlc_upi *dev;
                  dev = WPI_RES_DEV_PKT_HDLC_UPI(entry);

                  if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
                     WPI_Dump_wpi_res_device_pkt_hdlc_upi (level + 1, level + 3,
                                                           "Hdlc Upi res", dev);
                  }
                  else {
                     wpi_res_device_pkt_hdlc *hdlc;
                     wpi_res_device_pkt *pkt;

                     hdlc = &dev->hdlc;
                     pkt = &hdlc->pkt;

                     switch (pkt->flowmode)
                     {
                        case WPI_DEVICE_PKT_FLOWMODE_SINGLE:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_SINGLE");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_MULTI:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_MULTI");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_FAST:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_FAST");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_INVALID:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_INVALID");
                           break;
                        default:
                           break;
                     }

                     WPI_UintDisplay (level + 1, "Address Mask",
                                      hdlc->addr_mask);
                     if (hdlc->addr_mask != 0)
                     {
                        WPI_UintDisplay (level + 2, "Address 1", hdlc->address1);
                        WPI_UintDisplay (level + 2, "Address 2", hdlc->address2);
                     }

                     WPI_StringDisplay (level + 1, "Tx configuration", "");
                     WPI_DeviceTxDisplay (level + 2, &pkt->device);

                     if (res_upi->hw.tx_ms_mode == WPI_UPI_HW_SLAVE)
                        WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.tx_last_phy);
                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->tx_statmode]);
                     WPI_IntDisplay (level + 2, "Max sdu", pkt->tx_maxsdu);

                     WPI_StringDisplay (level + 1, "Rx configuration", "");
                     WPI_DeviceRxDisplay (level + 2, &pkt->device);
                     if (res_upi->hw.rx_ms_mode == WPI_UPI_HW_SLAVE)
                        WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.rx_last_phy);
                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->rx_statmode]);
                  }
               }
               break;

               case WP_DEVICE_FR:
               {
                  wpi_res_device_pkt_fr_upi *dev;
                  dev = WPI_RES_DEV_PKT_FR_UPI(entry);

                  if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
                     WPI_Dump_wpi_res_device_pkt_fr_upi (level + 1, level + 3,
                                                         "Fr Upi res", dev);
                  }
                  else {
                     wpi_res_device_pkt_fr *fr;
                     wpi_res_device_pkt *pkt;

                     fr = &dev->fr;
                     pkt = &fr->pkt;

                     WPI_StringDisplay (level + 1, "Tx configuration", "");
                     WPI_DeviceTxDisplay (level + 2, &pkt->device);

                     if (res_upi->hw.tx_ms_mode == WPI_UPI_HW_SLAVE)
                        WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.tx_last_phy);
                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->tx_statmode]);
                     WPI_IntDisplay (level + 2, "Max sdu", pkt->tx_maxsdu);

                     WPI_StringDisplay (level + 1, "Rx configuration", "");
                     WPI_DeviceRxDisplay (level + 2, &pkt->device);
                     if (res_upi->hw.rx_ms_mode == WPI_UPI_HW_SLAVE)
                        WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.rx_last_phy);
                     WPI_StringDisplay (level + 2, "Interrupts",
                                        wpi_pkt_intmode_string[fr->rx_intmode]);
                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->rx_statmode]);
                     WPI_Dump_enum_WP_fr_opmode (level + 2, "Opmode",
                                                 fr->rx_opmode);
                     WPI_Dump_enum_WP_fr_header (level + 2, "Header",
                                                 fr->rx_fr_header_type);
                     WPI_Dump_enum_WP_fr_lookupmode (level + 2, "Lookup mode",
                                                     fr->rx_lookupmode);
                     if (fr->rx_lookupmode == WP_FR_LOOKUP_TABLE) {
                        WPI_IntDisplay (level + 3, "Number of DLCI",
                                        fr->rx_n_dlci);
                        WPI_IntDisplay (level + 3, "Minimum DLCI",
                                        fr->rx_min_dlci);
                     }
                     WPI_IntDisplay (level + 2, "Max sdu", fr->rx_maxsdu);
                     if (WPI_DISPLAY_CONFIG(raw_handles))
                        WPI_UintDisplay (level + 2, "Queue handle",
                                         fr->rx_qnode);
                     else
                        WPI_QnodeDisplay (level + 2, fr->rx_qnode);
                  }
               }
               break;

               default:
                  break;
            }
         }
         break;

         case WPI_pg_pkt_tdi:
         {
            wpi_res_tdi *res_tdi;
            WP_U32 min_hdlc_flags;
            WP_S32 tdi_flag;

            res_tdi = WPI_RES_TDI(&wpi_registry_ptr->pwp_resources[wpid].entry[port]);

            status = WPI_CapabilityRead(wpid,
                                        WPI_CAP_TDI_HDLC_FLAG_SHARED,
                                        &tdi_flag);
            if (status != WP_OK) {
               /* Assume rev A */
               min_hdlc_flags = 2;
            }
            else {
               if (tdi_flag)
                  min_hdlc_flags = 1;
               else
                  min_hdlc_flags = 2;
            }

            switch (res_device_type) {
               case WP_DEVICE_HDLC:
               {
                  wpi_res_device_pkt_hdlc_tdi *dev;
                  dev = WPI_RES_DEV_PKT_HDLC_TDI(entry);

                  if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
                     WPI_Dump_wpi_res_device_pkt_hdlc_tdi (level + 1, level + 3,
                                                           "Hdlc Tdi res", dev);
                  }
                  else {
                     wpi_res_device_pkt_hdlc *hdlc;
                     wpi_res_device_pkt *pkt;

                     hdlc = &dev->hdlc;
                     pkt = &hdlc->pkt;
                     if (res_tdi) {
#if 0
                        WPI_TmrSlotDisplay (level + 1, res_tdi, WPI_TDI_TMR_PRSL_HDLC);
                        WPI_StringDisplay (level + 1, "Crc mode",
                                           wpi_hdlc_crc_string[hdlc->crc_type]);
                        WPI_IntDisplay (level + 1, "Number of flags", min_hdlc_flags
                                        + res_tdi->hw.hdlc_n_flags);
                        WPI_StringDisplay (level + 1, "Idle mode",
                                           wpi_tdi_idle_string[res_tdi->hw.hdlc_idle_flag]);
#endif
                     }
                     switch (pkt->flowmode)
                     {
                        case WPI_DEVICE_PKT_FLOWMODE_SINGLE:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_SINGLE");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_MULTI:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_MULTI");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_FAST:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_FAST");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL");
                           break;
                        case WPI_DEVICE_PKT_FLOWMODE_INVALID:
                           WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_INVALID");
                           break;
                        default:
                           break;
                     }

                     WPI_UintDisplay (level + 1, "Address Mask",
                                      hdlc->addr_mask);
                     if (hdlc->addr_mask != 0)
                     {
                        WPI_UintDisplay (level + 2, "Address 1", hdlc->address1);
                        WPI_UintDisplay (level + 2, "Address 2", hdlc->address2);
                     }

                     WPI_StringDisplay (level + 1, "Tx configuration", "");

                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->tx_statmode]);
                     WPI_IntDisplay (level + 2, "Max sdu", pkt->tx_maxsdu);

                     WPI_StringDisplay (level + 1, "Rx configuration", "");
                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->rx_statmode]);

                  }
               }
               break;

               case WP_DEVICE_FR:
               {
                  wpi_res_device_pkt_fr_tdi *dev;
                  wpi_res_tdi *res_tdi;

                  res_tdi = WPI_RES_TDI(&wpi_registry_ptr->pwp_resources[wpid].entry[port]);

                  dev = WPI_RES_DEV_PKT_FR_TDI(entry);
                  if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
                     WPI_Dump_wpi_res_device_pkt_fr_tdi (level + 1, level + 3,
                                                         "Fr Tdi res", dev);
                  }
                  else {
                     wpi_res_device_pkt_fr *fr;
                     wpi_res_device_pkt *pkt;

                     fr = &dev->fr;
                     pkt = &fr->pkt;
                     if (res_tdi) {
#if 0
                        WPI_TmrSlotDisplay (level + 1, res_tdi, WPI_TDI_TMR_PRSL_HDLC);

                        WPI_Dump_enum_WP_tdm_fr_opmode (level + 1, "Opmode",
                                                        fr->opmode);
                        WPI_IntDisplay (level + 1, "Number of flags", min_hdlc_flags
                                        + res_tdi->hw.hdlc_n_flags);
                        WPI_StringDisplay (level + 1, "Idle mode",
                                           wpi_tdi_idle_string[res_tdi->hw.hdlc_idle_flag]);
#endif
                     }

                     WPI_StringDisplay (level + 1, "Tx configuration", "");

                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->tx_statmode]);
                     WPI_IntDisplay (level + 2, "Max sdu", pkt->tx_maxsdu);

                     WPI_StringDisplay (level + 1, "Rx configuration", "");
                     WPI_StringDisplay (level + 2, "Interrupts",
                                        wpi_pkt_intmode_string[fr->rx_intmode]);
                     WPI_StringDisplay (level + 2, "Statistics",
                                        wpi_pkt_stat_string[pkt->rx_statmode]);
                     WPI_Dump_enum_WP_fr_opmode (level + 2, "Opmode",
                                                 fr->rx_intmode);
                     WPI_Dump_enum_WP_fr_header (level + 2, "Header",
                                                 fr->rx_fr_header_type);
                     WPI_Dump_enum_WP_fr_lookupmode (level + 2, "Lookup mode",
                                                     fr->rx_lookupmode);
                     if (fr->rx_lookupmode == WP_FR_LOOKUP_TABLE) {
                        WPI_IntDisplay (level + 3, "Number of DLCI",
                                        fr->rx_n_dlci);
                        WPI_IntDisplay (level + 3, "Minimum DLCI",
                                        fr->rx_min_dlci);
                     }
                     WPI_IntDisplay (level + 2, "Max sdu", fr->rx_maxsdu);
                     if (WPI_DISPLAY_CONFIG(raw_handles))
                        WPI_UintDisplay (level + 2, "Queue handle",
                                         fr->rx_qnode);
                     else
                        WPI_QnodeDisplay (level + 2, fr->rx_qnode);
                  }
               }
               break;

               case WP_DEVICE_MLPPP:
               {
                  WPI_MlpppBundleDisplay(level, wpid, dev_index);
               }
               break;

               default:
                  break;
            }
         }
         break;

         case WPI_pg_pkt_enet1:
         case WPI_pg_pkt_enet2:
         case WPI_pg_pkt_enet3:
         case WPI_pg_pkt_enet4:
         {
            wpi_res_device_hspi_enet *dev;

            dev = WPI_RES_DEV_HSPI_ENET(entry);

            if (dev->hspi.pkt.flowmode == WPI_DEVICE_PKT_FLOWMODE_FAST)
            {

               if (WPI_DISPLAY_CONFIG(raw_dps_structures))
               {
                  WPI_Dump_wpi_res_device_hspi_enet (level + 1, level + 3,
                                                     "Pkt fastflow res", dev);
               }
               else
               {
                  sprintf (name, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",
                           dev->mac_addr[0], dev->mac_addr[1],
                           dev->mac_addr[2], dev->mac_addr[3],
                           dev->mac_addr[4], dev->mac_addr[5]);
                  WPI_StringDisplay (level + 1, "MAC station address", name);
                  WPI_StringDisplay (level + 1, "Tx configuration", "");
                  WPI_DeviceTxDisplay (level + 2, &dev->hspi.pkt.device);
                  WPI_StringDisplay (level + 2, "Statistics",
                                     wpi_pkt_stat_string[dev->hspi.pkt.tx_statmode]);
                  WPI_StringDisplay (level + 1, "Rx configuration", "");
                  WPI_DeviceRxDisplay (level + 2, &dev->hspi.pkt.device);
                  WPI_StringDisplay (level + 2, "Statistics",
                                     wpi_pkt_stat_string[dev->hspi.pkt.rx_statmode]);
                  WPI_StringDisplay (level + 2, "Address mode",
                                     wpi_enet_address_mode_string[
                                        dev->hspi.pkt.rx_statmode]);
                  WPI_StringDisplay (level + 2, "Mac lookup mode",
                                     wpi_enet_lookup_mode_string[
                                        dev->maclookup]);
                  WPI_StringDisplay (level + 2, "Broadcast filter",
                                     wpi_enet_filter_string[
                                        dev->bcfilter]);
                  WPI_StringDisplay (level + 2, "Multicast filter",
                                     wpi_enet_filter_string[
                                        dev->mcfilter]);
               }
            }
            else
            {

               if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
                  WPI_Dump_wpi_res_device_hspi_enet (level + 1, level + 3,
                                                     "Pkt multiflow res", dev);
               }
               else {
                  wpi_res_device_pkt *pkt;

                  pkt = &dev->hspi.pkt;
                  sprintf (name, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",
                           dev->mac_addr[0], dev->mac_addr[1],
                           dev->mac_addr[2], dev->mac_addr[3],
                           dev->mac_addr[4], dev->mac_addr[5]);
                  WPI_StringDisplay (level + 1, "MAC station address", name);
                  switch (pkt->flowmode)
                  {
                     case WPI_DEVICE_PKT_FLOWMODE_SINGLE:
                        WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_SINGLE");
                        break;
                     case WPI_DEVICE_PKT_FLOWMODE_MULTI:
                        WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_MULTI");
                        break;
                     case WPI_DEVICE_PKT_FLOWMODE_FAST:
                        WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_FAST");
                        break;
                     case WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL:
                        WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL");
                        break;
                     case WPI_DEVICE_PKT_FLOWMODE_INVALID:
                        WPI_StringDisplay (level + 1, "Flow mode", "WPI_DEVICE_PKT_FLOWMODE_INVALID");
                        break;
                     default:
                        break;
                  }

                  WPI_StringDisplay (level + 1, "Tx configuration", "");
                  WPI_DeviceTxDisplay (level + 2, &pkt->device);

                  WPI_StringDisplay (level + 2, "Statistics",
                                     wpi_pkt_stat_string[pkt->tx_statmode]);
                  WPI_IntDisplay (level + 2, "Max sdu", pkt->tx_maxsdu);

                  WPI_StringDisplay (level + 1, "Rx configuration", "");
                  WPI_DeviceRxDisplay (level + 2, &pkt->device);
                  WPI_StringDisplay (level + 2, "Statistics",
                                     wpi_pkt_stat_string[pkt->rx_statmode]);
                  WPI_StringDisplay (level + 2, "Address mode",
                                     wpi_enet_address_mode_string[
                                        dev->hspi.addressmode]);
                  WPI_StringDisplay (level + 2, "Mac lookup mode",
                                     wpi_enet_lookup_mode_string[
                                        dev->maclookup]);
                  WPI_StringDisplay (level + 2, "Broadcast filter",
                                     wpi_enet_filter_string[
                                        dev->bcfilter]);
                  WPI_StringDisplay (level + 2, "Multicast filter",
                                     wpi_enet_filter_string[
                                        dev->mcfilter]);
               }
            }
         }
         break;

         case WPI_pg_hspi_hspos1:
         case WPI_pg_hspi_hspos2:
         case WPI_pg_hspi_hspos3:
         case WPI_pg_hspi_hspos4:
         {
            wpi_res_device_hspi_pos *dev;
            wpi_port_group *hspi_port_group;
            wpi_dps_hspi_tcpt *tcpt;
            wpi_dps_hspi_rcpt *rcpt;

            wpi_res_upi *res_upi;

            res_upi = (wpi_res_upi *)
               &wpi_registry_ptr->pwp_resources[wpid].entry[port - WP_PORT_UPI1 + WPI_upi_base];

            dev = WPI_RES_DEV_HSPI_POS(entry);
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryPortGroupArrayGet,
                                    WPI_REG_LOCKED,
                                    &hspi_port_group,
                                    wpid,
                                    WPI_pg_hspi_hspos1 + port);
            if (WP_ERROR_P(lock_status)) return;
            tcpt = hspi_port_group->host_tcpt_base_hspi;
            rcpt = hspi_port_group->host_rcpt_base.hspi;

            if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
               WPI_Dump_wpi_res_device_hspi_pos (level + 1, level + 3,
                                                 "Hspi pos res", dev);
               WPI_StringDisplay (level + 1, "Tcpt", "");
               WPI_HspiTcptDisplay (level + 2, tcpt);
               WPI_StringDisplay (level + 1, "Rcpt", "");
               WPI_HspiRcptDisplay (level + 2, rcpt, wpid);
            }
            else {
               wpi_res_device_hspi *hspi;

               hspi = &dev->hspi;

               WPI_StringDisplay (level + 1, "Tx configuration", "");
               WPI_DeviceTxDisplay (level + 2, &hspi->pkt.device);
               WPI_HspiTcptDisplay (level + 2, tcpt);
#if 0
               if (res_upi->hw.tx_ms_mode == WPI_UPI_HW_SLAVE)
                  WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.tx_last_phy);
               WPI_StringDisplay (level + 2, "Queue depth array", "");
#endif
               for (i = 0; i < 8; i++) {
                  sprintf (name, "Queue depth %d", i);
                  WPI_IntDisplay (level + 3, name, hspi->tx_queuedepth[i]);
               }
               WPI_UintDisplay (level + 2, "Tag", hspi->tx_tag);

               WPI_StringDisplay (level + 1, "Rx configuration", "");
               WPI_DeviceRxDisplay (level + 2, &hspi->pkt.device);
               WPI_HspiRcptDisplay (level + 2, rcpt, wpid);
#if 0
               if (res_upi->hw.rx_ms_mode == WPI_UPI_HW_SLAVE)
                  WPI_IntDisplay (level + 2, "Slave address", res_upi->hw.rx_last_phy);
               WPI_IntDisplay (level + 2, "Queue depth",
                               hspi->rx_queuedepth);
               WPI_UintDisplay (level + 2, "Tag", hspi->rx_tag);
#endif
            }
         }
         break;

         case WPI_pg_tdi_trans:
         {
            wpi_resource_entry *tdi_entry = &resources->entry[WPI_HANDLE_INDEX(port)];
            wpi_res_tdi *res_tdi = WPI_RES_TDI(tdi_entry);

            if (res_tdi)
               WPI_DeviceDisplayTdiTrans(wpid, level, res_tdi, WPI_RES_DEV_TRANS(entry));
         }
         break;
      }
      if (WPI_REGISTRY_USE() == WP_TRUE)
      {
         WPI_SaDeviceDisplay(wpid, res_dev, level, multi_class_physical_device);
      }
   }

   /* call the FMU device display */
   switch(res_device_type)
   {
      case WP_DEVICE_ENET:
      case WP_DEVICE_HSPOS:
      case WP_DEVICE_HDLC:
      case WP_DEVICE_ATM:
      {
         if(res_dev->tqmode == WP_TQ_FAST_MODE)
         {
            WPI_FmuHwDisplay(level,wpid,handle);
         }
         break;
      }
      default:
         break;
   }

#endif
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceTxDisplay
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/
void WPI_DeviceTxDisplay (WP_U32 level, wpi_res_device *dev)
{
#if _WP_MODULE_DUMP_ENABLE_
   WPI_UintDisplay (level, "Bit rate", dev->tx_bit_rate);
   WPI_StringDisplay (level, "Transmit queue mode",
                      wpi_tq_mode_string[dev->tqmode==WP_TQ_FAST_MODE]);

   if (dev->n_traffic_class == 0xee)
      return;
#endif
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceRxDisplay
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/
void WPI_DeviceRxDisplay (WP_U32 level, wpi_res_device *dev)
{
#if _WP_MODULE_DUMP_ENABLE_
   WPI_IntDisplay (level, "Bit rate", dev->rx_bit_rate);
   WPI_StringDisplay (level, "Interworking background mode",
                      wpi_iw_background_string[dev->iw_bkgnd]);
#endif
}
/*****************************************************************************
 *
 * Function:   WPI_VirtualDeviceDisplay
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/
void WPI_VirtualDeviceDisplay(WP_U32 level, WP_handle handle)
{
#if _WP_MODULE_DUMP_ENABLE_
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 wpid, type;
   wpi_resource_block *resources;
   wpi_resource_entry *entry = NULL;
   wpi_res_device *res_dev;
   WP_U32 port;

   WP_U32 dev_index, devtype;
   WP_device res_device_type;
   wpi_res_enet_multi_class *res_enet_multi_class = NULL;
   WP_boolean is_virtual_device = WP_FALSE; /* The channel is built on top of a virtual device */

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return;
   WPI_UintDisplay (level, "Device handle", handle);

   wpid = WPI_HANDLE_WPID(handle);
   type = WPI_HANDLE_TYPE(handle);
   dev_index = WPI_HANDLE_INDEX(handle);
   devtype = WPI_HANDLE_SUBTYPE(handle);

   if (type != WPI_handle_virtual_device)
      return;

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(handle))
   {
      WP_status status = WP_OK;
      wpi_res_enet_multi_class_device *enet_multi_class_device = NULL;
      is_virtual_device = WP_TRUE;
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryEnetMultiClassGet,
                              WPI_REG_LOCKED,
                              &res_enet_multi_class,
                              wpid);
      if (WP_ERROR_P(lock_status)) return;
      status = WPI_EnetMultiClassDeviceGetByHandle(res_enet_multi_class,
                                                   handle,
                                                   &enet_multi_class_device);
      if (WP_ERROR_P(status)) return;
      res_dev = (wpi_res_device *)enet_multi_class_device;
   }
   else
   {
      WPI_ACCESS_RES(entry, resources, dev_index);
      res_dev = WPI_RES_DEV(entry);
      if (entry == NULL || !WPI_INITIALIZED(entry->state))
         return;
   }

   res_device_type = res_dev->devtype;
   if (res_device_type == WP_DEVICE_ENET_MULTI_CLASS)
   {
      wpi_res_enet_multi_class_device *res_enet_multi_class_device = NULL;
      res_enet_multi_class_device = (wpi_res_enet_multi_class_device *)res_dev;
      if (WPI_RegistryDeviceEnetG999(res_enet_multi_class_device->virtual_dev_handle))
      {
         /* G999 multi-class device */
         WP_U8 stream_type;
         stream_type = res_enet_multi_class_device->enet_multi_class_g999.stream_type;
         WPI_Dump_enum_WP_G999_STREAM_TYPE(level + 1, "Stream type", stream_type);
      }
      else
      {
         /* Non-G999 multi-class device */
         WPI_Dump_enum_WP_DEVICE (level + 1, "Device type", res_device_type);
      }
   }
   else
   {
      WPI_Dump_enum_WP_DEVICE (level + 1, "Device type", res_device_type);
   }

   port = WPI_HANDLE_INDEX(res_dev->port);
   if (WPI_DISPLAY_CONFIG(raw_dps_structures) == 0)
   {
      WPI_Dump_enum_WP_PORT_NUMBERS (level + 1, "Port type", port);
      WPI_IntDisplay (level + 1, "Phy number", res_dev->phy_number);
   }

   switch (devtype) {

      case WP_DEVICE_MC_HDLC:
      {
         wpi_res_device_mch *dev;
         wpi_res_device_pkt_hdlc *hdlc;
         wpi_res_device_pkt *pkt;

         if (! entry )
         {
            return; // WPI_ERROR(WP_ERR_NULL_PTR);
         }
         dev = (wpi_res_device_mch *) entry->resource;
         hdlc = &dev->type.hdlc.hdlc;
         pkt = &hdlc->pkt;

         if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
            WPI_Dump_wpi_res_device_mch(level + 1, level + 2,
                                        "Mc hdlc", dev);
            WPI_Dump_wpi_res_device_pkt_hdlc(level + 2, level + 3,
                                             "Hdlc", hdlc);
            WPI_Dump_wpi_res_device_pkt (level + 3, level + 4,
                                         "Pkt", pkt);
         }
         else {
            WP_U8 opmode;
            WP_U8 tx_idlemode;

            WPI_IntDisplay (level + 1, "Number of slots",
                            dev->num_slots);
            WPI_IntDisplay (level + 1, "Frame number",
                            dev->frame_number);

            opmode = 255;
            tx_idlemode = 255;
            {
               wpi_resource_entry *trans_entry;
               wpi_res_device_trans *trans;
               wpi_res_device_trans_mch *trans_mch;
               WP_U32 control_bits_1;

               WPI_ACCESS_RES(trans_entry, resources,
                              WPI_HANDLE_INDEX(dev->main_device));

               trans = WPI_RES_DEV_TRANS(trans_entry);
               trans_mch = &trans->type.mch;

               if (trans_mch->n_tx_devices > 0) {
                  WPI_MEM_GET(control_bits_1,
                              dev->host_tsat->control_bits_1);
                  opmode = WPI_MCH_TSAT_C56K_V (control_bits_1);
                  tx_idlemode = WPI_MCH_TSAT_IFM_V (control_bits_1);
               }
               else if (trans_mch->n_rx_devices > 0) {
                  WPI_MEM_GET(control_bits_1,
                              dev->host_rsat->control_bits_1);
                  opmode = WPI_MCH_RSAT_C56K_V (control_bits_1);
               }
            }
            if (opmode == 255)
               WPI_StringDisplay (level + 1, "Opmode", "Unknown");
            else
               WPI_Dump_enum_wp_tdm_mch_opmodes (level + 1, "Opmode",
                                                 opmode);
            WPI_UintDisplay (level + 1, "Address Mask",
                             hdlc->addr_mask);
            if (hdlc->addr_mask != 0)
            {
               WPI_UintDisplay (level + 2, "Address 1", hdlc->address1);
               WPI_UintDisplay (level + 2, "Address 2", hdlc->address2);
            }

            WPI_StringDisplay (level + 1, "Crc type",
                               wpi_hdlc_crc_string[hdlc->crc_type]);

            WPI_StringDisplay (level + 1, "Tx configuration", "");
            if (tx_idlemode == 255)
               WPI_StringDisplay (level + 2, "Idle mode", "Unknown");
            else
               WPI_StringDisplay (level + 2, "Idle mode",
                                  wpi_tdi_mch_idle_string[tx_idlemode]);

            WPI_StringDisplay (level + 2, "Statistics",
                               wpi_pkt_stat_string[pkt->tx_statmode]);
            WPI_IntDisplay (level + 2, "Flow mode", pkt->flowmode);

            WPI_StringDisplay (level + 1, "Rx configuration", "");
            WPI_StringDisplay (level + 2, "Statistics",
                               wpi_pkt_stat_string[pkt->rx_statmode]);
         }
      }
      break;

      case WP_DEVICE_MC_FR:
      {
         wpi_res_device_mch *dev;
         wpi_res_device_pkt_fr *fr;
         wpi_res_device_pkt *pkt;

         if (! entry )
         {
            return; // WPI_ERROR(WP_ERR_NULL_PTR);
         }
         dev = (wpi_res_device_mch *) entry->resource;
         fr = &dev->type.fr.fr;
         pkt = &fr->pkt;

         if (WPI_DISPLAY_CONFIG(raw_dps_structures)) {
            WPI_Dump_wpi_res_device_mch(level + 1, level + 2,
                                        "Mc Fr", dev);
            WPI_Dump_wpi_res_device_pkt_fr(level + 2, level + 3,
                                           "Fr", fr);
            WPI_Dump_wpi_res_device_pkt (level + 3, level + 4,
                                         "Pkt", pkt);
         }
         else {
            WP_U8 opmode;
            WP_U8 tx_idlemode;

            WPI_IntDisplay (level + 1, "Number of slots",
                            dev->num_slots);
            WPI_IntDisplay (level + 1, "Frame number",
                            dev->frame_number);

            opmode = 255;
            tx_idlemode = 255;
            {
               wpi_resource_entry *trans_entry;
               wpi_res_device_trans *trans;
               wpi_res_device_trans_mch *trans_mch;
               WP_U32 control_bits_1;

               WPI_ACCESS_RES(trans_entry, resources,
                              WPI_HANDLE_INDEX(dev->main_device));

               trans = WPI_RES_DEV_TRANS(trans_entry);
               trans_mch = &trans->type.mch;

               if (trans_mch->n_tx_devices > 0) {
                  WPI_MEM_GET(control_bits_1,
                              dev->host_tsat->control_bits_1);
                  opmode = WPI_MCH_TSAT_C56K_V (control_bits_1);
                  tx_idlemode = WPI_MCH_TSAT_IFM_V (control_bits_1);
               }
               else if (trans_mch->n_rx_devices > 0) {
                  WPI_MEM_GET(control_bits_1,
                              dev->host_rsat->control_bits_1);
                  opmode = WPI_MCH_RSAT_C56K_V (control_bits_1);
               }
            }
            if (opmode == 255)
               WPI_StringDisplay (level + 1, "Opmode", "Unknown");
            else
               WPI_Dump_enum_wp_tdm_mch_opmodes (level + 1, "Opmode",
                                                 opmode);

            WPI_StringDisplay (level + 1, "Tx configuration", "");
            if (tx_idlemode == 255)
               WPI_StringDisplay (level + 2, "Idle mode", "Unknown");
            else
               WPI_StringDisplay (level + 2, "Idle mode",
                                  wpi_tdi_mch_idle_string[tx_idlemode]);

            WPI_StringDisplay (level + 2, "Statistics",
                               wpi_pkt_stat_string[pkt->tx_statmode]);

            WPI_StringDisplay (level + 1, "Rx configuration", "");
            WPI_StringDisplay (level + 2, "Interrupts",
                               wpi_pkt_intmode_string[fr->rx_intmode]);
            WPI_StringDisplay (level + 2, "Statistics",
                               wpi_pkt_stat_string[pkt->rx_statmode]);
            WPI_Dump_enum_WP_fr_opmode (level + 2, "Opmode",
                                        fr->rx_intmode);
            WPI_Dump_enum_WP_fr_header (level + 2, "Header",
                                        fr->rx_fr_header_type);
            WPI_Dump_enum_WP_fr_lookupmode (level + 2, "Lookup mode",
                                            fr->rx_lookupmode);
            if (fr->rx_lookupmode == WP_FR_LOOKUP_TABLE) {
               WPI_IntDisplay (level + 3, "Number of DLCI",
                               fr->rx_n_dlci);
               WPI_IntDisplay (level + 3, "Minimum DLCI",
                               fr->rx_min_dlci);
            }
            WPI_IntDisplay (level + 2, "Max sdu", fr->rx_maxsdu);
            if (WPI_DISPLAY_CONFIG(raw_handles))
               WPI_UintDisplay (level + 2, "Queue handle",
                                fr->rx_qnode);
            else
               WPI_QnodeDisplay (level + 2, fr->rx_qnode);
         }
      }
      break;
      case WP_DEVICE_MFR_FR:
         if (! entry )
         {
            return; // WPI_ERROR(WP_ERR_NULL_PTR);
         }
         WPI_MfrDeviceDisplay(level, entry);
         break;
      case WP_DEVICE_ENET_MULTI_CLASS:
      case WP_DEVICE_ENET_MULTI_CLASS_INTSW:
      {
         WPI_EnetMultiClassDeviceDisplay(level, res_dev);
         break;
      }
      default:
         break;
   }
   if (is_virtual_device)
   {
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                  WPI_RegistryEnetMultiClassRelease,
                                  WPI_REG_UNLOCKED,
                                  &res_enet_multi_class);
      if (WP_ERROR_P(lock_status)) return;
   }
#endif
}
/*****************************************************************************
 *
 * Function:   WPI_DeviceAllocDisplay
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/
void WPI_DeviceAllocDisplay(WP_U32 level, WP_handle handle)
{
   WPI_INTERNAL_FUNCTION();
}

/*****************************************************************************
 *
 * Function:   WPI_VirtualDevicesDisplay
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/
void WPI_VirtualDevicesDisplay(WP_U32 level, WP_U32 wpid)
{
   WPI_INTERNAL_FUNCTION();
}

/*****************************************************************************
 *
 * Function:   WPI_DevicesDisplay
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/

void WPI_DevicesDisplay(WP_U32 level, WP_U32 wpid)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *entry;
   WP_U32 i, n_initialized, n_allocated;
   WP_U16 emphy_trans_allocated;
   WP_handle handle;
   wpi_res_device *dev;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return;
   resources = &wpi_registry_ptr->pwp_resources[wpid];
   n_allocated = WPI_last_device - WPI_first_device + 1;
   emphy_trans_allocated = WPI_EmphyTransDevices(wpid);
   if (WPI_DISPLAY_CONFIG(raw_dps_structures))
      WPI_IntDisplay (level, "Number of allocated devices",
                      (n_allocated + emphy_trans_allocated));
   n_initialized = 0;
   for (i = 0; i < n_allocated; i++) {
      entry = &resources->entry[WPI_first_device + i];
      if (WPI_INITIALIZED (entry->state))
         n_initialized++;
   }
   n_initialized += WPI_EmphyTransDevicesInitialized(wpid);
   WPI_NintDisplay (level, "Number of initialized devices", n_initialized);
   for (i = 0; i < n_allocated; i++) {
      entry = &resources->entry[WPI_first_device + i];

      if (!WPI_DEVICE_CREATED(entry->state))
         continue;

      dev = entry->resource;
      if (dev->devtype == WP_DEVICE_MC_FR ||
          dev->devtype == WP_DEVICE_MC_HDLC ||
          dev->devtype == WP_DEVICE_MFR_FR)
      {
         handle = WPI_MAKE_HANDLE (WPI_handle_virtual_device,
                                   dev->devtype,
                                   wpid,
                                   i + WPI_first_device);
         if (WPI_DISPLAY_CONFIG(raw_handles))
            WPI_UintDisplay (level + 1, "Device handle", handle);
         else
            WPI_VirtualDeviceDisplay (level + 1, handle);
      }
      else
      {

         handle = WPI_MAKE_HANDLE (WPI_handle_device,
                                   WPI_DeviceHandleSubtype (dev->devtype,
                                                            dev->port),
                                   wpid, i + WPI_first_device);
         if (WPI_DISPLAY_CONFIG(raw_handles))
            WPI_UintDisplay (level + 1, "Device handle", handle);
         else
            WPI_DeviceDisplay (level + 1, handle);
         if (dev->devtype == WP_DEVICE_ENET)
         {
            wpi_res_device_hspi_enet *res_dev_enet = NULL;
            res_dev_enet = WPI_RES_DEV_HSPI_ENET(entry);
            if (res_dev_enet->enet_multi_class_used)
            {
               // erankd - need to add case for WP_DEVICE_ENET_MULTI_CLASS
               WP_handle multi_class_port;
               WP_U32 port_index;
               WP_S32 ii;
               wpi_res_enet_multi_class *res_enet_multi_class = NULL;
               wpi_res_enet_multi_class_port *port_entry = NULL;
               wpi_res_enet_multi_class_device *res_class = NULL;
               multi_class_port = res_dev_enet->hspi.pkt.device.port;
               WPI_REGISTRY_OBJECT_GET(lock_status,
                                       WPI_RegistryEnetMultiClassGet,
                                       WPI_REG_LOCKED,
                                       &res_enet_multi_class,
                                       wpid);
               if(WP_ERROR_P(lock_status)) return;
               port_index = WPI_HANDLE_INDEX(multi_class_port) - WPI_enet_base;
               port_entry = res_enet_multi_class->port[port_index];
               if(!port_entry) return;
               for (ii=port_entry->start_index ; ii<port_entry->end_index ; ii++)
               {
                  res_class = port_entry->multi_class_dev[ii];
                  if (res_class != NULL)
                  {
                     WPI_VirtualDeviceDisplay(level, res_class->virtual_dev_handle);
                  }
               }
               WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                           WPI_RegistryEnetMultiClassRelease,
                                           WPI_REG_UNLOCKED,
                                           &res_enet_multi_class);
               if(WP_ERROR_P(lock_status)) return;
            }
         }

      }
   }

   WPI_EmphyTransDevicesDisplay(level, wpid);
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceHandleSubtype
 *
 * Purpose:
 *             Display structure corresponding to handle.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      level   depth of display call
 *      handle  handle of object being displayed
 *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_Display
 *
 ****************************************************************************/
WP_U16 WPI_DeviceHandleSubtype (WP_device devtype, WP_handle port)
{
   WP_U32 port_index = WPI_HANDLE_INDEX (port);

   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
         switch(devtype)
         {
            case WP_DEVICE_ATM:
            case WP_DEVICE_IMA:
            case WP_DEVICE_GBOND:
               return WPI_pg_atm_upi1 + (port_index - WP_PORT_UPI1);
            case WP_DEVICE_TRANS:
               return WPI_pg_emphy_trans;
            default:
               return WPI_pg_pkt_upi1 + (port_index - WP_PORT_UPI1);
         }
         break;
      case WP_PORT_ENET1:
      case WP_PORT_ENET2:
      case WP_PORT_ENET3:
      case WP_PORT_ENET4:
      case WP_PORT_ENET5:
      case WP_PORT_ENET6:
      case WP_PORT_ENET7:
      case WP_PORT_ENET8:
      case WP_PORT_ENET9:
      case WP_PORT_ENET10:
      case WP_PORT_ENET11:
      case WP_PORT_ENET12:
      case WP_PORT_ENET13:
      case WP_PORT_ENET14:
      case WP_PORT_ENET15:
      case WP_PORT_ENET16:
      case WP_PORT_XGI1:
      case WP_PORT_XGI2:
         if (devtype == WP_DEVICE_ENET_G999)
            return WPI_pg_pkt_enet_g999;
         else
            return pg_pkt[port_index];
      default:
         if (port_index >= WPI_tdi_base && port_index <= WPI_tdi_last) {
            if (devtype == WP_DEVICE_TRANS)
               return WPI_pg_tdi_trans;
            else if (devtype == WP_DEVICE_ATM)
               return WPI_pg_atm_tdi;
            else
               return WPI_pg_pkt_tdi;
         }
         else
         {
#if _WP_MODULE_DUMP_ENABLE_
            WPI_Dump_enum_WP_DEVICE (0, "ERROR in WPI_DeviceHandleSubtype. Device",
                                     devtype);
            WPI_Dump_enum_WP_PORT_NUMBERS (0,
                                           "ERROR in WPI_DeviceHandleSubtype. Port",
                                           port_index);
#endif
         }
   }
   return 0;
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceHwqGet
 *
 * Purpose:
 *             fetches the hwq number of the device.
 *
 * Description:
 *             This function fetches the hwq number of the device.
 *
 * Inputs:
 *             device_handle: handle of device
 *             p_hwq: a pointer to a WP_U32 to be set with the hwq number
 *
 * Outputs:
 *
 * Return Value:
 *             status
 *
 * Called by:
 *     WPI_IwMcMemberSet
 *
 ****************************************************************************/
WP_status WPI_DeviceHwqGet(WP_handle device_handle, WP_U32 *p_hwq)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_resource_entry *dev_entry;
   wpi_res_device_hspi *dev_hspi;
   WP_U32 IsHsEnet = 0;
   wpi_res_device_hspi_enet *hs_enet_device = NULL;

   wpi_resource_block *resources;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   resources = &wpi_registry_ptr->pwp_resources[WPI_HANDLE_WPID(device_handle)];

   if (WPI_HANDLE_SUBTYPE(device_handle) == WPI_pg_emphy_trans)
      return WPI_ERROR(WP_ERR_WDDI);

   /* Get pointer to device and verify that it is direct-queued. */
   WPI_ACCESS_RES(dev_entry, resources, WPI_HANDLE_INDEX( device_handle));
   dev_hspi = WPI_RES_DEV_HSPI(dev_entry);

   if (dev_hspi->pkt.device.devtype != WP_DEVICE_HSPOS)
   {
      hs_enet_device = WPI_RES_DEV_HSPI_ENET(dev_entry);
      if (hs_enet_device->hspi.pkt.device.devtype != WP_DEVICE_ENET &&
          hs_enet_device->hspi.pkt.flowmode != WPI_DEVICE_PKT_FLOWMODE_FAST)
         return WPI_ERROR(WP_ERR_DEVICE_NOT_DIRECT_QUEUED);
      IsHsEnet = 1;
   }

   if (IsHsEnet)
   {
      *p_hwq = hs_enet_device->hspi.tx_fifo_start;
      WPI_LOG(WPI_DEBUG_LEVEL_3,
              (wpi_log,"**AGG** WPI_IwFiwtBinding, hwq = %x\n",
               (WP_U32)*p_hwq));
   }
   else
      *p_hwq = dev_hspi->tx_fifo_start;

   WPI_LOG( WPI_DEBUG_LEVEL_1,
            ( wpi_log, "WPI_DeviceHwqGet: hwq: %#x\n", *p_hwq));

   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceAssignChannel
 *
 * Purpose:
 *             To count the number of channels which are assign to the device.
 *             the function counter the number of channel created for the
 *             device.
 *
 * Description:
 *
 *
 *
 * Inputs:
 *      device  device handle
  *
 * Outputs:
 *
 * Return Value:
 *     void
 *
 * Called by:
 *     WP_ChannelCreate
 *
 ****************************************************************************/

WP_status WPI_DeviceAssignChannel(WP_handle device, WP_handle channel)
{
   wpi_res_enet_multi_class *res_enet_multi_class = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_device *dev;

   WPI_INTERNAL_FUNCTION();
   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_emphy_trans)
      dev = WPI_RegistryEmphyTransDeviceGetMain(device);
   else
   {
      if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(device))
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryEnetMultiClassGet,
                                 WPI_REG_LOCKED,
                                 &res_enet_multi_class,
                                 WPI_HANDLE_WPID(device));
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         dev = WPI_MULTI_CLASS_DEV_BYHANDLE(res_enet_multi_class,
                                            device);
      }
      else
      {
         WPI_REGISTRY_DEVICE_GET_BYHANDLE(lock_status,
                                          &dev,
                                          device);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
      }
   }

   if (!dev)
      return WPI_ERROR(WP_ERR_WDDI);

   dev->ch_counter++;
   if (WPI_HANDLE_TX_CHANNEL_P(channel))
      dev->tx_ch_counter++;
   else
      dev->rx_ch_counter++;

   if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(device))
   {
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                  WPI_RegistryEnetMultiClassRelease,
                                  WPI_REG_UNLOCKED,
                                  &res_enet_multi_class);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
   }
   return WP_OK;
}

WP_status WPI_DeviceUnassignChannel(WP_handle device, WP_handle channel)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_device *dev = NULL;
   wpi_res_enet_multi_class *res_enet_multi_class = NULL;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (WPI_HANDLE_SUBTYPE(device) == WPI_pg_emphy_trans)
      dev = WPI_RegistryEmphyTransDeviceGetMain(device);
   else
   {
      if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(device))
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryEnetMultiClassGet,
                                 WPI_REG_LOCKED,
                                 &res_enet_multi_class,
                                 WPI_HANDLE_WPID(device));
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         dev = WPI_MULTI_CLASS_DEV_BYHANDLE(res_enet_multi_class,
                                            device);
      }
      else
      {
         WPI_REGISTRY_DEVICE_GET_BYHANDLE(lock_status,
                                          &dev,
                                          device);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
      }
   }

   if (!dev)
      return WPI_ERROR(WP_ERR_WDDI);

   dev->ch_counter--;
   if (WPI_HANDLE_TX_CHANNEL_P(channel))
      dev->tx_ch_counter--;
   else
      dev->rx_ch_counter--;

   return WP_OK;
}


/*****************************************************************************
 *
 * Function:   WPI_DeviceGetType
 *
 * Purpose:    Returns device type
 *
 * Description:
 *
 * Inputs:
 *      dev - pointer to the device registry structure
 *
 * Outputs:
 *
 * Return Value:
 *     device type for valid pointer, WP_DEVICE_INVALID for NULL pointer.
 *
 * Called by:
 *     internal WDDI functions
 *
 ****************************************************************************/
WP_device WPI_DeviceGetType(wpi_res_device *dev)
{
   WPI_INTERNAL_FUNCTION();
   if (dev)
      return dev->devtype;

   return WP_DEVICE_INVALID;
}

/*****************************************************************************
 *
 * Function:   WPI_DeviceGetChCounter
 *
 * Purpose:    Returns number of channels assigned to the specific device
 *
 * Description:
 *
 * Inputs:
 *      dev - pointer to the device registry structure
 *
 * Outputs:
 *
 * Return Value:
 *     number of channels assigned to the given device
 *
 * Called by:
 *     internal WDDI functions
 *
 ****************************************************************************/
WP_U32 WPI_DeviceGetChCounter(wpi_res_device *dev)
{
   return (dev->ch_counter);
}

WP_status WPI_FeatureInitDeviceRateLimit(WP_handle device, WP_feature_sw_phy_rate_limit *config)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   WP_U32 wpid = WPI_HANDLE_WPID(device);

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_RETURN_ERROR_IF_FALSE(
      WPI_DpsFeatureCheckStatus(wpid,
                                WPI_FEATUREMODE_GLOBAL,
                                0,
                                WPI_DPS_MODULE_PSU_PHY_RATE_LIMITER_ENABLE),
      WP_ERR_DPS_FEATURE_UNAVAILABLE);
   if (WPI_HANDLE_TYPE(device) == WPI_handle_gbond_group)
   {
      wpi_atm_gbond_functions *gbond_res = WPI_RegistryGbondFunctions(wpid);
      if( gbond_res )
      {
         status = gbond_res->init_rate_limit(device, config->rate_limit_mode);
      }
      else
      {
         status = WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
      }

   }
   else if (WPI_HANDLE_TYPE(device) == WPI_handle_device)
   {
      wpi_resource_entry *device_entry;
      wpi_res_device *res_dev;
      WPI_ACCESS_RES(device_entry, resources, WPI_HANDLE_INDEX(device));
      if (device_entry == NULL)
         WPI_BREAK(status, WPI_FeatureInitDeviceRateLimitEnd, WP_ERR_WDDI);

      /* Check that device was initialized. */
      if (!WPI_INITIALIZED(device_entry->state))
         WPI_BREAK(status, WPI_FeatureInitDeviceRateLimitEnd, WP_ERR_DEVICE);

      res_dev = WPI_RES_DEV(device_entry);
      switch (WPI_HANDLE_SUBTYPE(device))
      {
         case WPI_pg_pkt_upi1:
         case WPI_pg_pkt_upi2:
         case WPI_pg_pkt_upi3:
         case WPI_pg_pkt_upi4:
            switch (res_dev->devtype)
            {
               case WP_DEVICE_ATM:
               case WP_DEVICE_GBOND:
                  WPI_RETURN_IF_ERROR(status,
                                      WPI_FeatureInitUpiAtmRateLimit(wpid, resources,
                                                                     device_entry,
                                                                     res_dev,
                                                                     config));
                  break;
               case WP_DEVICE_HDLC:
                  WPI_RETURN_IF_ERROR(status,
                                      WPI_FeatureInitUpiHdlcRateLimit(wpid, resources,
                                                                      device_entry,
                                                                      res_dev,
                                                                      config));
                  break;
               default:
                  WPI_BREAK(status, WPI_FeatureInitDeviceRateLimitEnd, WP_ERR_NOT_IMPLEMENTED);
            }
            break;
         default:
            WPI_BREAK(status, WPI_FeatureInitDeviceRateLimitEnd, WP_ERR_NOT_IMPLEMENTED);
      }

   }
   else
      status = WPI_ERROR(WP_ERR_HANDLE);
WPI_FeatureInitDeviceRateLimitEnd:
   return status;
}


/*****************************************************************************
 *
 * Function: WPI_DeviceIsFast
 *
 * Purpose: Check if the input device word at fast mode (flow-mode is FAST or FMU_HIERARCHICAL)
 *
 * Description:
 *
 * Inputs:
 *         device_handle: handle of device
 *
 * Outputs: result
 *
 * Return Value:
 *
 * Called by:
 *     internal WDDI functions
 *
 ****************************************************************************/

WP_status WPI_DeviceIsFast(WP_handle device_handle, WP_boolean *result)
{
   WP_status lock_status = WP_OK;
   wpi_res_device *res_device;
   wpi_res_device_hspi *res_device_hspi = NULL;

   WPI_INTERNAL_FUNCTION();

   if (WPI_HANDLE_DEVICE_P(device_handle))
   {
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryDeviceGet,
                              WPI_REG_LOCKED,
                              &res_device,
                              WPI_HANDLE_WPID(device_handle),
                              WPI_HANDLE_INDEX(device_handle));
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      res_device_hspi = (wpi_res_device_hspi *) res_device;

      if ((res_device->devtype == WP_DEVICE_ENET ||
           res_device->devtype == WP_DEVICE_ENET_MULTI_CLASS ||
           res_device->devtype == WP_DEVICE_HSPOS) &&
          (res_device_hspi->pkt.flowmode == WPI_DEVICE_PKT_FLOWMODE_FAST ||
           res_device_hspi->pkt.flowmode == WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL))
         *result = WP_TRUE;
      else
         *result = WP_FALSE;
   }
   else
   {
      return WPI_ERROR(WP_ERR_DEVICE);
   }

   return WP_OK;
}

WP_handle WP_G999StreamCreate(WP_handle device_handle,
                              WP_U16 stream_type,
                              void *config)
{
   WP_handle stream_handle;
   WPI_WDDI_ATTACH_MT();
   stream_handle = WPI_G999StreamCreate(device_handle,
                                        stream_type,
                                        config);
   WPI_WDDI_DETACH_MT();
   return stream_handle;
}

WP_status WP_G999StreamDelete(WP_handle stream_handle)
{
   WP_status status = WP_OK;
   WPI_WDDI_ATTACH_MT();
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(stream_handle))
   {
      if (! WPI_RegistryDeviceEnetG999(stream_handle))
      {
         WPI_WDDI_DETACH_MT();
         return WPI_ERROR(WP_ERR_HANDLE);
      }
   }
   status = WPI_EnetMultiClassDeviceDelete(stream_handle);
   WPI_WDDI_DETACH_MT();
   return status;
}

WP_status WP_G999StreamTxEnable(WP_handle stream_handle)
{
   WP_status status = WP_OK;
   WPI_WDDI_ATTACH_MT();
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(stream_handle))
   {
      if (! WPI_RegistryDeviceEnetG999(stream_handle))
      {
         WPI_WDDI_DETACH_MT();
         return WPI_ERROR(WP_ERR_HANDLE);
      }
   }
   status = WPI_EnetMultiClassDeviceEnable(stream_handle, WP_DIRECTION_TX);
   WPI_WDDI_DETACH_MT();
   return status;

}

WP_status WP_G999StreamTxDisable(WP_handle stream_handle)
{
   WP_status status = WP_OK;
   WPI_WDDI_ATTACH_MT();
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH_MT();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(stream_handle))
   {
      if (! WPI_RegistryDeviceEnetG999(stream_handle))
      {
         WPI_WDDI_DETACH_MT();
         return WPI_ERROR(WP_ERR_HANDLE);
      }
   }
   status = WPI_EnetMultiClassDeviceDisable(stream_handle, WP_DIRECTION_TX);
   WPI_WDDI_DETACH_MT();
   return status;
}

WP_status WP_G999StreamStatistics(WP_handle stream_handle,
                                  void *stats)
{
   WP_status status = WP_OK;
   WPI_WDDI_ATTACH_MT();
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(stream_handle))
   {
      if (! WPI_RegistryDeviceEnetG999(stream_handle))
      {
         WPI_WDDI_DETACH_MT();
         return WPI_ERROR(WP_ERR_HANDLE);
      }
   }
   else
   {
      WPI_WDDI_DETACH_MT();
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   status = WPI_EnetMultiClassStatistics(stream_handle,
                                 stats,
                                 WP_TRUE /* g999_stats */);
   WPI_WDDI_DETACH_MT();
   return status;
}

WP_status WP_G999StreamstatisticsReset(WP_handle stream_handle,
                                       void *stats)
{
   WP_status status = WP_OK;
   WPI_WDDI_ATTACH_MT();
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(stream_handle))
   {
      if (! WPI_RegistryDeviceEnetG999(stream_handle))
      {
         WPI_WDDI_DETACH_MT();
         return WPI_ERROR(WP_ERR_HANDLE);
      }
   }
   else
   {
      WPI_WDDI_DETACH_MT();
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   status = WPI_EnetMultiClassStatisticsReset(stream_handle,
                                              stats,
                                              WP_TRUE /* g999_stats */);
   WPI_WDDI_DETACH_MT();
   return status;
}

