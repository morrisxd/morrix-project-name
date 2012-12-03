/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpi_lookup.c
 *
 * Purpose: conatins memory operation functions
 *
 * Functions contained in this file:
 *
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |  WP_LookupCreate          |   Allocate memory for a Lookup table.
 * |  -------------------------+----------------------------------------------
 * |  WP_LookupDelete          |   Free the memory of the lookup table.
 * |  -------------------------+----------------------------------------------
 * |  WP_LookupEntryInsert     |   Insert the data according to the key.
 * |                           |   Lookup[index]= data.
 * |  -------------------------+----------------------------------------------
 * |  WP_LookupEntryRemove     |   Set the entry valid bit to none valid
 * |  -------------------------+----------------------------------------------
 * |  WP_LookupEntryGet        |   Return a pointer to the data according
 * |                           |    to the key.
 * |  -------------------------+----------------------------------------------
 * |  WP_LookupKeyFind         |   Return the in the found_index paramater
 * |                           |     the index of the first next hop.
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal WDDI Functions
 * |  -------------------------+----------------------------------------------
 * |  WPI_LookupCreate         |   Allocate memory for a Lookup table.
 * |  -------------------------+----------------------------------------------
 * |  WPI_LookupDelete         |   Free the memory of the lookup table.
 * |  -------------------------+----------------------------------------------
 * |  WPI_LookupEntryInsert    |   Insert the data according to the key.
 * |                           |   Lookup[index]= data.
 * |  -------------------------+----------------------------------------------
 * |  WPI_LookupEntryRemove    |   Set the entry valid bit to none valid
 * |  -------------------------+----------------------------------------------
 * |  WPI_LookupEntryGet       |   Return a pointer to the data according
 * |                           |    to the key.
 * |  -------------------------+----------------------------------------------
 * |  WPI_LookupKeyFind        |   Return the in the found_index paramater
 * |                           |     the index of the first next hop.
 * |  -------------------------+----------------------------------------------
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WPI_TYPES_H
#include "include/core/registry/wpi_types.h"
#endif
#ifndef WPI_WDDI_TYPEDEFS_H
#include "include/core/registry/wpi_wddi_typedefs.h"
#endif
#ifndef WP_WDDI_H
#include "api/wp_wddi.h"
#endif
#ifndef WPI_REG_H
#include "include/core/hardware/wpi_reg.h"
#endif
#ifndef WPI_ADAPTOR_H
#include "include/core/memory/wpi_adaptor.h"
#endif
#ifndef WPI_MEMORY_H
#include "include/core/memory/wpi_memory.h"
#endif
#ifndef WPI_UTILS_H
#include "include/core/utility/wpi_utils.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_DEVICE_HSPI_INT_H
#include "include/layer_2/packet/hspi/wpi_device_hspi_int.h"
#endif
#ifndef WPI_LOOKUP_H
#include "include/core/wpi_lookup.h"
#endif
#ifndef WP_IW_H
#include "api/wp_iw.h"
#endif
#ifndef WPI_VENEER_H
#include "veneer/wpi_veneer.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"     /* For WPI_ERROR */
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif

#include "include/core/wpi_hash_table.h"
#include "include/core/wpi_memory_class.h"
#include "include/core/wpi_system_class.h"

//#define WP_FLOWNUM_TO_RT_MAP_SIZE 0x40000

#define WP_UL_RT_BASE_ADDRESS 0x0
#define WP_UL_RT_MEMORY_SIZE 0x4080000 //0x1040000

#define WP_IPEP_BASE_ADDRESS (WP_UL_RT_BASE_ADDRESS + WP_UL_RT_MEMORY_SIZE) //0x1040000
#define WP_IPEP_MEMOREY_SIZE 0x2040000

#define WP_VPORT_BASE_ADDRESS (WP_IPEP_BASE_ADDRESS + WP_IPEP_MEMOREY_SIZE) //0x3080000
#define WP_VPORT_MEMOREY_SIZE 0x480000  //here we reach 0x3500000

#define WP_DL_RT_BASE_ADDRESS (WP_VPORT_BASE_ADDRESS + WP_VPORT_MEMOREY_SIZE) //0x3500000
#define WP_DL_RT_MEMOREY_SIZE 0x80000  //here we reach 0x3580000

#define WP_FLOW_STATIC_BASE_ADDRESS (WP_DL_RT_BASE_ADDRESS + WP_DL_RT_MEMOREY_SIZE) //0x3580000
//#define WP_FLOW_STATIC_MEMOREY_SIZE 0x80000  //here we reach 0x3600000
#define WP_FLOW_STATIC_MEMOREY_SIZE 0x600000  //here we reach 0x3600000

#define WP_PRIORITY_HOST_ADDRESS (WP_FLOW_STATIC_BASE_ADDRESS + WP_FLOW_STATIC_MEMOREY_SIZE)
#define WP_PRIORITY_HOST_MEMORY_SIZE 0x100

#define WP_PRIORIYT_MSG_FR_TYPE 0
#define WP_PRIORIYT_MSG_IP_TYPE 1

#define WP_IPEP_PORT_BASE 1024
/*BSSGP PDU type defenitions*/
#define	DL_UNITDATA                         0x00
#define	UL_UNITDATA                         0x01
#define	RA_CAPABILITY                       0x02
#define	DL_MBMS_UNITDATA                    0x04
#define	PAGING_PS                           0x06
#define	PAGING_CS                           0x07
#define	RA_CAPABILITY_UPDATE                0x08
#define	RA_CAPABILITY_UPDATE_ACK            0x09
#define	RADIO_STATUS                        0x0a
#define	SUSPEND                             0x0b
#define	SUSPEND_ACK                         0x0c
#define	SUSPEND_NACK                        0x0d
#define	RESUME                              0x0e
#define	RESUME_ACK                          0x0f
#define	RESUME_NACK                         0x10
#define	BVC_BLOCK                           0x20
#define	BVC_BLOCK_ACK                       0x21
#define	BVC_RESET                           0x22
#define	BVC_RESET_ACK                       0x23
#define	BVC_UNBLOCK                         0x24
#define	BVC_UNBLOCK_ACK                     0x25
#define	FLOW_CONTROL_BVC                    0x26
#define	FLOW_CONTROL_BVC_ACK                0x27
#define	FLOW_CONTROL_MS                     0x28
#define	FLOW_CONTROL_MS_ACK                 0x29
#define	FLUSH_LL                            0x2a
#define	FLUSH_LL_ACK                        0x2b
#define	LLC_DISCARDED                       0x2c
#define	FLOW_CONTROL_PFC                    0x2d
#define	FLOW_CONTROL_PFC_ACK                0x2e
#define	SGSN_INVOKE_TRACE                   0x40
#define	STATUS                              0x41
#define	DOWNLOAD_BSS_PFC                    0x50
#define	CREATE_BSS_PFC                      0x51
#define	CREATE_BSS_PFC_ACK                  0x52
#define	CREATE_BSS_PFC_NACK                 0x53
#define	MODIFY_BSS_PFC                      0x54
#define	MODIFY_BSS_PFC_ACK                  0x55
#define	DELETE_BSS_PFC                      0x56
#define	DELETE_BSS_PFC_ACK                  0x57
#define	DELETE_BSS_PFC_REQ                  0x58
#define	PS_HANDOVER_REQUIRED                0x59
#define	PS_HANDOVER_REQUIRED_ACK            0x5a
#define	PS_HANDOVER_REQUIRED_NACK           0x5b
#define	PS_HANDOVER_REQUEST                 0x5c
#define	PS_HANDOVER_REQUEST_ACK             0x5d
#define	PS_HANDOVER_REQUEST_NACK            0x5e
#define	PS_HANDOVER_COMPLETE                0x91
#define	PS_HANDOVER_CANCEL                  0x92
#define	PERFORM_LOCATION_REQUEST            0x60
#define	PERFORM_LOCATION_RESPONSE           0x61
#define	PERFORM_LOCATION_ABORT              0x62
#define	POSITION_COMMAND                    0x63
#define	POSITION_RESPONSE                   0x64
#define	RAN_INFORMATION                     0x70
#define	RAN_INFORMATION_REQUEST             0x71
#define	RAN_INFORMATION_ACK                 0x72
#define	RAN_INFORMATION_ERROR               0x73
#define	RAN_INFORMATION_APPLICATION_ERROR   0x74
#define	MBMS_SESSION_START_REQUEST          0x80
#define	MBMS_SESSION_START_RESPONSE         0x81
#define	MBMS_SESSION_STOP_REQUEST           0x82
#define	MBMS_SESSION_STOP_RESPONSE          0x83
#define	MBMS_SESSION_UPDATE_REQUEST         0x84
#define	MBMS_SESSION_UPDATE_RESPONSE        0x85
#define	BSSGP_PDU_TYPE        0x100

WP_U32 nri_bit_length = 0x6;

typedef struct 
{
   WP_U32 tlli_offset    :9;
   WP_U32 nri_offset    :7;
   WP_U32 bvci_offset  :8;
   WP_U32                     :6;
   WP_U32 upflow_bit    :1;
   WP_U32 valid_bit      :1;
}Wp_parse_bits;


#define WPI_MAX_DLRT_NUMBER 65530 /*maybe 0xfffe 0xffff used by hash collision*/
#define WPI_NO_FREE_DLRT 0xffffffff
#define WPI_ERR_NO_FREE -1

struct wpi_res_list
{
      WP_U32 next;
};
struct wpi_res_list_block
{
   WP_U32 head;
   WP_U32 tail;
   struct wpi_res_list *list_data;
};

struct wpi_res_list dlrt_table[WPI_MAX_DLRT_NUMBER] ={{0}};
struct wpi_res_list_block dlrt_list=
{
      .head = 0,
      .tail = 0,
      .list_data = dlrt_table,
};

#define WPI_GET_DLRT_HANDLE(handle, p) do { WP_U32 next; \
if (p->head != WPI_NO_FREE_DLRT) { \
handle = p->head; \
p->head = next = p->list_data[handle].next; \
if (next == WPI_NO_FREE_DLRT) p->tail = WPI_NO_FREE_DLRT; } \
else handle = WPI_ERR_NO_FREE; } while (0)

#define WPI_FREE_DLRT_HANDLE(handle,p) do { \
WP_U32 tail = p->tail; \
if (tail == WPI_NO_FREE_DLRT) { p->head = handle; } \
else {p->list_data[tail].next = handle; } \
p->tail = handle; p->list_data[handle].next = WPI_NO_FREE_DLRT; \
} while (0)



static WP_status WPI_RegistryFetchLKUpParams(WP_handle device,
                                             WP_U32 **reg_ptr);

#define WPI_VALID_BIT_MASK 0x80000000
#define WPI_NOPOOL_BIT_MASK 0x40000000
#define WPI_NSVC_REMOTE_BIT_MASK 0x20000000

#define WPI_MV_TO_OFFSET(host_ptr,key,entry_size) ((WP_U32 *)( host_ptr + (key)*(entry_size)))

static wpi_res_lookup *fr_reg_ptr = NULL;

void WP_BssGp_Parse( WP_U32 *bssgp_parse_result, WP_U32 type)
{
   WP_U32 value=0;
   Wp_parse_bits *parse_res = (Wp_parse_bits *)&value;
   
   parse_res->nri_offset = 8 - nri_bit_length;

   switch(type)
   {
         case UL_UNITDATA:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 1;
            parse_res->bvci_offset = 0;
            parse_res->tlli_offset = 5;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;

         case RA_CAPABILITY_UPDATE:
         //case RADIO_STATUS:
         case SUSPEND:
         case RESUME:
         case FLOW_CONTROL_MS:
         case FLOW_CONTROL_PFC:
         case DOWNLOAD_BSS_PFC:
         case CREATE_BSS_PFC_ACK:
         case CREATE_BSS_PFC_NACK:
         case MODIFY_BSS_PFC:
         case DELETE_BSS_PFC_ACK:
         case DELETE_BSS_PFC_REQ:
         case PS_HANDOVER_REQUIRED:
         case PS_HANDOVER_REQUEST_ACK:
         case PS_HANDOVER_REQUEST_NACK:
         case PS_HANDOVER_COMPLETE:
         case PS_HANDOVER_CANCEL:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 1;
            parse_res->bvci_offset = 0;
            parse_res->tlli_offset = 7;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;
            
         case LLC_DISCARDED:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 1;
            parse_res->bvci_offset = 16;
         //   parse_res->bvci_offset = 14;
            parse_res->tlli_offset = 7;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;
         case PERFORM_LOCATION_RESPONSE:
         case POSITION_COMMAND:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 1;
            parse_res->bvci_offset = 13;
         //   parse_res->bvci_offset = 11;
            parse_res->tlli_offset = 7;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;
         case DL_UNITDATA:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 0;
            parse_res->tlli_offset = 5;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;

         case RA_CAPABILITY:
         //case PAGING_PS:
         //case PAGING_CS:
         case RA_CAPABILITY_UPDATE_ACK:
         case FLOW_CONTROL_MS_ACK:
         case FLOW_CONTROL_PFC_ACK:
         case CREATE_BSS_PFC:
         case MODIFY_BSS_PFC_ACK:
         case DELETE_BSS_PFC:
         case PS_HANDOVER_REQUIRED_ACK:
         case PS_HANDOVER_REQUIRED_NACK:
         case PS_HANDOVER_REQUEST:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 0;
            parse_res->bvci_offset = 0;
            parse_res->tlli_offset = 7;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;
            
 /*        case DL_MBMS_UNITDATA:
         case FLOW_CONTROL_BVC_ACK:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 0;
            parse_res->bvci_offset = 0;
            parse_res->tlli_offset = 0;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;*/
            
         case PAGING_PS:
         case PAGING_CS:
            parse_res->valid_bit = 1;
            parse_res->upflow_bit = 0;
            parse_res->bvci_offset = 7;
        //    parse_res->bvci_offset = 5;
    //        parse_res->tlli_offset = 29;
            parse_res->tlli_offset = 0;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;
            
        default:
            parse_res->valid_bit = 0;
            parse_res->upflow_bit = 0;
            parse_res->bvci_offset = 0;
            parse_res->tlli_offset = 0;
            WPI_MEM_SET(bssgp_parse_result[type], value);
            break;
   }
   
}


static WP_status WPI_RegistryFetchLKUpParams(WP_handle device,
                                             WP_U32 **reg_ptr)
{
   //wpi_res_registry *wpi_registry_ptr = NULL;
   //WP_status lock_status = WP_OK;
  // wpi_resource_entry *dev_entry;
   //WP_U32 dev_index,devtype;
   //wpi_resource_block *resources;
   
   *reg_ptr =(WP_U32*) (WPI_FORCE_CAST) &fr_reg_ptr;
   return WP_OK;
#if 0
   WPI_INTERNAL_FUNCTION();
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   if (!WPI_HANDLE_DEVICE_P(device))
      return WPI_ERROR(WP_ERR_HANDLE);

   resources = &wpi_registry_ptr->pwp_resources[WPI_HANDLE_WPID(device)];
   dev_index = WPI_HANDLE_INDEX(device);
   WPI_ACCESS_RES(dev_entry, resources, dev_index);
   devtype = WPI_HANDLE_TYPE(device);

   switch(devtype)
   {
      case WP_DEVICE_HSPOS:
      {
         wpi_res_device_hspi_pos *dev_hspi_pos;
         dev_hspi_pos = WPI_RES_DEV_HSPI_POS(dev_entry);
         *reg_ptr =(WP_U32*) (WPI_FORCE_CAST) &(dev_hspi_pos->hspi.reg_ptr);
         break;
      }
     case WP_DEVICE_FR:
      {
         *reg_ptr =(WP_U32*) (WPI_FORCE_CAST) &fr_reg_ptr;
         break;
      }

      default:
         return WPI_ERROR(WP_ERR_HANDLE);
   }
   return WP_OK;
   #endif
}

/*****************************************************************************
 *
 * Function: WPI_LookupCreate
 *
 * Purpose: Allocate memory for Lookup table.
 *
 * Description:
 *       Allocate memory for Lookup table.
 *       The memory size equal to  num_of_entries * entry_size.
 *       Save the Lookup parameters in the registry.
 *       Save the lookup pointer in the registry.
 *
 * Inputs:
 *       wpi_res_lookup *reg_ptr - points on the entity
 *                                 to whom the lookup belong.
 *
 *
 *
 * Outputs:
 *       none.
 * Return Value:
 *     WP_OK - if the function ends successfully.
 *     WP_ERR_MEMORY_ALLOCATION  - ` if there is not enough memory.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPI_LookupCreate( wpi_res_lookup **reg_ptr)
{
   WP_U8 *host_offset;
   wpi_memory *memory;

   WPI_INTERNAL_FUNCTION();
   memory = WPI_RegistryMemory((*reg_ptr)->wpid);
   host_offset = WPI_MemoryOffsetOfVirtualBus(memory, (*reg_ptr)->vbank);
   (*reg_ptr)->base_ptr = (WPI_PTR)WPI_malloc(
      WPI_MemoryVbankOfVirtualBus(memory, (*reg_ptr)->vbank),
      //(*reg_ptr)->num_of_entries * (*reg_ptr)->entry_size 
      WP_UL_RT_MEMORY_SIZE
      + WP_IPEP_MEMOREY_SIZE
      + WP_VPORT_MEMOREY_SIZE
      + WP_DL_RT_MEMOREY_SIZE
      + WP_FLOW_STATIC_MEMOREY_SIZE
      + WP_PRIORITY_HOST_MEMORY_SIZE,
      (*reg_ptr)->entry_size,
      "lookup table"
      );
   if ( (*reg_ptr)->base_ptr == WPI_PTR_NULL)
      return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);

   (*reg_ptr)->host_ptr = (WP_U8 *)WPI_HOST_PTR((*reg_ptr)->base_ptr,
                                             host_offset);
   printf("(*reg_ptr)->base_ptr = 0x%x (*reg_ptr)->host_ptr = 0x%x\n",(*reg_ptr)->base_ptr,(*reg_ptr)->host_ptr);
   WPI_memclr((WP_U32 *)(*reg_ptr)->host_ptr,
              //(*reg_ptr)->num_of_entries * (*reg_ptr)->entry_size + WP_IPEP_MEMOREY_SIZE);
                    WP_UL_RT_MEMORY_SIZE
            + WP_IPEP_MEMOREY_SIZE
            + WP_VPORT_MEMOREY_SIZE
            + WP_DL_RT_MEMOREY_SIZE
            + WP_FLOW_STATIC_MEMOREY_SIZE
            + WP_PRIORITY_HOST_MEMORY_SIZE);


   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_LookupDelete
 *
 * Purpose: Free the memory of the Lookup table.
 *
 * Description:
 *       Translate the wincom pointer to virtual.
 *       Free the memory.
 *
 * Inputs:
 *       wpi_res_lookup *reg_ptr - points on the entity
 *                                 to whom the lookup belong.
 * Outputs:
 *       none.
 * Return Value:
 *     WP_OK - if the function ends successfully.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPI_LookupDelete(wpi_res_lookup *reg_ptr)
{
   wpi_memory *memory;
   WPI_INTERNAL_FUNCTION();
   memory = WPI_RegistryMemory(reg_ptr->wpid);

   return  WPI_free(WPI_MemoryVbankOfVirtualBus(memory, reg_ptr->vbank),
                    reg_ptr->base_ptr);
}

/*****************************************************************************
 *
 * Function: WPI_LookupEntryInsert
 *
 * Purpose: Insert the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 *          Set the entry.
 *          Switch the valid bit on.
 * Inputs:
 *       wpi_res_lookup *reg_ptr - points on the entity
 *                                 to whom the lookup belong.
 *       WP_U32 key - the index to the lookup table.
 *       WP_U8 *data - a pointer to the entry data.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPI_LookupEntryInsert(wpi_res_lookup *reg_ptr,
                                WP_U32 key,
                                WP_U32 flow_num,
                                WP_U32 nri,
                                 WP_U32 port_num)
{
//   WP_U32 i;
   WP_U32 *tmp_ptr;
   wp_globle_rt *rt_ptr;

   WPI_INTERNAL_FUNCTION();
   
   tmp_ptr = WPI_MV_TO_OFFSET(reg_ptr->host_ptr, 
                              key,
                              reg_ptr->entry_size);

   rt_ptr = (wp_globle_rt *)tmp_ptr;
 
  WPI_MEM_SET (rt_ptr->flow_num, 
                                    flow_num);

  port_num -= WP_IPEP_PORT_BASE;
  if(256 == nri)
  {
      port_num |= WPI_NOPOOL_BIT_MASK;
  }
  port_num |= WPI_VALID_BIT_MASK;
  
  WPI_MEM_SET (rt_ptr->port_num[nri], 
                              port_num);
  
   /*WPI_memcpy_pc_to_win((WP_U32 *) rt_ptr->local_rt[nri].ip_ep,
                        (WP_U32 *)local_rt->ip_ep,
                        local_rt->valid_num*sizeof(wp_ip_ep));*/
  
  //s cntrl |= WPI_VALID_BIT_MASK;

   //WPI_MEM_SET(tmp_ptr[0],cntrl);

   return WP_OK;
}


/*****************************************************************************
 *
 * Function: WP_LookupEntryInsert
 *
 * Purpose: Insert the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 *          Set the entry.
 *          Switch the valid bit on.
 * Inputs:
 *       WP_handle      device - returned by the WP_DeviceCreate
 *       WP_U32 elink - the index to the lookup table.
 *       WP_handle flow_agg_h A flow aggregation handle returned
 *                    by WP_IwFlowAggregationCreate.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/
WP_status WPI_DlLookupEntryInsert(wpi_res_lookup *reg_ptr,
                                WP_U32 key,
                                 wp_dl_rt *dl_rt)
{
 //  WP_U32 value;
   WP_U32 *tmp_ptr;
   wp_dl_rt *dl_ptr;
    
   WPI_INTERNAL_FUNCTION();

   tmp_ptr = WPI_MV_TO_OFFSET(reg_ptr->host_ptr 
                              + WP_DL_RT_BASE_ADDRESS,
                              key,
                              sizeof(wp_dl_rt));
//   printf("key = %d, \n", key0, key1, dlrt_num);
   dl_ptr = (wp_dl_rt *)tmp_ptr;
  WPI_MEM_SET (dl_ptr->flow_num, dl_rt->flow_num);

  WPI_MEM_SHORT_SET (dl_ptr->nsei_i, dl_rt->nsei_i);
  
  WPI_MEM_SHORT_SET (dl_ptr->bvci_n, dl_rt->bvci_n);
  
   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_LookupEntryRemove
 *
 * Purpose: Remove the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 * Inputs:
 *          wpi_res_lookup *reg_ptr - points on the entity
 *                                    to whom the lookup belong.
 *          WP_U32 key - the index to the lookup table.
 *          WP_U8 *data - a pointer to the entry data.
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPI_LookupEntryRemove(wpi_res_lookup *reg_ptr,
                                WP_U32 key,
                                WP_U32 nri)
{
   WP_U32 *tmp_ptr;
   wp_globle_rt *rt_ptr;
   WPI_INTERNAL_FUNCTION();
      
   tmp_ptr = WPI_MV_TO_OFFSET(reg_ptr->host_ptr, 
                              key,
                              reg_ptr->entry_size);

   rt_ptr = (wp_globle_rt *)tmp_ptr;

  WPI_MEM_SET (rt_ptr->port_num[nri],   0);

   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_LookupEntryRemove
 *
 * Purpose: Remove the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 * Inputs:
 *          wpi_res_lookup *reg_ptr - points on the entity
 *                                    to whom the lookup belong.
 *          WP_U32 key - the index to the lookup table.
 *          WP_U8 *data - a pointer to the entry data.
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPI_DlLookupEntryRemove(wpi_res_lookup *reg_ptr,
                                WP_U32 key)
{
   WP_U32 *tmp_ptr;
   wp_dl_rt *rt_ptr;
   WPI_INTERNAL_FUNCTION();
      
   tmp_ptr = WPI_MV_TO_OFFSET(reg_ptr->host_ptr + WP_DL_RT_BASE_ADDRESS,
                              key,
                              sizeof(wp_dl_rt));

   rt_ptr = (wp_dl_rt *)tmp_ptr;

  //WPI_MEM_SET (rt_ptr->nsei_i,   0);
  //WPI_MEM_SET (rt_ptr->bvci_n,   0);
  /*rizh should use short set*/
  WPI_MEM_SHORT_SET (rt_ptr->nsei_i,   0);
  WPI_MEM_SHORT_SET (rt_ptr->bvci_n,   0);

   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_LookupEntryGet
 *
 * Purpose: Get the data acording to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          set data to point on entry data.
 * Inputs:
 *          wpi_res_lookup *reg_ptr - points on the entity
 *                                    to whom the lookup belong.
 *          WP_U32 key - the index to the lookup table.
 *          WP_U32 *data - a pointer to the entry data.
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 *       WP_ERR_ELEMENT_NOT_FOUND - element not exist.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPI_LookupEntryGet(wpi_res_lookup *reg_ptr,
                             WP_U32 key,
                             WP_U32 nri,
                              WP_U32 *port_num)
{
   WP_U32 *tmp_ptr;
   wp_globle_rt *rt_ptr;
//   WP_U32 i;
   WPI_INTERNAL_FUNCTION();

   
   tmp_ptr = WPI_MV_TO_OFFSET(reg_ptr->host_ptr, 
                              key,
                              reg_ptr->entry_size);

    rt_ptr = (wp_globle_rt *)tmp_ptr;
 
   WPI_MEM_GET(*port_num,rt_ptr->port_num[nri]);
   /*
   WPI_memcpy_win_to_pc((WP_U32 *)rt_ptr->local_rt[nri].ip_ep, 
                                                (WP_U32 *)local_rt->ip_ep, 
                                                local_rt->valid_num*sizeof(wp_ip_ep) );
   
   for(i =0; i< local_rt->valid_num; i++)
   {
	   WPI_MEM_GET (local_rt->ip_ep[i].ip_addr,rt_ptr->local_rt[nri].ip_ep[i].ip_addr);
	   WPI_MEM_SHORT_GET (local_rt->ip_ep[i].src_port,rt_ptr->local_rt[nri].ip_ep[i].src_port);
	   WPI_MEM_SHORT_GET (local_rt->ip_ep[i].dst_port,rt_ptr->local_rt[nri].ip_ep[i].dst_port);
   }
 */
   

   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WPI_LookupKeyFind
 *
 * Purpose:  Return the index of the first next hop.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Search for the first entry where
 *             next_hop_index == lookup[key].next_hop_index starting
 *             from the index.
 * Inputs:
 *          wpi_res_lookup *reg_ptr - points on the entity
 *                                    to whom the lookup belong.
 *          WP_U32         index - starting search from that index.
 *          WP_U16         next_hop_index - flow aggregation index.
 *          WP_U32         *found_index - the first key of the entry
 *                         where  next_hop_index == lookup[key].next_hop_index
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 *       WP_ERR_ELEMENT_NOT_FOUND - element not exist.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WPI_LookupKeyFind(wpi_res_lookup *reg_ptr,
                            WP_U32 *found_index,
                            WP_U32 data)
{
   WP_U32 ii;
   WP_U32 *tmp_ptr=NULL;
   WP_U32 cntl,val;

   WPI_INTERNAL_FUNCTION();
   if(reg_ptr->num_of_entries < *found_index)
      return WPI_ERROR(WP_ERR_OUT_OF_RANGE);

   for(ii=*found_index;ii<reg_ptr->num_of_entries;ii++)
   {
      tmp_ptr = WPI_MV_TO_OFFSET(reg_ptr->host_ptr,
                                 ii,
                                 reg_ptr->entry_size);
      WPI_MEM_GET(val,tmp_ptr[reg_ptr->entry_size/4-1]);
      WPI_MEM_GET(cntl,tmp_ptr[0]);

      if(val == data  &&
        (cntl & WPI_VALID_BIT_MASK))
      {
         *found_index= ii;
         return WP_OK;
      }
   }

   return WPI_ERROR(WP_ERR_ELEMENT_NOT_FOUND);
}

/*****************************************************************************
 *
 * Function: WP_LookupCreate
 *
 * Purpose: Allocate memory for Lookup table.
 *
 * Description:
 *       Allocate memory for Lookup table.
 *       The memory size equal to  num_of_entries * entry_size.
 *       Save the Lookup parameters in the registry.
 *       Save the lookup pointer in the registry.
 *
 * Inputs:
 *       WP_handle      device - returned by the WP_DeviceCreate
 *
 * Outputs:
 *       none.
 * Return Value:
 *     WP_OK - if the function ends successfully.
 *     WP_ERR_MEMORY_ALLOCATION  -  if there is not enough memory.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_LookupCreate( WP_handle device,
                           WP_U32 num_of_entries,
                           WP_U32 entry_size)
{
   WP_status status, lock_status = WP_OK;
   wpi_res_lookup *res_params_ptr=NULL;
   WP_U32 *reg_ptr=NULL;
   WP_S32 type =BSSGP_PDU_TYPE;
   WP_U32 dev_type = WP_DEVICE_FR;
   WPI_WDDI_ATTACH();

   /* Return The Address of the pointer in the Registry.  */
    WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));

   res_params_ptr =(wpi_res_lookup *)
      WPI_WDDI_MALLOC(sizeof(wpi_res_lookup));

   if(res_params_ptr == NULL)
   {
       WPI_WDDI_DETACH();
       return WPI_ERROR(WP_ERR_MEMORY_ALLOCATION);
   }

   memset(res_params_ptr,0,sizeof(wpi_res_lookup));
   /* Set the pointer in the Registry to point on the parameters. */
   *reg_ptr =(WP_U32)res_params_ptr;

   /* Set the parameters in the registry .*/
   res_params_ptr->entry_size = entry_size;
   res_params_ptr->num_of_entries = num_of_entries;
   res_params_ptr->vbank = WP_VB_ATM_ADDLOOKUP;
   res_params_ptr->wpid = 0;//WPI_HANDLE_WPID(device);

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_LookupCreate((wpi_res_lookup **)reg_ptr));
   
   while(--type>= 0)
   {
      WP_BssGp_Parse( (WP_U32 *)res_params_ptr->host_ptr, type);
   }
   
   for(type=0; type<WPI_MAX_DLRT_NUMBER; type++)
   {
      WPI_FREE_DLRT_HANDLE(type, (&dlrt_list));
   }

   switch(dev_type)
   {
      case WP_DEVICE_FR:
      case WP_DEVICE_ENET:
      {
         wpi_res_reg_bases *res_reg_bases = NULL;
         wpi_ta *ta;

         WPI_REGISTRY_OBJECT_GET(lock_status,
                                 WPI_RegistryRegisterBaseGet,
                                 WPI_REG_LOCKED,
                                 &res_reg_bases,
                                 res_params_ptr->wpid);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);

         ta =WPI_TaRegBaseGet(res_reg_bases->reg_bases_ptr);
         //param is bus 7, packet-6 , internal -5, host -4 , application - 3 ... 0 is FMU tx...
         WPI_TaRegisterBSet(ta, WPI_TA_B_CUSTOMER1, res_params_ptr->base_ptr | 0x7);
         WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                                     WPI_RegistryRegisterBaseRelease,
                                     WPI_REG_UNLOCKED,
                                     &res_reg_bases);
         WPI_RETURN_IF_ERROR_STATUS(lock_status);
         break;
      }
      default:
         return WPI_ERROR(WP_ERR_HANDLE);
   }

   WPI_WDDI_DETACH();
   return status;
}





/*****************************************************************************
 *
 * Function: WP_LookupDelete
 *
 * Purpose: Free the memory of the Lookup table.
 *
 * Description:
 *       Translate the wincom pointer to virtual.
 *       Free the memory.
 *
 * Inputs:
 *       WP_handle      device - returned by the WP_DeviceCreate
 * Outputs:
 *       none.
 * Return Value:
 *     WP_OK - if the function ends successfully.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_LookupDelete(WP_handle device)
{
   WP_status status;
   WP_U32 *reg_ptr=NULL;
   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));
   status = WPI_LookupDelete((wpi_res_lookup *)*reg_ptr);

   WPI_WDDI_FREE((wpi_res_lookup*) *reg_ptr);

   WPI_WDDI_DETACH();
   return status;
}

//#if _WP_MODULE_FIGARO_ENABLE_
/*****************************************************************************
 *
 * Function: WP_LookupEntryInsert
 *
 * Purpose: Insert the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 *          Set the entry.
 *          Switch the valid bit on.
 * Inputs:
 *       WP_handle      device - returned by the WP_DeviceCreate
 *       WP_U32 elink - the index to the lookup table.
 *       WP_handle flow_agg_h A flow aggregation handle returned
 *                    by WP_IwFlowAggregationCreate.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/
WP_status WP_LookupEntryInsert(WP_handle device,
                               WP_handle elink,
                               WP_handle flow_agg_h[],
                               WP_U32 nri,
                                WP_U32 port_num)
{
   WP_status status;
   WP_U32 *reg_ptr=NULL;
    WP_U32 key = WPI_HANDLE_INDEX(elink);
    WP_U16 flow_num0 = WPI_HANDLE_INDEX(flow_agg_h[0]);//FIWT number cannot larger than 0xffff
    WP_U16 flow_num1 = WPI_HANDLE_INDEX(flow_agg_h[1]);
    
   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));

   

   status = WPI_LookupEntryInsert((wpi_res_lookup *)*reg_ptr,
                                  key,
                                  (flow_num1<<16) |flow_num0 ,//so we can use U32 to hold 2 flow number?
                                  nri,
                                  port_num);


   WPI_WDDI_DETACH();
   return status;
}


/*****************************************************************************
 *
 * Function: WP_LookupEntryInsert
 *
 * Purpose: Insert the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 *          Set the entry.
 *          Switch the valid bit on.
 * Inputs:
 *       WP_handle      device - returned by the WP_DeviceCreate
 *       WP_U32 elink - the index to the lookup table.
 *       WP_handle flow_agg_h A flow aggregation handle returned
 *                    by WP_IwFlowAggregationCreate.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/
WP_status WP_DlLookupEntryInsert(WP_handle device,
                               WP_handle elink,
                               WP_U16 key1,
                                wp_dl_rt *dl_rt)
{
   WP_status status;
   WP_U16 dlrt_num=0;
   WP_U32 *reg_ptr=NULL;
   WP_U32 key0 = WPI_HANDLE_INDEX(elink);
   
//   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));
   status = WPI_UserDefHashEntryDisplay(0, key0, key1,&dlrt_num);
   if(WP_ERR_NOT_EXIST_IN_HASH == status)
   {
       WPI_GET_DLRT_HANDLE(dlrt_num, (&dlrt_list));
	   if(dlrt_num == (WP_U16)WPI_ERR_NO_FREE)
       {
           //WPI_WDDI_DETACH();
           return WPI_ERROR(WP_ERR_OUT_OF_RANGE);
       }
	   //printf("hash not found key0 = %d, key1 = %d, dlrt_num =%d\n", key0, key1, dlrt_num);
   }
   //printf("key0 = %d, key1 = %d, dlrt_num =%d\n", key0, key1, dlrt_num);
   
//       WPI_WDDI_DETACH();
   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_UserDefHashEntryInsert(device, key0, key1,
                                  dlrt_num));
   
   WPI_WDDI_ATTACH();
   
   status = WPI_DlLookupEntryInsert((wpi_res_lookup *)*reg_ptr,
                                  dlrt_num,
                                  dl_rt);


   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function: WPI_LookupEntryInsert
 *
 * Purpose: Insert the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 *          Set the entry.
 *          Switch the valid bit on.
 * Inputs:
 *       wpi_res_lookup *reg_ptr - points on the entity
 *                                 to whom the lookup belong.
 *       WP_U32 key - the index to the lookup table.
 *       WP_U8 *data - a pointer to the entry data.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_IPEndPointEntryInsert(WP_handle device,
                                WP_U32 key,
                                 wp_local_rt *local_rt)
{
   WP_U32 i;
   WP_U32 *tmp_ptr;
   WP_U32 *reg_ptr=NULL;
   wp_local_rt *rt_ptr;

   WPI_RegistryFetchLKUpParams(device,
                            &reg_ptr);


   WPI_INTERNAL_FUNCTION();
   
   tmp_ptr = WPI_MV_TO_OFFSET(((wpi_res_lookup *)(*reg_ptr))->host_ptr 
                              + WP_IPEP_BASE_ADDRESS,
                              key - WP_IPEP_PORT_BASE,
                              sizeof(wp_local_rt));

   rt_ptr = (wp_local_rt *)tmp_ptr;
 
  //port_num |= WPI_VALID_BIT_MASK;
  
  WPI_MEM_SET (rt_ptr->valid_num, local_rt->valid_num);
  local_rt->valid_num&=0xffff;
  for(i =0; i< local_rt->valid_num; i++)
  {
      WPI_MEM_SET (rt_ptr->ip_ep[i].ip_addr, 
                                          local_rt->ip_ep[i].ip_addr);
      WPI_MEM_SHORT_SET (rt_ptr->ip_ep[i].src_port, 
                                                      local_rt->ip_ep[i].src_port);
      WPI_MEM_SHORT_SET (rt_ptr->ip_ep[i].dst_port,       
                                                      local_rt->ip_ep[i].dst_port);
  }

   /*WPI_memcpy_pc_to_win((WP_U32 *) rt_ptr->local_rt[nri].ip_ep,
                        (WP_U32 *)local_rt->ip_ep,
                        local_rt->valid_num*sizeof(wp_ip_ep));*/
  
  //s cntrl |= WPI_VALID_BIT_MASK;

   //WPI_MEM_SET(tmp_ptr[0],cntrl);

   return WP_OK;
}


/*****************************************************************************
 *
 * Function: WPI_LookupEntryInsert
 *
 * Purpose: Insert the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 *          Set the entry.
 *          Switch the valid bit on.
 * Inputs:
 *       wpi_res_lookup *reg_ptr - points on the entity
 *                                 to whom the lookup belong.
 *       WP_U32 key - the index to the lookup table.
 *       WP_U8 *data - a pointer to the entry data.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_VPortEntryInsert(WP_handle device,
                                WP_U16 key,
                                wp_vport_table *vport_table)
{
   WP_U32 i;
   WP_U32 valid_num;
   WP_U32 *tmp_ptr;
   WP_U32 *reg_ptr=NULL;
   wp_vport_table *vp_ptr;

   WPI_RegistryFetchLKUpParams(device,
                            &reg_ptr);


   WPI_INTERNAL_FUNCTION();
   
   tmp_ptr = WPI_MV_TO_OFFSET(((wpi_res_lookup *)(*reg_ptr))->host_ptr 
                              + WP_VPORT_BASE_ADDRESS,
                              key,
                              sizeof(wp_vport_table));

   vp_ptr = (wp_vport_table *)tmp_ptr;
  if(vport_table->valid_num == 0)
  {
      valid_num = 0;
  }
  else
  {
	  valid_num = vport_table->valid_num | WPI_VALID_BIT_MASK;
  }
  WPI_MEM_SET (vp_ptr->valid_num, valid_num);
  //WPI_MEM_SET (vp_ptr->curr_num, vport_table->curr_num);
  WPI_MEM_SET (vp_ptr->curr_num, 0);
  vport_table->valid_num&=0xffff;/*clean WPI_NSVC_REMOTE_BIT_MASK*/
  for(i =0; i < vport_table->valid_num; i++)
  {
      WPI_MEM_SET (vp_ptr->flow_num[i], 
                                          vport_table->flow_num[i]);
  }


   return WP_OK;
}

/*****************************************************************************
 *
 * Function: WP_LookupEntryRemove
 *
 * Purpose: Remove the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 * Inputs:
 *          WP_handle      device - returned by the WP_DeviceCreate
 *          WP_U32 elink - the index to the lookup table.
 *          WP_U8 *data - a pointer to the entry data.
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_LookupEntryRemove(WP_handle device,
                               WP_handle elink,
                               WP_U32 nri)
{
   WP_status status;
   WP_U32 *reg_ptr=NULL;
   WP_U32 key = WPI_HANDLE_INDEX(elink);
   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));

   status = WPI_LookupEntryRemove( (wpi_res_lookup *)*reg_ptr,
									   key,
									   nri);
   WPI_WDDI_DETACH();
   return status;
}


/*****************************************************************************
 *
 * Function: WP_LookupEntryRemove
 *
 * Purpose: Remove the data according to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Switch the valid bit off
 * Inputs:
 *          WP_handle      device - returned by the WP_DeviceCreate
 *          WP_U32 elink - the index to the lookup table.
 *          WP_U8 *data - a pointer to the entry data.
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_DlLookupEntryRemove(WP_handle device,
                               WP_handle elink,
                               WP_U16 key1)
{
   WP_status status;
   WP_U16 dlrt_num;
   WP_U32 *reg_ptr=NULL;
   WP_U32 key = WPI_HANDLE_INDEX(elink);

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));
   
   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_UserDefHashEntryDisplay(device, key, key1,
                                  &dlrt_num));
   //printf("hash remove key0 = %d, key1 = %d, dlrt_num =%d\n", key, key1, dlrt_num);
   WPI_WDDI_ATTACH();
   
   status = WPI_DlLookupEntryRemove( (wpi_res_lookup *)*reg_ptr,
									   dlrt_num);
   WPI_WDDI_DETACH();
   
   WPI_DETACH_RETURN_IF_ERROR(
      status,
   WPI_UserDefHashEntryRemove(device, key, key1, 
                                    dlrt_num));
   
   WPI_FREE_DLRT_HANDLE(dlrt_num, (&dlrt_list));
   
   return status;
}


/*****************************************************************************
 *
 * Function: WP_LookupEntryGet
 *
 * Purpose: Get the data acording to the key.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          set data to point on entry data.
 * Inputs:
 *          WP_handle      device - returned by the WP_DeviceCreate
 *          WP_U32 elink - the index to the lookup table.
 *          WP_U8 *data - a pointer to the entry data.
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 *       WP_ERR_ELEMENT_NOT_FOUND - element not exist.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_LookupEntryGet(WP_handle device,
                            WP_U32 elink,
                            WP_U32 nri,
                            WP_U32 *port_num)
{
   WP_status status;
   WP_U32 *reg_ptr=NULL;
    WP_U32 key = WPI_HANDLE_INDEX(elink);
   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));

   status = WPI_LookupEntryGet( (wpi_res_lookup *)*reg_ptr,
                                key,
                                nri,
                                port_num);

   WPI_WDDI_DETACH();
   return status;
}

/*****************************************************************************
 *
 * Function: WP_LookupKeyFind
 *
 * Purpose:  Return the index of the first next hop.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Search for the first entry where
 *             next_hop == lookup[key].next_hop starting
 *             from the index.
 * Inputs:
 *          WP_handle      device - returned by the WP_DeviceCreate
 *          WP_U32
 *          WP_U32         *found_elink -
 *                          starting search from that index.
 *                           The first key of the entry
 *                            where next_hop == lookup[key].next_hop
 *          WP_handle     The flow_aggregation we are looking for.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 *       WP_ERR_ELEMENT_NOT_FOUND - element not exist.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_LookupKeyFind(WP_handle device,
                           WP_U32 *found_elink,
                           WP_handle flow_aggregation_h)
{
   WP_status status;
   WP_U32 *reg_ptr=NULL;
   WP_U32 flow_agg_indx;

   WPI_WDDI_ATTACH();

   WPI_DETACH_RETURN_IF_ERROR(
      status,
      WPI_RegistryFetchLKUpParams(device,
                                  &reg_ptr));

   flow_agg_indx = WPI_HANDLE_INDEX(flow_aggregation_h);

   status = WPI_LookupKeyFind( (wpi_res_lookup *)*reg_ptr,
                               found_elink,
                               flow_agg_indx);

   WPI_WDDI_DETACH();
   return status;
}



/*****************************************************************************
 *
 * Function: WP_LookupKeyFind
 *
 * Purpose:  Return the index of the first next hop.
 *
 * Description:
 *          Check if the key is in the range.
 *          Translate the lookup wincom pointer to virtual pointer.
 *          Search for the first entry where
 *             next_hop == lookup[key].next_hop starting
 *             from the index.
 * Inputs:
 *          WP_handle      device - returned by the WP_DeviceCreate
 *          WP_U32
 *          WP_U32         *found_elink -
 *                          starting search from that index.
 *                           The first key of the entry
 *                            where next_hop == lookup[key].next_hop
 *          WP_handle     The flow_aggregation we are looking for.
 *
 * Outputs:
 *       none.
 * Return Value:
 *       WP_OK - if the function ends successfully.
 *       WP_ERR_OUT_OF_RANGE - the key is too big.
 *       WP_ERR_ELEMENT_NOT_FOUND - element not exist.
 * Called by:
 *
 *
 ****************************************************************************/

WP_status WP_FlowStatisticsGet(WP_handle device,
                           WP_U32 elink,
                           wp_ns_bssgp_statis * statis_ptr)
{
   WP_status status = 0;
   WP_U32 *reg_ptr=NULL;
    WP_U32 key = WPI_HANDLE_INDEX(elink);
    WP_U32 *tmp_ptr;
//    wp_ns_bssgp_statis *wp_ptr;
   WPI_WDDI_ATTACH();

   WPI_RegistryFetchLKUpParams(device,
                            &reg_ptr);


   WPI_INTERNAL_FUNCTION();
   
   tmp_ptr = WPI_MV_TO_OFFSET(((wpi_res_lookup *)(*reg_ptr))->host_ptr 
                              + WP_FLOW_STATIC_BASE_ADDRESS,
                              key,
                              sizeof(wp_ns_bssgp_statis));

   //printf("WP_FlowStatisticsGet key = %d", key);
  // wp_ptr = (wp_ns_bssgp_statis *)tmp_ptr;
 
  //port_num |= WPI_VALID_BIT_MASK;
  
  WPI_memcpy_win_to_pc((WP_U32 *)tmp_ptr, (WP_U32 *)statis_ptr,  sizeof(wp_ns_bssgp_statis));
  

   WPI_WDDI_DETACH();
   return status;
}

WP_status WP_FlowStatisticsClear(WP_handle device,
                           WP_U32 elink
                           )
{
   WP_status status = 0;
   WP_U32 *reg_ptr=NULL;
    WP_U32 key = WPI_HANDLE_INDEX(elink);
    WP_U32 *tmp_ptr;
//    wp_ns_bssgp_statis *wp_ptr;
   WPI_WDDI_ATTACH();

   WPI_RegistryFetchLKUpParams(device,
                            &reg_ptr);


   WPI_INTERNAL_FUNCTION();
   
   tmp_ptr = WPI_MV_TO_OFFSET(((wpi_res_lookup *)(*reg_ptr))->host_ptr 
                              + WP_FLOW_STATIC_BASE_ADDRESS,
                              key,
                              sizeof(wp_ns_bssgp_statis));

   //printf("WP_FlowStatisticsGet key = %d", key);
  // wp_ptr = (wp_ns_bssgp_statis *)tmp_ptr;
 
  //port_num |= WPI_VALID_BIT_MASK;
  
  //WPI_memcpy_win_to_pc((WP_U32 *)tmp_ptr, (WP_U32 *)statis_ptr,  sizeof(wp_ns_bssgp_statis));
  WPI_memclr((WP_U32 *)tmp_ptr,sizeof(wp_ns_bssgp_statis));

   WPI_WDDI_DETACH();
   return status;
}



WP_status WP_PriorityMsgFlowSet(WP_handle device,
                                WP_U16 type,/*FR or IP*/
                                wp_priority_msg *flowtable)
{
   WP_U16 i,key;
   WP_U32 *tmp_ptr;
   WP_U32 *reg_ptr=NULL;
   wp_priority_msg *ptr;

   WPI_RegistryFetchLKUpParams(device,
                            &reg_ptr);


   if(type == WP_PRIORIYT_MSG_FR_TYPE)
   {
      key = 0;
   }
   else
   {
      key = 1;
   }
   tmp_ptr = WPI_MV_TO_OFFSET(((wpi_res_lookup *)(*reg_ptr))->host_ptr 
                              + WP_PRIORITY_HOST_ADDRESS,
                              key,
                              sizeof(wp_priority_msg));

   ptr = (wp_priority_msg *)tmp_ptr;

   for(i = 0;i <4;i++)
   {
	   if(flowtable->flow_num[i] != 0)
	   {
		   WPI_MEM_SET (ptr->flow_num[i], WPI_VALID_BIT_MASK | WPI_HANDLE_INDEX(flowtable->flow_num[i]));

	   }
	   else
	   {
		   WPI_MEM_SET (ptr->flow_num[i], 0);
	   
	   }
   }

   return WP_OK;
}

WP_status WP_WDDI_ResFitwGetTag(WP_U32 fiwtIndex, WP_U32 *tag)
{   
    wpi_res_fiwt *res_fiwt =NULL;
	wpi_res_registry *wpi_registry_ptr = NULL;
	WP_status status = WP_OK, lock_status = WP_OK;
    wpi_res_fiwt_block *fiwt_block;
    WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
    WPI_RETURN_IF_ERROR_STATUS(lock_status);
	fiwt_block = &wpi_registry_ptr->pwp_block[0].fiwts;
	res_fiwt = &(fiwt_block->fiwt_data[fiwtIndex]);
	*tag = res_fiwt->tag;
	return status;
}
//#endif
