/*****************************************************************************
 * (C) Copyright 2000-2007, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpi_hw_sa.c
 *
 * Purpose:  SA hardware
 *
 * Functions contained in this file:
 * +----------------------------+---------------------------------------------
 * |   Function name            | Description
 * +----------------------------+---------------------------------------------
 * |   API Functions
 * |   -------------------------+---------------------------------------------
 * |  WP_SaEntryGet             | Gets the TRS limit and mask config of
 * |                            | a specific serial
 * |  WP_SaEntrySet             | Sets the TRS limit and mask config of
 * |                            | a specific serial
 * |   Internal WDDI Functions
 * |   -------------------------+---------------------------------------------
 * +----------------------------+---------------------------------------------
 * +----------------------------+---------------------------------------------
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
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
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_SA_UTIL_H
#include "include/core/hardware/wpi_sa_util.h"
#endif
#ifndef WPI_SA_H
#include "include/core/hardware/wpi_sa.h"
#endif
#ifndef WPI_FEATURES_INT_H
#include "include/core/features/wpi_features_int.h"
#endif
#ifndef WPI_PACKET_H
#include "include/layer_2/packet/wpi_packet.h"
#endif
#ifndef WPI_OS_ENET_H
#include "veneer/wpi_os_enet.h"
#endif
#ifndef WPI_HW_H
#include "include/core/hardware/wpi_hw.h"
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif
#ifndef WPI_HW_TGU_H
#include "include/core/hardware/wpi_hw_tgu.h"
#endif
#ifndef WPI_PORT_UPI_H
#include "include/layer_1/upi/wpi_port_upi.h"
#endif
#ifndef WPI_PORT_ENET_H
#include "include/layer_1/enet/wpi_port_enet.h"
#endif
#ifndef WPI_PORT_ENET_INT_H
#include "include/layer_1/enet/wpi_port_enet_int.h"
#endif
#ifndef WPI_ENET_MULTI_CLASS_INT_H
#include "include/layer_1/enet/wpi_enet_multi_class_int.h"
#endif
#ifndef WPI_ENET_MULTI_CLASS_H
#include "include/layer_1/enet/wpi_enet_multi_class.h"
#endif
#ifndef WPI_PORT_TDI_INT_H
#include "include/layer_1/tdi/wpi_port_tdi_int.h"
#endif
#ifndef WPI_TGU_INT_H
#include "include/core/hardware/wpi_tgu_int.h"
#endif
#ifndef WPI_DEVICE_ATM_INT_H
#include "include/layer_2/atm/wpi_device_atm_int.h"
#endif
#ifndef WPI_DPS_FEATURES_H
#include "include/core/features/wpi_dps_features.h"
#endif
#ifndef WPI_HW_DEVICE_H
#include "wpi_hw_device.h"
#endif
#ifndef WPI_HW_WINNET_H
#include "wpi_hw_winnet.h"
#endif
#ifndef WPI_HW_ADV_USER_H
#include "wpi_hw_adv_user.h"
#endif
#ifndef WPI_SA_INT_H
#include "wpi_sa_int.h"
#endif
#ifndef WPI_HW_AA_H
#include "wpi_hw_aa.h"
#endif
#ifndef WPI_HW_AA_INT_H
#include "wpi_hw_aa_int.h"
#endif
#ifndef WPI_HW_CFU_INT_H
#include "wpi_hw_cfu_int.h"
#endif
#ifndef WPI_STATIC_H
#include "include/core/registry/wpi_static.h"
#endif
#ifndef WPI_DISPLAY_H
#include "include/core/wpi_display.h"
#endif
#ifndef WPI_WIMAX_H
#include "include/layer_2/packet/wimax/wpi_wimax.h"
#endif
#ifndef WPI_PTP_H
#include "include/iw/ptp/wpi_ptp.h"
#endif
#ifndef WPI_IW_CESOP_H
#include "include/iw/pwe3/wpi_iw_cesop.h"
#endif
#ifndef WPI_SCHEDULER_H
#include "include/core/scheduler/wpi_scheduler.h"
#endif
#ifndef WPI_SCHEDULER_INT_H
#include "include/core/scheduler/wpi_scheduler_int.h"
#endif
#ifndef WPI_SCHEDULER_CLASS_H
#include "include/core/scheduler/wpi_scheduler_class.h"
#endif
#ifndef WPI_PORTS_H
#include "include/core/wpi_ports.h"
#endif
#ifndef WPI_HW_ENET_H
#include "wpi_hw_enet_common.h"
#endif

#ifndef WPI_HW_FMU_H
#include "wpi_hw_fmu.h"
#endif

#include "include/core/hardware/wpi_sa_class.h"
#include "include/core/hardware/wpi_tgu_class.h"
#include "include/core/wpi_system_class.h"
#include "include/layer_1/enet/wpi_enet_multi_class_class.h"
#include "include/iw/core/wpi_iw_background_class.h"
#include "include/layer_2/packet/link_aggregation/wpi_link_aggregation_class.h"
#include "wpi_hw_link_aggregation.h"
#include "include/layer_2/packet/efm_bonding/wpi_efm_bonding_class.h"

/* declarations of global functions */
static WP_status WPI_WinMaskToTrsMask(WP_U32 wpid, wpi_aa_trs_mask *trs_mask, wpi_trs_mask *winfarm_mask);
static WP_status WPI_TrsAllMaskGet (WP_U32 wpid, WP_U32 *low_trs, WP_U32 *high_trs);

/* declarations of local functions */
/* WDDI port level */
static WP_status WPI_SaPortCreate(WP_U32 wpid,
                                  WP_port port_index,
                                  wpi_direction direction,
                                  wpi_aa_entry **aa_rx_entry,
                                  wpi_aa_entry **aa_tx_entry);

inline static WP_status WPI_SaSerialGet(WP_port port_index,
                                        WP_sa_serial *serial_name_rx,
                                        WP_sa_serial *serial_name_tx,
                                        wpi_aa_entry_type *aa_entry_type_rx,
                                        wpi_aa_entry_type *aa_entry_type_tx);

/* WDDI device level */
static WP_status WPI_SaDeviceCreate(WP_U32 wpid,
                                    wpi_res_device *res_dev,
                                    WP_direction direction,
                                    wpi_cfu_rx_entry **cfu_rx_entry,
                                    wpi_cfu_tx_entry **cfu_tx_entry);

/* AA entry */
static WP_status WPI_SaAaEntryConfigBuild(WP_U32 wpid,
                                          wpi_res_sa* res_sa,
                                          WP_port port_index,
                                          wpi_resource_block *resources,
                                          WP_direction direction,
                                          wpi_aa_entry_config *aa_entry_config);
/* CFU entry */
static WP_status WPI_SaCfuRxAgentConfigBuild(wpi_res_sa* res_sa,
                                             WP_port port_index,
                                             wpi_cfu_rx_agent_config *cfu_rx_aa_entry_config,
                                             WP_boolean dynamic_entry);

static WP_status WPI_SaCfuRxEntryConfigBuild(WP_U32 wpid,
                                             wpi_res_sa* res_sa,
                                             wpi_res_device *res_dev,
                                             wpi_cfu_rx_entry_config *cfu_rx_entry_config);

static WP_status WPI_SaCfuTxEntryConfigBuild(WP_U32 wpid,
                                             wpi_res_sa* res_sa,
                                             wpi_res_device *res_dev,
                                             wpi_cfu_tx_entry_config *cfu_tx_entry_config);

static WP_status WPI_SaCfuTxEntryActiveRequestsWorkaroundAlloc(WP_U32 wpid,
                                                               WP_port port_index,
                                                               wpi_res_device *res_dev,
                                                               wpi_cfu_tx_entry* cfu_tx_entry,
                                                               wpi_cfu_tx_entry_config* config,
                                                               WP_U16* buffers_num);
#if !defined (WINSIM) && !defined(VERILOG)
static WP_status WPI_HwCommitG999EnetSwapRx(WP_U32 wpid,
                                            wpi_res_sa *res_sa,
                                            wpi_res_device_hspi_enet *dev_enet);
#endif //!defined (WINSIM) && !defined(VERILOG)

/* per serial type functions */
static WP_status WPI_SaUpiAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources);
static WP_status WPI_SaEnetAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources);
static WP_status WPI_SaLagAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources);
static WP_status WPI_SaTdiAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources);
static WP_status WPI_SaEfmBondingAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources);

#if _WP_MODULE_CLOCK_RECOVERY_ENABLE_
static WP_status WPI_SaCESShapingAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources);
#endif


static WP_status WPI_SaTguConfig(WP_U32 wpid,
                                 wpi_res_tgu *res_tgu,
                                 wpi_res_sa *res_sa,
                                 WP_U32 tgu_type,
                                 WP_U32 limit_value,
                                 wpi_aa_trs_mask *mask_value);
static WP_status WPI_SaTguAlloc(WP_U32 wpid,
                                wpi_res_sa* res_sa,
                                wpi_resource_block *resources);
static WP_status
WPI_SaAgentsAllocPerEnetPorts(wpi_enet_ports_devs_types *ports_devs_types,
                              wpi_res_sa* res_sa);

/* inline functions - switches according to serial types */
inline static WP_boolean WPI_SaPortUseCfu(WP_port port_index);
inline static WP_U16 WPI_SaCfuTxBuffersDefaultGet(WP_U32 wpid,
                                                  WP_port port_index,
                                                  wpi_res_device *res_dev,
                                                  WP_boolean dynamic_entry);
inline static void WPI_SaCfuSerialPhyConvert(WP_U32 wpid,
                                             WP_port port_index,
                                             WP_U32 phy_index,
                                             WP_U32 *converted_serial,
                                             WP_U32 *converted_phy);
inline static WP_status WPI_SaSerialToAaEntry(wpi_res_sa *res_sa,
                                              WP_sa_serial serial,
                                              wpi_aa_entry **entry);

inline static WP_status WPI_SaDeviceAllocate(wpi_res_device *res_dev);
inline static WP_status WPI_SaCfuRxEntrySet(wpi_res_device *res_dev,
                                            wpi_cfu_rx_entry* entry);
inline static WP_status WPI_SaCfuTxEntrySet(wpi_res_device *res_dev,
                                            wpi_cfu_tx_entry* entry);
static WP_U16 WPI_SaTodSourceGet(wpi_res_fifo_alloc_type tx_alloc_type,
                                 WP_U32 tx_alloc_num);


static WP_status WPI_TsLoopbackDeviceAlloc(WP_U32 wpid,wpi_res_cfu* res_cfu,
                                           wpi_res_device_hspi_enet *dev_enet,
                                           WP_U32 port_index,
                                           wpi_aa_entry *aa_rx_entry);

static WP_status WPI_SaTxPortGracefulDisable(wpi_res_device *res_dev,
                                             wpi_res_sa *res_sa);

static wpi_rx_agent_pri_drop_th default_rx_agent_pri_drop_th[] =
{
   {
      WP_TRUE,          /* int_th_en */
      WP_FALSE,          /* ext_th_en */
      /* int_1st_th */
      {WPI_CFU_SMART_DROP_1ST_PRIOR_0/32,
       WPI_CFU_SMART_DROP_1ST_PRIOR_1/32,
       WPI_CFU_SMART_DROP_1ST_PRIOR_2/32,
       WPI_CFU_SMART_DROP_1ST_PRIOR_3/32,
       WPI_CFU_SMART_DROP_1ST_PRIOR_4/32,
       WPI_CFU_SMART_DROP_1ST_PRIOR_5/32,
       WPI_CFU_SMART_DROP_1ST_PRIOR_6/32,
       WPI_CFU_SMART_DROP_1ST_PRIOR_7/32
      },
      /* int_non_1st_th */
      {WPI_CFU_SMART_DROP_NON_1ST_PRIOR_0/32,
       WPI_CFU_SMART_DROP_NON_1ST_PRIOR_1/32,
       WPI_CFU_SMART_DROP_NON_1ST_PRIOR_2/32,
       WPI_CFU_SMART_DROP_NON_1ST_PRIOR_3/32,
       WPI_CFU_SMART_DROP_NON_1ST_PRIOR_4/32,
       WPI_CFU_SMART_DROP_NON_1ST_PRIOR_5/32,
       WPI_CFU_SMART_DROP_NON_1ST_PRIOR_6/32,
       WPI_CFU_SMART_DROP_NON_1ST_PRIOR_7/32
      },
      /* agent_1st_th */
      {WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32
      },
      /*agent_non_1st_th */
      {WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32,
       WPI_CFU_SMART_DROP_AGENTQ_START_LEVEL/32
      },
      {0,0,0,0,0,0,0,0}, /* ext_1st_th */
      {0,0,0,0,0,0,0,0}, /* ext_non_1st_th */
   }
};

static wpi_rx_agent_pri_prof default_rx_agent_pri_prof[] =
{
   {
      0,                     /* priority_offset */
      {0, 0, 0, 0, 0, 0, 0}, /* priority_th */
   }
};

static wpi_rx_agent_pause_settings default_wpi_rx_agent_pause_settings[] =
{
   {
      0,        /* agent_counter_th_high */
      0,        /* agent_counter_th_low */
      0,        /* int_fbp_th_high */
      0,        /* int_fbp_th_low */
      0,        /* dps_fbp_th_high */
      0,        /* dps_fbp_th_low */
   }
};


/* this constant is the difference between RX and TX profile_id
   of the same protocol
   it is used because the retreival of the profile_id is done in pairs
   of rx and tx, but only the rx is returned.
*/
#define WPI_SA_PROFILE_ID_TX_OFFSET 1

#define WPI_SA_BAD_PROFILE_ID 0xFF

#define WPI_SA_TDI_RX_PCT_INDEX 0
#define WPI_SA_TDI_TX_PCT_INDEX 1

/*****************************************************************************
 *
 * Function:   WPI_HwSerialId
 *
 * Purpose:    return the profile Id number of the serial
 *
 * Description: does a search on the PCT
 *
 * Inputs:
 *         serial_index    the serial index
 *
 * Outputs:
 *         profile_id      the profile id of the serial
 * Return Value:
 *        WP_OK
 *        WP_ERR_INVALID_SERIAL_INDEX  the serial_index is wrong or has no
 *                                     profile_id in this hardware device
 *
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_HwSerialId(WP_U32 wpid, WP_U32 serial_index, WP_U32 *profile_id)
{
   /* the serial id in WP1 and WP2 is replaced by the profile id */
   wpi_res_sa *res_sa;
   WP_status lock_status = WP_OK;

   WPI_INTERNAL_FUNCTION();

   if ((serial_index >= WPI_HW_SERIAL_TDI1_RX) && (serial_index <= WPI_HW_SERIAL_TDI16_TX))
   {
      *profile_id =  (serial_index - WPI_HW_SERIAL_TDI1_RX + WPI_TDI_MASTER_KEY_OFFSET);
      return WP_OK;
   }

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if (serial_index >= WPI_HW_SERIAL_INDEX_N)
      return WPI_ERROR(WP_ERR_INVALID_SERIAL_INDEX);

   *profile_id = res_sa->serial_to_profile_id_map[serial_index];

   if (*profile_id == WPI_SA_BAD_PROFILE_ID)
      return WPI_ERROR(WP_ERR_INVALID_SERIAL_INDEX);

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_HwSerialIndex
 *
 * Purpose:    return the serial index by profile Id
 *
 * Description:
 *
 * Inputs:
 *         profile_id      the (constant) profile id of the serial
 *
 * Outputs:
 *         serial_index    the serial index
 * Return Value:
 *        WP_OK
 *        WP_ERR_INVALID_SERIAL_ID  the profile_id is wrong or has no
 *                                  serial mapped to it in this hardware device
 *
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_HwSerialIndex(WP_U32 wpid, WP_U32 serial_id, WP_U32 *serial_index)
{

   WP_status status;

   /* the serial id in WP1 and WP2 is replaced by the profile id */
   WP_U32 profile_id = serial_id;

   WPI_INTERNAL_FUNCTION();

   if (profile_id >= WPI_AA_PROFILE_ID_SIZE)
   {
      /* Check if TDI */
      if (profile_id <= WPI_AA_PROFILE_ID_SIZE + WPI_HW_SERIAL_TDI16_TX - WPI_HW_SERIAL_TDI1_RX)
      {
         *serial_index = profile_id - WPI_AA_PROFILE_ID_SIZE + WPI_HW_SERIAL_TDI1_RX;
         return WP_OK;
      }
      else
         return WPI_ERROR(WP_ERR_INVALID_SERIAL_ID);
   }

   status = WPI_RegistrySerialIndexGet(wpid, profile_id, serial_index);
   if (*serial_index == WPI_BAD_SERIAL_ID)
      return WPI_ERROR(WP_ERR_INVALID_SERIAL_ID);

   return status;
}

/* get port index by serial index */
WP_U8 WPI_PortIndexLookup[WPI_HW_SERIAL_INDEX_N] = {
   WP_PORT_UPI1,   /* 0 */
   WP_PORT_UPI1,
   WP_PORT_UPI2,
   WP_PORT_UPI2,
   WP_PORT_UPI3,
   WP_PORT_UPI3,
   WP_PORT_UPI4,
   WP_PORT_UPI4,
   WP_PORT_ENET1,  /* 8 */
   WP_PORT_ENET1,
   WP_PORT_ENET2,
   WP_PORT_ENET2,
   WP_PORT_ENET3,
   WP_PORT_ENET3,
   WP_PORT_ENET4,
   WP_PORT_ENET4,
   WP_PORT_ENET5,
   WP_PORT_ENET5,
   WP_PORT_ENET6,
   WP_PORT_ENET6,
   WP_PORT_ENET7,
   WP_PORT_ENET7,
   WP_PORT_ENET8,
   WP_PORT_ENET8,
   WP_PORT_ENET9,
   WP_PORT_ENET9,
   WP_PORT_ENET10,
   WP_PORT_ENET10,
   WP_PORT_ENET11,
   WP_PORT_ENET11,
   WP_PORT_ENET12,
   WP_PORT_ENET12,
   WP_PORT_ENET13,
   WP_PORT_ENET13,
   WP_PORT_ENET14,
   WP_PORT_ENET14,
   WP_PORT_ENET15,
   WP_PORT_ENET15,
   WP_PORT_ENET16,
   WP_PORT_ENET16,
   WP_PORT_XGI1, /* 40 */
   WP_PORT_XGI1,
   WP_PORT_XGI2,
   WP_PORT_XGI2,
   WP_PORT_TDM1, /* 44 */
   WP_PORT_TDM1,
   WP_PORT_TDM2,
   WP_PORT_TDM2,
   WP_PORT_TDM3,
   WP_PORT_TDM3,
   WP_PORT_TDM4,
   WP_PORT_TDM4,
   WP_PORT_TDM5,
   WP_PORT_TDM5,
   WP_PORT_TDM6,
   WP_PORT_TDM6,
   WP_PORT_TDM7,
   WP_PORT_TDM7,
   WP_PORT_TDM8,
   WP_PORT_TDM8,
   WP_PORT_TDM9,
   WP_PORT_TDM9,
   WP_PORT_TDM10,
   WP_PORT_TDM10,
   WP_PORT_TDM11,
   WP_PORT_TDM11,
   WP_PORT_TDM12,
   WP_PORT_TDM12,
   WP_PORT_TDM13,
   WP_PORT_TDM13,
   WP_PORT_TDM14,
   WP_PORT_TDM14,
   WP_PORT_TDM15,
   WP_PORT_TDM15,
   WP_PORT_TDM16,
   WP_PORT_TDM16,
   WPI_BAD_SERIAL_ID, /* 76 */
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID,
   WPI_BAD_SERIAL_ID, /* 94 */
   WPI_BAD_SERIAL_ID, /* 95 */
   WPI_BAD_SERIAL_ID, /* 96 */
};

WP_status WPI_ApiPortIndex(WP_U32 wpid, WP_U32 serial_id, WP_U32 *port_index)
{
   WP_U32 serial_index;
   WP_U32 status;
   WPI_INTERNAL_FUNCTION();

   WPI_RETURN_IF_ERROR(status,
                       WPI_HwSerialIndex(wpid, serial_id, &serial_index));

   *port_index = WPI_PortIndexLookup[serial_index];
   if (*port_index == WPI_BAD_SERIAL_ID)
      return WPI_ERROR(WP_ERR_INVALID_SERIAL_ID);

   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaEntryGet
 *
 * Purpose:    Retrieves the configuration of TRS limit and mask of
 *             a specific serial (a network device or a TGU)
 *
 * Description:
 *
 * Inputs:
 *     wpid   Winpath ID
 *
 *     serial_name   The serial name of the entry to be retrieved.
 *
 * Outputs:
 *     entry  The current SA configuration of the requested serial
 *
 * Return Value: WP_OK
 *               WP_ERR_DEVICE - sa is not initialized
 *               WP_ERR_SERIAL_NOT_ALLOC - the serial requested was not
 *                                         initialized in the SA
 *               WP_ERR_WRONG_SERIAL_NAME - serial_name exceeds the
 *                                          enum of known serials
 *               WP_ERR_INCORRECT_REGISTRY_STATE - function called before
 *                                                 SysCommit()
 *
 * Called by:    user application
 *
 ****************************************************************************/

WP_status WP_SaEntryGet (WP_U32 wpid,
                         WP_sa_serial serial_name,
                         WP_sa_entry *entry)
{
   WP_status status;
   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR( status,
                               WPI_SaEntryGet(wpid,
                                              serial_name,
                                              entry));

   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaEntryGet
 *
 * Purpose:    Retrieves the configuration of TRS limit and mask of
 *             a specific serial (a network device or a TGU)
 *
 * Description:
 *
 * Inputs:
 *     wpid   Winpath ID
 *
 *     serial_name   The serial name of the entry to be retrieved.
 *
 * Outputs:
 *     entry  The current SA configuration of the requested serial
 *
 * Return Value: WP_OK
 *               WP_ERR_DEVICE - sa is not initialized
 *               WP_ERR_SERIAL_NOT_ALLOC - the serial requested was not
 *                                         initialized in the SA
 *               WP_ERR_WRONG_SERIAL_NAME - serial_name exceeds the
 *                                          enum of known serials
 *               WP_ERR_INCORRECT_REGISTRY_STATE - function called before
 *                                                 SysCommit()
 *
 * Called by:    WP_SaEntryGet
 *
 ****************************************************************************/

WP_status WPI_SaEntryGet(WP_U32 wpid,
                         WP_sa_serial serial_name,
                         WP_sa_entry *sa_entry)
{
   wpi_aa_entry *aa_entry;
   WP_status status, lock_status;
   wpi_res_sa *res_sa;
   WP_U32 trs_limit;
   wpi_trs_mask trs_mask;
   WP_U32 i;

   WPI_INTERNAL_FUNCTION();
   /* check if function was called after SysCommit */
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   if (serial_name >= WP_SA_SERIALS_N)
   {
      return WPI_ERROR(WP_ERR_WRONG_SERIAL_NAME);
   }

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (!res_sa)
      return WPI_ERROR(WP_ERR_WDDI);

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialToAaEntry(res_sa,
                                              serial_name,
                                              &aa_entry));

   if(!WPI_ENTRY_CONFIGURED(aa_entry->state))
      return WPI_ERROR( WP_ERR_AA_ENTRY_WRONG_STATE);

   WPI_RETURN_IF_ERROR(status, WPI_AaEntryConfigGet(res_sa->aa, aa_entry, &trs_limit, &trs_mask));

   /* the limit in the AA register is 1 less than
      the real max_trs */
   sa_entry->max_trs = trs_limit + 1;
   for (i=0; i<WP_SA_WINFARMS; i++)
      sa_entry->trs_mask[i] = trs_mask.wf[i];

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/************************************************************************
 *
 * Function:  WP_SaEntrySet
 *
 * Purpose:   Sets the configuration of TRS limit and mask of
 *            a specific serial (a network device or a TGU)
 *
 * Description:   Changes both the SA register and the WDDI registry
 *
 * Inputs:
 *     wpid   Winpath ID
 *
 *     serial_name   The serial name of the entry to be set.
 *
 *     entry  The entry (both TRS limit and mask) of the serial
 *            to be set and written to the SA
 *
 * Outputs:
 *
 * Return Value: WP_OK
 *               WP_ERR_DEVICE - sa is not initialized
 *               WP_ERR_SERIAL_NOT_ALLOC - the serial requested was not
 *                                         initialized in the SA
 *               WP_ERR_WRONG_SERIAL_NAME - serial_name exceeds the
 *                                          enum of known serials
 *               WP_ERR_SAME_POLL_ADDR_AS_LOWER_SERIAL -
 *                              when two serials are set to the same
 *                              polling address (might happen with ENET
 *                              or SCHED serials) only the "lower" serial
 *                              can be set
 *               WP_ERR_TRS_SET_NOT_VALID_IN_WP1 -
 *                              the mask in the entry exceeds the TRS
 *                              exists in winpath1 (only the 12 LSBs are
 *                              allowed to be set in winpath1)
 *               WP_ERR_TRS_SET_NOT_VALID_IN_CHIP_FAMILY -
 *                              the mask exceeds the TRS valid to this
 *                              chip's configuration (i.e. the chip has
 *                              less then 4 wingines)
 *               WP_ERR_INCORRECT_REGISTRY_STATE - function called before
 *                                                 SysCommit()
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_SaEntrySet (WP_U32 wpid,
                         WP_sa_serial serial_name,
                         WP_sa_entry *entry)
{
   WP_status status;
   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR( status,
                               WPI_SaEntrySet (wpid,
                                               serial_name,
                                               entry));

   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaEntrySet
 *
 * Purpose:   Sets the configuration of TRS limit and mask of
 *            a specific serial (a network device or a TGU)
 *
 * Description:  Changes both the SA register and the WDDI registry
 *
 * Inputs:
 *     wpid   Winpath ID
 *
 *     serial_name   The serial name of the entry to be set.
 *
 *     entry  The entry (both TRS limit and mask) of the serial
 *            to be set and written to the SA
 *
 * Outputs:
 *
 * Return Value: WP_OK
 *               WP_ERR_DEVICE - sa is not initialized
 *               WP_ERR_SERIAL_NOT_ALLOC - the serial requested was not
 *                                         initialized in the SA
 *               WP_ERR_WRONG_SERIAL_NAME - serial_name exceeds the
 *                                          enum of known serials
 *               WP_ERR_INCORRECT_REGISTRY_STATE - function called before
 *                                                 SysCommit()
 *
 * Called by:
 *               WP_SaEntrySa
 *
 ****************************************************************************/

WP_status WPI_SaEntrySet(WP_U32 wpid,
                         WP_sa_serial serial_name,
                         WP_sa_entry *sa_entry)
{
   wpi_aa_entry *aa_entry;
   WP_status status, lock_status;
   wpi_aa_entry_config config;
   wpi_res_sa *res_sa;
   wpi_trs_mask tmp_trs_mask;
   WP_U32 i;

   WPI_INTERNAL_FUNCTION();
   /* check if function was called after SysCommit */
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   if (serial_name >= WP_SA_SERIALS_N)
   {
      return WPI_ERROR(WP_ERR_WRONG_SERIAL_NAME);
   }

   /* Get pointer to serial arbiter registry information */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if (!res_sa)
      return WPI_ERROR(WP_ERR_WDDI);

   /* make a copy of the configuration entry */
   /* the limit to be written to the AA register is one less
      the real max_trs */
   for (i=0; i<WP_SA_WINFARMS; i++)
      tmp_trs_mask.wf[i] = sa_entry->trs_mask[i];

   WPI_RETURN_IF_ERROR(status,
                       WPI_WinMaskToTrsMask(wpid,
                                            &config.trs_mask,
                                            &tmp_trs_mask));

   if(sa_entry->max_trs > WPI_TrsNumGet())
      return WPI_ERROR(WP_ERR_TRS_LIMIT_TOO_HIGH);
   else if(sa_entry->max_trs > 0)
      config.max_trs = sa_entry->max_trs - 1;
   else
   {
      config.max_trs = 0;
      config.trs_mask.trs_mask_high = 0;
      config.trs_mask.trs_mask_low = 0;
   }

   /* get entry by serial */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialToAaEntry(res_sa,
                                              serial_name,
                                              &aa_entry));
   /* apply configuration */
   WPI_RETURN_IF_ERROR( status,
                        WPI_AaEntryConfig(res_sa->aa,
                                          aa_entry,
                                          &config));
   /* do the entry commit, no need for mapping commit */
   WPI_RETURN_IF_ERROR( status,
                        WPI_AaEntryCommit(res_sa->aa,
                                          aa_entry,
                                          WP_FALSE));

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaInitialize
 *
 * Purpose:    initialize registry structures and registers
 *
 *
 * Description: Allocate an array for sa regs and initialize them
 *
 * Inputs:
 *      wpid   Winpath ID
 *
 *      resources      WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *         WP_OK upon successful completion
 *
 * Called by: WP_SysInit
 *
 ****************************************************************************/

WP_status WPI_SaInitialize(WP_U32 wpid)
{
   WP_status status;
   wpi_res_sa* sa;
   WP_U32 ii, jj;
   WP_S32 shaped_enet_multi_class_enable;

   WPI_INTERNAL_FUNCTION();

   sa = (wpi_res_sa *) WPI_WDDI_MALLOC(sizeof(wpi_res_sa));

   if( sa == NULL)
      return WPI_ERROR( WP_ERR_RESOURCE_ALLOCATION);
   memset( sa, 0, sizeof( wpi_res_sa));

   for(ii = 0 ; ii < WP_SA_SERIALS_N ; ++ii)
      sa->serial_to_entry_map[ii] = NULL;

   for(ii = 0 ; ii < WPI_HW_SERIAL_INDEX_N ; ii++)
      sa->serial_to_profile_id_map[ii] = WPI_SA_BAD_PROFILE_ID;

   for(ii = 0 ; ii < WPI_AA_PROFILE_ID_SIZE ; ii++)
   {
      for(jj = 0; jj< WPI_WINFARM_N; jj++)
         sa->aa_profiles_map[ii].pc[jj] = 0;
      sa->aa_profiles_map[ii].serial_id = WPI_BAD_SERIAL_ID;
   }

   WPI_RETURN_IF_ERROR( status, WPI_AaInitialize(wpid, &(sa->aa)));
   WPI_RETURN_IF_ERROR( status, WPI_CfuInitialize(wpid, &(sa->cfu)));

   WPI_RETURN_IF_ERROR(status, WPI_CapabilityRead(wpid,
                                                  WPI_CAP_ENET_MULTI_CLASS_SHAPING,
                                                  &shaped_enet_multi_class_enable));
   if(shaped_enet_multi_class_enable)
      WPI_RETURN_IF_ERROR( status, WPI_CsuInitialize(wpid, &(sa->csu)));

   WPI_RegistrySaInstall(wpid, sa);

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_AllocSa
 *
 * Purpose:   allocate aa and cfu entries,
 *            configure global fields of the cfu
 *
 * Description:
 *     This function does the allocates of all the created CFU and AA entries
 *
 * Inputs:
 *      wpid   Winpath ID
 *
 *      resources      WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *      WP_OK upon successful completion
 *
 * Called by:
 *      WPI_ResourceAllocate (wpi_res_alloc.c)
 ****************************************************************************/

WP_status WPI_AllocSa(WP_U32 wpid, wpi_resource_block *resources)
{
   WP_status status, lock_status;
   wpi_res_sa* res_sa;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* general CFU configuration - read from advanced user file */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuConfig(res_sa->cfu));

   /* allocate aa and cfu entries for all the used ports */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaUpiAlloc(wpid, res_sa, resources));

   /* XAUI Sa allocation need to be here - TBD*/

   /* WPI_SaEnetAlloc function should be called last
      between CFU using ports*/
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaEnetAlloc(wpid, res_sa, resources));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTdiAlloc(wpid, res_sa, resources));


   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguAlloc(wpid, res_sa, resources));

#if _WP_MODULE_CLOCK_RECOVERY_ENABLE_
/* Allocate aa and cfu entries for CES shaping */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaCESShapingAlloc(wpid, res_sa, resources));
#endif

   /* Allocate aa and cfu entries for LAG groups */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaLagAlloc(wpid, res_sa, resources));

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaEfmBondingAlloc(wpid,res_sa,resources));

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaCommit
 *
 * Purpose:    do actual configuration of Aa and Cfu,
 *             commit settings to hardware registers
 *
 * Description:  Initializes the serial arbiter hardware registers
 *          based on perviouslly defined registry values.  PCT table
 *          entries, GMR, etc. are being written here.
 *
 * Inputs:     wpid     Winpath ID
 *
 * Outputs:
 *
 * Return Value:
 *          WP_OK if successful, else it returns an error code:
 *          WP_ERR_DEVICE
 * Called by:  WP_SysCommit
 *
 ****************************************************************************/

WP_status WPI_SaCommit(WP_U32 wpid)
{
   WP_status status, lock_status;
   wpi_res_sa *res_sa;
   WP_U32 pct_entry;
   wpi_trs_mask mask;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaPctInstall(wpid,
                                        WPI_DPS_ENTRYPT_CFU_IDLE,
                                        0,
                                        1,
                                        WPI_HW_SERIAL_CFU_IDLE,
                                        &pct_entry,
                                        NULL,
                                        &mask));

   /* actual writing of registry to hardware */
   WPI_RETURN_IF_ERROR( status,
                        WPI_AaCommit(wpid, res_sa->aa, res_sa->cfu));

   /* actual writing of registry to hardware */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuCommit(res_sa->cfu, res_sa->aa));

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/************************************************************************
 *
 * Function:  WPI_SaDecommit
 *
 * Purpose:   Stop generation of threads
 *
 * Description:
 *
 * Inputs:
 *     wpid   Winpath ID
 *
 * Outputs:
 *
 * Return Value: WP_OK
 *
 * Called by:
 *     WPI_OsShutdownWinpath (wpi_enet_transfer.c)
 *
 ****************************************************************************/

WP_status WPI_SaDecommit(WP_U32 wpid)
{
   wpi_res_reg_bases *res_reg_bases = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryRegisterBaseGet,
                           WPI_REG_LOCKED,
                           &res_reg_bases,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   status = WPI_AaDecommit(res_reg_bases->reg_bases_ptr);
   WPI_RETURN_IF_ERROR_STATUS(status);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryRegisterBaseRelease,
                               WPI_REG_UNLOCKED,
                               &res_reg_bases);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/* WDDI port level */
/*****************************************************************************
 *
 * Function:   WPI_SaPortCreate
 *
 * Purpose:    create and configure aa entry for wddi port,
 *
 * Description:
 *
 * Inputs:
 *             wpid        Winpath ID
 *             port_index   the port to create
 *
 * Outputs:    aa entries are created (by WPI_SaPortInit) and configured
 *
 * Return Value:
 *
 * Called by: WPI_SaUpiAlloc, WPI_SaEnetAlloc, WPI_SaTdiAlloc
 *
 ****************************************************************************/

static WP_status WPI_SaPortCreate(WP_U32 wpid,
                                  WP_port port_index,
                                  wpi_direction direction,
                                  wpi_aa_entry **aa_rx_entry,
                                  wpi_aa_entry **aa_tx_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;

   wpi_aa_entry_config aa_entry_config;
   wpi_res_sa *res_sa;
   WP_sa_serial serial_name_rx = 0, serial_name_tx = 0;
   wpi_aa_entry_type aa_entry_type_rx = 0, aa_entry_type_tx = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialGet(port_index,
                                        &serial_name_rx,
                                        &serial_name_tx,
                                        &aa_entry_type_rx,
                                        &aa_entry_type_tx));

   /* create port aa resource */

   /* RX */
   if(direction == WPI_DIRECTION_DUPLEX || direction == WPI_DIRECTION_RX || direction == WPI_DIRECTION_RX_INTSW)
   {
      wpi_cfu_rx_agent_config cfu_rx_aa_entry_config;
      WP_boolean use_cfu;
      WP_boolean dynamic_entry = (direction == WPI_DIRECTION_RX_INTSW);

      memset(&cfu_rx_aa_entry_config, 0, sizeof(cfu_rx_aa_entry_config));

      WPI_RETURN_IF_ERROR( status, WPI_AaEntryCreate(res_sa->aa,
                                                     aa_entry_type_rx,
                                                     aa_rx_entry,
                                                     dynamic_entry));

      /* set configuration to entry */
      /* build configuration */
      WPI_RETURN_IF_ERROR( status,
                           WPI_SaAaEntryConfigBuild(wpid,
                                                    res_sa,
                                                    port_index,
                                                    resources,
                                                    direction,
                                                    &aa_entry_config));

      /* apply configuration */
      WPI_RETURN_IF_ERROR( status,
                           WPI_AaEntryConfig(res_sa->aa,
                                             *aa_rx_entry,
                                             &aa_entry_config));

      use_cfu = WPI_SaPortUseCfu(port_index);
      if(use_cfu)
      {
         /* get default values */
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaCfuRxAgentConfigBuild(res_sa,
                                                          port_index,
                                                          &cfu_rx_aa_entry_config,
                                                          dynamic_entry));
         /* set the agent CFU configuration */
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuRxAgentsConfig(res_sa->cfu,
                                                    *aa_rx_entry,
                                                    &cfu_rx_aa_entry_config));
      }

      WPI_ENTRY_SET_USED((*aa_rx_entry)->state);
      /* map serial->entry */
      if(direction != WPI_DIRECTION_RX_INTSW)
         res_sa->serial_to_entry_map[serial_name_rx] = *aa_rx_entry;
   }
   /* TX */
   if(direction == WPI_DIRECTION_DUPLEX || direction == WPI_DIRECTION_TX)
   {
      WP_boolean dynamic_entry = WP_FALSE;

      WPI_RETURN_IF_ERROR( status, WPI_AaEntryCreate(res_sa->aa,
                                                     aa_entry_type_tx,
                                                     aa_tx_entry,
                                                     dynamic_entry));
      /* set configuration to entry */
      /* build configuration */
      WPI_RETURN_IF_ERROR( status,
                           WPI_SaAaEntryConfigBuild(wpid,
                                                    res_sa,
                                                    port_index,
                                                    resources,
                                                    WPI_DIRECTION_TX,
                                                    &aa_entry_config));
      /* apply configuration */
      WPI_RETURN_IF_ERROR( status,
                           WPI_AaEntryConfig(res_sa->aa,
                                             *aa_tx_entry,
                                             &aa_entry_config));

      WPI_ENTRY_SET_USED((*aa_tx_entry)->state);
      /* map serial->entry */
      res_sa->serial_to_entry_map[serial_name_tx] = *aa_tx_entry;
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/* WDDI device level */
/*****************************************************************************
 *
 * Function:   WPI_SaDeviceCreate
 *
 * Purpose:    create and configure Cfu entry for WDDI device
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             res_dev        pointer to device resources
 *
 * Outputs:    CFU entries are created (by WPI_SaDeviceInit) and configured
 *
 * Return Value:
 *
 * Called by: WPI_SaUpiAlloc, WPI_SaEnetAlloc
 *
 ****************************************************************************/

static WP_status WPI_SaDeviceCreate(WP_U32 wpid,
                                    wpi_res_device *res_dev,
                                    WP_direction direction,
                                    wpi_cfu_rx_entry **cfu_rx_entry,
                                    wpi_cfu_tx_entry **cfu_tx_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_boolean use_cfu;
   WP_U32 port_index;
   WP_U32 serial_num=0, phy_num=0;

   wpi_res_sa *res_sa;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   port_index = WPI_HANDLE_INDEX(res_dev->port);

   use_cfu = WPI_SaPortUseCfu(port_index);

   if(!use_cfu)
      return WP_OK;

   /* convert port and phy to cfu serial and phy */


   WPI_SaCfuSerialPhyConvert(wpid,
                             port_index,
                             res_dev->phy_number,
                             &serial_num,
                             &phy_num);

   /* RX */
   if(direction == WPI_DIRECTION_DUPLEX || direction == WPI_DIRECTION_RX)
   {
      wpi_cfu_rx_entry_config cfu_rx_entry_config;
      /* create entry */
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuRxEntryCreate(res_sa->cfu,
                                                serial_num,
                                                phy_num,
                                                cfu_rx_entry));

      /* build configured values */
      WPI_RETURN_IF_ERROR( status,
                           WPI_SaCfuRxEntryConfigBuild(wpid,
                                                       res_sa,
                                                       res_dev,
                                                       &cfu_rx_entry_config));
      /* set configuration */
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuRxEntryConfig(res_sa->cfu,
                                                *cfu_rx_entry,
                                                &cfu_rx_entry_config));
   }

   /* TX */
   if(direction == WPI_DIRECTION_DUPLEX || direction == WPI_DIRECTION_TX)
   {
      WP_U16 tx_buff;
      wpi_cfu_tx_entry_config cfu_tx_entry_config;
      wpi_res_sa_device *sa_dev;

      memset(&cfu_tx_entry_config, 0, sizeof(cfu_tx_entry_config));

      /* create entry */
      /* malloc and save serial + phy_num */
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuTxEntryCreate(res_sa->cfu,
                                                serial_num,
                                                phy_num,
                                                cfu_tx_entry));

      /* build configured values */
      /* Find profile id and set the PCT entries in it,
       * if it doesn't exist, allocate one. */
      WPI_RETURN_IF_ERROR( status,
                           WPI_SaCfuTxEntryConfigBuild(wpid,
                                                       res_sa,
                                                       res_dev,
                                                       &cfu_tx_entry_config));

      /* tx buffers in the CFU - can be initialized through extended parameters */
      sa_dev = (wpi_res_sa_device *)(res_dev->hardware_specific);

      tx_buff = sa_dev->tx_buffers_request;

      if(tx_buff == 0) /* uninitialized */
      {
         /* get default */
         tx_buff = WPI_SaCfuTxBuffersDefaultGet(wpid, port_index, res_dev, WP_FALSE);
      }

      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuTxEntryActiveRequestsWorkaroundAlloc(wpid,
                                                                        port_index,
                                                                        res_dev,
                                                                        *cfu_tx_entry,
                                                                        &cfu_tx_entry_config,
                                                                        &tx_buff));

      /* add request for buffers number */
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryBuffersRequest(res_sa->cfu,
                                                       *cfu_tx_entry,
                                                       tx_buff));

      /* set configuration */
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuTxEntryConfig(res_sa->cfu,
                                                *cfu_tx_entry,
                                                &cfu_tx_entry_config));

      if(res_dev->devtype == WP_DEVICE_EMPHY)
      {
         (*cfu_tx_entry)->configuration.tx_start_th = WPI_CFU_TX_START_TH_0_8;
      }
   }
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaMultiClassDeviceCreate
 *
 * Purpose:    create and configure Cfu entry for WDDI device
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *
 * Outputs:    CFU entries are created (by WPI_SaDeviceInit) and configured
 *
 * Return Value:
 *
 * Called by: WPI_SaEnetMultiClassAlloc
 *
 ****************************************************************************/

static WP_status WPI_SaMultiClassDeviceCreate(WP_U32 wpid,
                                              wpi_res_device_hspi_enet *physical_dev_enet,
                                              WP_U32 allocation_phy_number,
                                              wpi_res_enet_multi_class_device *res_class,
                                              wpi_cfu_tx_entry **cfu_tx_entry)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 port_index;
   WP_U32 serial_num = 0, phy_num;
   WP_U16 tx_buff;
   wpi_cfu_tx_entry_config cfu_tx_entry_config;
   wpi_res_sa_device *sa_dev;
   wpi_res_sa *res_sa;
   WP_boolean dynamic_entry = WP_FALSE;

   WPI_INTERNAL_FUNCTION();

   memset(&cfu_tx_entry_config, 0, sizeof(wpi_cfu_tx_entry_config));

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   port_index = WPI_HANDLE_INDEX(physical_dev_enet->hspi.pkt.device.port);

   /* convert port and phy to cfu serial and phy */
   WPI_SaCfuSerialPhyConvert(wpid,
                             port_index,
                             physical_dev_enet->hspi.pkt.device.phy_number,
                             &serial_num,
                             &phy_num);

   phy_num = allocation_phy_number; /*In case of G999<-->ENET swap mode,
                                      allocation_phy_number has offset of 1, in order
                                      to reserve PHY 0 to the physical device*/
   /* create entry */
   /* malloc and save serial + phy_num */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuTxEntryCreate(res_sa->cfu,
                                             serial_num,
                                             phy_num,
                                             cfu_tx_entry));


   if(res_class->pkt.device.devtype == WP_DEVICE_ENET_MULTI_CLASS_INTSW)
   {
      /* if it is an internal switchign class, the intsw_config should be allocated and would be used
       * later on as indication that this cfu_tx_entry is an intsw tx entry (for WPI_SaPctInstall) */
      cfu_tx_entry_config.intsw_tx_config = (wpi_intsw_tx_config *) WPI_WDDI_MALLOC(sizeof(wpi_intsw_tx_config));
      if(cfu_tx_entry_config.intsw_tx_config == NULL)
         return WPI_ERROR( WP_ERR_RESOURCE_ALLOCATION);
      memset( cfu_tx_entry_config.intsw_tx_config, 0, sizeof( wpi_intsw_tx_config));
   }

   /* build configured values */
   /* Find profile id and set the PCT entries in it,
    * if it doesn't exist, allocate one. */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaCfuTxEntryConfigBuild(wpid,
                                                    res_sa,
                                                    &(physical_dev_enet->hspi.pkt.device),
                                                    &cfu_tx_entry_config));
   /* tx buffers in the CFU - can be initialized through extended parameters */
   sa_dev = (wpi_res_sa_device *)(res_class->pkt.device.hardware_specific);

   tx_buff = sa_dev->tx_buffers_request;

   if(tx_buff == 0) /* uninitialized */
   {
      /* get default */
      dynamic_entry = (res_class->pkt.device.devtype == WP_DEVICE_ENET_MULTI_CLASS_INTSW);
      tx_buff = WPI_SaCfuTxBuffersDefaultGet(wpid, port_index, &(physical_dev_enet->hspi.pkt.device), dynamic_entry);
   }

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuTxEntryActiveRequestsWorkaroundAlloc(wpid,
                                                                     port_index,
                                                                     &(physical_dev_enet->hspi.pkt.device),
                                                                     *cfu_tx_entry,
                                                                     &cfu_tx_entry_config,
                                                                     &tx_buff));

   /* add request for buffers number */
   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuTxEntryBuffersRequest(res_sa->cfu,
                                                    *cfu_tx_entry,
                                                    tx_buff));

   /* set configuration */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuTxEntryConfig(res_sa->cfu,
                                             *cfu_tx_entry,
                                             &cfu_tx_entry_config));
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceStateSet
 *
 * Purpose:    enable device CFU Tx port
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid          Winpath ID
 *             device_index  the index of the device entry in the registry
 *             enable        enable / disable boolean
 *
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *             WP_DeviceEnable
 ****************************************************************************/
WP_status WPI_SaDeviceStateSet(WP_U32 wpid,
                               wpi_res_device *res_dev,
                               WP_U32 direction,
                               WP_boolean enable)
{
   wpi_res_sa *res_sa;
   WP_U32 port_index;
   WP_status status, lock_status;

   port_index = WPI_HANDLE_INDEX(res_dev->port);

   /* if the serial doesnt use CFU - do nothing */
   if(res_dev->port != WPI_efm_bonding_over_enet)
   {
      if(!WPI_SaPortUseCfu(port_index))
      {
         return WP_OK;
      }
   }

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if (direction == WP_DIRECTION_TX || direction == WP_DIRECTION_DUPLEX)
   {
      wpi_hw_cfu_tx *hw_cfu_tx;
      wpi_cfu_tx_entry *cfu_tx_entry = 0;
      WP_U16 cfu_tx_port;
      WP_U8 cfu_tx_agent;

      hw_cfu_tx = res_sa->cfu->cfu_tx->hw_cfu_tx;

      /* get device resources */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuTxEntryGet(res_dev, &cfu_tx_entry));

      /* enable the CFU Tx port associated with the device */
      if (! cfu_tx_entry )
         return WPI_ERROR(WP_ERR_DEVICE);

      cfu_tx_agent = cfu_tx_entry->allocation.agent_num;
      cfu_tx_port = cfu_tx_entry->allocation.cfu_port_num;

      if((enable == WP_DISABLE) &&
         (WPI_HANDLE_ENET_DEVICE_P(res_dev->device_handle)))
      {
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaTxPortGracefulDisable(res_dev,
                                                         res_sa));
      }
      else
      {
         WPI_RETURN_IF_ERROR(status,
                             WPI_CfuRegTxPortEnable(hw_cfu_tx,
                                                    cfu_tx_agent,
                                                    cfu_tx_port,
                                                    enable));
      }
   }

#if !defined (WINSIM) && !defined(VERILOG)
   if (direction == WP_DIRECTION_RX || direction == WP_DIRECTION_DUPLEX)
   {
      wpi_hw_cfu_rx *hw_cfu_rx;
      wpi_cfu_rx_entry *cfu_rx_entry = 0;
      WP_U32 base;

      if((res_dev->devtype == WP_DEVICE_ENET) ||
         (res_dev->devtype == WP_DEVICE_ENET_MULTI_CLASS) ||
         (res_dev->devtype == WP_DEVICE_HSPOS))
      {
         hw_cfu_rx = res_sa->cfu->cfu_rx->hw_cfu_rx;
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuRxEntryGet(res_dev, &cfu_rx_entry));

         switch(port_index)
         {
            case WP_PORT_UPI1:
               base = WPI_TA_BASE_UPI1;
               break;
            case WP_PORT_UPI2:
               base = WPI_TA_BASE_UPI2;
               break;
            case WP_PORT_UPI3:
               base = WPI_TA_BASE_UPI3;
               break;
            case WP_PORT_UPI4:
               base = WPI_TA_BASE_UPI4;
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
            {
               wpi_res_device_hspi_enet *dev_enet = (wpi_res_device_hspi_enet*)res_dev;
               if (dev_enet->swap_g999_with_enet_mode)
               {
                  WPI_RETURN_IF_ERROR(status,
                                      WPI_HwCommitG999EnetSwapRx(wpid,
                                                                 res_sa,
                                                                 dev_enet));
               }
               base = WPI_HwEnetTaBaseGet(port_index);
               break;
            }
            case WP_PORT_XGI1:
            case WP_PORT_XGI2:
               base = WPI_HwXgiTaBaseGet(port_index);
               break;
            default:
               return WPI_ERROR(WP_ERR_UNIMPLEMENTED);
               break;
         }
         WPI_RETURN_IF_ERROR(status,
                             WPI_CfuRxPortEnable(res_sa->cfu,
                                                 hw_cfu_rx,
                                                 cfu_rx_entry,
                                                 base,
                                                 enable));
      }
   }
#endif

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceStatistics
 *
 * Purpose:    read CFU device statistics
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid          Winpath ID
 *             device_index  the index of the device entry in the registry
 *             stats         pointer to the CFU port statistics table
 * Outputs:
 *             statistics  device cfu statistics
 * Return Value:
 *             WP_OK
 * Called by:
 *             WP_DeviceStatistics
 ****************************************************************************/

WP_status WPI_SaDeviceStatistics(WP_U32 wpid,
                                 WP_U32 device_index,
                                 void* stats)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block * resources;
   wpi_res_sa *res_sa;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   WP_U32 port_index;
   wpi_cfu_rx_entry *cfu_rx_entry = 0;
   wpi_cfu_tx_entry *cfu_tx_entry = 0;
   WP_boolean use_cfu;
   WP_stats_serial_dps_if *cfu_stats = (WP_stats_serial_dps_if *)stats;
   WP_stats_serial_tx_dps_if *cfu_tx_stats = (WP_stats_serial_tx_dps_if *)stats;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);

   port_index = WPI_HANDLE_INDEX(res_dev->port);

   use_cfu = WPI_SaPortUseCfu(port_index);

   if(use_cfu && (res_dev->devtype != WP_DEVICE_EMPHY))
   {
      /* get the CFU entry */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuTxEntryGet(res_dev,
                                              &cfu_tx_entry));
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuRxEntryGet(res_dev,
                                              &cfu_rx_entry));

      /* get TX statistics */
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryStatistics(res_sa->cfu,
                                                   cfu_tx_entry,
                                                   cfu_tx_stats));

      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuRxEntryStatistics(res_sa->cfu,
                                                   cfu_rx_entry,
                                                   cfu_stats));
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaTotalMultiClassDevStatistics
 *
 * Purpose:    In Multi Class device:
 *             1. read RX CFU port for physical device statistics
 *             2. Accumulate all Multi class devices (CFU tx ports)
 *                which are attached to this physical device
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid          Winpath ID
 *             device_index  the index of the device entry in the registry
 *             stats         pointer to the CFU port statistics table
 * Outputs:
 *             statistics  device cfu statistics
 * Return Value:
 *             WP_OK
 * Called by:
 *             WPI_EnetStatistics
 ****************************************************************************/

WP_status WPI_SaTotalMultiClassDevStatistics(WP_U32 wpid,
                                             WP_U32 device_index,
                                             void* stats)
{
   WP_status status = WP_OK, lock_status = WP_OK;

   /* Physical device params*/
   wpi_res_registry *wpi_registry_ptr = NULL;
   wpi_resource_block * resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device_hspi_enet *dev_enet;

   /* Multi class params */
   wpi_res_enet_multi_class_device *res_class;
   wpi_res_enet_multi_class *res_enet_multi_class = NULL;
   wpi_res_device *res_dev;
   wpi_res_enet_multi_class_port *port_entry;
   WP_U32 class_index;
   WP_U32 port_index;

   /* cfu params */
   wpi_res_sa *res_sa;
   wpi_cfu_rx_entry *cfu_rx_entry = 0;
   wpi_cfu_tx_entry *cfu_tx_entry = 0;
   WP_boolean use_cfu;

   WP_U64 temp_tx_frames;
   WP_U64 temp_tx_bytes;
   WP_stats_serial_dps_if *cfu_stats = (WP_stats_serial_dps_if *)stats;
   WP_stats_serial_tx_dps_if *cfu_tx_stats = (WP_stats_serial_tx_dps_if *)stats;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   dev_entry = WPI_RES_ENTRY(wpid, device_index);
   if (!WPI_INITIALIZED(dev_entry->state))
   {
      return WPI_ERROR(WP_ERR_ENET_DEV_UNINITIALIZED);
   }
   dev_enet = WPI_RES_DEV_HSPI_ENET(dev_entry);
   port_index = WPI_HANDLE_INDEX(dev_enet->hspi.pkt.device.port);

   /*********************/
   /* CFU TX statistics */
   /*********************/

   memset(&temp_tx_frames, 0, sizeof(WP_U64));
   memset(&temp_tx_bytes , 0, sizeof(WP_U64));

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryEnetMultiClassGet,
                           WPI_REG_LOCKED,
                           &res_enet_multi_class,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_RETURN_IF_NULL(res_enet_multi_class, WP_ERR_ENET_MULTI_CLASS_NOT_ENABLED);

   port_entry = res_enet_multi_class->port[port_index - WPI_enet_base];
   device_index = WPI_HANDLE_INDEX(port_entry->main_dev_handle);
   use_cfu = WPI_SaPortUseCfu(WPI_enet_base + port_index); /* Port's physical index */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   /* Pass through all multi class devices, connected to device and
      accumulate tx statistics */
   for(class_index = port_entry->start_index;
       class_index < port_entry->end_index;
       class_index++)
   {
      res_class = port_entry->multi_class_dev[class_index];

      if (res_class != NULL)
      {
         res_dev = &(res_class->pkt.device);
         if(use_cfu && (res_dev->devtype != WP_DEVICE_EMPHY))
         {
            /* Get the CFU TX entry */
            WPI_RETURN_IF_ERROR(status,
                                WPI_SaCfuTxEntryGet(res_dev,
                                                    &cfu_tx_entry));

            /* Get TX statistics */
            WPI_RETURN_IF_ERROR(status,
                                WPI_CfuTxEntryStatistics(res_sa->cfu,
                                                         cfu_tx_entry,
                                                         cfu_tx_stats));

            /* Accumulate all multi class devices stats */
            temp_tx_frames += cfu_tx_stats->tx_frames;
            temp_tx_bytes  += cfu_tx_stats->tx_bytes;
         }
      }
   }

   memcpy(&cfu_tx_stats->tx_frames, &temp_tx_frames, sizeof(WP_U64));
   memcpy(&cfu_tx_stats->tx_bytes , &temp_tx_bytes , sizeof(WP_U64));

   /*********************/
   /* CFU RX statistics */
   /*********************/

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);

   port_index = WPI_HANDLE_INDEX(res_dev->port);

   use_cfu = WPI_SaPortUseCfu(port_index);

   if(use_cfu && (res_dev->devtype != WP_DEVICE_EMPHY))
   {
      /* Get the CFU RX entry */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuRxEntryGet(res_dev,
                                              &cfu_rx_entry));

      /* Get RX statistics */
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuRxEntryStatistics(res_sa->cfu,
                                                   cfu_rx_entry,
                                                   cfu_stats));
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryEnetMultiClassRelease,
                               WPI_REG_UNLOCKED,
                               &res_enet_multi_class);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}


/*****************************************************************************
 *
 * Function:   WPI_SaMultiClassDeviceStatistics
 *
 * Purpose:    read only CFU TX device statistics for multi class device
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid          Winpath ID
 *             device_index  the index of the device entry in the multi class registry
 *             stats         pointer to the CFU port statistics table
 * Outputs:
 *             statistics  device cfu statistics
 * Return Value:
 *             WP_OK
 * Called by:
 *             WPI_EnetMultiClassStatistics
 ****************************************************************************/

WP_status WPI_SaMultiClassDeviceStatistics(WP_U32 wpid,
                                           wpi_res_device *res_dev,
                                           void* stats)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_sa *res_sa = NULL;
   WP_U32 port_index;
   wpi_cfu_rx_entry *cfu_rx_entry = 0;
   wpi_cfu_tx_entry *cfu_tx_entry = 0;
   WP_boolean use_cfu;
   WP_stats_serial_tx_dps_if *cfu_tx_stats = (WP_stats_serial_tx_dps_if *)stats;
   WP_stats_serial_tx_dps_if temp_cfu_tx_stats;

   WPI_INTERNAL_FUNCTION();

   memset(&temp_cfu_tx_stats, 0, sizeof(WP_stats_serial_tx_dps_if));

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   port_index = WPI_HANDLE_INDEX(res_dev->port);
   use_cfu = WPI_SaPortUseCfu(port_index);

   if(use_cfu && (res_dev->devtype != WP_DEVICE_EMPHY))
   {
      /* get the CFU entry */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuTxEntryGet(res_dev,
                                              &cfu_tx_entry));
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuRxEntryGet(res_dev,
                                              &cfu_rx_entry));

      /* get TX statistics */
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryStatistics(res_sa->cfu,
                                                   cfu_tx_entry,
                                                   &temp_cfu_tx_stats));

      memcpy(&(cfu_tx_stats->tx_frames), &(temp_cfu_tx_stats.tx_frames), sizeof(WP_U64));
      memcpy(&(cfu_tx_stats->tx_bytes) , &(temp_cfu_tx_stats.tx_bytes) , sizeof(WP_U64));
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaIntswMultiClassDeviceStatistics
 *
 * Purpose:    read only CFU TX device statistics for multi class device
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid          Winpath ID
 *             device_index  the index of the device entry in the multi class registry
 *             stats         pointer to the CFU port statistics table
 * Outputs:
 *             statistics  device cfu statistics
 * Return Value:
 *             WP_OK
 * Called by:
 *             WPI_EnetMultiClassStatistics
 ****************************************************************************/

WP_status WPI_SaIntswMultiClassDeviceStatistics(WP_U32 wpid,
                                                wpi_res_device *res_dev,
                                                void* stats)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_sa *res_sa = NULL;
   WP_U32 port_index;
   wpi_cfu_rx_entry *cfu_rx_entry = 0;
   wpi_cfu_tx_entry *cfu_tx_entry = 0;
   WP_boolean use_cfu;
   WP_stats_intsw_serial_dps_if *cfu_intsw_stats = &(((WP_stats_enet_multi_class_intsw *)stats)->serial_intsw_dps_if_stats);
   WP_stats_intsw_serial_dps_if temp_cfu_intsw_stats;
   WP_stats_serial_tx_dps_if *cfu_tx_stats = &(((WP_stats_enet_multi_class_intsw *)stats)->serial_tx_dps_if_stats);
   WP_stats_serial_tx_dps_if temp_cfu_tx_stats;

   WPI_INTERNAL_FUNCTION();

   memset(&temp_cfu_tx_stats, 0, sizeof(WP_stats_serial_tx_dps_if));
   memset(&temp_cfu_intsw_stats, 0, sizeof(WP_stats_intsw_serial_dps_if));

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   port_index = WPI_HANDLE_INDEX(res_dev->port);
   use_cfu = WPI_SaPortUseCfu(port_index);

   if(use_cfu && (res_dev->devtype != WP_DEVICE_EMPHY))
   {
      /* get the CFU entry */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuTxEntryGet(res_dev,
                                              &cfu_tx_entry));
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuRxEntryGet(res_dev,
                                              &cfu_rx_entry));

      /* get TX statistics */
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryStatistics(res_sa->cfu,
                                                   cfu_tx_entry,
                                                   &temp_cfu_tx_stats));

      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuIntswEntryStatistics(res_sa->cfu,
                                                      cfu_tx_entry,
                                                      &temp_cfu_intsw_stats));


      memcpy(&(cfu_tx_stats->tx_frames), &(temp_cfu_tx_stats.tx_frames), sizeof(WP_U64));
      memcpy(&(cfu_tx_stats->tx_bytes) , &(temp_cfu_tx_stats.tx_bytes) , sizeof(WP_U64));

      memcpy(&(cfu_intsw_stats->rx_pce_denied_frames), &temp_cfu_intsw_stats.rx_pce_denied_frames, sizeof(WP_U64));
      memcpy(&(cfu_intsw_stats->rx_pce_denied_bytes), &temp_cfu_intsw_stats.rx_pce_denied_bytes, sizeof(WP_U64));

      memcpy(cfu_intsw_stats->rx_frames, temp_cfu_intsw_stats.rx_frames, sizeof(cfu_intsw_stats->rx_frames));
      memcpy(cfu_intsw_stats->rx_bytes, temp_cfu_intsw_stats.rx_bytes, sizeof(cfu_intsw_stats->rx_bytes));
      memcpy(cfu_intsw_stats->rx_pce_dropped_frames, temp_cfu_intsw_stats.rx_pce_dropped_frames, sizeof(cfu_intsw_stats->rx_pce_dropped_frames));
      memcpy(cfu_intsw_stats->rx_pce_dropped_bytes, temp_cfu_intsw_stats.rx_pce_dropped_bytes, sizeof(cfu_intsw_stats->rx_pce_dropped_bytes));
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}
/*****************************************************************************
 *
 * Function:   WPI_SaDeviceStatisticsReset
 *
 * Purpose:    reset CFU device statistics
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the
 *             stats          pointer to the CFU port statistics
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *             WP_DeviceStatisticsReset
 ****************************************************************************/

WP_status WPI_SaDeviceStatisticsReset(WP_U32 wpid,
                                      WP_U32 device_index,
                                      void* stats)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block * resources;
   wpi_res_sa *res_sa;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   WP_U32 port_index = 0;
   wpi_cfu_rx_entry *cfu_rx_entry = 0;
   wpi_cfu_tx_entry *cfu_tx_entry = 0;
   WP_boolean use_cfu;
   WP_stats_serial_dps_if *cfu_stats = (WP_stats_serial_dps_if *)stats;
   wpi_resource_entry *port_entry = NULL;
   wpi_res_enet *res_enet = NULL;
   WP_boolean enable_multiclass = WP_FALSE;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);
   port_index = WPI_HANDLE_INDEX(res_dev->port);

   use_cfu = WPI_SaPortUseCfu(port_index);
   WPI_ACCESS_RES(port_entry, resources, port_index);


   if ((res_dev->devtype == WP_DEVICE_ENET) &&
       WPI_RES_ENTRY_INITIALIZED(port_entry)  )
   {
      res_enet = WPI_RES_ENET(port_entry);
      enable_multiclass = res_enet->enable_multiclass;
   }

   if(use_cfu)
   {
      /* get the CFU entry */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuRxEntryGet(res_dev,
                                              &cfu_rx_entry));

      /* clear RX statistics */
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuRxEntryStatisticsReset(res_sa->cfu,
                                                        cfu_rx_entry,
                                                        cfu_stats));

      if( ! enable_multiclass )
      {
         /*if it is a physical device without Multi-Class */
         /* get the CFU entry */
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet(res_dev,
                                                 &cfu_tx_entry));

         /* reset TX statistics */
         WPI_RETURN_IF_ERROR(status,
                             WPI_CfuTxEntryStatisticsReset(res_sa->cfu,
                                                           cfu_tx_entry,
                                                           cfu_stats));
      }
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaMultiClassDeviceStatisticsReset
 *
 * Purpose:    reset CFU TX port device statistics for Multi Class Devices
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the
 *             stats          pointer to the CFU port statistics
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *             WP_DeviceStatisticsReset
 ****************************************************************************/

WP_status WPI_SaMultiClassDeviceStatisticsReset(WP_U32 wpid,
                                                wpi_res_device *res_dev,
                                                void* stats)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_sa *res_sa = NULL;
   WP_U32 port_index;
   wpi_cfu_tx_entry *cfu_tx_entry = 0;
   WP_boolean use_cfu;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   port_index = WPI_HANDLE_INDEX(res_dev->port);

   use_cfu = WPI_SaPortUseCfu(port_index);

   if(use_cfu)
   {
      /* get the CFU TX entry */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuTxEntryGet(res_dev,
                                              &cfu_tx_entry));

      /* reset TX statistics */
      if(cfu_tx_entry->configuration.intsw_tx_config)
      {
         if (stats)
         {
            memset(stats,0,sizeof(WP_stats_enet_multi_class_intsw));
         }
         /* reset statistics for intsw multi-class device */
         WPI_RETURN_IF_ERROR(status,
			     WPI_CfuIntswEntryStatisticsReset(res_sa->cfu,
                                                              cfu_tx_entry,
                                                              stats));
      }
      else
      {
         if (stats)
         {
            memset(stats,0,sizeof(WP_stats_enet_multi_class_tx));
         }
         /* reset statistics for non-intsw multi-class device */
         WPI_RETURN_IF_ERROR(status,
                             WPI_CfuTxEntryStatisticsReset(res_sa->cfu,
                                                           cfu_tx_entry,
                                                           stats));
      }
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}


/* user defined bits accessors */
/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxUDSet
 *
 * Purpose:    set the user defined bits of rx cfuport
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *             user_defined   32bit passed with rx data from cfu to dps
 *
 * Outputs:
 *
 * Return Value:
 *               WP_OK
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_SaDeviceRxUDSet(WP_U32 wpid,
                              wpi_res_device *res_dev,
                              WP_U32 *user_defined,
                              void *entry)
{
   WP_status status = WP_OK, lock_status;
   wpi_cfu_rx_entry *rx_entry = 0;
   wpi_res_sa *res_sa = NULL;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if(entry != NULL)
   {
      rx_entry = (wpi_cfu_rx_entry  *)entry;
   }
   else
   {
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuRxEntryGet(res_dev,
                                              &rx_entry));
   }

   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuRxEntryUDSet(wpid,
                                           rx_entry,
                                           user_defined));
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceSpecificCFUEntryUDSet
 *
 * Purpose:    set the user defined bits of a specific cfuport.
 *             (made for timestamp loopback cfu rx)
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *             user_defined   32bit passed with rx data from cfu to dps
 *             entry          the specific entry pointer
 *
 * Outputs:
 *
 * Return Value:
 *               WP_OK
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_SaDeviceSpecificCFUEntryUDSet(WP_U32 wpid,
                                            WP_U32 *user_defined,
                                            void * entry)
{
   wpi_cfu_rx_entry * cfu_rx_entry = entry;  /* only cfu rx*/
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_sa *res_sa = NULL;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuRxEntryUDSet(wpid,
                                           cfu_rx_entry,
                                           user_defined));

   /* after sys commit */
   if(WPI_REGISTRY_USE() == WP_TRUE)
   {

      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuRxEntryCommit(res_sa->cfu,
                                                cfu_rx_entry,
                                                WP_FALSE));
   }
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}




/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxUDGet
 *
 * Purpose:    get the user defined bits of rx cfuport
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *
 * Outputs:
 *             user_defined   32bit passed with rx data from cfu to dps
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_SaDeviceRxUDGet(WP_U32 wpid,
                              WP_U32 device_index,
                              WP_U32 *user_defined)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block * resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   wpi_cfu_rx_entry *rx_entry=0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuRxEntryGet(res_dev,
                                           &rx_entry));

   return WPI_CfuRxEntryUDGet(wpid,
                              rx_entry,
                              user_defined);
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxPceUDGet
 *
 * Purpose:    get the pce user defined bits of rx cfuportg
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *
 * Outputs:
 *             user_defined the UD array passed with rx data from CFU to PCE
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_SaDeviceRxPceUDGet(WP_U32 wpid,
                                 WP_U32 device_index,
                                 WP_U32 *user_defined)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block * resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   wpi_cfu_rx_entry *rx_entry = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuRxEntryGet(res_dev,
                                           &rx_entry));

   return WPI_CfuRxEntryUDGet(wpid,
                              rx_entry,
                              user_defined);
}

/*****************************************************************************
 *
 * Function:   WPI_CfuIntswAgentEntryUDSet
 *
 * Purpose:    set the user defined bits of tx cfuport
 *
 *
 * Description:
 *
 * Inputs:
 *        cfu_tx_entry  pointer to created entry
 *        user_defined   32bit passed with tx data from cfu to dps
 *
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *             WPI_SaDeviceTxUDSet (wpi_hw_sa.c)
 ****************************************************************************/

WP_status WPI_CfuIntswAgentEntryUDSet(wpi_cfu_tx_entry* cfu_tx_entry,
                                      WP_U32 *user_defined)
{
   WPI_INTERNAL_FUNCTION();

   if(!WPI_ENTRY_CONFIGURED(cfu_tx_entry->state))
      return WPI_ERROR( WP_ERR_CFU_ENTRY_WRONG_STATE);

   if(! (cfu_tx_entry->configuration.intsw_tx_config))
      return WPI_ERROR( WP_ERR_CFU_ENTRY_WRONG_STATE);

   cfu_tx_entry->configuration.intsw_tx_config->intsw_agent_entry->dynamic_settings->user_defined = *user_defined;
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceTxUDSet
 *
 * Purpose:    set the user defined bits of tx cfuport
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *             user_defined   32bit passed with tx request from cfu to dps
 *
 * Outputs:
 *
 * Return Value:
 *               WP_OK
 * Called by:
 *
 ****************************************************************************/
WP_status WPI_SaDeviceTxUDSet(WP_U32 wpid,
                              wpi_res_device *res_dev,
                              WP_U32 *user_defined)
{
   WP_status status = WP_OK;
   wpi_cfu_tx_entry *tx_entry = 0;

   WPI_INTERNAL_FUNCTION();

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuTxEntryGet(res_dev,
                                           &tx_entry));

   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuTxEntryUDSet(wpid,
                                           tx_entry,
                                           user_defined));
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaIntswEntryUDSet
 *
 * Purpose:    set the user defined bits of tx cfuport
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *             user_defined   32bit passed with tx request from cfu to dps
 *
 * Outputs:
 *
 * Return Value:
 *               WP_OK
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_SaIntswEntryUDSet(WP_U32 wpid,
                                wpi_res_enet_multi_class_device *enet_multi_class_device)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_cfu_tx_entry *tx_entry = 0;
   wpi_res_sa *res_sa = NULL;
   wpi_res_device *res_dev = &(enet_multi_class_device->pkt.device);
   WP_U32 user_defined;
   WP_U32 tx_phy_num;
   WP_U32 sub_port_vlan_mode;




   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_RETURN_IF_ERROR(status,
                       WPI_HwEnetPhyIndexGet(enet_multi_class_device->virtual_dev_handle,
                                             &tx_phy_num,
                                             NULL));

   sub_port_vlan_mode = (enet_multi_class_device->sub_port_vlan_mode == WP_INTSW_SUB_PORT_VLAN_ADD);
   user_defined = WPI_REG_FIELD(WPI_INTSW_STATUS2_PHYNUM, tx_phy_num & WPI_REG_MASK_BITS(WPI_INTSW_STATUS2_PHYNUM)) |
      WPI_REG_FIELD(WPI_INTSW_STATUS2_MAXSDU, (enet_multi_class_device->intsw_maxsdu / WPI_INTSW_MAXSDU_UNITS) & WPI_REG_MASK_BITS(WPI_INTSW_STATUS2_MAXSDU)) |
      WPI_REG_FIELD(WPI_INTSW_STATUS2_STE, enet_multi_class_device->tx_statmode) |
      WPI_REG_FIELD(WPI_INTSW_STATUS2_SUB_PORT_VLAN_ADD, sub_port_vlan_mode & WPI_REG_MASK_BITS(WPI_INTSW_STATUS2_SUB_PORT_VLAN_ADD));

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuTxEntryGet(res_dev,
                                           &tx_entry));

   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuIntswAgentEntryUDSet(tx_entry,
                                                   &user_defined));


   /* after sys commit */
   if(WPI_REGISTRY_USE() == WP_TRUE)
   {
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuDynamicAgentRegSet(res_sa->cfu->cfu_rx->hw_cfu_rx,
                                                     tx_entry->configuration.intsw_tx_config->intsw_agent_entry));
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceTxUDGet
 *
 * Purpose:    get the user defined bits of tx cfuport
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *
 * Outputs:
 *             user_defined   32bit passed with tx request from cfu to dps
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_SaDeviceTxUDGet(WP_U32 wpid,
                              WP_U32 device_index,
                              WP_U32 *user_defined)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block * resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   wpi_cfu_tx_entry *tx_entry = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuTxEntryGet(res_dev,
                                           &tx_entry));

   return WPI_CfuTxEntryUDGet(wpid,
                              tx_entry,
                              user_defined);
}

/*****************************************************************************
 *
 * Function:      WPI_HwDeviceTodSet
 *
 * Purpose:       Set TOD source for device / Tx CfuPort
 *
 * Description:
 *
 * Inputs:
 *        wpid          Winpath ID
 *        device_index  index of the device in the registry
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_HwDeviceTodSet(WP_U32 wpid,
                             wpi_res_device *res_dev,
                             wpi_res_fifo_alloc_type tx_alloc_type,
                             WP_U32 tx_alloc_num)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_cfu_tx_entry *tx_entry = 0;
   wpi_cfu_tx_entry_config cfu_tx_entry_config;
   wpi_res_sa *res_sa = NULL;
   WP_U32 port_index;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   port_index = WPI_HANDLE_INDEX(res_dev->port);

   /* get entry */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuTxEntryGet(res_dev,
                                           &tx_entry));
   /* get entry configuration */
   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuTxEntryConfigGet(res_sa->cfu,
                                               tx_entry,
                                               &cfu_tx_entry_config));
   /* set the TOD source */
   if(res_dev->devtype == WP_DEVICE_EMPHY)
   {
      cfu_tx_entry_config.tod_input_number =
         WPI_SA_TOD_SOURCE_UPI1_EMPHY + (port_index - WP_PORT_UPI1);
   }
   else
   {
      cfu_tx_entry_config.tod_input_number =
         WPI_SaTodSourceGet(tx_alloc_type, tx_alloc_num);
   }

   /* if port uses TOD, wait_entry should be configured to timer 0,
      that configured to 0 wait cycles*/
   if(cfu_tx_entry_config.tod_input_number != WPI_CFU_TX_CFUPORT_NO_TOD)
      cfu_tx_entry_config.wait_entry = 0;

   /* set the configuration */
   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuTxEntryConfig(res_sa->cfu,
                                            tx_entry,
                                            &cfu_tx_entry_config));

   /* if after syscommit - write to hw registers */
   if (WPI_REGISTRY_USE() == WP_TRUE)
   {
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuTxEntryCommit(res_sa->cfu,
						res_sa->aa,
                                                tx_entry,
                                                WP_FALSE));
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:      WPI_SaSerialRxWmmFifoSet
 *
 * Purpose:       Set WMM num and fifo num to be used for CFU "external FBP"
 *                thresholds checks
 *
 * Description:
 *
 * Inputs:
 *        wpid          Winpath ID
 *        serial_name   the serial_name for which the wmm fifo is set
 *        wmm           wmm number to be used by the device
 *        fifo_num      the number of wmm fifo to be used by the device
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *
 ****************************************************************************/

WP_status WPI_SaSerialRxWmmFifoSet(WP_U32 wpid,
                                   WP_U8 serial_name,
                                   WP_U32 wmm,
                                   WP_U16 fifo_num)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_cfu_rx_agent_config cfu_rx_aa_entry_config;
   wpi_res_sa *res_sa = NULL;
   wpi_aa_entry *aa_entry;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   /* get AA entry associated with the serial */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialToAaEntry(res_sa,
                                              serial_name,
                                              &aa_entry));

   /* get the agent CFU configuration */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuRxAgentsConfigGet(res_sa->cfu,
                                                 aa_entry,
                                                 &cfu_rx_aa_entry_config));

   cfu_rx_aa_entry_config.wmm_num = wmm;
   cfu_rx_aa_entry_config.dps_fbp_num = fifo_num;

   /* set the agent CFU configuration */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuRxAgentsConfig(res_sa->cfu,
                                              aa_entry,
                                              &cfu_rx_aa_entry_config));

   /* if after syscommit - write to hw registers */
   if (WPI_REGISTRY_USE() == WP_TRUE)
   {
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuRxAaEntryCommit(res_sa->cfu,
                                                  aa_entry));
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return WP_OK;
}

/************************************************************************
 *
 * Function:  WPI_SetTrsStl
 *
 * Purpose:   Initializes the trs_allocation struct in the registry.
 *            This struct (declared in wpi_sa.h) will contain all
 *            the default limits for all the serials. Later the SA will
 *            be configured according to the values in this struct.
 *
 * Description:
 *
 * Inputs:
 *     wpid   Winpath ID
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *     WP_SysInit (wpi_driver.c)
 *
 ****************************************************************************/

void WPI_SetTrsStl(WP_U32 wpid)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_trs_allocation *trs_allocation;
   wpi_aa_trs_mask aa_trs_mask;
   WP_U32 n_wingines;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return;

   /* Get the trs_allocation registry entry. */
   trs_allocation = wpi_registry_ptr->pwp_block[wpid].trs_allocation;

   /* Set the "all" mask according to what Wingines are present. */
   WPI_TrsAllMaskGet(wpid, &aa_trs_mask.trs_mask_low, &aa_trs_mask.trs_mask_high);
   WPI_AaTrsMaskFill(&trs_allocation->trs_limit_all, &aa_trs_mask);

   n_wingines = WPI_WingineNumGet(wpid);

   /* Specific allocations */
   trs_allocation->trs_limit_enet_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_ENET_RX, n_wingines);
   trs_allocation->trs_limit_enet_rx_iw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_ENET_RX_IW, n_wingines);
   trs_allocation->trs_limit_enet_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_ENET_TX, n_wingines);
   trs_allocation->trs_limit_genet_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_GENET_RX, n_wingines);
   trs_allocation->trs_limit_genet_rx_iw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_GENET_RX_IW, n_wingines);
   trs_allocation->trs_limit_genet_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_GENET_TX, n_wingines);
   trs_allocation->trs_limit_genet_intsw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_GENET_INTSW, n_wingines);
   trs_allocation->trs_limit_xgi_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_XGI_RX, n_wingines);
   trs_allocation->trs_limit_xgi_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_XGI_TX, n_wingines);
   trs_allocation->trs_limit_xgi_intsw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_XGI_INTSW, n_wingines);
   trs_allocation->trs_limit_token_thread =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TOKEN_THREAD, n_wingines);
   trs_allocation->trs_limit_token_thread_2 =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TOKEN_THREAD_2, n_wingines);
   trs_allocation->trs_limit_host_cmd =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_HOST_CMD, n_wingines);
   trs_allocation->trs_limit_gbond_bg =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_GBOND_BG, n_wingines);
   trs_allocation->trs_limit_gbond_to =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_GBOND_TO, n_wingines);
   trs_allocation->trs_limit_ima_rx_iw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_IMA_RX_IW, n_wingines);
   trs_allocation->trs_limit_ima_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_IMA_TX, n_wingines);
   trs_allocation->trs_limit_iw_bkgnd =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_IW_BKGND, n_wingines);
   trs_allocation->trs_limit_iwgp_bkgnd =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_IWGP_BKGND, n_wingines);
   trs_allocation->trs_limit_iw_tx_host =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_IW_TX_HOST, n_wingines);
   trs_allocation->trs_limit_l2_bkgnd =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_L2_BKGND, n_wingines);
   trs_allocation->trs_limit_mch_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_MCH_RX, n_wingines);
   trs_allocation->trs_limit_mch_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_MCH_TX, n_wingines);
   trs_allocation->trs_limit_mlppp_rx_iw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_MLPPP_RX_IW, n_wingines);
   trs_allocation->trs_limit_oam_fm =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_OAM_FM, n_wingines);
   trs_allocation->trs_limit_policer_wrap_prev =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_POLICER_WRAP_PREV, n_wingines);
   trs_allocation->trs_limit_sched =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_SCHED, n_wingines);
   trs_allocation->trs_limit_tdi_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TDI_RX, n_wingines);
   trs_allocation->trs_limit_tdi_rx_iw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TDI_RX_IW, n_wingines);

   /*TDI_workaround
   now again set back to 2*/
   trs_allocation->trs_limit_tdi_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TDI_TX, n_wingines);

   trs_allocation->trs_limit_upi_hspi_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_UPI_HSPI_RX, n_wingines);
   trs_allocation->trs_limit_upi_hspi_rx_iw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_UPI_HSPI_RX_IW, n_wingines);
   trs_allocation->trs_limit_upi_rx_iw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_UPI_RX_IW, n_wingines);
   trs_allocation->trs_limit_upi_rx_pos =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_UPI_RX_POS, n_wingines);
   trs_allocation->trs_limit_upi_rx_utopia =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_UPI_RX_UTOPIA, n_wingines);
   trs_allocation->trs_limit_upi_tx_pos =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_UPI_TX_POS, n_wingines);
   trs_allocation->trs_limit_upi_tx_utopia =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_UPI_TX_UTOPIA, n_wingines);
   trs_allocation->trs_limit_tgu_sw_timestamp =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TGU_SW_TIMESTAMP, n_wingines);
   trs_allocation->trs_limit_tgu_millisecond_timer =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TGU_MILLISECOND_TIMER, n_wingines);
   trs_allocation->trs_limit_mfr_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_MFR_RX, n_wingines);
   trs_allocation->trs_limit_mfr_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_MFR_TX, n_wingines);
   trs_allocation->trs_limit_mfr_emphy_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_MFR_EMPHY_TX, n_wingines);
   trs_allocation->trs_limit_tgu_bpressure =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TGU_BPRESSURE, n_wingines);
   trs_allocation->trs_limit_ras_to =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_RAS_TO, n_wingines);
   trs_allocation->trs_limit_itdm_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_ITDM_RX, n_wingines);
   trs_allocation->trs_limit_wse_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_WSE_RX, n_wingines);
   trs_allocation->trs_limit_wse_tx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_WSE_TX, n_wingines);
   trs_allocation->trs_limit_efm_bond_rx =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_EFM_BOND_RX, n_wingines);
   trs_allocation->trs_limit_slow_pro_cw =
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_SLOW_PRO_CW, n_wingines);
   trs_allocation->trs_limit_10G_long_pkt=
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_10G_LONG_PKT, n_wingines);
   trs_allocation->trs_limit_lag=
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_LAG, n_wingines);
   trs_allocation->trs_limit_efm_bonding=
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_EFM_BONDING, n_wingines);
   trs_allocation->trs_limit_ces_shaping=
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_CES_SHAPING, n_wingines);
   trs_allocation->trs_limit_sgmii_an_tgu=
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_SGMII_AN_TGU, n_wingines);
   trs_allocation->trs_limit_winnet_an_restart_tgu=
      WPI_TRS_LIMIT(WPI_DEFAULT_COUNT_TGU_AN_RESTART, n_wingines);

}


/************************************************************************
 *
 * Function:  WPI_SaPctFind
 *
 * Purpose:  Searches a PC according to an given entry
 *
 * Description:  Checks all the winfarms for a PC according
 *               to given entries (Rx and Tx)
 *
 * Inputs:     wpid               Winpath ID
 *             protocol_type_rx   receive protocol type
 *             protocol_type_tx   transmit protocol type
 *             num_p              number of protocol types
 *
 * Outputs:
 *             pc_rx          the pointer to the PC for the Rx entry
 *             pc_tx          the pointer to the PC for the Tx entry
 *             trs_mask       TRS mask entry
 *
 * Return Value:
 *     WP_OK
 *
 * Called by: WPI_SaPctSedChange, WPI_SaPctInstall
 *
 **************************************************************************/

WP_status WPI_SaPctFind(WP_U32 wpid,
                        WP_U32 protocol_type_rx,
                        WP_U32 protocol_type_tx,
                        WP_U32 num_p,
                        WP_U16 *pc_rx,
                        WP_U16 *pc_tx,
                        wpi_trs_mask *mask_temp)
{
   WP_status status = WP_OK;
   wpi_res_winfarm *res_winfarm;
   WP_U16 pc_rx_found = 0xFFFF, pc_tx_found = 0xFFFF;
   WP_U32 ii;
   WP_U16 n_winfarm;


   WPI_INTERNAL_FUNCTION();

   WPI_RETURN_IF_ERROR(status, WPI_Limits(wpid, WPI_LIMITS_WINFARM, &n_winfarm));

   for (ii = 0; ii < n_winfarm; ii++)
   {
      res_winfarm = WPI_RegistryResWinfarmGet(wpid, ii);
      if (res_winfarm == NULL)
         continue;


      /* Check if protocol types are in supported range */
      if ((protocol_type_rx > res_winfarm->n_entry_points) ||
          ((protocol_type_tx > res_winfarm->n_entry_points) && (num_p == 2)))
         continue;
      /* Get PCs from registry */
      pc_rx[ii] = res_winfarm->entry_points[protocol_type_rx];

      if (num_p == 2)
         pc_tx[ii] = res_winfarm->entry_points[protocol_type_tx];

      /* Verify WinFarms' PC match (only when the entries are non-zero) */
      if (((pc_rx[ii] != 0) && (pc_rx[ii] != pc_rx_found) && (pc_rx_found != 0xFFFF)) ||
          ((pc_tx[ii] != 0) && (pc_tx[ii] != pc_tx_found) && (pc_tx_found != 0xFFFF)))
         return WPI_ERROR(WP_ERR_DPS_IMAGES_INCOMPATIBLE_ACROSS_WINFARMS);
      if(pc_rx[ii] != 0)
         pc_rx_found = pc_rx[ii];
      if (num_p == 2)
         pc_tx_found = pc_tx[ii];
      /* Set TRS mask field according to winfarm on which PC was found */
      /* Currently all TRS are available for WP3 */
      if((pc_rx[ii]!= 0)||
         ((num_p == 2)&&(pc_rx[ii]!= 0)))
         mask_temp->wf[ii] = 0xFFFF;
   }
   /* Return error if PCs not found */
   if ((pc_rx_found == 0xFFFF) || ((pc_tx_found == 0xFFFF) && (num_p == 2)))
   {
      return WPI_ERROR(WP_ERR_DPS_FEATURE_UNAVAILABLE);
   }

   return WP_OK;
}

/************************************************************************
 *
 * Function:  WPI_SaPctTxOffsetGet
 *
 * Purpose:   return the offset between pct entries for rx and tx
 *            when installing PCT entry only the rx entry is returned
 *            the tx entry is (rx_entry + WPI_SaPctTxOffsetGet())
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *            offset between the TX and RX
 *
 * Called by: many functions
 *
 ****************************************************************************/

WP_U8 WPI_SaPctTxOffsetGet(void)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_SA_PROFILE_ID_TX_OFFSET;
}

/************************************************************************
 *
 * Function:  WPI_SaPctInstall
 *
 * Purpose:  Install DPS feature in PC Table and return profile id
 *
 * Description:  Puts in a Feature (i.e. IMA, MLPPP) into the PCT table
 *               entry.
 *
 * Inputs:     wpid               Winpath ID
 *             protocol_type_rx   receive protocol type
 *             protocol_type_tx   transmit protocol type
 *             num_p              number of protocol types: 1 - rx, 2 - rx and tx
 *
 * Outputs:
 *             pct_entry_rx      a pointer to the PCT entry table and Ta order for rx
 *             pct_entry_tx      a pointer to the PCT entry table and Ta order for tx
 *                                privously named (pct_entry)
 *             trs_mask           TRS mask entry
 *
 * Return Value:
 *     WP_OK
 *
 * Called by: many functions
 *
 **************************************************************************/
WP_status WPI_SaPctInstall(WP_U32 wpid,
                           WP_U32 protocol_type_rx,
                           WP_U32 protocol_type_tx,
                           WP_U32 num_p,
                           WP_U8 serial_index,
                           WP_U32 *pct_entry_rx,
                           WP_U32 *pct_entry_tx,
                           wpi_trs_mask *trs_mask)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   WP_U32 ii = 0;
   WP_U16 pc_rx[WPI_WINFARM_N];
   WP_U16 pc_tx[WPI_WINFARM_N];
   WP_U16 *temp_pc_tx;
   wpi_res_sa *res_sa = NULL;
   wpi_trs_mask mask_temp;
   WP_U32 temp_pct_tx_entry;
   WP_U32 max_profiles;
   WP_U16 n_winfarm;

   wpi_res_trs_allocation *trs_allocation;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_RETURN_IF_ERROR(status, WPI_Limits(wpid, WPI_LIMITS_WINFARM, &n_winfarm));

   memset(&mask_temp, 0, sizeof(wpi_trs_mask));


   WPI_RETURN_IF_ERROR(status,
                       WPI_SaPctFind(wpid, protocol_type_rx,
                                     protocol_type_tx, num_p,
                                     pc_rx, pc_tx, &mask_temp));


   trs_allocation = wpi_registry_ptr->pwp_block[wpid].trs_allocation;
   switch(protocol_type_rx)
   {
      case WPI_DPS_ENTRYPT_TOKEN_THREAD:
         trs_allocation->trs_mask_token_thread = mask_temp;
         break;

      case WPI_DPS_ENTRYPT_TOKEN_THREAD_2:
         trs_allocation->trs_mask_token_thread_2 = mask_temp;
         break;

      default:
         break;
   }

   max_profiles = WPI_SaProfilesNumberGet();

   if(serial_index == WPI_HW_SERIAL_CFU_IDLE)
   {
      *pct_entry_rx = WPI_DEFAULT_CFU_IDLE_PROFILE;
   }
   else
   {
      for (ii = 0; ii < max_profiles; ii++)
      {
         if (! res_sa->aa_profiles_map[ii].initialized)
         {
            /*TDI_workaround*/
            if((protocol_type_tx != WPI_DPS_ENTRYPT_TDI_TX) && (ii <= 1))
               continue;
            break;
         }

         /* for CFU serials and TGU, Find if the serial has a PCT entry */
         if (serial_index < WPI_HW_SERIAL_TDI1_RX || serial_index > WPI_HW_SERIAL_TDI16_TX)
         {
            if (res_sa->aa_profiles_map[ii].serial_id == serial_index)
               break;
         }
         else /* TDI case */
         {
            /* because of all winfarms have the same PC entry
             * it is enough to check for winfarm 0 only */
            if (res_sa->aa_profiles_map[ii].pc[0] == pc_rx[0])
               break;
         }
      }

      if (ii == max_profiles)
         return WPI_ERROR(WP_ERR_SA_PCT_FULL);

      *pct_entry_rx = ii;  /* the next free profile id or a profile id that allocated for this serial before */
      if(num_p == 2)
         *pct_entry_tx = *pct_entry_rx + WPI_SaPctTxOffsetGet();
   }

   if (! res_sa->aa_profiles_map[*pct_entry_rx].initialized) /*for the first phy in this serial   */
   {
      /* Store PCs and serial ID's in the profile table */
      res_sa->aa_profiles_map[*pct_entry_rx].serial_id = serial_index;
      res_sa->aa_profiles_map[*pct_entry_rx].initialized = WP_TRUE;
      if (num_p == 2)
      {
         res_sa->aa_profiles_map[*pct_entry_tx].serial_id = serial_index + WPI_SaPctTxOffsetGet();
         res_sa->aa_profiles_map[*pct_entry_tx].initialized = WP_TRUE;
      }

      for (ii = 0; ii < n_winfarm; ii++)
      {
         res_sa->aa_profiles_map[*pct_entry_rx].pc[ii] = pc_rx[ii];
         if (num_p == 2)
            res_sa->aa_profiles_map[*pct_entry_tx].pc[ii] = pc_tx[ii];
      }

      /* Store profile ID's in the serial table */
      res_sa->serial_to_profile_id_map[serial_index] = *pct_entry_rx;
      if (num_p == 2)
         res_sa->serial_to_profile_id_map[serial_index + WPI_SaPctTxOffsetGet()] = *pct_entry_tx;
   }

   if(pct_entry_tx == 0)
   {
      temp_pc_tx = 0;
      temp_pct_tx_entry = 0xFFFFFFFF;
   }
   else
   {
      temp_pc_tx = res_sa->aa_profiles_map[*pct_entry_tx].pc;
      temp_pct_tx_entry = *pct_entry_tx;
   }

   /* set pct in the AA */
   WPI_RETURN_IF_ERROR(status,
                       WPI_AaProfilePcSet(wpid,
                                          res_sa->aa,
                                          res_sa->aa_profiles_map[*pct_entry_rx].pc,
                                          temp_pc_tx,
                                          *pct_entry_rx,
                                          temp_pct_tx_entry,
                                          num_p));
   if (trs_mask)
      memcpy(trs_mask, &mask_temp, sizeof(wpi_trs_mask));

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return WP_OK;
}

WP_status WPI_SaProfileIdGet(WP_U32 wpid,
                             WP_U32 protocol_type_rx,
                             WP_U32 protocol_type_tx,
                             WP_U32 num_p,
                             WP_U32 *profile_id_tx,
                             WP_U32 *profile_id_rx)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 ii, winfarm;
   WP_U16 pc_rx[WPI_WINFARM_N];
   WP_U16 pc_tx[WPI_WINFARM_N];
   wpi_res_sa *res_sa = NULL;
   WP_U32 max_profiles;
   wpi_trs_mask mask_temp;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   memset(&mask_temp, 0, sizeof(wpi_trs_mask));

   WPI_RETURN_IF_ERROR(status,
                       WPI_SaPctFind(wpid, protocol_type_rx,
                                     protocol_type_tx, num_p,
                                     pc_rx, pc_tx, &mask_temp));

   max_profiles = WPI_SaProfilesNumberGet();

   for (ii = 0 ; ii < max_profiles; ii++)
   {
      if( ii <= WPI_SA_TDI_TX_PCT_INDEX) //TDI-Workaround - PCT 0 and 1 are reserved for TDI
         continue;

      if (res_sa->aa_profiles_map[ii].initialized)
      {
         WP_boolean found_entry = WP_FALSE;

         /* TX */

         /* because of all winfarms have the same PC entry
          * it is enough to check for winfarm 0 only */
         if (res_sa->aa_profiles_map[ii].pc[0] == pc_tx[0])
         {
            found_entry = WP_TRUE;

            /* Check the other winfarms */
            for (winfarm = 1 ; winfarm < WPI_WINFARM_N ; winfarm++)
            {
               if (res_sa->aa_profiles_map[ii].pc[winfarm] != pc_tx[winfarm])
               {
                  found_entry = WP_FALSE;
                  break;
               }
            }
            if (found_entry)
               *profile_id_tx = ii;
         }

         /* Reset flag */
         found_entry = WP_FALSE;

         /* RX */

         /* because of all winfarms have the same PC entry
          * it is enough to check for winfarm 0 only */
         if (res_sa->aa_profiles_map[ii].pc[0] == pc_rx[0])
         {
            found_entry = WP_TRUE;

            /* Check the other winfarms */
            for (winfarm = 1 ; winfarm < WPI_WINFARM_N ; winfarm++)
            {
               if (res_sa->aa_profiles_map[ii].pc[winfarm] != pc_rx[winfarm])
               {
                  found_entry = WP_FALSE;
                  break;
               }
            }
            if (found_entry)
               *profile_id_rx = ii;
         }
      }
      else
         break;
   }

   if (ii == max_profiles)
      return WPI_ERROR(WP_ERR_SA_PCT_FULL);

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return WP_OK;
}


/************************************************************************
 *
 * Function:  WPI_WingineNumGet
 *
 * Purpose:   Returns the number of active wingines
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *     wpid   Winpath ID
 *
 * Return Value:
 *
 * Called by:
 *     WPI_SetTrsStl (wpi_hw_sa.c)
 *
 ****************************************************************************/

WP_U32 WPI_WingineNumGet (WP_U32 wpid)
{
   WP_U32 i;
   WP_U32 n_wingines = 0;
   WP_S32 present;
   WP_status status;

   WPI_INTERNAL_FUNCTION();

   for (i = 0; i < MAX_NUM_WINGINES; i++) {
      status = WPI_CapabilityReadIndexed(wpid, WPI_CAP_HW_WINGINE_AVAILABLE, i, &present);
      if (status == WP_OK && present) {
         n_wingines++;
      }
   }
   return n_wingines;
}

/************************************************************************
 *
 * Function:  WPI_TrsAllMaskGet
 *
 * Purpose:   Returns the mask of All TRS's according to active Wingines
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *     wpid   Winpath ID
 *
 * Return Value:
 *
 * Called by:
 *     WPI_SetTrsStl (wpi_hw_sa.c)
 *
 ****************************************************************************/

WP_status WPI_TrsAllMaskGet (WP_U32 wpid, WP_U32 *low_trs, WP_U32 *high_trs)
{
/*There are 12 wingines in WP3:
  3 wingines per winfarm.
  Winfarm 0 - trs 0 - 15 - wingines 0-2
  Winfarm 1 - trs 16 - 31 - wingines 3-5
  Winfarm 2 - trs 32 - 47 - wingines 6-8
  Winfarm 3 - trs 48 - 63 - wingines 9-11 */

   WP_U32 i;
   WP_U32 mask[2] = {0, 0};
   WP_S32 present[12];
   WP_status status;

   WPI_INTERNAL_FUNCTION();

   for (i = 0; i < MAX_NUM_WINGINES; i++) {
      WPI_RETURN_IF_ERROR(status,
                          WPI_CapabilityReadIndexed(wpid, WPI_CAP_HW_WINGINE_AVAILABLE,
                                                    i, &present[i]));
   }

   if (present[0] || present[1] || present[2]) mask[0] |= 0x0000ffff;
   if (present[3] || present[4] || present[5]) mask[0] |= 0xffff0000;
   if (present[6] || present[7] || present[8]) mask[1] |= 0x0000ffff;
   if (present[9] || present[10] || present[11]) mask[1] |= 0xffff0000;

   *low_trs = mask[0];
   *high_trs = mask[1];
   return WP_OK;
}

/************************************************************************
 *
 * Function:  WPI_WinMaskToTrsMask
 *
 * Purpose:   Translate an array of 2 16bit fields divided
 *            according to WinFarms to a 32 bit mask
 *
 * Description:
 *
 * Inputs:
 *     input_trs_mask   2 16 bit masks divided according to WinFarms
 *
 * Outputs:
 *     trs_mask       A 32 bit mask
 *
 * Return Value:
 *     WP_OK
 *     WP_ERR_TRS_SET_NOT_VALID_IN_CHIP_FAMILY -
 *                      the mask exceeds the TRS valid to this chip's configuration
 *                      (i.e. the chip has less then 4 wingines)
 *
 * Called by:
 *     WP_SaEntrySet (wpi_hw_sa.c)
 *
 ****************************************************************************/

WP_status WPI_WinMaskToTrsMask(WP_U32 wpid, wpi_aa_trs_mask *tm, wpi_trs_mask *input_trs_mask)
{
   WP_U32 all_mask_low, all_mask_high;
   WP_status status;

   WPI_INTERNAL_FUNCTION();

   all_mask_low = all_mask_high = 0;
   tm->trs_mask_low = WPI_AA_TRS_MASK_LOW(input_trs_mask);
   tm->trs_mask_high = WPI_AA_TRS_MASK_HIGH(input_trs_mask);

   WPI_RETURN_IF_ERROR(status,
                       WPI_TrsAllMaskGet(wpid, &all_mask_low, &all_mask_high));

   if ( (tm->trs_mask_low != (tm->trs_mask_low & all_mask_low)) ||
        (tm->trs_mask_high != (tm->trs_mask_high & all_mask_high)) )
      return WPI_ERROR(WP_ERR_TRS_SET_NOT_VALID_IN_CHIP_FAMILY);

   return WP_OK;
}

/************************************************************************
 *
 * Function:  WPI_SaStlSizeGet
 *
 * Purpose:   Returns the size of the STL
 *
 * Return Value:
 *     The size of the STL field
 *
 * Called by:
 *     various functions
 *
 ****************************************************************************/

WP_U32 WPI_SaStlSizeGet(void)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_AA_SERIALS_AND_AGENTS_NUMBER;
}

WP_U32 WPI_SaSerCntsSizeGet(void)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_AA_SERIALS_AND_AGENTS_NUMBER / 4;
}

/************************************************************************
 *
 * Function:  WPI_SaPctSizeGet
 *
 * Purpose:   Returns the size of the PCT
 * PCT table size: we need 16 bit per every winfarm for every profile.
 *                 In 32 bit register there is place for 2 winfarms.
 *
 *
 * Return Value:
 *     The size of the PCT field
 *
 * Called by:
 *     various functions
 *
 ****************************************************************************/

WP_U32 WPI_SaPctSizeGet(void)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_AA_PCT_SIZE;
}
/************************************************************************
 *
 * Function:  WPI_SaProfilesNumberGet
 *
 * Purpose:   Returns the size of the PCT
 *
 * Return Value:
 *     The size of the PCT field
 *
 * Called by:
 *     various functions
 *
 ****************************************************************************/

WP_U32 WPI_SaProfilesNumberGet(void)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_AA_PROFILE_ID_SIZE;
}

/************************************************************************
 *
 * Function:  WPI_SaRegBaseGet
 *
 * Purpose:   Returns the base address of the AA register
 *
 * Inputs:
 *     base   Winpath Register base address
 *
 * Return Value:
 *     The AA base address
 *
 * Called by:
 *     various functions
 *
 ****************************************************************************/

wpi_sa *WPI_SaRegBaseGet(WP_U8 *base)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_AaRegBaseGet(base);
}

/* the following functions used to be in the sa
   they are now passed as-is to the aa
*/
void WPI_SaRegisterGmrGet(wpi_sa *sa_reg_ptr, WP_U8 *enable, WP_U32 *lsa, WP_U8 *poll_method)
{
   WPI_INTERNAL_FUNCTION();
   WPI_AaRegisterGmrGet(sa_reg_ptr, enable, lsa, poll_method);
}

/************************************************************************
 *
 * Function:  WPI_IsSaEnabled
 *
 * Purpose:   Decide whether to execute the shutdown process
 *
 * Description:
 *
 * Inputs:
 *     wpid   Winpath ID
 *
 * Outputs:
 *
 * Return Value:
 *     WP_TRUE if the SA is enabled
 *     WP_FALSE if not
 *
 * Called by:
 *     WPI_OsShutdownWinpath (wpi_system_enet.c)
 *
 ****************************************************************************/
WP_boolean WPI_IsSaEnabled(WP_U32 wpid)
{
   wpi_res_reg_bases *res_reg_bases = NULL;
   WP_status lock_status = WP_OK;
   wpi_sa *sa_ptr;
   WP_U8 enable;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryRegisterBaseGet,
                           WPI_REG_LOCKED,
                           &res_reg_bases,
                           wpid);
   if (WP_ERROR_P(lock_status)) return WP_FALSE;
   sa_ptr = WPI_AaRegBaseGet(res_reg_bases->reg_bases_ptr);

   WPI_AaRegisterGmrGet(sa_ptr, &enable, NULL, NULL);

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryRegisterBaseRelease,
                               WPI_REG_UNLOCKED,
                               &res_reg_bases);
   if (WP_ERROR_P(lock_status)) return WP_FALSE;
   if (enable == WPI_SA_GMR_ENB_ENABLE)
      return WP_TRUE;
   else
      return WP_FALSE;
}

/************************************************************************
 *
 * Function:  WPI_SaRegisterWfCntsGet
 *
 * Purpose:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/
WP_U32 WPI_SaRegisterWfCntsGet(wpi_sa *sa_reg_ptr)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_AaRegisterWfCntsGet(sa_reg_ptr);
}

void WPI_SaRegisterStlGet(wpi_sa *sa_reg_ptr, WP_U32 polling_addr,
                          WP_U32 *trs_limit, wpi_trs_mask *trs_mask)
{
   WPI_INTERNAL_FUNCTION();
   WPI_AaRegisterStlGet(sa_reg_ptr, polling_addr, trs_limit, trs_mask);
   return;
}

WP_status WPI_SaRegisterStlSet(WP_U32 wpid,
                               wpi_sa *sa_reg_ptr, WP_U32 polling_addr,
                               WP_U32 trs_limit, wpi_trs_mask *trs_mask)
{
   wpi_aa_trs_mask aa_trs_mask;
   WP_status status;

   WPI_INTERNAL_FUNCTION();

   WPI_RETURN_IF_ERROR(status, WPI_WinMaskToTrsMask(wpid, &aa_trs_mask, trs_mask));

   WPI_AaRegisterStlSet(sa_reg_ptr, polling_addr, trs_limit, &aa_trs_mask);

   return WP_OK;
}

/************************************************************************
 *
 * Function:  WPI_SaRegisterPctGet
 *
 * Purpose:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/
WP_U16 WPI_SaRegisterPctGet(wpi_sa *sa_reg_ptr, WP_U32 pct_index)
{
   /* For now, function returnes profile for WINFARM1, assumption that there are
      same PCs for all WINFARMS*/
   WP_U16 pc[4];
   WPI_INTERNAL_FUNCTION();
   WPI_AaRegisterPctGet(sa_reg_ptr, pct_index, pc);
   return pc[0];
}

WP_U32 WPI_SaRegisterSerCntsGet(wpi_sa *sa_reg_ptr, WP_U32 index)
{
   WPI_INTERNAL_FUNCTION();
   return WPI_AaRegisterSerCntsGet(sa_reg_ptr, index);
}

/*****************************************************************************
 *
 * Function:     WPI_SaAaEntryConfigBuild
 *
 * Purpose:      build AA entry configuration
 *
 * Description:  read values from the registry and default values
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        port_index     the port to allocate
 *        resources     WDDI registry resource block
 *        direction     Rx, Tx
 *
 * Outputs:
 *        aa_entry_config    the built configuration
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_WDDI        illegal port
 *
 * Called by:
 *        WPI_SaPortCreate
 ****************************************************************************/

static WP_status WPI_SaAaEntryConfigBuild(WP_U32 wpid,
                                          wpi_res_sa* res_sa,
                                          WP_port port_index,
                                          wpi_resource_block *resources,
                                          WP_direction direction,
                                          wpi_aa_entry_config *aa_entry_config)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 emphy_upi_mode, upi_pkt_pg, emphy_mfr_mode;
   wpi_resource_entry *port_entry;
   wpi_res_tdi *res_tdi;
   wpi_res_upi *res_upi;
   WP_U32 iw_bkgnd;
   wpi_res_trs_allocation *trs_allocation;
   wpi_trs_mask *trs_allocation_mask;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* Get the trs_allocation registry entry. */
   trs_allocation = wpi_registry_ptr->pwp_block[wpid].trs_allocation;
   trs_allocation_mask = &(trs_allocation->trs_limit_all);
   aa_entry_config->trs_mask.trs_mask_high = WPI_AA_TRS_MASK_HIGH(trs_allocation_mask);
   aa_entry_config->trs_mask.trs_mask_low = WPI_AA_TRS_MASK_LOW(trs_allocation_mask);

   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
      {
         WP_U32 upimode, is_hspi;
         if(WPI_UpiParamsGet(wpid,
                             port_index,
                             &is_hspi,
                             NULL,
                             NULL,
                             &iw_bkgnd,
                             &upimode,
                             NULL,
                             NULL,
                             NULL) == WP_TRUE)
         {
            if (is_hspi)
            {
               if(direction == WPI_DIRECTION_RX)
               {
                  if (iw_bkgnd == WP_IW_BKGND_USED)
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_upi_hspi_rx;
                  }
                  else
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_upi_hspi_rx_iw;
                  }
               }
               else /* TX */
               {
                  aa_entry_config->max_trs = trs_allocation->trs_limit_upi_tx_pos;
               }
            }
            else
            {
               if(direction == WPI_DIRECTION_RX)
               {
                  if (iw_bkgnd == WP_IW_BKGND_USED)
                  {
                     if (upimode == WPI_UPI_HW_POS)
                     {
                        aa_entry_config->max_trs = trs_allocation->trs_limit_upi_rx_pos;

                        WPI_ACCESS_RES(port_entry, resources, port_index);
                        res_upi = WPI_RES_UPI(port_entry);

                        if(res_upi == NULL)
                           return WPI_ERROR(WP_ERR_WDDI);

                        if (res_upi->uses_emphy == WPI_UPI_EMPHY_MODE_UFE4 &&
                            res_upi->tx_buswidth == WPI_UPI_HW_BUS_32)
                        {
                           /* In UFE448 more TRS are needed in CR mode to forward timestamp from UFE to WP */
                           aa_entry_config->max_trs += 12;
                        }

                     }
                     else
                     {
                        aa_entry_config->max_trs =
                           trs_allocation->trs_limit_upi_rx_utopia;

                     }
                  }
                  else
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_upi_rx_iw;

                  }
               }
               else /* TX */
               {
                  upi_pkt_pg = pg_pkt[port_index];
                  emphy_upi_mode = WPI_PacketConfiguration(wpid, upi_pkt_pg,
                                                           WPI_PKT_CONFIG_UPI_EMPHY);
                  emphy_mfr_mode = WPI_PacketConfiguration(wpid, upi_pkt_pg,
                                                           WPI_PKT_CONFIG_MFR_EMPHY);
                  if (emphy_upi_mode && emphy_mfr_mode)
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_mfr_emphy_tx;
                  }
                  else
                  {
                     if (upimode == WPI_UPI_HW_POS)
                     {
                        aa_entry_config->max_trs = trs_allocation->trs_limit_upi_tx_pos;
                     }
                     else
                     {
                        aa_entry_config->max_trs = trs_allocation->trs_limit_upi_tx_utopia;
                     }
                  }
               }
            }
         }
         else
            return WPI_ERROR(WP_ERR_WDDI);
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
      {
         WP_U32 is_giga, is_sgmii_2500;
         if (WPI_EnetPortModeGet(wpid,
                                 port_index,
                                 &is_giga,
                                 &is_sgmii_2500,
                                 NULL,
                                 &iw_bkgnd) == WP_TRUE)
         {
            if (is_giga || is_sgmii_2500)
            {
               if(direction == WPI_DIRECTION_RX)
               {
                  if (iw_bkgnd == WP_IW_BKGND_USED)
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_genet_rx;
                  }
                  else
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_genet_rx_iw;
                  }
               }
               else if(direction == WPI_DIRECTION_RX_INTSW)
               {
                  aa_entry_config->max_trs = trs_allocation->trs_limit_genet_intsw;
               }
               else /* TX */
               {
                  aa_entry_config->max_trs = trs_allocation->trs_limit_genet_tx;
               }
            }
            else
            {
               if(direction == WPI_DIRECTION_RX)
               {
                  if (iw_bkgnd == WP_IW_BKGND_USED)
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_enet_rx;
                  }
                  else
                  {
                     aa_entry_config->max_trs = trs_allocation->trs_limit_enet_rx_iw;
                  }
               }
               else /* TX */
               {
                  aa_entry_config->max_trs = trs_allocation->trs_limit_enet_tx;
               }
            }
         }
         else
            return WPI_ERROR(WP_ERR_WDDI);
      }
      break;
      case WP_PORT_XGI1:
      case WP_PORT_XGI2:
      {
         if(direction == WPI_DIRECTION_RX)
         {
            aa_entry_config->max_trs = trs_allocation->trs_limit_xgi_rx;
         }
         else if(direction == WPI_DIRECTION_RX_INTSW)
         {
            aa_entry_config->max_trs = trs_allocation->trs_limit_xgi_intsw;
         }
         else
         {
            aa_entry_config->max_trs = trs_allocation->trs_limit_xgi_tx;
         }
      }
      break;
      case WP_PORT_TDM1:
      case WP_PORT_TDM2:
      case WP_PORT_TDM3:
      case WP_PORT_TDM4:
      case WP_PORT_TDM5:
      case WP_PORT_TDM6:
      case WP_PORT_TDM7:
      case WP_PORT_TDM8:
      case WP_PORT_TDM9:
      case WP_PORT_TDM10:
      case WP_PORT_TDM11:
      case WP_PORT_TDM12:
      case WP_PORT_TDM13:
      case WP_PORT_TDM14:
      case WP_PORT_TDM15:
      case WP_PORT_TDM16:
         WPI_ACCESS_RES(port_entry, resources, WPI_tdi_base + (port_index - WP_PORT_TDM1));
         if (WPI_INITIALIZED(port_entry->state))
         {
            if(direction == WPI_DIRECTION_RX)
            {
               res_tdi = WPI_RES_TDI(port_entry);
               if (res_tdi->iw_bkgnd == WP_IW_BKGND_USED)
               {
                  aa_entry_config->max_trs = trs_allocation->trs_limit_tdi_rx;
               }
               else
               {
                  aa_entry_config->max_trs = trs_allocation->trs_limit_tdi_rx_iw;
               }
            }
            else /* TX */
            {
               aa_entry_config->max_trs = trs_allocation->trs_limit_tdi_tx;
            }
         }
         else
            return WPI_ERROR(WP_ERR_WDDI);
         break;

      case WPI_PORT_VIRTUAL_LAG:
         aa_entry_config->max_trs = trs_allocation->trs_limit_lag;
         break;
      case WPI_PORT_VIRTUAL_EFM_BONDING_OVER_ENET:
         aa_entry_config->max_trs = trs_allocation->trs_limit_efm_bonding;
         break;

      case WPI_PORT_VIRTUAL_CES_SHAPING:
         aa_entry_config->max_trs = trs_allocation->trs_limit_ces_shaping;
         break;

      default:
         return WPI_ERROR(WP_ERR_UNIMPLEMENTED);
         break;
   }

   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaCfuRxAgentConfigBuild
 *
 * Purpose:    build cfu rx aa entry configuration
 *
 *
 * Description:  read values from the registry and default values
 *
 * Inputs:
 *        res_sa        the SA registry structure
 *        port_index     the port to allocate
 *
 * Outputs:
 *        cfu_rx_aa_entry_config    the built configuration
 *
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *        WPI_SaPortCreate
 ****************************************************************************/

static WP_status WPI_SaCfuRxAgentConfigBuild(wpi_res_sa* res_sa,
                                             WP_port port_index,
                                             wpi_cfu_rx_agent_config *cfu_rx_aa_entry_config,
                                             WP_boolean dynamic_entry)
{
   WP_U32 ii;
   /* get default values */
   wpi_rx_agent_pri_drop_th *pri_drop_th = &default_rx_agent_pri_drop_th[0];

   WPI_INTERNAL_FUNCTION();

   switch(port_index)
   {
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
         /* priority profile is defined in the wpi_hw_adv_user.h
            offset 1 is used because
            in the CFU, the thresholds are set for even priority,
            but in the adv_user file the thresholds are for odd priorities.
            in addition there are only 7 CFU thresholds, so priority 1 is
            the default until WPI_WINNET_RX_FIFO_SA_PRIOR3
         */
         cfu_rx_aa_entry_config->pri_prof.priority_offset = 1;
         cfu_rx_aa_entry_config->pri_prof.priority_th[0] = WPI_WINNET_RX_FIFO_SA_PRIOR3;
         cfu_rx_aa_entry_config->pri_prof.priority_th[1] = WPI_WINNET_RX_FIFO_SA_PRIOR5;
         cfu_rx_aa_entry_config->pri_prof.priority_th[2] = WPI_WINNET_RX_FIFO_SA_PRIOR7;
         cfu_rx_aa_entry_config->pri_prof.priority_th[3] = WPI_WINNET_RX_FIFO_SA_PRIOR9;
         cfu_rx_aa_entry_config->pri_prof.priority_th[4] = WPI_WINNET_RX_FIFO_SA_PRIOR11;
         cfu_rx_aa_entry_config->pri_prof.priority_th[5] = WPI_WINNET_RX_FIFO_SA_PRIOR13;
         cfu_rx_aa_entry_config->pri_prof.priority_th[6] = WPI_WINNET_RX_FIFO_SA_PRIOR15;

         /* pause settings */
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_high = WPI_UPI_RX_PAUSE_AGENT_FIFO_HIGH_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_low = WPI_ENET_RX_PAUSE_AGENT_FIFO_LOW_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_high =
            (pri_drop_th->int_non_1st_th[0]*32)+WPI_ENET_RX_PAUSE_AGENT_STOP_PAUSE_DELAY;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_low =
            pri_drop_th->int_non_1st_th[0]*32;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_high =
            WPI_ENET_RX_PAUSE_EXT_FIFO_HIGH_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_low =
            WPI_ENET_RX_PAUSE_EXT_FIFO_LOW_THRESHOLD;

         break;
      case WP_PORT_XGI1:
      case WP_PORT_XGI2:
         /* priority profile is defined in the wpi_hw_adv_user.h
            offset 1 is used because
            in the CFU, the thresholds are set for even priority,
            but in the adv_user file the thresholds are for odd priorities.
            in addition there are only 7 CFU thresholds, so priority 1 is
            the default until WPI_WINNET_RX_FIFO_SA_PRIOR3
         */
         cfu_rx_aa_entry_config->pri_prof.priority_offset = 1;
         cfu_rx_aa_entry_config->pri_prof.priority_th[0] = WPI_WINNET_RX_FIFO_SA_PRIOR3;
         cfu_rx_aa_entry_config->pri_prof.priority_th[1] = WPI_WINNET_RX_FIFO_SA_PRIOR5;
         cfu_rx_aa_entry_config->pri_prof.priority_th[2] = WPI_WINNET_RX_FIFO_SA_PRIOR7;
         cfu_rx_aa_entry_config->pri_prof.priority_th[3] = WPI_WINNET_RX_FIFO_SA_PRIOR9;
         cfu_rx_aa_entry_config->pri_prof.priority_th[4] = WPI_WINNET_RX_FIFO_SA_PRIOR11;
         cfu_rx_aa_entry_config->pri_prof.priority_th[5] = WPI_WINNET_RX_FIFO_SA_PRIOR13;
         cfu_rx_aa_entry_config->pri_prof.priority_th[6] = WPI_WINNET_RX_FIFO_SA_PRIOR15;

         /* pause settings */
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_high =
            WPI_XGI_RX_PAUSE_AGENT_FIFO_HIGH_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_low =
            WPI_XGI_RX_PAUSE_AGENT_FIFO_LOW_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_high =
            (pri_drop_th->int_non_1st_th[0]*32)+WPI_ENET_RX_PAUSE_AGENT_STOP_PAUSE_DELAY;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_low =
            pri_drop_th->int_non_1st_th[0]*32;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_high =
            WPI_ENET_RX_PAUSE_EXT_FIFO_HIGH_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_low =
            WPI_ENET_RX_PAUSE_EXT_FIFO_LOW_THRESHOLD;

         break;
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
         /* priority profile is defined in the wpi_hw_adv_user.h
            offset 1 is used because
            in the CFU, the thresholds are set for even priority,
            but in the adv_user file the thresholds are for odd priorities.
            in addition there are only 7 CFU thresholds, so priority 1 is
            the default until WPI_UPI_RX_FIFO_SA_PRIOR3
         */
         cfu_rx_aa_entry_config->pri_prof.priority_offset = 1;
         cfu_rx_aa_entry_config->pri_prof.priority_th[0] = WPI_UPI_RX_FIFO_SA_PRIOR3;
         cfu_rx_aa_entry_config->pri_prof.priority_th[1] = WPI_UPI_RX_FIFO_SA_PRIOR5;
         cfu_rx_aa_entry_config->pri_prof.priority_th[2] = WPI_UPI_RX_FIFO_SA_PRIOR7;
         cfu_rx_aa_entry_config->pri_prof.priority_th[3] = WPI_UPI_RX_FIFO_SA_PRIOR9;
         cfu_rx_aa_entry_config->pri_prof.priority_th[4] = WPI_UPI_RX_FIFO_SA_PRIOR11;
         cfu_rx_aa_entry_config->pri_prof.priority_th[5] = WPI_UPI_RX_FIFO_SA_PRIOR13;
         cfu_rx_aa_entry_config->pri_prof.priority_th[6] = WPI_UPI_RX_FIFO_SA_PRIOR15;

         /* pause settings */
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_high =
            WPI_UPI_RX_PAUSE_AGENT_FIFO_HIGH_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_low =
            WPI_UPI_RX_PAUSE_AGENT_FIFO_LOW_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_high =
            (pri_drop_th->int_non_1st_th[0]*32)+WPI_UPI_RX_PAUSE_AGENT_STOP_PAUSE_DELAY;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_low =
            pri_drop_th->int_non_1st_th[0]*32;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_high =
            WPI_UPI_RX_PAUSE_EXT_FIFO_HIGH_THRESHOLD;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_low =
            WPI_UPI_RX_PAUSE_EXT_FIFO_LOW_THRESHOLD;
         break;
      default:
      {
         wpi_rx_agent_pri_prof *pri_prof = &default_rx_agent_pri_prof[0];
         wpi_rx_agent_pause_settings *pause_settings = &default_wpi_rx_agent_pause_settings[0];
         /* default priority profile */
         cfu_rx_aa_entry_config->pri_prof.priority_offset = pri_prof->priority_offset;
         for(ii = 0 ; ii < WPI_CFU_RX_AGENT_PRIO_TH_N ; ++ii)
         {
            cfu_rx_aa_entry_config->pri_prof.priority_th[ii] = pri_prof->priority_th[ii];
         }

         /* pause settings */
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_high =
            pause_settings->agent_counter_th_high;
         cfu_rx_aa_entry_config->pause_settings.agent_counter_th_low =
            pause_settings->agent_counter_th_low;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_high =
            pause_settings->int_fbp_th_high;
         cfu_rx_aa_entry_config->pause_settings.int_fbp_th_low =
            pause_settings->int_fbp_th_low;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_high =
            pause_settings->dps_fbp_th_high;
         cfu_rx_aa_entry_config->pause_settings.dps_fbp_th_low =
            pause_settings->dps_fbp_th_low;
      }
   }

   cfu_rx_aa_entry_config->pri_drop_th.int_th_en = pri_drop_th->int_th_en;
   cfu_rx_aa_entry_config->pri_drop_th.ext_th_en = pri_drop_th->ext_th_en;
   /* priority drop thresholds (smart discard) */
   for(ii = 0 ; ii < WPI_CFU_RX_AGENT_DROP_TH_N ; ++ii)
   {
      cfu_rx_aa_entry_config->pri_drop_th.int_1st_th[ii] =
         pri_drop_th->int_1st_th[ii];
      cfu_rx_aa_entry_config->pri_drop_th.agent_1st_th[ii] =
         pri_drop_th->agent_1st_th[ii];

      if(dynamic_entry)
      {
         /* dynamic agents has no external therhsold and must not have 'non first' values*/
         cfu_rx_aa_entry_config->pri_drop_th.agent_1st_th[ii] = WPI_CFU_SMART_DROP_INTSW_AGENTQ_START_LEVEL/32;

         cfu_rx_aa_entry_config->pri_drop_th.int_non_1st_th[ii] = WPI_CFU_SMART_DROP_NO_FBP_DROP;
         cfu_rx_aa_entry_config->pri_drop_th.agent_non_1st_th[ii] = WPI_CFU_SMART_DROP_NO_QUEUE_DROP_8BITS;

         cfu_rx_aa_entry_config->pri_drop_th.ext_1st_th[ii] = WPI_CFU_SMART_DROP_NO_FBP_DROP;
         cfu_rx_aa_entry_config->pri_drop_th.ext_non_1st_th[ii] = WPI_CFU_SMART_DROP_NO_FBP_DROP;
      }
      else
      {
         cfu_rx_aa_entry_config->pri_drop_th.int_non_1st_th[ii] =
            pri_drop_th->int_non_1st_th[ii];
         cfu_rx_aa_entry_config->pri_drop_th.agent_non_1st_th[ii] =
            pri_drop_th->agent_non_1st_th[ii];

         cfu_rx_aa_entry_config->pri_drop_th.ext_1st_th[ii] =
            pri_drop_th->ext_1st_th[ii];
         cfu_rx_aa_entry_config->pri_drop_th.ext_non_1st_th[ii] =
            pri_drop_th->ext_non_1st_th[ii];
      }
   }

   return WP_OK;
}

/*****************************************************************************
 *
 * Function:     WPI_SaCfuRxEntryConfigBuild
 *
 * Purpose:      build cfu rx entry configuration
 *
 * Description:  read values from the registry and default values
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        device_index  the index of the device entry in the registry
 *
 * Outputs:
 *        cfu_rx_entry_config    the built configuration
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_WDDI   problem in getting device properties from registry
 *
 * Called by:
 *        WPI_SaDeviceCreate
 ****************************************************************************/

static WP_status WPI_SaCfuRxEntryConfigBuild(WP_U32 wpid,
                                             wpi_res_sa* res_sa,
                                             wpi_res_device *res_dev,
                                             wpi_cfu_rx_entry_config *cfu_rx_entry_config)
{
   WP_status status = WP_OK;
   WP_U32 port_index;
   WP_U32 upimode;
   WP_U32 profile_id_rx;
   WP_U32 profile_id_tx;
   wpi_trs_mask mask;
   WP_U32 protocol_type_rx, protocol_type_tx;

   WPI_INTERNAL_FUNCTION();

   port_index = WPI_HANDLE_INDEX(res_dev->port);

   cfu_rx_entry_config->user_defined = 0;

   cfu_rx_entry_config->pce_user_defined[0] = 0;
   cfu_rx_entry_config->pce_user_defined[1] = 0;
   cfu_rx_entry_config->pce_user_defined[2] = 0;
   cfu_rx_entry_config->pce_user_defined[3] = 0;

   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
      {
         if(WPI_UpiParamsGet(wpid,
                             port_index,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             &upimode,
                             NULL,
                             &protocol_type_rx,
                             &protocol_type_tx) == WP_FALSE)
            return WPI_ERROR( WP_ERR_WDDI);

         WPI_RETURN_IF_ERROR(
            status,
            WPI_SaPctInstall(wpid,
                             protocol_type_rx,
                             protocol_type_tx,
                             2,
                             WPI_HW_SERIAL_UPI1_RX + (2 *(port_index - WP_PORT_UPI1)),
                             &profile_id_rx, &profile_id_tx, &mask));

         cfu_rx_entry_config->profile_id = profile_id_rx;

         if(upimode == WPI_UPI_HW_POS)
         {
            /*Added to support ATM OVER POS (per phy)
             * At ATM OVER POS, an ATM device can be created on WPI_UPI_HW_POS
             *take appropriate action for each one  */
            if (res_dev->devtype != WP_DEVICE_ATM)
            {
               wpi_res_device_pkt *pkt_device;
               pkt_device = (wpi_res_device_pkt *)res_dev;
               cfu_rx_entry_config->rx_stats_en =
                  (pkt_device->rx_statmode == WP_PKT_STAT_ENABLE ? WP_TRUE : WP_FALSE);
               switch(pkt_device->device.devtype)
               {
                  case WP_DEVICE_HSPOS:
                     /* l2pi device */
                     cfu_rx_entry_config->du_size = WPI_CFU_DU_SIZE_512;
                     break;
                  default:
                     cfu_rx_entry_config->du_size = WPI_CFU_DU_SIZE_64;
               }
            }
            else /*ATM Device*/
            {
               wpi_res_device_atm *atm_device;
               atm_device = (wpi_res_device_atm *)res_dev;
               cfu_rx_entry_config->rx_stats_en =
                  (atm_device->atm_ext_phy_stats == WP_ATM_EXT_PHY_STATS_ENABLE ?
                   WP_TRUE :
                   WP_FALSE);
               cfu_rx_entry_config->du_size = WPI_CFU_DU_SIZE_64;
            }
         }
         else /* atm  UTOPIA*/
         {
            wpi_res_device_atm *atm_device;
            atm_device = (wpi_res_device_atm *)res_dev;
            cfu_rx_entry_config->rx_stats_en =
               (atm_device->atm_ext_phy_stats == WP_ATM_EXT_PHY_STATS_ENABLE ?
                WP_TRUE :
                WP_FALSE);
            cfu_rx_entry_config->du_size = WPI_CFU_DU_SIZE_64;
         }
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
      {
         WP_U32 prot_rx, prot_tx;
         wpi_res_device_hspi_enet *enet_device;
         WP_S32 pce_via_cfu_value;

         enet_device = (wpi_res_device_hspi_enet *)res_dev;
         if (enet_device->enet_g999_used)
         {
            /* G999 Physical device or Enet stream or Debug stream. */
            /* G999  configured over L2PI CB code */
            prot_rx = WPI_DPS_ENTRYPT_L2PI_RX_G999;
            prot_tx = WPI_DPS_ENTRYPT_L2PI_TX_G999;
         }
         else
         {
            /* if PCE Workaround enabled change set the specific DPS entry point */
            WPI_CapabilityRead(wpid, WPI_CAP_HW_CFU_PCE_INTERFACE, &pce_via_cfu_value);
            if(pce_via_cfu_value)
               prot_rx = WPI_DPS_ENTRYPT_L2PI_RX;
            else
               prot_rx = WPI_DPS_ENTRYPT_L2PI_RX_PCE_WA;

            if(WPI_HwL2piUsesPB(wpid, WPI_HANDLE_INDEX(res_dev->device_handle)))
            {
               prot_tx = WPI_DPS_ENTRYPT_L2PI_TX_PB;
            }
            else
            {
               prot_tx = WPI_DPS_ENTRYPT_L2PI_TX_CB;
            }
         }
         /* Install DPS features in PCT and return PCT entry */
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaPctInstall(wpid,
                                              prot_rx,
                                              prot_tx,
                                              2,
                                              WPI_HW_SERIAL_ENET1_RX +
                                              (2 *(port_index - WP_PORT_ENET1)),
                                              &profile_id_rx, &profile_id_tx, &mask));

         cfu_rx_entry_config->profile_id = profile_id_rx;
         cfu_rx_entry_config->rx_stats_en =
            (enet_device->phystatmode == WP_ENET_STAT_ENABLE ? WP_TRUE : WP_FALSE);

         cfu_rx_entry_config->du_size = WPI_CFU_DU_SIZE_512;
      }
      break;

      default:
         return WPI_ERROR(WP_ERR_UNIMPLEMENTED);
   }

   /* pce settings */
   cfu_rx_entry_config->pce_settings.pce_mode = WPI_CFU_PMODE_NO_PCE;
   cfu_rx_entry_config->pce_settings.parser_start = 0;
   cfu_rx_entry_config->pce_settings.pce_set_index = 0;
   cfu_rx_entry_config->pce_settings.pce_priority = 0;

   return status;
}

/*****************************************************************************
 *
 * Function:     WPI_SaCfuTxEntryConfigBuild
 *
 * Purpose:      build cfu tx entry configuration
 *
 * Description:  read values from the registry and default values
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        device_index  the index of the device entry in the registry
 *
 * Outputs:
 *        cfu_tx_entry_config    the built configuration
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_WDDI   problem in getting device properties from registry
 *
 * Called by:
 *        WPI_SaDeviceCreate, WPI_SaMultiClassDeviceCreate
 ****************************************************************************/

static WP_status WPI_SaCfuTxEntryConfigBuild(WP_U32 wpid,
                                             wpi_res_sa* res_sa,
                                             wpi_res_device *res_dev,
                                             wpi_cfu_tx_entry_config *cfu_tx_entry_config)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block * resources;
   wpi_resource_entry *port_entry;
   WP_U32 port_index;
   WP_U32 profile_id_rx = 0;
   WP_U32 profile_id_tx = 0;
   wpi_trs_mask mask;
   WP_U32 protocol_type_rx, protocol_type_tx;
   wpi_res_upi *res_upi;
   WP_U32 wait_value;
   WP_U32 unitsize;
   WP_U32 sysclk;
#if _WP_MODULE_CLOCK_RECOVERY_ENABLE_
   WP_U8 socket_mode = 0;
#endif

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   sysclk = wpi_registry_ptr->sysclk;
   cfu_tx_entry_config->user_defined = 0;

   /* requests_limit */
   cfu_tx_entry_config->requests_limit = WPI_CFU_TX_REQUESTS_LIMIT_MAX;

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   /* dev_index passed in here is the index of the device entry in the
      registry */
   port_index = WPI_HANDLE_INDEX(res_dev->port);

   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
      {
         WP_U32 support_atm;
         WP_U32 upimode;
         WP_U32 tx_phy_mode;

         WPI_ACCESS_RES(port_entry, resources, port_index);
         res_upi = WPI_RES_UPI(port_entry);
         if(WPI_UpiParamsGet(wpid,
                             port_index,
                             NULL,
                             NULL,
                             &support_atm,
                             NULL,
                             &upimode,
                             &tx_phy_mode,
                             &protocol_type_rx,
                             &protocol_type_tx) == WP_FALSE)
            return WPI_ERROR( WP_ERR_WDDI);

         WPI_RETURN_IF_ERROR(
            status,
            WPI_SaPctInstall(wpid,
                             protocol_type_rx,
                             protocol_type_tx,
                             2,
                             WPI_HW_SERIAL_UPI1_RX + (2 *(port_index - WP_PORT_UPI1)),
                             &profile_id_rx, &profile_id_tx, &mask));

         cfu_tx_entry_config->profile_id = profile_id_tx;

         /* wait value  - in this stage configured to positive value,
            if cfu port uses tod, this value will be set to 0 */
         if (support_atm)
            unitsize = 53 * 8;
         else
            unitsize = 64 * 8;

         wait_value = wpi_32x32_32(sysclk, unitsize, res_dev->tx_bit_rate);

         cfu_tx_entry_config->wait_entry =
            WPI_CfuTxWaitEntryGet(res_sa->cfu, wait_value);

         if(upimode == WPI_UPI_HW_POS)
         {
            /*Added to support ATM OVER POS (per phy)
             * At ATM OVER POS, an ATM device can be created on WPI_UPI_HW_POS
             *take appropriate action for each one  */
            if (res_dev->devtype != WP_DEVICE_ATM)
            {
               wpi_res_device_pkt *pkt_device;
               pkt_device = (wpi_res_device_pkt *)res_dev;
               cfu_tx_entry_config->tx_stats_en =
                  (pkt_device->tx_statmode == WP_PKT_STAT_ENABLE ? WP_TRUE : WP_FALSE);
               switch(pkt_device->device.devtype)
               {
                  case WP_DEVICE_HSPOS:
                     /* l2pi device */
                     cfu_tx_entry_config->du_size = WPI_CFU_DU_SIZE_512;
                     break;

                  default:

#if _WP_MODULE_CLOCK_RECOVERY_ENABLE_
                     WPI_RETURN_IF_ERROR(status,WPI_RegistryIwModesCheckCesopClockRecSocketModeEnabled(wpid,&socket_mode));

                     if(socket_mode  == WP_TRUE)
                     {
                        cfu_tx_entry_config->du_size = WPI_CFU_DU_SIZE_128;
                     }
                     else
#endif  /* _WP_MODULE_CLOCK_RECOVERY_ENABLE_  */
                        cfu_tx_entry_config->du_size = WPI_CFU_DU_SIZE_64;
               }
            }
            else /*ATM device*/
            {
               wpi_res_device_atm *atm_device;
               atm_device = (wpi_res_device_atm *)res_dev;
               cfu_tx_entry_config->tx_stats_en =
                  (atm_device->atm_ext_phy_stats == WP_ATM_EXT_PHY_STATS_ENABLE ?
                   WP_TRUE :
                   WP_FALSE);
               /* for atm tx_start_th is set to 0 */
               cfu_tx_entry_config->tx_start_th = WPI_CFU_TX_START_TH_0_8;
               cfu_tx_entry_config->du_size = WPI_CFU_DU_SIZE_64;
            }
         }
         else /* atm UTOPIA*/
         {
            wpi_res_device_atm *atm_device;
            atm_device = (wpi_res_device_atm *)res_dev;
            cfu_tx_entry_config->tx_stats_en =
               (atm_device->atm_ext_phy_stats == WP_ATM_EXT_PHY_STATS_ENABLE ?
                WP_TRUE :
                WP_FALSE);
            /* for atm tx_start_th is set to 0 */
            cfu_tx_entry_config->tx_start_th = WPI_CFU_TX_START_TH_0_8;
            cfu_tx_entry_config->du_size = WPI_CFU_DU_SIZE_64;
         }
         cfu_tx_entry_config->priority_profile_entry = WPI_CFU_UPI_TX_PROFILE_ENTRY;
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
      {
         WP_U32 prot_rx, prot_tx;
         wpi_res_device_hspi_enet *enet_device;
         WP_S32 pce_via_cfu_value;

         enet_device = (wpi_res_device_hspi_enet *)res_dev;

         prot_rx = WPI_DPS_ENTRYPT_L2PI_RX;

         if (enet_device->enet_g999_used)
         {
            /* G999 Physical device or Enet stream or Debug stream. */
            /* G999  configured over L2PI CB code */
            prot_rx = WPI_DPS_ENTRYPT_L2PI_RX_G999;
            prot_tx = WPI_DPS_ENTRYPT_L2PI_TX_G999;
            cfu_tx_entry_config->du_size = WPI_CFU_DU_SIZE_512;
         }
         else
         {
            /* if PCE Workaround enabled change set the specific DPS entry point */
            WPI_CapabilityRead(wpid, WPI_CAP_HW_CFU_PCE_INTERFACE, &pce_via_cfu_value);
            if(pce_via_cfu_value)
               prot_rx = WPI_DPS_ENTRYPT_L2PI_RX;
            else
               prot_rx = WPI_DPS_ENTRYPT_L2PI_RX_PCE_WA;

            if(WPI_HwL2piUsesPB(wpid, WPI_HANDLE_INDEX(res_dev->device_handle)))
            {
               prot_tx = WPI_DPS_ENTRYPT_L2PI_TX_PB;
               cfu_tx_entry_config->du_size = enet_device->hspi.pkt.tx_maxsdu/WPI_CFU_TX_BUFFER_SIZE;
            }
            else
            {
               prot_tx = WPI_DPS_ENTRYPT_L2PI_TX_CB;
               cfu_tx_entry_config->du_size = WPI_CFU_DU_SIZE_512;
            }
         }

         /* Install DPS features in PCT and return PCT entry */
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaPctInstall(wpid,
                                              prot_rx,
                                              prot_tx,
                                              2,
                                              WPI_HW_SERIAL_ENET1_RX +
                                              (2 *(port_index - WP_PORT_ENET1)),
                                              &profile_id_rx, &profile_id_tx, &mask));
         cfu_tx_entry_config->profile_id = profile_id_tx;

         /* wait entry  - in this stage configured to positive value,
            if cfu port uses tod, this value will be set to 0 */
         cfu_tx_entry_config->wait_entry =
            WPI_CfuTxWaitEntryGet(res_sa->cfu,
                                  WPI_WINNET_POLLING_SPEED);

         cfu_tx_entry_config->tx_stats_en =
            (enet_device->phystatmode == WP_ENET_STAT_ENABLE ? WP_TRUE : WP_FALSE);

         cfu_tx_entry_config->priority_profile_entry = WPI_CFU_ENET_TX_PROFILE_ENTRY;

         if(cfu_tx_entry_config->intsw_tx_config != NULL)
         {
            /* if the intsw_tx_config field is allocated, then it is an intsw entry, and a dedicated
             * pct entry should be created and connected to the agent */
            prot_rx = WPI_DPS_ENTRYPT_INTSW;
            prot_tx = 0;
            /* Install DPS features in PCT and return PCT entry */
            WPI_RETURN_IF_ERROR(status,
                                WPI_SaPctInstall(wpid,
                                                 prot_rx,
                                                 prot_tx,
                                                 1,
                                                 WPI_HW_SERIAL_ENET1_INTSW +
                                                 (port_index - WP_PORT_ENET1),
                                                 &profile_id_rx, &profile_id_tx, &mask));
            cfu_tx_entry_config->intsw_tx_config->profile_id = profile_id_rx;

         }
      }
      break;
      default:
         return WPI_ERROR(WP_ERR_UNIMPLEMENTED);
   }

   /* the tod source will be set after the allocation */
   cfu_tx_entry_config->tod_input_number = WPI_CFU_TX_CFUPORT_NO_TOD;
   return status;
}

/*****************************************************************************
 *
 * Function:      WPI_SaCfuTxEntryActiveRequestsWorkaroundAlloc
 *
 * Purpose:       Set the use of the Workaround if needed
 *
 * Description:   this function checks if the device is exposed to the
 *                CFU FIFO override problem, and updates the required FIFO size
 *                for the workaround (active requests initial value).
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *        WPI_SaDeviceCreate
 ****************************************************************************/
static WP_status WPI_SaCfuTxEntryActiveRequestsWorkaroundAlloc(WP_U32 wpid,
                                                               WP_port port_index,
                                                               wpi_res_device *res_dev,
                                                               wpi_cfu_tx_entry* cfu_tx_entry,
                                                               wpi_cfu_tx_entry_config* config,
                                                               WP_U16* buffers_num)
{

   /* setting the active requests initial value is only relevant to ENET ports */
   switch(port_index)
   {
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
      {
         /* the workaround is only relevant to higher then 1Gbps interfaces */

         WP_U32 is_sgmii_2500, is_giga;
         wpi_res_device_hspi_enet *enet_device;

         enet_device = (wpi_res_device_hspi_enet *)res_dev;
         if (enet_device->enet_g999_used)
            break;

         WPI_EnetPortModeGet(wpid,
                             port_index,
                             &is_giga,
                             &is_sgmii_2500,
                             NULL,
                             NULL);
         if( ! (is_sgmii_2500 || is_giga) )
         {
            break;
         }
      }
      case WP_PORT_XGI1:
      case WP_PORT_XGI2:
      {
         if(!(WPI_HwL2piUsesPB(wpid, WPI_HANDLE_INDEX(res_dev->device_handle))))
         {
            /* the workaround is only relevant when l2pi_cb.dpl module is used */
            cfu_tx_entry->configuration.active_requests_initial_value = CFU_TX_CFUPORT_ACTIVE_REQUESTS_INITIAL_VALUE_WA;
            config->active_requests_initial_value = CFU_TX_CFUPORT_ACTIVE_REQUESTS_INITIAL_VALUE_WA;
            /* the enum of 512bytes is equal to 7, and we want a value of 8 */
            *buffers_num += CFU_TX_CFUPORT_ACTIVE_REQUESTS_INITIAL_VALUE_WA*(config->du_size + 1);
         }
         break;
      }
      default:
         /* do nothing */
         break;
   }

   return WP_OK;

} /* WPI_SaCfuTxEntryActiveRequestsWorkaroundAlloc */

/*****************************************************************************
 *
 * Function:      WPI_SaUpiAlloc
 *
 * Purpose:       do allocation of the upi aa and cfu entries
 *
 * Description:   go through all the ports and devices and allocate the used
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        resources     WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *        WPI_AllocSa
 ****************************************************************************/

static WP_status WPI_SaUpiAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources)
{
   WP_U32 ii, jj, agents_num;
   WP_U32 num_of_phys, max_phy_num;
   wpi_resource_entry *port_entry;
   wpi_res_upi *res_upi;
   wpi_aa_entry *aa_rx_entry, *aa_tx_entry;
   WP_U32 index, upi_port, is_emphy;
   wpi_cfu_rx_entry *cfu_rx_entry;
   wpi_cfu_tx_entry *cfu_tx_entry;
   wpi_res_device *res_dev;
   WP_status status = WP_OK;

   WPI_INTERNAL_FUNCTION();
   index = WPI_upi_device_base;
   upi_port = WP_PORT_UPI1;

   /* for each upi port in use */
   for (ii = WPI_upi_base; ii < WPI_upi_base + WPI_HwUpiMaxNumGet();
        ii++, index += WPI_upi_device_offset, upi_port++)
   {
      WPI_ACCESS_RES(port_entry, resources, ii);
      if (WPI_RES_ENTRY_INITIALIZED(port_entry))
      {
         res_upi = WPI_RES_UPI(port_entry);
         max_phy_num = res_upi->max_phy_index;
         num_of_phys = res_upi->num_of_phys;

         /* create the ports AA entries */
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaPortCreate(wpid,
                                               upi_port,
                                               WPI_DIRECTION_DUPLEX,
                                               &aa_rx_entry,
                                               &aa_tx_entry));

         /* allocate Aa entry (block of agents on an agent group) */
         /* rx agents can be used with any number of phys */
         agents_num = 1;
         WPI_RETURN_IF_ERROR( status,
                              WPI_AaEntryAlloc(res_sa->aa,
                                               aa_rx_entry,
                                               agents_num, 0));
         /* set allocation in the cfu */
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuRxAaEntryAllocSet(res_sa->cfu,
                                                       res_sa->aa,
                                                       aa_rx_entry));
         agents_num = WPI_DIV_ROUND_UP(num_of_phys + 1, WPI_CFU_TX_CFUPORTS_PER_AGENT);
         /* get the upi port's emphy mode */
         if(WPI_UpiParamsGet(wpid,
                             ii,
                             NULL,
                             &is_emphy,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL) == WP_FALSE)
            is_emphy = WP_FALSE;


         if(is_emphy)
         {
            /* Only 1 agent needed for emphy port*/
            agents_num = 1;
            WPI_RETURN_IF_ERROR( status,
                                 WPI_AaEntryAlloc(res_sa->aa,
                                                  aa_tx_entry,
                                                  agents_num, 0));

         }
         else
         {
            if(res_upi->demandmode)
            {
               WPI_RETURN_IF_ERROR( status,
                                    WPI_AaEntryAlloc(res_sa->aa,
                                                     aa_tx_entry,
                                                     agents_num, 0));
            }
            else
            {
               WPI_RETURN_IF_ERROR( status,
                                    WPI_AaEntryAlloc(res_sa->aa,
                                                     aa_tx_entry,
                                                     agents_num, 1));
            }
         }

         /* allocate cfuports from the agents' cfuports pool */
         /* should be consecutive calls for consecutive phys of the same serial */
         if (is_emphy)
         {

            res_upi->ufe_phy_device = (wpi_res_device*) WPI_WDDI_MALLOC(sizeof(wpi_res_device));
            if (!(res_upi->ufe_phy_device))
               return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);

            memset(res_upi->ufe_phy_device, 0, sizeof(wpi_res_device));

            /* set the device type to WP_DEVICE_EMPHY */
            res_upi->ufe_phy_device->devtype = WP_DEVICE_EMPHY;
            res_upi->ufe_phy_device->port = WPI_MAKE_HANDLE(WPI_handle_port, WPI_PORT_UPI, wpid, upi_port);
            max_phy_num = 0;
         }

         for(jj = 0 ; jj <= max_phy_num ; ++jj)
         {
            if (is_emphy)
            {
               res_dev = res_upi->ufe_phy_device;
            }
            else
            {
               wpi_resource_entry  *dev_entry;

               WPI_ACCESS_RES(dev_entry, resources, index + jj);
               if (!WPI_RES_ENTRY_INITIALIZED(dev_entry))
                  continue;

               res_dev = WPI_RES_DEV(dev_entry);

            }
            /* create the device's CFU entries */
            WPI_RETURN_IF_ERROR( status,
                                 WPI_SaDeviceAllocate(res_dev));

            WPI_RETURN_IF_ERROR( status,
                                 WPI_SaDeviceCreate(wpid,
                                                    res_dev,
                                                    WPI_DIRECTION_DUPLEX,
                                                    &cfu_rx_entry,
                                                    &cfu_tx_entry));

            /* store the entries pointers in the registry for future usage*/

            WPI_RETURN_IF_ERROR( status,
                                 WPI_SaCfuRxEntrySet(res_dev,
                                                     cfu_rx_entry));

            WPI_RETURN_IF_ERROR( status,
                                 WPI_SaCfuTxEntrySet(res_dev,
                                                     cfu_tx_entry));

            /* do allocation */
            WPI_RETURN_IF_ERROR( status,
                                 WPI_CfuRxEntryAlloc(res_sa->cfu,
                                                     aa_rx_entry,
                                                     cfu_rx_entry,
                                                     WP_FALSE));

            WPI_RETURN_IF_ERROR( status,
                                 WPI_CfuTxEntryAlloc(res_sa->cfu,
                                                     aa_tx_entry,
                                                     cfu_tx_entry));
         }
         if (is_emphy)
         {
            res_sa->cfu->cfu_tx->upi_emphy[upi_port].emphy_en = WP_TRUE;
            res_sa->cfu->cfu_tx->upi_emphy[upi_port].cfuport_tx_number =
               cfu_tx_entry->allocation.cfu_port_num;
         }
      }
   }
   return status;
}

/*****************************************************************************
 *
 * Function:      WPI_SaLagAlloc
 *
 * Purpose:       do allocation of the LAG aa and cfu entries
 *
 * Description:   go through all the ports and devices and allocate the used
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        resources     WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *        WPI_AllocSa
 ****************************************************************************/

static WP_status WPI_SaLagAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_U32 agents_num;
   wpi_aa_entry *aa_tx_entry;

   wpi_cfu_tx_entry *cfu_tx_entry;
   wpi_link_aggregation_functions *res_la = NULL;
   WP_status status = WP_OK;
   WP_U32 group_index = 0;
   WP_U16 tx_buff;
   wpi_cfu_tx_entry_config cfu_tx_entry_config;
   WP_U32 prot_tx;
   wpi_trs_mask mask;
   WP_U32 profile_id_tx;
   wpi_res_device_hspi_enet *res_dev;
   WP_status lock_status = WP_OK;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   memset(&cfu_tx_entry_config, 0, sizeof(wpi_cfu_tx_entry_config));

   if (! WPI_RegistryResourceEntryValidCheck(wpid, WPI_link_aggregation) )
   {
      return WP_OK;
   }

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryLinkAggregationFunctionsGet,
                           WPI_REG_LOCKED,
                           &res_la,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);



   /* create the ports AA entries */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaPortCreate(wpid,
                                         WPI_PORT_VIRTUAL_LAG,
                                         WP_DIRECTION_TX,
                                         NULL,
                                         &aa_tx_entry));
   agents_num = 1;
   WPI_RETURN_IF_ERROR( status,
                        WPI_AaEntryAlloc(res_sa->aa,
                                         aa_tx_entry,
                                         agents_num, WP_FALSE /* TOD */));

   for (group_index = 0; group_index < res_la->get_num_of_groups(res_la); group_index++)
   {
      res_dev = res_la->get_dev_of_group(wpid,group_index,wpi_registry_ptr);

      WPI_RETURN_IF_ERROR(status,
                          WPI_SaDeviceAllocate((wpi_res_device *)res_dev));


      /* create entry */
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuTxEntryCreate(res_sa->cfu,
                                                WPI_CFU_SERIAL_VIRTUAL_LAG,
                                                group_index, /* PHY Number */
                                                &cfu_tx_entry));

      tx_buff = WPI_LA_MAX_TX_BUFF;

      /* add request for buffers number */
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryBuffersRequest(res_sa->cfu,
                                                       cfu_tx_entry,
                                                       tx_buff));
      /* build configured values */
      prot_tx = WPI_DPS_ENTRYPT_LAG;
      cfu_tx_entry_config.du_size = WPI_CFU_DU_SIZE_64;

      /* Install DPS features in PCT and return PCT entry */
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaPctInstall(wpid,
                                           prot_tx,
                                           0,
                                           1,
                                           WPI_HW_SERIAL_LAG,
                                           &profile_id_tx,
                                           NULL,
                                           &mask));

      cfu_tx_entry_config.profile_id = profile_id_tx;

      /* wait entry  - in this stage configured to positive value,
         if cfu port uses tod, this value will be set to 0 */
      cfu_tx_entry_config.wait_entry =
         WPI_CfuTxWaitEntryGet(res_sa->cfu,
                               WPI_WINNET_POLLING_SPEED);

      cfu_tx_entry_config.tx_stats_en = WP_DISABLE;
      cfu_tx_entry_config.priority_profile_entry = WPI_CFU_ENET_TX_PROFILE_ENTRY;
      cfu_tx_entry_config.requests_limit = WPI_CFU_TX_REQUESTS_LIMIT_MAX;

      /* the tod source will be set after the allocation */
      cfu_tx_entry_config.tod_input_number = WPI_CFU_TX_CFUPORT_NO_TOD;

      /* set configuration */
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuTxEntryConfig(res_sa->cfu,
                                                cfu_tx_entry,
                                                &cfu_tx_entry_config));


      WPI_RETURN_IF_ERROR( status,
                           WPI_SaCfuTxEntrySet((wpi_res_device *)res_dev,
                                               cfu_tx_entry));

      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuTxEntryAlloc(res_sa->cfu,
                                               aa_tx_entry,
                                               cfu_tx_entry));

   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryLinkAggregationFunctionsRelease,
                               WPI_REG_UNLOCKED,
                               &res_la);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

#if _WP_MODULE_CLOCK_RECOVERY_ENABLE_

/*****************************************************************************
 *
 * Function:      WPI_SaCESShapingAlloc
 *
 * Purpose:       do allocation of the TSSU aa and cfu entries
 *
 * Description:
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        resources     WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *        WP_SysCommit->WPI_HwResourceAllocate->WPI_AllocSa )
 ****************************************************************************/

static WP_status WPI_SaCESShapingAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources)
{
   WP_U32 agents_num;
   wpi_aa_entry *aa_tx_entry;

   wpi_cfu_tx_entry *cfu_tx_entry;
   WP_status status = WP_OK;
   WP_U16 tx_buff;
   wpi_cfu_tx_entry_config cfu_tx_entry_config = {0};
   wpi_trs_mask mask;
   WP_U32 profile_id;
   wpi_i_sched *sched;
   WP_U32  group_index = 0;
   wpi_scheduler *res_sched;
   wpi_ces_shaping_sched_ext *ces_shaping_sched_ext;
   WP_boolean ces_shaping_present = WP_FALSE;
   WPI_INTERNAL_FUNCTION();

   /* Get scheduler registry */
   res_sched = WPI_RegistryScheduler(wpid);



   /* Go over all sched units to see if  CES SHAPING is present */
   for(sched = res_sched->first_sched; sched; sched = sched->next_sched)
   {
      if(sched->sched_type == WPI_SCHED_TYPE_PSU && sched->extended_params == WPI_SCHED_TYPE_EXT_CES_SHAPING)
      {
         ces_shaping_present = WP_TRUE;
      }
   }

   if(ces_shaping_present == WP_FALSE)
      return WP_OK;


   /* create the ports AA entries (inc trs allocate in WPI_SaAaEntryConfigBuild) */
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaPortCreate(wpid,
                                         WPI_PORT_VIRTUAL_CES_SHAPING,  /* (WP_PORT_LAST + 2)  */
                                         WP_DIRECTION_TX,
                                         NULL,
                                         &aa_tx_entry));

   /* Using one agent & port for all scheds (check it works for more than 1 TBD) */
   agents_num = 1;
   WPI_RETURN_IF_ERROR( status,
                        WPI_AaEntryAlloc(res_sa->aa,
                                         aa_tx_entry,
                                         agents_num, WP_FALSE /* TOD */));


   /* Go over all sched units and configure the CES SHAPING ones. */
   for (sched = res_sched->first_sched; sched; sched = sched->next_sched)
   {
      if (sched->sched_type == WPI_SCHED_TYPE_PSU && sched->extended_params == WPI_SCHED_TYPE_EXT_CES_SHAPING)
      {
         ces_shaping_sched_ext =  (wpi_ces_shaping_sched_ext *)sched->extended_structure;


         /*  Allocate Device */
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaDeviceAllocate(&ces_shaping_sched_ext->dev_virtual));


         /* create entry */
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuTxEntryCreate(res_sa->cfu,
                                                   WPI_CFU_SERIAL_VIRTUAL_CES_SHAPING,
                                                   group_index, /* PHY Number */
                                                   &cfu_tx_entry));
         group_index++;

         tx_buff = WPI_CES_SHAPING_MAX_TX_BUFF;

         /* add request for buffers number */
         WPI_RETURN_IF_ERROR(status,
                             WPI_CfuTxEntryBuffersRequest(res_sa->cfu,
                                                          cfu_tx_entry,
                                                          tx_buff));


         cfu_tx_entry_config.du_size = WPI_CFU_DU_SIZE_64;   /* minimal since not really using the DU */



         /* Install DPS features in PCT and return PCT entry */
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaPctInstall(wpid,
                                              WPI_DPS_ENTRYPT_CES_SHAPING,
                                              0,
                                              1,
                                              WPI_HW_SERIAL_CES_SHAPING,
                                              &profile_id,
                                              NULL,
                                              &mask));

         cfu_tx_entry_config.profile_id = profile_id;

         /* wait entry  - in this stage configured to positive value,
            if cfu port uses tod, this value will be set to 0 */
         cfu_tx_entry_config.wait_entry =
            WPI_CfuTxWaitEntryGet(res_sa->cfu,
                                  WPI_WINNET_POLLING_SPEED);

         cfu_tx_entry_config.tx_stats_en = WP_DISABLE;
         cfu_tx_entry_config.priority_profile_entry = WPI_CFU_ENET_TX_PROFILE_ENTRY;
         cfu_tx_entry_config.requests_limit = WPI_CFU_TX_REQUESTS_LIMIT_MAX;


         /* the tod source will be set after the allocation (in scheduler alloc) */
         cfu_tx_entry_config.tod_input_number = WPI_CFU_TX_CFUPORT_NO_TOD;

         /* set configuration */
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuTxEntryConfig(res_sa->cfu,
                                                   cfu_tx_entry,
                                                   &cfu_tx_entry_config));

         WPI_RETURN_IF_ERROR( status,
                              WPI_SaCfuTxEntrySet(&ces_shaping_sched_ext->dev_virtual,
                                                  cfu_tx_entry));

         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuTxEntryAlloc(res_sa->cfu,
                                                  aa_tx_entry,
                                                  cfu_tx_entry));



      }
   }


   return status;
}

#endif  /* _WP_MODULE_CLOCK_RECOVERY_ENABLE_  */


/*****************************************************************************
 *
 * Function:   WPI_SaEnetMultiClassAlloc
 *
 * Purpose:    do allocation of cfu entries for multiclass port
 *
 * Description: go through all ports, devices and classes and allocate entries
 *
 * Inputs:
 *             wpid           Winpath ID
 *             port_index     the index of the port entry in the registry
 *             aa_tx_entry    AA tx entry of this port
 *             aa_rx_entry    AA rx entry of this port
 *
 * Outputs:    CFU entries are created and configured
 *
 * Return Value:
 *
 * Called by: WPI_SaEnetAlloc
 *
 ****************************************************************************/

WP_status WPI_SaEnetMultiClassAlloc(WP_U32 wpid,
                                    wpi_res_sa* res_sa,
                                    WP_U32 dev_index,
                                    wpi_res_device_hspi_enet *physical_dev_enet,
                                    wpi_aa_entry *aa_rx_entry,
                                    wpi_aa_entry *aa_tx_entry,
                                    wpi_aa_entry *aa_tx_intsw_entry)
{
   WP_status status, lock_status = WP_OK;
   wpi_aa_entry *aa_rx_dynamic_entry;

   wpi_res_enet_multi_class *res_enet_multi_class = NULL;
   wpi_res_enet_multi_class_port *enet_multi_class_port_entry;
   wpi_res_enet_multi_class_device *res_class;
   wpi_cfu_tx_entry *physical_dev_cfu_tx_entry = NULL;
   wpi_cfu_tx_entry *class_cfu_tx_entry = NULL;
   WP_direction dev_direction;

   WP_U32 port_index = WPI_HANDLE_INDEX(physical_dev_enet->hspi.pkt.device.port);
   WP_U32 class_index, agents_num;
   WP_U16 allocation_phy_number = 0;
   WP_boolean allocate_extra_cfu_tx_port = WP_FALSE; /* G999<-->ENET */
   WPI_INTERNAL_FUNCTION();

   /* Allocate another tx device for a case of a possible
    * swap between a G999 and an Enet physical device */
   if (physical_dev_enet->swap_g999_with_enet_mode == WP_TRUE)
   {
      allocate_extra_cfu_tx_port = WP_TRUE;
   }
   else
   {
      allocate_extra_cfu_tx_port = WP_FALSE;
   }
   dev_direction = WPI_DIRECTION_TX;

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryEnetMultiClassGet,
                           WPI_REG_LOCKED,
                           &res_enet_multi_class,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   enet_multi_class_port_entry = res_enet_multi_class->port[port_index - WPI_enet_base];

   if (allocate_extra_cfu_tx_port == WP_TRUE)
   {
      if ( ! ( (dev_index - WPI_enet_device_base) % 2) )
      {
         /* allocate the GROUP A CFU Tx port before the multi-class CFU Tx ports allocation.  */
         /* In case we would want to switch from G999 port to Enet
          * (G999<-->ENET Swap feature), a cfu_port is reserved first
          * for the physical device */
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaDeviceCreate(wpid,
                                                 &( physical_dev_enet->hspi.pkt.device ),
                                                 WPI_DIRECTION_TX,
                                                 NULL,
                                                 &physical_dev_cfu_tx_entry));
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaCfuTxEntrySet(&( physical_dev_enet->hspi.pkt.device ),
                                                  physical_dev_cfu_tx_entry));

         /* do tx allocation */
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuTxEntryAlloc(res_sa->cfu,
                                                  aa_tx_entry,
                                                  physical_dev_cfu_tx_entry));
         /* reset the set_tx_map entry for this port, since it will be initialized as a G999 port
          * at SysCommit.
          * this is done so that the cfuport_tx_map table will be initialized correctly.
          * because the phy numbers of the multi-class devices start at zero as well, so
          * we need the ser_tx_map entry of this port to be initialized to the CFU port
          * of the first multi-class device. */
         enet_multi_class_port_entry->original_ser_tx_map =
            res_sa->cfu->cfu_tx->ser_tx_map[physical_dev_cfu_tx_entry->serial_num];
         res_sa->cfu->cfu_tx->ser_tx_map[physical_dev_cfu_tx_entry->serial_num] = CFU_SER_MAP_ENTRY_UNUSED;
      }
   }

   for(class_index = enet_multi_class_port_entry->start_index;
       class_index <= enet_multi_class_port_entry->end_index;
       class_index++)
   {
      res_class = enet_multi_class_port_entry->multi_class_dev[class_index];
      if(res_class)
      {
         allocation_phy_number = class_index - enet_multi_class_port_entry->start_index;
         /*allocate registry structure for class*/
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaDeviceAllocate(&(res_class->pkt.device)));

         /* In virtual device, the cfu rx is handled by the main (physical) device,
          *  so both should point to the same cfu_rx_entry*/

         ((wpi_res_sa_device *)res_class->pkt.device.hardware_specific)->cfu_rx_entry =
            ((wpi_res_sa_device *)physical_dev_enet->hspi.pkt.device.hardware_specific)->cfu_rx_entry;

         /* create the CFU TX entries and configure it */
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaMultiClassDeviceCreate(wpid,
                                                           physical_dev_enet,
                                                           allocation_phy_number,
                                                           res_class,
                                                           &class_cfu_tx_entry));

         /* do tx allocation */
         if(res_class->pkt.device.devtype == WP_DEVICE_ENET_MULTI_CLASS_INTSW)
         {
            /* special care for internal switching class */

            /*******************************************/
            /* creating the dedicated RX dynamic agent */
            /*******************************************/
            WPI_RETURN_IF_ERROR( status,
                                 WPI_SaPortCreate(wpid,
                                                  port_index,
                                                  WPI_DIRECTION_RX_INTSW,
                                                  &aa_rx_dynamic_entry,
                                                  NULL));

            /* allocate Aa entry (block of agents on an agent group) */
            /* rx agents can be used with any number of phys
               so 1 agent is enough
            */
            agents_num = 1;
            WPI_RETURN_IF_ERROR( status,
                                 WPI_AaEntryAlloc(res_sa->aa,
                                                  aa_rx_dynamic_entry,
                                                  agents_num, 0));

            /* set aa allocation in the cfu */
            WPI_RETURN_IF_ERROR( status,
                                 WPI_CfuRxAaEntryAllocSet(res_sa->cfu,
                                                          res_sa->aa,
                                                          aa_rx_dynamic_entry));


            /*******************************************************************************/
            /* allocate the TX cfu port over the tx agent including all the intsw TX ports */
            /*******************************************************************************/

            WPI_RETURN_IF_ERROR( status,
                                 WPI_CfuTxEntryAlloc(res_sa->cfu,
                                                     aa_tx_intsw_entry,
                                                     class_cfu_tx_entry));

            WPI_RETURN_IF_ERROR( status,
                                 WPI_CfuRxAaIntswEntryAllocSet(res_sa->cfu,
                                                               class_cfu_tx_entry,
                                                               aa_rx_dynamic_entry));
            /* an internal switching block list always holds a single agent (the list
             * length is 1) */
            res_class->intsw_dynamic_agent_index = aa_rx_dynamic_entry->allocation.block_list->start_index;
            res_class->intsw_cfu_tx_port = aa_rx_dynamic_entry->dynamic_settings->cfu_tx_entry->allocation.cfu_port_num;
         }
         else
         {
            WPI_RETURN_IF_ERROR( status,
                                 WPI_CfuTxEntryAlloc(res_sa->cfu,
                                                     aa_tx_entry,
                                                     class_cfu_tx_entry));
         }

         /* store the tx entry pointer in the registry for future usage*/
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaCfuTxEntrySet(&res_class->pkt.device,
                                                  class_cfu_tx_entry));


         if(res_class->shaped_class)
         {
            WPI_RETURN_IF_ERROR( status,
                                 WPI_CsuEntryAlloc(res_sa->csu,
                                                   class_cfu_tx_entry->serial_num,
                                                   class_cfu_tx_entry->phy_num,
                                                   &res_class->csu_shaper_params) );

            WPI_RETURN_IF_ERROR( status,
                                 WPI_CsuEntrySet(res_sa,
                                                 res_class->csu_shaper_params.shaper_index,
                                                 &res_class->csu_shaper_params) );
         }
      }
   }

   if (allocate_extra_cfu_tx_port == WP_TRUE)
   {
      if ( (dev_index - WPI_enet_device_base) % 2)
      {
         /* allocate the GROUP B CFU Tx port after the multi-class CFU Tx ports allocation.  */
         /* In case we would want to switch from G999 port to Enet
          * (G999<-->ENET Swap feature), we reserve the last cfu_port
          * to the physical device */
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaDeviceCreate(wpid,
                                                 &( physical_dev_enet->hspi.pkt.device ),
                                                 WPI_DIRECTION_TX,
                                                 NULL,
                                                 &physical_dev_cfu_tx_entry));
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaCfuTxEntrySet(&( physical_dev_enet->hspi.pkt.device ),
                                                  physical_dev_cfu_tx_entry));
         enet_multi_class_port_entry->original_ser_tx_map =
            res_sa->cfu->cfu_tx->ser_tx_map[physical_dev_cfu_tx_entry->serial_num];
         res_sa->cfu->cfu_tx->ser_tx_map[physical_dev_cfu_tx_entry->serial_num] = CFU_SER_MAP_ENTRY_UNUSED;
         /* do tx allocation */
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuTxEntryAlloc(res_sa->cfu,
                                                  aa_tx_entry,
                                                  physical_dev_cfu_tx_entry));
         res_sa->cfu->cfu_tx->ser_tx_map[physical_dev_cfu_tx_entry->serial_num] =
            enet_multi_class_port_entry->original_ser_tx_map;
      }
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryEnetMultiClassRelease,
                               WPI_REG_UNLOCKED,
                               &res_enet_multi_class);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   return WP_OK;
} /* WPI_SaEnetMultiClassAlloc */

/*****************************************************************************
 *
 * Function:      WPI_SaEnetAlloc
 *
 * Purpose:       do allocation of the enet aa and cfu entries
 *
 * Description:   go through all the ports and devices and allocate the used
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        resources     WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *        WPI_AllocSa
 ****************************************************************************/

static WP_status WPI_SaEnetAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources)
{
   WP_U32 ii;
   WP_U8 agents_num;
   wpi_resource_entry *port_entry,*dev_entry;
   wpi_res_enet *res_enet;
   WP_U32 port_index, dev_index, abs_port_index;
   wpi_enet_ports_devs_types ports_devs_types;
   WP_sa_serial serial_name_tx;
   WP_boolean doubling_trs_number_needed = WP_FALSE;

   wpi_aa_entry *aa_rx_entry = NULL, *aa_tx_entry = NULL;
   wpi_aa_entry *aa_tx_intsw_entry = NULL; /* this entry holds all the IntSw TX CFU ports */
   wpi_aa_entry *aa_last_entry_fast= NULL, *aa_last_entry_giga=NULL;
   wpi_aa_entry *aa_last_entry_xgi_multi_class=NULL, *aa_last_entry_enet_multi_class=NULL;
   wpi_cfu_rx_entry *cfu_rx_entry;
   wpi_cfu_tx_entry *cfu_tx_entry;
   wpi_res_device *res_dev;
   wpi_res_device_hspi_enet *dev_enet;

   WP_status status = WP_OK;

   WPI_INTERNAL_FUNCTION();
   memset(&ports_devs_types, 0, sizeof(wpi_enet_ports_devs_types));
   WPI_EnetDevsNum(wpid, &ports_devs_types);
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaAgentsAllocPerEnetPorts(&ports_devs_types,
                                                      res_sa));
   /* alloc aa entries for ports */
   for (port_index = WPI_enet_base;
        port_index < WPI_enet_base + WPI_ENET_N+WPI_XGI_N;
        port_index++)
   {
      abs_port_index = port_index - WPI_enet_base;
      WPI_ACCESS_RES(port_entry, resources, port_index);
      if (WPI_RES_ENTRY_INITIALIZED(port_entry))
      {
         res_enet = WPI_RES_ENET(port_entry);

         /* create the ports AA entries */

         /* In ENET WPI_SaPortCreate function called separately for RX and TX,
          * because of in TX sometimes we need allocate one agent
          * per several wddi ports according to
          * ports_devs_types.aa_agent_allocation_needed parameter,
          * that have been calculated
          * at WPI_SaAgentsAllocPerEnetPorts function*/

         /* RX TREATMENT */
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaPortCreate(wpid,
                                               port_index,
                                               WPI_DIRECTION_RX,
                                               &aa_rx_entry,
                                               NULL));

         /* allocate Aa entry (block of agents on an agent group) */
         /* rx agents can be used with any number of phys
            so 1 agent is enough
         */
         agents_num = 1;
         WPI_RETURN_IF_ERROR( status,
                              WPI_AaEntryAlloc(res_sa->aa,
                                               aa_rx_entry,
                                               agents_num, 0));

         /* set aa allocation in the cfu */
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuRxAaEntryAllocSet(res_sa->cfu,
                                                       res_sa->aa,
                                                       aa_rx_entry));

         /*TX TREATMENT*/

    	 /* if internal switching classes are available, a dedicated tx agent should
    	  * be created, so all intsw classes are located on consequant cfu tx ports.
    	  * the aa entry should only be created once */
         if(res_enet->enable_multiclass)
         {
            if(ports_devs_types.internal_switching_num_of_classes > 0)
            {
               if(aa_tx_intsw_entry == NULL)
               {
                  WPI_RETURN_IF_ERROR( status,
                                       WPI_SaPortCreate(wpid,
                                                        port_index,
                                                        WPI_DIRECTION_TX,
                                                        NULL,
                                                        &aa_tx_intsw_entry));
                  if (aa_tx_intsw_entry == NULL)
                     return WPI_ERROR(WP_ERR_NULL_PTR);

                  aa_tx_intsw_entry->intsw_type = WPI_AA_INTSW_TX_GLOBAL_ENTRY;
                  WPI_RETURN_IF_ERROR( status,
                                       WPI_AaEntryAlloc(res_sa->aa,
                                                        aa_tx_intsw_entry,
                                                        agents_num, 1)); /* no TOD */

                  /* overwrite the type of the aa entry to TX_INTSW */

               }
            }
         }

         if(ports_devs_types.aa_agent_allocation_needed[abs_port_index])
         {
            WPI_RETURN_IF_ERROR( status,
                                 WPI_SaPortCreate(wpid,
                                                  port_index,
                                                  WPI_DIRECTION_TX,
                                                  NULL,
                                                  &aa_tx_entry));
            if (aa_tx_entry == NULL)
               return WPI_ERROR(WP_ERR_NULL_PTR);

            /* currently 1 to 1 relation between wddi port and aa agent
               unless enet multiclass is enabled */
            if(res_enet->enable_multiclass)
            {
               /* Either 1 or 2 aa tx agents are needed, depending on the number of multi-class devices used. */
               agents_num = ports_devs_types.agents_num[abs_port_index];
            }
            else
               agents_num = 1;

            WPI_RETURN_IF_ERROR( status,
                                 WPI_AaEntryAlloc(res_sa->aa,
                                                  aa_tx_entry,
                                                  agents_num, 0));

            /* update pointer to last entry for ports without agent allocation*/

            /* If 2 XGI ports configured at multiclass mode, second port needs not
               allocation, so aa_entry should be saved  */
            if(WPI_INDEX_XGI_PORT_P(port_index) &&
               ports_devs_types.xgi_multi_class_ports_num == 2)
            {
               aa_last_entry_xgi_multi_class = aa_tx_entry;
            }

            /* Saving last created entry per winnet ports*/
            if(!(WPI_INDEX_XGI_PORT_P(port_index)))
            {

               /* if 2 enet ports of winnet configured at multiclass mode, second port
                  needs not allocation, so aa_entry should be saved */

               if(WPI_MULTICLASS_ENABLED(
                     ports_devs_types.port_multi_classes_num[abs_port_index]) &&
                  ( abs_port_index+1 < WPI_HwEnetMaxNumGet() ) &&
                  ( (abs_port_index + 1) < ((WPI_ENET_N) + (WPI_XGI_N)) ) &&
                  WPI_MULTICLASS_ENABLED(
                     ports_devs_types.port_multi_classes_num[abs_port_index+1] ))
               {
                  aa_last_entry_enet_multi_class = aa_tx_entry;
               }

               if(ports_devs_types.winnet_port_bitrate[abs_port_index]==
                  WPI_ENET_BITRATE_10_100)
               {
                  aa_last_entry_fast = aa_tx_entry;
                  doubling_trs_number_needed = WP_TRUE;
               }
               if (ports_devs_types.winnet_port_bitrate[abs_port_index]==
                   WPI_ENET_BITRATE_1000)
               {
                  aa_last_entry_giga = aa_tx_entry;
               }
            }
         }
         else /* allocation not needed, used allocated aa_tx_entry*/
         {
            if(WPI_INDEX_XGI_PORT_P(port_index) &&
               ports_devs_types.xgi_multi_class_ports_num == 2)
            {
               aa_tx_entry=aa_last_entry_xgi_multi_class;

               if (aa_tx_entry == NULL)
                  return WPI_ERROR(WP_ERR_WDDI);

               WPI_AaDoubleTrsNumber(aa_tx_entry);
            }
            else if(WPI_MULTICLASS_ENABLED(
                       ports_devs_types.port_multi_classes_num[abs_port_index])
                    &&
                    abs_port_index > 0 &&
                    WPI_MULTICLASS_ENABLED(
                       ports_devs_types.port_multi_classes_num[abs_port_index-1]))
            {
               /* NOTE: this code can only be reached for Group B since this code
                * can only be reached when aa_agent_allocation_needed == WP_FALSE,
                * and this can only be in a group B, since we will unite the
                * TxAgents of Group B with Group A  */
               aa_tx_entry = aa_last_entry_enet_multi_class;

               if (aa_tx_entry == NULL)
                  return WPI_ERROR(WP_ERR_WDDI);

               WPI_AaDoubleTrsNumber(aa_tx_entry);
            }
            else if(ports_devs_types.winnet_port_bitrate[abs_port_index]==
                    WPI_ENET_BITRATE_10_100)
            {
               aa_tx_entry=aa_last_entry_fast;

               if (aa_tx_entry == NULL)
                  return WPI_ERROR(WP_ERR_WDDI);

               /* For fast enet ports we need to double trs number only once
                  for every agent, without reference to number of ports, connected
                  to agent, so doubling_trs_number_needed flag used */
               if(doubling_trs_number_needed == WP_TRUE)
               {
                  WPI_AaDoubleTrsNumber(aa_tx_entry);
                  doubling_trs_number_needed = WP_FALSE;
               }
            }
            else
            {
               aa_tx_entry=aa_last_entry_giga;

               if (aa_tx_entry == NULL)
                  return WPI_ERROR(WP_ERR_WDDI);

               WPI_AaDoubleTrsNumber(aa_tx_entry);
            }
            serial_name_tx = WP_SA_ENET1_TX + ((port_index - WP_PORT_ENET1) * 2);
            res_sa->serial_to_entry_map[serial_name_tx] = aa_tx_entry;
         } /* else (ports_devs_types.aa_agent_allocation_needed[abs_port_index]) */

         /* allocate cfuports for devices */
         /* should be consecutive calls for consecutive phys of the same serial */
         for (ii = WPI_HwEnetDeviceFirst(port_index);
              ii < WPI_HwEnetMaxDeviceNumber(port_index);
              ii += WPI_HwEnetDeviceOffset(port_index))
         {
            dev_index = WPI_enet_device_base + ii;

            WPI_ACCESS_RES(dev_entry, resources, dev_index);
            if (WPI_RES_ENTRY_INITIALIZED(dev_entry))
            {
               /* RX Allocation */

               res_dev = WPI_RES_DEV(dev_entry);

               if(WPI_HANDLE_INDEX(res_dev->port) != port_index)
                  return WPI_ERROR(WP_ERR_WDDI);

               dev_enet = WPI_RES_DEV_HSPI_ENET(dev_entry);

               /* Allocate the Device */
               WPI_RETURN_IF_ERROR( status,
                                    WPI_SaDeviceAllocate(res_dev));

               /* create the CFU RX entry and configure it */
               WPI_RETURN_IF_ERROR( status,
                                    WPI_SaDeviceCreate(wpid,
                                                       res_dev,
                                                       WPI_DIRECTION_RX,
                                                       &cfu_rx_entry,
                                                       NULL));

               /* store the rx entry pointer in the registry for future usage*/
               WPI_RETURN_IF_ERROR( status,
                                    WPI_SaCfuRxEntrySet(res_dev,
                                                        cfu_rx_entry));

               /* do rx allocation */
               WPI_RETURN_IF_ERROR( status,
                                    WPI_CfuRxEntryAlloc(res_sa->cfu,
                                                        aa_rx_entry,
                                                        cfu_rx_entry,
                                                        dev_enet->rx_intsw_traffic_mode));

               if(res_enet->enable_multiclass)
               {
                  WPI_RETURN_IF_ERROR( status,
                                       WPI_SaEnetMultiClassAlloc(wpid,
                                                                 res_sa,
                                                                 dev_index,
                                                                 dev_enet,
                                                                 aa_rx_entry,
                                                                 aa_tx_entry,
                                                                 aa_tx_intsw_entry));
               }
               else
               {
                  /* create the CFU TX entry and configure it */
                  WPI_RETURN_IF_ERROR( status,
                                       WPI_SaDeviceCreate(wpid,
                                                          res_dev,
                                                          WPI_DIRECTION_TX,
                                                          NULL,
                                                          &cfu_tx_entry));

                  WPI_RETURN_IF_ERROR( status,
                                       WPI_SaCfuTxEntrySet(res_dev,
                                                           cfu_tx_entry));

                  /* do tx allocation */
                  WPI_RETURN_IF_ERROR( status,
                                       WPI_CfuTxEntryAlloc(res_sa->cfu,
                                                           aa_tx_entry,
                                                           cfu_tx_entry));
                  /*TBD loopback treatment*/
                  if(dev_enet->tx_timestamp == WP_ENABLE)
                  {
                     WPI_RETURN_IF_ERROR(status,
                                         WPI_TsLoopbackDeviceAlloc(wpid,
                                                                   res_sa->cfu,
                                                                   dev_enet,
                                                                   port_index,
                                                                   aa_rx_entry));
                  }
               }
            } /* if (WPI_RES_ENTRY_INITIALIZED(dev_entry)) */
         } /* for (ii = WPI_HwEnetDeviceFirst(port_index); */
      } /* if (WPI_RES_ENTRY_INITIALIZED(port_entry)) */
   } /* for (port_index = WPI_enet_base; */
   return status;
}

/*****************************************************************************
 *
 * Function:      WPI_SaEfmBondingAlloc
 *
 * Purpose:       Do the allocations for EFM Bonding AA and CFU entries
 *
 * Description:   go through all the ports and devices and allocate the used
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        resources     WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *        WPI_AllocSa
 ****************************************************************************/
static WP_status WPI_SaEfmBondingAlloc(WP_U32 wpid, wpi_res_sa * res_sa, wpi_resource_block * resources)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK,status = WP_OK;
   wpi_aa_entry *aa_tx_entry;
   WP_U16 tx_buff=0;
   WP_U32 agents_num,group_index=0,prot_tx,profile_id_tx;
   wpi_res_device_hspi_enet *res_dev=NULL;
   wpi_cfu_tx_entry *cfu_tx_entry;
   wpi_cfu_tx_entry_config cfu_tx_entry_config;
   wpi_trs_mask mask;

   wpi_efm_bonding_functions *res_efm=NULL;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   memset(&cfu_tx_entry_config, 0, sizeof(wpi_cfu_tx_entry_config));

   if(! WPI_RegistryResourceEntryValidCheck(wpid,WPI_efm_bonding_over_enet))
   {
      return WP_OK;
   }

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryEfmBondingEnetFunctionsGet,
                           WPI_REG_LOCKED,
                           &res_efm,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);


   /*Create the ports AA entries*/
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaPortCreate(wpid,
                                        WPI_PORT_VIRTUAL_EFM_BONDING_OVER_ENET,
                                        WP_DIRECTION_TX,
                                        NULL,
                                        &aa_tx_entry));

   if(((res_efm->get_num_of_groups(res_efm)) / WPI_CFU_TX_CFUPORTS_PER_AGENT) >= 1)
      agents_num = WPI_DIV_ROUND_UP(res_efm->get_num_of_groups(res_efm), WPI_CFU_TX_CFUPORTS_PER_AGENT);
   else
      agents_num = 1;

   WPI_RETURN_IF_ERROR(status,
                       WPI_AaEntryAlloc(res_sa->aa,
                                        aa_tx_entry,
                                        agents_num,
                                        WP_FALSE /* TOD */));

   for(group_index=0;group_index<res_efm->get_num_of_groups(res_efm);group_index++)
   {

      res_dev = res_efm->get_dev_of_group(wpid,group_index,wpi_registry_ptr);

      WPI_RETURN_IF_ERROR(status,
                          WPI_SaDeviceAllocate((wpi_res_device *) res_dev));

      /*Create entry*/
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryCreate(res_sa->cfu,
                                               WP_SA_EFM_BONDING_ENET,
                                               group_index,
                                               &cfu_tx_entry));

      tx_buff = WPI_CFU_TX_BUFFERS_EFM_BONDING;
      /*Add request for buffers*/
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryBuffersRequest(res_sa->cfu,
                                                       cfu_tx_entry,
                                                       tx_buff));

      prot_tx = WPI_DPS_ENTRYPT_EFM_BONDING_ENET;
      //prot_tx = WPI_DPS_ENTRYPT_EFM_BOND_RX_TGU;
      cfu_tx_entry_config.du_size = WPI_CFU_DU_SIZE_512;

      /*Install DPS feature in PCT and return PCT entry*/
      WPI_RETURN_IF_ERROR(status,
                          WPI_SaPctInstall(wpid,
                                           prot_tx,
                                           0,
                                           1,
                                           WPI_HW_SERIAL_EFM_BONDING,
                                           &profile_id_tx,
                                           NULL,
                                           &mask));

      cfu_tx_entry_config.profile_id = profile_id_tx;
      cfu_tx_entry_config.wait_entry = 0;
      cfu_tx_entry_config.tx_stats_en = WP_DISABLE;
      cfu_tx_entry_config.priority_profile_entry = 0;
      //cfu_tx_entry_config.requests_limit = WPI_CFU_TX_REQUESTS_LIMIT_MAX;
      cfu_tx_entry_config.requests_limit = 1; //Only a single thread per CFU port is activated.

      /*Set Configuration*/
      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryConfig(res_sa->cfu,
                                               cfu_tx_entry,
                                               &cfu_tx_entry_config));

      /*The TOD source will be set after the allocation*/
      cfu_tx_entry_config.tod_input_number = WPI_CFU_TX_CFUPORT_NO_TOD;

      WPI_RETURN_IF_ERROR(status,
                          WPI_SaCfuTxEntrySet((wpi_res_device *) res_dev,
                                              cfu_tx_entry));

      WPI_RETURN_IF_ERROR(status,
                          WPI_CfuTxEntryAlloc(res_sa->cfu,
                                              aa_tx_entry,
                                              cfu_tx_entry));

   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryEfmBondingEnetFunctionsRelease,
                               WPI_REG_UNLOCKED,
                               &res_efm);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   return status;
}


/*****************************************************************************
 *
 * Function:      WPI_SaTdiAlloc
 *
 * Purpose:       do allocation of the tdi aa and cfu entries
 *
 * Description:   go through all the ports and devices and allocate the used
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        resources     WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *        WPI_AllocSa
 ****************************************************************************/

static WP_status WPI_SaTdiAlloc(WP_U32 wpid, wpi_res_sa* res_sa, wpi_resource_block *resources)
{
   WP_U32 ii;
   wpi_resource_entry *port_entry;
   wpi_aa_entry *aa_rx_entry, *aa_tx_entry;
   wpi_res_tdi *res_tdi;
   WP_status status = WP_OK;
   WP_U8 agent_num;

   WP_U32 tdi_port = WP_PORT_TDM1;

   WPI_INTERNAL_FUNCTION();
   for (ii = WPI_tdi_base;
        ii < (WP_U32)(WPI_tdi_base + WPI_HwTdmMaxNumGet());
        ii++, tdi_port++)
   {
      WPI_ACCESS_RES(port_entry, resources, ii);

      if (WPI_RES_ENTRY_INITIALIZED(port_entry))
      {
         res_tdi = WPI_RES_TDI(port_entry);

         /* create the ports AA entries */
         WPI_RETURN_IF_ERROR( status,
                              WPI_SaPortCreate(wpid,
                                               tdi_port,
                                               WPI_DIRECTION_DUPLEX,
                                               &aa_rx_entry,
                                               &aa_tx_entry));

         agent_num = WPI_AA_TDI_RX_AGENT(tdi_port - WP_PORT_TDM1);
         /* allocate Aa entry (block of agents on an agent group) */
         WPI_RETURN_IF_ERROR( status,
                              WPI_AaEntrySpecificAlloc(res_sa->aa,
                                                       aa_rx_entry,
                                                       agent_num));

         /* write agent number in the TDI registry */
         WPI_HwTdiSetSaPollingRx(wpid,
                                 tdi_port - WP_PORT_TDM1,
                                 res_tdi,
                                 (agent_num - (WPI_AA_CFU_RX_AGENT_GROUPS + WPI_AA_CFU_TX_AGENT_GROUPS)*WPI_AA_AGENTS_PER_GROUP));

         agent_num = WPI_AA_TDI_TX_AGENT(tdi_port - WP_PORT_TDM1);
         /* allocate AA entry with specific agent */
         WPI_RETURN_IF_ERROR( status,
                              WPI_AaEntrySpecificAlloc(res_sa->aa,
                                                       aa_tx_entry,
                                                       agent_num));

         /* write agent number in the TDI registry */
         WPI_HwTdiSetSaPollingTx(wpid,
                                 tdi_port - WP_PORT_TDM1,
                                 res_tdi,
                                 (agent_num - (WPI_AA_CFU_RX_AGENT_GROUPS + WPI_AA_CFU_TX_AGENT_GROUPS)*WPI_AA_AGENTS_PER_GROUP));
      }
   }

   return status;
}


/*****************************************************************************
 *
 * Function:    WPI_TguConfig
 *
 * Purpose:     replaces WPI_TguPollingAddr used in WP2, create AA entry for
 *              the tgu type
 *
 * Description:
 *              create AA entry for the TGU type,
 *              set configuration to the  TGU type and save pointer from the serial_name
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_tgu       the tgu registry structure
 *        res_sa        the sa registry structure
 *        tgu_type      the type of the tgu configured
 *        limit_value   the trs stc for the tgu type
 *        mask_value   the trs stl for the tgu type
 *
 * Outputs:
 *        AA entry for the tgu type
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *        WPI_SaTguAlloc
 ****************************************************************************/

static WP_status WPI_SaTguConfig(WP_U32 wpid,
                                 wpi_res_tgu *res_tgu,
                                 wpi_res_sa *res_sa,
                                 WP_U32 tgu_type,
                                 WP_U32 limit_value,
                                 wpi_aa_trs_mask *mask_value)
{
   WP_U32 ii;
   wpi_aa_entry *tgu_entry;
   WP_status status = WP_OK;
   WP_sa_serial serial_name;
   wpi_aa_entry_config config;
   WP_U8 agent_num;
   WP_U32 max_tgu = WPI_HwTguCount();

   WPI_INTERNAL_FUNCTION();

   /* configuration */
   config.trs_mask.trs_mask_high = mask_value->trs_mask_high;
   config.trs_mask.trs_mask_low = mask_value->trs_mask_low;
   config.max_trs = limit_value;

   for (ii = WPI_TGU1; ii < max_tgu; ii++)
   {
      if (res_tgu->tgu[ii].allocation == tgu_type ||
          ((tgu_type >= WPI_TGU_SCHED_A)&&((res_tgu->tgu[ii].allocation & tgu_type) != 0)))
      {
         WP_boolean dynamic_entry = WP_FALSE;

         /* create new entry for the allocation */
         WPI_RETURN_IF_ERROR( status,
                              WPI_AaEntryCreate(res_sa->aa,
                                                WPI_AA_ENTRY_TYPE_TGU,
                                                &tgu_entry,
                                                dynamic_entry));
         /* apply configuaration */
         WPI_RETURN_IF_ERROR( status,
                              WPI_AaEntryConfig(res_sa->aa,
                                                tgu_entry,
                                                &config));

         agent_num = WPI_AA_TGU_AGENT(ii - WPI_TGU1);
         /* allocate AA entry with specific agent */
         WPI_RETURN_IF_ERROR( status,
                              WPI_AaEntrySpecificAlloc(res_sa->aa,
                                                       tgu_entry,
                                                       agent_num));

         /* write agent number as the acknowledge address for the TMR */
         WPI_HwTguAddrSet(res_tgu,
                          ii,
                          agent_num);

         /* save mapping of serial name to aa_entry */
         switch (tgu_type)
         {
            case WPI_TGU_SCHED:
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_A) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_A] = tgu_entry;
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_B) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_B] = tgu_entry;
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_C) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_C] = tgu_entry;
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_D) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_D] = tgu_entry;
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_E) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_E] = tgu_entry;
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_F) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_F] = tgu_entry;
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_G) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_G] = tgu_entry;
               if ((res_tgu->tgu[ii].allocation & WPI_TGU_SCHED_H) != 0)
                  res_sa->serial_to_entry_map[WP_SA_SCHED_CW_H] = tgu_entry;
               continue;
               break;
            case WPI_TGU_IMA_RX:
               serial_name = WP_SA_IMA_RX;
               break;
            case WPI_TGU_IMA_TX:
               serial_name = WP_SA_IMA_TX;
               break;
            case WPI_TGU_MLPPP_RX:
               serial_name = WP_SA_MLPPP_RX;
               break;
            case WPI_TGU_L2_BKGND:
               serial_name = WP_SA_L2_BKGND;
               break;
            case WPI_TGU_IW_BKGND:
               serial_name = WP_SA_IW_BKGND;
               break;
            case WPI_TGU_IWGP_BKGND:
               serial_name = WP_SA_IWGP_BKGND;
               break;
            case WPI_TGU_HOST_CMD:
               serial_name = WP_SA_HOST_CMD;
               break;
            case WPI_TGU_IW_TX_HOST:
               serial_name = WP_SA_IW_TX_HOST;
               break;
            case WPI_TGU_MCH_RX:
               serial_name = WP_SA_MCH_RX;
               break;
            case WPI_TGU_MCH_TX:
               serial_name = WP_SA_MCH_TX;
               break;
            case WPI_TGU_OAM_FM:
               serial_name = WP_SA_OAM_FM;
               break;
            case WPI_TGU_POLICER_WRAP_PREV:
               serial_name = WP_SA_POLICER_WRAP_PREV;
               break;
            case WPI_TGU_SW_TIMESTAMP:
               serial_name = WP_SA_SW_TIMESTAMP;
               break;
            case WPI_TGU_MFR_RX:
               serial_name = WP_SA_MFR_RX;
               break;
            case WPI_TGU_MFR_TX:
               serial_name = WP_SA_MFR_TX;
               break;
            case WPI_TGU_GBOND_BG:
               serial_name = WP_SA_GBOND_BG;
               break;
            case WPI_TGU_GBOND_TO:
               serial_name = WP_SA_GBOND_TO;
               break;
            case WPI_TGU_BPRESSURE:
               serial_name = WP_SA_BPRESSURE;
               break;
            case WPI_TGU_EFM_BOND_RX:
               serial_name = WP_SA_EFM_BOND_RX;
               break;
            case WPI_TGU_RAS_TO:
               serial_name = WP_SA_RAS_TO;
               break;
            case WPI_TGU_ITDM_RX:
               serial_name = WP_SA_ITDM_RX;
               break;

            case WPI_TGU_SLOW_PRO_CW:
               serial_name = WP_SA_SLOW_PRO_CW;
               break;
            case WPI_TGU_MILLISECOND_TIMER:
               serial_name = WP_SA_MILLISECOND_TIMER;
               break;
            case WPI_TGU_TOKEN_THREAD:
               serial_name = WP_SA_TOKEN_THREAD;
               break;
            case WPI_TGU_TOKEN_THREAD_2:
               serial_name = WP_SA_TOKEN_THREAD_2;
               break;

            case WPI_TGU_10G_LONG_PKT:
               serial_name = WP_SA_10G_LONG_PKT;
               break;
            case WPI_TGU_SGMII_AN:
               serial_name = WP_SA_SGMII_AN_TGU;
               break;
            case WPI_TGU_WINNET_AN_RESTART:
               serial_name = WP_SA_WINNET_AN_RESTART_TGU;
               break;
            default:
               return WPI_ERROR(WP_ERR_WDDI);
         }
         res_sa->serial_to_entry_map[serial_name] = tgu_entry;

         /* if we're after syscommit. commit the entry */
         if (WPI_REGISTRY_USE() == WP_TRUE)
         {
            WPI_RETURN_IF_ERROR( status,
                                 WPI_AaEntryCommit(res_sa->aa,
                                                   tgu_entry,
                                                   WP_TRUE));
         }
      }
   }
   return status;
}

/*****************************************************************************
 *
 * Function:      WPI_SaTguAlloc
 *
 * Purpose:       do allocation of the tgu aa and cfu entries
 *
 * Description:   go through all the tgu typess and allocate the used
 *
 * Inputs:
 *        wpid          Winpath ID
 *        res_sa        the SA registry structure
 *        resources     WDDI registry resource block
 *
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *        WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *        WPI_AllocSa
 ****************************************************************************/

static WP_status WPI_SaTguAlloc(WP_U32 wpid,
                                wpi_res_sa* res_sa,
                                wpi_resource_block *resources)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_tgu *res_tgu;
   wpi_res_trs_allocation *trs_allocation;
   wpi_resource_entry *tgu_entry;
   wpi_aa_trs_mask trs_mask, token_mask;
   wpi_trs_mask *trs_allocation_mask;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   tgu_entry = &resources->entry[WPI_tgu];
   res_tgu = WPI_RES_TGU(tgu_entry);
   trs_allocation = wpi_registry_ptr->pwp_block[wpid].trs_allocation;
   trs_allocation_mask = &(trs_allocation->trs_limit_all);

   /* tgu allocation */
   /* Allocate Aa entry to TGUs.
      Order of processing (below) implies priority of the TGU function. */
   trs_mask.trs_mask_high = WPI_AA_TRS_MASK_HIGH(trs_allocation_mask);
   trs_mask.trs_mask_low = WPI_AA_TRS_MASK_LOW(trs_allocation_mask);

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_SLOW_PRO_CW,
                                        trs_allocation->trs_limit_slow_pro_cw,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_EFM_BOND_RX,
                                        trs_allocation->trs_limit_efm_bond_rx,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_GBOND_BG,
                                        trs_allocation->trs_limit_gbond_bg,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_GBOND_TO,
                                        trs_allocation->trs_limit_gbond_to,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_IMA_RX,
                                        trs_allocation->trs_limit_ima_rx_iw,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_IMA_TX,
                                        trs_allocation->trs_limit_ima_tx,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_MLPPP_RX,
                                        trs_allocation->trs_limit_mlppp_rx_iw,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_MCH_RX,
                                        trs_allocation->trs_limit_mch_rx,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_MCH_TX,
                                        trs_allocation->trs_limit_mch_tx,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_MFR_RX,
                                        trs_allocation->trs_limit_mfr_rx,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_MFR_TX,
                                        trs_allocation->trs_limit_mfr_tx,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_L2_BKGND,
                                        trs_allocation->trs_limit_l2_bkgnd,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_IW_BKGND,
                                        trs_allocation->trs_limit_iw_bkgnd,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_IWGP_BKGND,
                                        trs_allocation->trs_limit_iwgp_bkgnd,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu,res_sa, WPI_TGU_SCHED,
                                        trs_allocation->trs_limit_sched,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_OAM_FM,
                                        trs_allocation->trs_limit_oam_fm,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_POLICER_WRAP_PREV,
                                        trs_allocation->trs_limit_policer_wrap_prev,
                                        &trs_mask));

   token_mask.trs_mask_high = WPI_AA_TRS_MASK_HIGH((&trs_allocation->trs_mask_token_thread));
   token_mask.trs_mask_low = WPI_AA_TRS_MASK_LOW((&trs_allocation->trs_mask_token_thread));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_TOKEN_THREAD,
                                        trs_allocation->trs_limit_token_thread,
                                        &token_mask));

   token_mask.trs_mask_high = WPI_AA_TRS_MASK_HIGH((&trs_allocation->trs_mask_token_thread_2));
   token_mask.trs_mask_low = WPI_AA_TRS_MASK_LOW((&trs_allocation->trs_mask_token_thread_2));
   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_TOKEN_THREAD_2,
                                        trs_allocation->trs_limit_token_thread_2,
                                        &token_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_HOST_CMD,
                                        trs_allocation->trs_limit_host_cmd,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_IW_TX_HOST,
                                        trs_allocation->trs_limit_iw_tx_host,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_SW_TIMESTAMP,
                                        trs_allocation->trs_limit_tgu_sw_timestamp,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_MILLISECOND_TIMER,
                                        trs_allocation->trs_limit_tgu_millisecond_timer,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_BPRESSURE,
                                        trs_allocation->trs_limit_tgu_bpressure,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_RAS_TO,
                                        trs_allocation->trs_limit_ras_to,
                                        &trs_mask));

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_10G_LONG_PKT,
                                        trs_allocation->trs_limit_10G_long_pkt,
                                        &trs_mask));



   WPI_RETURN_IF_ERROR( status,
                        WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_ITDM_RX,
                                        trs_allocation->trs_limit_itdm_rx,
                                        &trs_mask));

 WPI_RETURN_IF_ERROR( status,
                      WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_SGMII_AN,
                                      trs_allocation->trs_limit_sgmii_an_tgu,
                                      &trs_mask));

 WPI_RETURN_IF_ERROR( status,
                      WPI_SaTguConfig(wpid, res_tgu, res_sa, WPI_TGU_WINNET_AN_RESTART,
                                      trs_allocation->trs_limit_winnet_an_restart_tgu,
                                      &trs_mask));

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaPortUseCfu
 *
 * Purpose:    identify ports that use CFU
 *
 * Description:
 *
 * Inputs:
 *             port   wddi port
 *
 * Outputs:
 *
 * Return Value:
 *             WP_TRUE if the port uses CFU
 * Called by:
 *             functions in this file
 ****************************************************************************/

inline static WP_boolean WPI_SaPortUseCfu(WP_port port_index)
{
   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
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
      case WPI_PORT_VIRTUAL_LAG:
         return WP_TRUE;
      default:
         return WP_FALSE;
   }
}

/*****************************************************************************
 *
 * Function:       WPI_SaCfuTxBuffersDefaultGet
 *
 * Purpose:        get default tx buffers per cfu entry according to port
 *
 * Description:    the CFU tx buffers are allocated per cfu TX entry. this
 *                 defaults are defined in wpi_hw_adv_user.h
 *
 * Inputs:         wpid         Winpath Id
 *                 port_index   the wddi port
 *
 * Outputs:
 *
 * Return Value:
 *                 default number of buffers as defined in the
 *                 advanced user file
 *
 * Called by:
 *                 WPI_SaDeviceCreate
 ****************************************************************************/

inline static WP_U16 WPI_SaCfuTxBuffersDefaultGet(WP_U32 wpid,
                                                  WP_port port_index,
                                                  wpi_res_device *res_dev,
                                                  WP_boolean dynamic_entry)
{
   WP_U16 tx_buffers = 0;
   wpi_res_device_pkt *pkt_dev;
   WP_U32 device_index = WPI_HANDLE_INDEX(res_dev->device_handle);

   pkt_dev = (wpi_res_device_pkt *)res_dev;

   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
      {
         WP_U32 tx_phy_mode;
         WP_boolean upi_exists;

         upi_exists = WPI_UpiParamsGet(wpid,
                                       port_index,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL,
                                       &tx_phy_mode,
                                       NULL,
                                       NULL);
         if (upi_exists == WP_FALSE)
            tx_buffers = WPI_UPI_SINGLEPHY_TX_BUFFERS_NUM;

         if(tx_phy_mode == WPI_UPI_HW_PHY_MULTI)
            tx_buffers = WPI_UPI_MULTIPHY_TX_BUFFERS_NUM;
         else /* single phy mode */
            tx_buffers = WPI_UPI_SINGLEPHY_TX_BUFFERS_NUM;

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
      {
         WP_U32 is_sgmii_2500, is_giga;

         WPI_EnetPortModeGet(wpid,
                             port_index,
                             &is_giga,
                             &is_sgmii_2500,
                             NULL,
                             NULL);
         if(is_sgmii_2500)
         {
            if(dynamic_entry)
            {
               tx_buffers = WPI_WINNET_2500_INTSW_BUFFER_NUM;
            }
            else if(WPI_HwL2piUsesPB(wpid, device_index))
            {
               tx_buffers = pkt_dev->tx_maxsdu/WPI_CFU_TX_BUFFER_SIZE
                  * WPI_NUMBER_OF_THREADS_PER_2500;
            }
            else
               tx_buffers = WPI_WINNET_2500_TX_BUFFERS_NUM;
         }

         else if(is_giga)
         {
            if(dynamic_entry)
            {
               tx_buffers = WPI_WINNET_1000_INTSW_BUFFER_NUM;
            }
            else if(WPI_HwL2piUsesPB(wpid, device_index))
            {
               tx_buffers = pkt_dev->tx_maxsdu/WPI_CFU_TX_BUFFER_SIZE
                  * WPI_NUMBER_OF_THREADS_PER_1000;
            }
            else
            {
               tx_buffers = WPI_WINNET_1000_TX_BUFFERS_NUM;
            }
         }
         else
            tx_buffers = WPI_WINNET_10_100_TX_BUFFERS_NUM;
      }
      break;
      case WP_PORT_XGI1:
      case WP_PORT_XGI2:
       	 if(dynamic_entry)
      	 {
      	    tx_buffers = WPI_XGI_INTSW_BUFFERS_NUM;
      	 }
      	 else if(WPI_HwL2piUsesPB(wpid, device_index))
         {
            tx_buffers = pkt_dev->tx_maxsdu/WPI_CFU_TX_BUFFER_SIZE
               * WPI_NUMBER_OF_THREADS_PER_10G;
         }
         else
            tx_buffers = WPI_XGI_TX_BUFFERS_NUM;
         break;
      default:
         tx_buffers = 0;
   }

   return tx_buffers;
}

/*****************************************************************************
 *
 * Function:   WPI_SaCfuSerialPhyConvert
 *
 * Purpose:    mapping of port and index to cfu numbers
 *
 * Description: the CFU serials has internal numbering, this function converts
 *              external port and phy to this value
 *
 * Inputs:
 *             port_index   wddi port
 *             phy_index    index of phy under the the serial (upi / winnet)
 *
 * Outputs:
 *             converted_serial  CFU serial number
 *             converted_phy     CFU phy number
 * Return Value:
 *
 * Called by:
 *             WPI_SaDeviceInit
 ****************************************************************************/

inline static void WPI_SaCfuSerialPhyConvert(WP_U32 wpid,
                                             WP_port port_index,
                                             WP_U32 phy_index,
                                             WP_U32 *converted_serial,
                                             WP_U32 *converted_phy)
{
   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
         *converted_serial = (port_index - WP_PORT_UPI1 + WPI_CFU_SERIAL_UPI1);
         *converted_phy = phy_index;
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
      {
         WP_U32 winnet;
         WP_status status;

         status = WPI_HwWinnetNumGet(wpid, port_index, &winnet);
         WPI_ASSERT(status == WP_OK);
         *converted_serial = (winnet * (WPI_WINNET_MAX_PHYS+WPI_TIMESTAMPS_PER_WINNET)) +
            phy_index +
            WPI_CFU_SERIAL_ENET1_IF0;
         *converted_phy = 0;
      }
      break;
      case WP_PORT_XGI1:
      case WP_PORT_XGI2:
         *converted_serial = ((port_index - WP_PORT_XGI1)*
                              (WPI_CFU_SERIAL_XGI2-WPI_CFU_SERIAL_XGI1)
                              + WPI_CFU_SERIAL_XGI1);
         *converted_phy = 0;
      default:
         break;
   }
}

/*****************************************************************************
 *
 * Function:     WPI_SaSerialToAaEntry
 *
 * Purpose:      get Aa entry by serial name
 *
 *
 * Description:
 *
 * Inputs:
 *     wpid          Winpath ID
 *     serial_name   The serial name of the entry to be retrieved.
 *
 * Outputs:
 *     entry         The AA entry connected to the serial name
 * Return Value:
 *         WP_OK
 *         WP_ERR_SERIAL_NOT_ALLOC - serial has no Aa entry connected to it
 *
 * Called by:
 *         WPI_SaEntryGet, WPI_SaEntrySet
 ****************************************************************************/

inline static WP_status WPI_SaSerialToAaEntry(wpi_res_sa *res_sa,
                                              WP_sa_serial serial,
                                              wpi_aa_entry **entry)
{
   *entry = res_sa->serial_to_entry_map[serial];

   if(!(*entry))
   {
      return WPI_ERROR( WP_ERR_SERIAL_NOT_ALLOC);
   }
   return WP_OK;
}
/*****************************************************************************
 *
 * Function:     WPI_SaDeviceAllocate
 *
 * Purpose:      Allocate wpi_res_sa_device structure per WDDI device
 *
 * Description:
 *
 * Inputs:
 *               res_dev    the device registry structure
 *
 * Outputs:
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_RESOURCE_ALLOCATION - memory allocation failed
 *
 * Called by:
 *          WPI_SaDeviceInit
 ****************************************************************************/

inline static WP_status WPI_SaDeviceAllocate(wpi_res_device *res_dev)
{
   /* make sure the sa resource is allocated */
   res_dev->hardware_specific =
      WPI_WDDI_MALLOC(sizeof(wpi_res_sa_device));
   if( res_dev->hardware_specific == NULL)
      return WPI_ERROR( WP_ERR_RESOURCE_ALLOCATION);

   memset( res_dev->hardware_specific, 0, sizeof( wpi_res_sa_device));
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:     WPI_SaCfuRxEntrySet
 *
 * Purpose:      set the wddi_device's CFU entry pointer
 *
 * Description:
 *
 * Inputs:
 *               res_dev    the device registry structure
 *               entry      a created cfu rx entry
 *
 * Outputs:
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_RESOURCE_ALLOCATION - memory allocation failed
 *
 * Called by:
 *          WPI_SaDeviceInit
 ****************************************************************************/

inline static WP_status WPI_SaCfuRxEntrySet(wpi_res_device *res_dev,
                                            wpi_cfu_rx_entry* entry)
{
   wpi_res_sa_device* res_sa;
   res_sa = (wpi_res_sa_device*) (res_dev->hardware_specific);
   res_sa->cfu_rx_entry = entry;
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:    WPI_SaCfuTxEntrySet
 *
 * Purpose:     set the wddi_device's CFU entry pointer
 *
 *
 * Description:
 *
 * Inputs:
 *             res_dev    the device registry structure
 *             entry      a created cfu tx entry
 *
 * Outputs:
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_RESOURCE_ALLOCATION - memory allocation failed
 *
 * Called by:
 *          WPI_SaDeviceInit
 ****************************************************************************/

inline static WP_status WPI_SaCfuTxEntrySet(wpi_res_device *res_dev,
                                            wpi_cfu_tx_entry* entry)
{
   wpi_res_sa_device* res_sa;
   res_sa = (wpi_res_sa_device*) (res_dev->hardware_specific);
   res_sa->cfu_tx_entry = entry;
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaCfuRxEntryGet
 *
 * Purpose:    return the wddi_device's CFU entry pointer
 *
 *
 * Description:
 *
 * Inputs:
 *             res_dev    the device registry structure
 *
 * Outputs:
 *             entry      a created cfu rx entry
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *          many function in this file
 ****************************************************************************/

WP_status WPI_SaCfuRxEntryGet(wpi_res_device *res_dev,
                              wpi_cfu_rx_entry** entry)
{
   if(res_dev->hardware_specific == NULL)
      return WPI_ERROR( WP_ERR_DEVICE);

   *entry = ((wpi_res_sa_device *)(res_dev->hardware_specific))->cfu_rx_entry;
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:     WPI_SaCfuTxEntryGet
 *
 * Purpose:      return the wddi_device's CFU entry pointer
 *
 * Description:
 *
 * Inputs:
 *             res_dev    the device registry structure
 *
 * Outputs:
 *             entry      a created cfu tx entry
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *          many function in this file
 ****************************************************************************/

WP_status WPI_SaCfuTxEntryGet(wpi_res_device *res_dev,
                              wpi_cfu_tx_entry** entry)
{
   WPI_INTERNAL_FUNCTION();
   if(res_dev->hardware_specific == NULL)
      return WPI_ERROR( WP_ERR_DEVICE);
   *entry = ((wpi_res_sa_device *)(res_dev->hardware_specific))->cfu_tx_entry;
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:      WPI_SaTodSourceGet
 *
 * Purpose:       get the TOD source number for a device
 *
 * Description:   the function translate the alloc_type and alloc_num
 *                to internal CFU tod number
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *        tod_source    absolute (internal) source number
 *
 * Called by:
 *        WPI_SaCfuRxEntryConfigBuild
 ****************************************************************************/

static WP_U16 WPI_SaTodSourceGet(wpi_res_fifo_alloc_type tx_alloc_type,
                                 WP_U32 tx_alloc_num)
{
   WP_U16 tod_source = tx_alloc_num;
   WPI_INTERNAL_FUNCTION();

   switch(tx_alloc_type)
   {
      case WPI_TX_FIFO_ALLOC_WMM1:
      case WPI_TX_FIFO_ALLOC_WMM2:
      case WPI_TX_FIFO_ALLOC_WMM3:
         tod_source /= 4; /* wmm fifos tod is connected per group of 4 fifos */
         break;
      default:
         break;
   }

   switch(tx_alloc_type)
   {
      case WPI_TX_FIFO_ALLOC_WMM1:
         tod_source += WPI_SA_TOD_SOURCE_WMM1_OFFSET;
         break;
      case WPI_TX_FIFO_ALLOC_WMM2:
         tod_source += WPI_SA_TOD_SOURCE_WMM2_OFFSET;
         break;
      case WPI_TX_FIFO_ALLOC_WMM3:
         tod_source += WPI_SA_TOD_SOURCE_WMM3_OFFSET;
         break;
      case WPI_TX_FIFO_ALLOC_FMU_MASK:
         tod_source += WPI_SA_TOD_SOURCE_FMU_MASK_OFFSET;
         break;
      case WPI_TX_FIFO_ALLOC_FMU_L1:
         tod_source += WPI_SA_TOD_SOURCE_FMU_L1_OFFSET;
         break;
      default:
         tod_source = WPI_CFU_TX_CFUPORT_NO_TOD;
   }

   return tod_source;
}
/*****************************************************************************
 *
 * Function:      WPI_HwBufferSizeSet
 *
 * Purpose:       set the CFU tx fifo size for a device
 *
 * Description:   the function updates tx fifo size value per device
 *
 * Inputs:
 *                params - pointer to WP_syscmd_tx_buff_size structure
 * Outputs:
 *
 * Return Value:
 *        WP_OK, WP_ERR_TX_BUFFERS_MODIFY_AFTER_ALLOC
 *
 * Called by:
 *        WP_SysCommand
 ****************************************************************************/

WP_status WPI_HwBufferSizeSet(WP_syscmd_tx_buff_size *params)
{
   wpi_res_device *dev = NULL;
   wpi_res_sa_device * sa_dev;
   WP_status lock_status = WP_OK;
   WPI_INTERNAL_FUNCTION();

   if(WPI_REGISTRY_USE() == WP_TRUE)
   {
      return WPI_ERROR(WP_ERR_TX_BUFFERS_MODIFY_AFTER_ALLOC);
   }
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryDeviceGet,
                           WPI_REG_LOCKED,
                           &dev,
                           WPI_HANDLE_WPID(params->device),
                           WPI_HANDLE_INDEX(params->device));
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   sa_dev = (wpi_res_sa_device *)(dev->hardware_specific);
   sa_dev->tx_buffers_request = params->tx_buffers_num;
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaSmartDropEnable
 *
 * Purpose:    Enables or disables smart drop option per port
 *
 * Description:
 *
 * Inputs:
 *     port          Port handle
 *     enable        Enable or disable mode
 *
 * Outputs:
 *
 * Return Value: WP_OK
 *               WP_ERR_PORT - sa is not initialized
 *
 * Called by:    WP_PortSmartDrop
 *
 ****************************************************************************/

WP_status WPI_SaSmartDropEnable (WP_handle port,
                                 WP_boolean enable)
{
   WP_status status, lock_status = WP_OK;
   WP_U32 port_index;
   WP_U32 wpid;
   wpi_res_sa *res_sa = NULL;
   wpi_aa_rx_entry *aa_rx_entry;
   WP_sa_serial serial_name_rx = 0;

   WPI_INTERNAL_FUNCTION();
   port_index = WPI_HANDLE_INDEX(port);
   wpid = WPI_HANDLE_WPID(port);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if(res_sa == NULL || res_sa->cfu == NULL)
      return WPI_ERROR(WP_ERR_SA_UNINITIALIZED);

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialGet(port_index,
                                        &serial_name_rx,
                                        NULL,
                                        NULL,
                                        NULL));

   aa_rx_entry = res_sa->serial_to_entry_map[serial_name_rx] ;
   status = WPI_CfuSmartDropEnable(aa_rx_entry, enable, res_sa->cfu);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaAgentsAllocPerEnetPorts
 *
 * Purpose:    Calculates optimal way of TX agents allocation
 *
 * Description: This function checks if there enough unused agents to allocate
 *              TX agent per every ethernet port. If yes, aa_agent_allocation_needed
 *              field should be set as WP_TRUE for every ethernet port, else FAST Ethernet
 *              ports should be gropped together to TX agent. For first fast Ethernet port,
 *              connected to TX agent, aa_agent_allocation_needed field should be set as
 *              WP_TRUE, and for last ports - as WP_FALSE. If still there are not enough
 *              TX agents, every couple of GIGA Ethernet ports should be groped to one
 *              TX agent, for first port in the couple, aa_agent_allocation_needed
 *              field should be set WP_TRUE, for second - WP_FALSE. If still there are
 *              not enough TX agents, WP_ERR_NO_FREE_AA_ENTRY error returned.
 *
 * Inputs:
 *     ports_devs_types   structure with information needed for TX agents allocation
 *     res_sa             sa_registry
 *
 * Outputs:
 *     ports_devs_types->aa_agent_allocation_needed[WPI_ENET_N]
 * Return Value: WP_OK
 *               WP_ERR_NO_FREE_AA_ENTRY
 *
 * Called by:    WPI_SaEnetAlloc
 *
 ****************************************************************************/
static WP_status
WPI_SaAgentsAllocPerEnetPorts(wpi_enet_ports_devs_types *ports_devs_types,
                              wpi_res_sa* res_sa )
{
   WP_S8 free_agents;
   WP_U8 needed_agents=0;
   WP_U8 used_agents;
   WP_U8 ii;
   WP_U8 device_counter=0, agent_counter_fast=0, agent_counter_giga=0;
   WP_U8 first_fast_agent_allocated = 0;
   WP_status status = WP_OK;
   wpi_res_aa *res_aa;
   WP_U32 group_b_port_multi_classes_num = 0;

   WPI_INTERNAL_FUNCTION();
   /*default case: every port gets his own agent*/
   for(ii = 0; ii<WPI_ENET_N+WPI_XGI_N; ii++ )
   {
      /* NOTE: we set the aa_agent_allocation_needed for all ports, but only ports
       * who have devices created on them will check this flag.
       * this flag is relevant for created devices, who use the same
       * Tx Agent as other devices, in these cases it will be turned off. */
      ports_devs_types->aa_agent_allocation_needed[ii] =  WP_TRUE;
      ports_devs_types->agents_num[ii] = 0;

   }
   res_aa = res_sa->aa;
   /*Calculating used_agents -
     - agents that have been allocated before for non-enet ports,
     - agents for xgi ports - multiclass and single class,
     - agents for multiclass-configured enet ports
     - agents for enet ports with 2500 mbps bitrate*/

   used_agents = WPI_AaUsedAgentsGet(res_aa, WPI_AGENT_GROUP_CFU_TX);

   for(ii = 0; ii<WPI_HwEnetMaxNumGet()+WPI_HwXgiMaxNumGet(); ii++ )
   {
      if(WPI_MULTICLASS_ENABLED(ports_devs_types->port_multi_classes_num[ii]))
      {
         /*****************************************************************************/
         /* Multi-Class on Group A only, allocate 1 or 2 Tx agents for group A        */
         /* Multi-Class on Group B only, allocate 1 Tx agent for group B              */
         /* Multi-Class on both Groups,  allocate 1 or 2 Tx agents for groups A and B */
         /* NOTE: the same is for Multi-Class over XGI, only more Tx agents as there can
          *       be more Multi-Class devices.                                        */
         /*****************************************************************************/
         if ( ! (ii % 2) )
         {
            /* WINNET group A */
            /* NOTE: if an ENET port over WINNET uses WPI_MULTICLASS_PORT_STATE_SWAP,
             * than an additional port was counted for the ENET mode. */
            if (WPI_MULTICLASS_ENABLED(ports_devs_types->port_multi_classes_num[ii+1]))
               group_b_port_multi_classes_num = ports_devs_types->port_multi_classes_num[ii+1];
            else
               group_b_port_multi_classes_num = 0;
            ports_devs_types->agents_num[ii] =
               WPI_DIV_ROUND_UP(
               ports_devs_types->port_multi_classes_num[ii] + group_b_port_multi_classes_num,
               WPI_CFU_TX_CFUPORTS_PER_AGENT);
            used_agents += ports_devs_types->agents_num[ii];
            if( WPI_MULTICLASS_ENABLED(ports_devs_types->port_multi_classes_num[ii+1]) )
            {
               /* in case of multi-class on both groups,
                * the Tx agents are shared between group A and group B */
               ports_devs_types->aa_agent_allocation_needed[ii+1] = WP_FALSE;
               ports_devs_types->agents_num[ii+1] = 0;
            }
         }
         else
         {
            /* WINNET group B */
            if(! WPI_MULTICLASS_ENABLED(ports_devs_types->port_multi_classes_num[ii-1]) )
            {
               /* in case there is only multi-class on group B, the Tx agents are calculated
                * only for the group B multi-class devices. */
               ports_devs_types->agents_num[ii] =
                  WPI_DIV_ROUND_UP(ports_devs_types->port_multi_classes_num[ii],
                                   WPI_CFU_TX_CFUPORTS_PER_AGENT);
               used_agents += ports_devs_types->agents_num[ii];
            }
         }
      }
   }

   used_agents =used_agents + ports_devs_types->winnet_2500_ports_num +
      ports_devs_types->xgi_single_class_ports_num;

   /*Calculating free agents - agents for ports, that can be grouped to agent*/

   /* internal switching classes requires cfu TX ports, all allocated on a single TX agent
    * therefore, used_agents is increased by 1 */
   used_agents += WPI_DIV_ROUND_UP(ports_devs_types->internal_switching_num_of_classes,
                                   WPI_CFU_TX_CFUPORTS_PER_AGENT);

   free_agents = WPI_AA_CFU_TX_AGENT_GROUPS*WPI_AA_AGENTS_PER_GROUP - used_agents;
   if(free_agents > 0)
   {
      needed_agents =
         ports_devs_types->winnet_10_100_ports_num+
         ports_devs_types->winnet_1000_ports_num;

      if(needed_agents > free_agents)
      {
         /* Calculate number of agents that needed for fast devices*/
         for(ii = 0; ii <WPI_HwEnetMaxNumGet(); ii++ )
         {
            /*Calculation done only for ports that not in multiclass mode
              and bitrate - 10-100 Mbps*/
            if (!(ports_devs_types->port_multi_classes_num[ii] == WPI_MULTICLASS_DISABLED &&
                  ports_devs_types->winnet_port_bitrate[ii] == WPI_ENET_BITRATE_10_100))
               continue;
            if(device_counter+ports_devs_types->winnet_dev_per_port[ii]<=
               WPI_CFU_TX_CFUPORTS_PER_AGENT && first_fast_agent_allocated != 0)
            {/* case, that new agent allocation not needed*/
               device_counter += ports_devs_types->winnet_dev_per_port[ii];
               /* Signing agents, that not need allocation*/
               ports_devs_types->aa_agent_allocation_needed[ii] =  WP_FALSE;
            }
            else
            {/* case, that new agent allocation needed*/
               agent_counter_fast++;
               device_counter = ports_devs_types->winnet_dev_per_port[ii];
               if(first_fast_agent_allocated == 0)
                  first_fast_agent_allocated = 1;

            }
         }
         needed_agents =
            agent_counter_fast+
            ports_devs_types->winnet_1000_ports_num;
         if(needed_agents > free_agents)
         {
            /*For giga ports agent will be allocated per 2 ports */
            agent_counter_giga =
               WPI_DIV_ROUND_UP(ports_devs_types->winnet_1000_ports_num, 2);
            needed_agents =
               agent_counter_fast+
               agent_counter_giga;
            if(needed_agents <= free_agents)
            {
               WP_boolean odd_flag = WP_TRUE;
               status = WP_OK;
               /* Agent should be allocated only for odd giga ports*/

               for(ii = 0; ii <WPI_HwEnetMaxNumGet(); ii++ )
               {
                  /*Calculation done only for ports that not in multiclass mode
                    and bitrate - 1000 Mbps*/
                  if (!(ports_devs_types->port_multi_classes_num[ii] ==
                        WPI_MULTICLASS_DISABLED &&
                        ports_devs_types->winnet_port_bitrate[ii] ==
                        WPI_ENET_BITRATE_1000))
                     continue;
                  if(odd_flag == WP_TRUE)/*allocation needed*/
                  {
                     odd_flag = WP_FALSE;
                  }
                  else/*allocation not needed*/
                  {
                     /*signing agents, that not need allocation*/
                     ports_devs_types->aa_agent_allocation_needed[ii] = WP_FALSE;
                     odd_flag = WP_TRUE;
                  }
               }
            }
            else
               status =  WPI_ERROR(WP_ERR_NO_FREE_AA_ENTRY);
         }
      }
   }
   else
      status =  WPI_ERROR(WP_ERR_NO_FREE_AA_ENTRY);
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaHwDisplay
 *
 * Purpose:    Display Sa registers
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid            winpath id
 *             level           display level
 *             reg_base        pointer to registers
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *             WP_Display function
 ****************************************************************************/
void WPI_SaHwDisplay(WP_U32 wpid, WP_U32 level, WP_U8 *reg_base)
{
   WP_CHAR name[128];
   WP_U32 i,j;
   wpi_sa *sa_reg; /*AA registers*/
   wpi_res_sa *res_sa = NULL;
   wpi_res_cfu* res_cfu;

   WP_U32 trs_limit, lsa;
   WP_U8 enable, poll_method, tte, cot, count_id, priority_mask;
   wpi_trs_mask trs_mask;
   WP_U16 pc[4];
   WP_U32 agent_group_map, group_stcl_map_high, group_stcl_map_low;
   WP_status lock_status = WP_OK;

#if 0
   wpi_aa_trs_mask aa_trs_mask;
   WP_U16 preset_value, temporary_value;
#endif

   WPI_INTERNAL_FUNCTION();
   sa_reg = WPI_AaRegBaseGet(reg_base);
   WPI_StringDisplay (level, "Wincomm Agent Arbiter", "");
   WPI_AaRegisterGmrGet(sa_reg, &enable, &lsa, &poll_method);
   WPI_AaRegisterGmrWatchdogGet(sa_reg, &tte, &cot);
   WPI_UintDisplay (level + 1, "General Mode Register - enable", enable);
#if 0
   WPI_UintDisplay (level + 1, "General Mode Register - lsa", lsa);
   WPI_UintDisplay (level + 1, "General Mode Register - poll method", poll_method);
   WPI_UintDisplay (level + 1, "General Mode Register - timeout counter enable",
                    tte);
   WPI_UintDisplay (level + 1, "General Mode Register - clear on termination",
                    cot);
#endif
   /* Group agent map and group stcl map*/
   WPI_AaRegisterGroupAgentMapGet(sa_reg, &agent_group_map);
   WPI_UintDisplay(level+1, "Agent groups map register", agent_group_map);

   for(i = 0; i < WPI_AA_GROUP_N; i++)
   {
      WPI_AaRegisterGroupStclMapGet(sa_reg, i, &group_stcl_map_high, &group_stcl_map_low);
      sprintf(name, "Agent group %d stcl map 0x%x %x", i, group_stcl_map_high, group_stcl_map_low);
      WPI_StringDisplay (level+1, name, "");
   }


/* STL and STC tables display*/
   WPI_StringDisplay(level + 1, "STL and STC tables","");
   for(i = 0; i<WPI_SaStlSizeGet(); i++)
   {
      WP_U16 temp = 0;
      WPI_AaRegisterStlGet(sa_reg,
                           i,
                           &trs_limit,
                           &trs_mask);
      for (j=0; j<WP_SA_WINFARMS; j++)
      {
         temp |= trs_mask.wf[j];
      }
      if(temp)
      {
         sprintf(name, "TRS limit[%2d]", i);
         WPI_UintDisplay(level+2, name, trs_limit);
         for (j=0; j<WP_SA_WINFARMS; j++)
         {
            sprintf(name, "WinFarm %d: TRS Mask", j);
            WPI_UintDisplay(level+2, name, trs_mask.wf[j]);
         }
      }
   }

   WPI_StringDisplay(level + 1, "Program counters table","");
   for (i = 0; i < WPI_AA_PROFILE_ID_SIZE; i++)
   {
      WPI_AaRegisterPctGet(sa_reg, i, pc);
      sprintf (name, "PC Table[%2d] winfarm1", i);
      WPI_NuintDisplay (level + 2, name, pc[0]);
#if 0
      sprintf (name, "PC Table[%2d] winfarm2", i);
      WPI_NuintDisplay (level + 2, name, pc[1]);
      sprintf (name, "PC Table[%2d] winfarm3", i);
      WPI_NuintDisplay (level + 2, name, pc[2]);
      sprintf (name, "PC Table[%2d] winfarm4", i);
      WPI_NuintDisplay (level + 2, name, pc[3]);
#endif
   }

   WPI_StringDisplay(level + 1, "Profile ID table","");
   for (i = 0; i < WPI_AA_PROFILE_ID_SIZE; i++) {
      WPI_AaRegisterPtlGet(sa_reg, i,
                           &priority_mask, &count_id);
      sprintf (name, "Profile Table[%2d] priority mask", i);
      WPI_NuintDisplay (level + 2, name, priority_mask);
      sprintf (name, "Profile Table[%2d] count id", i);
      WPI_NuintDisplay (level + 2, name, count_id);
   }
#if 0
   WPI_StringDisplay(level + 1, "Agent TRS counters","");
   for (i = 0; i < WPI_AA_PROFILE_ID_SIZE; i++) {
      sprintf (name, "Agent[%2d]", i);
      WPI_NuintDisplay (level + 2, name, WPI_AaRegisterSerCntsGet(sa_reg, i));
   }
   WPI_StringDisplay(level + 1, "TRS counters per WINFARMS","");
   WPI_UintDisplay (level + 1, "Winfarm counts", WPI_SaRegisterWfCntsGet(sa_reg));

   WPI_StringDisplay(level + 1, "TRS busy status","");
   WPI_AaRegisterTbsrGet(sa_reg,
                         &aa_trs_mask);
   WPI_UintDisplay(level+2, "TRS busy status high", aa_trs_mask.trs_mask_high);
   WPI_UintDisplay(level+2, "TRS busy status low", aa_trs_mask.trs_mask_low);

   WPI_StringDisplay(level + 1, "Time-out Prescalar Counter","");
   WPI_AaRegisterTopcGet(sa_reg, &preset_value, &temporary_value);
   WPI_UintDisplay(level+2, "Prescalar Preset Value", preset_value);
   WPI_UintDisplay(level+2, "Temporary Timer Value", temporary_value);

   WPI_StringDisplay(level + 1, "Time-out events per TRS", "");
   WPI_AaRegisterToerGet(sa_reg,
                         &aa_trs_mask);
   WPI_UintDisplay(level+2, "Time-out events for high TRS", aa_trs_mask.trs_mask_high);
   WPI_UintDisplay(level+2, "Time-out events for low TRS", aa_trs_mask.trs_mask_low);

   WPI_StringDisplay(level + 1, "Time-out mask per TRS", "");
   WPI_AaRegisterTomrGet(sa_reg,
                         &aa_trs_mask);
   WPI_UintDisplay(level+2, "Time-out mask for high TRS", aa_trs_mask.trs_mask_high);
   WPI_UintDisplay(level+2, "Time-out mask for low TRS", aa_trs_mask.trs_mask_low);
#endif
   /*CFU registers display*/
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   res_cfu = res_sa->cfu;
   WPI_CfuHwDisplay(res_cfu, level, reg_base);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
}

/*****************************************************************************
 *
 * Function:      WPI_HwSetSmartDropThresholds
 *
 * Purpose:       Updates wpi_rx_agent_pri_drop_th structure with new
 *                smart drop start levels
 *
 * Description:   int_1st_th and int_non_1st_th values for every priority level calculated
 *                linearly starting with smart_drop_start_level_1st and
 *                smart_drop_start_level_non_1st values from WP_syscmd_smart_drop_params
 *                structures
 *
 * Inputs:
 *                wpid - Winpath ID
 *                params - pointer to WP_syscmd_smart_drop_params structure
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *        WP_SysCommand
 ****************************************************************************/

WP_status WPI_HwSetSmartDropThresholds(WP_syscmd_smart_drop_params *params)
{
   WP_status status, lock_status = WP_OK;
   WP_U32 wpid;
   WP_U32 port_index;
   wpi_res_sa *res_sa = NULL;
   wpi_aa_rx_entry *aa_rx_entry;
   WP_sa_serial serial_name_rx = 0;
   WP_U32 max_buffers;

   WPI_INTERNAL_FUNCTION();

   if(!params)
      return WPI_ERROR(WP_ERR_CONFIG);

   if (!WPI_HANDLE_PORT_P(params->port))
      return WPI_ERROR(WP_ERR_HANDLE);

   if((!(WPI_HANDLE_ENET_PORT_P(params->port)))&&
      (!(WPI_HANDLE_XGI_PORT_P(params->port)))&&
      (!(WPI_HANDLE_UPI_PORT_P(params->port))))
      return WPI_ERROR(WP_ERR_UNSUPPORTED);


   if(params->threshold_type >
      (WP_SYSCMD_FBP_THRESHOLD |
       WP_SYSCMD_AGENT_THRESHOLD))
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->priority >= WPI_CFU_RX_AGENT_DROP_TH_N)
      return WPI_ERROR(WP_ERR_CONFIG);

   port_index = WPI_HANDLE_INDEX(params->port);
   wpid = WPI_HANDLE_WPID(params->port);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if(res_sa == NULL ||
      res_sa->cfu == NULL)
      return WPI_ERROR(WP_ERR_SA_UNINITIALIZED);

   WPI_RETURN_IF_ERROR(status, WPI_HwCfuMaxRxBuffersByRegistry(res_sa->cfu, &max_buffers));

   if(params->first_pkt_drop_level > max_buffers ||
      params->non_first_pkt_drop_level > max_buffers ||
      params->first_pkt_agentq_drop_level > max_buffers ||
      params->non_first_pkt_agentq_drop_level > max_buffers)
      return WPI_ERROR(WP_ERR_CONFIG);

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialGet(port_index,
                                        &serial_name_rx,
                                        NULL,
                                        NULL,
                                        NULL));

   aa_rx_entry = res_sa->serial_to_entry_map[serial_name_rx] ;
   status = WPI_CfuSmartDropParamsModify(aa_rx_entry,
                                         res_sa->cfu,
                                         params->priority,
                                         params->threshold_type,
                                         params->first_pkt_drop_level,
                                         params->non_first_pkt_drop_level,
                                         params->first_pkt_agentq_drop_level,
                                         params->non_first_pkt_agentq_drop_level);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}


/*****************************************************************************
 *
 * Function:      WPI_HwSetAutoPauseThresholds
 *
 * Purpose:       Updates pause_settings structure with new
 *                internal FBP high and low thresholds
 *
 * Description:   auto  pause signal transmitted to serial if number of FBP buffers
 *                less than int_fbp_th_low threshold and stoped if number of
 *                FBP buffers more, than int_fbp_th_high
 *
 * Inputs:
 *                params - pointer to WP_syscmd_auto_pause_params structure
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *        WP_SysCommand
 ****************************************************************************/

WP_status WPI_HwSetAutoPauseThresholds(WP_syscmd_pause_params *params)
{
   WP_status status, lock_status = WP_OK;
   WP_U32 wpid;
   WP_U32 port_index;
   wpi_res_sa *res_sa = NULL;
   wpi_aa_rx_entry *aa_rx_entry;
   WP_sa_serial serial_name_rx = 0;
   WP_U32 max_buffers = 0;

   WPI_INTERNAL_FUNCTION();

   if (!WPI_HANDLE_PORT_P(params->port))
      return WPI_ERROR(WP_ERR_HANDLE);

   if((!(WPI_HANDLE_ENET_PORT_P(params->port)))&&
      (!(WPI_HANDLE_XGI_PORT_P(params->port)))&&
      (!(WPI_HANDLE_UPI_PORT_P(params->port))))
      return WPI_ERROR(WP_ERR_UNSUPPORTED);

   if(params->threshold_type >
      (WP_SYSCMD_FBP_THRESHOLD |
       WP_SYSCMD_AGENT_THRESHOLD))
      return WPI_ERROR(WP_ERR_CONFIG);

   port_index = WPI_HANDLE_INDEX(params->port);

   wpid = WPI_HANDLE_WPID(params->port);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if(res_sa == NULL ||
      res_sa->cfu == NULL)
      return WPI_ERROR(WP_ERR_SA_UNINITIALIZED);

   WPI_RETURN_IF_ERROR(status, WPI_HwCfuMaxRxBuffersByRegistry(res_sa->cfu, &max_buffers));

   if(params->auto_pause_start_level > max_buffers ||
      params->auto_pause_stop_level > max_buffers ||
      params->auto_pause_agentq_start_level > max_buffers ||
      params->auto_pause_agentq_stop_level > max_buffers)
      return WPI_ERROR(WP_ERR_CONFIG);

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialGet(port_index,
                                        &serial_name_rx,
                                        NULL,
                                        NULL,
                                        NULL));

   aa_rx_entry = res_sa->serial_to_entry_map[serial_name_rx] ;
   status = WPI_CfuAutoPauseParamsModify(aa_rx_entry,
                                         res_sa->cfu,
                                         params->threshold_type,
                                         params->auto_pause_start_level,
                                         params->auto_pause_stop_level,
                                         params->auto_pause_agentq_start_level,
                                         params->auto_pause_agentq_stop_level);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:      WPI_SaSerialGet
 *
 * Purpose:       Returns Sa serial name and aa entry type per wddi port
 *
 * Inputs:
 *                port_index - index of WDDI port
 * Outputs:
 *                serial_name_rx - serial name for RX direction
 *                serial_name_tx - serial name for TX direction
 *                aa_entry_type_rx - aa entry type for RX direction
 *                aa_entry_type_tx - aa entry type for TX direction
 *
 * Return Value:
 *        WP_OK, WP_ERR_UNIMPLEMENTED
 *
 * Called by:
 *        Different functions
 ****************************************************************************/
inline static WP_status WPI_SaSerialGet(WP_port port_index,
                                        WP_sa_serial *serial_name_rx,
                                        WP_sa_serial *serial_name_tx,
                                        wpi_aa_entry_type *aa_entry_type_rx,
                                        wpi_aa_entry_type *aa_entry_type_tx)
{
   switch(port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
         if(serial_name_rx != NULL)
            *serial_name_rx = WP_SA_UPI1_RX + ((port_index - WP_PORT_UPI1) * 2);
         if(serial_name_tx != NULL)
            *serial_name_tx = WP_SA_UPI1_TX + ((port_index - WP_PORT_UPI1) * 2);
         if(aa_entry_type_rx != NULL)
            *aa_entry_type_rx = WPI_AA_ENTRY_TYPE_CFU_RX;
         if(aa_entry_type_tx != NULL)
            *aa_entry_type_tx = WPI_AA_ENTRY_TYPE_CFU_TX;
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
         if(serial_name_rx != NULL)
            *serial_name_rx = WP_SA_ENET1_RX + ((port_index - WP_PORT_ENET1) * 2);
         if(serial_name_tx != NULL)
            *serial_name_tx = WP_SA_ENET1_TX + ((port_index - WP_PORT_ENET1) * 2);
         if(aa_entry_type_rx != NULL)
            *aa_entry_type_rx = WPI_AA_ENTRY_TYPE_CFU_RX;
         if(aa_entry_type_tx != NULL)
            *aa_entry_type_tx = WPI_AA_ENTRY_TYPE_CFU_TX;
         break;
      case WP_PORT_TDM1:
      case WP_PORT_TDM2:
      case WP_PORT_TDM3:
      case WP_PORT_TDM4:
      case WP_PORT_TDM5:
      case WP_PORT_TDM6:
      case WP_PORT_TDM7:
      case WP_PORT_TDM8:
      case WP_PORT_TDM9:
      case WP_PORT_TDM10:
      case WP_PORT_TDM11:
      case WP_PORT_TDM12:
      case WP_PORT_TDM13:
      case WP_PORT_TDM14:
      case WP_PORT_TDM15:
      case WP_PORT_TDM16:
         if(serial_name_rx != NULL)
            *serial_name_rx = WP_SA_TDI1_RX + ((port_index - WP_PORT_TDM1) * 2);
         if(serial_name_tx != NULL)
            *serial_name_tx = WP_SA_TDI1_TX + ((port_index - WP_PORT_TDM1) * 2);
         if(aa_entry_type_rx != NULL)
            *aa_entry_type_rx = WPI_AA_ENTRY_TYPE_TDI;
         if(aa_entry_type_tx != NULL)
            *aa_entry_type_tx = WPI_AA_ENTRY_TYPE_TDI;
         break;
      case WPI_PORT_VIRTUAL_LAG:
         *serial_name_tx = WPI_HW_SERIAL_LAG;
         *aa_entry_type_tx = WPI_AA_ENTRY_TYPE_CFU_TX;
         break;
      case WPI_PORT_VIRTUAL_EFM_BONDING_OVER_ENET:
         *serial_name_tx = WPI_HW_SERIAL_EFM_BONDING;
         *aa_entry_type_tx = WPI_AA_ENTRY_TYPE_CFU_TX;
         break;
         /*Calling function have to use WP_DIRECTION_DUPLEX direction
           for initialization of rx and tx data aa entries and
           WPI_DIRECTION_RX direction for rx control aa entry.*/

      case WPI_PORT_VIRTUAL_CES_SHAPING:
         *serial_name_tx = WP_SA_CES_SHAPING ;
         *aa_entry_type_tx = WPI_AA_ENTRY_TYPE_CFU_TX;
         break;

      default:
         return WPI_ERROR(WP_ERR_UNIMPLEMENTED);
         break;
   }
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaPortDisplay
 *
 * Purpose:    Display SA information per WDDI port
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           winpath index
 *             port           WDDI port index
 *             level           display level
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *             WPI_Display (WP_Display) function
 ****************************************************************************/
void WPI_SaPortDisplay(WP_U32 wpid,WP_U32 port, WP_U32 level)
{
   WP_CHAR name[128];
   WP_sa_serial serial_name_rx = 0, serial_name_tx = 0;
   wpi_aa_entry *aa_entry;
   wpi_aa_agent_block_alloc* agent_alloc;
   WP_U8 block_index = 1;
   wpi_res_reg_bases *res_reg_bases = NULL;
   WP_status lock_status = WP_OK;
   wpi_sa *sa_reg; /*AA registers*/
   WP_U32 trs_limit;
   wpi_trs_mask trs_mask;
   wpi_aa_trs_mask aa_trs_mask;
   wpi_res_sa *res_sa;
   wpi_res_aa *res_aa;
   WP_U8 polling_order;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   if (WP_ERROR_P(lock_status)) return;
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryRegisterBaseGet,
                           WPI_REG_LOCKED,
                           &res_reg_bases,
                           wpid);
   if (WP_ERROR_P(lock_status)) return;
   res_aa = res_sa->aa;

   WPI_SaSerialGet(port, &serial_name_rx, &serial_name_tx, NULL, NULL);
   WPI_SaSerialToAaEntry(res_sa, serial_name_rx, &aa_entry);
   sa_reg = WPI_AaRegBaseGet(res_reg_bases->reg_bases_ptr);
   sprintf(name, "Rx Agent for port %d", port);
   WPI_StringDisplay (level, name, "");
   WPI_UintDisplay (level + 1, "state",aa_entry->state);
   WPI_UintDisplay (level + 1, "index",aa_entry->index);
   WPI_UintDisplay (level + 1, "type",aa_entry->type);
   WPI_UintDisplay (level + 1, "max trs",aa_entry->configuration.max_trs);
   WPI_UintDisplay (level + 1, "trs_mask high",
                    aa_entry->configuration.trs_mask.trs_mask_high);
   WPI_UintDisplay (level + 1, "trs_mask low",
                    aa_entry->configuration.trs_mask.trs_mask_low);
   WPI_UintDisplay (level + 1, "blocks count",
                    aa_entry->allocation.blocks_count);

   agent_alloc = aa_entry->allocation.block_list;
   while (agent_alloc != NULL)
   {
      sprintf(name, "Agent block %d", block_index);
      WPI_StringDisplay (level+1, name, "");
      WPI_UintDisplay (level+2, "trs_rule_entry",agent_alloc->trs_rule_entry);
      WPI_UintDisplay (level+2, "agent_group",agent_alloc->agent_group);
      WPI_UintDisplay (level+2, "start_index",agent_alloc->start_index);
      WPI_UintDisplay (level+2, "block_size",agent_alloc->block_size);

      polling_order = res_aa->group_desc[agent_alloc->agent_group].poll_order;
      WPI_UintDisplay (level + 1, "polling order", polling_order);

      WPI_StringDisplay (level, "Hardware Registers:", "");
      WPI_AaRegisterStlGet(sa_reg,
                           polling_order*WPI_AA_TRS_RULES_PER_GROUP+agent_alloc->trs_rule_entry,
                           &trs_limit,
                           &trs_mask);
      aa_trs_mask.trs_mask_high = WPI_AA_TRS_MASK_HIGH((&trs_mask));
      aa_trs_mask.trs_mask_low = WPI_AA_TRS_MASK_LOW((&trs_mask));
      WPI_UintDisplay(level+2, "TRS limit", trs_limit);
      WPI_UintDisplay(level+2, "TRS mask high", aa_trs_mask.trs_mask_high);
      WPI_UintDisplay(level+2, "TRS mask low", aa_trs_mask.trs_mask_low);

      agent_alloc = agent_alloc->next;
      block_index++;
   }

   WPI_SaSerialToAaEntry(res_sa, serial_name_tx, &aa_entry);
   sprintf(name, "Tx Agent for port %d", port);
   WPI_StringDisplay (level, name, "");
   WPI_UintDisplay (level + 1, "state",aa_entry->state);
   WPI_UintDisplay (level + 1, "index",aa_entry->index);
   WPI_UintDisplay (level + 1, "type",aa_entry->type);
   WPI_UintDisplay (level + 1, "max trs",aa_entry->configuration.max_trs);
   WPI_UintDisplay (level + 1, "trs_mask high",
                    aa_entry->configuration.trs_mask.trs_mask_high);
   WPI_UintDisplay (level + 1, "trs_mask low",
                    aa_entry->configuration.trs_mask.trs_mask_low);
   WPI_UintDisplay (level + 1, "blocks count",
                    aa_entry->allocation.blocks_count);

   agent_alloc = aa_entry->allocation.block_list;
   while (agent_alloc != NULL)
   {
      sprintf(name, "Agent block %d", block_index);
      WPI_StringDisplay (level+1, name, "");
      WPI_UintDisplay (level+2, "trs_rule_entry",agent_alloc->trs_rule_entry);
      WPI_UintDisplay (level+2, "agent_group",agent_alloc->agent_group);
      WPI_UintDisplay (level+2, "start_index",agent_alloc->start_index);
      WPI_UintDisplay (level+2, "block_size",agent_alloc->block_size);

      polling_order = res_aa->group_desc[agent_alloc->agent_group].poll_order;
      WPI_UintDisplay (level + 1, "polling order", polling_order);

      WPI_StringDisplay (level, "Hardware Registers:", "");
      WPI_AaRegisterStlGet(sa_reg,
                           polling_order*WPI_AA_TRS_RULES_PER_GROUP+agent_alloc->trs_rule_entry,
                           &trs_limit,
                           &trs_mask);
      aa_trs_mask.trs_mask_high = WPI_AA_TRS_MASK_HIGH((&trs_mask));
      aa_trs_mask.trs_mask_low = WPI_AA_TRS_MASK_LOW((&trs_mask));

      WPI_UintDisplay(level+2, "TRS limit", trs_limit);
      WPI_UintDisplay(level+2, "TRS mask high", aa_trs_mask.trs_mask_high);
      WPI_UintDisplay(level+2, "TRS mask low", aa_trs_mask.trs_mask_low);
      agent_alloc = agent_alloc->next;
      block_index++;
   }
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryRegisterBaseRelease,
                               WPI_REG_UNLOCKED,
                               &res_reg_bases);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceDisplay
 *
 * Purpose:    Display SA information per WDDI port
 *
 *
 * Description:
 *
 * Inputs:
 *             wpid           winpath index
 *             res_dev         device registry
 *             level           display level
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *             WPI_Display (WP_Display) function
 ****************************************************************************/
void WPI_SaDeviceDisplay(WP_U32 wpid,
                         wpi_res_device *res_dev,
                         WP_U32 level,
                         WP_boolean multi_class_physical_device)
{
   wpi_res_reg_bases *res_reg_bases = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_sa_device* sa_res;
   wpi_cfu_rx_entry *cfu_rx_entry;
   wpi_cfu_tx_entry *cfu_tx_entry;

   WPI_INTERNAL_FUNCTION();

   if (!res_dev)
      return;

   sa_res = (wpi_res_sa_device*)res_dev->hardware_specific;

   if (!sa_res)
      return;

   cfu_rx_entry = sa_res->cfu_rx_entry;
   cfu_tx_entry = sa_res->cfu_tx_entry;

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryRegisterBaseGet,
                           WPI_REG_LOCKED,
                           &res_reg_bases,
                           wpid);
   if (WP_ERROR_P(lock_status))
      return;

   WPI_StringDisplay (level, "RX CFU port", "");
   WPI_CfuRxDeviceDisplay(res_reg_bases->reg_bases_ptr, cfu_rx_entry, level);

   /* A Multi-Class (or G999) physical device doesn't
    * have any CFU TX entries - only CFU RX.
    * (Tx is performed through the classes / streams) */
   if (multi_class_physical_device == WP_FALSE)
   {
      WPI_StringDisplay (level, "TX CFU port", "");
      WPI_CfuTxDeviceDisplay(res_reg_bases->reg_bases_ptr, cfu_tx_entry, level);
   }
   else
   {
      WPI_StringDisplay (level, "No TX CFU port on a Multi-Class/G999 physical device...", "");
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryRegisterBaseRelease,
                               WPI_REG_UNLOCKED,
                               &res_reg_bases);
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxPceBind
 *
 * Purpose:    bind the rx cfuport by the pce
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *             pce_settings   the parameters of pce usage from this cfu rx port
 *
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/
WP_status WPI_SaDeviceRxPceBind(WP_U32 wpid,
                                WP_handle device_handle,
                                wpi_sa_pce_bind_settings *pce_bind_settings)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   wpi_cfu_rx_entry *rx_entry = 0;
   wpi_res_sa *res_sa;
   wpi_cfu_rx_entry_pce_bind_settings pce_cfu_bind_settings;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   pce_cfu_bind_settings.user_defined[0] = pce_bind_settings->user_defined[0];
   pce_cfu_bind_settings.user_defined[1] = pce_bind_settings->user_defined[1];
   pce_cfu_bind_settings.user_defined[2] = pce_bind_settings->user_defined[2];
   pce_cfu_bind_settings.user_defined[3] = pce_bind_settings->user_defined[3];
   pce_cfu_bind_settings.parser_start = pce_bind_settings->parser_start;
   pce_cfu_bind_settings.pce_set_index = pce_bind_settings->pce_set_index;
   pce_cfu_bind_settings.pce_priority = pce_bind_settings->pce_priority;
   if(pce_bind_settings->pce_mode == WPI_SA_PCE_DISABLE)
      pce_cfu_bind_settings.pce_mode = WPI_CFU_MODE_DONT_USE_PCE;
   else if(pce_bind_settings->pce_mode == WPI_SA_PCE_ENABLE)
      pce_cfu_bind_settings.pce_mode = WPI_CFU_MODE_USE_PCE_RESULT_LAST_DU;

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   res_sa = WPI_RES_SA(&resources->entry[WPI_sa]);
   WPI_ACCESS_RES(dev_entry, resources, WPI_HANDLE_INDEX(device_handle));
   res_dev = WPI_RES_DEV(dev_entry);

   /* Find CFU RX port corresponding to device.
      Get the struct of current values of corresponding CFU registers. */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuRxEntryGet(res_dev,
                                           &rx_entry));

   /* Update in struct the corresponding values: mode, filter sets line. */
   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuRxEntryPceSettingsSet(rx_entry,
                                                    WPI_RegistryDeviceEnetG999(device_handle),
                                                    &pce_cfu_bind_settings));

   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuRxEntryPceUDSet(rx_entry,
                                              &(pce_bind_settings->user_defined[0])));

   /* Update the struct to CFU (write values to registers) */
   if(WPI_REGISTRY_USE() == WP_TRUE)/* check if the sys commit is done*/
   {
      WPI_RETURN_IF_ERROR( status,
                           WPI_CfuRxEntryUpdate(res_sa->cfu,
                                                rx_entry,
                                                WP_FALSE));
   }

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxGetPceaWaEntriesNumber
 *
 * Purpose:    get the number of allocated rx cfu ports
 *
 * Description:
 *
 * Inputs:
 *             wpid             Winpath ID
 *
 * Outputs:
 *             entries_number   the number of PCE CFU workaround PCEAPT entries
 *                              that is the number of allocated cfu rx ports
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/
WP_status WPI_SaDeviceRxGetInterfacesNumber(WP_U32 wpid,
                                            WP_U32 *entries_number)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_res_sa *res_sa;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];
   res_sa = WPI_RES_SA(&resources->entry[WPI_sa]);

   if(res_sa->cfu == NULL)
   {
      /* Sa is still not allocated - can not be used */
      return WPI_ERROR(WP_ERR_WDDI);
   }

   *entries_number = res_sa->cfu->cfu_rx->cfuport_next;

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxGetInterfaceIndex
 *
 * Purpose:    return the index of PCE CFU workaround parameter table that is equal to cfu rx port
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *
 * Outputs:
 *             entry_index    the PCE WA PCEAPT entry number that is the number of cfu rx port
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/
WP_status WPI_SaDeviceRxGetInterfaceIndex(WP_U32 wpid,
                                          WP_U32 device_index,
                                          WP_U32 *entry_index)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   wpi_cfu_rx_entry *rx_entry = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];;
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);

   /* Find CFU RX port corresponding to device.
      Get the struct of current values of corresponding CFU registers. */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuRxEntryGet(res_dev,
                                           &rx_entry));

   *entry_index = rx_entry->allocation.cfu_port_num;

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxSetPceaWorkaround
 *
 * Purpose:    set the PCE WA enabled in the registry of cfu rx port of the given device
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_index   the index of the device entry in the registry
 *
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/
WP_status WPI_SaDeviceRxSetPceaWorkaround(WP_U32 wpid,
                                          WP_U32 device_index)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device *res_dev;
   wpi_cfu_rx_entry *rx_entry = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   resources = &wpi_registry_ptr->pwp_resources[wpid];;
   WPI_ACCESS_RES(dev_entry, resources, device_index);
   res_dev = WPI_RES_DEV(dev_entry);

   /* Find CFU RX port corresponding to device.
      Get the struct of current values of corresponding CFU registers. */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuRxEntryGet(res_dev,
                                           &rx_entry));

   rx_entry->configuration.pcea_wa_used = WP_ENABLE;

   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_SaDeviceRxGetPceaWorkaround
 *
 * Purpose:    get the PCE WA in the registry of cfu rx port of the given device
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *             device_handle  the handle of the device entry in the registry
 *             pcea_wa_used   the returned value, whether or not the PCEA workaround is used.
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *
 ****************************************************************************/
WP_status WPI_SaDeviceRxGetPceaWorkaround(WP_U32 wpid,
                                          WP_handle device_handle,
                                          WP_U8 *pcea_wa_used)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_device *res_device = NULL;
   wpi_cfu_rx_entry *rx_entry = NULL;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_DEVICE_GET_BYHANDLE(lock_status,
                                    &res_device,
                                    device_handle);
   if (WP_ERROR_P(lock_status))
   {
      *pcea_wa_used = WP_DISABLE;
      return WP_OK;
   }

   /* Find CFU RX port corresponding to device.
      Get the struct of current values of corresponding CFU registers. */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuRxEntryGet(res_device,
                                           &rx_entry));

   *pcea_wa_used = rx_entry->configuration.pcea_wa_used;

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryDeviceRelease,
                               WPI_REG_UNLOCKED,
                               (wpi_res_device **)&res_device);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaPceEnable
 *
 * Purpose:    Enable the PCE (in CFU)
 *
 * Description:
 *
 * Inputs:
 *             wpid           Winpath ID
 *
 * Outputs:
 *
 * Return Value:
 *             WP_OK
 * Called by:
 *             WP_PceModuleInit
 ****************************************************************************/
WP_status WPI_SaPceEnable(WP_U32 wpid)
{
   WP_status status, lock_status = WP_OK;
   wpi_res_sa *res_sa;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuPceBypassDisable(res_sa->cfu));

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}


/*****************************************************************************
 *
 * Function:   WPI_HwSaProfilesMapEntryGet
 *
 * Purpose:    get serial_id from profiles map table
 *
 * Description: function reads entry in aa_profiles_map table
 *
 * Inputs:
 *             res_sa           pointer to sa registry
 *             profile_id       index in the profiles map table
 *
 * Outputs:
 *
 * Return Value:
 *             serial_id
 * Called by: WPI_RegistrySerialIndexGet
 *
 ****************************************************************************/
WP_U32 WPI_HwSaProfilesMapEntryGet(wpi_res_sa * res_sa,
                                   WP_U32 profile_id)
{
   return res_sa->aa_profiles_map[profile_id].serial_id;
}


/************************************************************************
 *
 * Function:  WPI_WinFarmTrsMask
 *
 * Purpose:   Returns the mask of TRS's for each Winfarm
 *
 * Description:
 *
 * Inputs:
 *     winfarm        The winfarm number (WP_SA_WF0 or WP_SA_WF1)
 *
 * Outputs:
 *
 * Return Value:
 *                    The mask of all TRSs for the specific WinFarm
 *
 * Called by:
 *     various functions
 *
 ****************************************************************************/

WP_U32 WPI_WinFarmTrsMask(WP_U8 winfarm)
{
   WP_U32 winfarm_mask = 0;
   switch(winfarm)
   {
      case WP_SA_WF0:
      case WP_SA_WF2:
         winfarm_mask = WPI_AA_STL_TRSMASK_WF02_M;
         break;
      case WP_SA_WF1:
      case WP_SA_WF3:
         winfarm_mask = WPI_AA_STL_TRSMASK_WF13_M;
         break;
   }
   return winfarm_mask;
}


/************************************************************************
 *
 * Function:  WPI_TsLoopbackDeviceAlloc
 *
 * Purpose:  Creation of a semi device for the the timestamp rx loopback port.
 *
 * Description: The function contains a subset of the functionality that is performed on all
 *              other regular enet devices. The subset is taken (partially or in whole)
 *              and modified from the following functions:
 *              WPI_SaDeviceCreate ( WPI_CfuRxEntryCreate , WPI_SaCfuRxEntryConfigBuild , WPI_CfuRxEntryConfig);
 *              WPI_SaCfuRxEntrySet, WPI_CfuRxEntryAlloc
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *
 *
 * Called by:
 *
 *
 ****************************************************************************/



static WP_status WPI_TsLoopbackDeviceAlloc(WP_U32 wpid,
                                           wpi_res_cfu* res_cfu,
                                           wpi_res_device_hspi_enet *dev_enet,
                                           WP_U32 port_index,
                                           wpi_aa_entry *aa_rx_entry)

{

   /* put inside special loopback struct */
   wpi_cfu_rx_entry_config cfu_rx_entry_config;
   WP_U8 serial_index;
   WP_U32 serial_num=0, phy_num=0 , winnet = 0;
   wpi_cfu_rx_entry *cfu_rx_entry = NULL ;
   WP_U32 profile_id_rx;
   wpi_trs_mask mask;
   WP_status status = WP_OK;

   dev_enet->loopback_device.pg_phy_number =  dev_enet->hspi.pkt.device.pg_phy_number;

   if(port_index == WP_PORT_XGI1)
   {
      serial_num = WPI_CFU_SERIAL_XGI1_TS;
      serial_index = WPI_HW_SERIAL_XGI1_TS;

   }
   else if(port_index == WP_PORT_XGI2)
   {
      serial_num = WPI_CFU_SERIAL_XGI2_TS;
      serial_index = WPI_HW_SERIAL_XGI2_TS;
   }
   else
   {
      WPI_RETURN_IF_ERROR(status, WPI_HwWinnetNumGet(wpid, port_index, &winnet));
      serial_num = (winnet * (WPI_WINNET_MAX_PHYS+WPI_TIMESTAMPS_PER_WINNET)) + WPI_WINNET_MAX_PHYS +
         (port_index - WP_PORT_ENET1)%2  +  WPI_CFU_SERIAL_ENET1_IF0;
      serial_index =  WPI_HW_SERIAL_ENET1_TS + (port_index - WP_PORT_ENET1);
   }


   /* allocates space in cfu_rx_entry */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuRxEntryCreate(res_cfu,
                                             serial_num,
                                             phy_num,
                                             &cfu_rx_entry));

   /* build configured values to cfu_rx_entry_config */
   cfu_rx_entry_config.user_defined =  0;  /* will be set after channel is set */
   cfu_rx_entry_config.pce_user_defined[0] = 0;
   cfu_rx_entry_config.pce_user_defined[1] = 0;
   cfu_rx_entry_config.pce_user_defined[2] = 0;
   cfu_rx_entry_config.pce_user_defined[3] = 0;

   /* Install DPS features in PCT and return PCT entry */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaPctInstall(wpid,
                                        WPI_DPS_ENTRYPT_L2PI_TS_LOOPBACK_RX,
                                        0,
                                        1,
                                        serial_index,
                                        &profile_id_rx, NULL, &mask));

   cfu_rx_entry_config.profile_id = profile_id_rx;
   cfu_rx_entry_config.rx_stats_en =
      (dev_enet->loopback_device.stats_en == WP_ENET_STAT_ENABLE ? WP_TRUE : WP_FALSE);

   cfu_rx_entry_config.du_size = WPI_CFU_DU_SIZE_512;

   /* pce settings */
   cfu_rx_entry_config.pce_settings.pce_mode = WPI_CFU_PMODE_NO_PCE;
   cfu_rx_entry_config.pce_settings.parser_start = 0;
   cfu_rx_entry_config.pce_settings.pce_set_index = 0;
   cfu_rx_entry_config.pce_settings.pce_priority = 0;

   /* set configuration (simply copies cfu_rx_entry_config to cfu_rx_entry  (taken from WPI_SaDeviceCreate)*/
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuRxEntryConfig(res_cfu,
                                             cfu_rx_entry,
                                             &cfu_rx_entry_config));

   /* do rx allocation */
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuRxEntryAlloc(res_cfu,
                                            aa_rx_entry,
                                            cfu_rx_entry,
                                            WP_FALSE));


   /* instead of WPI_SaCfuRxEntrySet */
   dev_enet->loopback_device.cfu_rx_entry = cfu_rx_entry;


   return status;
}

/*****************************************************************************
 *
 * Function:      WPI_HwSetDevicePriority
 *
 * Purpose:       Updates pce priority field in CFU port that doesn't use PCE
 *
 * Description:   This function updates PCE priority field in CFU rx control
 *                register for CFU port that connected to device or UPI port
 *                in emphy  mode
 *
 * Inputs:
 *                params - pointer to WP_syscmd_device_priority_params structure
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *        WP_SysCommand
 ****************************************************************************/
WP_status WPI_HwSetDevicePriority(WP_syscmd_device_priority_params *params)
{
   WP_status status, lock_status = WP_OK;
   WP_U32 wpid;
   WP_U32 port_index, dev_index;
   wpi_res_device *res_dev;
   wpi_res_upi *res_upi;
   WP_U32 is_emphy = 0;
   wpi_res_sa_device* sa_res;
   wpi_cfu_rx_entry *cfu_rx_entry;
   wpi_res_sa *res_sa = NULL;
   wpi_res_registry *wpi_registry_ptr = NULL;

   WPI_INTERNAL_FUNCTION();

   if(!params)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->priority > WPI_CFU_RX_DROP_MAX_PRIORITY)
      return WPI_ERROR(WP_ERR_CONFIG);

   wpid = WPI_HANDLE_WPID(params->handle);

   if (WPI_HANDLE_UPI_PORT_P(params->handle))
   {
      port_index = WPI_HANDLE_INDEX(params->handle);
      if(WPI_UpiParamsGet(wpid,
                          port_index,
                          NULL,
                          &is_emphy,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL) == WP_FALSE)
         is_emphy = WP_FALSE;
      if(is_emphy == WP_FALSE)
         return WPI_ERROR(WP_ERR_HANDLE);

      WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      res_upi = WPI_RES_UPI(WPI_RES_ENTRY(wpid,
                                          WPI_HANDLE_INDEX(params->handle)));
      if(res_upi == NULL)
         return WPI_ERROR(WP_ERR_WDDI);

      res_dev = res_upi->ufe_phy_device;
      if(res_dev == NULL)
         return WPI_ERROR(WP_ERR_WDDI);
   }
   else if(WPI_HANDLE_DEVICE_P(params->handle))
   {
      dev_index = WPI_HANDLE_INDEX(params->handle);
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryDeviceGet,
                              WPI_REG_LOCKED,
                              &res_dev,
                              wpid,
                              dev_index);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      if (res_dev == NULL)
         return WPI_ERROR(WP_ERR_WDDI);
   }
   else
      return WPI_ERROR(WP_ERR_HANDLE);

   sa_res = (wpi_res_sa_device*)res_dev->hardware_specific;
   if (sa_res == NULL)
      return WPI_ERROR(WP_ERR_WDDI);

   cfu_rx_entry = sa_res->cfu_rx_entry;
   if (cfu_rx_entry == NULL)
      return WPI_ERROR(WP_ERR_WDDI);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if(res_sa == NULL || res_sa->cfu == NULL)
      return WPI_ERROR(WP_ERR_SA_UNINITIALIZED);

   status = WPI_CfuSetRxDropPriority(res_sa->cfu, cfu_rx_entry, params->priority);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

/*****************************************************************************
 *
 * Function:      WPI_HwSetPortPriority
 *
 * Purpose:       Updates wpi_rx_agent_pri_prof structure with new
 *                priority levels
 *
 * Description:  Fuction sets number of buffers for different priorities levels
 *               and sets start_priority
 *
 * Inputs:
 *                params - pointer to WP_syscmd_port_priority_params structure
 * Outputs:
 *
 * Return Value:
 *        WP_OK
 *
 * Called by:
 *        WP_SysCommand
 ****************************************************************************/
WP_status WPI_HwSetPortPriority(WP_syscmd_port_priority_params *params)
{
   WP_status status, lock_status = WP_OK;
   WP_U32 wpid;
   WP_U32 port_index;
   wpi_res_sa *res_sa = NULL;
   wpi_aa_rx_entry *aa_rx_entry;
   WP_sa_serial serial_name_rx = 0;

   WPI_INTERNAL_FUNCTION();

   if(!params)
      return WPI_ERROR(WP_ERR_CONFIG);

   if (!WPI_HANDLE_PORT_P(params->port))
      return WPI_ERROR(WP_ERR_HANDLE);

   if((!(WPI_HANDLE_ENET_PORT_P(params->port)))&&
      (!(WPI_HANDLE_XGI_PORT_P(params->port)))&&
      (!(WPI_HANDLE_UPI_PORT_P(params->port))))
      return WPI_ERROR(WP_ERR_UNSUPPORTED);

   if(params->start_priority > WPI_CFU_RX_AGENT_MAX_PRIO_OFFSET)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->pri2_buffers >WPI_CFU_RX_AGENT_MAX_PRIO_TH)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->pri4_buffers >WPI_CFU_RX_AGENT_MAX_PRIO_TH)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->pri6_buffers >WPI_CFU_RX_AGENT_MAX_PRIO_TH)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->pri8_buffers >WPI_CFU_RX_AGENT_MAX_PRIO_TH)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->pri10_buffers >WPI_CFU_RX_AGENT_MAX_PRIO_TH)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->pri12_buffers >WPI_CFU_RX_AGENT_MAX_PRIO_TH)
      return WPI_ERROR(WP_ERR_CONFIG);

   if(params->pri14_buffers >WPI_CFU_RX_AGENT_MAX_PRIO_TH)
      return WPI_ERROR(WP_ERR_CONFIG);

   port_index = WPI_HANDLE_INDEX(params->port);
   wpid = WPI_HANDLE_WPID(params->port);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if(res_sa == NULL || res_sa->cfu == NULL)
      return WPI_ERROR(WP_ERR_SA_UNINITIALIZED);

   WPI_RETURN_IF_ERROR( status,
                        WPI_SaSerialGet(port_index,
                                        &serial_name_rx,
                                        NULL,
                                        NULL,
                                        NULL));

   aa_rx_entry = res_sa->serial_to_entry_map[serial_name_rx] ;
   status = WPI_CfuSetRxAgentPriority(aa_rx_entry,
                                      res_sa->cfu,
                                      params);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   return status;
}

wpi_res_cfu* WPI_SaCfuResourcesGet(wpi_res_sa *res_sa)
{
   WPI_INTERNAL_FUNCTION();

   return (res_sa->cfu);
}

/*****************************************************************************
 *
 * Function:    WPI_SaCfuTxSerialIndexGet
 * Purpose:     Get the wddi_device's CFU TX serial index
 * Description:
 *
 * Inputs:
 *             res_dev     the device registry structure
 *             serial_num  a pointer to be filled with the cfu serial num
 *
 * Outputs:    The cfu serial num
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *      WPI_PceRuleResultExactMatchBuild (core\pce\wpi_pce_result.c)
 ****************************************************************************/
WP_status WPI_SaCfuTxSerialIndexGet(wpi_res_device *res_dev,
                                    WP_U8 *serial_num)
{
   WP_status status;
   wpi_cfu_tx_entry *tx_entry = 0;

   status = WPI_SaCfuTxEntryGet(res_dev, &tx_entry);
   *serial_num = tx_entry->serial_num;

   return status;
}

/*****************************************************************************
 * Function:    WPI_SaCfuTxSerialIndexGet
 * Purpose:     Get the wddi_device's CFU TX phy index
 * Description:
 *
 * Inputs:
 *             res_dev     the device registry structure
 *             phy_num     a pointer to be filled with the cfu phy_num
 *
 * Outputs:    phy_num     The cfu phy_num
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *      WPI_PceRuleResultExactMatchBuild (core\pce\wpi_pce_result.c)
 ****************************************************************************/
WP_status WPI_SaCfuTxPhyIndexGet(wpi_res_device *res_dev,
                                 WP_U16 *phy_num)
{
   WP_status status;
   wpi_cfu_tx_entry *tx_entry = 0;

   status = WPI_SaCfuTxEntryGet(res_dev, &tx_entry);
   *phy_num = tx_entry->phy_num;

   return status;
}

#if !defined (WINSIM) && !defined(VERILOG)
/*****************************************************************************
 * Function:    WPI_HwCommitG999EnetSwapRx
 * Purpose:     update the PCT entry when working in ENET<->G999 mode.
 *              NOTE: this code assumes that the device to be enabled is the
 *                    device who's handle is in the device registry.
 *
 * Inputs:
 *             res_dev     the device registry structure of the physical device.
 *             res_sa      sa resource for the hardware registers offset of the profile id.
 *
 * Outputs:    status
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_WDDI - wrong device handle in the resource.
 *
 * Called by:
 *     WPI_SaDeviceStateSet
 ****************************************************************************/
static WP_status WPI_HwCommitG999EnetSwapRx(WP_U32 wpid,
                                            wpi_res_sa *res_sa,
                                            wpi_res_device_hspi_enet *dev_enet)
{
   WP_status status = WP_OK;
   WP_U32 subtype;
   WP_S32 pce_via_cfu_value;
   WP_U32 protocol_type_rx;
   WP_U16 pc_rx[WPI_WINFARM_N], pc_tx[WPI_WINFARM_N];
   wpi_trs_mask trs_mask;
   wpi_cfu_rx_entry *cfu_rx_entry = NULL;
   subtype = WPI_HANDLE_SUBTYPE(dev_enet->hspi.pkt.device.device_handle);
   switch(subtype)
   {
         case WPI_pg_pkt_enet_g999:
         {
            protocol_type_rx = WPI_DPS_ENTRYPT_L2PI_RX_G999;
            break;
         }
         case WPI_pg_pkt_enet1:
         {
            WPI_CapabilityRead(wpid, WPI_CAP_HW_CFU_PCE_INTERFACE, &pce_via_cfu_value);
            if(pce_via_cfu_value)
               protocol_type_rx = WPI_DPS_ENTRYPT_L2PI_RX;
            else
               protocol_type_rx = WPI_DPS_ENTRYPT_L2PI_RX_PCE_WA;
            break;
         }
         default:
            return WPI_ERROR(WP_ERR_WDDI);
            break;
   }
   memset(&trs_mask, 0, sizeof(wpi_trs_mask));
   status = WPI_SaPctFind(wpid,
                          protocol_type_rx, 0, 1,
                          pc_rx, pc_tx, &trs_mask);
   WPI_RETURN_IF_ERROR_STATUS(status);
   /* need to updated the Rx CFU port only in case
    * of a move from Enet to G999 or G999 to Enet */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuRxEntryGet(&(dev_enet->hspi.pkt.device),
                                           &cfu_rx_entry));
   WPI_AaRegisterPctSet(res_sa->aa->hw_aa,
                        cfu_rx_entry->configuration.profile_id,
                        pc_rx);
   return WP_OK;
}
#endif //!defined (WINSIM) && !defined(VERILOG)

/*****************************************************************************
 * Function:    WPI_HwCommitG999EnetSwapTx
 * Purpose:     Commit the winnet again with the new RX/TX PCT's
 * Description: In G999 swap mode, after determin
 *
 * Inputs:
 *             res_dev     the device registry structure
 *             phy_num     a pointer to be filled with the cfu phy_num
 *
 * Outputs:    phy_num     The cfu phy_num
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *     WPI_HwDeviceG999EnetSwap (hardware/winpath3/wpi_hw_winnet.c)
 ****************************************************************************/
WP_status WPI_HwCommitG999EnetSwapTx(WP_U32 wpid,
                                     WPI_G999EnetSwapMode G999EnetSwapMode,
                                     wpi_res_device_hspi_enet *dev_enet,
                                     wpi_res_enet_multi_class_port *group_a_port_entry,
                                     wpi_res_enet_multi_class_port *group_b_port_entry)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_sa *res_sa = NULL;
   wpi_res_device *res_dev = NULL;
   wpi_res_enet_multi_class_device *res_enet_multi_class_device = NULL;
   wpi_cfu_tx_entry *cfu_tx_entry = NULL;
   WP_U32 serial_num = 0, phy_num = 0, port_index = 0, hw_serial_index;
   WP_U8 ser_map;
   WP_S32 ii;
   WP_U16 pc_rx[WPI_WINFARM_N];
   WP_U16 pc_tx[WPI_WINFARM_N];
   WP_U32 protocol_type_rx, protocol_type_tx;
   WP_S32 pce_via_cfu_value;
   wpi_trs_mask trs_mask;

   WPI_INTERNAL_FUNCTION();

   res_dev = &(dev_enet->hspi.pkt.device);
   port_index = WPI_HANDLE_INDEX(res_dev->port);
   /**************/
   /* Get res_sa */
   /**************/
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   /* convert port and phy to cfu serial and phy */
   WPI_SaCfuSerialPhyConvert(wpid,
                             port_index,
                             res_dev->phy_number,
                             &serial_num,
                             &phy_num);
   WPI_CapabilityRead(wpid, WPI_CAP_HW_CFU_PCE_INTERFACE, &pce_via_cfu_value);
   hw_serial_index = WPI_HW_SERIAL_ENET1_RX + (2 *(port_index - WP_PORT_ENET1));
   /***********************************************/
   /* Find updated PCT entry points for Rx and Tx */
   /* Only the Tx PCT entry points are required   */
   /***********************************************/
   switch (G999EnetSwapMode)
   {
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_ENET:
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_ENET:
      {
         if(pce_via_cfu_value)
            protocol_type_rx = WPI_DPS_ENTRYPT_L2PI_RX;
         else
            protocol_type_rx = WPI_DPS_ENTRYPT_L2PI_RX_PCE_WA;
         protocol_type_tx = WPI_DPS_ENTRYPT_L2PI_TX_CB;
         break;
      }
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_G999:
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_G999:
      case G999_ENET_SWAP_MOVE_GROUP_B_CFU_PORTS_TO_GROUP_A:
      case G999_ENET_SWAP_MOVE_GROUP_A_CFU_PORTS_TO_GROUP_B:
      {
         protocol_type_rx = WPI_DPS_ENTRYPT_L2PI_RX_G999;
         protocol_type_tx = WPI_DPS_ENTRYPT_L2PI_TX_G999;
         break;
      }
      default:
      {
         return WPI_ERROR(WP_ERR_ENET_G999_SWAP_MODE_NOT_SUPPORTED);
      }
   }
   memset(&trs_mask, 0, sizeof(wpi_trs_mask));
   status = WPI_SaPctFind(wpid,
                          protocol_type_rx, protocol_type_tx, 2,
                          pc_rx, pc_tx, &trs_mask);
   WPI_RETURN_IF_ERROR_STATUS(status);

   /**********************************/
   /* Update TX CFU ports, if needed */
   /**********************************/
   switch (G999EnetSwapMode)
   {
      case G999_ENET_SWAP_MOVE_GROUP_B_CFU_PORTS_TO_GROUP_A:
      case G999_ENET_SWAP_MOVE_GROUP_A_CFU_PORTS_TO_GROUP_B:
      {
         for (ii=group_b_port_entry->start_index ;
              ii<=group_b_port_entry->end_index ;
              ii++)
         {
            res_enet_multi_class_device = group_b_port_entry->multi_class_dev[ii];
            if (res_enet_multi_class_device != NULL)
            {
               /* get entry */
               WPI_RETURN_IF_ERROR(status,
                                   WPI_SaCfuTxEntryGet(&(res_enet_multi_class_device->pkt.device),
                                                       &cfu_tx_entry));
               cfu_tx_entry->serial_num = serial_num;
               if (G999EnetSwapMode == G999_ENET_SWAP_MOVE_GROUP_B_CFU_PORTS_TO_GROUP_A)
                  cfu_tx_entry->phy_num = ii - group_a_port_entry->start_index;
               else if (G999EnetSwapMode == G999_ENET_SWAP_MOVE_GROUP_A_CFU_PORTS_TO_GROUP_B)
                  cfu_tx_entry->phy_num = ii - group_b_port_entry->start_index;

               WPI_RETURN_IF_ERROR( status,
                                    WPI_CfuTxEntryCommit(res_sa->cfu,
                                                         res_sa->aa,
                                                         cfu_tx_entry,
                                                         WP_FALSE));
               cfu_tx_entry->configuration.profile_id = res_sa->serial_to_profile_id_map[hw_serial_index + WPI_SaPctTxOffsetGet()];
            }
         }
         break;
      }
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_ENET:
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_ENET:
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_G999:
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_G999:
      {
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet(res_dev, &cfu_tx_entry));
         break;
      }
      default:
         break;
   }

   if (!cfu_tx_entry)
   {
      return WPI_ERROR(WP_ERR_NULL_PTR);
   }

   WPI_AaRegisterPctSet(res_sa->aa->hw_aa, cfu_tx_entry->configuration.profile_id, pc_tx);

   /********************************/
   /* set new TX Serial Tx Mapping */
   /********************************/
   switch (G999EnetSwapMode)
   {
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_ENET:
      {
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet(res_dev, &cfu_tx_entry));
         res_sa->cfu->cfu_tx->ser_tx_map[serial_num] = group_a_port_entry->original_ser_tx_map;
         break;
      }
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_ENET:
      {
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet(res_dev, &cfu_tx_entry));
         res_sa->cfu->cfu_tx->ser_tx_map[serial_num] =
            group_b_port_entry->original_ser_tx_map +
            group_b_port_entry->original_max_multi_class_devices;
         break;
      }
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_G999:
      {
         ii = group_a_port_entry->start_index;
         res_enet_multi_class_device = group_a_port_entry->multi_class_dev[ii];
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet((wpi_res_device *)res_enet_multi_class_device,
                                                 &cfu_tx_entry));
         res_sa->cfu->cfu_tx->ser_tx_map[serial_num] = group_a_port_entry->original_ser_tx_map + 1;
         break;
      }
      case G999_ENET_SWAP_MOVE_GROUP_B_CFU_PORTS_TO_GROUP_A:
      {
         /* NOTE: this code should not be reached in case group B does not exist. */
         WP_U32 group_b_serial_num;
         wpi_cfu_tx_entry cfu_tx_entry_config;
         group_b_serial_num = serial_num + WPI_WINNET_GROUP0_MAX_PHYS;

         res_sa->cfu->cfu_tx->ser_tx_map[group_b_serial_num] =
            group_b_port_entry->original_ser_tx_map +
            group_b_port_entry->original_max_multi_class_devices;
         memset (&cfu_tx_entry_config, 0, sizeof (wpi_cfu_tx_entry));
         ser_map = res_sa->cfu->cfu_tx->ser_tx_map[group_b_serial_num];
         cfu_tx_entry_config.serial_num = group_b_serial_num;
         cfu_tx_entry_config.phy_num = 0;
         cfu_tx_entry_config.allocation.cfu_port_num = CFU_SER_MAP_ENTRY_UNUSED;
         WPI_RETURN_IF_ERROR( status,
                              WPI_CfuRegTxPortMapSet(res_sa->cfu->cfu_tx->hw_cfu_tx,
                                                     &cfu_tx_entry_config,
                                                     ser_map));

         ii = group_a_port_entry->start_index;
         res_enet_multi_class_device = group_a_port_entry->multi_class_dev[ii];
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet((wpi_res_device *)res_enet_multi_class_device,
                                                 &cfu_tx_entry));
         res_sa->cfu->cfu_tx->ser_tx_map[serial_num] = group_a_port_entry->original_ser_tx_map + 1;

         break;
      }
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_G999:
      {
         ii = group_b_port_entry->start_index;
         res_enet_multi_class_device = group_b_port_entry->multi_class_dev[ii];
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet((wpi_res_device *)res_enet_multi_class_device,
                                                 &cfu_tx_entry));
         res_sa->cfu->cfu_tx->ser_tx_map[serial_num] =
            group_b_port_entry->original_ser_tx_map;
         break;
      }
      case G999_ENET_SWAP_MOVE_GROUP_A_CFU_PORTS_TO_GROUP_B:
      {
         ii = group_b_port_entry->start_index;
         res_enet_multi_class_device = group_b_port_entry->multi_class_dev[ii];
         WPI_RETURN_IF_ERROR(status,
                             WPI_SaCfuTxEntryGet((wpi_res_device *)res_enet_multi_class_device,
                                                 &cfu_tx_entry));
         res_sa->cfu->cfu_tx->ser_tx_map[serial_num] =
            group_b_port_entry->original_ser_tx_map;
         break;
      }
   }
   ser_map = res_sa->cfu->cfu_tx->ser_tx_map[serial_num];
   WPI_RETURN_IF_ERROR( status,
                        WPI_CfuRegTxPortMapSet(res_sa->cfu->cfu_tx->hw_cfu_tx,
                                               cfu_tx_entry,
                                               ser_map));

   /*************************/
   /* update MPE            */
   /*************************/
   switch (G999EnetSwapMode)
   {
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_ENET:
         group_a_port_entry->state &= ~WPI_MULTICLASS_PORT_STATE_IN_MULTI_CLASS_MODE;
         break;
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_ENET:
         group_b_port_entry->state &= ~WPI_MULTICLASS_PORT_STATE_IN_MULTI_CLASS_MODE;
         break;
      case G999_ENET_SWAP_MOVE_GROUP_A_TO_G999:
         group_a_port_entry->state |= WPI_MULTICLASS_PORT_STATE_IN_MULTI_CLASS_MODE;
         break;
      case G999_ENET_SWAP_MOVE_GROUP_B_TO_G999:
         group_b_port_entry->state |= WPI_MULTICLASS_PORT_STATE_IN_MULTI_CLASS_MODE;
         break;
      case G999_ENET_SWAP_MOVE_GROUP_B_CFU_PORTS_TO_GROUP_A:
         group_b_port_entry->state &= ~WPI_MULTICLASS_PORT_STATE_IN_MULTI_CLASS_MODE;
         break;
      case G999_ENET_SWAP_MOVE_GROUP_A_CFU_PORTS_TO_GROUP_B:
         group_b_port_entry->state |= WPI_MULTICLASS_PORT_STATE_IN_MULTI_CLASS_MODE;
         break;
   }

   WPI_RETURN_IF_ERROR(status,
                       WPI_HwWinnetSetMpeM(wpid,
                                           (port_index - WP_PORT_ENET1) ) );

   /******************/
   /* Release res_sa */
   /******************/
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_SaTxPortGracefulDisable
 *
 * Purpose:    Disable CFU TX port
 *
 *
 * Description: The function disables CFU TX ports
 *
 * Inputs:
 *          res_dev     pointer to device registry
 *          res_sa      pointer to sa registry
 *
 * Outputs:
 *
 * Return Value:
 *             WP_OK if succeded, error code otherwise
 * Called by:
 *             WPI_SaDeviceStateSet
 ****************************************************************************/
WP_status WPI_SaTxPortGracefulDisable(wpi_res_device *res_dev,
                                      wpi_res_sa *res_sa)
{
   WP_U32 status;
   WP_U32 wpid;
   wpi_res_device_hspi *res_dev_hspi;
   wpi_hw_cfu_tx *hw_cfu_tx;
   wpi_cfu_tx_entry *cfu_tx_entry = 0;
   WP_boolean tgu_used_for_transmit  = WP_FALSE;
   WP_handle port_handle;
   WP_U32 port_index;
   WP_U8 tgu_num = 0;

   WPI_INTERNAL_FUNCTION();

   wpid = WPI_HANDLE_WPID(res_dev->device_handle);

   hw_cfu_tx = res_sa->cfu->cfu_tx->hw_cfu_tx;

   /* get device resources */
   WPI_RETURN_IF_ERROR(status,
                       WPI_SaCfuTxEntryGet(res_dev, &cfu_tx_entry));

   /* Casting is safe because of this code applies to enet devices only and for enet device
      wpi_res_device_hspi structure always have been allocated */
   res_dev_hspi = (wpi_res_device_hspi *)res_dev;

   if(res_dev_hspi->mode_10G_long_pkt == WP_ENET_TX_MODE_10G_LONG_PKT)
   {
      tgu_used_for_transmit = WP_TRUE;
      port_handle = res_dev->port;

      /* port_index of XGI port starting from XGI1 */
      port_index = WPI_HANDLE_INDEX(port_handle)-WP_PORT_XGI1;
      WPI_RETURN_IF_ERROR(status,
                          WPI_10GLongPacketRegistryGet(wpid,
                                                       port_index,
                                                       0, &tgu_num, 0));
   }

   WPI_RETURN_IF_ERROR(status,
                       WPI_CfuTxPortGracefulDisable(wpid,
                                                    hw_cfu_tx,
                                                    cfu_tx_entry,
                                                    tgu_used_for_transmit,
                                                    tgu_num,
                                                    res_sa->cfu->cfu_tx->tx_cfuport_disable_tod_source->l1_group));


   return WP_OK;
}
/*****************************************************************************
 *
 * Function:    WPI_SaRxAssemblersModeSet
 *
 * Purpose:     Set the indication of the use of RX assemblers (Internal Switching)
 *
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Return Value:
 *          WP_OK
 *          WP_ERR_DEVICE - registry structure is not initialized
 *
 * Called by:
 *      WPI_PceRuleResultExactMatchBuild (core\pce\wpi_pce_result.c)
 ****************************************************************************/
WP_status WPI_SaRxAssemblersModeSet(WP_U32 wpid)
{
   WP_status lock_status;
   wpi_res_sa* res_sa;

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistrySaGet,
                           WPI_REG_LOCKED,
                           &res_sa,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   res_sa->cfu->cfu_rx->intsw_assemblers_used = WP_TRUE;

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistrySaRelease,
                               WPI_REG_UNLOCKED,
                               &res_sa);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   return WP_OK;

}

