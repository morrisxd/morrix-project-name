/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
/*****************************************************************************
 *
 * File: wpi_channels.c
 *
 * Purpose:
 *     WDDI Channel services
 *     WDDI channel initialization.
 *     Allocation and freeing are done by macros in wpi_registry.h
 *
 * Functions contained in this file:
 * +------------+----------+---------------------------------------------
 * |   Function name       | Description
 * +------------+----------+---------------------------------------------
 * |   API Functions
 * |   --------------------+---------------------------------------------
 * |   WP_ChannelCreate    | Create and initialize an ATM or packet channel
 * |   WP_ChannelDelete    | Deletes an ATM or packet channel
 * |   WP_ChannelEnable    | Enable (activate) an ATM or packet channel
 * |   WP_ChannelDisable   | Disable (deactivate) an ATM or packet channel
 * |   WP_ChannelModify    | Modify parameters of an active channel
 * |   WP_ChannelMove      | Moves a channel to a different device
 * |   WP_ChannelStatistics| Obtains statistics for an active channel
 * |   WP_ChannelQDepth    | Check an Rx ATM switching channel's current
 * |                       |   queue depth.
 * |   WP_ChannelStatisticsReset | Reset statistics for an active channel
 * |
 * |   Internal WDDI Functions
 * |   --------------------+---------------------------------------------
 * |   WPI_ChannelAllocate | Allocate a channel from the port group
 * |   WPI_ChannelHandleGet | Allocate channel handle from channel block
 * |   WPI_ChannelHandleFree| Free a channel handle
 * |   WPI_ChannelGroupInit | Initialze the channel block for a port group
 * |   WPI_SysChannelsGet   | Allocates channels from one list to another
 * |   WPI_SysChannelsCommit   | Allocates channels from one list to another
 * |   WPI_ChannelBlocksInit   | Allocates channels from one list to another
 * |   WPI_GetQatEntry   | Allocates channels from one list to another
 * |   WPI_L2ProtConvert   | Allocates channels from one list to another
 * +------------------------+---------------------------------------------
 ****************************************************************************/

#include <stdlib.h>
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
#ifndef WPI_ATM_H
#include "include/layer_2/atm/wpi_atm.h"
#endif
#ifndef WPI_ATMSW_H
#include "include/layer_2/atm/atmsw/wpi_atmsw.h"
#endif
#ifndef WPI_PACKET_H
#include "include/layer_2/packet/wpi_packet.h"
#endif
#ifndef WPI_SERVICES_H
#include "include/core/utility/wpi_services.h"
#endif
#ifndef WPI_TRANS_H
#include "include/layer_2/trans/wpi_trans.h"
#endif
#ifndef WPI_IW_CHANNEL_H
#include "include/iw/core/wpi_iw_channel.h"
#endif
#ifndef WPI_ENET_H
#include "include/layer_2/packet/enet/wpi_enet.h"
#endif
#ifndef WPI_FR_H
#include "include/layer_2/packet/fr/wpi_fr.h"
#endif
#ifndef WPI_HDLC_H
#include "include/layer_2/packet/hdlc/wpi_hdlc.h"
#endif
#ifndef WPI_DISPLAY_H
#include "include/core/wpi_display.h"
#endif
#ifndef WPI_STATISTICS_H
#include "include/core/wpi_statistics.h"
#endif
#ifndef WPI_STATIC_H
#include "include/core/registry/wpi_static.h"
#endif
#ifndef WPI_PORT_GROUP_INT_H
#include "include/core/wpi_port_group_int.h"
#endif
#ifndef WPI_PQ_H
#include "include/core/wpi_pq.h"
#endif
#ifndef WPI_PQ_INT_H
#include "include/core/wpi_pq_int.h"
#endif
#ifndef WPI_PORTS_INT_H
#include "include/core/wpi_ports_int.h"
#endif
#ifndef WPI_PORT_UPI_INT_H
#include "include/layer_1/upi/wpi_port_upi_int.h"
#endif
#ifndef WPI_PORT_ENET_INT_H
#include "include/layer_1/enet/wpi_port_enet_int.h"
#endif
#ifndef WPI_DEVICE_HSPI_INT_H
#include "include/layer_2/packet/hspi/wpi_device_hspi_int.h"
#endif
#ifndef WPI_DUMP_UTIL_H
#include "include/core/wpi_dump_util.h"
#endif
#ifndef WPI_MQ_INT_H
#include "include/core/wpi_mq_int.h"
#endif
#ifndef WPI_DEVICE_TRANS_H
#include "include/layer_2/trans/wpi_device_trans.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_VENEER_H
#include "veneer/wpi_veneer.h"
#endif
#ifndef WPI_REG_H
#include "include/core/hardware/wpi_reg.h"
#endif
#ifndef WPI_CHANNEL_H
#include "include/core/wpi_channel.h"
#endif
#ifndef WPI_HOST_IO_H
#include "include/core/host_io/wpi_host_io.h"
#endif
#if _WP_MODULE_DUMP_ENABLE_
#ifndef WPI_DUMP_H
#include "include/core/wpi_dump.h"
#endif
#endif
#ifndef WPI_MC_H
#include "include/layer_2/packet/wpi_mc.h"
#endif
#ifndef WPI_TIMING_H
#include "include/core/wpi_timing.h"
#endif
#ifndef WPI_DPS_FEATURES_H
#include "include/core/features/wpi_dps_features.h"
#endif
#ifndef WPI_MFR_H
#include "include/layer_2/packet/wpi_mfr.h"
#endif
#ifndef WPI_TRANS_CHANNEL_PWE3_H
#include "include/layer_2/trans/wpi_trans_channel_pwe3.h"
#endif
#ifndef WPI_TRANS_CHANNEL_ITDM_H
#include "include/layer_2/trans/itdm/wpi_trans_channel_itdm.h"
#endif
#ifndef WPI_TRANS_CH_TDM_SWITCH_H
#include "include/layer_2/trans/tdm_switch/wpi_trans_ch_tdm_switch.h"
#endif
#ifndef WPI_EMPHY_TRANS_H
#include "include/layer_2/trans/wpi_emphy_trans.h"
#endif
#ifndef WPI_IMA_INT_H
#include "include/layer_2/atm/ima/wpi_ima_int.h"
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif
#ifndef WPI_AAL2_H
#include "include/layer_2/atm/aal2/wpi_aal2.h"
#endif
#include "include/core/hardware/wpi_hw.h"
#ifndef WP_PACKET_H
#include "api/wp_packet.h"
#endif
#ifndef WPI_IW_H
#include "include/iw/core/wpi_iw.h"
#endif
#ifndef WPI_IW_INT_H
#include "include/iw/core/wpi_iw_int.h"
#endif
#ifndef WPI_IW_TX_BINDING_H
#include "include/iw/core/wpi_iw_tx_binding.h"
#endif
#ifndef WPI_IW_BRIDGING_H
#include "include/iw/bridging/wpi_iw_bridging.h"
#endif

#include "include/core/wpi_port_group_class.h"
#include "include/core/wpi_port_limit_class.h"
#include "include/layer_2/atm/ima/wpi_ima_class.h"
#include "include/layer_2/atm/ima/wpi_ima_private.h"
#include "include/layer_2/atm/wpi_atm_class.h"
#include "include/iw/core/wpi_iw_global_class.h"

#include "include/core/wpi_port_group_private.h"
#include "include/core/wpi_port_limit_private.h"
#include "include/layer_2/atm/gbond/wpi_gbond_class.h"
#include "include/core/wpi_priority_queue_class.h"

#if _WP_MODULE_DUMP_ENABLE_
static WP_CHAR *wpi_transch_test_string[] = {
   "WP_TRANSCH_TEST_DISABLE",
   "WP_TRANSCH_TEST_ENABLE"
};

static WP_CHAR *wpi_aal0_ch_crcmode_string[] = {
   "WP_AAL0_CH_CRCMODE_IGNORE",
   "WP_AAL0_CH_CRCMODE_CHECK"
};

static WP_CHAR *wpi_trans_tx_udp_string[] = {
   "WP_TRANS_TX_LAST_FRAME",
   "WP_TRANS_TX_USER_DEFINED_FRAME"
};

static WP_CHAR *wpi_aal0_ch_insert_string[] = {
   "WP_AAL0_CH_INSERT_DISABLE",
   "WP_AAL0_CH_INSERT_ENABLE"
};

static WP_CHAR *wpi_atmch_test_string[] = {
   "WP_ATMCH_TEST_DISABLE",
   "WP_ATMCH_TEST_ENABLE"
};

static WP_CHAR *wpi_pktch_test_string[] = {
   "WP_PKTCH_TEST_DISABLE",
   "WP_PKTCH_TEST_ENABLE"
};

static WP_CHAR *wpi_transch_int_string[] = {
   "WP_TRANSCH_INT_DISABLE",
   "WP_TRANSCH_INT_ENABLE"
};

static WP_CHAR *wpi_ch_mode_string[] = {
   "WP_CH_RX",
   "WP_CH_TX"
};
#endif

/*****************************************************************************
 *
 * Function:   WPI_ChannelAllocate
 *
 * Purpose:    Allocate a new channel in the WDDI registry
 *
 * Description:
 *     This function allocates a Rx or Tx channel for ATM or packet
 *     traffic for the specified device type and creates and returns
 *     a handle for the new channel.
 *
 *
 * Inputs:
 *
 *     device           Handle for the already initialized device
 *                      with which this channel is to be associated
 *
 *     mode             Indicates direction of channel (Rx or Tx)
 *
 *     protocol         Indicates protocol associated with the channel.
 *
 *     ch_return_block  The address of the channel_block is
 *                       stored at this address
 *
 *
 * Outputs:
 *     The function only creates the new handle and records the new
 *     channel in the registry.  The calling function initializes
 *     actual channel parameters via the returned ch_return_block.
 *
 * Return Value:
 *     Upon successful completion, the function returns a WDDI handle
 *     for the allocated channel.
 *     Error codes: WP_ERR_PROTOCOL, WP_ERR_NO_FREE_CHANNEL
 *
 * Called by:
 *     WP_ChannelCreate (wpi_channels.c)
 *
 ****************************************************************************/
WP_handle WPI_ChannelAllocate(WP_handle device,
                              WP_handle virtual_device_handle,
                              WP_mode mode,
                              WP_protocol protocol,
                              wpi_res_channel_block **ch_return_block)
{
   /******************************
    * Local variable declarations
    ******************************/

   /* Registry structures */
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_channel_block *ch_block;
   wpi_res_channel_both *channel_data;
   wpi_channel_list *list;

   WP_U32 wpid;
   WP_U32 pg;
   WP_U32 handle_type;
   WP_U32 group_offset;
   WP_U32 dps_limit;

   /* The index relative to the beginning of the port group */
   WP_U32 dps_index;

   /* The index relative to the beginning of the channel block */
   WP_U32 handle_index;
   WP_U8 flowmode = WPI_DEVICE_PKT_FLOWMODE_MULTI;
   WP_U32 phy;
   WP_U8 enet_flag = WP_FALSE;

   wpi_res_enet *res_enet;
   wpi_resource_entry *entry = NULL;
   wpi_resource_block *resources = NULL;
   wpi_resource_entry *dev_entry;
   wpi_res_device *dev;


   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* Get the WinPath ID from device handle */
   wpid = WPI_HANDLE_WPID(device);

   /*
    * Determine the registry block from which to allocate the channel
    * and then complete the allocation by creating and returning a channel
    * handle.
    *
    * Rx channels are maintained separately from Tx channels.
    *
    * DPS Tx channel numbers are allocated separately per port group.
    * DPS Rx channel numbers are allocated from either the atm group or
    * the packet group.
    *
    * The device handle subtype is used to determine the group.
    */
   if (WPI_HANDLE_LAG_VIRTUAL_DEVICE_P(device))
   {
      pg = WPI_pg_pkt_link_aggregation;
   }
   else if(WPI_HANDLE_EFM_BONDING_OVER_ENET_VIRTUAL_DEVICE_P(device))
   {
      pg = WPI_pg_pkt_efm_bonding_enet;
   }
   else    
   {
      pg = WPI_HANDLE_SUBTYPE(device);
   }

   if (mode == WP_CH_RX)
   {
      /* Rx Channel -- translate port group to shared port group */
      ch_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
      handle_type = WPI_handle_rx_channel;
      switch (pg)
      {
         case WPI_pg_atm_upi1:
         case WPI_pg_atm_upi2:
         case WPI_pg_atm_upi3:
         case WPI_pg_atm_upi4:
         case WPI_pg_atm_tdi:
            pg = WPI_pg_atm_shared;
            break;
            /* for packet ports, pg is not shared */
         case WPI_pg_pkt_upi1:
         case WPI_pg_pkt_upi2:
         case WPI_pg_pkt_upi3:
         case WPI_pg_pkt_upi4:
            /* Receive channels for HDLC for UPI1, UPI2, UPI3, UPI4
               are allocated according to PHY number */
            if (protocol == WP_HDLC)
               flowmode = WPI_DEVICE_PKT_FLOWMODE_SINGLE;
            if (WPI_ATM_PROTOCOL_P(protocol))
               pg = WPI_pg_atm_shared;
            break;
         case WPI_pg_hspi_hspos1:
         case WPI_pg_hspi_hspos2:
         case WPI_pg_hspi_hspos3:
         case WPI_pg_hspi_hspos4:
            flowmode = WPI_DEVICE_PKT_FLOWMODE_SINGLE;
            /* for hspos channels use the channels allocated in the packet port group */
            pg = (pg - WPI_pg_hspi_hspos1) + WPI_pg_pkt_upi1;
            break;
         case WPI_pg_pkt_tdi:
            /* Receive channels for HDLC for TDM ports are
               allocated according to TDI number */
            if ((protocol == WP_HDLC) &&
                ((WPI_HANDLE_INDEX(device) < WPI_ima_base) ||
                 (WPI_HANDLE_INDEX(device) >= WPI_mch_dev_base &&
                  WPI_HANDLE_INDEX(device) <= WPI_mch_dev_last))
               )
               flowmode = WPI_DEVICE_PKT_FLOWMODE_SINGLE;
            break;
         case WPI_pg_pkt_enet_g999:
         case WPI_pg_pkt_enet1:
         case WPI_pg_pkt_enet2:
         case WPI_pg_pkt_enet3:
         case WPI_pg_pkt_enet4:
         case WPI_pg_pkt_efm_bonding_enet:
            if ( (protocol == WP_ATM_AAL5) ||
                 (protocol == WP_ATM_AAL0) ||
                 (protocol == WP_ATM_OAM) )
            {
               /* creating AAL5 Rx channel over G999 device, i.e. Ethernet device.
                * flowmode will be WPI_DEVICE_PKT_FLOWMODE_MULTI because there can
                * more than 1 Rx channel over each ATM stream. */
               pg = WPI_pg_atm_shared;
            }
            else
            {
               enet_flag = WP_TRUE;
               flowmode = WPI_GetDeviceFlowmode(device);
               if (pg == WPI_pg_pkt_enet_g999)
               {
                  /* handle G999 channels allocation
                   * as in a regular enet*/
                  pg = WPI_pg_pkt_enet1;
               }
            }
            break;
         case WPI_pg_tdi_trans:
         case WPI_pg_pkt_iw_host:
         case WPI_pg_mfr_rx:
         case WPI_pg_emphy_trans:
            break;
         default:
            return WPI_ERROR(WP_ERR_PROTOCOL);
      }
   }
   else
   {
      /************ Tx Channel ***************/

      if (protocol == WP_ATM_OAM)
         return WPI_ERROR(WP_ERR_PROTOCOL);


      handle_type = WPI_handle_tx_channel;
      ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;

      switch (pg)
      {
         case WPI_pg_atm_upi1:
         case WPI_pg_atm_upi2:
         case WPI_pg_atm_upi3:
         case WPI_pg_atm_upi4:
         case WPI_pg_atm_tdi:
            pg = WPI_pg_atm_shared;
            break;
         case WPI_pg_pkt_enet_g999:
         case WPI_pg_pkt_enet1:
         case WPI_pg_pkt_enet2:
         case WPI_pg_pkt_enet3:
         case WPI_pg_pkt_enet4:

            
            resources = &wpi_registry_ptr->pwp_resources[wpid];
            dev_entry = &resources->entry[WPI_HANDLE_INDEX(device)];
            dev = WPI_RES_DEV(dev_entry);
            WPI_ACCESS_RES(entry, resources, WPI_HANDLE_INDEX(dev->port));
            res_enet = WPI_RES_ENET(entry);
            if (!res_enet->used_by_lag)
               enet_flag = WP_TRUE;
            // fallthrough
         case WPI_pg_pkt_link_aggregation:
         case WPI_pg_pkt_efm_bonding_enet:
            if (pg == WPI_pg_pkt_efm_bonding_enet)
            {
               enet_flag = WP_TRUE;
            }
            if (pg == WPI_pg_pkt_enet_g999)
            {
               /* handle G999 channels allocation
                * as in a regular enet*/
               pg = WPI_pg_pkt_enet1;
            }
            if (protocol == WP_ATM_AAL5)
            {
               /* creating AAL5 channel over G999 device, i.e. Ethernet device. */
               pg = WPI_pg_atm_shared;
            }
            // fallthrough
         case WPI_pg_pkt_upi1:
         case WPI_pg_pkt_upi2:
         case WPI_pg_pkt_upi3:
         case WPI_pg_pkt_upi4:
         case WPI_pg_pkt_tdi:
            flowmode = WPI_GetDeviceFlowmode(device);
            if (protocol == WP_ATM_AAL5)
               pg = WPI_pg_atm_shared;
            break;
         case WPI_pg_hspi_hspos1:
         case WPI_pg_hspi_hspos2:
         case WPI_pg_hspi_hspos3:
         case WPI_pg_hspi_hspos4:
            /* for hspos channels use the packet channels allocated to the upi - UPI packet port group */
            flowmode = WPI_GetDeviceFlowmode(device);
            pg = (pg - WPI_pg_hspi_hspos1) + WPI_pg_pkt_upi1;
           break;
         case WPI_pg_tdi_trans:
         case WPI_pg_pkt_iw_host:
         case WPI_pg_emphy_trans:
         case WPI_pg_pkt_efm_bonding:
            break;
         default:
            return WPI_ERROR(WP_ERR_PROTOCOL);
      }
   }

   /* Save for the caller the address of the channel block */
   *ch_return_block = ch_block;

   /* Find the base address of all channels */
   channel_data = ch_block->channel_data;
   if(channel_data == NULL)
      return WPI_ERROR(WP_ERR_WDDI);

   /* and the list describing the specified port group. */
   if (protocol == WP_ATM_AAL1)
      pg = WPI_pg_atm_aal1;

   list = &ch_block->channel_list[pg];

   /* Calculate the port group offset and limit */
   group_offset = list->group_offset;
   dps_limit = list->dps_limit;

   if (protocol == WP_ATM_OAM)
   {
      handle_index = group_offset;
      if (WPI_INITIALIZED(channel_data[handle_index].mod.state))
         return WPI_ERROR(WP_ERR_NO_FREE_CHANNEL);
      WPI_LOG(WPI_DEBUG_LEVEL_3,
              (wpi_log,
               "ChannelAllocate2 0x%x pg:%d offset:%d "
               "handle_index:%d\n",
               (WP_U32) ch_block, pg, group_offset, handle_index));
   }
   else if (enet_flag == WP_TRUE &&
            (flowmode == WPI_DEVICE_PKT_FLOWMODE_FAST ||
             (flowmode == WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL &&
              mode == WP_CH_RX)))
   {
      wpi_res_device_hspi_enet *dev_enet;

      resources = &wpi_registry_ptr->pwp_resources[wpid];
      dev_entry = &resources->entry[WPI_HANDLE_INDEX(device)];
      dev_enet = WPI_RES_DEV_HSPI_ENET(dev_entry);

      status = WPI_HwEnetChannelAllocate(dev_enet,
                                         virtual_device_handle,
                                         mode,
                                         group_offset,
                                         channel_data,
                                         list,
                                         &handle_index);
      WPI_RETURN_IF_ERROR_STATUS(status);

      WPI_LOG(WPI_DEBUG_LEVEL_3,
              (wpi_log,
               "ChannelAllocate3 0x%x pg:%d offset:%d "
               "handle_index:%d\n",
               (WP_U32) ch_block, pg, group_offset, handle_index));
   }
   else if (flowmode == WPI_DEVICE_PKT_FLOWMODE_SINGLE)
   {
      phy = WPI_GetDevicePhy(device);
      handle_index = phy + group_offset;

      if (phy > dps_limit)
         return WPI_ERROR(WP_ERR_CHANNEL);

      if (WPI_INITIALIZED(channel_data[handle_index].mod.state))
         return WPI_ERROR(WP_ERR_NO_FREE_CHANNEL);

      WPI_LOG(WPI_DEBUG_LEVEL_3,
              (wpi_log,
               "ChannelAllocate4 0x%x pg:%d offset:%d dpsindex:%d "
               "handle_index:%d\n",
               (WP_U32) ch_block, pg, group_offset, phy, handle_index));
   }
   else
   {
      /* when the flow mode is WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE
         and the mode is WP_CH_TX the allocation is like in WP_FLOWMODE_MULTI */

      /* Allocate if head != tail */
      if (list->head != list->tail)
      {
         dps_index = list->head;
         handle_index = dps_index + group_offset;

         /* *INDENT-OFF* */
         WPI_LOG(WPI_DEBUG_LEVEL_3,
                 (wpi_log,
                  "ChannelAllocate5 0x%x pg:%d offset:%d dpsindex:%d "
                  "handle_index:%d\n",
                  (WP_U32) ch_block, pg, group_offset, dps_index,
                  handle_index));

         /* *INDENT-ON* */
         list->head = channel_data[handle_index].mod.du.next;
      }
      else
      {
         return WPI_ERROR(WP_ERR_NO_FREE_CHANNEL);
      }
      
   }

   /* this is work around in order not to pass another variable to this function.       *
    * the WPI_FAST_MIXED_ENET indicates that the channel is fast channel but assosiated *
    * to MIXED flowmode device, this define can't be another valid protocol because it  *
    * equals to the number of protocol in enum WP_PROTOCOL                              */
   if(protocol == WPI_FAST_MIXED_ENET)
      protocol = WP_ENET;

   return WPI_MAKE_HANDLE(handle_type, protocol, wpid, handle_index);
}

/*****************************************************************************
 *
 * Function:   WPI_ChannelHandleGet
 *
 * Purpose:    Allocate a channel handle
 *
 * Description:
 *     This function allocates a Rx or Tx channel.
 *
 * Inputs:
 *
 *     handle_type         Internal WDDI handle type
 *     protocol            Protocol of the channel
 *     wpid                WinPath ID
 *     ch_block            Pointer to registry channel block info
 *     pg                  port group information
 *
 * Outputs:
 *
 *
 * Return Value:
 *     WP_handle           channel handle
 *     Error code:  WP_ERR_NO_FREE_CHANNEL
 *
 * Called by:
 *     WP_ChannelCreate (Api function)
 *
 ***************************************************************************/
WP_handle WPI_ChannelHandleGet(WP_U32 handle_type,
                               WP_U32 protocol,
                               WP_U32 wpid,
                               wpi_res_channel_block *ch_block,
                               WP_U32 pg)
{
   /* Registry structures */
   wpi_res_channel *channel_data;
   wpi_channel_list *list = &ch_block->channel_list[pg];

   /* Local variables */
   WP_U32 group_offset;
   WP_U32 dps_index;
   WP_U32 handle_index;

   WPI_INTERNAL_FUNCTION();
   /* Calculate the port group offset */
   group_offset = list->group_offset;

   /* Allocate if head != tail */
   if (list->head != list->tail)
   {
      dps_index = list->head;
      handle_index = dps_index + group_offset;
      channel_data = &(ch_block->channel_data[handle_index].mod);
      /* *INDENT-OFF* */
      WPI_LOG(WPI_DEBUG_LEVEL_3,(
              wpi_log,
              "ChannelAllocate 0x%x pg:%d offset:%d "
              "dpsindex:%d handle_index:%d\n",
              (WP_U32) ch_block, pg, group_offset, dps_index, handle_index));
      /* *INDENT-ON* */
      list->head = channel_data->du.next;
      /* Set the pg into its field in the state variable.
       * This is used at freeing time to find where to return the handle. */
      channel_data->state = WPI_CHANNEL_STATE_PORT_INDEX_F(pg);
      return WPI_MAKE_HANDLE(handle_type, protocol, wpid, handle_index);
   }
   return WPI_ERROR(WP_ERR_NO_FREE_CHANNEL);
}

/*****************************************************************************
 *
 * Function:   WPI_ChannelHandleFree
 *
 * Purpose:    Return a channel handle, given the pointer to its ch_block.
 *
 * Description:
 *     This function frees an Rx or Tx channel.
 *
 * Inputs:
 *
 *     handle              handle of channel
 *
 * Outputs:
 *     The function returns the channel number to its port group.
 *
 * Return Value:
 *     WP_OK if completed successfully
 *
 * Called by:
 *     WP_ChannelDelete (wpi_channels.c)
 *
 ****************************************************************************/
WP_status WPI_ChannelHandleFree(WP_handle handle)
{
   /* Registry structures */
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_channel_block *ch_block;
   wpi_res_channel_both *channel_data;
   wpi_channel_list *list;

   /* Local variables */
   WP_U32 wpid = WPI_HANDLE_WPID(handle);
   WP_U32 handle_index = WPI_HANDLE_INDEX(handle);
   WP_U32 group_offset;
   WP_U32 pg;
   WP_U32 protocol = WPI_HANDLE_SUBTYPE(handle);

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* Find the block (tx or rx) from the handle type */
   if (WPI_HANDLE_TYPE(handle) == WPI_handle_rx_channel)
      ch_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
   else
      ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;

   /* Find the base of all channels for tx/rx */
   channel_data = ch_block->channel_data;

   /* Find the port group (in the channel state) */
   if (WPI_HANDLE_SUBTYPE(handle) == WP_ATM_AAL1)
      pg = WPI_pg_atm_aal1;
   else
   {
      status = WPI_ChannelPortGroupIndexGet(&pg, handle);
      WPI_RETURN_IF_ERROR_STATUS(status);
   }

   if(pg == WPI_pg_hspi_hspos1 || pg == WPI_pg_hspi_hspos2)
   {
      WP_U8 l2_group;
      
      if (WPI_HANDLE_TYPE(handle) == WPI_handle_tx_channel)
      {         
         l2_group = WPI_FmuL2GroupModeGet(channel_data[handle_index].mod.du.pkt.type.hdlc.l2_group);
         if(l2_group == WP_MODE_HW)
         {
            if (pg == WPI_pg_hspi_hspos1)
               pg = WPI_pg_pkt_upi1;
            else /* if (pg == WPI_pg_hspi_hspos2) */
               pg = WPI_pg_pkt_upi2;
         }
      }      
   }

   WPI_RES_CHANNEL_DEVICE( &channel_data[handle_index].mod) = WPI_NULL_HANDLE;

   if (WPI_PQ_ENABLED(channel_data[handle_index].mod.state) ||
       WPI_MQ_ENABLED(channel_data[handle_index].mod.state) ||
       WPI_CH_SINGLE_PER_PHY(channel_data[handle_index].mod.state) ||
       (protocol == WP_ATM_OAM))
   {
      /* clear state information */
      channel_data[handle_index].mod.state = 0;
      /* clear iw state information */
      channel_data[handle_index].mod.iw_state = 0;

      return WP_OK;
   }

   /* clear state information */
   channel_data[handle_index].mod.state = 0;
   /* clear iw state information */
   channel_data[handle_index].mod.iw_state = 0;

   /* Find the list describing the port group */
   list = &ch_block->channel_list[pg];

   /* Find the group offset for the port group */
   group_offset = list->group_offset;

   /* Make the previous tail point to the returned channel */
   channel_data[group_offset + list->tail].mod.du.next = handle_index
      - group_offset;

   WPI_LOG(WPI_DEBUG_LEVEL_3,
           (wpi_log,
            "ChannelHandleFree 0x%x pg:%d offset:%d tail:%d "
            "handle_index:%d\n",
            (WP_U32) ch_block, pg, group_offset, list->tail,
            handle_index));
   /* Make the tail point to the returned channel */
   list->tail = handle_index - group_offset;

   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WPI_ChannelGroupInit
 *
 * Purpose:    Set up the list structure for a port group's queue.
 *
 * Description: This function intializes the channel block for a
 *              specific port group.
 *
 * Inputs:
 *     ch_block   Channel Block for a specific port group
 *     pg         Port group
 *
 * Outputs:
 *     The function initializes the channel block list for the port
 *     group
 *
 * Return Value:
 *     WP_OK
 *     Error codes: None
 *
 * Called by:
 *     WPI_ChannelBlocksInit (wpi_channels.c)
 *
 ****************************************************************************/
WP_handle WPI_ChannelGroupInit(wpi_res_channel_block *ch_block,
                               WP_U32 pg)
{
   /* Registry structures */
   wpi_channel_list *list = &ch_block->channel_list[pg];
   wpi_res_channel *channel_data;

   /* Local variables */
   WP_U32 group_offset = list->group_offset;
   WP_U32 dps_next = list->dps_next;
   WP_U32 dps_limit = list->dps_limit;
   WP_U32 i;

   WPI_INTERNAL_FUNCTION();
   /* Find the base of all channels for the group */

  list->head = dps_next;
   if (dps_next < dps_limit)
      list->tail = dps_limit - 1;
   else
      list->tail = dps_next;    /* prevents allocation */

   for (i = dps_next; i < dps_limit; i++)
   {
      channel_data = &(ch_block->channel_data[group_offset + i].mod);
      channel_data->du.next = i + 1;
   }


   /* *INDENT-OFF* */
   WPI_LOG(WPI_DEBUG_LEVEL_3,(
           wpi_log,
           "WPI_ChannelGroupInit 0x%8.8x pg:%d ch:%d:%d list:%d:%d\n",
           (WP_U32) ch_block, pg, dps_next, dps_limit,
           list->head, list->tail));
   /* *INDENT-ON* */
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelCreate
 *
 * Purpose:    Create a new ATM or packet channel
 *
 *
 * Description:
 *      This is the user-called function to create a new ATM or packet
 *      channel.  Separate channels are created for Rx and Tx traffic.
 *      Parmaters include a user-configured structure of channel parameters
 *      and a user-specified tag that can be later used to identify
 *      the channel when certain host-level events (such as interrupts for
 *      received traffic) occur.
 *
 *      Most of the actual work is done by lowr level routines called
 *      by this function.
 *
 *
 * Inputs:
 *
 *     tag           User-assigned tag associated with this channel.
 *
 *     device        Handle for the already initialized device
 *                   with which this channel is to be associated
 *
 *     qnode         Handle for the queue node with which this channel
 *                   is to be associated
 *
 *     mode          Indicates direction of channel (Rx or Tx)
 *
 *     protocol      Indicates protocol associated with the channel.
 *
 *     config        Pointer to a user-configured structure containing
 *                   channel attributes.  Structure type varies depending
 *                   on protocol.
 *
 * Outputs:
 *     Assigns a channel number and records channel parameters in
 *     registry and DPS data structures as appropriate.
 *
 * Return Value:
 *     Upon successful completion, the function returns a WDDI handle
 *     for the created channel.
 *     Error codes: WP_ERR_DEVICE, WP_ERR_QNODE, WP_ERR_PROTOCOL
 *                  WP_ERR_CONFIG, WP_ERR_MODE, WP_ERR_NO_FREE_CHANNEL,
 *                  WP_ERR_INCORRECT_REGISTRY_STATE
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_handle WP_ChannelCreate(WP_tag tag,
                           WP_handle device,
                           WP_handle qnode,
                           WP_mode mode,
                           WP_protocol protocol,
                           void *config)
{
   /******************************
    * Local variable declarations
    ******************************/

   WP_status status = WP_OK, lock_status = WP_OK;
   WP_handle channel = 0;
   WP_U32 wpid;
   WP_handle dev_handle = WPI_NULL_HANDLE;
   wpi_res_device *res_dev;
   wpi_res_registry *wpi_registry_ptr;

   /* First, check to see that function parameters are valid */
   WPI_LOG(WPI_DEBUG_LEVEL_9,
           (wpi_log, "WP_ChannelCreate function entered.\n"));

   /* Check if device handle is valid */
   if ((!WPI_HANDLE_DEVICE_P(device)) &&
       (!WPI_HANDLE_VIRTUAL_DEVICE_P(device)) &&
       (!WPI_HANDLE_GBOND_GROUP_P(device)) &&
       (!WPI_HANDLE_IMA_P(device)) &&
       (!WPI_HANDLE_SHAPING_GROUP_P(device)))
      return WPI_ERROR(WP_ERR_DEVICE);

   /* Check if queue node handle is valid */
   if (!WPI_HANDLE_Q_NODE_P(qnode))
      return WPI_ERROR(WP_ERR_QNODE);
   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }

   if (WPI_REGISTRY_USE() == WP_FALSE)
      WPI_BREAK(status, WP_ChannelCreateEnd,
                WP_ERR_INCORRECT_REGISTRY_STATE);

   wpid = WPI_HANDLE_WPID(device);
   /*
    * Create the appropriate protocol-specific channel for the
    * specified device, using user-supplied channel parameters.
    */

   switch (protocol)
   {
      case WP_TRANSPARENT:
         if (!WPI_DpsFeatureCheckStatus(wpid,
                                        WPI_FEATUREMODE_DEVICE_HANDLE,
                                        device,
                                        WPI_DPS_MODULE_TRANSPARENT_ENABLE))
         {
            WPI_BREAK(status, WP_ChannelCreateEnd, WP_ERR_DPS_FEATURE_UNAVAILABLE);
         }

         /* Create a Transparent channel */
         status = WPI_TransChannelCreate(&channel, tag, device, qnode, mode,
                                         protocol, (WP_ch_trans *) config);
         break;
      case WP_TRANSPARENT_PWE3:
         /* Create a Transparent Pwe3 channel */
         status = WPI_TransPwe3ChannelCreate(&channel, tag, device, qnode, mode,
                                             (WP_ch_trans_pwe3 *) config);
         break;
      case WP_TRANSPARENT_ITDM:
         /* Create a Transparent ITDM channel */
         status = WPI_TransItdmChannelCreate(&channel, tag, device, qnode, mode,
                                             (WP_ch_trans_itdm *) config);
         break;
      case WP_TRANSPARENT_TDM_SWITCH:
         /* Create a Transparent TDM Switch channel */
         status = WPI_TransTdmSwitchChannelCreate(&channel, tag, device, qnode, mode,
                                                  config);
         break;
      case WP_IW_HOST:
      {
         if (WPI_HANDLE_SUBTYPE(device) != WPI_pg_pkt_iw_host)
         {
            status = WPI_ERROR(WP_ERR_DEVICE);
         }
         else
         {
            if (!WPI_DpsFeatureCheckStatus(
                   wpid,
                   WPI_FEATUREMODE_DEVICE_HANDLE,
                   device,
                   WPI_DPS_MODULE_INTERWORKING_HOST_TERMINATION_ENABLE))
            {
               WPI_BREAK(status, WP_ChannelCreateEnd, WP_ERR_DPS_FEATURE_UNAVAILABLE);
            }

            /* Create an interworking host termination channel */
            status = WPI_IwChannelCreate(&channel, tag, device, qnode, mode,
                                         protocol, config);
         }
         break;
      }
      case WP_IW_LOG:
      {
         if (WPI_HANDLE_SUBTYPE(device) != WPI_pg_pkt_iw_host)
         {
            status = WPI_ERROR(WP_ERR_DEVICE);
         }
         else
         {
            if (!WPI_DpsFeatureCheckStatus(
                   wpid,
                   WPI_FEATUREMODE_GLOBAL,
                   device,
                   WPI_DPS_MODULE_INTERWORKING_DFC_BRIDGE_LOGGING_ENABLE))
            {
               WPI_BREAK(status, WP_ChannelCreateEnd, WP_ERR_DPS_FEATURE_UNAVAILABLE);
            }
            status = WPI_IwLogChannelCreate(&channel, tag, device, qnode);
         }
      }
      break;
      default:
      {
         wpi_res_enet_multi_class *res_enet_multi_class = NULL;
         if (WPI_HANDLE_SHAPING_GROUP_P(device))
         {
            /* shaping groups hold the device handle in their resource */
            if(WPI_HANDLE_SUBTYPE(device) == WP_L2_GROUP)
               dev_handle = WPI_FmuL2GroupDeviceHandleGet(device);
            else
               WPI_BREAK (status, WP_ChannelCreateEnd, WP_ERR_HANDLE);
         }
         else
            dev_handle = device;

         if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(dev_handle))
         {
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryEnetMultiClassGet,
                                    WPI_REG_LOCKED,
                                    &res_enet_multi_class,
                                    wpid);
            WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
            res_dev = WPI_MULTI_CLASS_DEV_BYHANDLE(res_enet_multi_class,
                                                   dev_handle);
         }
         else
         {
            WPI_REGISTRY_DEVICE_GET_BYHANDLE(lock_status,
                                             &res_dev,
                                             dev_handle);
            WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
         }

         if (!res_dev)
         {
            status = WPI_ERROR(WP_ERR_WDDI);
         }
         else
         {
             if (res_dev->functions && res_dev->functions->channel_create)
             {
             /* WPI_PktChannelCreate
              * WPI_AtmChannelCreate
              * WPI_EfmBondingChannelCreate
              * WPI_EfmBondingOverEnetChannelCreate
              * WPI_LaChannelCreate */

                status =
                    res_dev->functions->channel_create(&channel, tag,
                                                       device, qnode,
                                                       mode, protocol, config);
             }
             else
                status = WPI_ERROR(WP_ERR_PROTOCOL);
         }
         if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(dev_handle))
         {
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryEnetMultiClassRelease,
                                        WPI_REG_UNLOCKED,
                                        &res_enet_multi_class);
            WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
         }
      }
   }
   if (protocol != WP_IW_HOST &&
       protocol != WP_IW_LOG &&
       status == WP_OK)
   {
      if(WPI_HANDLE_SHAPING_GROUP_P(device))
      {
         /* ENET channel can be created with L2 shaping group handle, in
            that case, the device handle need to be fetch from the L2 resource */
         WP_handle dev_handle;

         dev_handle = WPI_FmuL2GroupDeviceHandleGet(device);
         status = WPI_DeviceAssignChannel(dev_handle, channel);
      }
      else
         status = WPI_DeviceAssignChannel(device, channel);
   }

   if (status == WP_OK)
   {
      wpi_res_channel_block *ch_block;
      wpi_res_channel *ch_res = NULL;
      WP_U32 ch_num;

      if (protocol != WP_IW_LOG && protocol != WP_IW_HOST)
      {
         if (mode == WP_CH_RX)
         {
            ch_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
            ch_num = WPI_HANDLE_INDEX(channel);
            WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);
            ch_res->rx_priority = WP_IW_RX_BIND_PRIORITY_DISABLED;
            ch_res->qnode_handle = qnode;
            ch_res->wred_stat_ptr = 0;
            ch_res->iw_state &= ~WPI_IW_STATE_WRED_STAT_BIND_ENABLE;
         }
         else if (mode == WP_CH_TX)
         {
            ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
            ch_num = WPI_HANDLE_INDEX(channel);
            WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);
            ch_res->qnode_handle = qnode;
            ch_res->wred_stat_ptr = 0;
            ch_res->iw_state &= ~(WPI_IW_STATE_WRED_STAT_BIND_ENABLE |
                                      WPI_IW_STATE_HW_WRED_STATE |
                                      WPI_IW_STATE_HW_WRED_BIND_ENABLE|
                                      WPI_IW_CH_STATE);
         }
      }
      WPI_WDDI_DETACH();
      return channel;
   }

   /* An error occurred, so free up the channel handle and return an error */
   WPI_LOG(WPI_DEBUG_LEVEL_3,
           (wpi_log, "WP_ChannelCreate returns %8.8x\n", status));
   if (WPI_HANDLE_TX_CHANNEL_P(channel) || WPI_HANDLE_RX_CHANNEL_P(channel))
      WPI_ChannelHandleFree(channel);
   WPI_LOG(WPI_DEBUG_LEVEL_9,
           (wpi_log, "WP_ChannelCreate function exited.\n"));

  WP_ChannelCreateEnd:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelDelete
 *
 * Purpose:    Deletes a ATM or packet channel
 *
 * Description:
 *    This function deletes and removes a channel from the registry
 *    For each protocol, a protocol specific function is called.
 *
 * Inputs:
 *
 *    handle   WDDI handle to channel to be deleted
 *
 * Outputs:
 *    Channel resource is deleted from the registry
 *
 * Return Value:
 *    WP_OK
 *    Error codes: WP_ERR_INCORRECT_REGISTRY_STATE,
 *                 WP_ERR_CHANNEL, WP_ERR_HANDLE, WP_ERR_PROTOCOL
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WP_ChannelDelete(WP_handle channel)
{
   wpi_res_registry *wpi_registry_ptr;
   wpi_res_channel_block *ch_block;
   wpi_res_channel *ch_res = NULL;
   WP_U32 protocol, ch_num, wpid;
   WP_mode mode;
   WP_status status = WP_OK, lock_status = WP_OK;

   /* Check if channel handle is valid.  If not, error code is in handle */
   if (!WPI_HANDLE_RX_CHANNEL_P(channel) && !WPI_HANDLE_TX_CHANNEL_P(channel))
      return WPI_ERROR(WP_ERR_CHANNEL);

   protocol = WPI_HANDLE_SUBTYPE(channel);
   wpid = WPI_HANDLE_WPID(channel);
   ch_num = WPI_HANDLE_INDEX(channel);

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
      WPI_BREAK(status, WP_ChannelDeleteExit,
                WP_ERR_INCORRECT_REGISTRY_STATE);

   /* Find the block (tx or rx) from the handle type */
   if (WPI_HANDLE_TYPE(channel) == WPI_handle_rx_channel)
   {
      ch_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
      mode = WP_CH_RX;
   }
   else
   {
      ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
      mode = WP_CH_TX;
   }

   WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);
   if (ch_res == NULL)
      WPI_BREAK(status, WP_ChannelDeleteExit, WP_ERR_HANDLE);

   if (!WPI_INITIALIZED(ch_res->state))
      WPI_BREAK(status, WP_ChannelDeleteExit, WP_ERR_CH_NOT_INITIALIZED);

   if( WPI_IW_STATE_RX_BIND_ENABLE_P( ch_res->iw_state))
   {
      WPI_BREAK(status, WP_ChannelDeleteExit,
                WP_ERR_IW_CHANNEL_BOUND);
   }

   switch (protocol)
   {
      case WP_HDLC:
         status = WPI_HdlcChannelDelete(ch_res,
                                        mode,
                                        protocol,
                                        wpid, WPI_HDLC_USAGE_NORMAL, 0, 0);
         break;
      case WP_TRANSPARENT:
         status = WPI_TransChannelDelete(channel, ch_res, mode);
         break;
      case WP_TRANSPARENT_PWE3:
         status = WPI_TransPwe3ChannelDelete(channel, ch_res, mode);
         break;
      case WP_TRANSPARENT_ITDM:
         status = WPI_TransItdmChannelDelete(channel, ch_res, mode);
         break;
      case WP_TRANSPARENT_TDM_SWITCH:
         status = WPI_TransTdmSwitchChannelDelete(channel, ch_res, mode);
         break;
      default:
         if ((ch_res->functions) && (ch_res->functions->channel_delete))
         {
            /*
             * WPI_IwChannelDelete
             * WPI_EnetChannelDelete
             * WPI_AtmChannelDelete
             * WPI_FrChannelDelete
             * WPI_EfmBondingChannelDelete
             * WPI_LaChannelDelete
             */
            status = ch_res->functions->channel_delete(channel, ch_res, mode);
         }
         else
            WPI_BREAK(status, WP_ChannelDeleteExit,
                      WP_ERR_PROTOCOL);
         break;
   }

   if (!WP_ERROR_P(status))
   {
      if (WPI_IW_STATE_WRED_STAT_BIND_ENABLE_P(ch_res->iw_state))
      {
         wpi_res_wred_stat_block *wred_stat_block = NULL;

         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryWredStatBlockGet,
                                 WPI_REG_LOCKED,
                                 &wred_stat_block,
                                 wpid);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
         if(wred_stat_block != NULL)
         {
            WPI_FREE_WRED_STAT(wred_stat_block,ch_res->wred_stat_ptr);
         }
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryWredStatBlockRelease,
                                     WPI_REG_UNLOCKED,
                                     &wred_stat_block);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
      }
      if (protocol != WP_IW_HOST)
      {
         status = WPI_DeviceUnassignChannel(WPI_RES_CHANNEL_DEVICE( ch_res ), channel);
         WPI_BREAK_NOT_OK(status, WP_ChannelDeleteExit);
      }

      /* if relevant, decrement the number of Tx bound channels in the PQ block */
      if (WPI_ATM_PROTOCOL_P(protocol))
      {
         if(ch_res->du.atm.atm_pq_handle && WPI_IW_BOUND(ch_res->state))
            WPI_IwPQChCounterDecrement(ch_res->du.atm.atm_pq_handle);         
      }
      else
      {
         if(ch_res->du.pkt.pkt_pq_handle && WPI_IW_BOUND(ch_res->state))
            WPI_IwPQChCounterDecrement(ch_res->du.pkt.pkt_pq_handle);  
      }

      /* only free the channel if there are no errors */
      /* WPI_ChannelHandleFree will clear the state information */
      status = WPI_ChannelHandleFree(channel);
   }

  WP_ChannelDeleteExit:
   WPI_WDDI_DETACH();
   if (status == WP_OK)
      if (ch_res)
         ch_res->state &= ~WPI_STATE_INITIALIZED;

   return status;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelEnable
 *
 * Purpose:    Enable (activate) an ATM or packet channel
 *
 * Description:
 *     This is the user called function to enable a channels for Rx or Tx
 *     traffic.  Most of the real work is done by protocol-specific
 *     lower level routines.
 *
 * Inputs:
 *     channel      Handle of a previously created channel to be enabled
 *
 * Outputs:
 *     Updates DPS data structures to enable traffic on the specified channel
 *
 * Return Value:
 *     WP_OK if function completes successfully
 *     Error codes: WP_ERR_PROTOCOL, WP_ERR_HANDLE,
 *                  WP_ERR_INCORRECT_REGISTRY_STATE
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WP_ChannelEnable(WP_handle channel)
{
   wpi_res_registry *wpi_registry_ptr;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_channel_block *ch_block;
   wpi_res_channel *ch_res = NULL;
   WP_protocol protocol = WPI_HANDLE_SUBTYPE(channel);

   /* Check if channel handle is valid.  If not, error code is in handle */
   if (!WPI_HANDLE_RX_CHANNEL_P(channel) && !WPI_HANDLE_TX_CHANNEL_P(channel))
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

   switch (protocol)
   {
      case WP_TRANSPARENT:
         status = WPI_TransChannelEnable(channel);
         break;
      case WP_TRANSPARENT_PWE3:
         status = WPI_TransPwe3ChannelEnable(channel);
         break;
      case WP_TRANSPARENT_ITDM:
         status = WPI_TransItdmChannelEnable(channel);
         break;
      case WP_TRANSPARENT_TDM_SWITCH:
         status = WPI_TransTdmSwitchChannelEnable(channel);
         break;
      case WP_IW_HOST:
         /* Nothing to do here. */
         status = WP_OK;
         break;
      case WP_IW_LOG:
         status = WPI_IwLogChannelEnable(channel);
         break;
      default:
         /* Find the block (tx or rx) from the handle type */
         if (WPI_HANDLE_TYPE(channel) == WPI_handle_rx_channel)
         {
            ch_block = &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(channel)].rx_channels;
         }
         else
         {
            ch_block = &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(channel)].tx_channels;
         }

         WPI_ACCESS_CHANNEL(ch_res, ch_block, WPI_HANDLE_INDEX(channel));
         if (ch_res == NULL)
            status = WPI_ERROR(WP_ERR_HANDLE);
         else if ((ch_res->functions) && (ch_res->functions->channel_enable))
         {
            /*
             * WPI_PktChannelEnable
             * WPI_AtmChannelEnable
             * WPI_EfmBondingChannelEnable
             * WPI_EfmBondingOverEnetChannelEnable
             */
            status = ch_res->functions->channel_enable(channel);
         }
         else
            status = WPI_ERROR(WP_ERR_PROTOCOL);
         break;
   }

   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelDisable
 *
 * Purpose:    Disable (deactivate) an ATM or packet channel
 *
 * Description:
 *     This is the user called function to disable a channels for Rx or Tx
 *     traffic.  Most of the real work is done by protocol-specific
 *     lower level routines.
 *
 * Inputs:
 *     channel      Handle of a channel to be disabled
 *
 * Outputs:
 *     Updates DPS data structures to disable traffic on the specified channel
 *
 * Return Value:
 *     WP_OK if function completes successfully
 *     Error codes: WP_ERR_PROTOCOL, WP_ERR_INCORRECT_REGISTRY_STATE
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WP_ChannelDisable(WP_handle channel)
{
   wpi_res_registry *wpi_registry_ptr;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_channel_block *ch_block;
   wpi_res_channel *ch_res = NULL;
   WP_protocol protocol = WPI_HANDLE_SUBTYPE(channel);

   /* Check if channel handle is valid.  If not, error code is in handle */
   if (!WPI_HANDLE_RX_CHANNEL_P(channel) && !WPI_HANDLE_TX_CHANNEL_P(channel))
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
   switch (protocol)
   {
      case WP_TRANSPARENT:
         status = WPI_TransChannelDisable(channel);
         break;
      case WP_TRANSPARENT_PWE3:
         status = WPI_TransPwe3ChannelDisable(channel);
         break;
      case WP_TRANSPARENT_ITDM:
         status = WPI_TransItdmChannelDisable(channel);
         break;
      case WP_TRANSPARENT_TDM_SWITCH:
         status = WPI_TransTdmSwitchChannelDisable(channel);
         break;
      case WP_IW_HOST:
         /* Nothing to do here. */
         status = WP_OK;
         break;
      case WP_IW_LOG:
         status = WPI_IwLogChannelDisable(channel);
         break;
      default:
         /* Find the block (tx or rx) from the handle type */
         if (WPI_HANDLE_TYPE(channel) == WPI_handle_rx_channel)
         {
            ch_block = &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(channel)].rx_channels;
         }
         else
         {
            ch_block = &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(channel)].tx_channels;
         }

         WPI_ACCESS_CHANNEL(ch_res, ch_block, WPI_HANDLE_INDEX(channel));
         if (ch_res == NULL)
            status = WPI_ERROR(WP_ERR_HANDLE);
         else
            if ((ch_res->functions) && (ch_res->functions->channel_disable))
            {
               /*
                * WPI_EnetChannelDisable
                * WPI_AtmChannelDisable
                * WPI_FrChannelDisable
                * WPI_HdlcChannelDisable
                * WPI_EfmBondingChannelDisable
                *
                */
               status = ch_res->functions->channel_disable(channel);
            }
            else
               status = WPI_ERROR(WP_ERR_PROTOCOL);
         break;
   }
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelStatistics
 *
 * Purpose:    Gather and return statistics for a specified channel
 *
 * Description:
 *    This function updates statistics for a specified channel.
 *    Currently, only ATM channels support statistics at this level
 *    (statistics for packet traffic are obtained at the device-level only).
 *
 *    The statistics are provided to the user as 64-bit modulo values, despite
 *    the fact that internal DPS structures typically only maintain
 *    16-bit or 32-bit modulo statistics.
 *
 * Inputs:
 *    channel      Handle of a channel for which statistics are to be gathered
 *
 *    stats        Protocol-specific user-supplied structure into which
 *                 updated statistics are stored.
 *
 * Outputs:
 *     Updates user-supplied structure with current statistics for the
 *     specified channel.
 *
 * Return Value:
 *     WP_OK if function completes successfully
 *     Error code: WP_ERR_PROTOCOL, WP_ERR_INCORRECT_REGISTRY_STATE,
 *                 WP_ERR_HANDLE, WP_ERR_CHANNEL
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WP_ChannelStatistics(WP_handle channel,
                               void *stats)
{
   wpi_res_registry *wpi_registry_ptr;
   WP_status status = WP_OK, lock_status = WP_OK;

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

   if (WPI_HANDLE_IW_AGGREGATION(channel)||
       WPI_HANDLE_IW_SECOND_AGGREGATION(channel))
   {
      wpi_res_fiwt *res_fiwt = NULL;

      /* Checks if the channel handle is a aggreagation handle. In this case
       * the layer2 channel statistics should be gathered for this aggregation
       * flow. This is because this aggregation is mapped to the specific
       * Tx channel of layer 2. */
      if (WPI_HANDLE_IW_AGGREGATION(channel))
         res_fiwt = WPI_RES_FIWT(channel);
      else if (WPI_HANDLE_IW_SECOND_AGGREGATION(channel))
         res_fiwt = WPI_RES_FIWT_SECONDARY(channel);
      if (!WPI_INITIALIZED(res_fiwt->state) ||
          res_fiwt->params.binding == WPI_NULL_HANDLE)
      {
         WPI_WDDI_DETACH();
         return WPI_ERROR(WP_ERR_HANDLE);
      }
      if (WPI_HANDLE_TX_CHANNEL_P(res_fiwt->params.binding) &&
          WPI_HANDLE_SUBTYPE(res_fiwt->params.binding) == WP_FRAME_RELAY)
      {
         status = WPI_FrTxFlowL2Statistics(channel, stats);
         WPI_WDDI_DETACH();
         return status;
      }
      else
      {
         WPI_WDDI_DETACH();
         return WPI_ERROR(WP_ERR_HANDLE);
      }
   }
   else
   {
      /* Check if channel handle is valid. */
      if (!WPI_HANDLE_RX_CHANNEL_P(channel) &&
          !WPI_HANDLE_TX_CHANNEL_P(channel))
      {
         WPI_WDDI_DETACH();
         return WPI_ERROR(WP_ERR_CHANNEL);
      }
   }

   /* REGULAR MODE: Layer 2 channel statistics */

   /*
    * Note that there are current no channel-level statistics for HDLC and
    * Ethernet channels. All such statistics are obtained at the device
    * level or through interworking services.
    */

   switch (WPI_HANDLE_SUBTYPE(channel))
   {
      case WP_ATM_AAL0:
      case WP_ATM_AAL0_GROUP:
      case WP_ATM_OAM:
      case WP_ATM_AAL1:
      case WP_ATM_AAL2:
      case WP_ATM_AAL2U_SSSAR:
      case WP_ATM_AAL2U_SW:
      case WP_ATM_AAL5:
      case WP_ATM_ATMSW:
      case WP_ATM_RESERVED15:
      {
         status = WPI_AtmChannelStatistics(channel, stats);
         break;
      }
      case WP_FRAME_RELAY:
      {
         status = WPI_FrChannelStatistics(channel, stats);
         break;
      }
      case WP_TRANSPARENT:
      {
         status = WPI_TransChannelStatistics(channel, stats);
         break;
      }
      case WP_TRANSPARENT_PWE3:
      {
         status = WPI_TransPwe3ChannelStatistics(channel, stats);
         break;
      }
      case WP_TRANSPARENT_ITDM:
      {
         status = WPI_ERROR(WP_ERR_ITDM_NO_CHANNEL_STATISTICS);
         break;
      }
      case WP_TRANSPARENT_TDM_SWITCH:
      {
         status = WPI_TransTdmSwitchChannelStatistics(channel, stats);
         break;
      }
      default:
         status = WPI_ERROR(WP_ERR_PROTOCOL);
   }
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelStatisticsReset
 *
 * Purpose:    Clear statistics for a specified channel
 *
 * Description:
 *    This function Clears statistics for a specified channel.
 *    Currently, only ATM channels are supported.

 * Inputs:
 *    channel      Handle of a channel for which statistics are to be gathered
 *
 *    stats        Protocol-specific user-supplied structure into which
 *                 updated statistics are stored.
 *
 * Outputs:
 *     Updates user-supplied structure with cleared statistics for the
 *     specified channel.
 *
 * Return Value:
 *     WP_OK if function completes successfully
 *     Error code: WP_ERR_PROTOCOL, WP_ERR_INCORRECT_REGISTRY_STATE,
 *                 WP_ERR_HANDLE, WP_ERR_CHANNEL
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WP_ChannelStatisticsReset(WP_handle channel,
                                    void *stats)
{
   wpi_res_registry *wpi_registry_ptr;
   WP_status status = WP_OK, lock_status = WP_OK;

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
   /*Clearing statistics nly for aal5,aal0 and atmsw channels*/
   switch (WPI_HANDLE_SUBTYPE(channel))
   {
      case WP_ATM_AAL0:
      case WP_ATM_AAL0_GROUP:
      case WP_ATM_OAM:
      case WP_ATM_AAL1:
      case WP_ATM_AAL2:
      case WP_ATM_AAL2U_SSSAR:
      case WP_ATM_AAL2U_SW:
      case WP_ATM_AAL5:
      case WP_ATM_ATMSW:
      case WP_ATM_RESERVED15:
      {
         status = WPI_AtmChannelStatsReset(channel, stats);
         break;
      }
      case WP_TRANSPARENT_TDM_SWITCH:
      {
         status = WPI_TransTdmSwitchChannelStatsReset(channel, stats);
         break;
      }
      case WP_TRANSPARENT_PWE3:
      {
         status = WPI_TransPwe3ChannelStatsReset(channel, stats);
         break;
      }
      default:
         status = WPI_ERROR(WP_ERR_PROTOCOL);
   }
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelModify
 *
 * Purpose:    Modify the parameters of a channel
 *
 * Description:
 *           This function takes a command word and parses it to
 *           determine which fields need to be updated for a channel.
 *           Certain attiributes require the channel to be disabled
 *           first.
 *
 * Inputs:
 *     channel   WDDI handle for the channel
 *     command   WP_U32 command word with bits to indicate which
 *               attributes need to be modified.
 *
 * Outputs:
 *     Channel structures and DPS tables have been updated with the
 *     requested modification.
 *
 * Return Value:
 *   WP_OK
 *   Error code: WP_ERR_HANDLE, WP_ERR_CH_MODE_NOT_DISABLED,
 *               WP_ERR_CH_MOD_INVALID_COMMAND
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WP_ChannelModify(WP_handle channel,
                           WP_command command,
                           void *config)
{
   wpi_res_registry *wpi_registry_ptr;
   wpi_res_channel_block *ch_block;
   wpi_res_channel *ch_res;
   WP_U32 protocol, ch_num, wpid;
   WP_mode mode;
   WP_status status = WP_OK, lock_status = WP_OK;

   /* Check if channel handle is valid.  If not, error code is in handle */
   if (!WPI_HANDLE_RX_CHANNEL_P(channel) && !WPI_HANDLE_TX_CHANNEL_P(channel))
      return channel;

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

   wpid = WPI_HANDLE_WPID(channel);
   if (WPI_HANDLE_TYPE(channel) == WPI_handle_rx_channel)
   {
      ch_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
      mode = WP_CH_RX;
   }
   else
   {
      ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
      mode = WP_CH_TX;
   }

   ch_num = WPI_HANDLE_INDEX(channel);
   WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);
   if (ch_res == NULL)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   protocol = WPI_HANDLE_SUBTYPE(channel);


   /* for HDLC and ENET protocols, the disable check is done
      inside the function because there is host command that
      can try disable the channel */
   if(protocol != WP_HDLC &&
      protocol != WP_ENET &&
      protocol != WP_IW_HOST)
   {

      /* If a channel is active and the command is NOT on the fly, reject it */
      if ((command & ~WPI_CH_MOD_ONTHEFLY_M) &&
          !(WPI_ChannelIsDisabled(channel, ch_res)))
      {
         /* for HDLC, there is brute force host cmd that disable the channel
            and it is being called in WPI_HdlcChannelModify */
         if (command & WP_CH_MOD_FORCE)
         {
            switch (protocol) {
               case WP_ATM_AAL0:
               case WP_ATM_AAL0_GROUP:
               case WP_ATM_AAL1:
               case WP_ATM_AAL2:
               case WP_ATM_AAL2U_SSSAR:
               case WP_ATM_AAL2U_SW:
               case WP_ATM_AAL5:
               case WP_ATM_ATMSW:
                  /* list the allowable commands for ATM for "force" */
                  if (mode == WP_CH_TX) {
                     if (command &  WP_CH_MOD_ATM_VPIVCI)
                        break;
                  }
                  status = WPI_ERROR(WP_ERR_CH_MOD_CHANNEL_NOT_DISABLED);
                  goto exit_ch_modify;
               default:
               {
                  status = WPI_ERROR(WP_ERR_CH_MOD_CHANNEL_NOT_DISABLED);
                  /* All other cases, it's an error */
                  goto exit_ch_modify;
               }

            }
            command &= ~WP_CH_MOD_FORCE;
         }
         else
         {
            /* check if channel is disabled, if not call brute force
               channel disable for supported protocols or return error
               WP_ERR_CH_NOT_DISABLED */
            /*rizh start*/
            //status = WPI_AtmIssueBruteForceChannelDisable(channel, ch_res);
            //WPI_BREAK_IF_ERROR(status, exit_ch_modify);
           /*don't do anything*/
	   return WP_OK;
            /*rizh end*/
         }
      }
   }

   /* *INDENT-OFF* */
   switch (protocol) {
      case WP_TRANSPARENT:
         status = WPI_TransChannelModify (ch_res, wpid,
                                          mode, command, config);
         break;
      case WP_TRANSPARENT_PWE3:
         status = WPI_TransPwe3ChannelModify(ch_res, mode, command, config);
         break;
      case WP_TRANSPARENT_ITDM:
         status = WPI_TransItdmChannelModify(ch_res, mode, command, config);
         break;
      case WP_TRANSPARENT_TDM_SWITCH:
         status = WPI_ERROR(WP_ERR_TDM_SWITCH_NO_CH_MODIFY);
         break;
      default:
         if ((ch_res->functions) && (ch_res->functions->channel_modify))
         {
            /*
             * WPI_EnetChannelModify
             * WPI_AtmChannelModify
             * WPI_FrChannelModify
             * WPI_HdlcChannelModify
             * WPI_EfmBondingChannelModify
             * WPI_IwChannelModify
             */
            status = ch_res->functions->channel_modify(channel, ch_res, command, config);
         }
         else
            status = WPI_ERROR(WP_ERR_CH_MOD_INVALID_COMMAND);
         break;
   }
   /* *INDENT-ON* */


 exit_ch_modify:
   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelMove
 *
 * Purpose:    Move a disabled channel from one device to another
 *
 * Description:
 *             This function is used to change to association of a device
 *             with a channel. Internal registry structures are updated.
 *
 * Inputs:
 *   device      WDDI handle of the device to which channel is to be moved
 *   channel     WDDI handle of the channel
 *
 * Outputs:
 *
 * Return Value:
 *   WP_OK
 *   error code: WP_ERR_WDDI, WP_ERR_HANDLE, WP_ERR_CHANNEL
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WP_ChannelMove(WP_handle device,
                         WP_handle channel)
{
   wpi_res_registry *wpi_registry_ptr;
   wpi_res_channel_block *ch_block;     /* Channel */
   wpi_res_channel *ch_res;

   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 ch_num;
   WP_U32 wpid, protocol;

   /* Check if channel handle is valid.  If not, error code is in handle */
   if (!WPI_HANDLE_RX_CHANNEL_P(channel) && !WPI_HANDLE_TX_CHANNEL_P(channel))
      return channel;

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

   wpid = WPI_HANDLE_WPID(channel);
   ch_num = WPI_HANDLE_INDEX(channel);

   /* Find the block (tx or rx) from the handle type */
   if (WPI_HANDLE_TYPE(channel) == WPI_handle_rx_channel)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_WDDI);
   }

   ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;

   WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);

   if (ch_res == NULL)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   if (!WPI_INITIALIZED(ch_res->state))
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_CH_NOT_INITIALIZED);
   }

   protocol = WPI_HANDLE_SUBTYPE(channel);

   /* For Enet/Hdlc case, there is host command that tries to disable
      the channel by the DPS and it is called from WPI_EnetChannelMove */
   if(protocol != WP_ENET && protocol != WP_HDLC)
   {
      if (!WPI_ChannelIsDisabled(channel, ch_res))
      {
         WPI_WDDI_DETACH();
         return WPI_ERROR(WP_ERR_CH_NOT_DISABLED);
      }
   }

   switch (protocol)
   {
      case WP_ATM_AAL0:
      case WP_ATM_AAL0_GROUP:
      case WP_ATM_AAL1:
      case WP_ATM_AAL2:
      case WP_ATM_AAL2U_SSSAR:
      case WP_ATM_AAL2U_SW:
      case WP_ATM_AAL5:
      case WP_ATM_ATMSW:
         status = WPI_AtmChannelMove(device, channel, ch_res);
         break;
      case WP_ENET:
         status = WPI_EnetChannelMove(device,
                                      channel,
                                      ch_res);
         break;
      case WP_HDLC:
        status = WPI_HdlcChannelMove(device, channel, ch_res);
        break;
      default:
         status = WPI_ERROR(WP_ERR_CHANNEL);
         break;
   }

   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function:   WPI_ChannelDisplay
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
void WPI_ChannelDisplay(WP_U32 level, WP_handle handle)
{
   WP_U32 type;

   WPI_INTERNAL_FUNCTION();
   WPI_UintDisplay (level, "Channel handle", handle);

   type = WPI_HANDLE_TYPE(handle);

   if (type == WPI_handle_rx_channel) {
      WPI_RxChannelDisplay (level, handle);
   }
   else if (type == WPI_handle_tx_channel) {
      WPI_TxChannelDisplay (level, handle);
   }
   return;
}

/*****************************************************************************
 *
 * Function:   WPI_RxChannelDisplay
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
void WPI_RxChannelDisplay(WP_U32 level, WP_handle handle)
{
#if _WP_MODULE_DUMP_ENABLE_
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 wpid, channel_number, type;
   WP_mode mode;
   WP_U32 protocol;

   wpi_res_channel_block *channels;
   wpi_res_channel *channel_ptr;

   wpi_rx_bd_block *rx_bd;
   wpi_dps_bd *hu_bd_base;
   WP_U32 v_bits;

   wpi_port_group *port_group;
   WP_U32 port_group_index;
   WP_tag tag;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return;
   wpid = WPI_HANDLE_WPID(handle);
   type = WPI_HANDLE_TYPE(handle);
   protocol = WPI_HANDLE_SUBTYPE(handle);
   channel_number = WPI_HANDLE_INDEX(handle);
   WPI_RegistryChannelTagGet(wpid, channel_number, type, &tag);
   mode = WP_CH_RX;

   if (type != WPI_handle_rx_channel)
      return;
   channels = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
   channel_ptr = &(channels->channel_data[channel_number].mod);
   if (channel_ptr == NULL || !WPI_INITIALIZED (channel_ptr->state))
      return;

   port_group_index = WPI_CHANNEL_STATE_PG_V(channel_ptr->state);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &port_group,
                           wpid,
                           port_group_index);
   if (WP_ERROR_P(lock_status)) return;

   WPI_StringDisplay (level + 1, "Mode", wpi_ch_mode_string[mode]);
   WPI_IntDisplay (level + 1, "Tag", tag);
   WPI_Dump_WP_PROTOCOL (level + 1, "Protocol", protocol);

   v_bits = 0;
   if (protocol != WP_FRAME_RELAY && protocol != WP_ENET)
   {
      rx_bd = (wpi_rx_bd_block *) channel_ptr->host_io;
      if (rx_bd) {
         hu_bd_base = rx_bd->host_bd_base;
         WPI_MEM_GET (v_bits, hu_bd_base->v_bits);
      }
   }

   switch (protocol)
   {
      case WP_ATM_AAL0:
      case WP_ATM_AAL0_GROUP:
         WPI_StringDisplay (level + 1, "Crc mode",
                            wpi_aal0_ch_crcmode_string[
                               WPI_DPS_BD_VBITS_AAL0_CRC10_V(v_bits)]);
      case WP_ATM_AAL1:
      case WP_ATM_AAL2:
      case WP_ATM_AAL2U_SSSAR:
      case WP_ATM_AAL2U_SW:
      case WP_ATM_AAL5:
         WPI_StringDisplay (level + 1, "Test mode",
                            wpi_atmch_test_string[
                               WPI_DPS_BD_VBITS_TM_V(v_bits)]);
      case WP_ATM_ATMSW:
      case WP_ATM_OAM:
      {
         WPI_AtmChannelDisplay(level + 1, &channel_ptr->du.atm, protocol);

         /* RCPT */
         {
            wpi_dps_rcpt *host_rcpt_base = port_group->host_rcpt_base.atm;
            wpi_dps_rcpt *rcpt = host_rcpt_base + channel_number;
            WPI_AtmRcptDisplay (level + 1, rcpt, protocol, wpid);
         }
      }
      break;

      case WP_HDLC:
      {
        WPI_StringDisplay (level + 1, "Test mode",
                            wpi_pktch_test_string[
                               WPI_DPS_BD_VBITS_TM_V(v_bits)]);
         /* RCPT */
         {
            wpi_dps_hdlc_rcpt *rcpt = port_group->host_rcpt_base.hdlc;
            WPI_HdlcRcptDisplay (level + 1, rcpt);
         }
      }
      break;

      case WP_ENET:
      {
        WPI_StringDisplay (level + 1, "Test mode",
                            wpi_pktch_test_string[
                               WPI_DPS_BD_VBITS_TM_V(v_bits)]);
         /* RCPT */
         {
            wpi_dps_hspi_rcpt *rcpt = port_group->host_rcpt_base.hspi;
            WPI_EnetRcptDisplay (level + 1, rcpt);
         }
      }
      break;

      case WP_FRAME_RELAY:
      {
         WPI_Dump_wpi_res_channel_pkt (level + 1, level + 2,
                                       "wpi_res_channel_pkt",
                                       &channel_ptr->du.pkt);
         WPI_FrRxChannelDisplay(level, wpid, channel_ptr);
      }
      break;

      case WP_TRANSPARENT:
      {
         wpi_res_channel_trans *trans = &channel_ptr->du.trans;

         WPI_StringDisplay (level + 1, "Interrupt mode",
                            wpi_transch_int_string[
                               WPI_CH_REGISTRY_TESTMODE_V(
                                  channel_ptr->gen_config)]);
         WPI_StringDisplay (level + 1, "Test mode",
                            wpi_transch_test_string[
                               WPI_CH_REGISTRY_TESTMODE_V(
                                  channel_ptr->gen_config)]);
         WPI_IntDisplay (level + 1, "Frame number", trans->frame_number);
         WPI_IntDisplay (level + 1, "Number of slots", trans->num_slots);
         WPI_UintDisplay (level + 1, "Slots registered", trans->slots_reg);
         WPI_IntDisplay (level + 1, "Rx buffer size", trans->data_length);
         WPI_Dump_wpi_res_channel_trans (level, level + 3, "wpi_res_channel_trans", trans);
      }
      break;
      case WP_TRANSPARENT_PWE3:
         WPI_TransPwe3RxChannelDisplay(level, channel_ptr);
         break;
      case WP_TRANSPARENT_ITDM:
         WPI_TransItdmRxChannelDisplay(level, channel_ptr);
         break;
      case WP_TRANSPARENT_TDM_SWITCH:
         WPI_TransTdmSwitchRxChannelDisplay(level, channel_ptr);
         break;
      case WP_IW_HOST:
         break;

      default:
         break;
   }

#endif
}

/*****************************************************************************
 *
 * Function:   WPI_TxChannelDisplay
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
void WPI_TxChannelDisplay(WP_U32 level, WP_handle handle)
{

#if _WP_MODULE_DUMP_ENABLE_
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 wpid, channel_number, type;
   WP_mode mode;
   WP_U32 protocol;

   wpi_res_channel_block *channels;
   wpi_res_channel *channel_ptr;
   wpi_res_device *res_dev;

   wpi_tx_bd_block *tx_bd;
   wpi_dps_bd *hu_bd_base;
   WP_U32 v_bits;

   wpi_port_group *port_group;
   WP_U32 port_group_index;
   WP_tag tag;
   WP_handle dev_handle;
   wpi_res_enet_multi_class *res_enet_multi_class = NULL;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return;
   wpid = WPI_HANDLE_WPID(handle);
   type = WPI_HANDLE_TYPE(handle);
   protocol = WPI_HANDLE_SUBTYPE(handle);
   channel_number = WPI_HANDLE_INDEX(handle);
   WPI_RegistryChannelTagGet(wpid, channel_number, type, &tag);
   mode = WP_CH_TX;

   if (type != WPI_handle_tx_channel)
      return;
   channels = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
   channel_ptr = &(channels->channel_data[channel_number].mod);
   if (channel_ptr == NULL || !WPI_INITIALIZED (channel_ptr->state))
      return;

   /* get the device resources */
   dev_handle = WPI_RES_CHANNEL_DEVICE(channel_ptr );

   if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(dev_handle))
   {
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryEnetMultiClassGet,
                              WPI_REG_LOCKED,
                              &res_enet_multi_class,
                              wpid);
      if (WP_ERROR_P(lock_status)) return;
      res_dev = WPI_MULTI_CLASS_DEV_BYHANDLE(res_enet_multi_class,
                                             dev_handle);
   }
   else
   {
      WPI_REGISTRY_DEVICE_GET_BYHANDLE(lock_status,
                                       &res_dev,
                                       dev_handle);
      if (WP_ERROR_P(lock_status)) return;
   }

   port_group_index = WPI_CHANNEL_STATE_PG_V(channel_ptr->state);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &port_group,
                           wpid,
                           port_group_index);
   if (WP_ERROR_P(lock_status)) return;

   /* if this flag is set, only FMU information is displayed */
   if (!WPI_DISPLAY_CONFIG(fmu_only))
   {
      WPI_StringDisplay (level + 1, "Mode", wpi_ch_mode_string[mode]);
      WPI_IntDisplay (level + 1, "Tag", tag);
      WPI_Dump_WP_PROTOCOL (level + 1, "Protocol", protocol);

      v_bits = 0;
      tx_bd = (wpi_tx_bd_block *) channel_ptr->host_io;
      if (tx_bd && protocol != WP_FRAME_RELAY && protocol != WP_IW_HOST) {
         hu_bd_base = tx_bd->host_bd_base;
         WPI_MEM_GET (v_bits, hu_bd_base->v_bits);
      }

      switch (protocol)
      {
         case WP_ATM_AAL0:
         case WP_ATM_AAL0_GROUP:
            WPI_StringDisplay (level + 1, "Change header mode",
                               wpi_aal0_ch_insert_string[
                                  WPI_DPS_BD_VBITS_AAL0_CH_V(v_bits)]);
         case WP_ATM_AAL1:
         case WP_ATM_AAL2:
         case WP_ATM_AAL2U_SSSAR:
         case WP_ATM_AAL2U_SW:
         case WP_ATM_AAL5:
            WPI_StringDisplay (level + 1, "Test mode",
                               wpi_atmch_test_string[
                                  WPI_DPS_BD_VBITS_TM_V(v_bits)]);

         case WP_ATM_ATMSW:
         case WP_ATM_OAM:
         {
            wpi_res_device_atm *dev_atm;
            WP_U32 dev_index;
            wpi_resource_entry *dev_entry;
            wpi_resource_block *resources;

            WP_U32 shaping_type =
               WPI_ATMCH_REGISTRY_SHAPING_TYPE_V(channel_ptr->du.atm.common);
            WPI_AtmChannelDisplay(level + 1, &channel_ptr->du.atm, protocol);
            WPI_AtmChannelDisplayTx(level + 1, &channel_ptr->du.atm, protocol);

            dev_index = WPI_HANDLE_INDEX(WPI_RES_CHANNEL_DEVICE(channel_ptr));
            resources = &wpi_registry_ptr->pwp_resources[wpid];
            dev_entry = &resources->entry[dev_index];
            dev_atm = WPI_RES_DEV_ATM(dev_entry);

            /* CTD */
            if (WPI_PKT_SHAPED_ATM_DEVICE(dev_atm))
            {
               wpi_res_ctd_block *ctd_block = &wpi_registry_ptr->pwp_block[wpid].ctds;
               wpi_res_ctd *ctd_res = channel_ptr->du.atm.ctd;
               WP_U32 scheduled_rate = WPI_AtmScheduledRate(wpid, channel_ptr->tx_cwid);
               if ((WPI_MQ_ENABLED(channel_ptr->state)) &&
                   ((shaping_type == WP_ATM_SHAPING_UBR_PLUS) ||
                    (shaping_type == WP_ATM_SHAPING_HI_SPEED)))
               {
                  WP_U32 ii;
                  WPI_PTR link;
                  WPI_PTR ctd = ctd_res->ctd;
                  wpi_dps_ctde_mqubrp *host_ctde_mqubrp =
                     (wpi_dps_ctde_mqubrp *) (ctd_block->host_offset + ctd);

                  wpi_dps_ctd *host_ctd = (wpi_dps_ctd *)
                     &host_ctde_mqubrp->ctde_primary;
                  WPI_StringDisplay (level + 2, "CTD", "");
                  WPI_CtdDisplay (level + 3, host_ctd, shaping_type,
                                  scheduled_rate);

                  for (ii = 0; ii < 2; ii++)
                  {
                     host_ctd = &host_ctde_mqubrp->ctd_secondary[ii];
                     WPI_MEM_GET(link, host_ctd->link);
                     if (link != WPI_NO_CTD)
                     {
                        WPI_StringDisplay (level + 2, "CTD", "");
                        WPI_CtdDisplay (level + 3, host_ctd, shaping_type,
                                        scheduled_rate);
                     }
                  }
               }
               else
               {
                  do
                  {
                     WPI_PTR ctd = ctd_res->ctd;
                     wpi_dps_ctd *host_ctd = (wpi_dps_ctd *)
                        (ctd_block->host_offset + ctd);
                     WPI_StringDisplay (level + 2, "CTD", "");
                     WPI_CtdDisplay (level + 3, host_ctd, shaping_type,
                                     scheduled_rate);

                     ctd_res = ctd_res->next;

                  } while (ctd_res != NULL);
               }
            }

            /* TCPT */
            {
               wpi_dps_tcpt *host_tcpt_base = port_group->host_tcpt_base_atm.atm;
               wpi_dps_tcpt *tcpt = &host_tcpt_base[channel_number];
               WPI_AtmTcptDisplay (level + 1, tcpt, protocol);
            }

            /* Ext header */
            {
               wpi_atm *atm = WPI_RegistryAtm(wpid);
               wpi_dps_exthdr *host_ext_header =
                  &atm->host_exthdr_base[channel_number];
               WP_U32 extheader[3];
               WPI_MEM_ARRAY_GET (extheader, &host_ext_header->word0, 3);
               if (extheader[0] | extheader[1] | extheader[2]) {
                  WPI_UintDisplay (level + 1, "Ext header [0-3] ", extheader[0]);
                  WPI_UintDisplay (level + 1, "Ext header [4-7] ", extheader[1]);
                  WPI_UintDisplay (level + 1, "Ext header [8-11]", extheader[2]);
               }
            }

            if (protocol == WP_ATM_ATMSW
               )
               WPI_UintDisplay (level + 1, "Rx channel handle",
                                channel_ptr->du.atm.type.
                                atmsw.tx_rxchannel_handle);

         }
         break;

         case WP_HDLC:
         {
            WPI_StringDisplay (level + 1, "Test mode",
                               wpi_pktch_test_string[
                                  WPI_DPS_BD_VBITS_TM_V(v_bits)]);

            WPI_PacketChannelDisplay(level+1, channel_ptr);

            /* TCPT */
            {
               wpi_res_ftd_block *ftd_block = &wpi_registry_ptr->pwp_block[wpid].ftds;
               wpi_dps_ftd_tcpt *host_tcpt_base = port_group->host_tcpt_base_pkt;
               wpi_dps_ftd_tcpt *ftd_tcpt;
               wpi_dps_hdlc_tcpt *tcpt;

               if (channel_ptr->du.pkt.ftd != WPI_NO_FTD) {
                  ftd_tcpt = (wpi_dps_ftd_tcpt *)
                     (ftd_block->host_offset + channel_ptr->du.pkt.ftd);
               }
               else {
                  wpi_res_device *dev_res;
                  WP_U32 dev_index, phy;

                  dev_index = WPI_HANDLE_INDEX(WPI_RES_CHANNEL_DEVICE(channel_ptr));
                  dev_res = WPI_RES_DEV(WPI_RES_ENTRY(wpid, dev_index));
                  phy = dev_res->phy_number;

                  ftd_tcpt = host_tcpt_base + phy;
               }

               tcpt = &ftd_tcpt->pkt_tcpt.hdlc;
               WPI_HdlcTcptDisplay (level + 1, tcpt);
            }
         }
         break;

         case WP_ENET:
         {
            WPI_PacketChannelDisplay(level+1, channel_ptr);

            WPI_StringDisplay (level + 1, "Test mode",
                               wpi_pktch_test_string[
                                  WPI_DPS_BD_VBITS_TM_V(v_bits)]);
            /* TCPT */
            {
               wpi_res_ftd_block *ftd_block = &wpi_registry_ptr->pwp_block[wpid].ftds;
               wpi_dps_ftd_tcpt *host_tcpt_base = port_group->host_tcpt_base_pkt;
               wpi_dps_ftd_tcpt *ftd_tcpt;
               wpi_dps_enet_tcpt *tcpt;

               if (channel_ptr->du.pkt.ftd != WPI_NO_FTD) {
                  ftd_tcpt = (wpi_dps_ftd_tcpt *)
                     (ftd_block->host_offset + channel_ptr->du.pkt.ftd);
               }
               else {
                  ftd_tcpt = host_tcpt_base;
               }

               if (ftd_tcpt)
               {
                  tcpt = &ftd_tcpt->pkt_tcpt.enet;
                  WPI_EnetTcptDisplay (level + 1, tcpt);
               }
            }
         }
         break;

         case WP_FRAME_RELAY:
         {
            WPI_FrTxChannelDisplay(level, wpid, channel_ptr);
         }
         break;

         case WP_TRANSPARENT:
         {
            wpi_res_channel_trans *trans = &channel_ptr->du.trans;

            WPI_StringDisplay (level + 1, "Interrupt mode",
                               wpi_transch_int_string[
                                  WPI_CH_REGISTRY_TESTMODE_V(
                                     channel_ptr->gen_config)]);
            WPI_StringDisplay (level + 1, "Test mode",
                               wpi_transch_test_string[
                                  WPI_CH_REGISTRY_TESTMODE_V(
                                     channel_ptr->gen_config)]);
            WPI_IntDisplay (level + 1, "Frame number", trans->frame_number);
            WPI_IntDisplay (level + 1, "Number of slots", trans->num_slots);
            WPI_UintDisplay (level + 1, "Slots registered", trans->slots_reg);
            {
               wpi_dps_trans_tsat *host_tsat = trans->type.tdi.host_tsat;
               WP_U8 *udp = trans->host_udpattern;
               WP_U32 control_bits;
               WP_U32 i;

               WPI_MEM_GET(control_bits, host_tsat->control.control_bits);
               WPI_StringDisplay (level + 1, "User defined pattern",
                                  wpi_trans_tx_udp_string[
                                     WPI_TRANS_TTSAT_UDP_V(control_bits)]);
               for (i = 0; i < trans->num_slots; i++) {
                  WP_CHAR name[16];
                  sprintf (name, "Slot[%2d]", i);
                  WPI_Dump_unsigned_char (level + 2, level + 2, name, &udp[i]);
               }
               WPI_IntDisplay (level + 1, "User defined Cas",
                               WPI_TRANS_TTSAT_COND_CAS_V(control_bits));
            }
            WPI_Dump_wpi_res_channel_trans (level, level + 3, "wpi_res_channel_trans", trans);
         }
         break;
         case WP_TRANSPARENT_PWE3:
            WPI_TransPwe3TxChannelDisplay(level, channel_ptr);
            break;
         case WP_TRANSPARENT_ITDM:
            WPI_TransItdmTxChannelDisplay(level, channel_ptr);
            break;
         case WP_TRANSPARENT_TDM_SWITCH:
            WPI_TransTdmSwitchTxChannelDisplay(level, channel_ptr);
            break;
         case WP_IW_HOST:
            break;

         default:
            break;
      }

      WPI_Dump_enum_WP_cwid (level + 1, "Calendar wheel ID",
                             channel_ptr->tx_cwid);
      WPI_IntDisplay (level + 1, "Transmit queue",
                      channel_ptr->tx_tq);
   }

   /* display FMU information only */
   switch(res_dev->devtype)
   {
   case WP_DEVICE_ENET_MULTI_CLASS:
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

   if ( WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(dev_handle))
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
 * Function:   WPI_ChannelsDisplay
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
void WPI_ChannelsDisplay(WP_U32 level, WP_U32 wpid)
{
#if _WP_MODULE_DUMP_ENABLE_
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_channel_block *rx_block, *tx_block;
   wpi_channel_list *rx_list, *tx_list;
   WP_U32 i, n_initialized, n_allocated;
   WP_handle handle;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return;
   tx_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
   tx_list = tx_block->channel_list;
   n_allocated = tx_list[WPI_pg_tdi_trans].group_offset
      + tx_list[WPI_pg_tdi_trans].dps_limit;
   if (WPI_DISPLAY_CONFIG(raw_dps_structures))
      WPI_IntDisplay (level, "Number of allocated Tx channels", n_allocated);
   n_initialized = 0;
   for (i = 0; i < n_allocated; i++)
      if (WPI_INITIALIZED (tx_block->channel_data[i].mod.state))
         n_initialized++;
   WPI_NintDisplay (level,
                    "Number of initialized Tx channels", n_initialized);
   for (i = 0; i < n_allocated; i++) {
      if (!WPI_INITIALIZED (tx_block->channel_data[i].mod.state))
         continue;
      handle = tx_block->channel_data[i].mod.handle;
      if (WPI_DISPLAY_CONFIG(raw_handles))
         WPI_UintDisplay (level + 1, "Tx channel handle", handle);
      else
         WPI_ChannelDisplay (level + 1, handle);
   }

   rx_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
   rx_list = rx_block->channel_list;
   n_allocated = rx_list[WPI_pg_mfr_rx].group_offset
      + rx_list[WPI_pg_mfr_rx].dps_limit;
   if (WPI_DISPLAY_CONFIG(raw_dps_structures))
      WPI_IntDisplay (level, "Number of allocated Rx channels", n_allocated);
   n_initialized = 0;
   for (i = 0; i < n_allocated; i++)
      if (WPI_INITIALIZED (rx_block->channel_data[i].mod.state))
         n_initialized++;
   WPI_NintDisplay (level,
                    "Number of initialized Rx channels", n_initialized);
   for (i = 0; i < n_allocated; i++) {
      if (!WPI_INITIALIZED (rx_block->channel_data[i].mod.state))
         continue;
      handle = rx_block->channel_data[i].mod.handle;
      if (WPI_DISPLAY_CONFIG(raw_handles))
         WPI_UintDisplay (level + 1, "Rx channel handle", handle);
      else
         WPI_ChannelDisplay (level + 1, handle);
   }
#endif
}

/************************************************************************
 *
 * Function:  WPI_SysChannelsGet
 *
 * Purpose:
 *    Allocates a number of channels from one channel list to another
 *    channel list.
 *
 * Description:
 *    The function increments dps_next according to number of channels
 *    that should be taken from the list. If the number of channels
 *    exceeds the maximum channels in the list, the illegal channel index
 *    is return. Parameters of the second channel list are initialized.
 *
 * Inputs:
 *   from_list - channel list from which the channels are taken
 *   to_list   - channel list to which the channels are put
 *   max_channels - number of channels to transfer.
 *
 * Outputs:
 *   to_list
 *
 * Return Value:
 *   channel index, or 0xffffffff (illegal channel index) in case of error.
 *
 * Called by:
 *   WPI_Aal1ChannelsInit (wp_wddi.c)
 *
 **************************************************************************/

WP_U32 WPI_SysChannelsGet(wpi_channel_list *from_list,
                                 wpi_channel_list *to_list,
                                 WP_U32 max_channels)
{
   WP_U32 dps_next;
   WP_U32 dps_after;

   WPI_INTERNAL_FUNCTION();
   /* gets channel list of ATM shared port group */
   dps_next = from_list->dps_next;
   dps_after = dps_next + max_channels;
   if (dps_after > from_list->dps_limit)
      return (WPI_ILLEGAL_CHANNEL);

   from_list->dps_next = dps_after;

   to_list->group_offset = from_list->group_offset;
   to_list->dps_next = dps_next;
   to_list->dps_limit = dps_next + max_channels;

   return (dps_next);
}

/*****************************************************************************
 *
 * Function: WPI_SysChannelsCommit
 *
 * Purpose: Set up priority queues and other channels for each port group
 *
 * Description:
 * The structure of the Rx and Tx channel blocks is:
 *
 * First, max_tx_channels(xxx) / max_rx_channels are modified
 * For ATM, add 1 (for OAM)
 * For Tx pkt non-Enet, add 64 if max_tx_channels was not 0
 * For Tx pkt Enet, add 1 if max_tx_channels was not 0
 *
 *       Handle index       Port Group           DPS Channel Number
 *                          -----------------    ----------------------
 *                  0       WPI_pg_atm_upi1-5    0 (reserved)
 *                 ..
 *                                               max_tx_channels(atm)
 *                          ---------------      ----------------------
 *                 p1       WPI_pg_pkt_upi1      0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(upi1)
 *                          ---------------      ----------------------
 *                 p2       WPI_pg_pkt_upi2      0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(upi2)
 *                          ---------------      ----------------------
 *                 p3       WPI_pg_pkt_upi3      0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(upi3)
 *                          ---------------      ----------------------
 *                 p4       WPI_pg_pkt_upi4      0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(upi4)
 *                          ---------------      ----------------------
 *                 p5       WPI_pg_pkt_tdi      0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(tdi)
 *                          ---------------      ----------------------
 *                 e1       WPI_pg_pkt_enet1     0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(enet1)
 *                          ---------------      ----------------------
 *                 e2       WPI_pg_pkt_enet2     0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(enet2)
 *                          ---------------      ----------------------
 *                 e3       WPI_pg_pkt_enet3     0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(enet3)
 *                          ---------------      ----------------------
 *                 e4       WPI_pg_pkt_enet4     0
 *                 ..                            (0-63 reserved if present)
 *                                               max_tx_channels(enet4)
 *                          ---------------      ----------------------
 *                 iw       WPI_pg_pkt_iw_host   0
 *                                               (0-10 reserved if present)
 *                                               max_tx_channels(iw_host)
 *                          ---------------      ----------------------
 *                 t1       WPI_pg_tdm_trans     0-31
 *                          ---------------      ----------------------
 *                 xx
 *
 *
 *
 *   Specification for Tx:              *  Specification for Rx:
 *   p1 = max_tx_channels(atm)          *  p1 = max_rx_channels(atm)
 *   p2 = p1 + max_tx_channels(upi1)    *  p2 = p1 + pkt_data->max_rx_channels
 *   p3 = p2 + max_tx_channels(upi2)    *  p3 = p2 + pkt_data->max_rx_channels
 *   p4 = p3 + max_tx_channels(upi3)    *  p4 = p3 + pkt_data->max_rx_channels
 *   p5 = p4 + max_tx_channels(upi4)    *  p5 = p4 + pkt_data->max_rx_channels
 *   e1 = p5 + max_tx_channels(tdi)    *  e1 = p3 + pkt_data->max_rx_channels
 *   e2 = e1 + max_tx_channels(enet1)   *  e2 = e1 + pkt_data->max_rx_channels
 *   e3 = e2 + max_tx_channels(enet2)   *  e3 = e2 + pkt_data->max_rx_channels
 *   e4 = e3 + max_tx_channels(enet3)   *  e4 = e3 + pkt_data->max_rx_channels
 *   iw = e4 + max_tx_channels(enet4)   *  iw = 0 (unused)
 *   t1 = iw + max_tx_channels(iw_host) *  t1 = e4 + pkt_data->max_rx_channels
 *   g1 = t1 + max_tx_channels(trans)   *  g1 = t1 + max_rx_channels(trans)
 *
 *   list[port_group].group_offset is established with values:
 *
 *          Tx                          *          Rx
 *   WPI_pg_atm_shared    0             *  WPI_pg_atm_shared    0
 *   WPI_pg_pkt_upi1      p1            *  WPI_pg_pkt_upi1     p1
 *   WPI_pg_pkt_upi2      p2            *  WPI_pg_pkt_upi2     p2
 *   WPI_pg_pkt_upi3      p3            *  WPI_pg_pkt_upi3     p3
 *   WPI_pg_pkt_upi4      p4            *  WPI_pg_pkt_upi4     p4
 *   WPI_pg_pkt_tdi       p5            *  WPI_pg_pkt_tdi      p5
 *   WPI_pg_pkt_enet1     e1            *  WPI_pg_pkt_enet1    e1
 *   WPI_pg_pkt_enet2     e2            *  WPI_pg_pkt_enet2    e2
 *   WPI_pg_pkt_enet3     e3            *  WPI_pg_pkt_enet3    e3
 *   WPI_pg_pkt_enet4     e4            *  WPI_pg_pkt_enet4    e4
 *   WPI_pg_pkt_iw_host   iw            *  WPI_pg_pkt_iw_host  0 (unused)
 *   WPI_pg_tdm_trans     t1            *  WPI_pg_tdm_trans    t1
 *
 *   list[port_group].dps_next is initialized with relative values
 *   As tx PQ pools are created, this gets incremented.
 *   When PQ pools are committed the remaining channels in each group are free
 *   for individual allocation.  a/b means a if no channels were requested,
 *   b if channels were requested.
 *
 *          Tx                          *          Rx
 *   WPI_pg_atm_shared     0/1          *  WPI_pg_atm_shared     0/1
 *   WPI_pg_pkt_upi1      0/64          *  WPI_pg_pkt_upi1       0
 *   WPI_pg_pkt_upi2      0/64          *  WPI_pg_pkt_upi2       0
 *   WPI_pg_pkt_upi3      0/64          *  WPI_pg_pkt_upi3       0
 *   WPI_pg_pkt_upi4      0/64          *  WPI_pg_pkt_upi4       0
 *   WPI_pg_pkt_tdi       0/64          *  WPI_pg_pkt_tdi        0
 *   WPI_pg_pkt_enet1     0/1           *  WPI_pg_pkt_enet1      0
 *   WPI_pg_pkt_enet2     0/1           *  WPI_pg_pkt_enet2      0
 *   WPI_pg_pkt_enet3     0/1           *  WPI_pg_pkt_enet3      0
 *   WPI_pg_pkt_enet4     0/1           *  WPI_pg_pkt_enet4      0
 *   WPI_pg_pkt_iw_host   0             *  WPI_pg_pkt_iw_host    0
 *   WPI_pg_tdm_trans     0             *  WPI_pg_tdm_trans      0
 *                                      *
 *
 *   list[port_group].dps_limit is established with relative values:
 *
 *          Tx                          *          Rx
 *   WPI_pg_atm_shared    p1            *  WPI_pg_atm_shared    p1
 *   WPI_pg_pkt_upi1      p2 - p1       *  WPI_pg_pkt_upi1      p2 - p1
 *   WPI_pg_pkt_upi2      p3 - p2       *  WPI_pg_pkt_upi2      p3 - p2
 *   WPI_pg_pkt_upi3      p4 - p3       *  WPI_pg_pkt_upi3      p4 - p3
 *   WPI_pg_pkt_upi4      p5 - p4       *  WPI_pg_pkt_upi4      p5 - p4
 *   WPI_pg_pkt_tdi       e1 - p5       *  WPI_pg_pkt_tdi       e1 - p5
 *   WPI_pg_pkt_enet1     e2 - e1       *  WPI_pg_pkt_enet1     e2 - e1
 *   WPI_pg_pkt_enet2     e3 - e2       *  WPI_pg_pkt_enet2     e3 - e2
 *   WPI_pg_pkt_enet3     e4 - e3       *  WPI_pg_pkt_enet3     e4 - e3
 *   WPI_pg_pkt_enet4     iw - e4       *  WPI_pg_pkt_enet4     t1 - e4
 *   WPI_pg_pkt_iw_host   t1 - iw       *  WPI_pg_pkt_iw_host   0 (unused)
 *   WPI_pg_tdm_trans     xx - t1       *  WPI_pg_tdm_trans     xx - t1
 *
 *   xx is the maximum number of channels
 *
 * Inputs:
 *     wpid      index of Winpath
 *
 * Outputs:
 *
 * Global variables:
 *     wpi_registry
 *
 * Return Value:
 *     WP_OK
 *     WP_ERR_INIT        if registry not initialized
 *     WP_ERR_INUSE       if registry locked
 *     WP_ERR_CHANNEL_REG_ALLOC_FAILED  if memory could not be acquired
 *
 ****************************************************************************/

WP_status WPI_SysChannelsCommit(WP_U32 wpid)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   wpi_resource_block *resources;
   wpi_res_channel_block *rx_block, *tx_block;
   wpi_port_group *port_group;
   wpi_port_group *atm_port_group;
   wpi_port_group *upi1_pkt_port_group;
   wpi_port_group *upi2_pkt_port_group;
   wpi_port_group *upi3_pkt_port_group;
   wpi_port_group *upi4_pkt_port_group;
   wpi_port_group *tdi_pkt_port_group;
   wpi_port_group *enet1_pkt_port_group;
   wpi_port_group *enet2_pkt_port_group;
   wpi_port_group *enet3_pkt_port_group;
   wpi_port_group *enet4_pkt_port_group;
   wpi_port_group *efm_bonding_pkt_port_group;
   wpi_port_group *link_aggregation_pkt_port_group;
   wpi_port_group *efm_bonding_enet_pkt_port_group;
   wpi_port_group *iw_host_port_group;
   wpi_port_group *aal1_port_group;
   wpi_port_group *tdi_trans_port_group;
   wpi_port_limit *port_limit;
   wpi_res_channel_both *channel_data;
   wpi_channel_list *rx_list, *tx_list;

   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 p1, p2, p3, p4, p5;
   WP_U32 e1, e2, e3, e4, e5, e6, e7;
   WP_U32 iw, t1, t2, m, xx;
   WP_U32 max_rx_channels = 0;
   WP_U32 max_tx_channels = 0;
   WP_U32 port;
   WP_U32 pg, i, mfr_channels, emphy_trans_channels;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* Get pointer to per-winpath resources */
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   /* Get pointer to the tx_block and clear its channel_list. */
   tx_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
   tx_list = tx_block->channel_list;
   memset(tx_block->channel_list, 0, sizeof(tx_block->channel_list));

   /* Get pointer to the rx_block and clear its channel_list. */
   rx_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
   rx_list = rx_block->channel_list;
   memset(rx_block->channel_list, 0, sizeof(rx_block->channel_list));

   /**************************************************************************
    *
    * By summing over ports in a port group,
    *     Calculate max_tx_channels
    *         (Add max_tx_channels + pq_block_size * n_pq_blocks)
    *     Calculate max_rx_channels
    *     Calculate transparent max_tx_channels
    *     Calculate transparent max_rx_channels
    *     Calculate AAL1 max_rx_channels
    *     Calculate AAL1 max_tx_channels
    *
    * The port_group arrays were memset to 0 before this call.
    *
    *************************************************************************/

   for (port = WPI_first_port; port < WPI_n_ports; port++)
   {
      port_limit = WPI_RegistryPortLimit(wpid, port);
      /* ATM */
      pg = pg_atm[port];
      if (pg != WPI_pg_none)
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryPortGroupArrayGet,
                                 WPI_REG_LOCKED,
                                 &port_group,
                                 wpid,
                                 pg);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         /* if the user sets the wrong size n_mq_pq_blocks, then
            increase the n_mq_blocks by the difference to make sure
            there is enough spaces available */
         if (port_limit->atm_limits.n_mq_pq_blocks >
             port_limit->atm_limits.n_mq_blocks) {
            port_limit->atm_limits.n_mq_blocks
               += (port_limit->atm_limits.n_mq_pq_blocks -
                   port_limit->atm_limits.n_mq_blocks);
         }

         port_group->max_tx_channels +=
            port_limit->atm_limits.max_tx_channels
            + port_limit->atm_limits.pq_block_size
            * port_limit->atm_limits.n_pq_blocks
            + (WPI_MQ_BLOCK_SIZE
               * (port_limit->atm_limits.n_mq_blocks -
                  port_limit->atm_limits.n_mq_pq_blocks));

         if (port_limit->max_apsu_channels)
         {
            port_group->max_apsu_channels +=
               port_limit->max_apsu_channels
               + port_limit->atm_limits.pq_block_size
               * port_limit->atm_limits.n_pq_blocks;
         }

         port_group->max_rx_channels +=
            port_limit->atm_limits.max_rx_channels;
         port_group[WPI_pg_atm_aal1].max_tx_channels +=
            port_limit->atm_limits.aal1_tx_channels;
         port_group[WPI_pg_atm_aal1].max_rx_channels +=
            port_limit->atm_limits.aal1_rx_channels;
      }

      /* packet */
      pg = pg_pkt[port];
      if (pg != WPI_pg_none)
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryPortGroupArrayGet,
                                 WPI_REG_LOCKED,
                                 &port_group,
                                 wpid,
                                 pg);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);

         port_group->max_tx_channels +=
            port_limit->pkt_limits.max_tx_channels +
            (port_limit->pkt_limits.pq_block_size * port_limit->pkt_limits.n_pq_blocks);

         if (port_limit->max_apsu_channels)
         {
            port_group->max_tx_channels +=
               port_limit->max_apsu_channels
               + (port_limit->atm_limits.pq_block_size * port_limit->atm_limits.n_pq_blocks);
         }

         port_group->max_rx_channels += port_limit->pkt_limits.max_rx_channels;
      }

      /* transparent */
      pg = pg_trans[port];
      if (pg != WPI_pg_none)
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryPortGroupArrayGet,
                                 WPI_REG_LOCKED,
                                 &port_group,
                                 wpid,
                                 pg);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         /* Collect number of transparent channels only for
          * devices that configured as Transparent CES devices.
          * No need to count the max channels for TDM device
          * that configured as MCH device for the specific TDM
          * port. Note, that currently only CES or MCH may be
          * defined for the specific TDM port */
         if (WPI_TransCesChannelsExist(wpid, port))
         {
            max_tx_channels = 0;
            max_rx_channels = 0;
            for (i = 0; i < WP_TRANS_MULTI_FRAME_N; i++)
            {
               max_tx_channels +=
                  port_limit->trans_limits.max_tx_channels[i];
               max_rx_channels +=
                  port_limit->trans_limits.max_rx_channels[i];
            }
            port_group->max_tx_channels += max_tx_channels;
            port_group->max_rx_channels += max_rx_channels;
         }
      }
   }

   /* Iniitialize some port-group pointers */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &atm_port_group,
                           wpid,
                           WPI_pg_atm_shared);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &upi1_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_upi1);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &upi2_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_upi2);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &upi3_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_upi3);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &upi4_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_upi4);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &tdi_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_tdi);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &enet1_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_enet1);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &enet2_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_enet2);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &enet3_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_enet3);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &enet4_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_enet4);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &efm_bonding_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_efm_bonding);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &link_aggregation_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_link_aggregation);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &efm_bonding_enet_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_efm_bonding_enet);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &iw_host_port_group,
                           wpid,
                           WPI_pg_pkt_iw_host);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &aal1_port_group,
                           wpid,
                           WPI_pg_atm_aal1);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &tdi_trans_port_group,
                           wpid,
                           WPI_pg_tdi_trans);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);


   /* Adjust atm by 1 for ATM for Rx if atm was not zero. */
   if (atm_port_group->max_rx_channels != 0)
   {
      atm_port_group->max_rx_channels++;
      rx_list[WPI_pg_atm_shared].dps_next = 1;
   }

   /* add MFR tx channels to the list of the WPI_pg_pkt_tdi
    * if they exists. */
   mfr_channels = WPI_MfrTxChannels(wpid);
   if (mfr_channels > 0)
      mfr_channels++;
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &tdi_pkt_port_group,
                           wpid,
                           WPI_pg_pkt_tdi);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   tdi_pkt_port_group->max_tx_channels += mfr_channels;

   /* computes maximum PHYs allocated for port groups of
    * packet oriented channels over UPI and TDI */
   for (port = WPI_upi_base;
        port < WPI_upi_base + WPI_HwUpiMaxNumGet();
        port++)
   {
      pg = pg_pkt[port];
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryPortGroupArrayGet,
                              WPI_REG_LOCKED,
                              &port_group,
                              wpid,
                              pg);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      WPI_PacketPortGetLastPhy(wpid, port, &port_group->rx_phys,
                               &port_group->tx_phys);
   }
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &port_group,
                           wpid,
                           WPI_pg_pkt_tdi);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_PacketPortGetLastPhy(wpid, WPI_tdi_base,
                            &port_group->rx_phys,
                            &port_group->tx_phys);

   /* Increment max_tx_channels by maximum PHYs
    * if it was previously non-zero for UPI1, UPI2, UPI3, UPI4, TDI .*/
   tx_list[WPI_pg_atm_shared].dps_next = 0;
   for (pg = WPI_pg_pkt_upi1; pg <= WPI_pg_pkt_tdi; pg++)
   {
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryPortGroupArrayGet,
                              WPI_REG_LOCKED,
                              &port_group,
                              wpid,
                              pg);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      if (port_group->max_tx_channels != 0)
      {
         port_group->max_tx_channels += port_group->tx_phys;
         tx_list[pg].dps_next = port_group->tx_phys;
      }
      if (atm_port_group->max_apsu_channels != 0)
      {
         atm_port_group->max_tx_channels += port_group->tx_phys;
         tx_list[WPI_pg_atm_shared].dps_next += port_group->tx_phys;
      }
   }

   /* Increment max_tx_channels by maximum PHYs
    * if it was previously non-zero for EFM Bonding .*/
   pg = WPI_pg_pkt_efm_bonding;
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &port_group,
                           wpid,
                           pg);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (port_group->max_tx_channels != 0)
      port_group->max_tx_channels += 256;

   /* Increment max_tx_channels by maximum PHYs
    * if it was previously non-zero for EFM Bonding .*/
   pg = WPI_pg_pkt_efm_bonding_enet;
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &port_group,
                           wpid,
                           pg);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (port_group->max_tx_channels != 0)
      port_group->max_tx_channels += 256;

   if (port_group->max_rx_channels != 0)
      port_group->max_rx_channels += 256;

   /* Increment max_rx_channels by maximum PHYs for Frame Relay
    * Global Receive tables for UPI1, UPI2, UPI3, UPI4 and TDI */
   for (pg = WPI_pg_pkt_upi1; pg <= WPI_pg_pkt_tdi; pg++)
   {
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryPortGroupArrayGet,
                              WPI_REG_LOCKED,
                              &port_group,
                              wpid,
                              pg);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      if (port_group->max_rx_channels != 0)
      {
         port_group->max_rx_channels += port_group->rx_phys;
         rx_list[pg].dps_next = port_group->rx_phys;
      }
   }

   /* Increment max_tx_channels by 1 (for single-flow)
    * if it was previously non-zero for ENET1, ENET2, ENET3, ENET4. */
   for (pg = WPI_pg_pkt_enet1; pg <= WPI_pg_pkt_enet4; pg++)
   {
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryPortGroupArrayGet,
                              WPI_REG_LOCKED,
                              &port_group,
                              wpid,
                              pg);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      if (port_group->max_tx_channels != 0)
      {
         port_group->max_tx_channels++;
         tx_list[pg].dps_next = 1;
      }
   }

   /* The channel allocation mechanism requires one
    * extra channel if max_channels is non-zero.
    * Now add that one in, and sum over port groups. */

   for (pg = 0; pg < WPI_pg_n_groups; pg++)
   {
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryPortGroupArrayGet,
                              WPI_REG_LOCKED,
                              &port_group,
                              wpid,
                              pg);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      if (port_group->max_tx_channels != 0)
      {
         port_group->max_tx_channels++;
         max_tx_channels += port_group->max_tx_channels;
      }
      if (port_group->max_rx_channels != 0)
      {
         port_group->max_rx_channels++;
         max_rx_channels += port_group->max_rx_channels;
      }
   }

   emphy_trans_channels = WPI_EmphyTransChannels(wpid);
   if (emphy_trans_channels > 0)
      emphy_trans_channels++;

   /**********************************************************
    * Do allocation for tx_block.
    **********************************************************/

   /* Compute the tx channel data */
   p1 = atm_port_group->max_tx_channels;
   if (aal1_port_group->max_tx_channels != 0)
      p1 += 1;
   p2 = p1 + upi1_pkt_port_group->max_tx_channels;
   p3 = p2 + upi2_pkt_port_group->max_tx_channels;
   p4 = p3 + upi3_pkt_port_group->max_tx_channels;
   p5 = p4 + upi4_pkt_port_group->max_tx_channels;

   e1 = p5 + tdi_pkt_port_group->max_tx_channels;
   e2 = e1 + enet1_pkt_port_group->max_tx_channels;
   e3 = e2 + enet2_pkt_port_group->max_tx_channels;
   e4 = e3 + enet3_pkt_port_group->max_tx_channels;
   e5 = e4 + enet4_pkt_port_group->max_tx_channels;
   e6 = e5 + link_aggregation_pkt_port_group->max_tx_channels;

   e7 = e6 + efm_bonding_pkt_port_group->max_tx_channels;

   iw = e7 + efm_bonding_enet_pkt_port_group->max_tx_channels;

   t1 = iw + iw_host_port_group->max_tx_channels;
   t2 = t1 + tdi_trans_port_group->max_tx_channels;
   xx = t2 + emphy_trans_channels;

   tx_list[WPI_pg_atm_shared].group_offset = 0;
   tx_list[WPI_pg_pkt_upi1].group_offset = p1;
   tx_list[WPI_pg_pkt_upi2].group_offset = p2;
   tx_list[WPI_pg_pkt_upi3].group_offset = p3;
   tx_list[WPI_pg_pkt_upi4].group_offset = p4;
   tx_list[WPI_pg_pkt_tdi].group_offset = p5;
   tx_list[WPI_pg_pkt_enet1].group_offset = e1;
   tx_list[WPI_pg_pkt_enet2].group_offset = e2;
   tx_list[WPI_pg_pkt_enet3].group_offset = e3;
   tx_list[WPI_pg_pkt_enet4].group_offset = e4;
   tx_list[WPI_pg_pkt_link_aggregation].group_offset = e5;
   tx_list[WPI_pg_pkt_efm_bonding].group_offset = e6;
   tx_list[WPI_pg_pkt_efm_bonding_enet].group_offset = e7;
   tx_list[WPI_pg_pkt_iw_host].group_offset = iw;
   tx_list[WPI_pg_tdi_trans].group_offset = t1;
   tx_list[WPI_pg_emphy_trans].group_offset = t2;


   tx_list[WPI_pg_atm_shared].dps_limit = p1;
   tx_list[WPI_pg_pkt_upi1].dps_limit = p2 - p1;
   tx_list[WPI_pg_pkt_upi2].dps_limit = p3 - p2;
   tx_list[WPI_pg_pkt_upi3].dps_limit = p4 - p3;
   tx_list[WPI_pg_pkt_upi4].dps_limit = p5 - p4;
   tx_list[WPI_pg_pkt_tdi].dps_limit = e1 - p5;
   tx_list[WPI_pg_pkt_enet1].dps_limit = e2 - e1;
   tx_list[WPI_pg_pkt_enet2].dps_limit = e3 - e2;
   tx_list[WPI_pg_pkt_enet3].dps_limit = e4 - e3;
   tx_list[WPI_pg_pkt_enet4].dps_limit = e5 - e4;
   tx_list[WPI_pg_pkt_link_aggregation].dps_limit = e6 - e5;
   tx_list[WPI_pg_pkt_efm_bonding].dps_limit = e7 - e6;
   tx_list[WPI_pg_pkt_efm_bonding_enet].dps_limit = iw - e7;
   tx_list[WPI_pg_pkt_iw_host].dps_limit = t1 - iw;
   tx_list[WPI_pg_tdi_trans].dps_limit = t2 - t1;
   tx_list[WPI_pg_emphy_trans].dps_limit = xx - t2;

   /* Allocate the tx channel data */
   if (xx > 0)
   {
      tx_block->channel_data = channel_data = (wpi_res_channel_both *)
         WPI_WDDI_MALLOC(xx * sizeof(wpi_res_channel_both));
      /* Return an error if the WDDI malloc failed */
      if (channel_data == NULL)
         WPI_BREAK(status, common_return, WP_ERR_CHANNEL_REG_ALLOC_FAILED);

      /* Initialize the channel data */
      memset(channel_data, 0, xx * sizeof(wpi_res_channel_both));
   }

   /**********************************************************
    * Do allocation for rx_block.
    **********************************************************/

   /* The channels are not assigned to sub-blocks until after
    * the priority queues are allocated. */

   p1 = atm_port_group->max_rx_channels;
   if (aal1_port_group->max_rx_channels != 0)
      p1 += 1;
   p2 = p1 + upi1_pkt_port_group->max_rx_channels;
   p3 = p2 + upi2_pkt_port_group->max_rx_channels;
   p4 = p3 + upi3_pkt_port_group->max_rx_channels;
   p5 = p4 + upi4_pkt_port_group->max_rx_channels;

   e1 = p5 + tdi_pkt_port_group->max_rx_channels;
   e2 = e1 + enet1_pkt_port_group->max_rx_channels;
   e3 = e2 + enet2_pkt_port_group->max_rx_channels;
   e4 = e3 + enet3_pkt_port_group->max_rx_channels;
   e5 = e4 + enet4_pkt_port_group->max_tx_channels;

   e6 = e5 + efm_bonding_pkt_port_group->max_tx_channels;
   iw = e6 + efm_bonding_enet_pkt_port_group->max_tx_channels;

   t1 = iw + iw_host_port_group->max_rx_channels;
   m = t1 + tdi_trans_port_group->max_rx_channels;

   mfr_channels = WPI_MfrRxChannels(wpid);
   if (mfr_channels > 0)
      mfr_channels++;
   t2 = m + mfr_channels;
   xx = t2 + emphy_trans_channels;

   rx_list[WPI_pg_atm_shared].group_offset = 0;
   rx_list[WPI_pg_pkt_upi1].group_offset = p1;
   rx_list[WPI_pg_pkt_upi2].group_offset = p2;
   rx_list[WPI_pg_pkt_upi3].group_offset = p3;
   rx_list[WPI_pg_pkt_upi4].group_offset = p4;
   rx_list[WPI_pg_pkt_tdi].group_offset = p5;
   rx_list[WPI_pg_pkt_enet1].group_offset = e1;
   rx_list[WPI_pg_pkt_enet2].group_offset = e2;
   rx_list[WPI_pg_pkt_enet3].group_offset = e3;
   rx_list[WPI_pg_pkt_enet4].group_offset = e4;
   rx_list[WPI_pg_pkt_efm_bonding].group_offset = e5;
   rx_list[WPI_pg_pkt_efm_bonding_enet].group_offset = e6;
   rx_list[WPI_pg_pkt_iw_host].group_offset = iw;
   rx_list[WPI_pg_tdi_trans].group_offset = t1;
   rx_list[WPI_pg_mfr_rx].group_offset = m;
   rx_list[WPI_pg_emphy_trans].group_offset = t2;

   rx_list[WPI_pg_atm_shared].dps_limit = p1;
   rx_list[WPI_pg_pkt_upi1].dps_limit = p2 - p1;
   rx_list[WPI_pg_pkt_upi2].dps_limit = p3 - p2;
   rx_list[WPI_pg_pkt_upi3].dps_limit = p4 - p3;
   rx_list[WPI_pg_pkt_upi4].dps_limit = p5 - p4;
   rx_list[WPI_pg_pkt_tdi].dps_limit = e1 - p5;
   rx_list[WPI_pg_pkt_enet1].dps_limit = e2 - e1;
   rx_list[WPI_pg_pkt_enet2].dps_limit = e3 - e2;
   rx_list[WPI_pg_pkt_enet3].dps_limit = e4 - e3;
   rx_list[WPI_pg_pkt_enet4].dps_limit = e5 - e4;
   rx_list[WPI_pg_pkt_efm_bonding].dps_limit = e6 - e5;
   rx_list[WPI_pg_pkt_efm_bonding_enet].dps_limit = iw - e6;
   rx_list[WPI_pg_pkt_iw_host].dps_limit = t1 - iw;
   rx_list[WPI_pg_tdi_trans].dps_limit = m - t1;
   rx_list[WPI_pg_mfr_rx].dps_limit = t2 - m;
   rx_list[WPI_pg_emphy_trans].dps_limit = xx - t2;

   /* Allocate the rx channel block */
   if (xx > 0)
   {
      rx_block->channel_data = channel_data = (wpi_res_channel_both *)
         WPI_WDDI_MALLOC(xx * sizeof(wpi_res_channel_both));
      /* Return an error if the WDDI malloc failed */
      if (channel_data == NULL)
         WPI_BREAK(status, common_return, WP_ERR_CHANNEL_REG_ALLOC_FAILED);

      /* Clear the channel data */
      memset(channel_data, 0, xx * sizeof(wpi_res_channel_both));
   }

   status = WP_OK;

 common_return:
   return status;
}

/*****************************************************************************
 *
 * Function: WPI_ChannelBlocksInit
 *
 * Purpose: Initialize channels not beloning to a priority queue
 *
 * Description:
 *     This function called WPI_ChannelGroup to initialize each tx block
 *     with channels remaining after assignments to interworking pools.
 *
 * Inputs:
 *     resources       *** unused ***
 *     pwp_block       per winpath block in registry
 *
 * Outputs:
 *
 * Return Value:
 *     WP_OK
 *
 * Called by:
 *     WP_SysCommit
 *
 ****************************************************************************/

WP_status WPI_ChannelBlocksInit(wpi_resource_block *resources,
                                wpi_res_pwp_block *pwp_block)
{
   wpi_res_channel_block *tx_block;
   wpi_res_channel_block *rx_block;

   /* Initialize each tx block with channels left over from assignment to
      interworking pools. */

   WPI_INTERNAL_FUNCTION();
   tx_block = &pwp_block->tx_channels;
   WPI_ChannelGroupInit(tx_block, WPI_pg_atm_shared);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_upi1);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_upi2);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_upi3);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_upi4);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_tdi);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_enet1);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_enet2);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_enet3);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_enet4);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_link_aggregation);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_efm_bonding);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_efm_bonding_enet);
   WPI_ChannelGroupInit(tx_block, WPI_pg_pkt_iw_host);
   WPI_ChannelGroupInit(tx_block, WPI_pg_atm_aal1);
   WPI_ChannelGroupInit(tx_block, WPI_pg_tdi_trans);
   WPI_ChannelGroupInit(tx_block, WPI_pg_emphy_trans);

   /* Initialize each rx block. */

   rx_block = &pwp_block->rx_channels;
   WPI_ChannelGroupInit(rx_block, WPI_pg_atm_shared);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_upi1);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_upi2);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_upi3);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_upi4);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_tdi);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_enet1);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_enet2);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_enet3);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_enet4);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_efm_bonding);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_efm_bonding_enet);
   WPI_ChannelGroupInit(rx_block, WPI_pg_pkt_iw_host);
   WPI_ChannelGroupInit(rx_block, WPI_pg_atm_aal1);
   WPI_ChannelGroupInit(rx_block, WPI_pg_tdi_trans);
   WPI_ChannelGroupInit(rx_block, WPI_pg_mfr_rx);
   WPI_ChannelGroupInit(rx_block, WPI_pg_emphy_trans);

   /* Don't init block for iw_host; there are no rx channels for this. */

   return WP_OK;
}

wpi_dps_qat *WPI_RegistryChannelGetQat (WP_handle channel)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   wpi_port_group *port_group;
   WP_U32 pg_index, qat_index;
   wpi_dps_qat *host_qat_base;
   wpi_res_channel_block *ch_block;
   wpi_res_channel *ch_res;
   WP_U32 wpid, ch_num;
   WP_status status = WP_OK, lock_status = WP_OK;

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return NULL;

   if (!WPI_HANDLE_TX_CHANNEL_P(channel))
       return NULL;
   wpid = WPI_HANDLE_WPID(channel);
   ch_num = WPI_HANDLE_INDEX(channel);
   ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
   WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);
   status = WPI_ChannelPortGroupIndexGet(&pg_index, channel);
   if (WP_ERROR_P(status)) return NULL;
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &port_group,
                           wpid,
                           pg_index);
   if (WP_ERROR_P(lock_status)) return NULL;
   host_qat_base = (wpi_dps_qat *)port_group->host_qat_base;

   switch (WPI_HANDLE_SUBTYPE(channel))
   {
   case WP_FRAME_RELAY:
   case WP_HDLC:
      qat_index = ch_res->du.pkt.flow_code;
      break;
   case WP_TRANSPARENT_PWE3:
      return 0;
   case WP_TRANSPARENT_ITDM:
      return 0;
   case WP_TRANSPARENT_TDM_SWITCH:
      return 0;
   default:
      qat_index = ch_num - ch_block->channel_list[pg_index].group_offset;
   }

   return (&host_qat_base[qat_index]);
}

/*****************************************************************************
 *
 * Function: WPI_ChannelGetQatIndex
 *
 * Purpose: return channel QAT offset
 *
 * Description:
 *     This function return the channel QAT offset.
 *
 *
 * Inputs:
 *     WP_handle channel - channel handle
 *
 * Outputs:
 *        channel QAT offset
 * Return Value:
 *     channel QAT offset
 *
 * Called by:
 *     WPI_IwTxBindingWredCreate
 *
 ****************************************************************************/

WP_U32 WPI_ChannelGetQatIndex (WP_handle channel)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 pg_index, qat_index;
   wpi_res_channel_block *ch_block;
   wpi_res_channel *ch_res;
   WP_U32 wpid, ch_num;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
      return 0;
   if (!WPI_HANDLE_TX_CHANNEL_P(channel))
       return 0;
   wpid = WPI_HANDLE_WPID(channel);
   ch_num = WPI_HANDLE_INDEX(channel);
   ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
   WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);
   status = WPI_ChannelPortGroupIndexGet(&pg_index, channel);
   WPI_RETURN_IF_ERROR_STATUS(status);

   switch (WPI_HANDLE_SUBTYPE(channel))
   {
   case WP_FRAME_RELAY:
   case WP_HDLC:
      qat_index = ch_res->du.pkt.flow_code;
      break;
   case WP_TRANSPARENT_PWE3:
      return 0;
   case WP_TRANSPARENT_ITDM:
      return 0;
   case WP_TRANSPARENT_TDM_SWITCH:
      return 0;
   default:
      qat_index = ch_num - ch_block->channel_list[pg_index].group_offset;
   }

   return qat_index;
}

/*****************************************************************************
 *
 * Function: WPI_GetSecL2Protocol
 *
 * Purpose: return the secondary l2 protocol type when using
 *          FMU shaping.
 * Description:
 *     this function checks if the flowmode is MIXED_SHAPING and if so
 *     returns the secondary FTD mode, if not MIXED_SHAPING the function
 *     returns zero.
 *
 *     This function should be called before WPI_L2ProtConvert.
 *
 * Inputs:
 *     WP_handle channel - channel handle
 *
 * Outputs:
 *        sec_l2prot
 * Return Value:
 *     sec_l2prot
 *
 * Called by:
 *     WPI_IwNextForwardingStage, WPI_IwFiwtBinding
 *
 ****************************************************************************/
WP_U32 WPI_GetSecL2Protocol(WP_handle txfunc)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 sec_l2prot=0;
   WP_U32 handle_type=0;
   WP_U32 wpid=0;
   wpi_resource_block *resources = NULL;

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status)) return 0;
   wpid = WPI_HANDLE_WPID(txfunc);
   resources = &wpi_registry_ptr->pwp_resources[wpid];

   handle_type = WPI_HANDLE_TYPE(txfunc);
   switch (handle_type)
   {
      case WPI_handle_iw_pq:
      {
         wpi_pq_data *pq_data = NULL;
         wpi_priority_queue *priority_queue = NULL;
         enum WP_PORT_NUMBERS port_index = 0;
         wpi_resource_entry *entry = NULL;

         priority_queue = WPI_RegistryPriorityQueue(wpid);

         pq_data = priority_queue->pq_data;
         port_index = pq_data->item[WPI_HANDLE_INDEX(txfunc)].pq_port_index;
         sec_l2prot = pq_data->item[WPI_HANDLE_INDEX(txfunc)].pq_protocol;
         WPI_ACCESS_RES(entry, resources, port_index);

         if (port_index >= WP_PORT_UPI1 && port_index <= WP_PORT_UPI4)
         {
            wpi_res_upi *res_upi;

            res_upi = WPI_RES_UPI(entry);
            if ((WP_UPI_POS_HIGHSPEED == res_upi->mode) &&
                (WPI_UPI_FLOWMODE_FMU_HIERARCHICAL == res_upi->hw.tx_flowmode))
            {
               sec_l2prot = WPI_FIWT_FLOWMODE_MIXED;
            }
         }
         else if (port_index >= WP_PORT_ENET1 && port_index <= WP_PORT_ENET16)
         {
            wpi_res_enet *res_enet;
            res_enet = WPI_RES_ENET(entry);
            if(WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE == res_enet->flowmode)
            {
               /* when the txfunc is PQ handle, the protocol of the PQ is ENET and
                  the flowmode is MIXED_SHAPING, the TxProtocol in FIWT can be
                  WPI_FIWT_MIXED_ENET or WPI_FIWT_MIXED_ENET_NO_EIR, to determine
                  which one, the sec_l2pro is fetched from the PQ. this field was initialized
                  in WPI_EnetChannelCreate()*/
               sec_l2prot = WPI_FIWT_FLOWMODE_MIXED;
            }
         }
      }
      break;

      case WPI_handle_tx_channel:
      {
         WP_U32 protocol=WPI_HANDLE_SUBTYPE(txfunc);

         if (WP_ENET == protocol || WP_HDLC == protocol)
            if(WPI_RegistryChannelPacketOverShapingGroup(txfunc))
               sec_l2prot =  WPI_FIWT_FLOWMODE_MIXED;

      }
      break;

      case WPI_handle_tx_cid:
      {
         sec_l2prot = WPI_HANDLE_SUBTYPE(txfunc);
      }

      default:
         break;
   }

   return sec_l2prot;
}

WP_U32 WPI_L2ProtConvert(wpi_res_channel *res_ch,
                         WP_U32 protocol,
                         WP_U32 sec_protocol)
{
   WPI_INTERNAL_FUNCTION();
   switch (protocol)
   {
      case WP_ATM_AAL5:
         return WPI_FIWT_AAL5;
      case WP_HDLC:
      {
         wpi_res_device *dev_entry = NULL;
         WP_status lock_status = WP_OK;

         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryDeviceGet,
                                 WPI_REG_LOCKED,
                                 &dev_entry,
                                 WPI_HANDLE_WPID( WPI_RES_CHANNEL_DEVICE( res_ch ) ),
                                 WPI_HANDLE_INDEX( WPI_RES_CHANNEL_DEVICE( res_ch )));
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         if (WP_DEVICE_HSPOS == dev_entry->devtype)
         {
            switch (sec_protocol)
            {
               case  WPI_FIWT_FLOWMODE_MIXED:
                  return WPI_FIWT_MIXED_ENET;
               default:
                  return WPI_FIWT_HSPI_POS;
            }
         }
         else
            return WPI_FIWT_HDLC;
      }
      case WP_FRAME_RELAY:
         return WPI_FIWT_FRAME_RELAY;
      case WP_ENET:
         switch (sec_protocol)
         {
            case  WPI_FIWT_FLOWMODE_MIXED:
            {
#if _WP_MODULE_LINK_AGGREGATION_ENABLE_
               WP_U16 dev_index = WPI_HANDLE_INDEX(WPI_RES_CHANNEL_DEVICE( res_ch ));
               if (dev_index >= WPI_link_aggregation_group_base &&
                   dev_index <= WPI_link_aggregation_group_last)
                  return WPI_FIWT_L2PROT_LINK_AGGREGATION;
               else
#endif                 
                  return WPI_FIWT_MIXED_ENET;
            }
            default:
               if(WPI_ChannelIsFastByRes(res_ch))
               {
                  return WPI_FIWT_HSPI_GENET;
               }
               else
                  return WPI_FIWT_ENET;
         }
      case WP_TRANSPARENT:
      case WP_TRANSPARENT_PWE3:
      case WP_TRANSPARENT_ITDM:
      case WP_TRANSPARENT_TDM_SWITCH:
         return WPI_FIWT_TRANSPARENT;
      case WP_ATM_AAL0:
      case WP_ATM_AAL0_GROUP:
         return WPI_FIWT_AAL0;
      case WP_IW_HOST:
        return WPI_FIWT_IW_HOST;
      case WP_ATM_OAM:
      case WP_ATM_AAL1:
      case WP_ATM_ATMSW:
      case WP_ATM_AAL2:
         switch (sec_protocol)
         {
            case WP_AAL2_CPS_IW:
               return WPI_FIWT_AAL2_CPS;
            case WP_AAL2_SSSAR:
            case WP_AAL2_SSTED:
               return WPI_FIWT_AAL2_SSAR;
               /* unsupported secondary protocol */
            default:
               return 0xFFFF;
         }
      case WP_ATM_AAL2U_SSSAR:
          return WPI_FIWT_AAL2_SSAR;
      case WP_EFM_BONDING:
         return res_ch->fiwt_protocol;
         /* Fall through */
      default:
         return 0xFFFF;
   }
}

/*****************************************************************************
 *
 * Function:   WPI_ChannelIsDisabled
 *
 * Purpose:    Say whether a channel is disabled or not
 *
 * Description:
 *             Returns WP_boolean indicating whether a channel is disabled or not.
 *             This is determined on a per-protocol basis.
 *
 * Inputs:
 *   channel     WDDI handle of the channel
 *   ch_res      pointer to wpi_res_channel structure
 *
 * Outputs:
 *
 * Return Value:
 *   WP_TRUE     if disabled
 *   WP_FALSE    if not disabled
 *
 * Called by:
 *     WP_ChannelStatus
 *
 ****************************************************************************/
WP_boolean WPI_ChannelIsDisabled(WP_handle channel, wpi_res_channel *ch_res)
{
   WP_U32 protocol = WPI_HANDLE_SUBTYPE(channel);
   WPI_PTR ftd = WPI_NO_FTD, sec_ftd = WPI_NO_FTD;

   WPI_INTERNAL_FUNCTION();
   switch (protocol)
   {
   case WP_TRANSPARENT:
   case WP_TRANSPARENT_PWE3:
   case WP_TRANSPARENT_ITDM:
   case WP_TRANSPARENT_TDM_SWITCH:
         if ((WPI_CHANNEL_STATE_FLAGS_V(ch_res->state) &
           WPI_STATE_ACTIVE) == 0)
         return WP_TRUE;
      else return WP_FALSE;
   default:
      if ((ch_res->functions) && (ch_res->functions->channel_is_disabled))
      {
         /*
          * WPI_EfmBondingChannelIsDisabled
          * WPI_EnetChannelIsDisabled
          * WPI_AtmChannelIsDisabled
          * WPI_FrChannelIsDisabled
          * WPI_HdlcChannelIsDisabled
          */
         return ch_res->functions->channel_is_disabled(channel, ch_res, &ftd, &sec_ftd );
      }
      else
         return WP_FALSE;
      break;
   }

   return WP_FALSE;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelDeviceHandle
 *
 * Purpose:    Return handle of what was used to create a channel
 *
 * Description:
 *             Based on device index from channel state, figure out what
 *             kind of device it is and create or look up
 *             the corresponding handle.  Note that device handle may
 *             correspond to a physical device, but may be a virtual
 *             device, like an ima group, an mlppp bundle, or an MC
 *             virtual device.
 *
 *             This code requires the caller to have checked
 *             (1) channel is a tx or rx channel handle
 *             (2) channel has been initialized
 *
 *             When restructured, this code should have less reliance
 *             on knowledge of the registry structure.
 *
 * Inputs:
 *   channel     WDDI channel handle
 *   ch_res      pointer to channel resource structure
 *
 * Outputs:
 *
 * Return Value:
 *   WP_ERR_CH_NO_DEVICE             If device can't be found for channel
 *   Device handle                   If device can.
 *
 * Called by:
 *     WP_ChannelStatus
 *
 ****************************************************************************/
WP_handle WPI_ChannelDeviceHandle(wpi_res_channel *ch_res, WP_U32 request_type)
{
   wpi_ima *ima_res;
#if _WP_MODULE_GBOND_ENABLE_
   wpi_atm_gbond_functions* gbond_res;
#endif
   WP_U32 wpid = WPI_HANDLE_WPID( WPI_RES_CHANNEL_DEVICE( ch_res ));
   WP_U32 state = ch_res->state;
   WP_U32 dev_index = WPI_HANDLE_INDEX( WPI_RES_CHANNEL_DEVICE( ch_res ) );
   WP_U32 pg = WPI_CHANNEL_STATE_PG_V(state);
   WP_handle device_handle = WPI_RES_CHANNEL_DEVICE( ch_res );
   WP_status lock_status = WP_OK;

   WPI_INTERNAL_FUNCTION();
   if (WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(device_handle))
   {
      lock_status = WPI_RegistryEnetMultiClassMainDeviceHandle(device_handle,
                                                               &device_handle);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      dev_index = WPI_HANDLE_INDEX(device_handle);
   }

   if (WPI_HANDLE_SUBTYPE( device_handle ) == WPI_pg_emphy_trans)
      return (device_handle );

   /* Consider possible device classes, to find out what it is. */
   if (dev_index < WPI_upi_device_base)
      return WPI_ERROR(WP_ERR_CH_NO_DEVICE);

   else if (dev_index < WPI_ima_base) {
      /* UPI, TDI ATM, TDI HDLC, TDI TRANS, ENET
       * There is enough information in the state variable to give the result
       * Return the same thing, independent of request_type
       * The port group for an ATM channel will indicate WPI_pg_atm_shared
       * and must be adjusted based on the dev_index. */
      if (pg == WPI_pg_atm_shared) {
         if (dev_index < WPI_upi_device_base + WPI_upi_device_offset)
            pg = WPI_pg_atm_upi1;
         else if (dev_index < WPI_tdi_atm_base)
            pg = WPI_pg_atm_upi2;
         else
            pg = WPI_pg_atm_tdi;
      }
      return WPI_MAKE_HANDLE(WPI_handle_device, pg, wpid, dev_index);
   }
   else if (dev_index >= WPI_ima_base && dev_index <= WPI_ima_last) {
      /* IMA */
      /* Access the ima system to find out what port group is involved */
      ima_res = WPI_RegistryIma(wpid);

      return WPI_MAKE_HANDLE(WPI_handle_ima, ima_res->pg, wpid, dev_index);
   }
#if _WP_MODULE_GBOND_ENABLE_
   else if (dev_index >= WPI_gbond_base && dev_index <= WPI_gbond_last)
   {
      /* GBOND */
      /* Access the gbond system to find out what port group is involved */
      WP_U32 sub_type = WPI_pg_atm_upi1;
      gbond_res = WPI_RegistryGbondFunctions(wpid);
      if( gbond_res->get_port_interface(wpid) == WP_GBOND_ATM_PORT_UPI2 )
         sub_type = WPI_pg_atm_upi2;

      return WPI_MAKE_HANDLE(WPI_handle_gbond_group, sub_type, wpid, dev_index);
   }
#endif
   else if (dev_index >= WPI_mch_dev_base && dev_index <= WPI_mch_dev_last) {
      /* multi-channel:  can return either the device handle used to open the
       * channel (WP_CH_STATUS_CH_DEVICE) or the actual device handle on which the
       * multi-channel is based (WP_CH_STATUS_PH_DEVICE).
       * In either case, it is necessary to get the resource for the
       * mc device. */
      wpi_res_device *res_dev = NULL;
      WP_device devtype;
      WP_status lock_status = WP_OK;

      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryDeviceGet,
                              WPI_REG_LOCKED,
                              &res_dev,
                              wpid,
                              dev_index);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      devtype = res_dev->devtype;
      if (devtype == WP_DEVICE_MFR_FR)
      {
         return WPI_MfrDeviceHandleCreate(wpid, dev_index);
      }
      else if (devtype == WP_DEVICE_MC_FR || devtype == WP_DEVICE_MC_HDLC)
      {
         return WPI_MchDeviceHandleCreate(wpid, dev_index, request_type);
      }
      else
         return WPI_ERROR(WP_ERR_DEVICE);
   }
   else if( dev_index  >=  WPI_emy_atm_dev_base &&
            dev_index  <=  WPI_emy_atm_dev_last )
      return (device_handle);

   else if( dev_index  >= WPI_emy_pkt_dev_base &&
            dev_index  <= WPI_emy_pkt_dev_last )
      return (device_handle);

   /* else */
   return WPI_ERROR(WP_ERR_CH_NO_DEVICE);
}

/*****************************************************************************
 *
 * Function:   WP_ChannelStatus
 *
 * Purpose:    Return specified status about a channel
 *
 * Description:
 *             Returns information about a channel
 *
 * Inputs:
 *   channel      WDDI handle of the channel
 *   result_type  type of status requested
 *                WP_CH_STATUS_DISABLED -> return if channel is disabled
 *                WP_CH_STATUS_CH_DEVICE  -> return device handle used
 *                                           to create channel (this
 *                                           may be a physical device
 *                                           handle, an ima device handle,
 *                                           or a virtual device handle).
 *                WP_CH_STATUS_PH_DEVICE  -> return device handle associated
 *                                           with physical device (e.g.,
 *                                           tdi device handle instead of
 *                                           mch device handle for mch)
 *   result       pointer to returned data
 *
 * Outputs:
 *
 * Return Value:
 *   WP_OK                           If channel exists
 *   WP_ERR_CH_STATUS_BAD_CHANNEL    If channel does not exist, or if handle
 *                                   is not a channel handle.
 *   WP_ERR_CH_NO_DEVICE             Channel does not correspond to an initialized
 *                                   device
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/
WP_status WP_ChannelStatus(WP_handle channel, WP_U32 request_type, void *result)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_channel_block *ch_block;     /* Channel */
   wpi_res_channel *ch_res;

   WP_U32 wpid;
   WP_U32 ch_num;

   /* Return if handle is not actually a channel handle */
   if (!WPI_HANDLE_RX_CHANNEL_P(channel) && !WPI_HANDLE_TX_CHANNEL_P(channel))
      return WPI_ERROR(WP_ERR_CH_STATUS_BAD_CHANNEL);

   /* Get registry */
   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return lock_status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE) {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }
   /* Get channel */
   wpid = WPI_HANDLE_WPID(channel);
   ch_num = WPI_HANDLE_INDEX(channel);

   /* Find the block (tx or rx) from the handle type */
   if (WPI_HANDLE_TYPE(channel) == WPI_handle_rx_channel)
      ch_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
   else
      ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;

   WPI_ACCESS_CHANNEL(ch_res, ch_block, ch_num);

   if (ch_res == NULL || !WPI_INITIALIZED(ch_res->state)) {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_CH_STATUS_BAD_CHANNEL);
   }

   /* Switch based on request_type */
   switch(request_type) {
      case WP_CH_STATUS_DISABLED:
         *((WP_boolean *)result) = WPI_ChannelIsDisabled(channel, ch_res);
         break;
      case WP_CH_STATUS_CH_DEVICE:
         *((WP_handle *)result) = WPI_ChannelDeviceHandle(ch_res, request_type);
         break;
      case WP_CH_STATUS_PH_DEVICE:
         *((WP_handle *)result) = WPI_ChannelDeviceHandle(ch_res, request_type);
         break;
      case WP_CH_STATUS_TX_UNDERRUN:
      {
         WP_U32 protocol = WPI_HANDLE_SUBTYPE(channel);
         WP_U32 valid_protocol;

         valid_protocol = (protocol == WP_ATM_AAL0 ||
                           protocol == WP_ATM_AAL1 ||
                           protocol == WP_ATM_AAL5 ||
                           protocol == WP_ATM_ATMSW);
         if (WPI_HANDLE_TYPE(channel) == WPI_handle_tx_channel &&
             valid_protocol)
         {
            *((WP_boolean *)result) = WPI_AtmChannelTxUnderrun(channel, ch_res);
         }
         else
         {
            WPI_WDDI_DETACH();
            return WPI_ERROR(WP_ERR_CHANNEL);
         }
      }
      break;
      case WP_CH_STATUS_FMU_WRED_INFO:
      {
          WP_status status = WP_OK;

         WPI_DETACH_RETURN_IF_ERROR(status,
                                    WPI_FmuHwGetInfo(wpid, channel, (WP_fmu_wred_info *)result));
      }
      break;
   }

   /* Release registry and return */
   WPI_WDDI_DETACH();
   return WP_OK;
}

/*********************************************************************

   function name : WPI_ChannelPortGroupIndexGet

   parameters : WP_handle channel_handle

   description :

   called by :

   input : channel_handle

   output : wpi_port_group *

 ********************************************************************/
WP_status WPI_ChannelPortGroupIndexGet(WP_U32 *pg_index, WP_handle channel_handle)
{
   WP_status lock_status = WP_OK;
   WP_U32 state = 0;

   WPI_INTERNAL_FUNCTION();
   lock_status = WPI_RegistryChannelStateGet(channel_handle, &state);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   *pg_index = WPI_CHANNEL_STATE_PG_V(state);
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelQDepth
 *
 * Purpose:    Check any channel's current queue depth
 *
 * Description:
 *
 *     This function reads the current queue depth from the RCPT for
 *     ATM switching channels or from the QAT for all other channels.
 *     For AAL2 CID/CIDgroup it reads from TCQDT.
 *
 * Inputs:
 *     channel         Channel handle
 *     qdepth          Pointer to 16-bit value to be updated with queue depth
 *
 * Outputs:
 *     Updates user-supplied 16-bit variable with queue depth
 *
 *
 * Return Value:
 *     WP_OK if table is successfully created;
 *     Error codes : WP_ERR_HANDLE
 *                   WP_ERR_INCORRECT_REGISTRY_STATE
 *
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_ChannelQDepth(WP_handle channel,
                           WP_U32 *qdepth)
{
   /* Local temp variables */
   wpi_res_registry *wpi_registry_ptr = NULL;
   wpi_res_channel_block *ch_block = NULL;
   WP_U32 current_depth;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 protocol, ch_index, wpid;


   /* handle is either tx or rx channel or cidgroup, or tx cid */
   /* if rx channel then protocol has to be ATMSW */
   if ((!WPI_HANDLE_TX_CHANNEL_P(channel) &&
        !WPI_HANDLE_RX_CHANNEL_P(channel) &&
        !WPI_HANDLE_TX_AAL2CID_P(channel) &&
        !WPI_HANDLE_CIDGROUP_P(channel)) ||
       (WPI_HANDLE_RX_CHANNEL_P(channel) &&
          WPI_HANDLE_SUBTYPE(channel) != WP_ATM_ATMSW))
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

   current_depth = 0;

   if(WPI_HANDLE_TX_CHANNEL_P(channel) || WPI_HANDLE_RX_CHANNEL_P(channel))
   {
      wpid = WPI_HANDLE_WPID(channel);
      ch_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
      protocol = WPI_HANDLE_SUBTYPE(channel);
      ch_index = WPI_HANDLE_INDEX(channel);

      switch(protocol)
      {
         case WP_ATM_ATMSW:
         {
            WPI_DETACH_RETURN_IF_ERROR(status, WPI_AtmSwChannelQDepth(wpid,
                                                                      channel,
                                                                      &current_depth,
                                                                      ch_block,
                                                                      ch_index));
         }
         break;

         case WP_TRANSPARENT_PWE3:
         {
            WPI_DETACH_RETURN_IF_ERROR(status, WPI_TransPwe3TxChannelQDepth(channel,
                                                                            &current_depth));
         }
         break;

         case WP_ENET:
         {
            WPI_DETACH_RETURN_IF_ERROR(status, WPI_EnetChannelQDepth(channel,
                                                                     &current_depth));
         }
         break;

         default:
         {
            /* TX channel handle for non ATMSW.
             * Copy code from WP_PrintQat(). */

            WPI_DETACH_RETURN_IF_ERROR(status, WPI_IwQatCounterGet(wpid,
                                                                   channel,
                                                                   &current_depth));
         }
         break;
      }
   }
   else
   {
      WPI_DETACH_RETURN_IF_ERROR(status, WPI_Aal2ChannelQDepth(channel,
                                                               &current_depth));
   }

   /* Return value to user */
   *qdepth = current_depth;

   WPI_WDDI_DETACH();
   return WP_OK;
}

/*****************************************************************************
 *
 * Function:   WP_ChannelMaxQDepth
 *
 * Purpose:    Check a channel's maximum queue depth
 *
 * Description:
 *
 *     This function reads the maximum queue depth from the appropriate
 *     DPS structure.
 *
 * Inputs:
 *     channel         Channel handle or AAL2 CID/CIDGroup handle
 *     qdepth          Pointer to 32-bit value to be updated with queue depth
 *
 * Outputs:
 *     Updates user-supplied 32-bit variable with queue depth
 *
 *
 * Return Value:
 *     WP_OK if table is successfully created;
 *     Error codes : WP_ERR_HANDLE
 *                   WP_ERR_INCORRECT_REGISTRY_STATE
 *
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_ChannelMaxQDepth(WP_handle channel,
                              WP_U32 *qdepth)
{

   /* Local temp variables */
   wpi_res_registry *wpi_registry_ptr;
   WP_U32 current_depth;
   WP_status status = WP_OK, lock_status = WP_OK;

   /* Only supported for AAL2 */
   if ((!WPI_HANDLE_TX_AAL2CID_P(channel) &&
        !WPI_HANDLE_CIDGROUP_P(channel)))
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

   current_depth = 0;

   WPI_DETACH_RETURN_IF_ERROR(status,
            WPI_Aal2ChannelMaxQDepth(channel, &current_depth));

   /* Return value to user */
   *qdepth = current_depth;

   WPI_WDDI_DETACH();
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_ChannelSwQueueForce
 *
 * Purpose: force the flow to use/not use the L2 Host Queue
 *
 * Description: direct the flow to the L2 Host queue for debug use
 *              Note: This function is for testing purpose use.
 *
 * Inputs:
 *     rx_channel           Rx channel handle
 *     force                boolean flag to force the use of the queue
 *
 * Outputs:
 *
 * Return Value:
 *     list of status values including error values WP_OK, WP_ERR_HANDLE, ...
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WPI_ChannelSwQueueForce(WP_handle rx_channel, WP_boolean force)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_U32 wpid;
   WP_U32 ch_num;
   WP_handle device;
   WP_U32 dev_index;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_channel *ch_pkt;
   wpi_resource_entry *dev_entry;
   wpi_res_channel_block *ch_block;
   wpi_resource_block *resources;

   /* verify that the input handle is of Rx channel */
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (! WPI_HANDLE_RX_CHANNEL_P(rx_channel))
   {
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   wpid = WPI_HANDLE_WPID(rx_channel);
   ch_block = &wpi_registry_ptr->pwp_block[wpid].rx_channels;
   ch_num = WPI_HANDLE_INDEX(rx_channel);
   WPI_ACCESS_CHANNEL(ch_pkt, ch_block, ch_num);
   if (ch_pkt == NULL)
   {
      return WPI_ERROR(WP_ERR_HANDLE);
   }
   device = WPI_RES_CHANNEL_DEVICE( ch_pkt );

   dev_index = WPI_HANDLE_INDEX(device);
   resources = &wpi_registry_ptr->pwp_resources[wpid];
   dev_entry = &resources->entry[dev_index];

   /* Check that device was initialized. */
   if (!WPI_INITIALIZED(dev_entry->state))
   {
      return WPI_ERROR(WP_ERR_DEVICE);
   }

   switch (WPI_HANDLE_SUBTYPE(rx_channel))
   {
      case WP_ENET:
         status = WPI_EnetChannelSwQueueForce(wpid, dev_entry, force);
         break;

      default:
         status = WPI_ERROR(WP_ERR_PROTOCOL);
         break;
   }

   return status;
}

/*****************************************************************************
 *
 * Function: WPI_ChannelIsFastByHandle
 *
 * Purpose: Check if the channel type is HW mode (according to the channel handle)
 *
 * Description:
 *
 * Inputs: channel_handle
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/

WP_boolean WPI_ChannelIsFastByHandle(WP_handle channel_handle)
{
   wpi_res_channel *ch_res;

   WPI_INTERNAL_FUNCTION();

   /* check if the handle type is tx-channel or rx-channel (the input most be handle of channel) */
   if (WPI_HANDLE_TX_CHANNEL_P(channel_handle) || WPI_HANDLE_RX_CHANNEL_P(channel_handle))
   {
      /* get the resources of the channel (without lock) in order to get the channel information. */
      ch_res = WPI_RegistryChannelGetNoLock(channel_handle);

      /* return HW/SW indication (according to the channel resources) */
      return WPI_ChannelIsFastByRes(ch_res);
   }

   return WP_FALSE;
}

/*****************************************************************************
 *
 * Function: WPI_ChannelIsFastByRes
 *
 * Purpose: Check if the channel type is HW mode (according to the channel resources)
 *
 * Description:
 *
 * Inputs:
 *         ch_res      pointer to wpi_res_channel structure
 *
 * Outputs:
 *
 * Return Value:
 *
 * Called by:
 *
 ****************************************************************************/

WP_boolean WPI_ChannelIsFastByRes(wpi_res_channel *ch_res)
{
   WP_U32 protocol;

   WPI_INTERNAL_FUNCTION();

   if (!ch_res)
      return WP_FALSE;

   protocol = WPI_HANDLE_SUBTYPE(ch_res->handle);

   if ((protocol == WP_ENET || protocol == WP_HDLC) &&
       WPI_PKTCH_REGISTRY_CHANNEL_TYPE_V(ch_res->du.pkt.common) == WPI_FAST_CHANNEL)
      return WP_TRUE;
   else if ((protocol == WP_ATM_AAL5) &&
       WPI_ATMCH_REGISTRY_CHANNEL_TYPE_V(ch_res->du.atm.common) == WPI_FAST_CHANNEL)
      return WP_TRUE;
   else
      return WP_FALSE;
}
