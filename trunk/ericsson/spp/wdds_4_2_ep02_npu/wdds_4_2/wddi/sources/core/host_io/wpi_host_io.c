/*************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * File: wpi_host_io.c
 *
 * Purpose: Implements host i/o API
 *
 * Functions contained in this file:
 *
 * Table can be removed for a header file.
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |  WP_HostSend              | Host termination send function
 * |  -------------------------+----------------------------------------------
 * |  WP_HostReceive           | Host termination receive function
 * |  -------------------------+----------------------------------------------
 * |  WP_HostSendDirect        | Host termination send (direct) function
 * |  -------------------------+----------------------------------------------
 * |  WP_HostReceiveDirect     | Host termination receive (direct) function
 * |  -------------------------+----------------------------------------------
 * |  WP_HostRecover           | Recover buffers after transmit is complete
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |  Internal WDDI Functions
 * |  -------------------------+----------------------------------------------
 * |  WPI_HostReceive          | General Host Receive function
 * |  -------------------------+----------------------------------------------
 * |  WPI_IndRingCreate        | Create BD ring which points to buffers
 * |  -------------------------+----------------------------------------------
 * |  WPI_BdRingDirectCreateMp | Create BD ring with buffers for MP
 * |  -------------------------+----------------------------------------------
 * |  WPI_BdRingDirectDeleteMp | Delete BD ring with buffers for MP
 * |  -------------------------+----------------------------------------------
 * |  WPI_DirectRingCreate     | Create BD ring with internal buffers
 * |  -------------------------+----------------------------------------------
 * |  WPI_SendFrame            | Specific send for packets on channels
 * |  -------------------------+----------------------------------------------
 * |  WPI_SendBuffer           | Specific send for AAL0, AAL1
 * |  -------------------------+----------------------------------------------
 * |  WPI_SendBufferCopy       | Specific send for AAL2
 * |  -------------------------+----------------------------------------------
 * |  WPI_RecoverBuffers       | Recover buffers after transmit, channels
 * |  -------------------------+----------------------------------------------
 * |  WPI_RecoverDirectBuffers | Recover buffers after transmit, direct
 * |  -------------------------+----------------------------------------------
 * |  WPI_BdClearFrame         | Clean up an errored frame
 * |  -------------------------+----------------------------------------------
 * |  WPI_BdSwapBuffers        | Swap received buffers for empty ones
 * |  -------------------------+----------------------------------------------
 * |  WPI_ReceiveFrame         | Specific receive for packets on channels
 * |  -------------------------+----------------------------------------------
 * |  WPI_ReceiveBuffer        | Specific receive for AAL0, AAL1
 * |  -------------------------+----------------------------------------------
 * |  WPI_ReceiveBufferCopy    | Specific receive for AAL2
 * |  -------------------------+----------------------------------------------
 * |  WPI_RxBdReportAal1       | Recover extra BD flags for AAL1
 * |  -------------------------+----------------------------------------------
 * |  WPI_SendFrameDirect      | Specific send for direct
 * |  -------------------------+----------------------------------------------
 * |  WPI_MfrSendFrameDirect   | Mfr Specific send for direct
 * |  -------------------------+----------------------------------------------
 * |  WPI_ReceiveFrameDirect   | Specific receive for direct
 * |  -------------------------+----------------------------------------------
 * |  WPI_MfrReceiveFrameDirect| Mfr Specific receive for direct
 * |  -------------------------+----------------------------------------------
 * |  WPI_ReissueFtdEnable     | Issue enable after autodeactivation
 * +---------------------------+----------------------------------------------
 * |  WPI_ReissueCtdEnable     | Issue enable after autodeactivation
 * +---------------------------+----------------------------------------------
 * |  WPI_IndRingDelete        | Delete BD ring which points to buffers
 * +---------------------------+----------------------------------------------
 * |  WPI_DirectRingDelete     | Delete BD ring with internal buffers
 * +---------------------------+----------------------------------------------
 * |  WPI_HostReceiveIw        | Specific receive for interworking
 * +---------------------------+----------------------------------------------
 * |  WPI_SwQueueHostReceive   | Specific receive for L2Host
 * +---------------------------+----------------------------------------------
 * |  WPI_MatrixChainingNextBd | Calculate next bd addr in matrix caining mode
 * +---------------------------+----------------------------------------------
 * |  WPI_TakeNextBdAddress    | Take next bd address
 * +---------------------------+----------------------------------------------
 * |  WPI_IwLoggingHostReceive | Specific receive for DFC Logging
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#if defined( __CYGWIN__) && defined(__STRICT_ANSI__)
#undef __STRICT_ANSI__
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
#ifndef WPI_HOST_IO_H
#include "include/core/host_io/wpi_host_io.h"
#endif
#ifndef WPI_STATIC_H
#include "include/core/registry/wpi_static.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_CHANNEL_INT_H
#include "include/core/wpi_channel_int.h"
#endif
#ifndef WPI_DEVICE_HSPI_INT_H
#include "include/layer_2/packet/hspi/wpi_device_hspi_int.h"
#endif
#ifndef WPI_HS_ENET_INT_H
#include "include/layer_2/packet/hs_enet/wpi_hs_enet_int.h"
#endif
#ifndef WPI_HS_ENET
#include "include/layer_2/packet/hs_enet/wpi_hs_enet.h"
#endif
#ifndef WPI_HW_H
#include "include/core/hardware/wpi_hw.h"
#endif
#ifndef WPI_FIFO_INT_H
#include "include/core/hardware/wpi_fifo_int.h"
#endif
#ifndef WPI_SERVICES_H
#include "include/core/utility/wpi_services.h"
#endif
#ifndef WPI_PORT_GROUP_INT_H
#include "include/core/wpi_port_group_int.h"
#endif
#ifndef WPI_POOL_INT_H
#include "include/core/wpi_pool_int.h"
#endif
#ifndef WPI_PACKET_H
#include "include/layer_2/packet/wpi_packet.h"
#endif
#ifndef WPI_ATM_H
#include "include/layer_2/atm/wpi_atm.h"
#endif
#ifndef WPI_PORT_UPI_INT_H
#include "include/layer_1/upi/wpi_port_upi_int.h"
#endif
#ifndef WPI_CHANNEL_H
#include "include/core/wpi_channel.h"
#endif
#ifndef WPI_MEMORY_H
#include "include/core/memory/wpi_memory.h"
#endif
#ifndef WPI_DPS_ATM_H
#include "include/layer_2/atm/wpi_dps_atm.h"
#endif
#ifndef WPI_MFR_H
#include "include/layer_2/packet/wpi_mfr.h"
#endif
#ifndef WPI_PPPMUX_H
#include "include/layer_2/packet/wpi_pppmux.h"
#endif
#ifndef WPI_VENEER_H
#include "veneer/wpi_veneer.h"
#endif
#ifndef WPI_DEVICE_ENET_INT_H
#include "include/layer_2/packet/enet/wpi_device_enet_int.h"
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif
#ifndef WPI_MQ_INT_H
#include "include/core/wpi_mq_int.h"
#endif
#ifndef WPI_MQ_H
#include "include/core/wpi_mq.h"
#endif
#ifndef WPI_IW_PORT_H
#include "include/iw/core/wpi_iw_port.h"
#endif
#ifndef WPI_DPS_FEATURES_H
#include "include/core/features/wpi_dps_features.h"
#endif

#ifndef WPI_IW_INT_H
#include "include/iw/core/wpi_iw_int.h"
#endif
#ifndef WPI_BD_H
#include "include/core/wpi_bd.h"
#endif


#include "include/core/wpi_memory_class.h"
#include "include/core/wpi_port_group_class.h"
#include "include/core/wpi_port_group_private.h"
#include "include/core/hardware/wpi_fifo_class.h"
#include "include/iw/core/wpi_iw_global_class.h"
#include "include/layer_2/atm/gbond/wpi_gbond_class.h"
#include "include/layer_2/atm/wpi_device_atm_int.h"
#include "include/iw/ptp/wpi_ptp_class.h"
#include "include/layer_2/packet/link_aggregation/wpi_link_aggregation_class.h"


WP_status WPI_GetAdjPoolFromBd(WP_U32 v_bits,
                               wpi_res_node *iwq,
                               wpi_res_pool **pool_pointer,
                               WP_handle *pool_handle);

/*****************************************************************************
 * Functions used to make protocol-specific adjustments to
 * BD's for WP_HostSend() for indirect BD rings.
 ****************************************************************************/

extern WPI_TxBdAdjustFun WPI_TxBdAdjustSssar;
extern WPI_TxBdAdjustFun WPI_TxBdAdjustSsted;
extern WPI_TxBdAdjustFun WPI_TxBdAdjustAal5;
extern WPI_TxBdAdjustFun WPI_TxBdAdjustEnet;
extern WPI_TxBdAdjustFun WPI_TxBdAdjustHdlc;
extern WPI_TxBdAdjustFun WPI_TxBdAdjustFr;

static WPI_TxBdAdjustFun *WPI_TxBdAdjust[WP_DATA_TYPES] = {
   NULL,                        /* WP_DATA_AAL0 */
   NULL,                        /* WP_DATA_AAL0_OAM */
   NULL,                        /* WP_DATA_AAL1 */
   NULL,                        /* WP_DATA_AAL2CPS */
   WPI_TxBdAdjustSssar,         /* WP_DATA_AAL2SSSAR */
   WPI_TxBdAdjustSsted,         /* WP_DATA_AAL2SSTED */
   WPI_TxBdAdjustAal5,          /* WP_DATA_AAL5 */
   WPI_TxBdAdjustEnet,          /* WP_DATA_ENET */
   WPI_TxBdAdjustHdlc,          /* WP_DATA_PPP_HDLC */
   NULL,                        /* WP_DATA_IW */
   NULL,                        /* WP_DATA_TRANSPARENT */
   WPI_TxBdAdjustFr             /* WP_DATA_FRAME_RELAY */
};

/*****************************************************************************
 * Functions used to collect protocol-specific information from
 * BD's for WP_HostReceive() for indirect BD rings.
 * Note:  AAL0OAM is treated differently from all other cases.
 ****************************************************************************/
extern WPI_RxBdReportFun WPI_RxBdReportSssar;
extern WPI_RxBdReportFun WPI_RxBdReportSsted;
extern WPI_RxBdReportFun WPI_RxBdReportAal0;
extern WPI_RxBdReportFun WPI_RxBdReportAal1;
extern WPI_RxBdReportFun WPI_RxBdReportAal5;
extern WPI_RxBdReportFun WPI_RxBdReportEnet;
extern WPI_RxBdReportFun WPI_RxBdReportHdlc;
extern WPI_RxBdReportFun WPI_RxBdReportTrans;
extern WPI_RxBdReportFun WPI_RxBdReportFR;

static WPI_RxBdReportFun *WPI_RxBdReport[WP_DATA_TYPES] = {
   WPI_RxBdReportAal0,          /* WP_DATA_AAL0 */
   NULL,                        /* WP_DATA_AAL0_OAM */
   WPI_RxBdReportAal1,          /* WP_DATA_AAL1 */
   NULL,                        /* WP_DATA_AAL2CPS */
   WPI_RxBdReportSssar,         /* WP_DATA_AAL2SSSAR */
   WPI_RxBdReportSsted,         /* WP_DATA_AAL2SSTED */
   WPI_RxBdReportAal5,          /* WP_DATA_AAL5 */
   WPI_RxBdReportEnet,          /* WP_DATA_ENET */
   WPI_RxBdReportHdlc,          /* WP_DATA_PPP_HDLC */
   NULL,                        /* WP_DATA_IW */
   WPI_RxBdReportTrans,         /* WP_DATA_TRANSPARENT */
   WPI_RxBdReportFR             /* WP_DATA_FRAME_RELAY */
};

extern WP_status WPI_RxBdReportAal0OAM(wpi_bd_vx_bits *vx_bits,
                                       WP_data_segment *segment,
                                       WP_U32 wpid);

/*****************************************************************************
 * Functions used to make protocol-specific adjustments to
 * BD's for WP_HostSend() for direct BD rings.
 ****************************************************************************/

static WPI_TxBddAdjustFun *WPI_TxBddAdjust[WP_DATA_TYPES] = {
   NULL,                        /* WP_DATA_AAL0 */
   NULL,                        /* WP_DATA_AAL0_OAM */
   NULL,                        /* WP_DATA_AAL1 */
   WPI_TxBddAdjustCps,          /* WP_DATA_AAL2CPS */
   NULL,                        /* WP_DATA_AAL2SSSAR */
   NULL,                        /* WP_DATA_AAL2SSTED */
   NULL,                        /* WP_DATA_AAL5 */
   NULL,                        /* WP_DATA_ENET */
   NULL,                        /* WP_DATA_PPP_HDLC */
   NULL,                        /* WP_DATA_IW */
   NULL,                        /* WP_DATA_TRANSPARENT */
   NULL                         /* WP_DATA_FRAME_RELAY */
};

/*****************************************************************************
 * Functions used to collect protocol-specific information from
 * BD's for WP_HostReceive() for direct BD rings.
 ****************************************************************************/
WPI_RxBddReportFun WPI_RxBddReportCps;

static WPI_RxBddReportFun *WPI_RxBddReport[WP_DATA_TYPES] = {
   NULL,                        /* WP_DATA_AAL0 */
   NULL,                        /* WP_DATA_AAL0_OAM */
   NULL,                        /* WP_DATA_AAL1 */
   WPI_RxBddReportCps,          /* WP_DATA_AAL2CPS */
   NULL,                        /* WP_DATA_AAL2SSSAR */
   NULL,                        /* WP_DATA_AAL2SSTED */
   NULL,                        /* WP_DATA_AAL5 */
   NULL,                        /* WP_DATA_ENET */
   NULL,                        /* WP_DATA_PPP_HDLC */
   NULL,                        /* WP_DATA_IW */
   NULL,                        /* WP_DATA_TRANSPARENT */
   NULL                         /* WP_DATA_FRAME_RELAY */
};

/*****************************************************************************
  ============================================================================
  ============================================================================
  ==              W D D I   I N T E R F A C E   F U N C T I O N S           ==
  ==                        (D E F I N I T I O N S)                         ==
  ============================================================================
  ============================================================================
*****************************************************************************/

WP_status WPI_HspiHostSend(WP_handle channel_handle,
                           WP_data_unit *data_unit,
                           WP_U32 v_bits,
                           WP_U32 x_bits)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device_hspi *dev_hspi = NULL;
   wpi_res_device_hspi_enet *dev_hs_enet;
   WP_U32 tx_fifo_start = 0;
   WP_U32 wpid;
   wpi_res_node *iwqnode;
   wpi_memory *memory;
   wpi_dps_iwbd** tx_bd = NULL;
   WPI_PTR* bd_addr = NULL;
   WP_U32 bd_bus, buffer_bus;
   WPI_PTR dataptr, old_buffer;
   WP_status ret_val;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U8 *host_offset_buffer, *host_offset_bd;
   WP_data_segment *segment;
   WP_U32 fbp =0;
   wpi_res_pool *in_adjunct_pool, *out_adjunct_pool;
   wpi_res_node_block *node_block;
   WP_boolean int_enable = WP_FALSE;
   WP_boolean matrix_buffer_chaining_mode_enabled = WP_FALSE;
   WP_S32 ii, jj = 0;
   WP_U32 priority;
   WP_handle iwqnode_handle;
   WP_cmd_hspi_fifo hspi_info;
   WP_handle device_handle;
   wpi_res_channel_block *channels;
   wpi_res_channel *ch_pkt;
   WP_boolean is_atm_channel = WP_FALSE;
   wpi_res_pool_block *pool_block;

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if (data_unit == NULL)
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   /* Check user data input */
   if (data_unit->n_active == 0)
   {
      return WP_OK;
   }

   wpid = WPI_HANDLE_WPID(channel_handle);

   if (data_unit->n_active > data_unit->n_segments ||
       data_unit->segment == NULL )
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   /*Check if matrix buffer chaining enabled*/
   if (WPI_DpsFeatureCheckStatus(wpid,
                                 WPI_FEATUREMODE_GLOBAL,
                                 0,
                                 WPI_DPS_MODULE_MATRIX_BUFF_CHAIN_ENABLE_)== WP_TRUE)
      matrix_buffer_chaining_mode_enabled = WP_TRUE;


   /* If matrix buffer chaining mode is disabled
      data units for direct-queued devices must consist of a single buffer. */
   if( matrix_buffer_chaining_mode_enabled == WP_FALSE && data_unit->n_active != 1)
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   resources = &wpi_registry_ptr->pwp_resources[wpid];

   /* Direct-queued data units must be of interworking type */
   if ((data_unit->type == WP_DATA_ENET) || (data_unit->type == WP_DATA_PPP_HDLC))
   {
      is_atm_channel = WP_FALSE;
   }
   else if (data_unit->type == WP_DATA_AAL5)
   {
      /* NOTE: in case of WP_DATA_AAL5 it is an ATM channel over G999, which means that
       * physical device is wpi_res_device_hspi.  */
      is_atm_channel = WP_TRUE;
   }
   else
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   hspi_info.channel = channel_handle;
   memory = WPI_RegistryMemory(wpid);

   channels = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
   WPI_ACCESS_CHANNEL(ch_pkt, channels, WPI_HANDLE_INDEX(channel_handle));
   if (ch_pkt == NULL)
   {
      return WPI_ERROR(WP_ERR_WDDI);
   }

   iwqnode_handle = ch_pkt->qnode_handle;
   device_handle = WPI_RES_CHANNEL_DEVICE( ch_pkt );
   if(WPI_HANDLE_ENET_MULTI_CLASS_DEVICE_P(device_handle))
   {
      lock_status = WPI_RegistryEnetMultiClassMainDeviceHandle(device_handle,
                                                 &device_handle);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
   }
   priority = ch_pkt->tx_tq;


   /* Verify that the input handle is for a direct-queued device. */
   if (WPI_HANDLE_TYPE(device_handle) != WPI_handle_device)
   {
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   WPI_ACCESS_RES(dev_entry, resources, WPI_HANDLE_INDEX(device_handle));
   dev_hspi = WPI_RES_DEV_HSPI(dev_entry);

   if(dev_hspi->pkt.flowmode != WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL)
   {

      if (dev_hspi->pkt.device.devtype == WP_DEVICE_ENET)
      {
         dev_hs_enet = WPI_RES_DEV_HSPI_ENET(dev_entry);
         if ((dev_hs_enet->hspi.tx_intperiod > 0) && (--dev_hs_enet->hspi.tx_intperiod == 0))
         {
            int_enable = WP_TRUE;
            /* reset int_period */
            dev_hs_enet->hspi.tx_intperiod =
               WPI_HS_ENET_INTPERIOD(dev_hs_enet->hspi.tx_intmode);
         }
         tx_fifo_start = dev_hs_enet->hspi.tx_fifo_start;
      }
      else if (dev_hspi->pkt.device.devtype == WP_DEVICE_HSPOS)
      {
         if ((dev_hspi->tx_intperiod > 0) && (--dev_hspi->tx_intperiod == 0))
         {
            int_enable = WP_TRUE;
            /* reset int_period */
            dev_hspi->tx_intperiod =
               WPI_HS_ENET_INTPERIOD(dev_hspi->tx_intmode);
         }
         tx_fifo_start = dev_hspi->tx_fifo_start;
      }
      else if (dev_hspi->pkt.flowmode == WPI_DEVICE_PKT_FLOWMODE_FAST)
      {
         tx_fifo_start = dev_hspi->tx_fifo_start;
      }
      else
      {
         /* if the device is not HSPI type, it still can be HS_ENET device */
         return WPI_ERROR(WP_ERR_DEVICE_NOT_DIRECT_QUEUED);
      }
   }
   else
   {
      /* flowmode = MIXED_SHAPING
       * the fifo index is taken from the channel
       * parameters and not from the device */
      tx_fifo_start = WPI_HwChannelFifoNumGet(channel_handle);
      priority = 0;
   }

   /* Find qnode info in registry */
   if (! WPI_HANDLE_Q_NODE_P(iwqnode_handle))
   {
      return WPI_ERROR(WP_ERR_QNODE);
   }

   node_block = &wpi_registry_ptr->pwp_block[wpid].nodes;
   WPI_ACCESS_QNODE(iwqnode, node_block, WPI_HANDLE_INDEX(iwqnode_handle));
   if (iwqnode == NULL)
   {
      return WPI_ERROR(WP_ERR_WDDI);
   }

   switch (iwqnode->nodetype)
   {
      case WP_QNODE_IWQ:
         in_adjunct_pool = iwqnode->type.iwq.adjunct_pool;
         fbp = iwqnode->type.iwq.fbp;
         bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
         host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);

         break;
     case WP_QNODE_IW_MDB:
         fbp = iwqnode->type.mdb.fbp;
         bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
         host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);

         break;
      case WP_QNODE_IW_HIERARCHICAL:
      {
         WP_handle data_pool_handle = 0;
         wpi_res_iw_hierq *hierq = NULL;

         pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
         hierq = &(iwqnode->type.hierq);
         ii = hierq->num_groups - 1;

         while (ii >= 0)
         {
            for (jj = 0; jj < hierq->hier_group[ii].num_of_bd_pools; jj++)
            {
               data_pool_handle = hierq->hier_group[ii].bd_pool[jj]->data_buffer_pool_handle;
               if (data_pool_handle == data_unit->segment[0].pool_handle)
               {
                  fbp = hierq->hier_group[ii].wmm_fbp[jj];
                  ii = -1;
                  break;
               }
               else
               {
                  /* printf("data buffer pool: on group %d, pool %d is not compatible\n", ii, jj);
                     printf("segment handle is %d, incompatible pool handle is %d\n", data_unit->segment[0].pool_handle, data_pool_handle); */

               }
            }
            --ii;
         }

         in_adjunct_pool = &pool_block->pool_data[WPI_HANDLE_INDEX(data_pool_handle)];
         bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
         host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
         break;
      }

      default:
         return WPI_ERROR(WP_ERR_QNODE);
   }
   /* Supposing that for all segments same pool have been used,
      so one check is needed */
   segment = &(data_unit->segment[0]);
   /* Verify that data unit buffer and iwqnode are compatible. */
   WPI_RETURN_IF_ERROR(ret_val,
                       WPI_HwEnetQnodeCompatibleCheck(wpid, iwqnode, segment->pool_handle));

   pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
   out_adjunct_pool = (wpi_res_pool *)
      &pool_block->pool_data[WPI_HANDLE_INDEX(segment->pool_handle)];
   /* Get a BD from the FBP associated with the iwqnode_handle. */
   buffer_bus = out_adjunct_pool->bus;
   host_offset_buffer = WPI_MemoryOffsetOfVbank(out_adjunct_pool->vbank);

   tx_bd = (wpi_dps_iwbd**)WPI_WDDI_MALLOC(data_unit->n_active*sizeof(wpi_dps_iwbd*));
   if (tx_bd == NULL)
   {
      return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
   }

   bd_addr = (WPI_PTR*)WPI_WDDI_MALLOC(data_unit->n_active*sizeof(WPI_PTR));
   if (bd_addr == NULL)
   {
      WPI_WDDI_FREE(tx_bd);
      return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
   }

   for (ii = 0; ii < data_unit->n_active; ii++)
   {
      segment = &(data_unit->segment[ii]);
      if (segment->data == NULL)
      {
         ret_val = WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
      }

      status = WPI_WmmFifoGet(wpid, bd_bus, fbp, &bd_addr[ii]);

      if (bd_addr[ii] == WPI_PTR_NULL)
      {
         ret_val = WPI_ERROR(WP_ERR_WMM_FIFO_GET);
      }

      /* need to release all allocated BD*/
      if(WP_ERROR_P(ret_val) || WP_ERROR_P(status))
      {
         for(jj = 0 ; jj < ii ; jj ++)
         {
            /* If FIFO PUT failed, the BD should be returned to its original FIFO */
            status = WPI_WmmFifoPut(wpid, bd_bus, fbp, bd_addr[jj], (WP_U32*)tx_bd[jj], WP_TRUE);
            if (WP_ERROR_P(status))
            {
               WPI_WDDI_FREE(tx_bd);
               WPI_WDDI_FREE(bd_addr);
               return WPI_ERROR(WP_ERR_WDDI);
            }
         }
         WPI_WDDI_FREE(tx_bd);
         WPI_WDDI_FREE(bd_addr);
         return ret_val;
      }

      tx_bd[ii] = (wpi_dps_iwbd *)WPI_HOST_PTR(bd_addr[ii], host_offset_bd);

      if(iwqnode->nodetype == WP_QNODE_IW_MDB)
      {
         WP_handle in_pool_handle;
         WP_U32 v_bits_temp;
         /* read vbits to determine in_adjunct_pool */
         WPI_MEM_GET(v_bits_temp, tx_bd[ii]->v_bits);
         WPI_GetAdjPoolFromBd(v_bits_temp, iwqnode, &in_adjunct_pool, &in_pool_handle);
      }

      if(data_unit->control & WP_HT_INTERRUPT_M)
      {
         int_enable = WP_TRUE;
      }

#if _WP_WMM_FIFO_BD_NEW_STRUCT_
      v_bits |= ((WP_U32) WPI_DPS_IWBD_VBITS_V_M |
                 WPI_DPS_IWBD_VBITS_I_F(int_enable) |
                 WPI_DPS_IWBD_VBITS_E_M |
                 WPI_DPS_IWBD_VBITS_LEN_F(data_unit->data_size));
      dataptr = WPI_WPI_PTR(segment->data, host_offset_buffer);
      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, tx_bd[ii]->data);
      WPI_NpoolFree(in_adjunct_pool, old_buffer);

      /* For winsim, copy the data to the socket */
      WPI_MEM_HOST_TO_WINSIM(segment->data, segment->data_size);

      /*In matrix buffer chaining mode we need different treatment:
        In first bd -  MTBD bit should be ON
        In second bd - update x_bits in first bd ( also segment size)
        In third .. bd - bd address should be set to "Next" field of previous BD
        In last bd END bit should be ON, in non-last bd-s END bit should be OFF
        In non-first bd - length should be set to segment data_size
      */
      if((matrix_buffer_chaining_mode_enabled == WP_TRUE) /*&& (data_unit->n_active > 1)*/)
      {
         WP_U32 x_bits_prev;

         /*First bd treatment*/
         if(ii == 0 && (data_unit->n_active > 1))
         {
            v_bits = v_bits | WPI_DPS_MATRIX_BD_VBITS_MTBD_M;
         }
         else /*Data_size treatment for non-first bd*/
         {
            v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_LEN_M);
            v_bits = v_bits & (~ WPI_DPS_MATRIX_BD_VBITS_MTBD_M);
            v_bits = v_bits |  WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size);
         }

         /*Second bd treatment - update first bd x_bits*/
         if(ii == 1)
         {
            if(bd_addr[1] > bd_addr[0])
            {
               x_bits_prev = x_bits &(~WPI_DPS_MATRIX_BD_SIGN_M);
               x_bits_prev |= WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[1]-bd_addr[0])>>4);
            }
            else
            {
               x_bits_prev = x_bits | WPI_DPS_MATRIX_BD_SIGN_M;
               x_bits_prev |= WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[0]-bd_addr[1])>>4);
            }
            x_bits_prev |= WPI_DPS_MATRIX_BD_BDLENGTH_F(data_unit->segment[0].data_size);

            WPI_MEM_SET(tx_bd[0]->x_bits, x_bits_prev);
         }

        /*Update "Next" field of previous BD*/
        if(ii > 1)
        {
           WPI_MEM_SET(tx_bd[ii-1]->next_bd, bd_addr[ii]);
        }

        /*Non last bd treatment*/
        if(ii < data_unit->n_active - 1)
        {
           /*Default setting: End bit is ON, here turning it OFF*/
           v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_E_M);
        }
     }

      WPI_BdIwInit(wpid, tx_bd[ii], 0, dataptr, &v_bits, &x_bits, bd_bus, buffer_bus, fbp);

#else /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ */


      /* Formulate new BD data */
      v_bits |= ((WP_U32) WPI_DPS_IWBD_VBITS_V_M |
                 WPI_DPS_IWBD_VBITS_I_F(int_enable) |
                 WPI_DPS_IWBD_VBITS_E_M |
                 WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
                 WPI_DPS_IWBD_VBITS_FBP_F(fbp) |
                 WPI_DPS_IWBD_VBITS_LEN_F(data_unit->data_size));

      dataptr = WPI_WPI_PTR(segment->data, host_offset_buffer);

      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, tx_bd[ii]->data);
      WPI_NpoolFree(in_adjunct_pool, old_buffer);

      /* For winsim, copy the data to the socket */
      WPI_MEM_HOST_TO_WINSIM(segment->data, segment->data_size);

      /* Update the BD - common case*/
      WPI_MEM_SET(tx_bd[ii]->next_bd, 0);
      WPI_MEM_SET(tx_bd[ii]->data, dataptr);
      WPI_MEM_SET(tx_bd[ii]->x_bits, x_bits);

      /*In matrix buffer chaining mode we need different treatment:
        In first bd -  MTBD bit should be ON
        In second bd - update x_bits in first bd ( also segment size)
        In third .. bd - bd address should be set to "Next" field of previous BD
        In last bd END bit should be ON, in non-last bd-s END bit should be OFF
        In non-first bd - length should be set to segment data_size
      */
      if((matrix_buffer_chaining_mode_enabled == WP_TRUE) /*&& (data_unit->n_active > 1)*/)
     {
        /*First bd treatment*/
        if(ii == 0 && (data_unit->n_active > 1))
        {
            v_bits = v_bits | WPI_DPS_MATRIX_BD_VBITS_MTBD_M;
        }
        else /*Data_size treatment for non-first bd*/
         {
            v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_LEN_M);
            v_bits = v_bits & (~ WPI_DPS_MATRIX_BD_VBITS_MTBD_M);
            v_bits = v_bits |  WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size);
         }

        /*Second bd treatment - update first bd x_bits*/
        if(ii == 1)
        {
            if(bd_addr[1] > bd_addr[0])
            {
               x_bits= x_bits &(~WPI_DPS_MATRIX_BD_SIGN_M);
               x_bits= x_bits | WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[1]-bd_addr[0])>>4);
            }
            else
            {
               x_bits= x_bits | WPI_DPS_MATRIX_BD_SIGN_M;
               x_bits= x_bits | WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[0]-bd_addr[1])>>4);
            }
            x_bits= x_bits |
               WPI_DPS_MATRIX_BD_BDLENGTH_F(data_unit->segment[0].data_size);

            WPI_MEM_SET(tx_bd[0]->x_bits, x_bits);
         }

        /*Update "Next" field of previous BD*/
        if(ii > 1)
        {
           WPI_MEM_SET(tx_bd[ii-1]->next_bd, bd_addr[ii]);
        }

        /*Non last bd treatment*/
        if(ii < data_unit->n_active - 1)
        {
           /*Default setting: End bit is ON, here turning it OFF*/
           v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_E_M);
        }
     }
      WPI_MEM_SET(tx_bd[ii]->v_bits, v_bits);
#endif /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ - else */
   }


/* Insert the BD in the appropriate TQ FIFO. */
   /* In matrix buffer chaining mode only first bd should be inserted to fifo*/

   /* */

   if(WPI_ChannelIsFastByRes(ch_pkt))
   {

      ret_val = WPI_HwCmdFifoPut(wpid,
                                 bd_bus,
                                 tx_fifo_start + priority,
                                 bd_addr[0],
                                 NULL,
                                 WP_FALSE,
                                 hspi_info.channel,
                                 data_unit->data_size,
                                 fbp);
   }
   else
   {
      ret_val = WPI_HwCmdSwPut( bd_addr[0],
                                channel_handle,
                                fbp);
   }

   if (WP_ERR_PREVIOUS_HOST_CMD_DID_NOT_FINISH == WP_ERROR(ret_val))
   {
      /* If FIFO PUT failed, the BD should be returned to its original FIFO */
      status = WPI_WmmFifoPut(wpid, bd_bus, fbp, bd_addr[0], (WP_U32*)tx_bd[0], WP_TRUE);
      if (WP_ERROR_P(status))
      {
         ret_val = WPI_ERROR(WP_ERR_WDDI);

      }

      /* in case of matrix, we need to release all BDs */
      if(matrix_buffer_chaining_mode_enabled == WP_TRUE)
      {
         /* ii holds the number of BDs that were allocated
               in case there was just 1 BD, there will be no
               iteration within the for loop*/
         for(jj = 1 ; jj < ii ; jj ++)
         {
            /* If FIFO PUT failed, the BD should be returned to its original FIFO */
            status = WPI_WmmFifoPut(wpid, bd_bus, fbp, bd_addr[jj], (WP_U32*)tx_bd[jj], WP_TRUE);
            if (WP_ERROR_P(status))
            {
               return WPI_ERROR(WP_ERR_WDDI);
            }
         }
      }
   }

   WPI_WDDI_FREE(tx_bd);
   WPI_WDDI_FREE(bd_addr);

   return ret_val;
}

static WP_status WPI_HspiHostReceive(WP_handle handle,
                                     WP_data_unit *data_unit,
                                     WP_handle iwqnode_handle,
                                     wpi_res_channel *res_channel)
{
   WP_U32 subtype = WPI_HANDLE_SUBTYPE(handle);
   WP_status status = WP_OK;

   switch (subtype)
   {
      case WP_ENET:
         WPI_RETURN_IF_ERROR(status,
                             WPI_EnetInfoByChannelGet(handle,
                                                      data_unit,
                                                      iwqnode_handle,
                                                      res_channel));
         break;
      case WP_HDLC: /* hspos channels */
         WPI_RETURN_IF_ERROR(status,
                             WPI_HsposInfoByChannelGet(handle,
                                                       data_unit,
                                                       iwqnode_handle,
                                                       res_channel));
         break;
      default:
         return WPI_ERROR(WP_ERR_WDDI);
   }
   return status;
}

/*****************************************************************************
 *
 * Function: WP_HostSend
 *
 * Purpose: Send host data unit on a channel.
 *
 * Description:
 *     Select specific processing based on protocol of channel.
 *     Utilizes channel block.
 *
 * Inputs:
 *     handle          Handle associated with the channel
 *     data_unit       Pointer to data unit describing data to be sent.
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *
 *
 *   WP_OK                         The function has executed successfully.
 *   WP_ERR_HST_INVALID_CHANNEL    The specified channel does not exist.
 *   WP_ERR_HST_COULD_NOT_ENQUEUE_DU The tx origination queue is full or
 *                                 the total size of a data unit is too large
 *                                 to be accomodated at the tx origination
 *                                 queue (number of available BDs in the
 *                                 tx ring is less then the number of buffers
 *                                 used to describe the data unit).
 *   WP_ERR_HST_INVALID_INPUT      One or more function argumets appeared to
 *                                 by incorrect or missing.
 *   WP_ERR_NOT_IMPLEMENTED        The functionality implied by the input
 *                                 arguments has not yet been implemented.
 *   WP_ERR_WDDI                   Internal WDDI logic error.
 *   WP_ERR_INCORRECT_REGISTRY_STATE
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/

WP_status WP_HostSend(WP_handle handle,
                      WP_data_unit *data_unit)
{
   wpi_res_channel_block *channel_block;
   wpi_res_channel *cptr = NULL;
   wpi_res_cid_block *cid_block;
   wpi_res_cid *cidptr = NULL;
   wpi_res_cid_group *cidgroup = NULL;
   wpi_tx_bd_block *tx_bd = NULL;
   wpi_tx_bdd_block *tx_bdd = NULL;
   WP_U32 i;
   WP_S32 ii, jj = 0;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_status ret_val = WP_OK;
   WP_data_segment *segment;
   WP_U32 v_bits = 0 ,x_bits = 0;
   WPI_PTR ctd_ptr = WPI_NO_FTD, sec_ctd_ptr = WPI_NO_FTD;
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_boolean aal0_psu_crc_mode = WP_FALSE;
   wpi_res_device_atm *dev_atm = NULL;
   WP_U32 wpid = WPI_HANDLE_WPID(handle);

   if (data_unit == NULL)
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   /* Check data_unit segments */
   segment = data_unit->segment;
   for (i = 0; i < data_unit->n_active; i++)
   {
      if (segment == NULL)
      {
         return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
      }

      if (segment->data == NULL)
      {
         return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
      }

      segment = segment->next;
   }

   /* Check user data input */
   if (data_unit->n_active == 0)
   {
      return WP_OK;
   }

   if (data_unit->n_active > data_unit->n_segments ||
       data_unit->segment == NULL)
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

  /* Determine the type of handle:
      - could be a regular channel handle
      - a tx cid handle
   */
   switch (WPI_HANDLE_TYPE(handle))
   {
      case WPI_handle_tx_channel:
      {
         /* Get the channel data based on the protocol. */
         channel_block =
            &wpi_registry_ptr->pwp_block[wpid].tx_channels;
         cptr =
            (wpi_res_channel *) &channel_block->
            channel_data[WPI_HANDLE_INDEX(handle)].mod;
         if (cptr == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);
         if (!WPI_INITIALIZED(cptr->state))
            WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);

         break;
      }
      case WPI_handle_tx_cid:
      {
         cid_block = &wpi_registry_ptr->pwp_block[wpid].tx_cids;
         WPI_ACCESS_AAL2CID(cidptr, cid_block, WPI_HANDLE_INDEX(handle));
         if (cidptr == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);
         if (!WPI_INITIALIZED(cidptr->state))
            WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);
         cidgroup = cidptr->cidgroup;
         break;
      }

      case WPI_handle_rx_channel:
      {
         if (WPI_HANDLE_SUBTYPE(handle) != WP_IW_HOST)
            WPI_BREAK(ret_val, quit, WP_ERR_HANDLE);

         /* Check the data unit protocol type */
         if (data_unit->type != WP_DATA_IW)
         {
            WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_INPUT);
         }

         /* Get the channel data based on the protocol. */
         channel_block =
            &wpi_registry_ptr->pwp_block[wpid].rx_channels;
         cptr = (wpi_res_channel *)
            &channel_block->channel_data[WPI_HANDLE_INDEX(handle)].mod;
         if (cptr == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);
         if (!WPI_INITIALIZED(cptr->state))
            WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);
         break;
      }

      case WPI_handle_mfr:
      {
         ret_val = WPI_MfrLinkSend(handle, data_unit);
         WPI_WDDI_DETACH();
         return ret_val;
      }

      case WPI_handle_gbond_link:
      {
         wpi_atm_gbond_functions *gbond_res = NULL;
         gbond_res = WPI_RegistryGbondFunctions(wpid);
         if (gbond_res)
         {
            ret_val = gbond_res->link_send(handle, data_unit);
         }
         else
         {
            ret_val = WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
         }

         WPI_WDDI_DETACH();
         return ret_val;

      }

      case WPI_handle_la_link:
      {
         if ( WPI_RegistryResourceEntryValidCheck(wpid, WPI_link_aggregation) )
         {
            wpi_link_aggregation_functions *res_la = NULL;
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryLinkAggregationFunctionsGet,
                                    WPI_REG_LOCKED,
                                    &res_la,
                                    wpid);
            WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
            ret_val = res_la->link_send(handle, data_unit);
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryLinkAggregationFunctionsRelease,
                                        WPI_REG_UNLOCKED,
                                        &res_la);
            WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
         }
         else
         {
            ret_val = WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
         }

         WPI_WDDI_DETACH();
         return ret_val;
      }

      case WPI_handle_ptp:
      {
         if( WPI_RegistryResourceEntryValidCheck(wpid, WPI_ptp) )
         {
            wpi_ptp_functions *res_ptp_functions = NULL;
            
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryPtpFunctionsGet,
                                    WPI_REG_LOCKED,
                                    &res_ptp_functions,
                                    wpid);
            WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
            if (res_ptp_functions)
            {
               if (data_unit->type == WP_DATA_ENET)
               {
                  handle = res_ptp_functions->txbdadjust(handle, 
                                                         data_unit,
                                                         &v_bits,
                                                         &x_bits);
                  WPI_DETACH_RETURN_IF_ERROR_STATUS(handle);
               }
               else if (data_unit->type == WP_DATA_IW)
               {
                  handle = res_ptp_functions->gettxhostchannel(handle);
                  WPI_DETACH_RETURN_IF_ERROR_STATUS(handle);
               }
            }
            
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPtpFunctionsRelease,
                                        WPI_REG_UNLOCKED,
                                        &res_ptp_functions);
            WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);

            if (data_unit->type == WP_DATA_ENET)
            {
               /* Get the channel data based on the protocol. */
               channel_block =
                  &wpi_registry_ptr->pwp_block[wpid].tx_channels;
            }
            else /*if (data_unit->type == WP_DATA_IW)*/
            {
               /* Get the channel data based on the protocol. */
               channel_block =
                  &wpi_registry_ptr->pwp_block[wpid].rx_channels;
            }
            
            
            cptr =
               (wpi_res_channel *) &channel_block->
               channel_data[WPI_HANDLE_INDEX(handle)].mod;
            if (cptr == NULL)
               WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);
            if (!WPI_INITIALIZED(cptr->state))
               WPI_BREAK(ret_val, quit, WP_ERR_HST_INVALID_CHANNEL);
            

         }

         break;
      }


      default:
         WPI_BREAK(ret_val, quit, WP_ERR_HANDLE);
   }

   if ((data_unit->type == WP_DATA_AAL0) || (data_unit->type == WP_DATA_AAL0_OAM))
   {
      WP_U32                 dev_index;
      wpi_resource_entry    *dev_entry;
      wpi_resource_block    *resources;
      WP_handle              device;
      WP_U32                 wpid;
      WP_boolean             aal0_psu_check = WP_FALSE;
      WP_U8                  single_cell_data_size = 0;
      if (cptr == NULL)
      {
          WPI_WDDI_DETACH();
          return WPI_ERROR(WP_ERR_NULL_PTR);
      }

      device = WPI_RES_CHANNEL_DEVICE( cptr );
      wpid = WPI_HANDLE_WPID(device);
      if (WPI_ChannelIsFastByRes(cptr))
      {
         wpi_res_enet_multi_class *res_enet_multi_class = NULL;
         wpi_res_enet_multi_class_device *res_class;
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryEnetMultiClassGet,
                                 WPI_REG_LOCKED,
                                 &res_enet_multi_class,
                                 wpid);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
         WPI_DETACH_RETURN_IF_ERROR(status,
                                    WPI_EnetMultiClassDeviceGetByHandle(res_enet_multi_class,
                                                                        device,
                                                                        &res_class));
         if (res_class->enet_multi_class_g999.six_bytes_header_mode)
         {
            single_cell_data_size = 54;
         }
         else
         {
            single_cell_data_size = 53;
         }
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryEnetMultiClassRelease,
                                     WPI_REG_UNLOCKED,
                                     &res_enet_multi_class);
         WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
         /* AAL0 FAST channels are over FMU, which only supports
          * PSU shaping  */
         aal0_psu_check = WP_TRUE;
      }
      else
      {
         resources = &wpi_registry_ptr->pwp_resources[wpid];
         dev_index = WPI_HANDLE_INDEX(device);
         dev_entry = &resources->entry[dev_index];
         /* Check that device was initialized. */
         if (!WPI_INITIALIZED(dev_entry->state))
         {
            WPI_WDDI_DETACH();
            return WPI_ERROR(WP_ERR_DEVICE);
         }
         dev_atm = WPI_RES_DEV_ATM(dev_entry);

         if (WPI_PKT_SHAPED_ATM_DEVICE(dev_atm))
         {
            aal0_psu_check = WP_TRUE;
         }
         single_cell_data_size = WPI_BD_DATA_SIZE_AAL0;
      }

      if (aal0_psu_check == WP_TRUE)
      {
         /* This is AAL0 PSU OAM case, we change the data unit to AAL5 */
         if (data_unit->type == WP_DATA_AAL0_OAM)
         {
            /* Turn on bit 26 in v_bits for CRC calc. */
            aal0_psu_crc_mode = WP_TRUE;
         }

         data_unit->type = WP_DATA_AAL5;

         /* Verify that the data unit contain one cell only */
         if ((data_unit->n_active != 1) || (data_unit->data_size > single_cell_data_size))
         {
            WPI_WDDI_DETACH();
            return WPI_ERROR(WP_ERR_NUM_OF_CELLS_NEQ_ONE);
         }
      }
   }

   /* Schedule the data unit at the appropriate queue. */
   switch (data_unit->type)
   {
      /* Layer 2 Type 1, Beginning and End */
      case WP_DATA_AAL2_SSSAR:
      case WP_DATA_AAL2_SSTED:
      {
         wpi_memory *memory;
         WP_U32 wmm_cmd;
         WP_U32 wmm_reg = 0;
         WPI_PTR qsr;

         /* For these AAL2 protocols, we can only send on a CID */
         if (! cidgroup)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);
         tx_bd = (wpi_tx_bd_block *) cidgroup->host_io;
         if (tx_bd == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

         /* check sssar pkt len only if it's multi-frame */
         segment = data_unit->segment;
         if (data_unit->n_active > 1) {
            WP_data_segment *check_seg = segment;
            /* advance to the end of the multi-buffer chain */
            for (i = 0; i < (data_unit->n_active - 1); i++)
               check_seg = check_seg->next;

            if(check_seg->data_size < cidptr->pkt_len)
            {
               WPI_BREAK(ret_val,quit,WP_ERR_HST_INVALID_INPUT);
            }
         }

         /* set protocol data as CID number */
         tx_bd->protocol_data = cidptr->cidnumber;
         ret_val = WPI_SendFrame(tx_bd, data_unit, WP_FALSE);
         if (WP_ERROR_P(ret_val))
            break;

         memory = WPI_RegistryMemory(wpid);
         qsr = cidptr->qsr;

         /* OR command */
         wmm_cmd =
            WPI_WMM_HCR_CMD_OR |
            WPI_WMM_HCR_CMD_V_F(WPI_WMM_HCR_CMD_V_VALID)|
            WPI_WMM_HCR_CMD_SIZE_F(WPI_WMM_HCR_CMD_SIZE_4BYTE);

         /* Queues are numbered from msb = 0 to lsb = 15
            but the top 16 bits need to be used */
         wmm_reg = (1 << (31 - cidgroup->queue_number));

         /* Issue command to or in "QSR" bit */
         status = WPL_WmmHostCmd(wpid,
                                 WPI_MemoryBusOfVirtualBus(memory, WP_VB_AAL2_QSR),
                                 &wmm_cmd, &wmm_reg,
                                 &qsr);
         if (status != WP_TRUE)
         {
            status = WPI_WmmHostCmdLocal(wpid,
                  WPI_MemoryBusOfVirtualBus(memory, WP_VB_AAL2_QSR),
                  &wmm_cmd, &wmm_reg,
                  &qsr);
            if (status != WP_OK)
            {
               WPI_WDDI_DETACH();
               return status;
            }
         }
         break;
      }
      case WP_DATA_ENET:
      case WP_DATA_PPP_HDLC:
      {
         if (WPI_ChannelIsFastByHandle(handle) &&
             (((data_unit->type == WP_DATA_ENET) && (WPI_HANDLE_SUBTYPE(handle) == WP_ENET)) ||
              ((data_unit->type == WP_DATA_PPP_HDLC) && (WPI_HANDLE_SUBTYPE(handle) == WP_HDLC))))
         {
            ret_val = WPI_HspiHostSend(handle, data_unit,
                                       v_bits, x_bits);
            if (ret_val != WP_OK)
            {
               goto quit;
            }
         }
         else
         {
            if (data_unit->type == WP_DATA_PPP_HDLC)
            {
               /* Check for sending L2 on PPPmux-enabled MLPPP bundle */
               status = WPI_PppmuxCheckL2HostSend(handle, cptr);
               if (status != WP_OK)
               {
                  WPI_WDDI_DETACH();
                  return status;
               }
            }

            if (WPI_handle_tx_channel == WPI_HANDLE_TYPE(handle))
            {
               /* in case of a software channel, ENET or HS-POS
                  when working in MIXED_SHAPING mode, host send is
                  only possible if the level 2 shaping group is
                  configured in group_mode == WP_MODE_HW. */
               wpi_res_channel_block *channels;
               wpi_res_channel *ch_pkt;

               channels = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
               WPI_ACCESS_CHANNEL(ch_pkt, channels, WPI_HANDLE_INDEX(handle));
               if (ch_pkt == NULL)
               {
                  WPI_WDDI_DETACH();
                  ret_val = WPI_ERROR(WP_ERR_WDDI);
                  goto quit;
               }

               if(WPI_PacketChannelOverShapingGroup(ch_pkt))
               {
                  ret_val = WPI_HspiHostSend(handle, data_unit,
                                             v_bits, x_bits);
                  break;
               }
            }
            /* For these protocols, we send on a channel */
            if (!cptr)
               WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

            tx_bd = (wpi_tx_bd_block *) cptr->host_io;
            if (tx_bd == NULL)
               WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

            ret_val = WPI_SendFrame(tx_bd, data_unit, WP_FALSE);
            if (WP_ERROR_P(ret_val))
               break;
            if (WPI_CH_REGISTRY_AUTOD_V(cptr->gen_config) ==
                WPI_AUTOD_ENABLED)
            {
               /* Issue Tx enable host command for the autodeactivated FTD */
               ret_val = WPI_ReissueFtdEnable(cptr, handle);
            }
         }
         break;
      }
      case WP_DATA_AAL5:
      {
         if (WPI_ChannelIsFastByHandle(handle) &&
             (WPI_HANDLE_SUBTYPE(handle) == WP_ATM_AAL5) )
         {
            /* host send to HW-FMU FIFO. */
            ret_val = WPI_HspiHostSend(handle, data_unit,
                                       v_bits, x_bits);
            if (ret_val != WP_OK)
            {
               goto quit;
            }
         }
         else
         {
            /* SW channels */
            /* For these protocols, we send on a channel */
            if (!cptr)
               WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

            tx_bd = (wpi_tx_bd_block *) cptr->host_io;
            if (tx_bd == NULL)
               WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

            ret_val = WPI_SendFrame(tx_bd, data_unit, aal0_psu_crc_mode);

            /* Check if frame could not be en-queued */
            if (WP_ERROR(ret_val) == WP_ERR_HST_COULD_NOT_ENQUEUE_DU) {
               if (cptr->functions->channel_is_disabled(handle, cptr, &ctd_ptr,&sec_ctd_ptr)) {
                  /* If error in enabling, return */
                  /* chan enable error code.      */
                  status = cptr->functions->channel_enable(handle);
                  if (status != WP_OK)
                     ret_val = status;
               }
            }

            if (WP_ERROR_P(ret_val))
               break;

            /* if it has mq enabled, set the QSR bit */
            if (WPI_MQ_ENABLED(cptr->state)) {
               wpi_resource_block *resources;
               wpi_memory *memory;
               wpi_res_ctd_block *ctde_block;
               wpi_dps_ctd_ctde *host_ctde;
               wpi_res_ctd *ctd_res;
               WPI_PTR ctd;
               WPI_PTR qsr_register;
               WP_U32 wmm_cmd;
               WP_U32 wmm_reg;
               WP_U32 tx_mqlevel;
               WP_U8  drr_mq_enable;

               resources = &wpi_registry_ptr->pwp_resources[wpid];
               memory = WPI_RegistryMemory(wpid);

               ctde_block = &wpi_registry_ptr->pwp_block[wpid].ctdes;
               tx_mqlevel = cptr->du.atm.type.aal5.tx_mqlevel;
               ctd_res = cptr->du.atm.ctd;
               drr_mq_enable = cptr->du.atm.type.aal5.drr_mq_enable ;
               /* Set QSR bit in each CTD for this channel */
               do
               {
                  ctd = ctd_res->ctd;
                  host_ctde = (wpi_dps_ctd_ctde *) (ctde_block->host_offset + ctd);
                  if (drr_mq_enable == WPI_MQ_DRR_ENABLE)
                     qsr_register = WPI_WPI_PTR(&host_ctde->ctde_mode.ctde_drr.ctr,
                                                ctde_block->host_offset);
                  else
                     qsr_register = WPI_WPI_PTR(&host_ctde->ctde_mode.atm_ctde.servedq,
                                                ctde_block->host_offset);

                  wmm_cmd =
                     WPI_WMM_HCR_CMD_OR |
                     WPI_WMM_HCR_CMD_V_F(WPI_WMM_HCR_CMD_V_VALID)|
                     WPI_WMM_HCR_CMD_SIZE_F(WPI_WMM_HCR_CMD_SIZE_4BYTE);

                  /* SES Flag Value to "OR" in */
                  wmm_reg = (0x80000000 >> (16 + tx_mqlevel));

                  /* Issue command to or in "SES" bit */
                  status = WPL_WmmHostCmd(wpid,
                                          WPI_MemoryBusOfVirtualBus(memory, WP_VB_ASU_TXQUEUE),
                                          &wmm_cmd, &wmm_reg,
                                          &qsr_register);
                  if (status != WP_TRUE)
                  {
                     status = WPI_WmmHostCmdLocal(wpid,
                                                  WPI_MemoryBusOfVirtualBus(memory, WP_VB_ASU_TXQUEUE),
                                                  &wmm_cmd, &wmm_reg,
                                                  &qsr_register);
                     if (status != WP_OK)
                     {
                        WPI_WDDI_DETACH();
                        return status;
                     }
                  }
                  ctd_res = ctd_res->next;

               } while (ctd_res != NULL);
            }

            if (WPI_CH_REGISTRY_AUTOD_V(cptr->gen_config) != WPI_AUTOD_DISABLED)
            {
               /* If we're in AAL0 PSU mode we should use FTD not CTD */
               if ((dev_atm != NULL) && (WPI_PKT_SHAPED_ATM_DEVICE(dev_atm)))
               {
                  wpi_memory *memory;
                  WP_U32 ftd;

                  memory = WPI_RegistryMemory(wpid);

                  ftd = cptr->du.atm.ftd;

                  ret_val = (WPI_IssueFtdTxEnable(wpid, memory, cptr, ftd));
               }
               else
               {
                  /* Issue Tx enable host command for the autodeactivated CTD */
                  ret_val = WPI_ReissueCtdEnable(cptr, handle);
               }
            }
         }
         break;
      }
         /* Layer 2 Type 1, Single Unit */
      case WP_DATA_AAL0:
      case WP_DATA_AAL0_OAM:
      case WP_DATA_AAL1:
      {
         /* For these protocols, we send on a channel */
         if (!cptr)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

         tx_bd = (wpi_tx_bd_block *) cptr->host_io;
         if (tx_bd == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

         ret_val = WPI_SendBuffer(tx_bd, data_unit);

         /* Check if cell could not be en-queued */
         if (WP_ERROR(ret_val) == WP_ERR_HST_COULD_NOT_ENQUEUE_DU) {
            if (cptr->functions->channel_is_disabled(handle, cptr, &ctd_ptr, &sec_ctd_ptr)) {
               /* If error in enabling, return */
               /* chan enable error code.      */
               status = cptr->functions->channel_enable(handle);
               if (status != WP_OK)
               {
                  ret_val = status;
               }
            }
         }
         if (WP_ERROR_P(ret_val))
            break;

         /* if it has mq enabled, set the QSR bit */
         if (WPI_MQ_ENABLED(cptr->state) &&
             (data_unit->type == WP_DATA_AAL0 ||
              data_unit->type == WP_DATA_AAL0_OAM)) {

            wpi_resource_block *resources;
            wpi_memory *memory;
            wpi_res_ctd_block *ctde_block;
            wpi_dps_ctd_ctde *host_ctde;
            wpi_res_ctd *ctd_res;
            WPI_PTR ctd;
            WPI_PTR qsr_register;
            WP_U32 wmm_cmd;
            WP_U32 wmm_reg;
            WP_U32 tx_mqlevel;
            WP_U8 drr_mq_enable;

            resources = &wpi_registry_ptr->pwp_resources[wpid];
            memory = WPI_RegistryMemory(wpid);

            ctde_block = &wpi_registry_ptr->pwp_block[wpid].ctdes;
            tx_mqlevel = cptr->du.atm.type.aal0.tx_mqlevel;
            ctd_res = cptr->du.atm.ctd;

            drr_mq_enable = cptr->du.atm.type.aal0.drr_mq_enable ;

            /* Set QSR bit in each CTD for this channel */
            do
            {
               ctd = ctd_res->ctd;
               host_ctde = (wpi_dps_ctd_ctde*)(ctde_block->host_offset + ctd);

               if (drr_mq_enable == WPI_MQ_DRR_ENABLE)
                  qsr_register = WPI_WPI_PTR(&host_ctde->ctde_mode.ctde_drr.ctr,
                                             ctde_block->host_offset);
               else
                  qsr_register = WPI_WPI_PTR(&host_ctde->ctde_mode.atm_ctde.servedq,
                                             ctde_block->host_offset);

               wmm_cmd =
                  WPI_WMM_HCR_CMD_OR |
                  WPI_WMM_HCR_CMD_V_F(WPI_WMM_HCR_CMD_V_VALID)|
                  WPI_WMM_HCR_CMD_SIZE_F(WPI_WMM_HCR_CMD_SIZE_4BYTE);

               /* SES Flag Value to "OR" in */
               wmm_reg = (0x80000000 >> (16 + tx_mqlevel));

               /* Issue command to or in "SES" bit */
               status = WPL_WmmHostCmd(wpid,
                                       WPI_MemoryBusOfVirtualBus(memory, WP_VB_ASU_TXQUEUE),
                                       &wmm_cmd, &wmm_reg,
                                       &qsr_register);

               if (status != WP_TRUE)
               {
                  status = WPI_WmmHostCmdLocal(wpid,
                                               WPI_MemoryBusOfVirtualBus(memory, WP_VB_ASU_TXQUEUE),
                                               &wmm_cmd, &wmm_reg,
                                               &qsr_register);
                  if (status != WP_OK)
                  {
                     WPI_WDDI_DETACH();
                     return status;
                  }
               }

               ctd_res = ctd_res->next;

            } while (ctd_res != NULL);
         }

         if (WPI_CH_REGISTRY_AUTOD_V(cptr->gen_config) != WPI_AUTOD_DISABLED)
         {
            /* Issue Tx enable host command for the autodeactivated CTD */
            ret_val = WPI_ReissueCtdEnable(cptr, handle);
         }

         break;
      }
      case WP_DATA_TRANSPARENT:

         /* For these protocols, we send on a channel */
         if (!cptr)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);
         tx_bd = (wpi_tx_bd_block *) cptr->host_io;
         if (tx_bd == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);
         ret_val = WPI_SendBuffer(tx_bd, data_unit);
         break;
         /* Layer 2 Type 2 */
      case WP_DATA_AAL2_CPS:
      {
         wpi_memory *memory;
         WP_U32 wmm_cmd;
         WP_U32 wmm_reg = 0;
         WPI_PTR qsr;

         /* For this AAL2 protocol, we can only send on a CID */
         if (!cidgroup)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);
         tx_bdd = (wpi_tx_bdd_block *) cidgroup->host_io;
         if (tx_bdd == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);
         /* set protocol data as CID number */
         tx_bdd->protocol_data = cidptr->cidnumber;
         WPI_RecoverDirectBuffers(tx_bdd);
         ret_val = WPI_SendBufferCopy(tx_bdd, data_unit);
         if (WP_ERROR_P(ret_val))
            break;
         /* Set up the AAL2 QSR before transmitting on the CID */

         memory = WPI_RegistryMemory(wpid);
         qsr = cidptr->qsr;

         /* OR command */
         wmm_cmd =
            WPI_WMM_HCR_CMD_OR |
            WPI_WMM_HCR_CMD_V_F(WPI_WMM_HCR_CMD_V_VALID)|
            WPI_WMM_HCR_CMD_SIZE_F(WPI_WMM_HCR_CMD_SIZE_4BYTE);

         /* Queues are numbered from msb = 0 to lsb = 15
            but the top 16 bits need to be used */
         wmm_reg = (1 << (31 - cidgroup->queue_number));

         /* Issue command to or in "QSR" bit */
         status = WPL_WmmHostCmd(wpid,
                                 WPI_MemoryBusOfVirtualBus(memory, WP_VB_AAL2_QSR),
                                 &wmm_cmd,
                                 &wmm_reg, &qsr);
         if (status != WP_TRUE)
         {
            status = WPI_WmmHostCmdLocal(wpid,
                  WPI_MemoryBusOfVirtualBus(memory, WP_VB_AAL2_QSR),
                  &wmm_cmd,
                  &wmm_reg, &qsr);
            if (status != WP_OK)
            {
               WPI_WDDI_DETACH();
               return status;
            }
         }
         break;
      }
      case WP_DATA_FRAME_RELAY:
      {
         wpi_dps_iwbd *host_bd;
         wpi_res_pkt_fr *ch_fr = WPI_RES_PKT_FR(cptr);

         if (!cptr)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);
         host_bd = (wpi_dps_iwbd *) cptr->host_io;
         if (host_bd == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

         if (WPI_MfrChannel(cptr))
         {
        	 ret_val = WPI_MfrSendFrameDirect(WPI_HANDLE_WPID(ch_fr->qnode_handle),
                                       host_bd,
                                       ch_fr->adjunct_pool,
                                       ch_fr->fbp,
                                       data_unit,
                                       0,
                                       &cptr->host_io);
         }
         else
         {
         ret_val = WPI_SendFrameDirect(WPI_HANDLE_WPID(ch_fr->qnode_handle),
                                       host_bd,
                                       ch_fr->adjunct_pool,
                                       ch_fr->fbp,
                                       data_unit,
                                       0,
                                       &cptr->host_io);
         }

         if (ret_val == WP_OK &&
             WPI_CH_REGISTRY_AUTOD_V(cptr->gen_config) ==
             WPI_AUTOD_ENABLED)
         {
            /* Issue Tx enable host command for the autodeactivated FTD */
            ret_val = WPI_ReissueFtdEnable(cptr, handle);
         }

         break;
      }

      /* Layer 3 */
      case WP_DATA_IW:
      {
         wpi_res_node_block *node_block;
         wpi_res_node *qnode_ptr;
         wpi_res_pool *adjunct_pool;
         WP_U32 hiit_index;
         wpi_iw_gap_size *gap_size = NULL;
         WP_handle iw_port_handle;
         WP_U32 fbp =0 ;

         if (!cptr)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

         hiit_index = cptr->du.pkt.type.iw_host.hiit_index;

         gap_size = WPI_RegistryIwGapSize(wpid);
         if (WPI_HANDLE_TYPE(handle) != WPI_handle_rx_channel)
         {
            WPI_BREAK(ret_val, quit, WP_ERR_DATA_UNIT_INCOMPATIBLE);
         }

         node_block = &wpi_registry_ptr->pwp_block[wpid].nodes;
         WPI_ACCESS_QNODE(qnode_ptr, node_block,
                          WPI_HANDLE_INDEX(
                             cptr->du.pkt.type.iw_host.qnode_handle));
         if (qnode_ptr == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

         switch ( qnode_ptr->nodetype)
         {
            case WP_QNODE_IWQ:
               adjunct_pool = qnode_ptr->type.iwq.adjunct_pool;
               fbp = qnode_ptr->type.iwq.fbp;
               break;

            case WP_QNODE_IW_MDB:
               adjunct_pool = qnode_ptr->type.mdb.adjunct_pool[0];
               fbp = qnode_ptr->type.mdb.fbp;
               break;

            case WP_QNODE_IW_HIERARCHICAL:
            {
               WP_handle data_pool_handle = WPI_NULL_HANDLE;
               wpi_res_iw_hierq *hierq = NULL;
               wpi_res_pool_block *pool_block;

               pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
               hierq = &(qnode_ptr->type.hierq);
               ii = hierq->num_groups - 1;

               while (ii >= 0)
               {
                  for (jj = 0; jj < hierq->hier_group[ii].num_of_bd_pools; jj++)
                  {
                     data_pool_handle = hierq->hier_group[ii].bd_pool[jj]->data_buffer_pool_handle;
                     if (data_pool_handle == data_unit->segment[0].pool_handle)
                     {
                        fbp = hierq->hier_group[ii].wmm_fbp[jj];
                        ii = -1;
                        break;
                     }
                  }
                  --ii;
               }

               adjunct_pool = &pool_block->pool_data[WPI_HANDLE_INDEX(data_pool_handle)];
               break;
            }

            default :
               WPI_WDDI_DETACH();               
               return WPI_ERROR(WP_ERR_QNODE);
         }
         if (adjunct_pool->offset < gap_size->min_gap)
            WPI_BREAK(ret_val, quit,
                      WP_ERR_IW_BUFFER_INSUFFICIENT_GAP_SIZE);

         iw_port_handle = cptr->rx_bport_handle;
         if(WPI_HANDLE_IW_PORT_P(iw_port_handle))
         {
            wpi_res_iw_port *res_iw_port;
            WP_handle flow_agg;

            res_iw_port = WPI_RES_IW_PORT(iw_port_handle);
            flow_agg = res_iw_port->flow_agg;

            if(cptr->du.pkt.type.iw_host.pce_connect == WP_DISABLE &&
               WPI_HANDLE_IW_IWGP_AGGREGATION(flow_agg))
            {
               WPI_WDDI_DETACH();               
               return WPI_ERROR(WP_ERR_IW_FLOW_AGG_UNSUPPORTED);
            }
         }

         ret_val = WPI_SendFrameIw(wpid, hiit_index, qnode_ptr,
                                   data_unit,
                                   cptr->du.pkt.type.iw_host.pce_connect,
                                   cptr->du.pkt.type.iw_host.sys_subtype,
                                   cptr->du.pkt.type.iw_host.iw_port_index);

         break;
      }

      default:
         ret_val = WPI_ERROR(WP_ERR_WDDI);
   }

  quit:
   WPI_WDDI_DETACH();
   return ret_val;
}

WP_status WPI_HostReceive(WP_handle handle,
                          WP_data_unit *data_unit)
{
   wpi_rx_bd_block *rx_bd;
   wpi_rx_bdd_block *rx_bdd;
   wpi_res_channel *cptr = NULL;
   wpi_res_cid *cidptr;
   wpi_res_cid_group *cidgroup = NULL;
   WP_U8 data_return_enable = WP_DISABLE;
   WP_status status = WP_OK, lock_status = WP_OK;

   if (data_unit->type & WP_DATA_RETURN_ENABLE)
   {
      data_unit->type &= ~WP_DATA_RETURN_ENABLE;
      data_return_enable = WP_ENABLE;
   }

   switch (WPI_HANDLE_TYPE(handle))
   {
      case WPI_handle_tx_channel:
      {
         /* This handle type is valid only for IW interworking channels. */
         if (WPI_HANDLE_SUBTYPE(handle) != WP_IW_HOST)
         {
            return WPI_ERROR(WP_ERR_HANDLE);
         }
         /* Check the data unit protocol type */
         if (data_unit->type != WP_DATA_IW)
         {
            return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
         }
         status = WPI_HostReceiveIw(handle, data_return_enable,data_unit);
         return status;
         break;
      }

      case WPI_handle_rx_channel:
      {
         /* First, check if channel handle is not a log channel */
         if (data_unit->type != WP_DATA_AAL0_OAM)
         {
            /* Regular channel */
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryRxChannelGet,
                                    WPI_REG_LOCKED,
                                    &cptr,
                                    WPI_HANDLE_WPID(handle),
                                    WPI_HANDLE_INDEX(handle));
            WPI_RETURN_IF_ERROR_STATUS(lock_status);
         }
         else
         {
            /* Common OAM queue */
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryRxChannelGet,
                                    WPI_REG_LOCKED,
                                    &cptr,
                                    WPI_HANDLE_WPID(handle),
                                    0);
            WPI_RETURN_IF_ERROR_STATUS(lock_status);
         }
         /* Lock the controlling structure of the queue.
          * The caller may be blocked for a period of time. */
         if (cptr == NULL)
            return WPI_ERROR(WP_ERR_WDDI);
         if (!WPI_INITIALIZED(cptr->state))
            return WPI_ERROR(WP_ERR_CH_NOT_INITIALIZED);
         /* Find the method to get the resulting buffer(s). */
         switch (data_unit->type)
         {
            /* Layer 2 Type 1, Beginning and End */
            case WP_DATA_AAL5:
            {
               rx_bd = (wpi_rx_bd_block *) cptr->host_io;
               if (rx_bd == NULL)
               {
                  return WPI_ERROR(WP_ERR_WDDI);
                  break;
               }
               status = WPI_ReceiveFrame(rx_bd, data_unit);
               WPI_RETURN_IF_ERROR_STATUS(lock_status);
            }
            break;

            case WP_DATA_PPP_HDLC:
            {
               WP_handle device_handle;
               device_handle = WPI_RES_CHANNEL_DEVICE( cptr );

               switch(WPI_HANDLE_SUBTYPE(device_handle))
               {
                  case WPI_pg_hspi_hspos1:
                  case WPI_pg_hspi_hspos2:
                  case WPI_pg_hspi_hspos3:
                  case WPI_pg_hspi_hspos4:
                  {
                     /* hdlc channel*/
                     status = WPI_HspiHostReceive(handle,
                                                  data_unit,
                                                  cptr->qnode_handle,
                                                  cptr);
                     WPI_RETURN_IF_ERROR_STATUS(status);
                  }
                  break;

                  default:
                  {
                     rx_bd = (wpi_rx_bd_block *) cptr->host_io;
                     if (rx_bd == NULL)
                     {
                        return WPI_ERROR(WP_ERR_WDDI);
                     }
                     status = WPI_ReceiveFrame(rx_bd, data_unit);
                     WPI_RETURN_IF_ERROR_STATUS(status);
                  }
                  break;
               }
               break;
            }
            break;

            /* Layer 2 Type 1, Single Unit */
            case WP_DATA_ENET:
            {
               status = WPI_HspiHostReceive(handle,
                                            data_unit,
                                            cptr->qnode_handle,
                                            cptr);
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
            break;

            /* Layer 2 Type 1, Single Unit */
            case WP_DATA_AAL0:
            case WP_DATA_AAL0_OAM:
            case WP_DATA_AAL1:
            case WP_DATA_TRANSPARENT:
            {
               rx_bd = (wpi_rx_bd_block *) cptr->host_io;
               if (rx_bd == NULL)
               {
                  return WPI_ERROR(WP_ERR_WDDI);
                  break;
               }
               status = WPI_ReceiveBuffer(rx_bd,
                                          data_unit,
                                          WPI_HANDLE_WPID(handle));
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
            break;

            case WP_DATA_FRAME_RELAY:
            {
               WP_U32 fbp;
               wpi_dps_iwbd *host_bd;
               WP_U32 wpid = WPI_HANDLE_WPID(handle);
               wpi_res_pkt_fr *ch_fr = WPI_RES_PKT_FR(cptr);
               wpi_memory *memory = WPI_RegistryMemory(wpid);

               host_bd = (wpi_dps_iwbd *) cptr->host_io;
               if (host_bd == NULL)
                  return WPI_ERROR(WP_ERR_WDDI);

               if (WPI_MfrChannel(cptr))
               {
                  fbp = 0xffffffffU; /* read FBP from BD in the function */
                  status = WPI_MfrReceiveFrameDirect(
					  WPI_HANDLE_WPID(ch_fr->qnode_handle),
					  host_bd,
					  &ch_fr->buf_cnt,
					  &ch_fr->first_rx_bd,
					  ch_fr->adjunct_pool,
					  ch_fr->adjunct_pool_handle,
					  fbp,
					  data_unit,
					  wpid,
					  WPI_MemoryBusOfVirtualBus(memory, WP_VB_ETH_HDLC_CPT),
					  ch_fr->rx_host_counter,
					  &cptr->host_io);
               }
               else
               {
                  fbp = ch_fr->fbp;
               status = WPI_ReceiveFrameDirect(
                  WPI_HANDLE_WPID(ch_fr->qnode_handle),
                  host_bd,
                  &ch_fr->buf_cnt,
                  &ch_fr->first_rx_bd,
                  ch_fr->adjunct_pool,
                  ch_fr->adjunct_pool_handle,
                  fbp,
                  data_unit,
                  wpid,
                  WPI_MemoryBusOfVirtualBus(memory, WP_VB_ETH_HDLC_CPT),
                  ch_fr->rx_host_counter,
                  &cptr->host_io);
               }
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
            break;

            /* Layer 3 */
            case WP_DATA_IW:
            {
               return WPI_ERROR(WP_ERR_NOT_IMPLEMENTED);
            }
            break;

            case WP_DATA_IW_LOG:
            {
               if (WPI_HANDLE_SUBTYPE(handle) != WP_IW_LOG)
               {
                  return WPI_ERROR(WP_ERR_HANDLE);
               }

               status = WPI_IwLoggingHostReceive(handle,
                                                data_unit);
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
            break;

            default:
               return WPI_ERROR(WP_ERR_WDDI);
         }

         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryRxChannelRelease,
                                     WPI_REG_UNLOCKED,
                                     &cptr);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         break;
      }

      /* AAL2 CID group handle */
      case WPI_handle_cid_group:
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryCidGroupGet,
                                 WPI_REG_LOCKED,
                                 &cidgroup,
                                 WPI_HANDLE_WPID(handle),
                                 WPI_HANDLE_INDEX(handle));
         WPI_RETURN_IF_ERROR_STATUS(lock_status);

         if (cidgroup == NULL)
         {
            return WPI_ERROR(WP_ERR_WDDI);
         }
         else
         {
            switch (data_unit->type)
            {
               /* Layer 2 Type 1, Beginning and End */
               case WP_DATA_AAL2_SSSAR:
               case WP_DATA_AAL2_SSTED:
                  rx_bd = (wpi_rx_bd_block *) cidgroup->host_io;
                  if (rx_bd == NULL)
                  {
                     return WPI_ERROR(WP_ERR_WDDI);
                     break;
                  }
                  status = WPI_ReceiveFrame(rx_bd, data_unit);
                  WPI_RETURN_IF_ERROR_STATUS(status);
                  break;
                  /* Layer 2 Type 2 */
               case WP_DATA_AAL2_CPS:
                  rx_bdd = (wpi_rx_bdd_block *) cidgroup->host_io;
                  if (rx_bdd == NULL)
                  {
                     return WPI_ERROR(WP_ERR_WDDI);
                     break;
                  }
                  status = WPI_ReceiveBufferCopy(rx_bdd, data_unit);
                  WPI_RETURN_IF_ERROR_STATUS(status);
                  break;
               default:
                  return WPI_ERROR(WP_ERR_WDDI);
            }
         }
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryCidGroupRelease,
                                     WPI_REG_UNLOCKED,
                                     &cidgroup);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         break;
      }

      /* AAL2 CID handle */
      case WPI_handle_rx_cid:
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryRxCidGet,
                                 WPI_REG_LOCKED,
                                 &cidptr,
                                 WPI_HANDLE_WPID(handle),
                                 WPI_HANDLE_INDEX(handle));
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         if (cidptr == NULL)
            return WPI_ERROR(WP_ERR_WDDI);
         else
         {
            cidgroup = cidptr->cidgroup;
            /* Check if the CID belongs to a group */
            if (!(cidgroup->state & WPI_AAL2_STATE_PRIVATE_GROUP))
            {
               /* We should return an error here because the caller should
                  supply the cid group handle and not the cid handle */
               return WPI_ERROR(WP_ERR_HANDLE);
            }
            switch (data_unit->type)
            {
               /* Layer 2 Type 1, Beginning and End */
               case WP_DATA_AAL2_SSSAR:
               case WP_DATA_AAL2_SSTED:
                  rx_bd = (wpi_rx_bd_block *) cidgroup->host_io;
                  if (rx_bd == NULL)
                  {
                     return WPI_ERROR(WP_ERR_WDDI);
                     break;
                  }
                  status = WPI_ReceiveFrame(rx_bd, data_unit);
                  WPI_RETURN_IF_ERROR_STATUS(status);
                  break;
                  /* Layer 2 Type 2 */
               case WP_DATA_AAL2_CPS:
                  rx_bdd = (wpi_rx_bdd_block *) cidgroup->host_io;
                  if (rx_bdd == NULL)
                  {
                     return WPI_ERROR(WP_ERR_WDDI);
                     break;
                  }
                  status = WPI_ReceiveBufferCopy(rx_bdd, data_unit);
                  WPI_RETURN_IF_ERROR_STATUS(status);
                  break;
               default:
                  return WPI_ERROR(WP_ERR_WDDI);
            }
         }
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryRxCidRelease,
                                     WPI_REG_UNLOCKED,
                                     &cidptr);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         break;
      }

      case WPI_handle_mfr:
      {
         status = WPI_MfrLinkReceive(handle, data_unit);
         WPI_RETURN_IF_ERROR_STATUS(status);
         break;
      }

      default:
         return WPI_ERROR(WP_ERR_HANDLE);
   }
   return status;
}

/*****************************************************************************
 *
 * Function: WP_HostReceive
 *
 * Purpose: Receive a data unit from a host termination channel
 *
 * Description:
 *     Looks for data based on protocol associated with the channel.
 *     Uses channel block associated with the channel.
 *
 * Inputs:
 *     handle          Handle associated with the channel
 *     data_unit       Pointer to data unit to be filled by call.
 *
 * Outputs:
 *     data_unit       Returns no. of segments, data, buffer pools
 *
 * Return Value:       WP_status
 *
 *   WP_OK                         The function has executed successfully.
 *                                 One data unit was stored in the provided
 *                                 data unit descriptor.
 *   WP_ERR_HST_INVALID_CHANNEL    The specified channel does not exist.
 *   WP_ERR_HST_NO_DATA_TO_GET     The rx termination queue is empty.  There
 *                                 were no data units to get.
 *   WP_ERR_HST_DESCR_TOO_SMALL    The size of the data unit found
 *                                 at the rx termination queue was too large
 *                                 to be accomodated in the provided
 *                                 data unit descriptor.
 *   WP_ERR_DATA_NO_BEGINNING      A sequence of valid buffer descriptors
 *                                 found at the rx termination queue had
 *                                 no beginning of data unit indication.
 *   WP_ERR_DATA_NO_END            A sequence of valid buffer descriptors
 *                                 found at the rx termination queue had
 *                                 no end of data unit indication.
 *   WP_ERR_HST_INVALID_INPUT      One or more function argumets appeared to
 *                                 be incorrect or missing.
 *   WP_ERR_NOT_IMPLEMENTED        The functionality implied by the input
 *                                 arguments has not yet been implemented.
 *   WP_ERR_WDDI                   Internal WDDI logic error.
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/

WP_status WP_HostReceive(WP_handle handle,
                         WP_data_unit *data_unit)
{
   WP_status status = WP_OK;

   WPI_WDDI_ATTACH();
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }
   if ((data_unit == NULL) ||
       (data_unit->n_segments == 0) || (data_unit->segment == NULL))
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }
   else
   {
      WP_data_segment *segment = NULL;
      WP_U32 ii = 0;
      /* Check data_unit segments */
      segment = data_unit->segment;
      for (ii = 0; ii < data_unit->n_segments; ii++)
      {
         if (segment == NULL)
         {
            WPI_WDDI_DETACH();
            return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
         }
         segment = segment->next;
      }
   }

   status = WPI_HostReceive(handle, data_unit);

   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function: WP_HostSendDirect
 *
 * Purpose: Sends data unit on direct-queued (non-channel) device.
 *
 * Description:
 *     How does this function work. Is there something tricky done?
 *     Any global state that is read.
 *     Any global state that is written.
 *
 * Inputs:
 *     device_handle   Handle associated with direct-queued device.
 *     priority        Transmit queue priority.
 *     iwqnode_handle  Handle of an interworking qnode.
 *     data_unit       Pointer to data unit to be sent.
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *
 *   WP_OK                            The function has executed successfully.
 *   WP_ERR_HST_INVALID_INPUT         One or more function argumets appeared
 *                                    to by incorrect or missing.
 *   WP_ERR_HANDLE                    Invalid or incorrect device handle.
 *   WP_ERR_DEVICE_NOT_DIRECT_QUEUED  Direct access not supported by device.
 *   WP_ERR_WMM_FIFO_GET              Error in allocating BD from FIFO.
 *   WP_ERR_WDDI                      Internal WDDI logic error.
 *
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/

WP_status WP_HostSendDirect(WP_handle handle,
                            WP_U32 priority,
                            WP_handle iwqnode_handle,
                            WP_data_unit *data_unit)
{
   wpi_res_registry *wpi_registry_ptr;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   wpi_res_device_hspi *dev_hspi = NULL;
   wpi_res_device_hspi_enet *dev_hs_enet = NULL;
   WP_U32 tx_fifo_start = 0;
   WP_U32 wpid;
   wpi_res_node *iwqnode;
   wpi_memory *memory;
   wpi_dps_iwbd** tx_bd = NULL;
   WPI_PTR* bd_addr = NULL;
   WP_U32 v_bits, x_bits;
   WP_U32 bd_bus, buffer_bus, vbank;
   WPI_PTR dataptr, old_buffer;
   WP_status ret_val, status = WP_OK, lock_status = WP_OK;
   WP_U8 *host_offset_buffer, *host_offset_bd;
   WP_data_segment *segment = NULL;
   WP_U32 fbp;
   wpi_res_pool_block *pool_block;
   wpi_res_pool *du_pool, *adjunct_pool;
   wpi_res_node_block *node_block;
   WP_boolean int_enable = WP_FALSE;
   WP_boolean is_hspos = WP_FALSE;
   WP_boolean matrix_buffer_chaining_mode_enabled = WP_FALSE;
   WP_U32 ii;
   WP_S32 hspos_fmu_tq;


   wpid = WPI_HANDLE_WPID(handle);

   if (data_unit == NULL)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   /* Check user data input */
   if (data_unit->n_active == 0)
      return WP_OK;

   /* Data units for direct-queued devices must consist of a single buffer. */
   if (data_unit->n_active > data_unit->n_segments ||
       data_unit->segment == NULL )
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   for (ii = 0; ii < data_unit->n_active; ii++)
   {
      segment = &(data_unit->segment[ii]);
      if (segment->data == NULL)
         return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

      if (!WPI_HANDLE_POOL_P(segment->pool_handle))
         return WPI_ERROR(WP_ERR_POOL);
   }
   /* Direct-queued data units must be of interworking type */
   if (data_unit->type != WP_DATA_IW)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   /* Verify that the input handle is for a direct-queued device. */
   if (WPI_HANDLE_TYPE(handle) != WPI_handle_device)
      return WPI_ERROR(WP_ERR_HANDLE);

   tx_bd = (wpi_dps_iwbd**)WPI_WDDI_MALLOC(data_unit->n_active*sizeof(wpi_dps_iwbd*));
   if (tx_bd == NULL)
   {
      return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
   }

   bd_addr = (WPI_PTR*)WPI_WDDI_MALLOC(data_unit->n_active*sizeof(WPI_PTR));
   if (bd_addr == NULL)
   {
      WPI_WDDI_FREE(tx_bd);
      return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
   }

   WPI_WDDI_ATTACH();

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      WPI_WDDI_FREE(tx_bd);
      WPI_WDDI_FREE(bd_addr);
      return status;
   }

   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      WPI_WDDI_FREE(tx_bd);
      WPI_WDDI_FREE(bd_addr);
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   /*Check if matrix buffer chaining enabled*/
   if (WPI_DpsFeatureCheckStatus(wpid,
                                 WPI_FEATUREMODE_GLOBAL,
                                 0,
                                 WPI_DPS_MODULE_MATRIX_BUFF_CHAIN_ENABLE_)== WP_TRUE)
      matrix_buffer_chaining_mode_enabled = WP_TRUE;

   /* If matrix buffer chaining mode is disabled
      data units for direct-queued devices must consist of a single buffer. */
   if( matrix_buffer_chaining_mode_enabled == WP_FALSE && data_unit->n_active != 1)
   {
      WPI_WDDI_DETACH();
      WPI_WDDI_FREE(tx_bd);
      WPI_WDDI_FREE(bd_addr);
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   resources = &wpi_registry_ptr->pwp_resources[wpid];


   if (WPI_HANDLE_TYPE(handle) == WPI_handle_device)
   {
      WPI_ACCESS_RES(dev_entry, resources, WPI_HANDLE_INDEX(handle));
      dev_hspi = WPI_RES_DEV_HSPI(dev_entry);
   }
   else
   {
      dev_entry = 0;
      dev_hspi = 0;
   }

   memory = WPI_RegistryMemory(wpid);

   /* Find qnode info in registry */
   if (!WPI_HANDLE_Q_NODE_P(iwqnode_handle))
   {
      ret_val = WPI_ERROR(WP_ERR_QNODE);
      goto quit;
   }
   node_block = &wpi_registry_ptr->pwp_block[wpid].nodes;
   WPI_ACCESS_QNODE(iwqnode, node_block, WPI_HANDLE_INDEX(iwqnode_handle));
   if (iwqnode == NULL)
   {
      ret_val = WPI_ERROR(WP_ERR_WDDI);
      goto quit;
   }

   adjunct_pool = iwqnode->type.iwq.adjunct_pool;
   vbank = adjunct_pool->vbank;

   /* Verify that data unit buffer and iwqnode are compatible. */
   pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
   du_pool = (wpi_res_pool *)
      &pool_block->pool_data[WPI_HANDLE_INDEX(segment->pool_handle)];
   if ((du_pool->vbank != vbank) ||
       (du_pool->item_size != adjunct_pool->item_size))
   {
      ret_val = WPI_ERROR(WP_ERR_DATA_UNIT_INCOMPATIBLE);
      goto quit;
   }

   if (dev_hspi)
   {
      if (dev_hspi->pkt.device.devtype != WP_DEVICE_HSPOS)
      {
         /* if the device is not HSPI type, it still can be HS_ENET device */
         dev_hs_enet = WPI_RES_DEV_HSPI_ENET(dev_entry);
         if (dev_hs_enet->hspi.pkt.device.devtype != WP_DEVICE_ENET &&
             dev_hs_enet->hspi.pkt.flowmode != WPI_DEVICE_PKT_FLOWMODE_FAST)
         {
            ret_val = WPI_ERROR(WP_ERR_DEVICE_NOT_DIRECT_QUEUED);
            goto quit;
         }
         else
         {
            if (priority > 3)
            {
               ret_val = WPI_ERROR(WP_ERR_INVALID_PRIORITY);
               goto quit;
            }
            if((dev_hs_enet->hspi.tx_intperiod > 0) && --dev_hs_enet->hspi.tx_intperiod == 0)
            {
               int_enable = WP_TRUE;
               /* reset int_period */
               dev_hs_enet->hspi.tx_intperiod =
                  WPI_HS_ENET_INTPERIOD(dev_hs_enet->hspi.tx_intmode);
            }
         }
         tx_fifo_start = dev_hs_enet->hspi.tx_fifo_start;
      }
      else
      {
         if (priority > 7)
         {
            ret_val = WPI_ERROR(WP_ERR_INVALID_PRIORITY);
            goto quit;
         }
         if((dev_hspi->tx_intperiod > 0) && --dev_hspi->tx_intperiod == 0)
         {
            /* reset int_period */
            dev_hspi->tx_intperiod = WPI_HSPI_INTPERIOD(dev_hspi->tx_intmode);
         }
         tx_fifo_start = dev_hspi->tx_fifo_start;
         is_hspos = WP_TRUE;
      }
   }

   if (dev_hspi == NULL)
   {
      WPI_WDDI_DETACH();
      WPI_WDDI_FREE(tx_bd);
      WPI_WDDI_FREE(bd_addr);
      return WPI_ERROR(WP_ERR_WDDI);
   }

   tx_fifo_start = dev_hspi->tx_fifo_start;

   /* Get a BD from the FBP associated with the iwqnode_handle. */
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
   buffer_bus = adjunct_pool->bus;
   fbp = iwqnode->type.iwq.fbp;
   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
   host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);

   for (ii = 0; ii < data_unit->n_active; ii++)
   {

      segment = &(data_unit->segment[ii]);
      ret_val = WPI_WmmFifoGet(wpid, bd_bus, fbp, &bd_addr[ii]);
      if (ret_val != WP_OK)
         goto quit;

      if (bd_addr[ii] == WPI_PTR_NULL)
      {
         /* If the BD address retreived from the FIFO is 0, this is a fatal error */
         ret_val = WPI_ERROR(WP_ERR_WDDI);
         goto quit;
      }

      tx_bd[ii] = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr[ii], host_offset_bd);

      if(data_unit->control & WP_HT_INTERRUPT_M)
         int_enable = WP_TRUE;

#if _WP_WMM_FIFO_BD_NEW_STRUCT_
      /* Formulate new BD data */
      v_bits = ((WP_U32) WPI_DPS_IWBD_VBITS_V_M |
                WPI_DPS_IWBD_VBITS_I_F(int_enable) |
                WPI_DPS_IWBD_VBITS_E_M) |
                WPI_DPS_IWBD_VBITS_LEN_F(data_unit->data_size);
      x_bits = 0;

      dataptr = WPI_WPI_PTR(segment->data, host_offset_buffer);

      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, tx_bd[ii]->data);
      WPI_NpoolFree(du_pool, old_buffer);

      /* For winsim, copy the data to the socket */
      WPI_MEM_HOST_TO_WINSIM(segment->data, segment->data_size);

     /*In matrix buffer chaining mode we need different treatment:
        In first bd -  MTBD bit should be ON
        In second bd - update x_bits for first bd ( also segment size)
        In third .. bd - bd address should be set to "Next" field of previous BD
        In last bd END bit should be ON, in non-last bd-s END bit should be OFF
        In non-first bd - length should be set to segment data_size
      */
      if((matrix_buffer_chaining_mode_enabled == WP_TRUE) /*&& (data_unit->n_active > 1)*/)
      {
         WP_U32 x_bits_prev;

         /*First bd treatment*/
         if(ii == 0)
         {
            v_bits = v_bits | WPI_DPS_MATRIX_BD_VBITS_MTBD_M;
         }
         else /*Data_size treatment for non-first bd*/
         {
            v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_LEN_M);
            v_bits = v_bits |  WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size);
         }

         /*Second bd treatment - update first bd x_bits*/
         if(ii == 1)
         {
            if(bd_addr[1] > bd_addr[0])
            {
               x_bits_prev = x_bits &(~WPI_DPS_MATRIX_BD_SIGN_M);
               x_bits_prev |= WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[1]-bd_addr[0])>>4);
            }
            else
            {
               x_bits_prev = x_bits | WPI_DPS_MATRIX_BD_SIGN_M;
               x_bits_prev |= WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[0]-bd_addr[1])>>4);
            }
            x_bits_prev |= WPI_DPS_MATRIX_BD_BDLENGTH_F(data_unit->segment[0].data_size);

            WPI_MEM_SET(tx_bd[0]->x_bits, x_bits_prev);
         }
         /*Update "Next" field of previous BD*/
         if(ii > 1)
         {
            WPI_MEM_SET(tx_bd[ii-1]->next_bd, bd_addr[ii]);
         }

         /*Non last bd treatment*/
         if(ii < data_unit->n_active - 1)
         {
            /*Default setting: End bit is ON, here turning it OFF*/
            v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_E_M);
         }
      }/*if((matrix_buffer_chaining_mode_enabled == WP_TRUE)*/

   /* Update the BD */

      WPI_BdIwInit(wpid, tx_bd[ii], 0, dataptr, &v_bits, &x_bits, bd_bus, buffer_bus, fbp);


#else /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ */

      /* Formulate new BD data */
      v_bits = ((WP_U32) WPI_DPS_IWBD_VBITS_V_M |
                WPI_DPS_IWBD_VBITS_I_F(int_enable) |
                WPI_DPS_IWBD_VBITS_E_M |
                WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
                WPI_DPS_IWBD_VBITS_FBP_F(fbp) |
                WPI_DPS_IWBD_VBITS_LEN_F(data_unit->data_size));
      x_bits = 0;

      dataptr = WPI_WPI_PTR(segment->data, host_offset_buffer);

      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, tx_bd[ii]->data);
      WPI_NpoolFree(du_pool, old_buffer);

      /* For winsim, copy the data to the socket */
      WPI_MEM_HOST_TO_WINSIM(segment->data, segment->data_size);

      /* Update the BD */
      WPI_MEM_SET(tx_bd[ii]->next_bd, 0);
      WPI_MEM_SET(tx_bd[ii]->data, dataptr);
      WPI_MEM_SET(tx_bd[ii]->x_bits, x_bits);

      /*In matrix buffer chaining mode we need different treatment:
        In first bd -  MTBD bit should be ON
        In second bd - update x_bits for first bd ( also segment size)
        In third .. bd - bd address should be set to "Next" field of previous BD
        In last bd END bit should be ON, in non-last bd-s END bit should be OFF
        In non-first bd - length should be set to segment data_size
      */
      if((matrix_buffer_chaining_mode_enabled == WP_TRUE) /*&& (data_unit->n_active > 1)*/)
      {
         /*First bd treatment*/
         if(ii == 0)
         {
            v_bits = v_bits | WPI_DPS_MATRIX_BD_VBITS_MTBD_M;
         }
         else /*Data_size treatment for non-first bd*/
         {
            v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_LEN_M);
            v_bits = v_bits |  WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size);
         }

         /*Second bd treatment - update first bd x_bits*/
         if(ii == 1)
         {
            if(bd_addr[1] > bd_addr[0])
            {
               x_bits= x_bits &(~WPI_DPS_MATRIX_BD_SIGN_M);
               x_bits= x_bits | WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[1]-bd_addr[0])>>4);
            }
            else
            {
               x_bits= x_bits | WPI_DPS_MATRIX_BD_SIGN_M;
               x_bits= x_bits | WPI_DPS_MATRIX_BD_DIFFPTR_F((bd_addr[0]-bd_addr[1])>>4);
            }
            x_bits= x_bits |
               WPI_DPS_MATRIX_BD_BDLENGTH_F(data_unit->segment[0].data_size);

            WPI_MEM_SET(tx_bd[0]->x_bits, x_bits);
         }
         /*Update "Next" field of previous BD*/
         if(ii > 1)
         {
            WPI_MEM_SET(tx_bd[ii-1]->next_bd, bd_addr[ii]);
         }

         /*Non last bd treatment*/
         if(ii < data_unit->n_active - 1)
         {
            /*Default setting: End bit is ON, here turning it OFF*/
            v_bits = v_bits & (~ WPI_DPS_IWBD_VBITS_E_M);
         }
      }/*if((matrix_buffer_chaining_mode_enabled == WP_TRUE)*/
      WPI_MEM_SET(tx_bd[ii]->v_bits, v_bits);
#endif /* #else #if _WP_WMM_FIFO_BD_NEW_STRUCT_ */
   }/* for */

   /* Insert the BD in the appropriate TQ FIFO. */
   /* In matrix buffer chaining mode only first bd should be inserted to fifo*/
   status = WPI_CapabilityRead(wpid, WPI_CAP_UPI_HSPOS_FMU_TQ, &hspos_fmu_tq);
   if (WP_ERROR_P(status))
   {
       WPI_WDDI_DETACH();
       WPI_WDDI_FREE(tx_bd);
       WPI_WDDI_FREE(bd_addr);
       return (status);
   }

   if(is_hspos == WP_TRUE && hspos_fmu_tq)
   {
      ret_val = WPI_HwCmdFifoPut(wpid,
                                 bd_bus,
                                 tx_fifo_start + priority,
                                 bd_addr[0],
                                 NULL,
                                 WP_FALSE,
                                 handle,
                                 data_unit->data_size,
                                 fbp);
   }
   else
   {
      ret_val = WPI_WmmFifoPut(wpid, bd_bus,
                               tx_fifo_start + priority,
                               bd_addr[0], (WP_U32*)tx_bd[0], WP_TRUE);
   }
   if (WP_ERROR_P(ret_val))
   {
      /* If FIFO PUT failed, the BD should be returned to its original FIFO */
      if (WP_ERROR_P(WPI_WmmFifoPut(wpid, bd_bus,
                                    fbp, bd_addr[0],
                                    (WP_U32*)tx_bd[0],
                                    WP_TRUE)))
      {
         ret_val = WPI_ERROR(WP_ERR_WDDI);
      }
   }

  quit:

   WPI_WDDI_FREE(tx_bd);
   WPI_WDDI_FREE(bd_addr);

   WPI_WDDI_DETACH();
   return ret_val;
}

/*****************************************************************************
 *
 * Function: WP_HostReceiveDirect
 *
 * Purpose: Receive a data unit from a direct-queued device.
 *
 * Description:
 *     Selects device-specific receive function.
 *
 * Inputs:
 *     handle          Handle of direct-queued device.
 *     data_unit       Pointer to data unit to be filled with received data.
 *
 * Outputs:
 *     data_unit       Data segments with data and buffer pool information.
 *
 * Return Value:       WP_status
 *   WP_OK                         The function has executed successfully.
 *                                 One data unit was stored in the provided
 *                                 data unit descriptor.
 *   WP_ERR_HST_NO_DATA_TO_GET     The rx termination queue is empty.  There
 *                                 were no data units to get.
 *   WP_ERR_HST_INVALID_INPUT      One or more function argumets appeared to
 *                                 be incorrect or missing.
 *   WP_ERR_WDDI                   Internal WDDI logic error.
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/

WP_status WP_HostReceiveDirect(WP_handle device_handle,
                               WP_data_unit *data_unit)
{
   wpi_res_registry *wpi_registry_ptr;
   WP_status ret_val = WP_OK, status = WP_OK, lock_status = WP_OK;
   wpi_resource_block *resources;
   wpi_resource_entry *dev_entry;
   wpi_memory *memory;
   wpi_res_device_hspi *dev_hspi = NULL;
   wpi_res_device_hspi_enet *dev_hs_enet = NULL;
   WP_U32 rx_fifo = 0;
   WP_U32 wpid, v_bits, x_bits, fbp;
   WP_U32 bd_bus, vbank;
   WPI_PTR bd_addr = 0, next_bd_addr = 0, dataptr = 0;
   WP_U8 *host_offset_buffer, *host_offset_bd, *buffer;
   wpi_dps_iwbd *rx_bd;
   wpi_fifo *fifo_block;
   wpi_res_node *iwq;
   WPI_PTR swap_buffer;
   wpi_res_pool *adjunct_pool;
   WP_data_segment *segment;
   WP_U32 i;
   WP_boolean matrix_buffer_chaining_mode_enabled = WP_FALSE;
   WP_boolean matrix_flag = WP_FALSE /*,bd_started_not_ended_flag = WP_FALSE*/;


   if (data_unit == NULL)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   segment = data_unit->segment;

   if (data_unit->n_segments == 0 || segment == NULL)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   /* Direct-queued data units must be of interworking type */
   if (data_unit->type != WP_DATA_IW)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   /* Verify that the input handle is for a direct-queued device. */
   if (WPI_HANDLE_TYPE(device_handle) != WPI_handle_device)
      return WPI_ERROR(WP_ERR_HANDLE);

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   wpid = WPI_HANDLE_WPID(device_handle);

   /*Check if matrix buffer chaining enabled*/
   if (WPI_DpsFeatureCheckStatus(wpid,
                                 WPI_FEATUREMODE_GLOBAL,
                                 0,
                                 WPI_DPS_MODULE_MATRIX_BUFF_CHAIN_ENABLE_)== WP_TRUE)
      matrix_buffer_chaining_mode_enabled = WP_TRUE;


   resources = &wpi_registry_ptr->pwp_resources[wpid];

   WPI_ACCESS_RES(dev_entry, resources, WPI_HANDLE_INDEX(device_handle));
   dev_hspi = WPI_RES_DEV_HSPI(dev_entry);
   data_unit->data_size = 0;

   if (dev_hspi->pkt.device.devtype != WP_DEVICE_HSPOS)
   {
      dev_hs_enet = WPI_RES_DEV_HSPI_ENET(dev_entry);
      if (dev_hs_enet->hspi.pkt.device.devtype != WP_DEVICE_ENET &&
          (dev_hs_enet->hspi.pkt.flowmode != WPI_DEVICE_PKT_FLOWMODE_FAST ||
           dev_hs_enet->hspi.pkt.flowmode != WPI_DEVICE_PKT_FLOWMODE_FMU_HIERARCHICAL))
      {
         ret_val = WPI_ERROR(WP_ERR_DEVICE_NOT_DIRECT_QUEUED);
         goto quit;
      }
      else
      {
         rx_fifo = dev_hs_enet->hspi.rx_fifo;
      }
   }
   else
      rx_fifo = dev_hspi->rx_fifo;

   memory = WPI_RegistryMemory(wpid);

   /* Get a BD from the Host Rx FIFO for the direct-queued device. */
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryFifoGet,
                           WPI_REG_LOCKED,
                           &fifo_block,
                           wpid);
   WPI_DETACH_RETURN_IF_ERROR_STATUS(lock_status);
   for (i = 0; i < data_unit->n_segments; i++)
   {
      if(i == 0)
      {
         ret_val = WPI_WmmFifoGet(wpid, bd_bus, rx_fifo, &bd_addr);
         /* An empty FIFO means there is no data. */
         if (WP_ERROR(ret_val) == WP_ERR_WMM_FIFO_GET)
            ret_val = WPI_ERROR(WP_ERR_HST_NO_DATA_TO_GET);
         if (ret_val != WP_OK)
            goto quit;
      }
      else
      {
         bd_addr = next_bd_addr;
      }

      if (bd_addr == WPI_PTR_NULL)
      {
         ret_val = WPI_ERROR(WP_ERR_HST_NO_DATA_TO_GET);
         goto quit;
      }

      /* Get BD info */
      rx_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr, host_offset_bd);
      WPI_MEM_GET(dataptr, rx_bd->data);
      WPI_MEM_GET(v_bits, rx_bd->v_bits);
      WPI_MEM_GET(x_bits, rx_bd->x_bits);

      /* Get the free buffer pool FIFO and qnode associated with this buffer */
      fbp = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);

      /* Get a buffer from the qnode's associated adjunct pool */
      iwq = fifo_block->fifo[bd_bus][fbp].qnode;
      adjunct_pool = iwq->type.iwq.adjunct_pool;
      swap_buffer = WPI_NpoolAlloc(adjunct_pool);

      if (swap_buffer != WPI_PTR_NULL)
      {
         /* Attach empty buffer to BD and return to FIFO;
            return received buffer to user */

         /* Get info on buffer pool to which this buffer belongs */
         vbank = adjunct_pool->vbank;
         host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);

         /* Populate the data unit with the BD info. */
         buffer = (WP_U8 *) WPI_HOST_PTR(dataptr, host_offset_buffer);
         data_unit->n_active ++;

         if(matrix_buffer_chaining_mode_enabled == WP_TRUE)
         {
            if(v_bits & WPI_DPS_MATRIX_BD_VBITS_MTBD_M)
            {
               /* bd_started_not_ended_flag = WP_TRUE;*/
               matrix_flag = WP_TRUE;
            }
            if(i == 1)
               matrix_flag = WP_FALSE;
         }
         /*In matrix buffer chaining mode in first bd of packet */
         /*there is a different way to calculate bd length*/
         if((matrix_buffer_chaining_mode_enabled == WP_TRUE) &&
            (v_bits & WPI_DPS_MATRIX_BD_VBITS_MTBD_M ))
         {
            segment->data_size =WPI_DPS_MATRIX_BD_BDLENGTH_V( x_bits);
         }
         else
         {
            segment->data_size =
               WPI_DPS_IWBD_VBITS_LEN_V(v_bits);
         }

         data_unit->data_size += segment->data_size;
         segment->data = buffer;
         segment->pool_handle = iwq->type.iwq.adjunct_pool_handle;

         /* For WinSim, copy the data back from the simulator. */
         WPI_MEM_WINSIM_TO_HOST(buffer, data_unit->data_size);

         /* Swap newly allocated buffer into BD and reset BD status */
         WPI_MEM_SET(rx_bd->data, swap_buffer);
         WPI_MEM_SET(rx_bd->v_bits, v_bits &
                     (WPI_DPS_IWBD_VBITS_IWBT_M | WPI_DPS_IWBD_VBITS_FBP_M));

         WPI_TakeNextBdAddress(rx_bd,
                               matrix_flag,
                               &next_bd_addr,
                               host_offset_bd);

         /* Return BD to its originating free buffer pool */
         ret_val = WPI_WmmFifoPut(wpid, bd_bus, fbp, bd_addr,
                                  (WP_U32 *) rx_bd, WP_TRUE);
         segment = segment->next;
         /*In matrix chaining mode if E ( end ) bit is on, go out from loop*/
         if(matrix_buffer_chaining_mode_enabled == WP_TRUE)
         {
            if(v_bits & WPI_DPS_IWBD_VBITS_E_M)
               break;
         }
      }
      else
      {
         /* No empty buffers to attach to BD;
            discard returned frame and return BD to FIFO */

         data_unit->n_active = 0;

         /* Clear valid and state bits in BD */
         WPI_MEM_SET(rx_bd->v_bits, v_bits &
                     (WPI_DPS_IWBD_VBITS_IWBT_M | WPI_DPS_IWBD_VBITS_FBP_M));

         /* Return BD to its originating free buffer pool */
         ret_val = WPI_WmmFifoPut(wpid, bd_bus, fbp, bd_addr,
                                  (WP_U32 *) rx_bd, WP_TRUE);
         if (ret_val == WP_OK)
            ret_val = WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
         break;
      }


   }

  quit:
   WPI_WDDI_DETACH();
   return ret_val;
}

/*****************************************************************************
 *
 * Function: WP_HostRecover
 *
 * Purpose: Recover buffers which have been transmitted.
 *
 * Description:
 *     Look in channel BD for buffers which have been sent.
 *
 * Inputs:
 *     handle          Handle associated with channel.
 *     data_unit       Data unit to which recovered buffers are attached.
 *
 * Outputs:
 *     data_buffer     Segments contain data buffers and pool handles
 *
 * Return Value:       WP_status
 *
 *   WP_OK                       0 or more buffers returned.
 *
 *   WP_ERR_HST_INVALID_INPUT    Data unit had improper values.
 *
 *   WP_ERR_INUSE                Channel block is in use.
 *
 *   WP_ERR_HST_INVALID_CHANNEL  Channel not initialized OR
 *                               Invalid channel type for this call.
 *
 *   WP_ERR_WDDI                 Channel block improperly set up by WDDI.
 *
 *
 * Called by:
 *     Application
 *
 ****************************************************************************/
WP_status WP_HostRecover(WP_handle handle, WP_data_unit *data_unit)
{
   wpi_res_registry *wpi_registry_ptr;
   wpi_res_channel_block *channel_block;
   wpi_res_channel *cptr = NULL;
   wpi_res_cid_block *cid_block;
   wpi_res_cid *cidptr = NULL;
   wpi_tx_bd_block *tx_bd = NULL;
   WP_status ret_val, status = WP_OK, lock_status = WP_OK;
   WP_U32 i;
   WP_data_segment *segment;

   if (data_unit == NULL)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   /* Check data_unit segments */
   segment = data_unit->segment;
   for (i = 0; i < data_unit->n_segments; i++)
   {
      if (segment == NULL)
         return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
      segment = segment->next;
   }

   WPI_WDDI_ATTACH();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   if (WP_ERROR_P(lock_status))
   {
      WPI_WDDI_DETACH();
      return status;
   }
   if (WPI_REGISTRY_USE() == WP_FALSE)
   {
      WPI_WDDI_DETACH();
      return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
   }

   /* Make sure it has enough segments to receive at least
      one data frame */
   data_unit->n_active = 0;
   if (data_unit->n_segments < 1 || data_unit->segment == NULL)
   {
      ret_val = WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
      goto quit;
   }

   /* if it's IW HOST return an error, not allowed */
   if (WPI_HANDLE_SUBTYPE(handle) == WP_IW_HOST)
   {
      ret_val = WPI_ERROR(WP_ERR_HANDLE);
      goto quit;
   }

   switch (WPI_HANDLE_TYPE(handle))
   {
      case WPI_handle_tx_channel:
      {
         /* Get the channel data based on the protocol. */
         channel_block =
            &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(handle)].tx_channels;
         cptr =
            (wpi_res_channel *) &channel_block->
            channel_data[WPI_HANDLE_INDEX(handle)].mod;
         if (cptr == NULL)
         {
            ret_val = WPI_ERROR(WP_ERR_WDDI);
            goto quit;
         }
         if (!WPI_INITIALIZED(cptr->state))
         {
            ret_val = WPI_ERROR(WP_ERR_HST_INVALID_CHANNEL);
            goto quit;
         }

         /* This function shouldn't be called for Frame Relay */
         if (WPI_HANDLE_SUBTYPE(handle) == WP_FRAME_RELAY)
         {
            ret_val = WPI_ERROR(WP_ERR_HST_INVALID_CHANNEL);
            goto quit;
         }

         tx_bd = (wpi_tx_bd_block *) cptr->host_io;
         if (tx_bd == NULL)
         {
            ret_val = WPI_ERROR(WP_ERR_WDDI);
            goto quit;
         }
         ret_val = WPI_RecoverBuffers(tx_bd, data_unit);
         if (ret_val != WP_OK)
            goto quit;
         break;
      }
      case WPI_handle_tx_cid:
      {
         if (WPI_HANDLE_SUBTYPE(handle) == WP_AAL2_CPS)
         {
            ret_val = WPI_ERROR(WP_ERR_HST_INVALID_CHANNEL);
            goto quit;
         }

         cid_block = &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(handle)].tx_cids;
         WPI_ACCESS_AAL2CID(cidptr, cid_block, WPI_HANDLE_INDEX(handle));
         if (cidptr == NULL)
         {
            ret_val = WPI_ERROR(WP_ERR_INUSE);
            goto quit;
         }
         if (!WPI_INITIALIZED(cidptr->state))
         {
            ret_val = WPI_ERROR(WP_ERR_HST_INVALID_CHANNEL);
            goto quit;
         }

         tx_bd = (wpi_tx_bd_block *) cidptr->cidgroup->host_io;
         if (tx_bd == NULL)
         {
            ret_val = WPI_ERROR(WP_ERR_WDDI);
            goto quit;
         }
         /* Set protocol data as CID number */
         tx_bd->protocol_data = cidptr->cidnumber;
         ret_val = WPI_RecoverBuffers(tx_bd, data_unit);
         if (ret_val != WP_OK)
            goto quit;
         break;
      }
      case WPI_handle_gbond_link:
      {
         wpi_atm_gbond_functions* gbond_res;
         void *bd_block;

         gbond_res = WPI_RegistryGbondFunctions(WPI_HANDLE_WPID(handle));

         if( !gbond_res )
           WPI_BREAK(ret_val, quit, WP_ERR_INCORRECT_REGISTRY_STATE);



         WPI_CALL_BREAK_IF_ERROR(ret_val, quit,
                                 gbond_res->get_bd_block(handle, &bd_block));

         tx_bd = (wpi_tx_bd_block *) bd_block;

         if (tx_bd == NULL)
            WPI_BREAK(ret_val, quit, WP_ERR_WDDI);

         ret_val = WPI_RecoverBuffers(tx_bd, data_unit);
         if (ret_val != WP_OK)
            goto quit;
         break;
      }

      default:
         ret_val = WPI_ERROR(WP_ERR_HANDLE);
         goto quit;
   }
   ret_val = WP_OK;
  quit:
   WPI_WDDI_DETACH();
   return ret_val;
}

/*****************************************************************************
  ============================================================================
  ============================================================================
  ==               W D D I   I N T E R N A L   F U N C T I O N S            ==
  ==                         (D E F I N I T I O N S)                        ==
  ============================================================================
  ============================================================================
*****************************************************************************/
/*****************************************************************************
 *
 * Function: WPI_IndRingCreate
 *
 * Purpose: Create an indirect BD ring (points to buffers)
 *
 * Description:
 *     Allocate BD ring from pool.
 *     Allocate and attach buffers for an Rx BD ring.
 *
 * Inputs:
 *     bd_block_ptr    Pointer to the block controlling the BD ring.
 *     ch_direction    WP_CH_RX/WP_CH_TX
 *     ring_pool       Pointer to pool for BD ring.
 *     rx_buffer_pool  Pointer to pool for buffers for Rx channel
 *                     (NULL for Tx channel)
 *     v_bits          A word of values to set in BD control bits.
 *     x_bits          A word of user-defined values to set in BD
 *                     control bits.
 *     bd_base_return  Address at which this function should store
 *                     the WPI_PTR for the BD ring.
 *
 * Outputs:
 *     bd_base_return  WPI_PTR for the BD ring.
 *
 * Return Value:       WP_status
 *
 *   WP_OK             Successful.
 *
 * Called by:
 *     WPI_Aal0CreateRcpt
 *     WPI_Aal0CreateTcpt
 *     WPI_Aal1CreateRcpt
 *     WPI_Aal1CreateTcpt
 *     WPI_Aal2SssarRcqdtInit
 *     WPI_Aal2SssarTcqdtInit
 *     WPI_Aal5CreateRcpt
 *     WPI_Aal5CreateTcpt
 *     WPI_EnetChannelCreate
 *     WPI_HdlcChannelCreate
 *     WPI_MpChannelCreate
 *     WPI_OAMCreateRcpt
 *     WPI_TransTtsatCreate
 *     WPI_TransRtsatCreate
 ****************************************************************************/
WP_status WPI_IndRingCreate(void *bd_block_ptr,
                            WP_mode ch_direction,
                            wpi_res_pool *ring_pool,
                            wpi_res_pool *rx_buffer_pool,
                            WP_handle buffer_handle,
                            WP_U32 v_bits,
                            WP_U32 x_bits,
                            WP_U32 int_period,
                            WPI_PTR *bd_base_return)
{
   WP_status status = WP_OK;
   WP_U32 i;
   WP_U8 *hu_offset = ring_pool->hu_offset;
   WP_U32 n_bds = ring_pool->n_subunits;

   WPI_PTR bd_base = WPI_PTR_NULL;      /* Avoid compiler warnings. */
   wpi_dps_bd *hu_bd_base = 0;
   wpi_dps_bd *hu_bd;

   WPI_PTR next_bd;
   wpi_tx_bd_block *tx_bd = 0;
   wpi_rx_bd_block *rx_bd = 0;

   WPI_PTR data;

   WPI_INTERNAL_FUNCTION();

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start IndRingCreate\n");
   if (n_bds == 0)
   {
      status = WPI_ERROR(WP_ERR_CONFIG);
      goto err_quit;
   }

   /* Get the address of the pool */
   bd_base = WPI_NpoolAlloc(ring_pool);
   if (bd_base == WPI_PTR_NULL)
   {
      status = WPI_ERROR(WP_ERR_NO_FREE_RING);
      goto err_quit;
   }
   *bd_base_return = bd_base;

   /* Compute the uncached address */
   hu_bd = hu_bd_base = (wpi_dps_bd *) WPI_HOST_PTR(bd_base, hu_offset);
   /* Compute tentative address of next BD */
   next_bd = bd_base + sizeof(wpi_dps_bd);

   /* Setup for the case of Rx */
   if (ch_direction == WP_CH_RX)
   {
      for (i = 0; i < n_bds; i++)
      {
         WP_U32 buf_v_bits;

         buf_v_bits = v_bits;
         data = WPI_NpoolAlloc(rx_buffer_pool);
         if (data == WPI_PTR_NULL)
         {
            status = WPI_ERROR(WP_ERR_NO_FREE_BUFFER);
            goto err_quit_rx;
         }
         if (i == n_bds - 1)
            next_bd = bd_base;
         if (int_period > 0)
         {
            /* Set I bit for every int_period BDs */
            if ((i % int_period) == 0)
               buf_v_bits |= WPI_DPS_BD_VBITS_I_M;
            /* If int_period is larger than ring, truncate it
               to ring size (i.e. set I bit in last BD) */
            if ((int_period >= n_bds) && (i == (n_bds - 1)))
               buf_v_bits |= WPI_DPS_BD_VBITS_I_M;
         }
         WPI_MEM_SET(hu_bd->next_bd, next_bd);
         WPI_MEM_SET(hu_bd->v_bits, buf_v_bits);
         WPI_MEM_SET(hu_bd->x_bits, x_bits);
         WPI_MEM_SET(hu_bd->data, data);
         next_bd += sizeof(wpi_dps_bd);
         hu_bd++;

      }
      /* Allocate and construct rx_bd_block */
      if (!
          (rx_bd =
           (wpi_rx_bd_block *) WPI_WDDI_MALLOC(sizeof(wpi_rx_bd_block))))
      {
         status = WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
         goto err_quit_rx;
      }
      rx_bd->host_bd_base = hu_bd_base;
      rx_bd->out = 0;
      rx_bd->n_bds = n_bds;
      rx_bd->buffer_class = 1;
      rx_bd->ring_pool = ring_pool;
      rx_bd->buffer_pool = rx_buffer_pool;
      rx_bd->buffer_handle = buffer_handle;
      rx_bd->buf_size = rx_buffer_pool->unit_size;
      rx_bd->rx_start = 0;      /* for clarity of debugging */
      rx_bd->scan_state = WPI_BdScanNew;        /* initial scan state = New */
      *((wpi_rx_bd_block **) bd_block_ptr) = rx_bd;
   }
   else
   {                            /* Setup for the case of Tx */
      for (i = 0; i < n_bds; i++)
      {
         if (i == n_bds - 1)
            next_bd = bd_base;
         WPI_MEM_SET(hu_bd->next_bd, next_bd);
         WPI_MEM_SET(hu_bd->v_bits, v_bits);
         WPI_MEM_SET(hu_bd->x_bits, x_bits);
         WPI_MEM_SET(hu_bd->data, 0);
         next_bd += sizeof(wpi_dps_bd);
         hu_bd++;
      }
      /* Allocate and construct tx_bd_block */
      if (!(tx_bd = (wpi_tx_bd_block *)
            WPI_WDDI_MALLOC(sizeof(wpi_tx_bd_block) +
                            (n_bds - 1) * sizeof(wpi_tx_saved_data))))
      {
         status = WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
         goto err_quit;
      }
      tx_bd->host_bd_base = hu_bd_base;
      tx_bd->buffer_class = NULL;
      tx_bd->in = 0;
      tx_bd->out = 0;
      tx_bd->n_bds = n_bds;
      tx_bd->open = n_bds;
      tx_bd->ring_pool = ring_pool;
      *((wpi_tx_bd_block **) bd_block_ptr) = tx_bd;
   }
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop IndRingCreate\n");

   status = WP_OK;
   return status;

  err_quit_rx:
  err_quit:
   if (bd_base != WPI_PTR_NULL)
      WPI_NpoolFree(ring_pool, bd_base);
   if (rx_bd != NULL)
   {
      WPI_WDDI_FREE(rx_bd);
   }
   if (tx_bd != NULL)
      WPI_WDDI_FREE(tx_bd);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop IndRingCreate\n");
   return status;
}

/*****************************************************************************
 *
 * Function: WPI_DirectRingCreate
 *
 * Purpose: Create a direct BD ring
 *
 * Description:
 *     Allocate BD ring from direct BD ring pool
 *
 * Inputs:
 *     host_io         A pointer to store the address of the BD control block
 *     ch_direction    WP_CH_RX/WP_CH_TX
 *     ring_pool       Pointer to BD
 *     rx_buffer_pool  Pool to draw buffers from for copying
 *     buffer_handle   Pool handle for rx buffers
 *     v_bits          Common initial values for the BD's
 *     bd_base_return  Pointer at which to store WPI_PTR of first BD

 * Outputs:
 *     host_io         Pointer to BD control block stored here.
 *     bd_base_return  WPI_PTR of first BD stored here.
 *
 * Return Value:       WP_status
 *   WP_OK             Successful return.
 *   WP_ERR_POOL       Pool had insufficient items.
 *   WP_ERR_HST_INVALID_INPUT Invalid arguments supplied
 *   WP_ERR_MEMORY_ALLOCATION Memory allocation error
 *
 * Called by:
 *     WPI_Aal2CpsRcqdtInit
 *     WPI_Aal2CpsTcqdtInit
 ****************************************************************************/
WP_status WPI_DirectRingCreate(void *host_io,
                               WP_mode ch_direction,
                               wpi_res_pool *ring_pool,
                               wpi_res_pool *rx_buffer_pool,
                               WP_handle buffer_handle,
                               WP_U32 v_bits,
                               WP_U32 int_period,
                               WPI_PTR *bd_base_return)
{
   WP_U32 i;
   WP_U8 *hu_offset = ring_pool->hu_offset;
   WP_U32 n_bds = ring_pool->n_subunits;

   WPI_PTR bd_base;
   wpi_dps_bdd *hu_bd_base;
   wpi_dps_bdd *hu_bd;

   WPI_PTR next_bd;
   wpi_tx_bdd_block *tx_bdd = 0;
   wpi_rx_bdd_block *rx_bdd = 0;
   WP_status status = WP_OK;
   WP_U32 bd_vbits;

   WP_U32 bd_size = ring_pool->unit_size;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start DirectRingCreate\n");
   if (n_bds == 0) {
      status = WPI_ERROR(WP_ERR_WDDI);
      goto err_quit;
   }

   /* Get the address of the pool */
   bd_base = WPI_NpoolAlloc(ring_pool);
   if (bd_base == WPI_PTR_NULL) {
      status = WPI_ERROR(WP_ERR_POOL);
      goto err_quit;
   }
   *bd_base_return = bd_base;

   /* Compute the uncached address */
   hu_bd = hu_bd_base = (wpi_dps_bdd *) WPI_HOST_PTR(bd_base, hu_offset);
   /* Compute tentative address of next BD */
   next_bd = bd_base + bd_size;

   /* Construct the linked list of BDs. */
   for (i = 0; i < n_bds; i++)
   {
      if (i == n_bds - 1)
         next_bd = bd_base;
      bd_vbits = v_bits;
      if (int_period > 0)
      {
         if ((i % int_period) == 0)
            bd_vbits |= WPI_DPS_BD_VBITS_I_M;
         if ((int_period >= n_bds) && (i == (n_bds - 1)))
            bd_vbits |= WPI_DPS_BD_VBITS_I_M;
      }
      WPI_MEM_SET(hu_bd->next_bd, next_bd);
      /* write v_bits */
      WPI_MEM_SET(hu_bd->v_bits, bd_vbits);
      WPI_MEM_SET(hu_bd->next_bd, next_bd);
      /* write v_bits */
      WPI_MEM_SET(hu_bd->v_bits, v_bits|0x40000000);
      next_bd += bd_size;
      hu_bd = WPI_BDD_ADD(hu_bd, bd_size);
   }

   /* Now set up the bd control block */
   if (ch_direction == WP_CH_TX)
   {
      /* Allocate and construct tx_bd_block */
      if (!(tx_bdd = (wpi_tx_bdd_block *)
            WPI_WDDI_MALLOC(sizeof(wpi_tx_bdd_block)))) {
         status = WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
         goto err_quit;
      }
      tx_bdd->in = 0;
      tx_bdd->out = 0;
      tx_bdd->n_bds = n_bds;
      tx_bdd->open = n_bds;
      tx_bdd->ring_pool = ring_pool;
      tx_bdd->hu_bd_base = hu_bd_base;
      *((wpi_tx_bdd_block **) host_io) = tx_bdd;
   }
   else
   {
      /* Allocate and construct rx_bdd_block */
      if (!(rx_bdd = (wpi_rx_bdd_block *)
            WPI_WDDI_MALLOC(sizeof(wpi_rx_bdd_block)))) {
         status = WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
         goto err_quit;
      }
      rx_bdd->out = 0;
      rx_bdd->n_bds = n_bds;
      rx_bdd->buffer_pool = rx_buffer_pool;
      rx_bdd->buffer_handle = buffer_handle;
      rx_bdd->ring_pool = ring_pool;
      rx_bdd->hu_bd_base = hu_bd_base;
      *((wpi_rx_bdd_block **) host_io) = rx_bdd;
   }
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop DirectRingCreate\n");

   return WP_OK;

  err_quit:
   if (rx_bdd != NULL)
      WPI_WDDI_FREE(rx_bdd);

   if (tx_bdd != NULL)
      WPI_WDDI_FREE(tx_bdd);

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop DirectRingCreate\n");
   return status;
}

/*****************************************************************************
 *
 * Function: WPI_SendFrame
 *
 * Purpose: Send a packet on a channel.
 *
 * Description:
 *     Attach the data buffers to the channel BD ring.
 *
 * Inputs:
 *     tx_bd           Tx BD control block
 *     data_unit       Data unit containing frame
 *     aal0_oam_en     AAL0 PSU OAM mode
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *     WP_OK                            Successful attachment
 *     WP_ERR_HST_COULD_NOT_ENQUEUE_DU  Insufficient space in Tx BD
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/
WP_status WPI_SendFrame(wpi_tx_bd_block *tx_bd,
                        WP_data_unit *data_unit,
                        WP_boolean aal0_oam_en)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_data_segment *segment;
   wpi_dps_bd *host_bd_base;
   volatile wpi_dps_bd *host_bd;
   volatile wpi_dps_bd *host_first_bd;
   volatile wpi_dps_bd *host_last_bd;
   WP_U32 open;
   WP_U32 i;
   WP_U32 n_bds;
   WP_U32 n_buffs;
   WP_U32 in;
   WP_U32 length;
   WP_U32 v_bits;
   WP_U32 x_bits;
   wpi_bd_vx_bits vx_bits[2];
   wpi_bd_vx_bits *last_vx_bits;
   wpi_tx_saved_data *data_saved_base;
   WP_status lock_status = WP_OK;

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start SendFrame\n");
   /* Check if one can accomodate the data unit described by data_unit
      at the queue. */
   open = tx_bd->open;
   n_bds = tx_bd->n_bds;
   n_buffs = data_unit->n_active;
   if (n_buffs > open)
      return WPI_ERROR(WP_ERR_HST_COULD_NOT_ENQUEUE_DU);

   /* Attach each buffer to a BD in the BDR */
   in = tx_bd->in;
   host_bd_base = tx_bd->host_bd_base;
   host_bd = host_bd_base + in;
   host_first_bd = host_bd;

   /* Initialize last BD to be first BD.  Modified in loop if not so.   */
   host_last_bd = host_bd;
   last_vx_bits = &vx_bits[0];
   data_saved_base = tx_bd->saved_data;

   /* Loop over buffers to be attached to BD's. */
   segment = data_unit->segment;
   for (i = 0; i < n_buffs; i++)
   {
      /* Perform actions that are common to all buffers */
      /* Insert the address of the data into the BD */
      WP_handle pool_handle = segment->pool_handle;
      wpi_res_pool_block *pool_block =
         &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(pool_handle)].pools;
      wpi_res_pool *buffer_pool = (wpi_res_pool *)
         &pool_block->pool_data[WPI_HANDLE_INDEX(pool_handle)];
      WPI_MEM_SET(host_bd->data,
                  WPI_WPI_PTR(segment->data + segment->displacement,
                              buffer_pool->hc_offset));
      /* For Winsim, copy the data to the Winsim space. */
      WPI_MEM_HOST_TO_WINSIM(segment->data + segment->displacement,
                             segment->data_size);
      length = segment->data_size;
      /* Read one word of the BD and adjust the length field and V bit */
      WPI_MEM_GET(v_bits, host_bd->v_bits);
      /* Clear all but testmode bit */
      v_bits &= WPI_DPS_BD_VBITS_TM_M;
      /* Set V and length fields */
      v_bits |=
         (WPI_DPS_BD_VBITS_LENGTH_F(length) |   /* insert length */
          WPI_DPS_BD_VBITS_V_F(1));     /* insert v */

      /* AAL0 PSU OAM mode */
      if (aal0_oam_en)
      {
         v_bits |= WPI_DPS_BD_VBITS_AAL0_PSU_CRC_F(1);
      }

      if (i == 0)
      {
         /* First buffer.  Hold v_bits, x_bits for possible modification */
         WPI_MEM_GET(x_bits, host_bd->x_bits);
         vx_bits[0].v_bits = v_bits;
         vx_bits[0].x_bits = x_bits;
      }
      else if (i != n_buffs - 1)
      {
         /* Not the last buffer -- save new value of v_bits */
         WPI_MEM_SET(host_bd->v_bits, v_bits);
      }
      else
      {
         /* Last buffer (but not the first!) Hold the v_bits and x_bits */
         host_last_bd = host_bd;
         last_vx_bits = &vx_bits[1];
         WPI_MEM_GET(x_bits, host_bd->x_bits);
         vx_bits[1].v_bits = v_bits;
         vx_bits[1].x_bits = x_bits;
      }
      /* Increment to work on the next BD */
      data_saved_base[in].handle = segment->pool_handle;
      data_saved_base[in].displacement = segment->displacement;
      in++;
      host_bd++;
      if (in >= n_bds)
      {
         in = 0;
         host_bd = host_bd_base;
      }
      segment = segment->next;
   }

   /* Turn on E and (conditionally) I bits in last buffer */
   v_bits = last_vx_bits->v_bits;
   last_vx_bits->v_bits = v_bits | WPI_DPS_BD_VBITS_E_F(1)
      | (data_unit->control & WPI_BD_PROTOCOL_INFO_MASK);

   /* adjust first and last (possibly the same)
      vx_bits according to protocol */
   WPI_TxBdAdjust[data_unit->type](vx_bits, last_vx_bits, data_unit, tx_bd);

   /* If more than one buffer, write last BD vx_bits. */
   if (n_buffs > 1)
   {
      WPI_MEM_SET(host_last_bd->v_bits, last_vx_bits->v_bits);
      WPI_MEM_SET(host_last_bd->x_bits, last_vx_bits->x_bits);
   }

   /* Write first BD vx_bits, writing the V bit last */
   WPI_MEM_SET(host_first_bd->x_bits, vx_bits->x_bits);
   WPI_MEM_SET(host_first_bd->v_bits, vx_bits->v_bits);

   /* Update index to the first available
      BD and the number of available BD's */
   tx_bd->in = in;
   tx_bd->open -= n_buffs;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop SendFrame\n");
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_SendBuffer
 *
 * Purpose: Send single unit (AAL0, AAL2 CPS/SW) on channel.
 *
 * Description:
 *     Attach single unit to channel.
 *
 * Inputs:
 *     tx_bd           Pointer to BD control block.
 *     data_unit       Pointer to data unit describing data to be sent.
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *     WP_OK                            Successful attachment
 *     WP_ERR_HST_COULD_NOT_ENQUEUE_DU  Insufficient space on Tx BD ring
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/
WP_status WPI_SendBuffer(wpi_tx_bd_block *tx_bd,
                         WP_data_unit *data_unit)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_data_segment *segment;
   volatile wpi_dps_bd *host_bd_base;
   volatile wpi_dps_bd *host_bd;
   WP_U32 open;
   WP_U32 i;
   WP_U32 n_bds;
   WP_U32 n_buffs;
   WP_U32 in;
   WP_U32 length;
   WP_U32 v_bits;
   wpi_tx_saved_data *data_saved_base;
   WP_U32 int_period;

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start SendBuffer\n");
   /* Check if one can accomodate the data unit described by data_unit
      at the queue. */
   open = tx_bd->open;
   n_bds = tx_bd->n_bds;
   n_buffs = data_unit->n_active;

   if (n_buffs > open)
      return WPI_ERROR(WP_ERR_HST_COULD_NOT_ENQUEUE_DU);

   /* Attach each buffer to a BD in the BDR */
   in = tx_bd->in;
   host_bd_base = tx_bd->host_bd_base;
   host_bd = host_bd_base + in;
   data_saved_base = tx_bd->saved_data;

   int_period = WPI_DU_INTPERIOD(data_unit->control);

   /* Loop over buffers to be attached to BD's. */
   segment = data_unit->segment;
   for (i = 0; i < n_buffs; i++)
   {
      /* Perform actions that are common to all buffers */
      /* Insert the address of the data into the BD */
      WP_handle pool_handle = segment->pool_handle;
      wpi_res_pool_block *pool_block =
         &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(pool_handle)].pools;
      wpi_res_pool *buffer_pool = (wpi_res_pool *)
         &pool_block->pool_data[WPI_HANDLE_INDEX(pool_handle)];
      WPI_MEM_SET(host_bd->data,
                  WPI_WPI_PTR(segment->data + segment->displacement,
                              buffer_pool->hc_offset));
      /* For Winsim, copy the data to the Winsim space. */
      WPI_MEM_HOST_TO_WINSIM(segment->data + segment->displacement,
                             segment->data_size);
      length = segment->data_size;
      /* Read one word of the BD and adjust the length field and V bit */

      WPI_MEM_GET(v_bits, host_bd->v_bits);

      v_bits = (v_bits & ~(WPI_DPS_BD_VBITS_LENGTH_M | /* clear length */
                           WPI_DPS_BD_VBITS_AAL0_CRC10_M |
                           /* clear CRC10 */
                           WPI_DPS_BD_VBITS_I_M)) |    /* clear interrupt */
         (WPI_DPS_BD_VBITS_LENGTH_F(length) |          /* insert length */
          WPI_DPS_BD_VBITS_V_F(1) |                    /* insert v */
          (data_unit->control & WPI_BD_PROTOCOL_INFO_MASK));

      /* set interrupt bit */
      /* and control */
      if (int_period > 0)
      {
         if ((i % int_period) == 0)
            v_bits |= WPI_DPS_BD_VBITS_I_M;
         if ((int_period >= n_buffs) && (i == (n_buffs - 1)))
            v_bits |= WPI_DPS_BD_VBITS_I_M;
      }
      /* Since one buffer is a complete unit, set the V bit directly. */
      WPI_MEM_SET(host_bd->v_bits, v_bits);

      /* Increment to work on the next BD */
      data_saved_base[in].handle = segment->pool_handle;
      data_saved_base[in].displacement = segment->displacement;
      in++;
      host_bd++;
      if (in >= n_bds)
      {
         in = 0;
         host_bd = host_bd_base;
      }
      segment = segment->next;
   }

   /* Update index to the first available BD and
      the number of available BD's */
   tx_bd->in = in;
   tx_bd->open -= n_buffs;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop SendBuffer\n");
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_SendBufferCopy
 *
 * Purpose: Implements WP_HostSend for AAL2 CPS
 *
 * Description:
 *     Copies data to direct buffers.
 *
 * Inputs:
 *     tx_bdd          Pointer to control block for direct BD's.
 *     data_unit       Pointer to data unit being sent.
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *
 *   WP_OK                              Successful send
 *
 *   WP_ERR_HST_COULD_NOT_ENQUEUE_DU    Insufficient space on BD ring
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/
WP_status WPI_SendBufferCopy(wpi_tx_bdd_block *tx_bdd,
                             WP_data_unit *data_unit)
{
   wpi_res_pool *ring_pool = tx_bdd->ring_pool;

   WP_data_segment *segment;
   wpi_dps_bdd *hu_bd_base;
   volatile wpi_dps_bdd *hu_bd;

   WP_U32 bd_size = ring_pool->unit_size;
   WP_U32 open;
   WP_U32 i;
   WP_U32 n_bds;
   WP_U32 n_buffs;
   WP_U32 v_bits;
   WP_U32 in;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start SendBufferCopy\n");
   /* Check if one can accomodate the data unit described by data_unit
      at the queue. */
   open = tx_bdd->open;
   n_buffs = data_unit->n_active;
   if (n_buffs > open)
      return WPI_ERROR(WP_ERR_HST_COULD_NOT_ENQUEUE_DU);

   /* Get local variables from tx_bdd structure. */
   n_bds = tx_bdd->n_bds;
   hu_bd_base = tx_bdd->hu_bd_base;
   in = tx_bdd->in;
   hu_bd = WPI_BDD_ADD(hu_bd_base, in * bd_size);

   /* Loop over buffers to be attached to BD's. */
   segment = data_unit->segment;

   /* check if segment size is reasonable for the bdsize */
   /* subtract the sizes of the next ptr, vbits and aal2 cps header */
   if((bd_size  - (sizeof(WPI_PTR) + sizeof(v_bits) + WPI_AAL2CPS_HDR_SIZE))
      < segment->data_size)
      return WPI_ERROR(WP_ERR_HST_COULD_NOT_ENQUEUE_DU);

   for (i = 0; i < n_buffs; i++)
   {
      /* Call data unit type specific function.
         This function should format the data portion
         of the BD from the segment data. */
      WPI_TxBddAdjust[data_unit->type](hu_bd, data_unit, segment, tx_bdd);

      /* Read one word of the BD and adjust the length field and V bit */
      WPI_MEM_GET(v_bits, hu_bd->v_bits);
      v_bits = (v_bits & ~(WPI_DPS_BD_VBITS_I_M)) |    /* clear interrupt */
         (WPI_DPS_BD_VBITS_V_F(1) |                    /* insert v */
          (data_unit->control & WPI_BD_PROTOCOL_INFO_MASK));
      /* interrupt bit */
      /* Since one buffer is a complete unit, set the V bit directly. */
      WPI_MEM_SET(hu_bd->v_bits, v_bits);

      /* Increment to work on the next BD */
      in++;
      if (in >= n_bds)
      {
         in = 0;
         hu_bd = hu_bd_base;
      }
      else
         hu_bd = WPI_BDD_ADD(hu_bd, bd_size);

      /* Take next segment of data unit */
      segment = segment->next;
   }

   /* Store local variables back to tx_bdd state */
   tx_bdd->in = in;
   tx_bdd->open = open - n_buffs;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop SendBufferCopy\n");
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_RecoverBuffers
 *
 * Purpose: Performs buffer recovery for indirect BD rings.
 *
 * Description:
 *     Scan BD ring for buffers which are completed.
 *
 * Inputs:
 *     tx_bd           Pointer to control block for BD ring
 *     data_unit       Pointer to data unit to receive freed buffer
 *                     pointers and pools.
 *
 * Outputs:
 *     data_unit       Segments contain buffer pointers and pools.
 *
 * Return Value:       void
 *
 * Called by:
 *     WP_HostRecover
 *
 ****************************************************************************/
WP_status WPI_RecoverBuffers(wpi_tx_bd_block *tx_bd,
                             WP_data_unit *data_unit)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 v_bits = 0;
   WP_U32 open;
   WP_U32 out;
   WP_U32 n_bds;
   volatile wpi_dps_bd *host_bd, *host_bd_base;
   WP_data_segment *data_segment;
   WP_U32 i;
   WPI_PTR data;
   WP_U32 limit;
   wpi_tx_saved_data *data_saved_base;
   WP_U32 wpid;
   wpi_res_pool_block *pool_block;
   wpi_res_pool *pool;

   /* Initialize for the BD buffer recovery loop. */

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start RecoverBuffers\n");
   open = tx_bd->open;
   out = tx_bd->out;
   host_bd_base = tx_bd->host_bd_base;
   host_bd = host_bd_base + out;
   n_bds = tx_bd->n_bds;
   limit = n_bds - open;

   /* check to make sure data_unit has enough links to receive the
      total buffers, else set the limit to be the max of data_unit */
   if (data_unit->n_segments < limit)
      limit = data_unit->n_segments;
   data_saved_base = tx_bd->saved_data;
   data_segment = data_unit->segment;

   for (i = 0; i < limit; i++)
   {
      /* Read v_bits, check for V */

      WPI_MEM_GET(v_bits, host_bd->v_bits);
      if (WPI_DPS_BD_VBITS_V_V(v_bits) != 0)
         break;
      if (data_segment == NULL)
         break;
      WPI_MEM_GET(data, host_bd->data);
      data_segment->displacement = 0;
      data_segment->pool_handle = data_saved_base[out].handle;

      /* grab the host/virtual address */
      wpid = WPI_HANDLE_WPID(data_segment->pool_handle);
      pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
      pool =
         &pool_block->pool_data[WPI_HANDLE_INDEX(data_segment->pool_handle)];
      data_segment->data =
         WPI_HOST_PTR((data - data_saved_base[out].displacement),
                      pool->hu_offset);
      WPI_MEM_SET(host_bd->data, WPI_PTR_NULL);
      WPI_CIRC_INCR(out, n_bds, host_bd, host_bd_base);
      data_segment = data_segment->next;
   }

   /* Store error state of last buffer.
      This info is used to determine the state of the last
      transmitted packet in the event of a crash. */
   if (limit > 0)
      tx_bd->error_state = v_bits;

   data_unit->n_active = i;
   /* Update the out and open values.
      segments is the number of buffers recovered. */
   tx_bd->out = out;
   tx_bd->open = open + i;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop RecoverBuffers\n");
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_RecoverDirectBuffers
 *
 * Purpose: Performs buffer recovery for direct BD rings.
 *
 * Description:
 *     No apparent useful purpose.
 *
 * Inputs:
 *     tx_bdd          Pointer to direct BD ring control block
 *
 * Outputs:
 *
 * Return Value:       void
 *
 * Called by:
 *     WP_HostRecover
 *
 ****************************************************************************/
void WPI_RecoverDirectBuffers(wpi_tx_bdd_block *tx_bdd)
{
   wpi_res_pool *ring_pool = tx_bdd->ring_pool;
   WP_U32 v_bits;
   WP_U32 bd_size = ring_pool->unit_size;
   WP_U32 open;
   WP_U32 out;
   WP_U32 n_bds;
   volatile wpi_dps_bdd *host_bd, *host_bd_base;
   WP_U32 i;
   WP_U32 limit;

   /* Initialize for the BD buffer recovery loop. */

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start RecoverDirectBuffers\n");
   open = tx_bdd->open;
   out = tx_bdd->out;
   host_bd_base = tx_bdd->hu_bd_base;
   host_bd = WPI_BDD_ADD(host_bd_base, out * bd_size);
   n_bds = tx_bdd->n_bds;
   limit = n_bds - open;

   for (i = 0; i < limit; i++)
   {
      /* Read v_bits, check for V */
      WPI_MEM_GET(v_bits, host_bd->v_bits);
      if (WPI_DPS_BD_VBITS_V_V(v_bits) != 0)
         break;

      /* Increment to work on the next BD */
      out++;
      if (out >= n_bds)
      {
         out = 0;
         host_bd = host_bd_base;
      }
      else
         host_bd = WPI_BDD_ADD(host_bd, bd_size);
   }

   /* Update the out and open values.
      i is the number of buffers recovered. */
   tx_bdd->out = out;
   tx_bdd->open = open + i;
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop RecoverDirectBuffers\n");
}

/*****************************************************************************
 *
 * Function: WPI_BdClearFrame
 *
 * Purpose: Clear up BD ring for an errored frame.
 *
 * Description:
 *     Loop through BD ring revalidating the bad buffers.
 *
 * Inputs:
 *     bd_base         Pointer to first BD of BD ring.
 *     first           Index of first BD to clear.
 *     after           Index of BD after last BD to clear.
 *     n_bds           Number of BDs in the ring.
 *
 * Outputs:
 *
 * Return Value:       void
 *
 * Called by:
 *     WPI_ReceiveFrame
 *
 ****************************************************************************/
static void WPI_BdClearFrame(wpi_dps_bd *host_bd_base,
                             WP_U32 first,
                             WP_U32 after,
                             WP_U32 n_bds)
{
   WP_U32 v_bits;
   volatile wpi_dps_bd *host_bd = host_bd_base + first;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start BdClearFrame\n");
   while (first != after)
   {
      WPI_MEM_GET(v_bits, host_bd->v_bits);
      WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);
      WPI_CIRC_INCR(first, n_bds, host_bd, host_bd_base);
   }
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop BdClearFrame\n");
}

/*****************************************************************************
 *
 * Function: WPI_BdSwapBuffers
 *
 * Purpose: Replace received buffers in BD ring with new ones from pool.
 *
 * Description:
 *     This function swaps buffers for one frame from an Rx BD ring
 *     with buffers allocated from the associated pool, and then
 *     attaches those received buffers to a user-supplied data unit.
 *     This function must only be called if it is known that there
 *     are sufficient buffers in the associated pool.
 *
 * Inputs:
 *     rx_bd           Pointer to BD control block
 *     first           Index of first BD to swap
 *     last            Index of last BD to swap
 *     n_bds           Number of BDs in ring
 *     output          Pointer to data unit receiving buffers
 *
 * Outputs:
 *     output          Data unit receiving buffers
 *
 * Return Value:       WP_OK
 *                     WP_ERR_WDDI - Should never happen
 *
 * Called by:
 *     WPI_ReceiveFrame
 *
 ****************************************************************************/
WP_status WPI_BdSwapBuffers(wpi_rx_bd_block *rx_bd,
                            WP_U32 first,
                            WP_U32 last,
                            WP_data_unit *output)
{
   WP_data_segment *segment = output->segment;
   WP_data_segment *next_to_last_segment = output->segment;
   wpi_dps_bd *host_bd_base = rx_bd->host_bd_base;
   volatile wpi_dps_bd *host_bd = host_bd_base + first;
   WP_U32 buf_size = rx_bd->buf_size;
   WP_U32 n_bds = rx_bd->n_bds;
   WPI_PTR full_data;
   WP_U32 n_used = 1;
   WP_U32 running_size = 0;
   WP_U32 total_size;
   WP_U32 data_size;
   WP_U32 v_bits;
   wpi_res_pool *pool = NULL;
   WP_handle buffer_handle = rx_bd->buffer_handle;
   WP_U8 *hu_offset = NULL;
   WPI_PTR item;
   wpi_bd_vx_bits vx_bits;
   WP_U32 last_buffer_extra_data;

   WP_status lock_status = WP_OK;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start BdSwapBuffers\n");
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPoolFromPointerGet,
                           WPI_REG_LOCKED,
                           &pool,
                           rx_bd->buffer_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   hu_offset = pool->hu_offset;
   /* Do all but last buffer (possibly none). */
   while (first != last)
   {
      /* get a replacement buffer */
      item = WPI_NpoolAlloc(pool);
      /* This should never happen. This function should never be called
         with insufficient buffers in the pool. */
      if (item == WPI_PTR_NULL)
         return WPI_ERROR(WP_ERR_WDDI);
      WPI_MEM_GET(full_data, host_bd->data);
      /* do the swap */
      segment->data = WPI_HOST_PTR(full_data, hu_offset);
      segment->pool_handle = buffer_handle;
      WPI_MEM_SET(host_bd->data, item);
      /* for all but the last buffer, the size is the maximum size. */
      segment->data_size = buf_size;
      running_size += buf_size;
      /* for WinSim, copy the data back from the simulator. */
      WPI_MEM_WINSIM_TO_HOST(segment->data, buf_size);
      /* clear the status bits */
      WPI_MEM_GET(v_bits, host_bd->v_bits);
      WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);
      /* set for next iteration */
      n_used++;
      next_to_last_segment = segment;
      segment = segment->next;
      WPI_CIRC_INCR(first, n_bds, host_bd, host_bd_base);
   }
   /* Do last buffer; first == last */
   /* read out the data addresses */
   item = WPI_NpoolAlloc(pool);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryPoolRelease,
                               WPI_REG_UNLOCKED,
                               &pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* This should never happen. This function should never be called
      with insufficient buffers in the pool. */
   if (item == WPI_PTR_NULL)
      return WPI_ERROR(WP_ERR_WDDI);
   WPI_MEM_GET(full_data, host_bd->data);
   /* do the swap */
   segment->data = WPI_HOST_PTR(full_data, hu_offset);
   segment->pool_handle = buffer_handle;
   WPI_MEM_SET(host_bd->data, item);
   /* for the last buffer, the data size must be computed. */
   WPI_MEM_GET(v_bits, host_bd->v_bits);
   total_size = WPI_DPS_BD_VBITS_LENGTH_V(v_bits);

   if (total_size < running_size)
   {
      /* this segment contains no real data,
         can happen when dps allocates a bd that contains only AAL5 trailer */
      segment->data_size = 0;
      last_buffer_extra_data = running_size - total_size;
      next_to_last_segment->data_size = next_to_last_segment->data_size - last_buffer_extra_data;
   }
   else
   {
      data_size = total_size - running_size;
      segment->data_size = data_size;
      /* for WinSim, copy the data back from the simulator. */
      WPI_MEM_WINSIM_TO_HOST(segment->data, data_size);
   }


   /* clear the status bits */
   WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);

   vx_bits.v_bits = v_bits;
   WPI_MEM_GET(vx_bits.x_bits, host_bd->x_bits);

   /* set output values */
   output->n_active = n_used;
   output->data_size = total_size;

   /* Set the return values coming from v_bits */
   output->status = v_bits & WPI_BD_PROTOCOL_INFO_MASK;

   WPI_RxBdReport[output->type](&vx_bits, output);

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop BdSwapBuffers\n");
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_ReceiveFrame
 *
 * Purpose: Try to receive a frame from a BD ring.
 *
 * Description:
 *     There is a state machine that keeps track of the types of
 *     BD flags (B,E) which have been encountered and hence are
 *     allowed.
 *
 * Inputs:
 *     rx_bd           Pointer to BD control block.
 *     output          Pointer to data unit for received dataq.
 *
 * Outputs:
 *     output          Data unit receiving data
 *
 * Return Value:       WP_status
 *     WP_OK                               Data returned
 *     WP_ERR_HST_NO_DATA_TO_GET           No errors, but no data
 *     WP_ERR_HST_DU_NO_BEGINNING          Data unit had no beginning
 *     WP_ERR_HST_DU_NO_END                Data unit had no end
 *     WP_ERR_HST_DU_NO_BEGINNING_OR_END   Data unit had no start or end
 *     WP_ERR_HST_DESCR_TOO_SMALL          BD too small to hold full frame
 *     WP_ERR_HST_INSUFFICIENT_BUFFERS_IN_POOL  Pool too empty to detach
 *                                              entire frame of buffers
 *                                              from ring.
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_ReceiveFrame(wpi_rx_bd_block *rx_bd,
                           WP_data_unit *output)
{
   WP_U32 go;
   WP_U32 scan_state;
   WP_U32 out;
   WP_U32 n_bds;
   WP_U32 action;
   WP_U32 rx_start;
   WP_U32 rx_end;
   WP_U32 report = WP_OK;
   volatile wpi_dps_bd *host_bd_base, *host_bd;
   WP_U32 v_bits;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start ReceiveFrame\n");
   /* move rx_bd_block vars to local vars */
   /* read/write */
   scan_state = rx_bd->scan_state;
   out = rx_bd->out;
   rx_start = rx_bd->rx_start;
   rx_end = rx_start;   /* For state Pre, this is the right initialization.
                         * For ReportFrame, this gets set to the
                         * right value. */

   output->n_active = 0;

   /* read only */
   host_bd_base = rx_bd->host_bd_base;
   host_bd = host_bd_base + out;
   n_bds = rx_bd->n_bds;

   /* If previous call returned error condition and set up 1-buffer frame. */
   /* state = New, report = Frame, return 1-buffer frame */
   if (scan_state == WPI_BdScanPre)
   {
      report = (WP_U32) WPI_BdReportFrame;
      scan_state = WPI_BdScanNew;
   }

   else
   {                            /* do scan */
      /* The scan loop processes as many BD's as it can.  */
      go = 1;
      report = (WP_U32) WPI_BdReportNoFrame;
      while (go)
      {
         WPI_MEM_GET(v_bits, host_bd->v_bits);
         if (WPI_DPS_BD_VBITS_V_V(v_bits) == 0)
         {
            if (scan_state == WPI_BdScanError)
               scan_state = WPI_BdScanSkip;
            break;
         }
         action = scan_state + WPI_DPS_BD_VBITS_EB_V(v_bits);
         switch (action)
         {
            case WPI_BdActionNewE0B0:
               /* Missing beginning with no previous error. */
               /* state = Error, report = NoB, clear */
               scan_state = WPI_BdScanError;
               report = (WP_U32) WPI_BdReportNoB;
               WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);
               break;
            case WPI_BdActionNewE0B1:
               /* B expected and found with no E. */
               /* go = 1, state = Cont, start new frame */
            case WPI_BdActionSkipE0B1:
               /* Skipping to E or B.  Found B. */
               /* go = 1, state = Cont, start new frame */
               scan_state = WPI_BdScanCont;
               rx_start = out;
               break;
            case WPI_BdActionNewE1B0:
               /* B expected, but only E found with no previous error. */
               /* go = 1, state = New, report = NoB, clear */
               scan_state = WPI_BdScanNew;
               WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);
               report = (WP_U32) WPI_BdReportNoB;
               break;
            case WPI_BdActionNewE1B1:
               /* B expected, and BE found. */
               /* go = 0, state = New, report = Frame,
                  collect 1-buffer frame */
               go = 0;
               report = (WP_U32) WPI_BdReportFrame;
               rx_start = rx_end = out;
               break;
            case WPI_BdActionContE0B0:
               /* Looking for E with no B.  Found neither B nor E. */
               /* go = 1, state = Cont */
               break;
            case WPI_BdActionContE0B1:
               /* Looking for E with no B.  Found a B instead. */
               /* go = 0, state = Cont, report = NoE, clear old frame,
                * start new frame. */
               go = 0;
               report = (WP_U32) WPI_BdReportNoE;
               WPI_BdClearFrame(rx_bd->host_bd_base, rx_start, out, n_bds);
               rx_start = out;
               break;
            case WPI_BdActionContE1B0:
               /* Looking for E with no B.  Found it. */
               /* go = 0, state = New, report = Frame, return frame */
               go = 0;
               scan_state = WPI_BdScanNew;
               report = (WP_U32) WPI_BdReportFrame;
               rx_end = out;
               break;
            case WPI_BdActionContE1B1:
               /* Looking for E with no B.  Found E and B. */
               /* go = 0, state = Pre, report = NoE, clear old frame,
                * start new frame. */
               go = 0;
               scan_state = WPI_BdScanPre;
               report = (WP_U32) WPI_BdReportNoE;
               WPI_BdClearFrame(rx_bd->host_bd_base, rx_start, out, n_bds);
               rx_start = out;
               break;
            case WPI_BdActionErrorE0B0:
               /* Skipping to E or B with error to report.  Found neither. */
               /* go = 1, state = Error (unchanged), clear */
            case WPI_BdActionSkipE0B0:
               /* Skipping to E or B.  Found neither. */
               /* go = 1, state = Skip (unchanged), clear */
               go = 0;
               WPI_MEM_GET(v_bits, host_bd->v_bits);
               WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);
               break;
            case WPI_BdActionErrorE0B1:
               /* Skipping to E or B with error to report.  Found B. */
               /* go = 0, state = Cont, start new frame */
               go = 0;
               scan_state = WPI_BdScanCont;
               rx_start = out;
               break;
            case WPI_BdActionErrorE1B0:
               /* Skipping to E or B with error to report.  Found E. */
               /* go = 0; state = New, clear */
               go = 0;
               scan_state = WPI_BdScanNew;
               WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);
               break;
            case WPI_BdActionErrorE1B1:
               /* Skipping to E or B with error to report.  Found both. */
               /* go = 0; state = Pre; report = No_BE; start new frame */
               go = 0;
               scan_state = WPI_BdScanPre;
               report = (WP_U32) WPI_BdReportNoBE;
               rx_start = out;
               break;
            case WPI_BdActionSkipE1B0:
               /* Skipping to E or B.  Found E. */
               /* go = 1; state = New, clear */
               scan_state = WPI_BdScanNew;
               WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_TM_M);
               break;
            case WPI_BdActionSkipE1B1:
               /* Skipping to E or B.  Found both. */
               /* go = 0; state = New; report = Frame;
                  return 1-buffer frame. */
               go = 0;
               scan_state = WPI_BdScanNew;
               report = (WP_U32) WPI_BdReportFrame;
               rx_start = out;
               rx_end = out;
               break;
         }
         WPI_CIRC_INCR(out, n_bds, host_bd, host_bd_base);
      }                         /* while(go) */
   }                            /* else (do scan) */

   /* Return a frame, if one is complete. */
   if (report == (WP_U32) WPI_BdReportFrame)
   {
      WP_U32 buffers_required;

      /* Ensure that the pool has enough buffers to receive
         this frame. */
      if (rx_end >= rx_start)
         buffers_required = rx_end - rx_start + 1;
      else
      {
         buffers_required = (rx_bd->n_bds + 1 + rx_end) - rx_start;
      }
      if (buffers_required > rx_bd->buffer_pool->count)
      {
         /* Currently insufficient buffers to receive frame.
            Leave frame in BD ring and exit without updating ring. */
         report = WPI_ERROR(WP_ERR_HST_INSUFFICIENT_BUFFERS_IN_POOL);
         goto _receive_frame_exit;
      }
      if (buffers_required > output->n_segments)
      {
         /* Data unit is too small for frame.
            Leave frame in BD ring and exit without updating ring. */
         report = WPI_ERROR(WP_ERR_HST_DESCR_TOO_SMALL);
         goto _receive_frame_exit;
      }

      report = WPI_BdSwapBuffers(rx_bd, rx_start, rx_end, output);
   }

   /* restore read/write block vars */
   rx_bd->scan_state = scan_state;
   rx_bd->out = out;
   rx_bd->rx_start = rx_start;

   /* return result */
  _receive_frame_exit:
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop ReceiveFrame\n");
   return report;
}

/*****************************************************************************
 *
 * Function: WPI_ReceiveBuffer
 *
 * Purpose: Receive single-buffer data.
 *
 * Description:
 *     Check next BD in ring.  There is no need to look for B/E
 *     information.
 *
 * Inputs:
 *     rx_bd           Pointer to BD control block
 *     output          Pointer to data unit receiving output
 *     wpid            Identifies WinPath #.
 *
 * Outputs:
 *     output          Data unit receiving output
 *
 * Return Value:       WP_status
 *   WP_OK                       Successful recovery of a buffer
 *   WP_ERR_HST_NO_DATA_TO_GET   No errors, but no data.
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_ReceiveBuffer(wpi_rx_bd_block *rx_bd,
                            WP_data_unit *output,
                            WP_U32 wpid)
{
   WP_U32 i;
   WP_U32 out;
   WP_U32 n_bds;
   volatile wpi_dps_bd *host_bd_base, *host_bd;
   WP_U32 v_bits;
   WP_data_segment *segment = output->segment;
   WPI_PTR full_data;
   wpi_res_pool *pool = NULL;
   WP_U8 *hu_offset = NULL;
   WPI_PTR item;
   WP_handle buffer_handle = rx_bd->buffer_handle;
   wpi_bd_vx_bits vx_bits;
   WP_status status = WP_OK, lock_status = WP_OK;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start ReceiveBuffer\n");
   /* move rx_bd_block vars to local vars */
   /* read/write */
   out = rx_bd->out;

   /* read only */
   host_bd_base = rx_bd->host_bd_base;
   host_bd = host_bd_base + out;
   n_bds = rx_bd->n_bds;

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPoolFromPointerGet,
                           WPI_REG_LOCKED,
                           &pool,
                           rx_bd->buffer_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   hu_offset = pool->hu_offset;
   /* Return as many buffers as requested, if possible.  */
   for (i = 0; i < output->n_segments; i++)
   {
      WPI_MEM_GET(v_bits, host_bd->v_bits);
      if (WPI_DPS_BD_VBITS_V_V(v_bits) == 0)
      {
         break;
      }
      /* read out the data addresses */
      item = WPI_NpoolAlloc(pool);
      if (item == WPI_PTR_NULL)
      {
         break;
      }
      WPI_MEM_GET(full_data, host_bd->data);
      /* do the swap */
      segment->data = WPI_HOST_PTR(full_data, hu_offset);
      segment->pool_handle = buffer_handle;
      WPI_MEM_SET(host_bd->data, item);
      if ((output->type == WP_DATA_AAL0) ||
          (output->type == WP_DATA_AAL0_OAM))
         segment->data_size = WPI_AAL0_BUFFER_SIZE;
      else
         segment->data_size = WPI_DPS_BD_VBITS_LENGTH_V(v_bits);
      /* for WinSim, copy the data back from the simulator. */
      WPI_MEM_WINSIM_TO_HOST(segment->data, segment->data_size);
      /* clear the status bits */
      if (output->type == WP_DATA_TRANSPARENT)
         WPI_MEM_SET(host_bd->v_bits, v_bits & ~WPI_DPS_BD_VBITS_V_M);
      else
         WPI_MEM_SET(host_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_AAL0_M);
      vx_bits.v_bits = v_bits;
      WPI_MEM_GET(vx_bits.x_bits, host_bd->x_bits);

      /* Set the return values coming from v_bits */
      output->status = v_bits & WPI_BD_PROTOCOL_INFO_MASK;

      if (output->type != WP_DATA_AAL0_OAM)
         WPI_RxBdReport[output->type](&vx_bits, output);
      else
      {
         status = WPI_RxBdReportAal0OAM(&vx_bits, segment, wpid);
         WPI_RETURN_IF_ERROR_STATUS(status);
      }

      output->data_size = segment->data_size;

      /* set for the next iteration. */
      segment = segment->next;
      WPI_CIRC_INCR(out, n_bds, host_bd, host_bd_base);
   }
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryPoolRelease,
                               WPI_REG_UNLOCKED,
                               &pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   /* set output values */
   output->n_active = i;

   /* restore read/write block vars */
   rx_bd->out = out;

   /* return result */
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop ReceiveBuffer\n");
   return (i == 0) ? WPI_ERROR(WP_ERR_HST_NO_DATA_TO_GET) : WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_ReceiveBufferCopy
 *
 * Purpose: Implements host receive for direct buffers.
 *
 * Description:
 *     This looks for data received using direct buffers.  It is not
 *     necessary to look for B/E.
 *
 * Inputs:
 *     rx_bdd          Control block for Rx BDs
 *     output          Pointer to data unit for received data
 *
 * Outputs:
 *     output          Data unit segments get received data.
 *
 * Return Value:       WP_status
 *
 *   WP_OK                       Successful receive of a data unit
 *   WP_ERR_HST_NO_DATA_TO_GET    No data present to be received.
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_ReceiveBufferCopy(wpi_rx_bdd_block *rx_bdd,
                                WP_data_unit *output)
{
   wpi_res_pool *ring_pool = NULL;
   wpi_res_pool *buffer_pool = NULL;
   WP_U32 i;
   WP_U32 n_bds;
   wpi_dps_bdd *hu_bd;
   wpi_dps_bdd *hu_bd_base;
   WP_U32 v_bits;
   WP_data_segment *segment = output->segment;
   WP_handle buffer_handle = rx_bdd->buffer_handle;

   WP_U32 total_data_size = 0;
   WP_U32 out;
   WP_U32 bd_size = 0;
   WP_status lock_status = WP_OK;

   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "start ReceiveBufferCopy\n");

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPoolFromPointerGet,
                           WPI_REG_LOCKED,
                           &ring_pool,
                           rx_bdd->ring_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   bd_size = ring_pool->unit_size;
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryPoolRelease,
                               WPI_REG_UNLOCKED,
                               &ring_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPoolFromPointerGet,
                           WPI_REG_LOCKED,
                           &buffer_pool,
                           rx_bdd->buffer_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* read only */
   hu_bd_base = rx_bdd->hu_bd_base;
   n_bds = rx_bdd->n_bds;
   out = rx_bdd->out;

   hu_bd = WPI_BDD_ADD(hu_bd_base, out * bd_size);
   /* Return as many buffers as requested, if possible.  */
   for (i = 0; i < output->n_segments; i++)
   {
      WPI_MEM_GET(v_bits, hu_bd->v_bits);
      if (WPI_DPS_BD_VBITS_V_V(v_bits) == 0)
         break;
      /* Allocate a buffer to copy into. */
      segment->data = WPI_CpoolAlloc(buffer_pool);
      segment->pool_handle = buffer_handle;
      /* do the data copy - this is protocol specific */
      WPI_RxBddReport[output->type](hu_bd, segment);
      total_data_size += segment->data_size;
      /* clear the V bit */
      WPI_MEM_SET(hu_bd->v_bits, v_bits & WPI_DPS_BD_VBITS_AAL2_CPS_M);
      /* set for the next iteration. */
      segment = segment->next;
      /* Increment to work on the next BD */
      out++;
      if (out >= n_bds)
      {
         out = 0;
         hu_bd = hu_bd_base;
      }
      else
         hu_bd = WPI_BDD_ADD(hu_bd, bd_size);
   }

   /* set output values */
   output->n_active = i;
   output->data_size = total_data_size;
   rx_bdd->out = out;

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryPoolRelease,
                               WPI_REG_UNLOCKED,
                               &buffer_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* return result */
   WPI_LOG_MT(WPI_DEBUG_LEVEL_5, "stop ReceiveBufferCopy\n");
   return (i == 0) ? WPI_ERROR(WP_ERR_HST_NO_DATA_TO_GET) : WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_RxBdReportAal1
 *
 * Purpose: Collect AAL1-specific data from Rx BDs.
 *
 * Description:
 *     No actions required.
 *
 * Inputs:
 *     vx_bits         Pointer to vx_bits substructure of BD
 *     data_unit       Pointer to returned data unit
 *
 * Outputs:            None -- no action taken
 *
 * Return Value:       void
 *
 * Called by:
 *     WPI_ReceiveBuffer
 *
 ****************************************************************************/
void WPI_RxBdReportAal1(wpi_bd_vx_bits *vx_bits,
                        WP_data_unit *data_unit)
{
   /* nothing needed, it's in the status bits */
}

/*****************************************************************************
 *
 * Function: WPI_MfrSendFrameDirect
 *
 * Purpose: Performs host send function for direct-queued device.
 *
 * Description:
 *     Queue single buffer for device.
 *
 * Inputs:
 *     wpid            WinPath ID
 *     last_tx_bd      Pointer to Tx BD
 *     adjunct_pool    Pointer to the adjunct pool for this device
 *     fbp             Free buffer pool number
 *     data_unit       Pointer to the transmit data unit
 *     next_tx_bd      Pointer to where to store a pointer to the next BD
 *
 * Outputs:
 *     next_tx_bd      Receives address of next BD
 *
 * Return Value:
 *   WP_OK                         Successful call
 *   WP_ERR_HST_INVALID_INPUT      Input is more than one buffer
 *   WP_ERR_DATA_UNIT_INCOMPATIBLE Protocol incompatible with device
 *   WP_ERR_WMM_FIFO_GET           Unable to request a FIFO
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/
WP_status WPI_MfrSendFrameDirect(WP_U32 wpid,
                              wpi_dps_iwbd *last_tx_bd,
                              wpi_res_pool *adjunct_pool,
                              WP_U32 fbp,
                              WP_data_unit *data_unit,
                              WP_U32 x_bits_io,
                              void **next_tx_bd)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 i,j;
   wpi_memory *memory = NULL;
   wpi_dps_iwbd *host_first_bd=NULL;
   wpi_dps_iwbd *tx_iwbd=NULL,*last_bd=NULL,*hu_bd;
   WP_U32 v_bits,x_bits;
   WP_U32 buffer_bus, vbank;
   WPI_PTR dataptr, bd_addr=0,next_bd_addr=0, old_buffer;
   WP_status ret_val = WP_OK;
   WP_U8 *host_offset_buffer, *host_offset_bd;
   WP_data_segment *segment;
   wpi_res_pool_block *pool_block;
   wpi_res_pool *du_pool;
   WP_U32 remainder;
   WP_U32 n_buffs;
   WP_U32 bd_bus;
   wpi_iw_gap_size *gap_size;
   WP_U32 alignment;
   WP_U32 gap_low_bits;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* Check user data input */
   if (data_unit->n_active == 0)
      return WP_OK;

   /* Data units for direct-queued devices must consist of a single buffer. */

   if (data_unit->n_active > data_unit->n_segments ||
       data_unit->segment == NULL || data_unit->n_active==0)
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   segment = data_unit->segment;
   memory = WPI_RegistryMemory(wpid);
   vbank = adjunct_pool->vbank;
   gap_size = WPI_RegistryIwGapSize(wpid);
   alignment = gap_size->alignment; /* power of 2 */
   gap_low_bits = alignment - 1;

   /* Verify that data unit buffer and iwqnode are compatible. */
   pool_block = &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(segment->pool_handle)].pools;
   du_pool = (wpi_res_pool *)
      &pool_block->pool_data[WPI_HANDLE_INDEX(segment->pool_handle)];
   if ((du_pool->vbank != vbank) ||
       (du_pool->item_size != adjunct_pool->item_size))
   {
      ret_val = WPI_ERROR(WP_ERR_DATA_UNIT_INCOMPATIBLE);
      goto WPI_SendFrameDirectQuit;
   }

   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
   buffer_bus = adjunct_pool->bus;
   n_buffs = data_unit->n_active;

   for (i = 0; i < n_buffs; i++)
   {
      ret_val = WPI_WmmFifoGet(wpid, bd_bus, fbp,&bd_addr);
      if (ret_val == WPI_BASIC_ERROR(WP_ERR_WMM_FIFO_GET) && i>0)
      {
         hu_bd =  (wpi_dps_iwbd *)host_first_bd;
         for(j=0;j<(i-1);j++)
         {
            WPI_MEM_GET(bd_addr,hu_bd->next_bd);
            hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr,host_offset_bd);
            ret_val = WPI_WmmFifoPut(wpid,
                                     bd_bus,
                                     fbp,
                                     bd_addr,
                                     NULL,
                                     WP_FALSE);
            if (ret_val != WP_OK)
               goto WPI_SendFrameDirectQuit;
         }
      }
      else if( ret_val !=WP_OK)
         goto WPI_SendFrameDirectQuit;

      if(i==0)
         host_first_bd=(wpi_dps_iwbd *)WPI_HOST_PTR(bd_addr,host_offset_bd);
      else
      {
         WPI_MEM_SET(last_bd->next_bd,bd_addr);
      }
      last_bd =  (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr ,host_offset_bd);
   }
   host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);
   tx_iwbd = host_first_bd;

   for (i = 0; i < n_buffs; i++)
   {
      /* Perform actions that are common to all buffers */
      /* Insert the address of the data into the BD */

      /* Formulate new BD data */
      v_bits = (WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
                WPI_DPS_IWBD_VBITS_FBP_F(fbp) |
                WPI_DPS_BD_VBITS_V_F(1) |
                WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size));

      if (i == 0)
         x_bits = x_bits_io;
      else
         x_bits = 0;

      if( i==n_buffs-1)
         v_bits |=WPI_DPS_BD_VBITS_E_F(1);

      remainder = ((WP_U32)segment->data) & gap_low_bits;
      if ((remainder + segment->displacement) >= alignment)
      {
         ret_val = WPI_ERROR(WP_ERR_HST_IW_DU_INVALID_DISPLACEMENT_OR_OFFSET);
         goto WPI_SendFrameDirectQuit;
      }

      dataptr = WPI_WPI_PTR(segment->data + segment->displacement,
                            host_offset_buffer);

      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, tx_iwbd->data);
      WPI_NpoolFree(du_pool, old_buffer);
      /* For winsim, copy the data to the socket */
      WPI_MEM_HOST_TO_WINSIM(segment->data + segment->displacement,
                             segment->data_size);

      /* Update the BD */
      WPI_MEM_SET(tx_iwbd->data, dataptr);
      WPI_MEM_SET(tx_iwbd->v_bits, v_bits);
      WPI_MEM_SET(tx_iwbd->x_bits,x_bits);

      if( i==n_buffs-1)
      {
         WPI_MEM_SET(tx_iwbd->next_bd,0);
         last_bd = tx_iwbd;
      }
      WPI_MEM_GET(next_bd_addr,tx_iwbd->next_bd);
      tx_iwbd = (wpi_dps_iwbd *) WPI_HOST_PTR(next_bd_addr, host_offset_bd);
      segment = segment->next;
   }

   /* Insert the BD in the appropriate transmit list of this channel */
   bd_addr = WPI_WPI_PTR(host_first_bd,host_offset_bd);
   WPI_MEM_SET(last_tx_bd->next_bd, (WP_U32)bd_addr);

   *next_tx_bd = (void *)last_bd;

  WPI_SendFrameDirectQuit:
   return ret_val;
}

/*****************************************************************************
 *
 * Function: WPI_SendFrameDirect
 *
 * Purpose: Performs host send function for matrix buffer chaining device.
 *          (for FR only. doesnt support buffer chaining as dps doesnt as well)
 *
 * Description:
 *     Queue single buffer for device.
 *
 * Inputs:
 *     wpid                 WinPath ID
 *     last_tx_bd      Pointer to Tx BD
 *     adjunct_pool    Pointer to the adjunct pool for this device
 *     fbp                  Free buffer pool number
 *     data_unit       Pointer to the transmit data unit
 *     next_tx_bd      Pointer to where to store a pointer to the next BD
 *
 * Outputs:
 *     next_tx_bd      Receives address of next BD
 *
 * Return Value:
 *   WP_OK                         Successful call
 *   WP_ERR_HST_INVALID_INPUT      Input is more than one buffer
 *   WP_ERR_DATA_UNIT_INCOMPATIBLE Protocol incompatible with device
 *   WP_ERR_WMM_FIFO_GET           Unable to request a FIFO
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/
WP_status WPI_SendFrameDirect(WP_U32 wpid,
                              wpi_dps_iwbd *last_tx_bd,
                                 wpi_res_pool *adjunct_pool,
                                 WP_U32 fbp,
                                 WP_data_unit *data_unit,
                              WP_U32 x_bits_io,
                              void **next_tx_bd)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   WP_U32 i,j;
   wpi_memory *memory = NULL;
   wpi_dps_iwbd *host_first_bd=NULL;
   wpi_dps_iwbd *tx_iwbd=NULL,*last_bd=NULL,*hu_bd;
   WP_U32 v_bits,x_bits;
   WP_U32 buffer_bus, vbank;
   WPI_PTR dataptr, bd_addr=0,next_bd_addr=0, old_buffer;
   WP_status ret_val = WP_OK , ret_val_inner = WP_OK;
   WP_U8 *host_offset_buffer, *host_offset_bd;
   WP_data_segment *segment;
   wpi_res_pool_block *pool_block;
   wpi_res_pool *du_pool;
   WP_U32 remainder;
   WP_U32 n_buffs;
   WP_U32 bd_bus;
   wpi_iw_gap_size *gap_size;
   WP_U32 alignment;
   WP_U32 gap_low_bits;
   WP_boolean matrix_buffer_chaining_mode_enabled = WP_FALSE;
   WPI_PTR second_bd = 0, first_bd = 0;

   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* Check user data input */
   if (data_unit->n_active == 0)
      return WP_OK;

   /* Data units for direct-queued devices must consist of a single buffer. */

   if (data_unit->n_active > data_unit->n_segments ||
       data_unit->segment == NULL || data_unit->n_active==0)
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   /*Check if matrix buffer chaining enabled*/
   if (WPI_DpsFeatureCheckStatus(wpid,
                                 WPI_FEATUREMODE_GLOBAL,
                                 0,
                                 WPI_DPS_MODULE_MATRIX_BUFF_CHAIN_ENABLE_)== WP_TRUE)
   {
      matrix_buffer_chaining_mode_enabled = WP_TRUE;
   }

   segment = data_unit->segment;
   memory = WPI_RegistryMemory(wpid);
   vbank = adjunct_pool->vbank;
   gap_size = WPI_RegistryIwGapSize(wpid);
   alignment = gap_size->alignment; /* power of 2 */
   gap_low_bits = alignment - 1;

   /* Verify that data unit buffer and iwqnode are compatible. */
   pool_block = &wpi_registry_ptr->pwp_block[WPI_HANDLE_WPID(segment->pool_handle)].pools;
   du_pool = (wpi_res_pool *)
      &pool_block->pool_data[WPI_HANDLE_INDEX(segment->pool_handle)];
   if ((du_pool->vbank != vbank) ||
       (du_pool->item_size != adjunct_pool->item_size))
   {
      ret_val = WPI_ERROR(WP_ERR_DATA_UNIT_INCOMPATIBLE);
      goto WPI_SendFrameDirectQuit;
   }

   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
   buffer_bus = adjunct_pool->bus;
   n_buffs = data_unit->n_active;

   if (n_buffs != 1 && !matrix_buffer_chaining_mode_enabled)
   {
	   ret_val = WPI_ERROR(WP_ERR_DPS_FEATURE_UNAVAILABLE);
	   goto WPI_SendFrameDirectQuit;
   }

   for (i = 0; i < n_buffs; i++)
   {
      ret_val = WPI_WmmFifoGet(wpid, bd_bus, fbp,&bd_addr);
      if (ret_val == WPI_BASIC_ERROR(WP_ERR_WMM_FIFO_GET) && i > 0)
      {
         hu_bd =  (wpi_dps_iwbd *)host_first_bd;
         for(j = 0; j < i; j++)
   {
            WPI_MEM_GET(bd_addr,hu_bd->next_bd);
            hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr,host_offset_bd);
            ret_val_inner = WPI_WmmFifoPut(wpid,
                                     bd_bus,
                                     fbp,
                                     bd_addr,
                                     NULL,
                                     WP_FALSE);
            if (ret_val_inner != WP_OK)
               goto WPI_SendFrameDirectQuit;
         }

         goto WPI_SendFrameDirectQuit;
   }
      else if( ret_val !=WP_OK)
         goto WPI_SendFrameDirectQuit;

      if (i == 1)
   {
    	  /* backup the 2nd bd for matrix calculation */
    	  second_bd = bd_addr;
   }

      if (i == 0)
   {
         host_first_bd = (wpi_dps_iwbd *)WPI_HOST_PTR(bd_addr,host_offset_bd);
         first_bd = bd_addr;
   }
      else
   {
         WPI_MEM_SET(last_bd->next_bd,bd_addr);
   }

      last_bd =  (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr ,host_offset_bd);
   }

   host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);
   tx_iwbd = host_first_bd;

   for (i = 0; i < n_buffs; i++)
   {
      /* Perform actions that are common to all buffers */
      /* Insert the address of the data into the BD */

      /* Formulate new BD data */
	  if (i == 0 && n_buffs != 1)
	  {
		  /* for first BD in matrix , buffer hold full length
		   * and has MTBD set */
		  v_bits = (WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
					WPI_DPS_IWBD_VBITS_FBP_F(fbp) |
					WPI_DPS_BD_VBITS_V_F(1) |
					WPI_DPS_IWBD_VBITS_MTBD_F(1) |
					WPI_DPS_IWBD_VBITS_LEN_F(data_unit->data_size));
	  }
	  else
	  {
		  v_bits = (WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
					WPI_DPS_IWBD_VBITS_FBP_F(fbp) |
					WPI_DPS_BD_VBITS_V_F(1) |
					WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size));
	  }

      if ((i == 0 && n_buffs == 1) || (i == 1 && n_buffs != 1))
         x_bits = x_bits_io;
      else
         x_bits = 0;

      if (i == n_buffs - 1)
         v_bits |= WPI_DPS_BD_VBITS_E_F(1);

      remainder = ((WP_U32)segment->data) & gap_low_bits;
      if ((remainder + segment->displacement) >= alignment)
      {
         ret_val = WPI_ERROR(WP_ERR_HST_IW_DU_INVALID_DISPLACEMENT_OR_OFFSET);
         goto WPI_SendFrameDirectQuit;
      }

      dataptr = WPI_WPI_PTR(segment->data + segment->displacement,
                            host_offset_buffer);

      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, tx_iwbd->data);
      WPI_NpoolFree(du_pool, old_buffer);
      /* For winsim, copy the data to the socket */
      WPI_MEM_HOST_TO_WINSIM(segment->data + segment->displacement,
                             segment->data_size);

      if (i == 0 && n_buffs != 1)
      {
          if(second_bd > first_bd)
          {
             x_bits = x_bits & (~WPI_DPS_MATRIX_BD_SIGN_M);
             x_bits |= WPI_DPS_MATRIX_BD_DIFFPTR_F((second_bd-first_bd) >> 4);
          }
          else
          {
        	  x_bits = x_bits | WPI_DPS_MATRIX_BD_SIGN_M;
        	  x_bits |= WPI_DPS_MATRIX_BD_DIFFPTR_F((first_bd-second_bd)>>4);
          }
          x_bits |= WPI_DPS_MATRIX_BD_BDLENGTH_F(segment->data_size);
      }

      /* Update the BD */
      WPI_MEM_SET(tx_iwbd->data, dataptr);
      WPI_MEM_SET(tx_iwbd->v_bits, v_bits);
      WPI_MEM_SET(tx_iwbd->x_bits,x_bits);

      if (i == n_buffs - 1)
      {
         WPI_MEM_SET(tx_iwbd->next_bd,0);
      }

      if (i == 0)
      {
    	  last_bd = tx_iwbd;
      }

      WPI_MEM_GET(next_bd_addr,tx_iwbd->next_bd);

      if (i == 0 && n_buffs != 1)
      {
    	  WPI_MEM_SET(tx_iwbd->next_bd,0);
      }

      tx_iwbd = (wpi_dps_iwbd *) WPI_HOST_PTR(next_bd_addr, host_offset_bd);
      segment = segment->next;
   }

   /* Insert the BD in the appropriate transmit list of this channel */
   bd_addr = WPI_WPI_PTR(host_first_bd,host_offset_bd);
   WPI_MEM_SET(last_tx_bd->next_bd, (WP_U32)bd_addr);

   *next_tx_bd = (void *)last_bd;

  WPI_SendFrameDirectQuit:
   return ret_val;
}

/*****************************************************************************
 *
 * Function: WPI_ReceiveFrameDirect
 *
 * Purpose: Frame Relay Receive frame from L2 Host Queue,
 *          support Matrix buffer chaining, but not buffer chaining anymore (same as dps).
 *
 * Description:
 *     Collect data from L2 Host Queue.
 *
 * Inputs:
 *     wpid                 WinPath ID
 *     last_rx_bd           Pointer to last Rx BD
 *     adjunct_pool         Pointer to adjunct pool for this device
 *     adjunct_pool_handle  Pool handle for adjunct pool for this device
 *     fbp                  Free buffer pool number
 *     data_unit            Pointer to Rx data unit
 *     hostq_cnt_address    Pointer to host queue counter
 *     next_rx_bd           Pointer to store pointer to next_rx_bd
 *
 * Outputs:
 *     next_rx_bd           Receives address of next_rx_bd
 *
 * Return Value:
 *     list of status values including error values WP_OK, WP_ERR_DEVICE, ...
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_ReceiveFrameDirect(WP_U32 wpid,
								wpi_dps_iwbd *last_rx_bd,
								WP_U32 *buf_cnt,
								wpi_dps_iwbd **first_rx_bd,
								wpi_res_pool *adjunct_pool,
								WP_handle adjunct_pool_handle,
								WP_U32 fbp,
								WP_data_unit *data_unit,
								WP_U32 hostq_cnt_wpid,
								WP_U8 hostq_cnt_bus,
								WPI_PTR hostq_cnt_address,
								void **next_rx_bd)
{
   WP_status ret_val= WP_OK;
   wpi_memory *memory;
   WP_U32 v_bits=0, i, j, bd_bus;
   WP_U32 vbank;
   WPI_PTR next_bd_addr=0, bd_addr, dataptr, scan_bd_addr;
   WP_U8 *host_offset_buffer, *host_offset_bd, *buffer;
   WPI_PTR swap_buffer;
   WP_data_segment *segment;
   wpi_bd_vx_bits vx_bits;
   WP_U32 end;
   wpi_dps_iwbd *hu_bd;
   WP_boolean matrix_buffer_chaining_mode_enabled = WP_FALSE;
   WP_boolean isMatrix = 0;

   WPI_INTERNAL_FUNCTION();
   hu_bd = NULL;
   segment = data_unit->segment;
   if (data_unit->n_segments == 0 || segment == NULL)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   memory = WPI_RegistryMemory(wpid);
   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);

   /* Check existence of next_bd. If there is no next BD,
    * no received data */
   if(last_rx_bd == NULL)
   {
      return WPI_ERROR(WP_ERR_WDDI);
   }

   WPI_MEM_GET(bd_addr, last_rx_bd->next_bd);

   if (bd_addr == 0)
   {
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_HST_NO_DATA_TO_GET !!!\n");
      return WPI_BASIC_ERROR(WP_ERR_HST_NO_DATA_TO_GET);
   }
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "keep scanning for End!!!\n");
      WPI_MEM_GET(scan_bd_addr, last_rx_bd->next_bd);

   /*Check if matrix buffer chaining enabled*/
   if (WPI_DpsFeatureCheckStatus(wpid,
                                 WPI_FEATUREMODE_GLOBAL,
                                 0,
                                 WPI_DPS_MODULE_MATRIX_BUFF_CHAIN_ENABLE_) == WP_TRUE)
   {
      matrix_buffer_chaining_mode_enabled = WP_TRUE;
   }

   end = 0;
   while (!end && scan_bd_addr != 0)
   {
      /* Get BD info */
      hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(scan_bd_addr, host_offset_bd);
      WPI_MEM_GET(v_bits, hu_bd->v_bits);

      if (matrix_buffer_chaining_mode_enabled == WP_TRUE)
      {
    	  if(v_bits & WPI_DPS_MATRIX_BD_VBITS_MTBD_M)
		  {
     		  WPI_MatrixChainingNextBd(hu_bd, &scan_bd_addr, host_offset_bd);
	      }
          else
          {
              WPI_MEM_GET(scan_bd_addr, hu_bd->next_bd);
          }
      }
      else
      {
          WPI_MEM_GET(scan_bd_addr, hu_bd->next_bd);
      }

      end = WPI_DPS_BD_VBITS_E_V(v_bits);
      *buf_cnt += 1;
   }

   if(!end)
   {
	  *buf_cnt = 0;
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_FRAME_WITH_NO_END !!!\n");
      return WPI_ERROR(WP_ERR_FRAME_WITH_NO_END);
   }

   if(*buf_cnt > data_unit->n_segments)
   {
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_HST_DESCR_TOO_SMALL!!!\n");
      return WPI_ERROR(WP_ERR_HST_DESCR_TOO_SMALL);
   }

   /* read the frame to the data_unit */
   /* Get info on buffer pool to which this buffer belongs */
   vbank = adjunct_pool->vbank;
   host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);
   data_unit->data_size = 0;
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);

   if (*first_rx_bd == NULL)
      return WPI_ERROR(WP_ERR_WDDI);


   for (i = 0; i < *buf_cnt; i++)
   {
	   swap_buffer = WPI_NpoolAlloc(adjunct_pool);

	   if (swap_buffer == WPI_PTR_NULL)
	   {
		   segment = data_unit->segment;

		   for (j = 0; j < i; j++)
		   {
			   swap_buffer = WPI_WPI_PTR(segment->data, host_offset_buffer);

			   WPI_NpoolFree(adjunct_pool, swap_buffer);
			   segment = segment->next;
		   }

		   /* Return without receiving data */
		   data_unit->n_active = 0;
		   *buf_cnt = 0;
		   WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_HST_ADJUNCT_POOL_EMPTY !!!\n");
		   return WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
 	  }

	   buffer = (WP_U8 *) WPI_HOST_PTR(swap_buffer, host_offset_buffer);
	   segment->data = buffer;
	   segment = segment->next;
   }

   segment = data_unit->segment;

   WPI_MEM_GET(bd_addr, (*first_rx_bd)->next_bd);

   for (i = 0; i < *buf_cnt; i++)
   {
      swap_buffer = WPI_WPI_PTR(segment->data, host_offset_buffer);

      /* Get BD info */
      hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr, host_offset_bd);
      WPI_MEM_GET(dataptr, hu_bd->data);
      WPI_MEM_GET(v_bits, hu_bd->v_bits);
      WPI_MEM_GET(vx_bits.x_bits, hu_bd->x_bits);
      end = WPI_DPS_BD_VBITS_E_V(v_bits);
      vx_bits.v_bits = v_bits;

      /* Populate the data unit with the BD info. */
      buffer = (WP_U8 *) WPI_HOST_PTR(dataptr, host_offset_buffer);

      if(i==0 && matrix_buffer_chaining_mode_enabled && (v_bits & WPI_DPS_MATRIX_BD_VBITS_MTBD_M))
      {
         segment->data_size = WPI_DPS_MATRIX_BD_BDLENGTH_V(vx_bits.x_bits);
         isMatrix = 1;
      }
      else
         segment->data_size = WPI_DPS_IWBD_VBITS_LEN_V(v_bits);

      data_unit->data_size += segment->data_size;
      segment->data = buffer;
      /* For WinSim, copy the data back from the simulator. */
      WPI_MEM_WINSIM_TO_HOST(buffer,segment->data_size);
      segment->pool_handle = adjunct_pool_handle;

      /* Swap newly allocated buffer into BD and reset BD status */
      v_bits = WPI_DPS_IWBD_VBITS_FBP_F(fbp) | WPI_DPS_IWBD_VBITS_IWBT_F(adjunct_pool->bus);
      WPI_MEM_SET(hu_bd->v_bits, v_bits);
      WPI_MEM_SET(hu_bd->data, swap_buffer);

      if(!end)
      {
          if (i == 0)
          {
         	 /* the first will be the new dummy */
         	 *next_rx_bd = (void *)hu_bd;

			 if(isMatrix)
			 {
			    WPI_MatrixChainingNextBd(hu_bd, &bd_addr, host_offset_bd);
			 }
          }
	      else
	      {
		    WPI_MEM_GET(bd_addr, hu_bd->next_bd);
	      }

         segment = segment->next;
      }
      else /* end */
      {
         /* set x_bits information into the status field of the data_unit */
         WPI_RxBdReport[data_unit->type](&vx_bits, data_unit);

         if (i == 0)
         {
        	 /* will be the new dummy */
        	 *next_rx_bd = (void *)hu_bd;
         }
      }
   }

   /* Release dummy buffer to its free buffer pool */
   bd_addr = WPI_WPI_PTR(*first_rx_bd, host_offset_bd);
   WPI_MEM_GET(next_bd_addr,(*first_rx_bd)->next_bd);
   ret_val = WPI_WmmFifoPut(wpid,
                            bd_bus,
                            fbp,
                            bd_addr,
                            (WP_U32 *)(*first_rx_bd),
                            WP_TRUE);

   if (WP_ERROR_P(ret_val))
   {
	  /* fatal, someone returned bad bds to the fifo */
	  *buf_cnt = 0;
      return WPI_ERROR(WP_ERR_WMM_FIFO_PUT);
   }

   hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(next_bd_addr, host_offset_bd);
   if (isMatrix)
   {
	   /* Release the rest of frame buffers to their free buffer pool */

       WPI_MatrixChainingNextBd(hu_bd, &next_bd_addr, host_offset_bd);

	   while(next_bd_addr)
	   {
		   bd_addr = next_bd_addr;
	       hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(next_bd_addr, host_offset_bd);
	       WPI_MEM_GET(next_bd_addr, hu_bd->next_bd);
	       ret_val = WPI_WmmFifoPut(wpid,
	                                bd_bus,
	                                fbp,
	                                bd_addr,
	                                (WP_U32 *)hu_bd,
	                                WP_TRUE);

	       if (WP_ERROR_P(ret_val))
	          return WPI_ERROR(WP_ERR_WMM_FIFO_PUT);
	   }
   }


   data_unit->n_active = *buf_cnt;
   *buf_cnt = 0;
   *first_rx_bd = *next_rx_bd;

   /* Atomically decrement the Host termination counter */
   ret_val = WPI_WmmAtomicDecrement(hostq_cnt_wpid,
                                    hostq_cnt_bus,
                                    WPI_WMM_SIZE_HALFWORD,
                                    hostq_cnt_address,
                                    NULL);

   return ret_val;
}


/*****************************************************************************
 *
 * Function: WPI_MfrReceiveFrameDirect
 *
 * Purpose: MFR Receive frame from direct-queued device
 *
 * Description:
 *     Collect data from direct-queued device.
 *
 * Inputs:
 *     wpid                 WinPath ID
 *     last_rx_bd           Pointer to last Rx BD
 *     adjunct_pool         Pointer to adjunct pool for this device
 *     adjunct_pool_handle  Pool handle for adjunct pool for this device
 *     fbp                  Free buffer pool number
 *     data_unit            Pointer to Rx data unit
 *     hostq_cnt_address    Pointer to host queue counter
 *     next_rx_bd           Pointer to store pointer to next_rx_bd
 *
 * Outputs:
 *     next_rx_bd           Receives address of next_rx_bd
 *
 * Return Value:
 *     list of status values including error values WP_OK, WP_ERR_DEVICE, ...
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_MfrReceiveFrameDirect(WP_U32 wpid,
                                 wpi_dps_iwbd *last_rx_bd,
                                 WP_U32 *buf_cnt,
                                 wpi_dps_iwbd **first_rx_bd,
                                 wpi_res_pool *adjunct_pool,
                                 WP_handle adjunct_pool_handle,
                                 WP_U32 fbp,
                                 WP_data_unit *data_unit,
                                 WP_U32 hostq_cnt_wpid,
                                 WP_U8 hostq_cnt_bus,
                                 WPI_PTR hostq_cnt_address,
                                 void **next_rx_bd)
{
   WP_status ret_val= WP_OK;
   wpi_memory *memory;
   WP_U32 v_bits=0, i, bd_bus;
   WP_U32 vbank;
   WPI_PTR next_bd_addr=0, bd_addr, dataptr, scan_bd_addr;
   WP_U8 *host_offset_buffer, *host_offset_bd, *buffer;
   WPI_PTR swap_buffer;
   WP_data_segment *segment;
   wpi_bd_vx_bits vx_bits;
   WP_U32 end;
   wpi_dps_iwbd *hu_bd;

   WPI_INTERNAL_FUNCTION();
   hu_bd = NULL;
   segment = data_unit->segment;
   if (data_unit->n_segments == 0 || segment == NULL)
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);

   memory = WPI_RegistryMemory(wpid);
   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);

   /* Check existence of next_bd. If there is no next BD,
    * no received data */
   if(last_rx_bd == NULL)
   {
      return WPI_ERROR(WP_ERR_WDDI);
   }

   WPI_MEM_GET(bd_addr, last_rx_bd->next_bd);

   if (bd_addr == 0)
   {
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_HST_NO_DATA_TO_GET !!!\n");
      return WPI_BASIC_ERROR(WP_ERR_HST_NO_DATA_TO_GET);
   }
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "keep scanning for End!!!\n");
      WPI_MEM_GET(scan_bd_addr, last_rx_bd->next_bd);
   end = 0;
   while (!end && scan_bd_addr != 0)
   {
      /* Get BD info */
      hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(scan_bd_addr, host_offset_bd);
      WPI_MEM_GET(v_bits, hu_bd->v_bits);
      WPI_MEM_GET(scan_bd_addr, hu_bd->next_bd);
      end = WPI_DPS_BD_VBITS_E_V(v_bits);
      *buf_cnt+=1;
   }

   if(!end)
   {
      *next_rx_bd = hu_bd;
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_FRAME_WITH_NO_END !!!\n");
      return WPI_ERROR(WP_ERR_FRAME_WITH_NO_END);
   }

   if(*buf_cnt > data_unit->n_segments)
   {
      WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_HST_DESCR_TOO_SMALL!!!\n");
      return WPI_ERROR(WP_ERR_HST_DESCR_TOO_SMALL);
   }

   /* read the frame to the data_unit */
   /* Get info on buffer pool to which this buffer belongs */
   vbank = adjunct_pool->vbank;
   host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);
   data_unit->data_size = 0;
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);

   if (*first_rx_bd == NULL)
      return WPI_ERROR(WP_ERR_WDDI);

   WPI_MEM_GET(bd_addr, (*first_rx_bd)->next_bd);

   for (i = 0; i < *buf_cnt; i++)
   {
      /* Get a buffer from the qnode's associated adjunct pool */
      swap_buffer = WPI_NpoolAlloc(adjunct_pool);

      if (swap_buffer == WPI_PTR_NULL)
      {
         /* Return without receiving data */
         data_unit->n_active = 0;
         WPI_LOG_MT(WPI_DEBUG_LEVEL_4, "WP_ERR_HST_ADJUNCT_POOL_EMPTY !!!\n");
         return WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
      }

      /* Get BD info */
      hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr, host_offset_bd);
      WPI_MEM_GET(dataptr, hu_bd->data);
      WPI_MEM_GET(v_bits, hu_bd->v_bits);
      WPI_MEM_GET(vx_bits.x_bits, hu_bd->x_bits);
      end = WPI_DPS_BD_VBITS_E_V(v_bits);
      vx_bits.v_bits = v_bits;

      /* Populate the data unit with the BD info. */
      buffer = (WP_U8 *) WPI_HOST_PTR(dataptr, host_offset_buffer);
      segment->data_size = WPI_DPS_IWBD_VBITS_LEN_V(v_bits);
      data_unit->data_size += segment->data_size;
      segment->data = buffer;
      /* For WinSim, copy the data back from the simulator. */
      WPI_MEM_WINSIM_TO_HOST(buffer,segment->data_size);
      segment->pool_handle = adjunct_pool_handle;

         fbp = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);

      /* Swap newly allocated buffer into BD and reset BD status */
      v_bits = WPI_DPS_IWBD_VBITS_FBP_F(fbp) | WPI_DPS_IWBD_VBITS_IWBT_F(adjunct_pool->bus);
      WPI_MEM_SET(hu_bd->v_bits, v_bits);
      WPI_MEM_SET(hu_bd->data, swap_buffer);
      if(!end)
      {
         WPI_MEM_GET(bd_addr, hu_bd->next_bd);
         segment = segment->next;
      }
      else
      {
         /* set x_bits information into the status field of the data_unit */
         WPI_RxBdReport[data_unit->type](&vx_bits, data_unit);
      }
   }

   *next_rx_bd = (void *)hu_bd;

   /* Release dummy buffer to its free buffer pool */
   bd_addr = WPI_WPI_PTR(*first_rx_bd, host_offset_bd);
   WPI_MEM_GET(next_bd_addr,(*first_rx_bd)->next_bd);
   ret_val = WPI_WmmFifoPut(wpid,
                            bd_bus,
                            fbp,
                            bd_addr,
                            (WP_U32 *)(*first_rx_bd),
                            WP_TRUE);
   if (WP_ERROR_P(ret_val))
      return WPI_ERROR(WP_ERR_WMM_FIFO_PUT);

   /* Release the rest of frame buffers to their free buffer pool */
   for (i = 0; i < (*buf_cnt-1); i++)
   {
      bd_addr = next_bd_addr;
      hu_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr, host_offset_bd);
      WPI_MEM_GET(next_bd_addr, hu_bd->next_bd);
      ret_val = WPI_WmmFifoPut(wpid,
                               bd_bus,
                               fbp,
                               bd_addr,
                               (WP_U32 *)hu_bd,
                               WP_TRUE);
      if (WP_ERROR_P(ret_val))
         return WPI_ERROR(WP_ERR_WMM_FIFO_PUT);
   }

   data_unit->n_active = *buf_cnt;
   *buf_cnt = 0;
   *first_rx_bd = *next_rx_bd;

   /* Atomically decrement the Host termination counter */
   ret_val = WPI_WmmAtomicDecrement(hostq_cnt_wpid,
                                    hostq_cnt_bus,
                                    WPI_WMM_SIZE_HALFWORD,
                                    hostq_cnt_address,
                                    NULL);

   return ret_val;
}


/*****************************************************************************
 *
 * Function: WPI_SendFrameIw
 *
 * Purpose: Performs host send function for Iw transmit channels
 *
 * Description:
 *     Queue single buffer for device.
 *
 * Inputs:
 *     wpid            WinPath ID
 *     qnode_ptr       Pointer to the qnode for this device
 *     fbp             Free buffer pool number
 *     data_unit       Pointer to the transmit data unit
 *     pce_connect     if connected to PCE, use the PCE accelerator
 *     system_subtype  IW system subtype
 *     iw_port_index   IW port index
 *
 * Outputs:
 *     next_tx_bd      Receives address of next BD
 *
 * Return Value:
 *   WP_OK                         Successful call
 *   WP_ERR_HST_INVALID_INPUT      Input is more than one buffer
 *   WP_ERR_DATA_UNIT_INCOMPATIBLE Protocol incompatible with device
 *   WP_ERR_WMM_FIFO_GET           Unable to request a FIFO
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/
WP_status WPI_SendFrameIw(WP_U32 wpid,
                          WP_U32 hiit_index,
                          wpi_res_node *qnode_ptr,
                          WP_data_unit *data_unit,
                          WP_U8 pce_connect,
                          WP_U32 system_subtype,
                          WP_U16 iw_port_index)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   wpi_memory *memory;
   wpi_res_iw_tx_channels *res_iw_tx_channels = NULL;
   wpi_dps_iwbd *tx_bd;
   WP_U32 v_bits, x_bits;
   WP_U32 bd_bus=0, buffer_bus=0;
   WPI_PTR dataptr, bd_addr, old_buffer, matrix_dataptr ;
   WP_status ret_val = WP_OK, status=WP_OK, lock_status = WP_OK;
   WP_U8 *host_offset_buffer, *host_offset_bd=0;
   WP_data_segment *segment;
   wpi_res_pool_block *pool_block;
   wpi_res_pool *in_adjunct_pool, *out_adjunct_pool;
   WP_handle in_pool_handle;
   WP_U32 fbp =0 ;
   WP_U32 remainder, priority;
   WP_U32 alignment;
   WP_U32 gap_low_bits;

   WP_boolean matrix_buffer_chaining_mode_enabled = WP_FALSE;
   WPI_PTR prev_tx_bd_addr;
   WP_U32 matrix_v_bits = 0;
   WP_U32 matrix_x_bits = 0;
   WP_U32 matrix_next = 0;
   wpi_dps_iwbd *matrix_tx_bd, *prev_tx_bd = 0;
   WP_U8 diff_sign;
   WP_U32 first_segment_len;
   WP_S32 ii = 0, jj;
   WPI_PTR matrix_bd_addr, next_matrix_bd_addr;
   WP_S32 diff_ptr;
   WP_boolean matrix_flag;
   WP_boolean need_to_release_bd = WP_FALSE;
   wpi_iw_gap_size *gap_size = NULL;

   /* Check user data input */
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (data_unit->n_active == 0)
      return WP_OK;

   /*Check if matrix buffer chaining enabled*/
   if (WPI_DpsFeatureCheckStatus(wpid,
                                 WPI_FEATUREMODE_GLOBAL,
                                 0,
                                 WPI_DPS_MODULE_MATRIX_BUFF_CHAIN_ENABLE_)== WP_TRUE)
      matrix_buffer_chaining_mode_enabled = WP_TRUE;

   /* Data units for direct-queued devices must consist of a single buffer. */
   if (data_unit->n_active > data_unit->n_segments ||
       data_unit->segment == NULL || (data_unit->n_active != 1 && !matrix_buffer_chaining_mode_enabled))
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   segment = data_unit->segment;
   memory = WPI_RegistryMemory(wpid);
   gap_size = WPI_RegistryIwGapSize(wpid);
   alignment = gap_size->alignment; /* power of 2 */
   gap_low_bits = alignment - 1;

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryIwTxChannelsGet,
                           WPI_REG_LOCKED,
                           &res_iw_tx_channels,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (res_iw_tx_channels->max_tx_channels == 0)
      return WPI_ERROR(WP_ERR_WDDI);

   priority = WP_HT_IW_PRIORITY_VALUE (data_unit->control);

   if (priority>3)
   {
      ret_val = WPI_ERROR(WP_ERR_INVALID_PRIORITY);
      goto WPI_SendFrameIwQuit;
   }


   switch ( qnode_ptr->nodetype)
   {
      case WP_QNODE_IWQ:
         in_adjunct_pool = qnode_ptr->type.iwq.adjunct_pool;
         fbp = qnode_ptr->type.iwq.fbp;
         break;

      case WP_QNODE_IW_MDB:
         fbp = qnode_ptr->type.mdb.fbp;
         break;

      case WP_QNODE_IW_HIERARCHICAL:
      {
         WP_handle data_pool_handle = 0;
         wpi_res_iw_hierq *hierq = NULL;
         wpi_res_pool_block *pool_block;

         pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
         hierq = &(qnode_ptr->type.hierq);
         ii = hierq->num_groups - 1;

         while (ii >= 0)
         {
            for (jj = 0; jj < hierq->hier_group[ii].num_of_bd_pools; jj++)
            {
               data_pool_handle = hierq->hier_group[ii].bd_pool[jj]->data_buffer_pool_handle;
               if (data_pool_handle == data_unit->segment[0].pool_handle)
               {
                  fbp = hierq->hier_group[ii].wmm_fbp[jj];
                  ii = -1;
                  break;
               }
               else
               {
                  /* printf("data buffer pool: on group %d, pool %d is not compatible\n", ii, jj);
                     printf("segment handle is %d, incompatible pool handle is %d\n", data_unit->segment[0].pool_handle, data_pool_handle); */

               }
            }
            --ii;
         }

         in_adjunct_pool = &pool_block->pool_data[WPI_HANDLE_INDEX(data_pool_handle)];
         break;
      }

      default :
         return WPI_ERROR(WP_ERR_QNODE);
   }

   /* Verify that data unit buffer and iwqnode are compatible. */
   pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
   out_adjunct_pool = (wpi_res_pool *)
      &pool_block->pool_data[WPI_HANDLE_INDEX(segment->pool_handle)];
   host_offset_buffer = WPI_MemoryOffsetOfVbank(out_adjunct_pool->vbank);

   /* Get a BD from the FBP associated with the iwqnode_handle. */
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
   ret_val = WPI_WmmFifoGet(wpid, bd_bus, fbp, &bd_addr);
   if (ret_val != WP_OK)
      goto WPI_SendFrameIwQuit;
   if (bd_addr == WPI_PTR_NULL)
   {
      ret_val = WPI_ERROR(WP_ERR_WMM_FIFO_GET);
      goto WPI_SendFrameIwQuit;
   }
   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
   tx_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(bd_addr, host_offset_bd);

   /* save bd address for case of more than 1 BD */
   prev_tx_bd_addr = bd_addr;

   if(qnode_ptr->nodetype == WP_QNODE_IW_MDB)
   {
      /* read vbits to determine in_adjunct_pool */
      WPI_MEM_GET(v_bits, tx_bd->v_bits);
      WPI_GetAdjPoolFromBd(v_bits, qnode_ptr, &in_adjunct_pool, &in_pool_handle);
   }
   else if ((out_adjunct_pool->vbank != in_adjunct_pool->vbank) ||
       (out_adjunct_pool->item_size != in_adjunct_pool->item_size))
   {
      ret_val = WPI_ERROR(WP_ERR_DATA_UNIT_INCOMPATIBLE);
      goto WPI_SendFrameIwQuit;
   }
   buffer_bus = out_adjunct_pool->bus;
   if (in_adjunct_pool->offset < gap_size->min_gap)
   {
      ret_val = WPI_ERROR(WP_ERR_IW_BUFFER_INSUFFICIENT_GAP_SIZE);
      goto WPI_SendFrameIwQuit;
   }

   /* Formulate new BD data */
#if _WP_WMM_FIFO_BD_NEW_STRUCT_
   v_bits = (WPI_DPS_IWBD_VBITS_DCRC_F(WP_HT_IW_CRC_VALUE(
                                          data_unit->control)) |
             WPI_DPS_IWBD_VBITS_VA_DEST_F(
                WP_HT_IW_B_VA_DEST_TYPE_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_B_MODE_F(
                WP_HT_IW_B_MODE_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_CHECKSUM_F(
                WP_HT_IW_IP_CHECKSUM_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_L4CR_F(
                WP_HT_IW_NAT_L4_CHECKSUM_RECALC_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_LEN_F(data_unit->data_size));
#else /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ */
   v_bits = (WPI_DPS_IWBD_VBITS_DCRC_F(WP_HT_IW_CRC_VALUE(
                                          data_unit->control)) |
             WPI_DPS_IWBD_VBITS_VA_DEST_F(
                WP_HT_IW_B_VA_DEST_TYPE_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_B_MODE_F(
                WP_HT_IW_B_MODE_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_CHECKSUM_F(
                WP_HT_IW_IP_CHECKSUM_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_L4CR_F(
                WP_HT_IW_NAT_L4_CHECKSUM_RECALC_VALUE(data_unit->control)) |
             WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
             WPI_DPS_IWBD_VBITS_FBP_F(fbp) |
             WPI_DPS_IWBD_VBITS_LEN_F(data_unit->data_size));
#endif /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ - else */
   if(pce_connect == WP_ENABLE)
      v_bits = v_bits | WPI_NCM_FIELD(WPI_DPS_IWBD_VBITS_CONNECT_TO_PCE, 1);
   x_bits = WPI_DPS_IWBD_XBITS_HIIT_OFFSET_F(hiit_index) |
      WP_HT_IW_FSP_R_PROTOCOL_VALUE (data_unit->control);


   /* Host bridge port extension:
      In normal host send mode in bridging, the application may set the x_bits
      using the macro WP_HT_IW_FSP_VLAN_BPORT.
      If it doesn't WDDI should write the HIIT index in those bits.
   */
   if (WP_HT_IW_FSP_VLAN_BPORT_VALUE(data_unit->control) == 0 &&
       WP_HT_IW_B_MODE_VALUE(data_unit->control) == WP_HT_IW_B_NORMAL_MODE &&
       (system_subtype == WP_IW_VLAN_AWARE_BRIDGING_MODE ||
        system_subtype == WP_IW_TRANS_BRIDGING_MODE))
   {
      x_bits |= WP_HT_IW_FSP_VLAN_BPORT_FIELD(iw_port_index);
   }

   remainder = ((WP_U32)segment->data) & gap_low_bits;
   if ((remainder + segment->displacement) >= alignment)
   {
      return WPI_ERROR(WP_ERR_HST_IW_DU_INVALID_DISPLACEMENT_OR_OFFSET);
   }

   dataptr = WPI_WPI_PTR(segment->data + segment->displacement,
                         host_offset_buffer);
   
   /* Put BD's old buffer into iwqnode's buffer pool */
   WPI_MEM_GET(old_buffer, tx_bd->data);
   if((old_buffer < 0x30000000) ||(old_buffer > 0x3fffffff))//bus PACKET : 0x30000000 - 0x3fffffff
   {
     //printf("old_buffer address is illegal = 0x%x tx_bd = 0x%x\n",old_buffer,tx_bd);
     struct tm *p;
     time_t timep;
     time(&timep);
     p= localtime(&timep);
     
     printf("[%4d-%02d-%02d_%2.2d:%2.2d:%2.2d]old_buffer address is illegal = 0x%x tx_bd = 0x%x\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,old_buffer,tx_bd);

      WP_U32 i_bd = 0;
     wpi_dps_iwbd *tx_bd_tmp;
     tx_bd_tmp = tx_bd - 0x100;
     for(i_bd = 0;i_bd <0x100*2;i_bd++)
       {
	 //printf("tx_bd_address = 0x%x tx_bd->data = 0x%x\n", tx_bd_tmp,tx_bd_tmp->data);
	 printf("tx_bd_address = 0x%x tx_bd->data = 0x%x tx_bd_tmp->next_bd = 0x%x,tx_bd_tmp->v_bits=0x%x,tx_bd_tmp->x_bits=0x%x\n", tx_bd_tmp,tx_bd_tmp->data,tx_bd_tmp->next_bd,tx_bd_tmp->v_bits,tx_bd_tmp->x_bits);
         tx_bd_tmp++;
       }
   }
   else
   {
      WPI_NpoolFree(in_adjunct_pool, old_buffer);
   }
   

   /* For winsim, copy the data to the socket */
   WPI_MEM_HOST_TO_WINSIM(segment->data + segment->displacement,
                          segment->data_size);

   WPI_LOG (WPI_DEBUG_LEVEL_9, (wpi_log, "SendFrameIw: v_bits = 0x%08x, x_bits=0x%08x\n",
                                v_bits,
                                x_bits));

   /* fetch the first segment len */
   first_segment_len = segment->data_size;

   /* the first BD was fetched previos,
      this is the reason the for loop starts from 1 */
   for(ii = 1; ii < data_unit->n_active; ii++)
   {

      segment = segment->next;

      ret_val = WPI_WmmFifoGet(wpid, bd_bus, fbp, &matrix_bd_addr);
      if (ret_val != WP_OK)
      {
         need_to_release_bd = WP_TRUE;
         goto WPI_SendFrameIwQuit;
      }
      if(matrix_bd_addr == WPI_PTR_NULL)
      {
         need_to_release_bd = WP_TRUE;
         ret_val = WPI_ERROR(WP_ERR_WMM_FIFO_GET);
         goto WPI_SendFrameIwQuit;
      }

      host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
      matrix_tx_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(matrix_bd_addr, host_offset_bd);

#if _WP_WMM_FIFO_BD_NEW_STRUCT_

      if(ii == 1)
      {
         /* write the x_bits to the second BD */
         matrix_x_bits = x_bits;

         diff_ptr = prev_tx_bd_addr - matrix_bd_addr;
         if(diff_ptr < 0)
         {
            diff_sign = 0;
            diff_ptr = matrix_bd_addr - prev_tx_bd_addr;
         }
         else
         {
            diff_sign = 1;
         }
         /* prepare the x_bits and v_bits for the first BD */
         /* the x_bits won't be overide because we enter here only once */
         x_bits = WPI_DPS_MATRIX_BD_DIFFPTR_F(diff_ptr / 16) |
                  WPI_DPS_MATRIX_BD_SIGN_F(diff_sign) |
                  WPI_DPS_MATRIX_BD_BDLENGTH_F(first_segment_len);

         v_bits |= WPI_DPS_MATRIX_BD_VBITS_MTBD_M;
      }
      else
      {
         matrix_x_bits = 0;
      }

      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, matrix_tx_bd->data);
      WPI_NpoolFree(in_adjunct_pool, old_buffer);

      matrix_next = 0;
      matrix_dataptr = WPI_WPI_PTR(segment->data + segment->displacement,
                                   host_offset_buffer);
      matrix_v_bits = WPI_DPS_BD_VBITS_V_F(1);
      matrix_v_bits |= WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size);


      /* the last BD in chain, the nextBd field should be cleared */
      if(ii == (data_unit->n_active - 1))
      {
         /* mark BD as last BD in chain*/
         matrix_v_bits |= WPI_DPS_BD_VBITS_E_F(1);
      }

      /* update the prev BD.next with the ptr to the current bd */
      if(ii > 1)
      {
         WPI_MEM_SET(prev_tx_bd->next_bd, matrix_bd_addr);
      }

      WPI_BdIwInit(wpid,
                   matrix_tx_bd,
                   matrix_next,
                   matrix_dataptr,
                   &matrix_v_bits,
                   &matrix_x_bits,
                   bd_bus,
                   buffer_bus,
                   fbp);


#else /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ */

      /* to aviod errors */
      matrix_next = 0;
      matrix_x_bits = 0;

      /* calculate the diffptr from the first BD*/
      if(ii == 1)
      {
         /* write the x_bits to the second BD */
         WPI_MEM_SET(matrix_tx_bd->x_bits, x_bits);

         diff_ptr = prev_tx_bd_addr - matrix_bd_addr;
         if(diff_ptr < 0)
         {
            diff_sign = 0;
            diff_ptr = matrix_bd_addr - prev_tx_bd_addr;
         }
         else
         {
            diff_sign = 1;
         }
         /* prepare the x_bits and v_bits for the first BD */
         /* the x_bits won't be overide because we enter here only once */
         x_bits = WPI_DPS_MATRIX_BD_DIFFPTR_F(diff_ptr / 16) |
                  WPI_DPS_MATRIX_BD_SIGN_F(diff_sign) |
                  WPI_DPS_MATRIX_BD_BDLENGTH_F(first_segment_len);


         /* set the matrix ptr within the first BD
            for error case, in that case, the matrix_ptr is being
            fetched from he IWBD in the ext memory so this field
            should be update, tx_bd is the firs BDin the linked list */
         WPI_MEM_SET(tx_bd->x_bits, x_bits);

         v_bits |= WPI_DPS_MATRIX_BD_VBITS_MTBD_M;
      }

      /* Put BD's old buffer into iwqnode's buffer pool */
      WPI_MEM_GET(old_buffer, matrix_tx_bd->data);
      WPI_NpoolFree(in_adjunct_pool, old_buffer);

      matrix_dataptr = WPI_WPI_PTR(segment->data + segment->displacement,
                                   host_offset_buffer);

      WPI_MEM_SET(matrix_tx_bd->data, matrix_dataptr);

      /* update the prev BD.next with the ptr to the current bd */
      if(ii > 1)
      {
         WPI_MEM_SET(prev_tx_bd->next_bd, matrix_bd_addr);
      }

      matrix_v_bits = WPI_DPS_BD_VBITS_V_F(1);
      matrix_v_bits |= WPI_DPS_IWBD_VBITS_LEN_F(segment->data_size) |
         WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
         WPI_DPS_IWBD_VBITS_FBP_F(fbp);


      /* the last BD in chain, the nextBd field should be cleared */
      if(ii == (data_unit->n_active - 1))
      {
         /* mark BD as last BD in chain*/
         matrix_v_bits |= WPI_DPS_BD_VBITS_E_F(1);

         WPI_MEM_SET(matrix_tx_bd->next_bd, 0);
      }

      WPI_MEM_SET(matrix_tx_bd->v_bits, matrix_v_bits);

      /* if i == 1, the x_bits were written before
         and must not be override*/
      if(ii != 1)
         WPI_MEM_SET(matrix_tx_bd->x_bits, 0);

#endif /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ - else */



      prev_tx_bd_addr = matrix_bd_addr;
      prev_tx_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(prev_tx_bd_addr, host_offset_bd);
   }

   /* Update the First BD */
#if _WP_WMM_FIFO_BD_NEW_STRUCT_
      WPI_BdIwInit(wpid,
                   tx_bd,
                   0,
                   dataptr,
                   &v_bits,
                   &x_bits,
                   bd_bus,
                   buffer_bus,
                   fbp);
#else /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ */
   WPI_MEM_SET(tx_bd->next_bd, 0);
   WPI_MEM_SET(tx_bd->data, dataptr);
   WPI_MEM_SET(tx_bd->v_bits, v_bits);
   WPI_MEM_SET(tx_bd->x_bits, x_bits);
#endif /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ - else  */

   ret_val = WPI_WmmFifoPut(wpid, bd_bus,
                            res_iw_tx_channels->priority_fifo_number
                            + priority,
                            bd_addr, (WP_U32 *)tx_bd, WP_FALSE);

   if (WP_ERROR_P(ret_val))
      goto WPI_SendFrameIwQuit;

   /* set the counter fifo and initiate thread */
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_INTERNAL);
   ret_val = WPI_WmmFifoPut(wpid, bd_bus,
                            res_iw_tx_channels->counter_fifo_number,
                            res_iw_tx_channels->dummy,
                            NULL, WP_FALSE);

  WPI_SendFrameIwQuit:

   /* an error occoured and all BD that were allocated
      need to be freed */
   if (need_to_release_bd == WP_TRUE)
   {
      matrix_bd_addr = bd_addr;
      matrix_flag = WP_TRUE;
      matrix_tx_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(matrix_bd_addr, host_offset_bd);

      /* release BDs that were allready fetched */
      /* if ii = 0, there is no need to release BD */
      for(jj = 0 ; jj < ii ; jj++)
      {
         /* fetch the next BD in the linked list */
         WPI_TakeNextBdAddress(matrix_tx_bd,
                               matrix_flag,
                               &next_matrix_bd_addr,
                               host_offset_bd);

         matrix_flag = WP_FALSE;

#if _WP_WMM_FIFO_BD_NEW_STRUCT_

         matrix_next = 0;

         matrix_v_bits = matrix_tx_bd->v_bits;

         matrix_v_bits &= ~WPI_DPS_BD_VBITS_V_F(1);
         matrix_v_bits &=  ~WPI_DPS_IWBD_VBITS_LEN_M;
         matrix_v_bits &= ~WPI_DPS_MATRIX_BD_VBITS_MTBD_M;

         WPI_BdIwInit(wpid,
                      matrix_tx_bd,
                      matrix_next,
                      matrix_tx_bd->data,
                      &matrix_v_bits,
                      0,
                      bd_bus,
                      buffer_bus,
                      fbp);

#else /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ */

         WPI_MEM_SET(matrix_tx_bd->next_bd, 0);

         matrix_v_bits &= ~WPI_DPS_BD_VBITS_V_F(1);
         matrix_v_bits &=  ~WPI_DPS_IWBD_VBITS_LEN_M;
         matrix_v_bits |=  WPI_DPS_IWBD_VBITS_IWBT_F(buffer_bus) |
            WPI_DPS_IWBD_VBITS_FBP_F(fbp);

         matrix_v_bits &= ~WPI_DPS_MATRIX_BD_VBITS_MTBD_M;
         WPI_MEM_SET(matrix_tx_bd->v_bits , matrix_v_bits);


#endif /* #if _WP_WMM_FIFO_BD_NEW_STRUCT_ - else */

         /* put back the BD to the FPB */
         status = WPI_WmmFifoPut(wpid, bd_bus, fbp,
                                 matrix_bd_addr,
                                 (WP_U32 *)matrix_tx_bd,
                                 WP_TRUE);
         if(status != WP_OK)
         {
            ret_val = WPI_ERROR(WP_ERR_WDDI);
         }
         matrix_bd_addr = next_matrix_bd_addr;
         matrix_tx_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(matrix_bd_addr, host_offset_bd);

      }
   }

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryIwTxChannelsRelease,
                               WPI_REG_UNLOCKED,
                               &res_iw_tx_channels);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   return ret_val;
}

/*****************************************************************************
 *
 * Function: ReissueFtdEnable
 *
 * Purpose: Issue FTD enable after autodeactivation
 *
 * Description:
 *
 * Inputs:
 *     cptr            Pointer to channel structure
 *     handle          Handle associated with channel
 *
 * Outputs:

 *
 * Return Value:       void
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/

WP_status WPI_ReissueFtdEnable(wpi_res_channel *cptr,
                               WP_handle handle)
{
   wpi_memory *memory;
   WP_U32 wpid = WPI_HANDLE_WPID(handle);
   WP_U32 ftd;

   memory = WPI_RegistryMemory(wpid);

   ftd = cptr->du.pkt.ftd;

   return (WPI_IssueFtdTxEnable(wpid, memory, cptr, ftd));
}

/*****************************************************************************
 *
 * Function: WPI_ReissueCtdEnable
 *
 * Purpose: Issue CTD enable after autodeactivation
 *
 * Description:
 *
 * Inputs:
 *     cptr            Pointer to channel structure
 *     handle          Handle associated with channel
 *
 * Outputs:

 *
 * Return Value:       void
 *
 * Called by:
 *     WP_HostSend
 *
 ****************************************************************************/
WP_status WPI_ReissueCtdEnable(wpi_res_channel *cptr,
                               WP_handle handle)
{
   WPI_PTR ctd;
   wpi_memory *memory;
   WP_U32 wpid = WPI_HANDLE_WPID(handle);
   WP_U16 ch_num = WPI_HANDLE_INDEX(handle);

   /* Autoactivated CTD is always first in linked-list */
   ctd = cptr->du.atm.ctd->ctd;

   memory = WPI_RegistryMemory(wpid);

   return (WPI_IssueCtdTxEnable(cptr, memory, ch_num, wpid, ctd));
}

/*****************************************************************************
 *
 * Function: WPI_IndRingDelete
 *
 * Purpose: Delete an indirect BD ring, returning it to its pool
 *
 * Description:
 *     How does this function work. Is there something tricky done?
 *     Any global state that is read.
 *     Any global state that is written.
 *
 * Inputs:
 *     bd_block_ptr    Pointer to BD control block
 *     ch_direction    WP_CH_RX/WP_CH_TX
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *   WPI_OK            Successful
 *
 * Called by:
 *     WPI_AtmChannelDelete
 *     WPI_Aal2RxBdRingDelete
 *     WP_CidGroupDelete
 *     WP_CidDelete
 *     WPI_EnetChannelDelete
 *     WPI_HdlcChannelDelete
 *     WPI_TransChannelDelete
 ****************************************************************************/
WP_status WPI_IndRingDelete(void *bd_block_ptr,
                            WP_mode ch_direction)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_pool *buffer_pool;
   wpi_res_pool *ring_pool;
   wpi_tx_bd_block *tx_bd = 0;
   wpi_rx_bd_block *rx_bd = 0;
   wpi_dps_bd *hu_bd, *hu_bd_base;
   WPI_PTR data, bd_base;
   WP_U8 *hu_offset;
   WP_U32 i;
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (ch_direction == WP_CH_RX)
   {
      rx_bd = *((wpi_rx_bd_block **) bd_block_ptr);
      hu_bd = hu_bd_base = rx_bd->host_bd_base;
      buffer_pool = rx_bd->buffer_pool;
      ring_pool = rx_bd->ring_pool;
      hu_offset = ring_pool->hu_offset;
      for (i = 0; i < rx_bd->n_bds; i++)
      {
         WPI_MEM_GET(data, hu_bd->data);
         WPI_NpoolFree(buffer_pool, data);
         hu_bd++;
      }
      bd_base = WPI_WPI_PTR(hu_bd_base, hu_offset);
      WPI_NpoolFree(ring_pool, bd_base);
      WPI_WDDI_FREE(rx_bd);
   }
   else
   {
      wpi_tx_saved_data *data_saved_base;
      wpi_res_pool_block *pool_block;

      WP_U32 open, n_bds, out, limit, wpid;
      WPI_PTR free_data;
      WP_handle buffer_pool_handle;
      volatile wpi_dps_bd *host_bd;

      tx_bd = *((wpi_tx_bd_block **) bd_block_ptr);
      hu_bd = hu_bd_base = tx_bd->host_bd_base;

      open = tx_bd->open;
      n_bds = tx_bd->n_bds;
      out = tx_bd->out;

      host_bd = hu_bd_base + out;

      limit = n_bds - open;
      data_saved_base = tx_bd->saved_data;
      for (i = 0; i < limit; i++)
      {
         WPI_MEM_GET(data, host_bd->data);
         if (data == WPI_PTR_NULL)
            continue;
         buffer_pool_handle = data_saved_base[out].handle;
         /* grab the host/virtual address */
         wpid = WPI_HANDLE_WPID(buffer_pool_handle);
         pool_block = &wpi_registry_ptr->pwp_block[wpid].pools;
         buffer_pool =
            &pool_block->pool_data[WPI_HANDLE_INDEX(buffer_pool_handle)];
         free_data = (data - data_saved_base[out].displacement);
         WPI_MEM_SET(host_bd->data, WPI_PTR_NULL);
         WPI_NpoolFree(buffer_pool, free_data);
         WPI_CIRC_INCR(out, n_bds, host_bd, hu_bd_base);

      }

      ring_pool = tx_bd->ring_pool;
      hu_offset = ring_pool->hu_offset;
      bd_base = WPI_WPI_PTR(hu_bd_base, hu_offset);
      WPI_NpoolFree(ring_pool, bd_base);
      WPI_WDDI_FREE(tx_bd);

   }
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_DirectRingDelete
 *
 * Purpose: Delete a direct BD ring, returning it to its pool
 *
 * Description:
 *
 * Inputs:
 *     bd_block_ptr    Pointer to BD control block
 *     ch_direction    WP_CH_RX/WP_CH_TX
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *     WP_OK           Successful
 *
 * Called by:
 *     WPI_Aal2RxBdRingDelete
 *     WP_CidGroupDelete
 ****************************************************************************/
WP_status WPI_DirectRingDelete(void *bd_block_ptr,
                               WP_mode ch_direction)
{
   wpi_res_pool *ring_pool;
   wpi_tx_bdd_block *tx_bdd = 0;
   wpi_rx_bdd_block *rx_bdd = 0;
   wpi_dps_bdd *hu_bd_base;
   WPI_PTR bd_base;
   WP_U8 *hu_offset;

   if (ch_direction == WP_CH_RX)
   {
      rx_bdd = *((wpi_rx_bdd_block **) bd_block_ptr);
      hu_bd_base = rx_bdd->hu_bd_base;
      ring_pool = rx_bdd->ring_pool;
      hu_offset = ring_pool->hu_offset;
      bd_base = WPI_WPI_PTR(hu_bd_base, hu_offset);
      WPI_NpoolFree(ring_pool, bd_base);
      WPI_WDDI_FREE(rx_bdd);
   }
   else
   {
      tx_bdd = *((wpi_tx_bdd_block **) bd_block_ptr);
      hu_bd_base = tx_bdd->hu_bd_base;
      ring_pool = tx_bdd->ring_pool;
      hu_offset = ring_pool->hu_offset;
      bd_base = WPI_WPI_PTR(hu_bd_base, hu_offset);
      WPI_NpoolFree(ring_pool, bd_base);
      WPI_WDDI_FREE(tx_bdd);
   }
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_ApiSerialName
 *
 * Purpose:  convert serial index (wpi_sa.h) to API serial name
 *
 * Description:
 *
 * Inputs:
 *           serial_index   SA index of the serial
 *
 * Outputs:
 *           serial_name    API name for the serial
 * Return Value:
 *           WP_TRUE        if the index can be converted to a name
 *           WP_FALSE       otherwise
 *
 * Called by:
 *
 ****************************************************************************/
static
WP_boolean WPI_ApiSerialName(WP_U32 serial_index, WP_U32 *serial_name)
{
   if(serial_index < WPI_HW_SERIAL_TGU1)
      *serial_name = serial_index;
   else
      return WP_FALSE;

   return WP_TRUE;
}

/*****************************************************************************
 *
 * Function: WPI_GetRxTag
 *
 * Purpose: Get tag of the device/channel from phy number, serial ID and channel index.
 *
 * Description:
 *
 * Inputs:
 *     serial_id
 *     phy_number
 *     channel_index
 *     wpid
 *     valid_fields_mask
 *     returned_tag
 *
 * Outputs:
 *
 * Return Value:       WP_status
 *     WP_OK           Successful
 *
 * Called by:
 *     WPI_HostReceiveIw
 ****************************************************************************/
static WP_status WPI_GetRxTag(WP_U32 serial_id,
                              WP_U32 phy_number,
                              WP_U32 channel_index,
                              WP_U32 wpid,
                              WP_ht_iw_valid_fields_mask *valid_fields_mask,
                              WP_tag *returned_tag)
{
   wpi_res_device *device_res;
   WP_U32 device_type = WP_DEVICE_INVALID;
   WP_U32 port_index;
   WP_U32 dev_index = 0;
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_tag returned_tag_hspos;

   WPI_RETURN_IF_ERROR(status,
                       WPI_ApiPortIndex(wpid, serial_id, &port_index));

   switch (port_index)
   {
      case WP_PORT_UPI1:
      case WP_PORT_UPI2:
      case WP_PORT_UPI3:
      case WP_PORT_UPI4:
      {
         WP_U8 uses_emphy;
         wpi_res_upi *res_upi = NULL;
         wpi_res_port *res_port = NULL;

         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryPortGet,
                                 WPI_REG_LOCKED,
                                 &res_port,
                                 wpid,
                                 port_index);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         res_upi = (wpi_res_upi *)res_port;

         uses_emphy = res_upi->uses_emphy;
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryPortRelease,
                                     WPI_REG_UNLOCKED,
                                     &res_port);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);

         if (!uses_emphy)
         {
            dev_index = WPI_INDEX_UPI_PHY(port_index, phy_number);
         }
         else
         {
            *returned_tag = 0;
            return WP_OK;
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
         WP_U32 handle_index;
         WP_U32 pg = pg_pkt[port_index];
         WP_U32 group_offset;

         lock_status = WPI_RegistryChannelBlockGroupOffsetGet(wpid,
                                                              pg,
                                                              WPI_handle_rx_channel,
                                                              &group_offset);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);

         handle_index = group_offset + WPI_HwEnetRxChannelOffsetInGroupGet(phy_number, port_index);
         lock_status = WPI_RegistryChannelTagGet(wpid,
                                                 handle_index,
                                                 WPI_handle_rx_channel,
                                                 returned_tag);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         *valid_fields_mask |= WP_HT_RX_TAG;
         return WP_OK;
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
      case WP_PORT_IW_HOST:
      {
         *returned_tag = 0;
         return WP_OK;
      }
      break;

      default:
         break;
   }

   /* dev_index passed in here is the index of the device entry in the
    * registry */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryDeviceGet,
                           WPI_REG_LOCKED,
                           &device_res,
                           wpid,
                           dev_index);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   device_type = device_res->devtype;
   returned_tag_hspos = ((wpi_res_device_hspi *)device_res)->rx_tag;
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryDeviceRelease,
                               WPI_REG_UNLOCKED,
                               (wpi_res_device **)&device_res);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   switch(device_type)
   {
      /* Atm Gbond device*/
      case WP_DEVICE_GBOND:
         /* ATM device (UTOPIA PHY) */
      case WP_DEVICE_ATM:
      {
         WP_U32 handle_index;
         WP_U32 group_offset;
         lock_status = WPI_RegistryChannelBlockGroupOffsetGet(wpid,
                                                              WPI_pg_atm_shared,
                                                              WPI_handle_rx_channel,
                                                              &group_offset);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         /* Convert the ch_num to the handle index */
         handle_index = channel_index + group_offset;
         lock_status = WPI_RegistryChannelTagGet(wpid,
                                                 handle_index,
                                                 WPI_handle_rx_channel,
                                                 returned_tag);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         *valid_fields_mask |= WP_HT_RX_TAG;
      }
      break;
      /* HDLC device (POS PHY) */
      case WP_DEVICE_HDLC:
      {
         WP_U32 handle_index;
         WP_U32 pg;
         WP_U32 group_offset;

         switch(port_index)
         {
            case WP_PORT_UPI1:
               pg = WPI_pg_pkt_upi1;
               break;
            case WP_PORT_UPI2:
               pg = WPI_pg_pkt_upi2;
               break;
            case WP_PORT_UPI3:
               pg = WPI_pg_pkt_upi3;
               break;
            case WP_PORT_UPI4:
               pg = WPI_pg_pkt_upi4;
               break;
            default:
               pg = WPI_pg_pkt_tdi;
         }
         lock_status = WPI_RegistryChannelBlockGroupOffsetGet(wpid,
                                                              pg,
                                                              WPI_handle_rx_channel,
                                                              &group_offset);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         /* Convert the ch_num to the handle index */
         handle_index = phy_number + group_offset;
         lock_status = WPI_RegistryChannelTagGet(wpid,
                                                 handle_index,
                                                 WPI_handle_rx_channel,
                                                 returned_tag);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         *valid_fields_mask |= WP_HT_RX_TAG;
      }
      break;
      /* Frame Realy device */
      case WP_DEVICE_FR:
      {
         WP_U32 handle_index;
         WP_U32 pg;
         WP_U32 group_offset;
         switch(port_index)
         {
            case WP_PORT_UPI1:
               pg = WPI_pg_pkt_upi1;
               break;
            case WP_PORT_UPI2:
               pg = WPI_pg_pkt_upi2;
               break;
            case WP_PORT_UPI3:
               pg = WPI_pg_pkt_upi3;
               break;
            case WP_PORT_UPI4:
               pg = WPI_pg_pkt_upi4;
               break;
            default:
               pg = WPI_pg_pkt_tdi;
         }
         lock_status = WPI_RegistryChannelBlockGroupOffsetGet(wpid,
                                                              pg,
                                                              WPI_handle_rx_channel,
                                                              &group_offset);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         /* Convert the ch_num to the handle index */
         handle_index = channel_index + group_offset;
         lock_status = WPI_RegistryChannelTagGet(wpid,
                                                 handle_index,
                                                 WPI_handle_rx_channel,
                                                 returned_tag);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         *valid_fields_mask |= WP_HT_RX_TAG;
      }
      break;
      /* Transparent mode TDM device */
      case WP_DEVICE_TRANS:
         *returned_tag = 0;
         break;
         /* High speed POS device */
      case WP_DEVICE_HSPOS:
      {
         *returned_tag = returned_tag_hspos;
         *valid_fields_mask |= WP_HT_RX_TAG;
      }
      break;
      /* Link supporting an IMA group */
      case WP_DEVICE_IMA:
         *returned_tag = 0;
         break;
         /* MultiLink PPP bundle (virtual device) */
      case WP_DEVICE_MLPPP:
         *returned_tag = 0;
         break;
         /* Multi Channel HDLC virtual device */
      case WP_DEVICE_MC_HDLC:
         *returned_tag = 0;
         break;
         /* Multi Channel FR virtual device */
      case WP_DEVICE_MC_FR:
         *returned_tag = 0;
         break;
         /* Multilink FR virtual device */
      case WP_DEVICE_MFR_FR:
         *returned_tag = 0;
         break;
         /* An IMA group */
      case WP_DEVICE_IMA_GROUP:
         break;
      case WP_DEVICE_GENET:
         *returned_tag = 0;
         break;
      default:
         *returned_tag = 0;
         break;
   }
   return WP_OK;
}


/*****************************************************************************
 *
 * Function: WPI_GetAdjPoolFromBd
 *
 * Purpose: Get the correct Adjunct Pool for a Bd and Qnode
 *
 * Description:
 *
 * Inputs:
 *     v_bits          v_bits used in the Bd
 *     iwqnode         Qnode
 *
 * Outputs:
 *     adjunct_pool    Adjunct pool
 *
 * Return Value:                  WP_status
 *     WP_OK                      Successful
 *
 * Called by:
 *     WPI_SwQueueMatrixIwHostReceive, WPI_HostReceiveIw
 *
 ****************************************************************************/
WP_status WPI_GetAdjPoolFromBd(WP_U32 v_bits,
                               wpi_res_node *iwq,
                               wpi_res_pool **pool_pointer,
                               WP_handle *pool_handle)
{
   WP_status status = WP_OK;

   if (iwq->nodetype == WP_QNODE_IW_MDB)
   {
      WP_U32 ii,found_pool;

      found_pool = 0;
      for (ii=0; ii < iwq->type.mdb.num_of_adj_pools; ii++)
      {
         if (WPI_DPS_IWBD_VBITS_IWBT_V(v_bits) == iwq->type.mdb.adjunct_pool[ii]->bus)
         {
            found_pool = 1;
         }

         if (found_pool)
            break;
      }

      if (!found_pool)
         return WPI_ERROR(WP_ERR_DATA_BUFFER_POOL_INCOMPATIBLE);

      /*Found ii and jj, indexes for group and bd_pool*/
      *pool_pointer = iwq->type.mdb.adjunct_pool[ii];
      *pool_handle = iwq->type.mdb.adjunct_pool_handle[ii];
   }
   else if (iwq->nodetype == WP_QNODE_IW_HIERARCHICAL)
   {
      WP_U32 ii,jj,found_pool;
      wpi_res_iw_group *group = NULL;

      /*First - find L3,L2 group indexes with corrosponding Data buffer -and via bd_pool get adjunct pool*/
      ii = 0;
      found_pool = 0;
      for (ii=0; ii < iwq->type.hierq.num_groups; ii++)
      {
         group = &(iwq->type.hierq.hier_group[ii]);
         for (jj=0 ; jj < group->num_of_bd_pools ; jj++)
         {
            if (group->wmm_fbp[jj] == WPI_DPS_IWBD_VBITS_FBP_V(v_bits))
            {
               found_pool = 1;
               break;
            }
         }
         if (found_pool)
            break;
      }

      if (!found_pool)
         return WPI_ERROR(WP_ERR_DATA_BUFFER_POOL_INCOMPATIBLE);

      *pool_pointer = iwq->type.hierq.hier_group[ii].data_pool[jj];
      *pool_handle = iwq->type.hierq.hier_group[ii].bd_pool[jj]->data_buffer_pool_handle;
   }
   else
   {
      *pool_pointer = iwq->type.iwq.adjunct_pool;
      *pool_handle = iwq->type.iwq.adjunct_pool_handle;
   }

   return status;
}

/*****************************************************************************
 *
 * Function: WPI_SwQueueMatrixIwHostReceive
 *
 * Purpose: A generic receive frame from L2 Host Queue
 *
 * Description:
 *     Collect data from L2 Host Queue.
 *
 * Inputs:
 *     wpid                 Winpath id
 *     phy_index            phy_index
 *     subtype              channel subtype value
 *     memory               memory map of the resource
 *     fbp                  Free Buffer Pool value
 *     host_queue_base      pointer to the host queue base resource tabke
 *     rx_queue_dummy_iwbd  pointer to the first iw BD of the resource table
 *     data_unit            Pointer to Rx data unit
 *     qnode_handle         QNode handle
 *
 * Outputs:
 *     data_unit            Pointer to Rx data unit with the received data from the Ethernet queue
 *
 * Return Value:
 *     list of status values including error values WP_OK, WP_ERR_DEVICE, ...
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_SwQueueMatrixIwHostReceive(WP_U32 wpid,
                                         wpi_memory *memory,
                                         WP_U8 *host_offset_bd,
                                         WP_U32 fbp,
                                         WP_data_unit *data_unit,
                                         wpi_res_node *iwqnode,
                                         WPI_PTR next_bd_address,
                                         wpi_res_pool *out_adjunct_pool)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 v_bits = 0, fbp_val;
   WP_U32 x_bits;
   WP_U8 *host_offset_buffer;
   WP_U32 vbank;
   WPI_PTR bd_address = 0;
   wpi_dps_iwbd *iwbd = 0;
   WPI_PTR dataptr;
   WP_U8 *hostdataptr;
   WP_data_segment *segment;
   WPI_PTR swap_buffer;
   WP_U32 i;
   WP_boolean bd_started_not_ended_flag = WP_TRUE;
   wpi_res_pool *in_adjunct_pool = NULL;


   /* get the dummy BD address */
   if (iwqnode == NULL)
   {
      return WPI_ERROR(WP_ERR_WDDI);
   }

   segment = data_unit->segment;
   segment = segment->next;

   /* the first segment was filled in WPI_HostReceiveIw
      prior calling WPI_SwQueueMatrixIwHostReceive,
      this is the reason the for loop starts from index 1 */
   for (i = 1; i < data_unit->n_segments; i++)
   {
      if(iwqnode->nodetype == WP_QNODE_IW_MDB)
      {
         WP_U32 j = 0;
         while(j < iwqnode->type.mdb.num_of_adj_pools)
         {
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryPoolFromPointerGet,
                                    WPI_REG_LOCKED,
                                    &out_adjunct_pool,
                                    iwqnode->type.mdb.adjunct_pool[j]);
            if (out_adjunct_pool->count != 0)
            {
               break;
            }
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
            j++;
         }
         if(j == iwqnode->type.mdb.num_of_adj_pools)
         {
            status = WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
            break;
         }
      }
      else
      {
         if (out_adjunct_pool->count == 0)
            break;
      }
      /* next BD address read, the Rx data sits in it */

      /* pointer to the next BD */
      iwbd = (wpi_dps_iwbd *)WPI_HOST_PTR(next_bd_address, host_offset_bd);
      bd_address = next_bd_address;

      WPI_MEM_GET(v_bits, iwbd->v_bits);

      /* Get Adjunc Pool from the Bd and Qnode */
      WPI_GetAdjPoolFromBd(v_bits, iwqnode, &in_adjunct_pool, &segment->pool_handle);

      vbank = in_adjunct_pool->vbank;
      host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);

      /* read the pointer to the next BD data */
      WPI_MEM_GET(dataptr, iwbd->data);
      hostdataptr = (WP_U8*)WPI_HOST_PTR(dataptr, host_offset_buffer);
      WPI_MEM_GET(x_bits, iwbd->x_bits);

      swap_buffer = WPI_NpoolAlloc(out_adjunct_pool);
      if (swap_buffer == WPI_PTR_NULL)
      {
         /* need to free all allocated BDs */
         if(iwqnode->nodetype == WP_QNODE_IW_MDB)
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
         status =  WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
         break;
      }

      /* Increase the number of active data units*/
      data_unit->n_active ++;

      /* Populate the data unit with the BD info. */
      segment->data = hostdataptr;

      /*In matrix buffer chaining mode in first bd of packet */
      /*there is a different way to calculate bd length*/
      segment->data_size = WPI_DPS_IWBD_VBITS_LEN_V(v_bits);

      /* set new bus type from out_adjunct_pool */
      v_bits &= ~WPI_DPS_IWBD_VBITS_IWBT_M;
      v_bits |= WPI_DPS_IWBD_VBITS_IWBT_F(out_adjunct_pool->bus);

      WPI_MEM_SET(iwbd->data, swap_buffer);
      WPI_MEM_SET(iwbd->v_bits, v_bits &
                  (WPI_DPS_IWBD_VBITS_IWBT_M | WPI_DPS_IWBD_VBITS_FBP_M));
      WPI_MEM_SET(iwbd->x_bits, 0);

      fbp_val = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);

      WPI_TakeNextBdAddress(iwbd,
                            WP_FALSE,
                            &next_bd_address,
                            host_offset_bd);

      /*In matrix buffer chaining mode need to return
        to fifo bd if there were more than one bd */
      status = WPI_WmmFifoPut(wpid,
                              WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD),
                              fbp_val,
                              bd_address,
                              (WP_U32 *)iwbd,
                              WP_TRUE);

      if (status != WP_OK)
      {
         if(iwqnode->nodetype == WP_QNODE_IW_MDB)
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
         WPI_NpoolFree(out_adjunct_pool, swap_buffer);
         return status;
      }

      /*In the case of matrix buffer chaining update matrix_flag
       and bd_started_not_ended_flag*/
      /* Disabling matrix_flag for next bd*/
      if(v_bits & WPI_DPS_IWBD_VBITS_E_M)
      {
         bd_started_not_ended_flag = WP_FALSE;
      }

      if (next_bd_address == 0)
      {
         /*In matrix buffer chaining mode
           treat the case when last BD in packet not received yet,
           so bd_started_not_ended_flag is WP_TRUE,
           but there is no next BD*/
         if (bd_started_not_ended_flag == WP_TRUE)
         {
            if(iwqnode->nodetype == WP_QNODE_IW_MDB)
               WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                           WPI_RegistryPoolRelease,
                                           WPI_REG_UNLOCKED,
                                           &out_adjunct_pool);
            status = WPI_ERROR(WP_ERR_HST_PARTUAL_PACKET);
            break;
         }
      }

      segment = segment->next;

      if(iwqnode->nodetype == WP_QNODE_IW_MDB)
      {
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryPoolRelease,
                                     WPI_REG_UNLOCKED,
                                     &out_adjunct_pool);
      }
      /*If E ( end ) bit is on, go out from loop*/
      if(v_bits & WPI_DPS_IWBD_VBITS_E_M)
         break;
   }
   if (i == data_unit->n_segments)
      status = WPI_ERROR(WP_ERR_HST_INSUFFICIENT_SEGMENTS_IN_DATA_UNIT);


   /*In matrix buffer chaining mode check that there are enaugh segments for packet.
     When bd_started_not_ended_flag is WP_TRUE, meening is that
     last BD of packet not arrived yet, but there is no segmetns to fill*/
   if (bd_started_not_ended_flag  == WP_TRUE)
   {
      v_bits &= ~WPI_DPS_IWBD_VBITS_E_M;

      /* release all remaining BDs in the linked list
         the last BD is marked with the E bit */
      while(!(v_bits & WPI_DPS_IWBD_VBITS_E_M))
      {
         /* the next_bd_address may be 0 if not al packet is enqued,
            this if statement isrequired to prevent endless while loop */
         if(next_bd_address != 0)
         {
            WP_status ret_val;
            iwbd = (wpi_dps_iwbd *)WPI_HOST_PTR(next_bd_address, host_offset_bd);
            bd_address = next_bd_address;

            WPI_MEM_GET(v_bits, iwbd->v_bits);
            WPI_MEM_SET(iwbd->v_bits, v_bits &
                        (WPI_DPS_IWBD_VBITS_IWBT_M | WPI_DPS_IWBD_VBITS_FBP_M));
            WPI_MEM_SET(iwbd->x_bits, 0);

            fbp_val = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);

            /* fetch the next bd address before clearing
               the next filed of the current BD */
            WPI_TakeNextBdAddress(iwbd,
                                  WP_FALSE,
                                  &next_bd_address,
                                  host_offset_bd);

            /* release th current BD */
            ret_val = WPI_WmmFifoPut(wpid,
                                     WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD),
                                     fbp_val,
                                     bd_address,
                                     (WP_U32 *)iwbd,
                                     WP_TRUE);
            if(ret_val != WP_OK)
            {
               status = ret_val;
               break;
            }
         }
         else
         {
            /* not all BD were enqued, quit the
               function without release all BDs */
            break;
         }
      }
   }

   return status;
}

/*****************************************************************************
 *
 * Function: WPI_HostReceiveIw
 *
 * Purpose: Implements host receive from interworking
 *
 * Description:
 *
 * Inputs:
 *     handle          Handle for input channel
 *     data_unit       Pointer to data unit
 *
 * Outputs:
 *     data_unit       Gets received data
 *
 * Return Value:                  WP_status
 *     WP_OK                      Successful
 *     WP_ERR_INUSE               Channel is in use by another thread
 *     WP_ERR_HST_NO_DATA_TO_GET  No errors, but no data was received
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_HostReceiveIw(WP_handle handle,
                            WP_U8 data_return_enable,
                            WP_data_unit *data_unit)
{
   wpi_res_channel_block *channel_block = NULL;
   wpi_res_fiwt *res_fiwt = NULL;
   wpi_res_channel *cptr = NULL;
   wpi_res_pkt_iw_host res_pkt_iw_host;
   wpi_port_group *port_group;
   wpi_memory *memory;
   wpi_res_node *iwq = NULL;
   wpi_res_pool *in_adjunct_pool = NULL, *out_adjunct_pool = NULL;
   wpi_res_pool *pool_pointer = NULL;
   wpi_dps_qat *host_qat_base, *qat_hostptr;
   WP_U32 wpid, qat_index;
   wpi_dps_data_extended_info *host_buffer_ex_info_ptr = NULL;
   WPI_PTR bd_addr, next_bd, dataptr, swap_buffer, qat_bufcnt;
   wpi_dps_iwbd *rx_bd;
   WP_data_segment *segment;
   WP_U8 *host_offset_bd, *host_offset_buffer, *buffer;
   WP_U32 v_bits, fbp, vbank, bd_bus;
   WP_handle x_handle;
   WP_status ret_val, status = WP_OK, lock_status = WP_OK;
   WP_boolean status_bc = WP_OK;
   WP_boolean first_bd_returned = WP_FALSE;
   WP_iw_data_return* data_return = NULL;
   WP_U32 serial_index, serial_name;
   WP_U8 serial_id = 0, phy_number;
   WP_U16 temp_index,last_flow_agg_index;
   WP_U8 iw_modes = 0;
   WP_U32 fiwt_type = 0;
   WP_U16  vlan_tag = 0;
   WP_U32 status_bits = 0;
   WP_tag input_port_tag = 0, last_flow_agg_tag = 0, prev_flow_agg_tag = 0;
   WP_ht_iw_valid_fields_mask valid_fields_mask =
      WP_HT_LAST_FLOW_AGG_TAG |
      WP_HT_STATUS_BITS;
   wpi_iw_gap_size *gap_size;
   WP_U32 gap_low_bits;

   /* Lock the controlling structure of the queue.
    * The caller may be blocked for a period of time. */
   wpid = WPI_HANDLE_WPID(handle);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryTxChannelGet,
                           WPI_REG_LOCKED,
                           &cptr,
                           wpid,
                           WPI_HANDLE_INDEX(handle));
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if (cptr == NULL)
   {
      return WPI_ERROR(WP_ERR_INUSE);
   }

   data_unit->n_active = 0;
   segment = data_unit->segment;

   /* Read the BD. */
   rx_bd = (wpi_dps_iwbd *) cptr->host_io;
   WPI_MEM_GET(next_bd, rx_bd->next_bd);
   WPI_MEM_GET(dataptr, rx_bd->data);
   WPI_MEM_GET(v_bits, rx_bd->v_bits);
   WPI_MEM_GET(status_bits, rx_bd->x_bits);

   memory = WPI_RegistryMemory(wpid);
   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);

   if (WPI_DPS_IWBD_VBITS_V_V(v_bits) == 0)
   {
      /* Currently empty or previously empty queue. */

      if (next_bd == 0)
      {
         /* Queue is currently empty. */
         return (WPI_ERROR(WP_ERR_HST_NO_DATA_TO_GET));
      }
      else
      {
         /* Queue was previously empty. */
         /* Return this empty BD to FBP and advance to populated BD. */
         WPI_PTR empty_bd;

         empty_bd = WPI_WPI_PTR(rx_bd, host_offset_bd);
         fbp = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);

         /* Return empty BD to FBP. */
         ret_val = WPI_WmmFifoPut(wpid, bd_bus, fbp, empty_bd,
                                  (WP_U32 *) rx_bd, WP_TRUE);
         if (WP_ERROR_P(ret_val))
         {
            return (ret_val);
         }
         first_bd_returned = WP_TRUE;

         /* Point to populated BD, and fetch contents. */
         rx_bd = (wpi_dps_iwbd *) WPI_HOST_PTR(next_bd, host_offset_bd);
         WPI_MEM_GET(next_bd, rx_bd->next_bd);
         WPI_MEM_GET(dataptr, rx_bd->data);
         WPI_MEM_GET(v_bits, rx_bd->v_bits);
         WPI_MEM_GET(status_bits, rx_bd->x_bits);
      }
   }

   /* Get the free buffer pool FIFO and qnode associated with this buffer */
   fbp = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);
   /* Get a buffer from the qnode's associated adjunct pool */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryQNodeFromFifoGet,
                           WPI_REG_LOCKED,
                           &iwq,
                           wpid,
                           bd_bus,
                           fbp);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (!WPI_INITIALIZED(iwq->state))
   {
      return WPI_ERROR(WP_ERR_QNODE);
   }
   
   /* Get Adjunc Pool from the Bd and Qnode */
   WPI_GetAdjPoolFromBd(v_bits, iwq, &in_adjunct_pool, &segment->pool_handle);
   pool_pointer = in_adjunct_pool;
   if(iwq->nodetype == WP_QNODE_IW_MDB)
   {
      WP_U32 j = 0;
      while(j < iwq->type.mdb.num_of_adj_pools)
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryPoolFromPointerGet,
                                 WPI_REG_LOCKED,
                                 &out_adjunct_pool,
                                 iwq->type.mdb.adjunct_pool[j]);
         if (out_adjunct_pool->count != 0)
         {
            break;
         }
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryPoolRelease,
                                     WPI_REG_UNLOCKED,
                                     &out_adjunct_pool);
         j++;
      }
      if(j == iwq->type.mdb.num_of_adj_pools)
         swap_buffer = WPI_PTR_NULL;
      else
         swap_buffer = WPI_NpoolAlloc(out_adjunct_pool);
   }
   else 
   {
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryPoolFromPointerGet,
                              WPI_REG_LOCKED,
                              &in_adjunct_pool,
                              pool_pointer);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      out_adjunct_pool = in_adjunct_pool;
      swap_buffer = WPI_NpoolAlloc(out_adjunct_pool);
   }

   if (swap_buffer == WPI_PTR_NULL)
   {
      /* Can't allocate buffet from adjunct pool */
      /* Return without receiving data */
      data_unit->n_active = 0;

      if (first_bd_returned)
      {
         /* If first BD was empty and returned to FIFO, update cptr */
         cptr->host_io = rx_bd;

         /* First BD was returned, need to update QAT */

         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryPortGroupArrayGet,
                                 WPI_REG_LOCKED,
                                 &port_group,
                                 wpid,
                                 WPI_pg_pkt_iw_host);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         host_qat_base = (wpi_dps_qat *) port_group->host_qat_base;
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryTxChannelBlockGet,
                                 WPI_REG_LOCKED,
                                 &channel_block,
                                 wpid);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         qat_index =
            WPI_HANDLE_INDEX(handle) -
            channel_block->channel_list[WPI_pg_pkt_iw_host].group_offset;
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryTxChannelBlockRelease,
                                     WPI_REG_UNLOCKED,
                                     &channel_block);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         qat_hostptr = &host_qat_base[qat_index];

         /* Atomically decrement the QAT's buffer counter */
         qat_bufcnt = WPI_WPI_PTR(&qat_hostptr->neq,
                                  WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_IW_FIWT_QAT));

         ret_val = WPI_WmmAtomicDecrement(wpid,
                                          WPI_MemoryBusOfVirtualBus(memory, WP_VB_IW_FIWT_QAT),
                                          WPI_WMM_SIZE_WORD,
                                          qat_bufcnt,
                                          NULL);
         if (WP_ERROR_P(ret_val))
         {
            return ret_val;
         }
      }
      return WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
   }

   /* Get info on buffer pool to which this buffer belongs */
   vbank = in_adjunct_pool->vbank;
   host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);
   /* Populate the data unit with the BD info. */

   buffer = (WP_U8 *) WPI_HOST_PTR(dataptr, host_offset_buffer);

   data_unit->n_active = 1;
   segment->data_size =
      data_unit->data_size = WPI_DPS_IWBD_VBITS_LEN_V(v_bits);
   segment->data = buffer;

   /* For WinSim, copy the data back from the simulator. */
   WPI_MEM_WINSIM_TO_HOST(buffer, data_unit->data_size);

   v_bits &= ~WPI_DPS_IWBD_VBITS_IWBT_M;
   v_bits |= WPI_DPS_IWBD_VBITS_IWBT_F(out_adjunct_pool->bus);
   if(v_bits & WPI_DPS_MATRIX_BD_VBITS_MTBD_M)
   {
      WPI_PTR next_bd_address;

      if(iwq->nodetype == WP_QNODE_IW_MDB)
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryPoolRelease,
                                     WPI_REG_UNLOCKED,
                                     &out_adjunct_pool);
      /* update the first segment with the correct data len */
      /*In matrix buffer chaining mode in first bd of packet */
      /*there is a different way to calculate bd length*/
      segment->data_size = WPI_DPS_MATRIX_BD_BDLENGTH_V(status_bits);

      WPI_TakeNextBdAddress(rx_bd,
                            WP_TRUE,
                            &next_bd_address,
                            host_offset_bd);

      /* get new status bits from second BD, which are the host term protocol bits */
      WPI_MEM_GET(status_bits, 
                  *(WP_U32*)(WPI_HOST_PTR(next_bd_address, host_offset_bd) + 12));

      status = WPI_SwQueueMatrixIwHostReceive(wpid,
                                              memory,
                                              host_offset_bd,
                                              fbp,
                                              data_unit,
                                              iwq,
                                              next_bd_address,
                                              out_adjunct_pool);
   }

   if(iwq->nodetype != WP_QNODE_IW_MDB)
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                  WPI_RegistryPoolRelease,
                                  WPI_REG_UNLOCKED,
                                  &out_adjunct_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryQNodeRelease,
                               WPI_REG_UNLOCKED,
                               &iwq);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   /* Swap newly allocated buffer into BD and reset BD status */
   WPI_MEM_SET(rx_bd->data, swap_buffer);
   WPI_MEM_SET(rx_bd->v_bits, v_bits &
               (WPI_DPS_IWBD_VBITS_IWBT_M | WPI_DPS_IWBD_VBITS_FBP_M));

   WPI_MEM_SET(rx_bd->x_bits, 0);

   /* If this is not the last populated BD,
      then return BD to its originating free buffer pool. */
   if (next_bd != 0)
   {
      /* Point channel to next BD */
      cptr->host_io = WPI_HOST_PTR(next_bd, host_offset_bd);

      /* Get BD location info. */
      bd_addr = WPI_WPI_PTR(rx_bd, host_offset_bd);

      /* Return BD to FIFO */
      ret_val = WPI_WmmFifoPut(wpid, bd_bus, fbp, bd_addr,
                               (WP_U32 *) rx_bd, WP_TRUE);
      if (WP_ERROR_P(ret_val))
      {
         return (ret_val);
      }
   }
   else
   {
      /* Check this BD's link field next time. */
      cptr->host_io = rx_bd;
   }

   /* Find the QAT for this channel */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryTxChannelBlockGet,
                           WPI_REG_LOCKED,
                           &channel_block,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPortGroupArrayGet,
                           WPI_REG_LOCKED,
                           &port_group,
                           wpid,
                           WPI_pg_pkt_iw_host);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   host_qat_base = (wpi_dps_qat *) port_group->host_qat_base;
   qat_index =
      WPI_HANDLE_INDEX(handle) -
      channel_block->channel_list[WPI_pg_pkt_iw_host].group_offset;
   qat_hostptr = &host_qat_base[qat_index];

   /* Atomically decrement the QAT's buffer counter */
   qat_bufcnt = WPI_WPI_PTR(&qat_hostptr->neq,
                            WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_IW_FIWT_QAT));
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryPortGroupArrayRelease,
                               WPI_REG_UNLOCKED,
                               &port_group);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryTxChannelBlockRelease,
                               WPI_REG_UNLOCKED,
                               &channel_block);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   ret_val =
      WPI_WmmAtomicDecrement(wpid,
                             WPI_MemoryBusOfVirtualBus(memory, WP_VB_IW_FIWT_QAT),
                             WPI_WMM_SIZE_WORD,
                             qat_bufcnt,
                             NULL);
   if (WP_ERROR_P(ret_val))
   {
      return ret_val;
   }

   /* check matrix receive status */
   if (WP_ERROR_P(status))
   {
      return status;
   }

   /* Get the pointer to the start of the buffer gap */
   status_bc = WPI_DpsFeatureCheckStatus(wpid,
                                         WPI_FEATUREMODE_GLOBAL,
                                         0,
                                         WPI_DPS_MODULE_IW_BUFF_CHAIN_ENABLE);


   /* ptp loopback ,not regular Rx, so no valid returned data or status bits  */
   if(v_bits & WPI_DPS_IWBD_VBITS_PTP_TX_M)
   {

      if(data_return_enable)
      {
         if( WPI_RegistryResourceEntryValidCheck(wpid, WPI_ptp) )
         {
            wpi_ptp_functions *res_ptp_functions = NULL;
            data_return = (WP_iw_data_return*)data_unit->data_return;
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryPtpFunctionsGet,
                                    WPI_REG_LOCKED,
                                    &res_ptp_functions,
                                    wpid);
            WPI_RETURN_IF_ERROR_STATUS(lock_status);
            if (res_ptp_functions)
            {
               /* Function adds Ts to returned data struct and modifies status_bits*/
               ret_val = res_ptp_functions->data_return_adj(wpid,
                                                            data_return,
                                                            0,
                                                            data_unit,
                                                            v_bits,
                                                            status_bits);
               WPI_RETURN_IF_ERROR_STATUS(ret_val);
            }
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPtpFunctionsRelease,
                                        WPI_REG_UNLOCKED,
                                        &res_ptp_functions);
            WPI_RETURN_IF_ERROR_STATUS(lock_status);
         }
         /* set valid fields */
         if (data_return == NULL)
         {
             return WPI_ERROR(WP_ERR_NULL_PTR);
         }
         data_return->valid_fields_mask  |= WP_HT_PTP_INFO ;
      }
      return WP_OK ;
   }

   /*********************************************/
   /* copy data from the channel configuration  */
   /* so that the channel can be released.      */
   /* From here the channel cannot be modified. */
   /*********************************************/
   memcpy (&res_pkt_iw_host, &(cptr->du.pkt.type.iw_host), sizeof (wpi_res_pkt_iw_host));
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryTxChannelRelease,
                               WPI_REG_UNLOCKED,
                               &cptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   /*********************************************/
   /* Parse the returned data */
   /*********************************************/
   /* When using IP fragment the extended data is saved only in the first buffer */
   if (res_pkt_iw_host.ip_frag_host_io_info.ip_frag_pkt_left != 0)
   {
      /* an IPv4 fragment other than the first fragment.  */
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryTxChannelGet,
                              WPI_REG_LOCKED,
                              &cptr,
                              wpid,
                              WPI_HANDLE_INDEX(handle));
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      cptr->du.pkt.type.iw_host.ip_frag_host_io_info.ip_frag_pkt_left--;
      if (data_return_enable)
      {
         if (!(data_unit->data_return))
            return WPI_ERROR(WP_ERR_DATA_UNIT_INCOMPATIBLE);
         memcpy(data_unit->data_return,
                &cptr->du.pkt.type.iw_host.ip_frag_host_io_info.iw_data_return,
                sizeof(WP_iw_data_return));
      }
      /* Saved for backward compatibilty */
      vlan_tag = cptr->du.pkt.type.iw_host.ip_frag_host_io_info.iw_data_return.vlan_tag;
      status_bits = cptr->du.pkt.type.iw_host.ip_frag_host_io_info.iw_data_return.status_bits;
      last_flow_agg_tag =
         cptr->du.pkt.type.iw_host.ip_frag_host_io_info.iw_data_return.last_flow_agg_tag;
      serial_id = cptr->du.pkt.type.iw_host.ip_frag_host_io_info.iw_data_return.rx_serial_id;
      input_port_tag =
         cptr->du.pkt.type.iw_host.ip_frag_host_io_info.iw_data_return.input_port_tag;
      last_flow_agg_index = cptr->du.pkt.type.iw_host.ip_frag_host_io_info.last_flow_agg_index;
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                  WPI_RegistryTxChannelRelease,
                                  WPI_REG_UNLOCKED,
                                  &cptr);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      WPI_REGISTRY_OBJECT_GET(lock_status,
                              WPI_RegistryFiwtGet,
                              WPI_REG_LOCKED,
                              &res_fiwt,
                              wpid,
                              last_flow_agg_index);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);

      if ((WPI_STATE_IW_FIWT_FLAGS_V(res_fiwt->state)
           & WPI_STATE_INITIALIZED) == 0)
      {
         return WPI_ERROR(WP_ERR_IW_FLOW_UNINITIALIZED);
      }
      fiwt_type = res_fiwt->type;
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                  WPI_RegistryFiwtRelease,
                                  WPI_REG_UNLOCKED,
                                  &res_fiwt);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
   }
   else
   {
      /* Get ptr to the start of the buffer (start of the gap) */
      if (status_bc == WP_FALSE)
      {
         gap_size = WPI_RegistryIwGapSize(wpid);
         gap_low_bits = gap_size->alignment - 1;
         host_buffer_ex_info_ptr =
            (wpi_dps_data_extended_info*)(((WP_U32)buffer) & ~gap_low_bits);
      }
      else
      {
         host_buffer_ex_info_ptr =
            (wpi_dps_data_extended_info*)(((WP_U32)buffer) &
                                          ~WPI_POOL_ALIGNMENT_MASK_BUFFER_CHAIN_IW);
      }

      /* Get number of fragments left */
      WPI_MEM_BYTE_GET(res_pkt_iw_host.ip_frag_host_io_info.ip_frag_pkt_left,
                       host_buffer_ex_info_ptr->ip_frag_pkt_left);
      if (res_pkt_iw_host.ip_frag_host_io_info.ip_frag_pkt_left != 0)
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryTxChannelGet,
                                 WPI_REG_LOCKED,
                                 &cptr,
                                 wpid,
                                 WPI_HANDLE_INDEX(handle));
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         cptr->du.pkt.type.iw_host.ip_frag_host_io_info.ip_frag_pkt_left =
            res_pkt_iw_host.ip_frag_host_io_info.ip_frag_pkt_left;
         cptr->du.pkt.type.iw_host.ip_frag_host_io_info.ip_frag_pkt_left--;
         res_pkt_iw_host.ip_frag_host_io_info.ip_frag_pkt_left--;
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryTxChannelRelease,
                                     WPI_REG_UNLOCKED,
                                     &cptr);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
      }

      /* Get Serial number */
      WPI_MEM_BYTE_GET(serial_id,
                       host_buffer_ex_info_ptr->serial_number);

      WPI_MEM_BYTE_GET(iw_modes,
                       host_buffer_ex_info_ptr->iw_modes);

      if (iw_modes & (WPI_L2IWT_WORKMODE_B_MODE | WPI_L2IWT_WORKMODE_R_MODE))
      {
         WPI_MEM_SHORT_GET(vlan_tag,
                           host_buffer_ex_info_ptr->vlan_tag);
         valid_fields_mask |= WP_HT_VLAN_TAG;
      }
      else
         vlan_tag = 0;

      /*********************************************/
      /* Get handles and tags of the returned data */
      /*********************************************/

      if (status_bits & WP_HT_IW_RX_STATUS_FA_TO_FA)
      {
         /* Get handle and tag of the previous flow aggregation */
         WPI_MEM_SHORT_GET(temp_index,
                           host_buffer_ex_info_ptr->prev_flow_agg_index);
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryFiwtGet,
                                 WPI_REG_LOCKED,
                                 &res_fiwt,
                                 wpid,
                                 temp_index);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);

         if ((WPI_STATE_IW_FIWT_FLAGS_V(res_fiwt->state)
              & WPI_STATE_INITIALIZED) == 0)
         {
            return WPI_ERROR(WP_ERR_IW_FLOW_UNINITIALIZED);
         }
         prev_flow_agg_tag = res_fiwt->tag;
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryFiwtRelease,
                                     WPI_REG_UNLOCKED,
                                     &res_fiwt);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         valid_fields_mask |= WP_HT_PREV_FLOW_AGG_TAG;
      }

      /* Get handle and tag of the last flow aggregation */
      WPI_MEM_SHORT_GET(last_flow_agg_index,
                        host_buffer_ex_info_ptr->last_flow_agg_index);

      lock_status = WPI_RegistryFiwtTagGet(wpid,
                                           last_flow_agg_index,
                                           &last_flow_agg_tag);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);
      lock_status = WPI_RegistryFiwtTypeGet(wpid,
                                            last_flow_agg_index,
                                            &fiwt_type);
      WPI_RETURN_IF_ERROR_STATUS(lock_status);

      /* Get handle and tag of the iw port */
      if (iw_modes & (WPI_L2IWT_WORKMODE_B_MODE | WPI_L2IWT_WORKMODE_R_MODE))
      {
         WPI_MEM_SHORT_GET(temp_index,
                           host_buffer_ex_info_ptr->iw_port_index);
         lock_status = WPI_RegistryIwPortTagGet(wpid,
                                                temp_index,
                                                &input_port_tag);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         valid_fields_mask |= WP_HT_INPUT_PORT_TAG;
      }
      else
         input_port_tag = 0;

      if (data_return_enable)
      {
         data_return = (WP_iw_data_return*)data_unit->data_return;
         if (!data_return)
            return WPI_ERROR(WP_ERR_DATA_UNIT_INCOMPATIBLE);
         /* Get extended information on the packet */
         WPI_MEM_SHORT_GET(data_return->bfd_changes_bitset,
                           host_buffer_ex_info_ptr->u_protocol_specific.bfd_changes_bitset);
         if(status_bits & WP_HT_IW_RX_STATUS_BFD)
         {
            valid_fields_mask |= WP_HT_BFD_PACKET;
         }

         WPI_MEM_BYTE_GET(phy_number,
                          host_buffer_ex_info_ptr->phy_number);
         /* get sa serial index */
         WPI_RETURN_IF_ERROR(status,
                             WPI_HwSerialIndex(wpid, serial_id, (WP_U32*)&serial_index));

         WPI_MEM_SHORT_GET(temp_index,
                           host_buffer_ex_info_ptr->rx_channel_index);

         /* convert internal sa serial index to api name - doesnt work for TDI*/
         if(WPI_ApiSerialName(serial_index, &serial_name) &&
            /* In GBOND the returned phy number if 0xff - not supported */
            phy_number != WP_HT_INVALID_PHY)
         {
            valid_fields_mask |= WP_HT_RX_SERIAL_ID;
            WPI_RETURN_IF_ERROR(status,
                                WPI_GetRxTag(serial_id,
                                             phy_number,
                                             temp_index,
                                             wpid,
                                             &valid_fields_mask,
                                             &data_return->rx_tag));
            data_return->rx_serial_id = serial_name;
         }
         else
         {
            data_return->rx_tag = 0;
            data_return->rx_serial_id = (WP_U8)WP_SA_SERIAL_INVALID;
         }

         data_return->vlan_tag = vlan_tag;
         data_return->input_port_tag = input_port_tag;
         data_return->last_flow_agg_tag = last_flow_agg_tag;
         data_return->prev_flow_agg_tag = prev_flow_agg_tag;


         /* ptp Rx */
         if(status_bits & WPI_HT_IW_RX_STATUS_PTP_M)
         {
            if( WPI_RegistryResourceEntryValidCheck(wpid, WPI_ptp) )
            {
               wpi_ptp_functions *res_ptp_functions = NULL;
               WPI_REGISTRY_OBJECT_GET(lock_status,
                                       WPI_RegistryPtpFunctionsGet,
                                       WPI_REG_LOCKED,
                                       &res_ptp_functions,
                                       wpid);
               WPI_RETURN_IF_ERROR_STATUS(lock_status);
               if (res_ptp_functions)
               {
                  /* Function adds Ts to returned data struct and modifies status_bits*/
                  ret_val = res_ptp_functions->data_return_adj(
                     wpid,
                     data_return,
                     host_buffer_ex_info_ptr,
                     0,
                     v_bits,
                     status_bits) ;
                  if (WP_ERROR_P(ret_val))
                  {
                     return ret_val;
                  }
               }
               WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                           WPI_RegistryPtpFunctionsRelease,
                                           WPI_REG_UNLOCKED,
                                           &res_ptp_functions);
               WPI_RETURN_IF_ERROR_STATUS(lock_status);
            }
            /* set valid fields */
            valid_fields_mask |= WP_HT_PTP_INFO ;
         }

         data_return->status_bits = status_bits;
         data_return->valid_fields_mask = valid_fields_mask;

         if (iw_modes & WPI_L2IWT_WORKMODE_R_MODE)
            data_return->status_bits |= WP_HT_IW_RX_STATUS_ROUTING;
         if (iw_modes & WPI_L2IWT_WORKMODE_B_MODE)
            data_return->status_bits |= WP_HT_IW_RX_STATUS_BRIDGING;
         if (iw_modes & WPI_L2IWT_WORKMODE_S_MODE)
            data_return->status_bits |= WP_HT_IW_RX_STATUS_SWITCHING;

         if (res_pkt_iw_host.ip_frag_host_io_info.ip_frag_pkt_left != 0)
         {
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryTxChannelGet,
                                    WPI_REG_LOCKED,
                                    &cptr,
                                    wpid,
                                    WPI_HANDLE_INDEX(handle));
            WPI_RETURN_IF_ERROR_STATUS(lock_status);
            memcpy(&cptr->du.pkt.type.iw_host.ip_frag_host_io_info.iw_data_return,
                   data_unit->data_return,
                   sizeof(WP_iw_data_return));
            memcpy(&res_pkt_iw_host.ip_frag_host_io_info.iw_data_return,
                   data_unit->data_return,
                   sizeof(WP_iw_data_return));
            cptr->du.pkt.type.iw_host.ip_frag_host_io_info.last_flow_agg_index =
               last_flow_agg_index;
            res_pkt_iw_host.ip_frag_host_io_info.last_flow_agg_index =
               last_flow_agg_index;
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryTxChannelRelease,
                                        WPI_REG_UNLOCKED,
                                        &cptr);
            WPI_RETURN_IF_ERROR_STATUS(lock_status);
         }
      }
   }

   /* Backward compatibilty */
   /* return the information bits back to the user,
      for bridging for vlan, this is the vlan tag */
   x_handle = 0;
   data_unit->status = vlan_tag;
   switch(WPI_IwFlowAggHostTermType[fiwt_type])
   {
      case WPI_FLOW_AGG_HOST_QUEUE_TYPE:
         if ((iw_modes & WPI_L2IWT_WORKMODE_R_MODE) &&
             last_flow_agg_index == WP_IW_HQ_VIOLATION)
            data_unit->status = status_bits;

         if (((iw_modes & WPI_L2IWT_WORKMODE_R_MODE) &&
              last_flow_agg_index == WP_IW_HQ_VIOLATION &&
              (status_bits &
               (WP_HT_IW_RX_STATUS_TTLE |
                WP_HT_IW_RX_STATUS_MTUE | WP_HT_IW_RX_STATUS_MPE))) ||
             (res_pkt_iw_host.rx_fiwt_info & WPI_RES_IW_RX_FIWT_INFO_IFN_M))
         {
            if (res_pkt_iw_host.rx_fiwt_info & WPI_RES_IW_RX_FIWT_INFO_IPS_M)
               x_handle = prev_flow_agg_tag;
            else
               x_handle = last_flow_agg_tag;
         }
         else if (res_pkt_iw_host.rx_fiwt_info & WPI_RES_IW_RX_FIWT_INFO_ISN_M)
         {
            x_handle = serial_id << WP_HT_IW_RX_SERIAL_NUMBER_S;
         }
         else
            goto insert_iw_port_tag;
         break;
         /* For all other flow types, control = IW Port Tag */
      case WPI_FLOW_AGG_BRIDGE_TYPE:
         /* return DCRC bit */
         data_unit->status |= WP_HT_IW_CRC_FIELD(
            WPI_DPS_IWBD_VBITS_DCRC_V(v_bits));
      case WPI_FLOW_AGG_ROUTING_TYPE:
      case WPI_FLOW_AGG_DIRECT_TYPE:
     insert_iw_port_tag:
         x_handle = input_port_tag;
            break;
      default:
         break;
   }

   data_unit->control = x_handle;

   if (ret_val == WP_OK && status != WP_OK)
      return status;
   return ret_val;
}


/*****************************************************************************
 *
 * Function: WP_ReverseL2ProtConvert
 *
 * Purpose:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     WP_OK
 *     WP_ERR_IW_FLOW_AGG_NOT_EMPTY
 *                          - if there are flows connected to the aggregation
 * Called by:
 *     WPI_HostSendIw
 *
 ****************************************************************************/

WP_U32 WPI_ReverseL2ProtConvert(WP_U32 protocol)
{
   WPI_INTERNAL_FUNCTION();
   switch (protocol)
   {

      case WPI_FIWT_AAL5:
         return WP_ATM_AAL5;
      case WPI_FIWT_HSPI_POS:
      case WPI_FIWT_HDLC:
         return WP_HDLC;
      case WPI_FIWT_FRAME_RELAY:
         return WP_FRAME_RELAY;
      case WPI_FIWT_HSPI_GENET:
      case WPI_FIWT_ENET:
         return WP_ENET;
      case WPI_FIWT_TRANSPARENT:
         return WP_TRANSPARENT;
      case WPI_FIWT_AAL0:
         return WP_ATM_AAL0;
      case WPI_FIWT_IW_HOST:
         return WP_IW_HOST;
      case WPI_FIWT_AAL2_SSAR:
      case WPI_FIWT_AAL2_CPS:
         return WP_ATM_AAL2;
      default:
         return 0xffff;
   }
}



/*****************************************************************************
 *
 * Function: WPI_SwQueueHostReceive
 *
 * Purpose: A generic receive frame from L2 Host Queue
 *
 * Description:
 *     Collect data from L2 Host Queue.
 *
 * Inputs:
 *     wpid                 Winpath id
 *     phy_index            phy_index
 *     memory               memory map of the resource
 *     bd_bus               virtual bus type
 *     rx_host_queue_base   dps address of host queue base
 *     host_queue_base      pointer to the host queue base resource tabke
 *     rx_queue_dummy_iwbd  pointer to the iw BD
 *     data_unit            Pointer to Rx data unit
 *     qnode_handle         QNode handle
 *
 * Outputs
 *     data_unit            Pointer to Rx data unit with the received data from the Ethernet queue
 *
 * Return Value:
 *     list of status values including error values WP_OK, WP_ERR_DEVICE, ...
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_SwQueueHostReceive(WP_U32 wpid,
                                 wpi_memory *memory,
                                 WP_U32 bd_bus,
                                 wpi_dps_sw_host_rx_queue *rx_host_queue_base,
                                 WP_U32 host_queue_bus,
                                 WPI_PTR *rx_queue_dummy_iwbd,
                                 WP_data_unit *data_unit,
                                 WP_handle qnode_handle)
{
   WP_status status = WP_OK, lock_status = WP_OK;
   WP_U32 v_bits, next_v_bits, fbp_val;
   WP_U32 next_x_bits;
   WP_U8 *host_offset_bd, *host_offset_buffer;
   WPI_PTR dummy_bd_addr, next_bd_address = 0, bd_for_host_rx = 0;
   wpi_dps_iwbd *dummy_iwbd, *next_iwbd;
   WPI_PTR dataptr;
   WP_U8 *hostdataptr;
   WP_data_segment *segment;
   WPI_PTR swap_buffer;
   WPI_PTR list_cnt;
   wpi_res_node *iwqnode = NULL;
   wpi_res_pool *in_adjunct_pool = NULL, *out_adjunct_pool = NULL;
   WP_handle in_adjunct_pool_handle;
   WP_U32 i;
   WP_boolean matrix_buffer_chaining_mode_enabled = WP_FALSE;
   WP_boolean matrix_flag = WP_FALSE, bd_started_not_ended_flag = WP_FALSE;

   /*Check if matrix buffer chaining enabled*/
   if (WPI_DpsFeatureCheckStatus(wpid,
                                 WPI_FEATUREMODE_GLOBAL,
                                 0,
                                 WPI_DPS_MODULE_MATRIX_BUFF_CHAIN_ENABLE_)== WP_TRUE)
      matrix_buffer_chaining_mode_enabled = WP_TRUE;

   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, bd_bus);
   /* get the dummy BD address */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryQNodeGet,
                           WPI_REG_LOCKED,
                           &iwqnode,
                           wpid,
                           WPI_HANDLE_INDEX(qnode_handle));
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   if (iwqnode == NULL)
   {
      return WPI_ERROR(WP_ERR_WDDI);
   }
   if (iwqnode->nodetype != WP_QNODE_IW_HIERARCHICAL)
   {
      if(iwqnode->nodetype != WP_QNODE_IW_MDB)
      {
         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryPoolFromPointerGet,
                                 WPI_REG_LOCKED,
                                 &out_adjunct_pool,
                                 iwqnode->type.iwq.adjunct_pool);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
      }
   }
   else
      return(WPI_ERROR(WP_ERR_UNIMPLEMENTED));  /*hierarchical Qnode not supported*/

   segment = data_unit->segment;

   /*Set number of active segmets to 0*/
   data_unit->n_active = 0;
   for (i = 0; i < data_unit->n_segments; i++)
   {
      if(iwqnode->nodetype == WP_QNODE_IW_MDB)
      {
         WP_U32 j = 0;
         while(j < iwqnode->type.mdb.num_of_adj_pools)
         {
            WPI_REGISTRY_OBJECT_GET(lock_status,
                                    WPI_RegistryPoolFromPointerGet,
                                    WPI_REG_LOCKED,
                                    &out_adjunct_pool,
                                    iwqnode->type.mdb.adjunct_pool[j]);
            if (out_adjunct_pool->count != 0)
            {
               break;
            }
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
            j++;
         }
         if(j == iwqnode->type.mdb.num_of_adj_pools)
         {
            status = WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
            break;
         }
      }
      else
      {
         if (out_adjunct_pool->count == 0)
            break;
      }

      dummy_bd_addr = *rx_queue_dummy_iwbd;
      if (dummy_bd_addr == 0)
      {
         if(iwqnode->nodetype == WP_QNODE_IW_MDB)
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
         return WPI_ERROR(WP_ERR_WDDI);
      }

      /* pointer to the dummy BD */
      dummy_iwbd = (wpi_dps_iwbd *)WPI_HOST_PTR(dummy_bd_addr, host_offset_bd);

      /* next BD address read, the Rx data sits in it */
      WPI_TakeNextBdAddress(dummy_iwbd,
                            matrix_flag,
                            &next_bd_address,
                            host_offset_bd);

      if (next_bd_address == 0)
      {
         /* no received BD in the queue */
         if (i == 0)
         {
            if(iwqnode->nodetype == WP_QNODE_IW_MDB)
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
            status = WPI_BASIC_ERROR(WP_ERR_HST_NO_DATA_TO_GET);
            break;
         }
         /*In matrix buffer chaining mode
           treat the case when last BD in packet not received yet,
           so bd_started_not_ended_flag is WP_TRUE,
           but there is no next BD*/
         else if(matrix_buffer_chaining_mode_enabled == WP_TRUE)
         {
            if (bd_started_not_ended_flag == WP_TRUE)
               status = WPI_ERROR(WP_ERR_HST_PARTUAL_PACKET);
            if(iwqnode->nodetype == WP_QNODE_IW_MDB)
               WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                           WPI_RegistryPoolRelease,
                                           WPI_REG_UNLOCKED,
                                           &out_adjunct_pool);
            break;
         }
         else
         {
            status = WPI_ERROR(WP_ERR_WDDI);
            if(iwqnode->nodetype == WP_QNODE_IW_MDB)
               WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                           WPI_RegistryPoolRelease,
                                           WPI_REG_UNLOCKED,
                                           &out_adjunct_pool);
            break;
         }

      }

      *rx_queue_dummy_iwbd = next_bd_address;

      /* pointer to the next BD */
      next_iwbd = (wpi_dps_iwbd *)WPI_HOST_PTR(next_bd_address, host_offset_bd);
      /* read the pointer to the next BD data */
      WPI_MEM_GET(dataptr, next_iwbd->data);
      
      WPI_MEM_GET(next_v_bits, next_iwbd->v_bits);

      WPI_MEM_GET(next_x_bits, next_iwbd->x_bits);
      /* Increase the number of active data units*/
      data_unit->n_active ++;

      /*In the case of matrix buffer chaining update matrix_flag
       and bd_started_not_ended_flag*/
      if(matrix_buffer_chaining_mode_enabled == WP_TRUE)
      {
         if(i == 0)
            bd_for_host_rx = next_bd_address;
         /* Disabling matrix_flag for next bd*/
         if(i == 1)
            matrix_flag = WP_FALSE;
         if(next_v_bits & WPI_DPS_MATRIX_BD_VBITS_MTBD_M)
         {
            bd_started_not_ended_flag = WP_TRUE;
            matrix_flag = WP_TRUE;
            goto WPI_SwQueueHostReceiveDecListCounter;
         }
         else if(next_v_bits & WPI_DPS_IWBD_VBITS_E_M)
         {
            *rx_queue_dummy_iwbd = bd_for_host_rx;
            if(bd_started_not_ended_flag == WP_FALSE)
               goto WPI_SwQueueHostReceiveDecListCounter;
            bd_started_not_ended_flag = WP_FALSE;
         }
      }
      else
      {
        WPI_SwQueueHostReceiveDecListCounter:
         /* Atomically decrement the list's counter */
         list_cnt = (WPI_PTR) &(rx_host_queue_base->list_counter);
         status = WPI_WmmAtomicDecrement(wpid,
                                         host_queue_bus,
                                         WPI_WMM_SIZE_WORD,
                                         list_cnt,
                                         NULL);
         if (status != WP_OK)
         {
            if(iwqnode->nodetype == WP_QNODE_IW_MDB)
               WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                           WPI_RegistryPoolRelease,
                                           WPI_REG_UNLOCKED,
                                           &out_adjunct_pool);
            return status;
         }
      }

      host_offset_buffer = WPI_MemoryOffsetOfVirtualBus(memory, WPI_DPS_IWBD_VBITS_IWBT_V(next_v_bits));
      hostdataptr = (WP_U8*)WPI_HOST_PTR(dataptr, host_offset_buffer);

      /* find in_adjunct pool handle */
      status = WPI_GetAdjPoolFromBd(next_v_bits,
                                    iwqnode,
                                    &in_adjunct_pool,
                                    &in_adjunct_pool_handle);
      WPI_RETURN_IF_ERROR_STATUS(status);

      /* Populate the data unit with the BD info. */
      segment->data = hostdataptr;
      segment->pool_handle = in_adjunct_pool_handle;

      /*In matrix buffer chaining mode in first bd of packet */
      /*there is a different way to calculate bd length*/
      if((matrix_buffer_chaining_mode_enabled == WP_TRUE) &&
         (next_v_bits & WPI_DPS_MATRIX_BD_VBITS_MTBD_M ))
         segment->data_size =WPI_DPS_MATRIX_BD_BDLENGTH_V( next_x_bits);
      else
         segment->data_size = WPI_DPS_IWBD_VBITS_LEN_V(next_v_bits);
      data_unit->data_size += segment->data_size;

      swap_buffer = WPI_NpoolAlloc(out_adjunct_pool);
      if (swap_buffer == WPI_PTR_NULL)
      {
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
         return WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
      }

      WPI_MEM_SET(next_iwbd->data, swap_buffer);
      next_v_bits &= ~WPI_DPS_IWBD_VBITS_IWBT_M;
      next_v_bits |= WPI_DPS_IWBD_VBITS_IWBT_F(out_adjunct_pool->bus);
      WPI_MEM_SET(next_iwbd->v_bits, next_v_bits);

      /* return the dummy BD to its originating FBP */
      /* In matrix buffer chaining need not return second bd*/
      if((matrix_buffer_chaining_mode_enabled == WP_FALSE)||
         (i != 1))
      {
         WPI_MEM_GET(v_bits, dummy_iwbd->v_bits);
         fbp_val = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);
         status = WPI_WmmFifoPut(wpid,
                                 WPI_MemoryBusOfVirtualBus(memory, bd_bus),
                                 fbp_val,
                                 dummy_bd_addr,
                                 (WP_U32 *)dummy_iwbd,
                                 WP_TRUE);
      }

      /*In matrix buffer chaining mode need to return to fifo last bd if there were more,
       than one bd*/
      if((matrix_buffer_chaining_mode_enabled == WP_TRUE) && (i > 0 )&&
         (next_v_bits & WPI_DPS_IWBD_VBITS_E_M))
      {
         fbp_val = WPI_DPS_IWBD_VBITS_FBP_V(next_v_bits);
         status = WPI_WmmFifoPut(wpid,
                                 WPI_MemoryBusOfVirtualBus(memory, bd_bus),
                                 fbp_val,
                                 next_bd_address,
                                 (WP_U32 *)next_iwbd,
                                 WP_TRUE);
      }

      if (status != WP_OK)
      {
         WPI_NpoolFree(out_adjunct_pool, swap_buffer);
         if(iwqnode->nodetype == WP_QNODE_IW_MDB)
            WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                        WPI_RegistryPoolRelease,
                                        WPI_REG_UNLOCKED,
                                        &out_adjunct_pool);
         return status;
      }

      segment = segment->next;

      if(iwqnode->nodetype == WP_QNODE_IW_MDB)
      {
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryPoolRelease,
                                     WPI_REG_UNLOCKED,
                                     &out_adjunct_pool);
      }
      /*If E ( end ) bit is on, go out from loop*/
      if(next_v_bits & WPI_DPS_IWBD_VBITS_E_M)
         break;
   }
   if (i == data_unit->n_segments && 
       matrix_buffer_chaining_mode_enabled == WP_TRUE)
   {
      status = WPI_ERROR(WP_ERR_HST_INSUFFICIENT_SEGMENTS_IN_DATA_UNIT);
   }

   /*In matrix buffer chaining mode check that there are enaugh segments for packet.
     When bd_started_not_ended_flag is WP_TRUE, meening is that
     last BD of packet not arrived yet, but there is no segmetns to fill*/

   if(iwqnode->nodetype != WP_QNODE_IW_MDB)
   {
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                  WPI_RegistryPoolRelease,
                                  WPI_REG_UNLOCKED,
                                  &out_adjunct_pool);
   }
   if (matrix_buffer_chaining_mode_enabled == WP_TRUE &&
       bd_started_not_ended_flag == WP_TRUE)
   {
      /* release all remaining BDs in the linked list
         the last BD is marked with the E bit */
      while(1)
      {
         /* the next_bd_address may be 0 if not al packet is enqued,
            this if statement isrequired to prevent endless while loop */
         dummy_bd_addr = next_bd_address;
         dummy_iwbd = (wpi_dps_iwbd *)WPI_HOST_PTR(dummy_bd_addr, host_offset_bd);

         /* next BD address read, the Rx data sits in it */
         WPI_TakeNextBdAddress(dummy_iwbd,
                               matrix_flag,
                               &next_bd_address,
                               host_offset_bd);

         if (next_bd_address != 0)
         {
            WP_status ret_val;
            /* pointer to the next BD */
            next_iwbd = (wpi_dps_iwbd *)WPI_HOST_PTR(next_bd_address, host_offset_bd);

            WPI_MEM_GET(next_v_bits, next_iwbd->v_bits);
            WPI_MEM_SET(next_iwbd->v_bits, next_v_bits &
                        (WPI_DPS_IWBD_VBITS_IWBT_M | WPI_DPS_IWBD_VBITS_FBP_M));
            WPI_MEM_SET(next_iwbd->x_bits, 0);

            /* return the dummy BD to its originating FBP */
            /* In matrix buffer chaining need not return second bd*/
            if(i != 1)
            {
               WPI_MEM_GET(v_bits, dummy_iwbd->v_bits);
               fbp_val = WPI_DPS_IWBD_VBITS_FBP_V(v_bits);

               ret_val = WPI_WmmFifoPut(wpid,
                                       WPI_MemoryBusOfVirtualBus(memory, bd_bus),
                                       fbp_val,
                                       dummy_bd_addr,
                                       (WP_U32 *)dummy_iwbd,
                                       WP_TRUE);
               if(ret_val != WP_OK)
               {
                  status = ret_val;
                  break;
               }
            }

            /*In matrix buffer chaining mode need to return to fifo last bd if there were more,
             than one bd*/
            if (next_v_bits & WPI_DPS_IWBD_VBITS_E_M)
            {
               fbp_val = WPI_DPS_IWBD_VBITS_FBP_V(next_v_bits);
               ret_val = WPI_WmmFifoPut(wpid,
                                       WPI_MemoryBusOfVirtualBus(memory, bd_bus),
                                       fbp_val,
                                       next_bd_address,
                                       (WP_U32 *)next_iwbd,
                                       WP_TRUE);
               if(ret_val != WP_OK)
                  status = ret_val;
               break;
            }
            i++;
         }
         else
            break;
      }
   }
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryQNodeRelease,
                               WPI_REG_UNLOCKED,
                               &iwqnode);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   return status;
}

/*****************************************************************************
 *
 * Function: WPI_MatrixChainingNextBd
 *
 * Purpose: Calculate second BD address in matrix chaining mode.
 *
 * Description:
 *     Function uses specific fields in matrix bd:
 *           DiffPtr - difference from first BD in packet.
 *           Sign - direction of difference.
 *     To calculate next BD address:
 *     If sign - 0: second BD address = first BD address + DiffPtr * 16
 *     If sign - 1: second BD address = first BD address + DiffPtr * 16
 *
 * Inputs:
 *     bd                 Pointer to matrix bd structure
 *
 * Outputs:
 *     next_bd_addr       Pointer to next bd address
 *
 * Called by:
 *      WPI_TakeNextBdAddress
 *
 ****************************************************************************/
void  WPI_MatrixChainingNextBd(wpi_dps_iwbd *bd,
                               WPI_PTR *next_bd_addr,
                               WP_U8 *host_offset_bd)
{
   WP_U32 x_bits;
   WP_U8 sign;
   WP_U32 diff;
   WP_U32 next;

   WPI_MEM_GET(x_bits, bd->x_bits);
   sign = WPI_DPS_MATRIX_BD_SIGN_V(x_bits);
   diff = WPI_DPS_MATRIX_BD_DIFFPTR_V(x_bits);
   if (sign == 0)
      next = ((WP_U32)bd) + (diff<<4);
   else
      next = ((WP_U32)bd) - (diff<<4);
   *next_bd_addr = WPI_WPI_PTR(next, host_offset_bd);
}

/*****************************************************************************
 *
 * Function: WPI_TakeNextBdAddress
 *
 * Purpose: Take next bd address in different ways for matrix buffer chaining mode
 *          end standart mode.
 *
 * Description:
 *      In matrix buffer chaining mode there is different way to calculate
 *       next BD address for all BDs except last BD in packet.
 *       matrix_flag used to sign this case
 *
 * Inputs:
 *     bd                 Pointer to matrix bd structure
 *     matrix_flag        Flag to sign case for calculating next BD address
 *                        in matrix chaining way
 * Outputs:
 *     next_bd_addr       Pointer to next bd address
 *
 * Called by:
 *      WPI_SwQueueHostReceive
 *
 ****************************************************************************/
void  WPI_TakeNextBdAddress(wpi_dps_iwbd *bd,
                            WP_boolean matrix_flag,
                            WPI_PTR *next_bd_addr,
                            WP_U8 *host_offset_bd)
{
   if( matrix_flag == WP_TRUE )
      WPI_MatrixChainingNextBd(bd, next_bd_addr, host_offset_bd);
   else
      WPI_MEM_GET(*next_bd_addr, bd->next_bd);
}


WP_status WPI_HostReceiveBd(WP_handle handle, WPI_PTR *head_bd_phy_addr_ptr)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status lock_status = WP_OK;
   wpi_res_channel_block *channel_block;
   wpi_res_channel *cptr;
   WP_U8 *head_bd_vir_addr;
   WPI_PTR head_bd_phy_addr;
   wpi_memory *memory;
   WP_U32 wpid;

   WPI_INTERNAL_FUNCTION();

   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   wpid = WPI_HANDLE_WPID(handle);

   /* Find the channel */
   channel_block = &wpi_registry_ptr->pwp_block[wpid].tx_channels;
   cptr = (wpi_res_channel *) &channel_block-> channel_data[WPI_HANDLE_INDEX(handle)].mod;
   if (cptr == NULL)
      return WPI_ERROR(WP_ERR_INUSE);

   /* Get the conversion from physical to virtual addresses*/
   memory = WPI_RegistryMemory(wpid);

   /* Look up the virtual address and convert it to a physical address */
   head_bd_vir_addr = (WP_U8 *) cptr->host_io;
   head_bd_phy_addr =
      WPI_WPI_PTR(head_bd_vir_addr, WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD));
   *head_bd_phy_addr_ptr = head_bd_phy_addr;

   return WP_OK;
}


/*****************************************************************************
 *
 * Function: WPI_IwLoggingHostReceive
 *
 * Purpose:  Gets a logged packet from the Logging output queue
 *
 * Description:
 *     Gets a Log BD from the Logging output queue and gives the
 *     user the Log buffer of that BD.
 *
 * Inputs:
 *     handle                handle of the Logging
 *     data_unit             data unit of type WP_DATA_IW_LOG
 *
 * Outputs:
 *     data_unit             pointer to the logged data unit
 *
 * Return Value:
 *     WP_OK                          - success
 *     WP_ERR_HANDLE                  - input handle isn't a Logging handle type
 *     WP_ERR_NOT_IMPLEMENTED         - invalid data type passed in
 *     WP_ERR_HST_INVALID_INPUT       - invalid data unit passed in
 *     WP_ERR_HST_ADJUNCT_POOL_EMPTY  - no free buffers in adjunct pool
 *     WP_ERR_HST_NO_DATA_TO_GET      - no logged packets in output queue
 *
 * Called by:
 *     WP_HostReceive
 *
 ****************************************************************************/
WP_status WPI_IwLoggingHostReceive(WP_handle handle,
                                  WP_data_unit *data_unit)
{
   WP_U32 wpid;
   wpi_res_iw_logging_info *iw_logging_info = NULL;
   WP_data_segment *segment = NULL;
   wpi_res_node *logq = NULL;
   wpi_res_pool *adjunct_pool = NULL;
   WPI_PTR bd_addr, dataptr, swap_buffer;
   WP_U32 bd_bus, output_fifo_index;
   WP_U32 v_bits;
   WP_U32 logging_fbp, vbank;
   WP_U8 *buffer, *host_offset_buffer, *host_offset_bd;
   wpi_dps_iwbd *log_bd;
   wpi_memory *memory;
   wpi_dps_logging_control_word *logging_control_word_ptr = NULL;
   WP_iw_log_data_return *log_data_return;
   WP_U8 log_type_indicator;
   WP_U16 unit_size = 0;
   WP_status status = WP_OK, lock_status = WP_OK;


   WPI_INTERNAL_FUNCTION();

   if (!WPI_HANDLE_IW_LOG_CHAN_P(handle))
   {
      return WPI_ERROR(WP_ERR_HANDLE);
   }

   if (data_unit == NULL ||
       data_unit->segment == NULL ||
       data_unit->data_return == NULL ||
       data_unit->n_segments == 0)
   {
      return WPI_ERROR(WP_ERR_HST_INVALID_INPUT);
   }

   if (data_unit->type != WP_DATA_IW_LOG)
   {
      return WPI_ERROR(WP_ERR_NOT_IMPLEMENTED);
   }

   wpid = WPI_HANDLE_WPID(handle);
   memory = WPI_RegistryMemory(wpid);
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryIwLoggingInfoGet,
                           WPI_REG_LOCKED,
                           &iw_logging_info,
                           wpid);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   segment = data_unit->segment;

   /* Get a buffer from the adjunct pool to be used for buffer swapping */
   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryQNodeFromPointerGet,
                           WPI_REG_LOCKED,
                           &logq,
                           iw_logging_info->qnode);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_REGISTRY_OBJECT_GET(lock_status,
                           WPI_RegistryPoolFromPointerGet,
                           WPI_REG_LOCKED,
                           &adjunct_pool,
                           logq->type.log.adjunct_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   swap_buffer = WPI_NpoolAlloc(adjunct_pool);

   if (swap_buffer == WPI_PTR_NULL)
   {
      return WPI_ERROR(WP_ERR_HST_ADJUNCT_POOL_EMPTY);
   }

   /* Get a Log BD from Logging output queue */
   bd_bus = WPI_MemoryBusOfVirtualBus(memory, WP_VB_FBP_BD);
   output_fifo_index = iw_logging_info->output_fifo;

   status = WPI_WmmFifoGet(wpid,
                           bd_bus,
                           output_fifo_index,
                           &bd_addr);
   if (bd_addr == WPI_PTR_NULL || status != WP_OK)
   {
      /* return buffer to pool */
      WPI_NpoolFree(adjunct_pool,
                    swap_buffer);

      return WPI_ERROR(WP_ERR_HST_NO_DATA_TO_GET);
   }

   /* We have the address of the Log BD in bd_addr */

   host_offset_bd = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_FBP_BD);
   log_bd = (wpi_dps_iwbd*)WPI_HOST_PTR(bd_addr, host_offset_bd);

   WPI_MEM_GET(dataptr, log_bd->data);
   WPI_MEM_GET(v_bits, log_bd->v_bits);

   vbank = adjunct_pool->vbank;
   unit_size = adjunct_pool->unit_size;

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryPoolRelease,
                               WPI_REG_UNLOCKED,
                               &adjunct_pool);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   host_offset_buffer = WPI_MemoryOffsetOfVbank(vbank);
   buffer = (WP_U8*)WPI_HOST_PTR(dataptr, host_offset_buffer);


   /* Populate the data unit and data segment with the BD info. */

   data_unit->n_active = 1;
   /* Modify the data size to not include the size of the log id */
   segment->data_size = data_unit->data_size =
      WPI_DPS_IWBD_VBITS_LEN_V(v_bits) - sizeof(wpi_dps_logging_control_word);

   /* The data pointer will be advanced after the log id field later */
   segment->data = buffer;
   segment->pool_handle = logq->type.log.adjunct_pool_handle;

   /* Get Logging FBP number from wpi_res_log_node */
   logging_fbp = logq->type.log.fbp;


   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryQNodeRelease,
                               WPI_REG_UNLOCKED,
                               &logq);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                               WPI_RegistryIwLoggingInfoRelease,
                               WPI_REG_UNLOCKED,
                               &iw_logging_info);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);

   /* Prepare the log_data_return structure */
   log_data_return = (WP_iw_log_data_return*)(data_unit->data_return);
   memset(log_data_return, 0, sizeof(*log_data_return));


   logging_control_word_ptr = (wpi_dps_logging_control_word*)buffer;
   WPI_MEM_BYTE_GET(log_type_indicator, logging_control_word_ptr->log_type_indicator);

   switch (log_type_indicator)
   {
      case WPI_LOG_ID_TYPE_FILTER:
      {
         /* Logging at filter's level.
          * Logging occurred due to a packet entering a Bridge IWS via an IW Port
          * that had logging enabled and the packet reached a filter's no-match deny action.
          * In order to get the filter's handle we need to obtain the Bridge IWS handle.
          * This is done using the IwSystemId saved in the S_DfcFilterLogId DPS struct.
          * We cannot use the iw_sys_handle which was obtained above since this handle
          * represents the IWS in which the IW Port resides and this port may belong
          * to a previous system in case System To System is configured.
          * Meaning, the IWS of iw_port_handle and the IWS in which the filter resides may
          * be different. The code will refer to the IWS in which the filter resides
          * (i.e., the one that denied the packet) as filter_iw_sys.
          */

         WP_U8 filter_index = 0;
         WP_U8 filter_iw_sys_index = 0;
         WP_U8 filter_iw_sys_subtype = 0;
	 WP_U16 log_id, iw_port_index;
	 WP_handle filter_iw_sys_handle = 0;

	 WPI_MEM_SHORT_GET(iw_port_index,
	                   logging_control_word_ptr->u_logging_info.dfc_log_id.iw_port_index);
	 /* Save the IW System handle in the log data return struct */
	 status = WPI_RegistryIwPortIwPortHandleGet(wpid,
                                              iw_port_index,
                                              &log_data_return->log_type_data.filter.iw_port_handle);
	 WPI_RETURN_IF_ERROR_STATUS(status);

	 /* Get the IW Port index and LogId from the logged buffer data */
	 WPI_MEM_SHORT_GET(log_id,
                           logging_control_word_ptr->u_logging_info.dfc_log_id.log_id);

	 /* Save the IW Port handle in the log data return struct */
	 status = WPI_RegistryIwPortIwSysHandleGet(wpid,
                                             iw_port_index,
                                             &log_data_return->log_type_data.filter.iw_sys_handle);
	 WPI_RETURN_IF_ERROR_STATUS(status);

         log_data_return->log_type = WP_IW_LOG_FILTER_TYPE;

         filter_iw_sys_index = WPI_DPS_IW_DFC_FILTER_LOG_ID_IW_SYS_N_V(log_id);
         filter_iw_sys_handle = WPI_MAKE_HANDLE(WPI_handle_iw_system,
                                                0,
                                                wpid,
                                                filter_iw_sys_index);

         status = WPI_RegistryIwBridgingVlanAwareMode(filter_iw_sys_handle,
                                                      &filter_iw_sys_subtype);
         WPI_RETURN_IF_ERROR_STATUS(status);

         /* finish creating the iws handle using its subtype */
         filter_iw_sys_handle = WPI_MAKE_HANDLE(WPI_handle_iw_system,
                                                filter_iw_sys_subtype,
                                                wpid,
                                                filter_iw_sys_index);

         /* Extract the filter index from the DPS LogId field */
         filter_index = WPI_DPS_IW_DFC_FILTER_LOG_ID_FILTER_N_V(log_id) - 1;

         /* Save the Filter handle in the log data return */
         status = WPI_RegistryIwDfcFilterHandleGet(filter_iw_sys_handle,
                                                   filter_index,
                                                   &log_data_return->log_type_data.filter.filter_handle);
         WPI_RETURN_IF_ERROR_STATUS(status);
      }
      break;

      case WPI_LOG_ID_TYPE_RULE:
      {
	 WP_U16 log_id, iw_port_index;

	 log_data_return->log_type = WP_IW_LOG_RULE_TYPE;

	 /* Extract the User Log ID from the DPS LogId field */
	 WPI_MEM_SHORT_GET(log_id, logging_control_word_ptr->u_logging_info.dfc_log_id.log_id);
	 log_data_return->log_type_data.rule.user_log_id = WPI_DPS_IW_DFC_RULE_LOG_ID_USER_LOG_ID_V(log_id);

	 WPI_MEM_SHORT_GET(iw_port_index,
	                   logging_control_word_ptr->u_logging_info.dfc_log_id.iw_port_index);
	 /* Save the IW Port handle in the log data return struct */
	 status = WPI_RegistryIwPortIwPortHandleGet(wpid,
                                              iw_port_index,
                                              &log_data_return->log_type_data.rule.iw_port_handle);
	 WPI_RETURN_IF_ERROR_STATUS(status);

	 /* Save the IW System handle in the log data return struct */
	 status = WPI_RegistryIwPortIwSysHandleGet(wpid,
                                             iw_port_index,
                                             &log_data_return->log_type_data.rule.iw_sys_handle);
	 WPI_RETURN_IF_ERROR_STATUS(status);
      }
      break;

      default:
         /* Code should never reach here.
          * log_type_indicator is always one of the 2 cases above. */
         break;
   }

   /* Advance the data pointer to point after the log id */
   segment->data += sizeof(wpi_dps_logging_control_word);

   /* Modify length field in v_bits before writing v_bits to BD */
   v_bits = (v_bits & ~WPI_DPS_IWBD_VBITS_LEN_M) | WPI_DPS_IWBD_VBITS_LEN_F(unit_size);

   /* Modify BD before returning it to FBP */
   WPI_MEM_SET(log_bd->data, swap_buffer);
   WPI_MEM_SET(log_bd->v_bits, v_bits);

   /* Put BD back to its original FBP */
   status = WPI_WmmFifoPut(wpid,
                           bd_bus,
                           logging_fbp,
                           bd_addr,
                           (WP_U32*)log_bd,
                           WP_FALSE);
   WPI_RETURN_IF_ERROR_STATUS(status);

   return status;
}
