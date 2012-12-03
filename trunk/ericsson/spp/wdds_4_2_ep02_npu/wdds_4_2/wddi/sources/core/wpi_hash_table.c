/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpi_hash_table.c
 *
 * Purpose: MAC and Atm hash support
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
 * |  WP_IwMacAddressInsert    | Inserts a Mac entry to the hash.
 * |  WP_IwMacAddressRemove    | Removes a Mac entry from the hash.
 * |  WP_IwMacAddressModify    | Modifies a Mac entry to the hash.
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal WDDI Functions
 * |  -------------------------+----------------------------------------------
 * |  WPI_HashReadShort        |
 * |  WPI_HashReadLong         |
 * |  WPI_HashReadLong         |
 * |  WPI_AtmHashEntryInsert   | Inserts a Vp/Vc entry to hash.
 * |  WPI_AtmHashEntryRemove   | Removes a Vp/Vc entry from the hash.
 * |  WPI_AtmHashEntryFind     | Finds a Vp/Vc entry in the hash.
 * |  WPI_IwMacAddressFind     | Finds a Mac/Vp-Vc entry in the hash.
 * |  WPI_IwMacAddressInsert   | Inserts a Mac/Vp-Vc entry to the hash.
 * |  WPI_IwMacAddressRemove   | Removes a Mac/Vp-Vc entry from the hash.
 * |  WPI_IwPopToL2            | Folds L2 to L1.
 * |  WPI_CreateLinkList  | Creates a linked list when collision occurs in L2.
 * |  WPI_SearchLinkList       | Searches an entry in the linked list.
 * |  WPI_update_crc32         | Calculates crc-32 for a given 8 byte array.
 * |  WPI_structure_InsertAnalizer  | Performs statistics on the hash.
 * |  WPI_plot_structure       | Prints the hash current structure.
 * |  WPI_GetVpVcHashPtr       | Retieves the atm hash pointer.
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

/****************************************************************************
 *
 *  Data base description :
 *  ----------------------
 *  The data base is constructed from a dynamic hash table .
 *  It is build from 3 stages: level1 , level2 , linked-list.
 *
 *  1.level1 : A fixed hash table ,initilised by the program.
 *       Its' default size is 64K cells but can be changed by the user
 *       in the header file ("wp_hash_table.c") in the field :
 *       WPI_HASH_TABLE_SIZE_IN_BITS ,HASH_TABLE_SIZE.
 *       Both of them need to be changed.
 *       The location in the table is determined by a CRC32
 *       calculation of the MAC_Adress (MAC_H+MAC_L+Vlan).
 *
 * 2.level2 : A dynamic hash-table.
 *       When there is a collision in one location in level1 a local
 *       table is allocated.
 *       Both collided items are transferd to this new table and a pointer
 *       is put in level1 to point to this local table.
 *       The size of this local table is determined in the header file
 *       in the field : LOCAL_TABLE_SIZE. It is 8 by default.
 *
 * 3. linked-list : When there is a collision in level2 ( same modulus )
 *       a linked-list is generated. The to collided items are
 *       transfered to the linked list.
 *       Each additional item that statisfies the above demand
 *       will be linked to the end of this list.
 *       The list has no size limitations .
 *       !! During the transfer from L2 to the linked list
 *       the items change strucure : from : wpi_dps_hash ,to :link_list ,
 *       and a new field is added to the struct :LL_PTR,
 *       which acts as a "NEXT" to the next item in th list.
 *       The last item is always linked to NULL.
 *
 *
 *  A general glossary:
 *  ------------------------
 *
 *  collision - when to items (an exsisting item  and a new one )
 *              have the same calculated location ( CRC result or modulus )
 *              but have different MAC_H & MAC_L . This probloem is solved
 *              by allocation of a new place (L2 level or linked list).
 *
 *  match -when 2 items have the same MAC_H & MAC_L
 *         (hence they are mapped to the same location).When a match occours
 *         it is followed by a change of the Control Word.
 *         This feature can be used to change a Control Word to an
 *         exsisting item stored in the data base.
 *
 *  Enrty structue (and also structue in level 1 and level 2):
 *
 * +===============================================================+
 * |offset | 31                          16  15                   0|
 * +=======+=======================================================+
 * | 0x00  |                                                       |
 * +-------| <---------------- PTR - ----- ----------------------> |
 * | 0x02  |                                                       |
 * +=======+=======================================================+
 * | 0x04  |                                                       |
 * +-------| <---------------- MAC_H ----- ----------------------> |
 * | 0x06  |                                                       |
 * +=======+=======================================================+
 * | 0x08  | <-------------------- MAC_L ------------------------> |
 * +=======+=======================================================+
 * | 0x0a  | <---------------  out port ------ ------------------> |
 * +=======+=======================================================+
 * | 0x0c  | <--------------- MAC Control Word ------------------> |
 * +=======+=======================================================+
 *
 *
 * Flow description :
 * ------------------
 *
 * - The first step is building the main structure - the level 1
 *   hash table and initiising it.
 * - After buliding ,the structure is user defined , according to
 *   his (her) inserts and deletes.
 * - Both INSERT and DELETE can be described in a flow chart with
 *   several final states.
 *   These states define the state of the data structure and the
 *   action applied on it.
 *
 *
 * INSERT:
 * -------
 * INS1 : Insertion of a new block to the first level (L1),
 *        hence ,no collision.
 *
 * INS2 : Insertion of a new block to an exsisting level2 (L2).
 *         It must follow a collision in CRC calculation in level1.
 *
 * INS3 : Change of control word in linked list.
 *        It follows : - collision in level1
 *                     - collision in level2 ( same modulus )
 *                     - match with an exsisting item in the
 *                       linked list (same MAC_H & MAC_L)
 *
 * INS4 : Insertion of a new item to the end of the link list.
 *        It follows : - collision in level1
 *                     - collision in level2 ( same modulus )
 *                     - no match in the linked list
 *
 * INS5 : Change of control word in level2
 *        It follows : - collision in level1
 *                     - match with an item in level2
 *
 * INS6 : Creation of a linked list .
 *        It follows : - collision in level1
 *                     - collision in level2 when there isn't a linked
 *                       list concatenated to this location.
 *
 * INS7 : Change of control word in level1 ( main hash table )
 *        It follows : - collision in level1
 *                     - match with the item stored at this location
 *
 * INS8 : Creation of level2 and a linked list
 *        It follows : - collision in level 1 when there isn't a level2
 *                       concatenated to this location.
 *                     - same modulus result to both collided items :
 *                       the new entry and the item that was stored in level1.
 *
 * INS9 : Creation of a new level2 .
 *        It follows :- collision in level1 when there isn't a level2
 *                      concatenated to it yet
 *
 *
 *
 * DELETE :
 * --------
 *
 * DEL1 : A search error. An attempt to delete a non exsisting item
 *        from level1.(when there isn't any item in the calculated location).
 *
 * DEL2 : A search error. An attempt to delete a non exsisting item
 *        from level2.(when there isn't any item in the calculated location).
 *
 * DEL3 : Deletion of the an item from the linked list and folding that
 *        Linked List and its' anscesting L2.
 *        It follows : - only two items left in the linked list.
 *                     - no items in the anscesting L2.
 *
 * DEL4 : Deletion of the an item from the linked list and folding
 *        that Linked List:
 *        It follows : - only two items left in the linked list.
 *                     - one or more items in anscesting L2 apart
 *                       from the base of the linked list.
 *
 * DEL5 : Deletion of one item from the linked list.
 *        It follows : - the linked list containes three or more items
 *                       (including the removed item).
 *
 * DEL6 : A search error. An attempt to delete a non exsisting item
 *        from the linked list,(when there isn't any item in the
 *        calculated location).
 *
 * DEL7 : Deletion of an item from L2 ,and folding of this L2.
 *        It follows : - only 2 items in that level2
 *        (including the removed one).
 *
 * DEL8 : Deletion of a block from L2.
 *        It follows : - three or more items in L2
 *                       (including the removed one).
 *
 * DEL9 : A search error. An attempt to delete a non exsisting
 *        item from from level2,(but with another item
 *         in the calculated location).
 *
 * DEL10 : Deletion of one item from L1 ,hence ,no level2
 *         concatenated to this location.
 *
 * DEL11 : A search error. An attempt to delete a non exsisting item
 *         from from level1 ,(but with another item in the
 *         calculated location).
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
#ifndef WPI_UTILS_H
#include "include/core/utility/wpi_utils.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"
#endif
#ifndef WPI_SERVICES_H
#include "include/core/utility/wpi_services.h"
#endif
#ifndef WPI_HASH_TABLE_INT_H
#include "include/core/wpi_hash_table_int.h"
#endif
#ifndef WPI_IW_BRIDGING_H
#include "include/iw/bridging/wpi_iw_bridging.h"
#endif
#ifndef WPI_DEVICE_FR_INT_H
#include "include/layer_2/packet/fr/wpi_device_fr_int.h"
#endif
#ifndef WPI_DEVICE_ENET_INT_H
#include "include/layer_2/packet/enet/wpi_device_enet_int.h"
#endif
#ifndef WPI_DEVICE_HSPI_INT_H
#include "include/layer_2/packet/hspi/wpi_device_hspi_int.h"
#endif
#ifndef WPI_HS_ENET_INT_H
#include "include/layer_2/packet/hs_enet/wpi_hs_enet_int.h"
#endif
#ifndef WPI_VENEER_H
#include "veneer/wpi_veneer.h"
#endif
#ifndef WP_WDDI_H
#include "api/wp_wddi.h"
#endif
#ifndef WPI_REG_H
#include "include/core/hardware/wpi_reg.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_IW_ROUTING_H
#include "include/iw/routing/wpi_iw_routing.h"
#endif
#ifndef WPI_HASH_TABLE_H
#include "include/core/wpi_hash_table.h"
#endif
#ifndef WPI_IW_H
#include "include/iw/core/wpi_iw.h"
#endif
#ifndef WPI_IW_INT_H
#include "include/iw/core/wpi_iw_int.h"
#endif
#ifndef WPI_IW_DPS_MEM_FREE_MANAGER_H
#include "include/iw/core/wpi_iw_dps_mem_free_manager.h"
#endif
#ifndef WPI_IW_BRIDGING_INT_H
#include "include/iw/bridging/wpi_iw_bridging_int.h"
#endif
#ifndef WPI_IW_PWE3_H
#include "include/iw/pwe3/wpi_iw_pwe3.h"
#endif
#ifndef WPI_IMA_INT_H
#include "include/layer_2/atm/ima/wpi_ima_int.h"
#endif
#ifndef WPI_DEVICE_ENET_INT_H
#include "include/layer_2/packet/enet/wpi_device_enet_int.h"
#endif
#ifndef WPI_DEBUG_INT_H
#include "include/core/wpi_debug_int.h"
#endif
#ifndef WPI_STATIC_H
#include "include/core/registry/wpi_static.h"
#endif

#include "include/core/wpi_memory_class.h"
#include "include/layer_2/atm/ima/wpi_ima_class.h"
#include "include/layer_2/atm/ima/wpi_ima_private.h"
#include "include/layer_2/atm/wpi_atm_class.h"
#include "include/iw/core/wpi_iw_global_class.h"
#include "include/layer_2/atm/gbond/wpi_gbond_class.h"

#define WPI_IW_UNKNOWN_BPORT 0xffff
#ifdef STATISTICS
WP_U32 insert_count;
WP_U32 CTRL_change_count;
#endif


void  *UserDefHashBase = NULL;

static WP_U8 wpi_hash_size_power_of_two[WP_TOTAL_HASH_CONFIGURATIONS] = {
   /* WP_32_HASH_SIZE */   5,
   /* WP_64_HASH_SIZE */   6,
   /* WP_128_HASH_SIZE */  7,
   /* WP_256_HASH_SIZE */  8,
   /* WP_512_HASH_SIZE */  9,
   /* WP_1K_HASH_SIZE */   10,
   /* WP_2K_HASH_SIZE */   11,
   /* WP_4K_HASH_SIZE */   12,
   /* WP_8K_HASH_SIZE */   13,
   /* WP_16K_HASH_SIZE */  14,
   /* WP_32K_HASH_SIZE */  15,
   /* WP_64K_HASH_SIZE */  16,
   /* WP_128K_HASH_SIZE */ 17,
   /* WP_256K_HASH_SIZE */ 18,
   /* WP_512K_HASH_SIZE */ 19,
   /* WP_1M_HASH_SIZE, */  20
};

#define WPI_KEY_IS_EXIST(addr,entry,s) \
                          ( (s != WPI_IW_HASH_MEMBERSET_INSERT && \
                             s != WPI_IW_HASH_MEMBERSET_REMOVE) * \
                           ( R_MAC_H (addr) == entry->mac_h \
                             && R_MAC_L (addr) == entry->mac_l \
                             && R_VLAN_ID (addr) == entry->vlan_id ) | \
                           (s == WPI_IW_HASH_MEMBERSET_INSERT || \
                            s == WPI_IW_HASH_MEMBERSET_REMOVE ) * \
                           ( R_MAC_L (addr) == entry->vlan_id \
                             && R_VLAN_ID (addr) == entry->out_port )) \

#define WPI_CLEAR_HASH_ENTRY(x,m) \
               if (MAC_MODE(m))\
                  W_MAC_CNTLS (x, WPI_MAC_NON_VALID);\
               if (ATM_MODE (m))\
                  W_MAC_CNTLS (x, WPI_ATM_NON_VALID);\
               W_MAC_H (x, 0);\
               W_MAC_L (x, 0);\
               W_VLAN_ID (x, 0);\
               W_OUT_PORT (x, 0);\
               W_PTR(x,0); \

#define WPI_CURRENT_MAC_PER_BPORT_DEC(ptr,s,m,found_match,wpid)         \
do                                                                      \
{                                                                       \
   if((s == WPI_IW_FORWARDING_HASH_REMOVE ||                            \
       s == WPI_IW_FORWARDING_HASH_INSERT)  &&                          \
      m == WPI_MAC_HASH_LOOKUP_MODE &&                                  \
      found_match == WP_TRUE)                                           \
   {                                                                    \
      wpi_res_iw_port *res_iw_port = NULL;                              \
      wpi_iw_bases *iw_bases = NULL;                                    \
      WP_U16 iw_port_index = R_OUT_PORT(*ptr);                          \
      WP_U16 stat=0;                                                    \
      WPI_REGISTRY_OBJECT_GET(lock_status,                              \
                              WPI_RegistryIwPortGet,                    \
                              WPI_REG_LOCKED,                           \
                              &res_iw_port,                             \
                              wpid,                                     \
                              iw_port_index);                           \
      WPI_RETURN_IF_ERROR_STATUS(lock_status);                          \
      if(res_iw_port->info.bridge==NULL)                                \
         return WPI_ERROR(WP_ERR_CONFIG);                               \
      if(res_iw_port->info.bridge->cur_mac_addresses>0)                 \
         res_iw_port->info.bridge->cur_mac_addresses--;                 \
      if(res_iw_port->info.bridge->cur_mac_addresses!=                  \
         res_iw_port->info.bridge->max_mac_addresses)                   \
      {                                                                 \
         iw_bases = WPI_RegistryIwGlobalIwBasesGet(wpid);                     \
         WPI_MEM_SHORT_GET(                                                   \
            stat,(WPI_GET_BPORT_STAT(iw_bases, iw_port_index))->ctrl_bits);   \
         stat &= ~WPI_DPS_BPORT_INFO_CTRL_DISCARD_UNK_SA_M;                   \
         WPI_MEM_SHORT_SET(                                                   \
            (WPI_GET_BPORT_STAT(iw_bases, iw_port_index))->ctrl_bits,stat);   \
      }                                                                 \
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,                          \
                                  WPI_RegistryIwPortRelease,            \
                                  WPI_REG_UNLOCKED,                     \
                                  &res_iw_port);                        \
      WPI_RETURN_IF_ERROR_STATUS(lock_status);                          \
   }                                                                    \
} while(0);
   
#define WPI_WRITE_HASH_ENTRY(x,y,s) \
        if(s != WPI_IW_HASH_MEMBERSET_INSERT && \
           s != WPI_IW_HASH_MEMBERSET_REMOVE) \
                                  { \
                                    W_MAC_H(*x, y->mac_h); \
                                    W_MAC_L(*x, y->mac_l); \
                                    W_VLAN_ID(*x, y->vlan_id) ; \
                                    W_OUT_PORT(*x,y->out_port) ; \
                                    W_MAC_CNTLS(*x, y->cntrl);\
                                  } \
                                  else \
                                  { \
                                    W_MAC_H(*x,0); \
                                    W_MAC_L(*x, y->vlan_id); \
                                    W_VLAN_ID(*x, y->out_port) ; \
                                    W_OUT_PORT(*x,0) ; \
                                    W_MAC_CNTLS(*x, y->cntrl);\
                                  } \
                                  W_PTR(*x,y->ptr);\

#define WPI_CURRENT_MAC_PER_BPORT_INC(entry,s,m,wpid)                   \
do                                                                      \
{                                                                       \
   if(s == WPI_IW_FORWARDING_HASH_INSERT &&                             \
      m == WPI_MAC_HASH_LOOKUP_MODE  )                                  \
   {                                                                    \
      wpi_res_iw_port *res_iw_port = NULL;                              \
      wpi_iw_bases *iw_bases = NULL;                                    \
      WP_U16 iw_port_index = entry->out_port;                           \
      WP_U16 stat=0;                                                    \
      WPI_REGISTRY_OBJECT_GET(lock_status,                              \
                              WPI_RegistryIwPortGet,                    \
                              WPI_REG_LOCKED,                           \
                              &res_iw_port,                             \
                              wpid,                                     \
                              iw_port_index);                           \
      WPI_RETURN_IF_ERROR_STATUS(lock_status);                          \
      if(res_iw_port->info.bridge->cur_mac_addresses<                   \
         res_iw_port->info.bridge->max_mac_addresses)                   \
      {                                                                 \
         res_iw_port->info.bridge->cur_mac_addresses++;                 \
         if(res_iw_port->info.bridge->cur_mac_addresses==               \
            res_iw_port->info.bridge->max_mac_addresses)                \
         {                                                              \
            iw_bases = WPI_RegistryIwGlobalIwBasesGet(wpid);            \
            WPI_MEM_SHORT_GET(                                                   \
               stat,(WPI_GET_BPORT_STAT(iw_bases, iw_port_index))->ctrl_bits);   \
            stat |= WPI_DPS_BPORT_INFO_CTRL_DISCARD_UNK_SA_M;                    \
            WPI_MEM_SHORT_SET(                                                   \
               (WPI_GET_BPORT_STAT(iw_bases, iw_port_index))->ctrl_bits,stat);   \
         }                                                              \
      }                                                                 \
      else                                                              \
         return WPI_ERROR(WP_ERR_BPORT_MAX_MAC);                        \
      WPI_REGISTRY_OBJECT_RELEASE(lock_status,                          \
                                  WPI_RegistryIwPortRelease,            \
                                  WPI_REG_UNLOCKED,                     \
                                  &res_iw_port);                        \
      WPI_RETURN_IF_ERROR_STATUS(lock_status);                          \
   }                                                                    \
} while (0);

#define WPI_PTR_CNTRL_UPDATE(x,y) \
do{ \
   WP_U32 val; \
   WPI_MEM_GET(val,(x)->ptr); \
   val = (val & 0xfffffff0) | (R_PTR(y) & 0xf); \
   WPI_MEM_SET((x)->ptr,val);\
}while(0)

#define WPI_PTR_USER_CNTRL_UPDATE(x,y) \
do{ \
   WP_U32 val; \
   WPI_MEM_GET(val,(x)->ptr); \
   val = (val & 0xfffffff0) | ((y->ptr) & 0xf); \
   WPI_MEM_SET((x)->ptr,val);\
}while(0)

#define WPI_CLR_PTR(x) \
do{ \
   WP_U32 val; \
   WPI_MEM_GET(val,(x)->ptr); \
   val &= 0xf; \
   WPI_MEM_SET((x)->ptr,val);\
}while(0)

#define WPI_COPY_HASH_ENTRY(x,y) \
                  W_MAC_H(x, R_MAC_H(y)); \
                  W_MAC_L(x, R_MAC_L (y)); \
                  W_VLAN_ID(x, R_VLAN_ID(y)) ; \
                  W_OUT_PORT(x,R_OUT_PORT(y)) ; \
                  WPI_PTR_CNTRL_UPDATE(&(x),y); \
                  W_MAC_CNTLS(x, R_MAC_CNTLS(y));\
                  WPI_CLR_PTR(&(x)); \

#define WPI_USR_COPY_HASH_ENTRY(x,y) \
                  W_OUT_PORT(x,R_OUT_PORT(y)) ; \

#define WPI_UPDATE_HASH_ENTRY(x,y,s,wpid) \
                      WPI_PTR_USER_CNTRL_UPDATE(x,y); \
                      if(s != WPI_IW_HASH_MEMBERSET_INSERT && \
                         s != WPI_IW_HASH_MEMBERSET_REMOVE){ \
                        W_OUT_PORT(*x,y->out_port) ; \
                        W_MAC_CNTLS(*x, y->cntrl);\
                          } \
                      else \
                        W_MAC_CNTLS(*x, y->cntrl);\

WP_U16 WPI_HashReadShort (void *host_ptr)
{
  WP_U16 ret_short = 0;
  WPI_MEM_SHORT_GET (ret_short, *((WP_U16 *) host_ptr));
  return (ret_short);
}

WP_U32 WPI_HashReadLong (void *host_ptr)
{
  WP_U32 value = 0;
  WPI_MEM_GET (value, *((WP_U32 *) host_ptr));
  return value;
}

WP_U32 WPI_HashReadLongPtr (WP_U32 wpid, void *host_ptr)
{
  WP_U32 value = 0;
  WPI_MEM_GET (value, *((WP_U32 *) host_ptr));
  if( value != 0 )
  value = (WP_U32) WPI_get_host_from_wincom (wpid, WP_VB_HASH_IW_LPM, value);
  return value;
}

WP_status WPI_GetHashParams(WP_handle iw_system,
                            wpi_dps_hash **user_mac_entry,
                            wpi_dps_hash **level1_ptr,
                            WP_U8 *hash_mode,
                            WP_U32 *full_offset,
                            WP_U16 *L2_place,
                            WP_U8 search_mode,
                            void *hash_handler_object)
{
   WP_U32 CRC_offset;
   WP_U8 hash_input[8];
   WP_U32 wpid;
   WP_U32 iw_sysid;
   WPI_PTR ptr;
   WP_U32 mask=0;
   wpi_res_pwe_hash *res_pwehash = NULL;
   wpi_res_iw_bridging *bridging_node=NULL;

   WPI_INTERNAL_FUNCTION();

   *level1_ptr = (wpi_dps_hash *)0;
   WPI_LOG_MT (WPI_DEBUG_LEVEL_3, "WPI_GetHashParams() ");

   switch (WPI_HANDLE_TYPE (iw_system))
   {
      case WPI_handle_iw_system:
      {
        wpid = WPI_HANDLE_WPID (iw_system);
        iw_sysid = WPI_HANDLE_INDEX (iw_system);
        bridging_node = (wpi_res_iw_bridging *)hash_handler_object;
        *hash_mode = WPI_MAC_HASH_LOOKUP_MODE;

        if (WPI_HANDLE_SUBTYPE(iw_system) == WP_IW_TRANS_BRIDGING_MODE
           || WPI_HANDLE_SUBTYPE(iw_system) == WP_IW_VLAN_AWARE_BRIDGING_MODE)
        {
           if(search_mode == WPI_IW_HASH_MEMBERSET_INSERT ||
              search_mode == WPI_IW_HASH_MEMBERSET_REMOVE )
           {
              mask = (1<<(bridging_node->member_set_size+WPI_HASH_SIZE_SHIFT)) - 1;
              WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
                         "WPI_IwAlloc: memberset_table mask :%x\n",
                         mask);
              ptr = bridging_node->memberset_ptr;
              ptr = ptr & 0xfffffff0;
              *level1_ptr =(wpi_dps_hash *)
                 WPI_get_host_from_wincom(wpid,WP_VB_HASH_IW_LPM,ptr);
              WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                          "Member Set Base = %x \n",(WP_U32)ptr);
           }
           else if(search_mode == WPI_IW_FORWARDING_HASH_INSERT ||
                   search_mode == WPI_IW_FORWARDING_HASH_REMOVE ||
                   search_mode == WPI_IW_HASH_SEARCH )
           {
              mask = (1<<(bridging_node->forwarding_table_size+WPI_HASH_SIZE_SHIFT)) - 1;
              WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
                         "WPI_IwAlloc: forwarding_table mask :%x\n",
                         mask);
              ptr = bridging_node->hash_ptr;
              ptr = ptr & 0xfffffff0;
              *level1_ptr =(wpi_dps_hash *)
                 WPI_get_host_from_wincom(wpid,WP_VB_HASH_IW_LPM,ptr);
              WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                          "Forwarding Hash Base = %x \n",(WP_U32)ptr);
           }
        }
      }
      break;
      case WPI_handle_ima:
      case WPI_handle_port:
      case WPI_handle_gbond_group:
      {
         wpi_atm *atm = (wpi_atm *)hash_handler_object;
         wpid = WPI_HANDLE_WPID (iw_system);
         *hash_mode = WPI_VPI_VCI_HASH_LOOKUP_MODE;
         mask = WPI_HASH_TABLE_BLOCK_SIZE - 1;
         *level1_ptr = WPI_GetVpVcHashPtr(iw_system, wpid, atm);
      }
      break;
      case WPI_handle_iw_aggregation:
      {
         /* used for PWE3 ATM hash lookup */
           if (WPI_HANDLE_SUBTYPE(iw_system) == WP_IW_PWE3_MODE)
           {
              wpi_iw_global *iw_global = NULL;
              iw_global = (wpi_iw_global *)hash_handler_object;
              wpid = WPI_HANDLE_WPID (iw_system);
              res_pwehash = iw_global->pwehash;
              *hash_mode = WPI_VPI_VCI_HASH_LOOKUP_MODE;
              mask = res_pwehash->egress_hash_table_size - 1;
              *level1_ptr = res_pwehash->host_egress_hash_table_base;
           }
           else
              return WPI_ERROR (WP_ERR_HANDLE);
      }
      break;
      case WPI_handle_device:
      {
         wpi_resource_entry *dev_entry = NULL;
         dev_entry = (wpi_resource_entry *)hash_handler_object;
         switch( WPI_HANDLE_SUBTYPE(iw_system))
         {
            case WPI_pg_pkt_enet1:
            case WPI_pg_pkt_enet2:
            case WPI_pg_pkt_enet3:
            case WPI_pg_pkt_enet4:
            {
               wpi_res_device_hspi_enet *enet_dev;
               enet_dev = WPI_RES_DEV_HSPI_ENET(dev_entry);
               mask = WPI_HASH_TABLE_BLOCK_SIZE - 1;
               if(enet_dev != NULL &&
                  (enet_dev->hspi.pkt.device.devtype == WP_DEVICE_ENET ||
                  enet_dev->hspi.pkt.device.devtype == WP_DEVICE_ENET_MULTI_CLASS) )
               {
                  *level1_ptr = (wpi_dps_hash *)
                     enet_dev->host_hash_ptr;
               }
               *hash_mode = WPI_ENET_HASH_LOOKUP_MODE;
               break;
            }
            case WPI_pg_pkt_upi1:
            case WPI_pg_pkt_upi2:
            case WPI_pg_pkt_upi3:
            case WPI_pg_pkt_upi4:
            case WPI_pg_pkt_tdi:
            case WPI_pg_mfr_rx:
            {
               WPI_fr_dev_param dev_param;
               *hash_mode = WPI_VPI_VCI_HASH_LOOKUP_MODE;
               WPI_GetDlciHashPtr(iw_system,
                                  &dev_param,
                                  dev_entry);
               WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
                          "return from WPI_GetDlciHashPtr \n");
               *level1_ptr = dev_param.host_hash_ptr;
               mask = dev_param.mask;
               break;
            }
            default:
               return WPI_ERROR (WP_ERR_HANDLE);
         }
         break;
      }
      default:
         return WPI_ERROR (WP_ERR_HANDLE);

   }
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
              "host_hash_ptr : %x \n",(WP_U32)*level1_ptr);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
              "mask : %x \n",(WP_U32)mask);
   if (*level1_ptr == 0)
      return WPI_ERROR(WP_ERR_WDDI);

   if (*hash_mode == WPI_MAC_HASH_LOOKUP_MODE ||
       *hash_mode == WPI_ENET_HASH_LOOKUP_MODE)
   {
#ifndef DEBUG_MODE
      if(search_mode == WPI_IW_HASH_MEMBERSET_INSERT ||
         search_mode == WPI_IW_HASH_MEMBERSET_REMOVE )
      {
         hash_input[0] =0;
         hash_input[1] =0;
         hash_input[2] =0;
         hash_input[3] =0;
         hash_input[4] = ((*user_mac_entry)->vlan_id >> 8) & 0xff;
         hash_input[5] = ((*user_mac_entry)->vlan_id >> 0) & 0xff;
         hash_input[6] = ((*user_mac_entry)->out_port >> 8) & 0xff;
         hash_input[7] = ((*user_mac_entry)->out_port >> 0) & 0xff;
         *L2_place = (*user_mac_entry)->vlan_id % WPI_LOCAL_TABLE_SIZE;
      }
      else
      {     /* Forwarding hash */
         hash_input[0] = ((*user_mac_entry)->mac_h >> 24) & 0xff;
         hash_input[1] = ((*user_mac_entry)->mac_h >> 16) & 0xff;
         hash_input[2] = ((*user_mac_entry)->mac_h >> 8) & 0xff;
         hash_input[3] = ((*user_mac_entry)->mac_h >> 0) & 0xff;
         hash_input[4] = ((*user_mac_entry)->mac_l >> 8) & 0xff;
         hash_input[5] = ((*user_mac_entry)->mac_l >> 0) & 0xff;
         hash_input[6] = ((*user_mac_entry)->vlan_id >> 8) & 0xff;
         hash_input[7] = ((*user_mac_entry)->vlan_id >> 0) & 0xff;
         *L2_place = (*user_mac_entry)->mac_l % WPI_LOCAL_TABLE_SIZE;
      }

      CRC_offset = WPI_update_crc32 (hash_input);
#else
    CRC_offset = WPI_update_crc32 ((WP_U8 *) & (*user_mac_entry)->mac_h);
#endif
   }
   else
   {          /*   ATM Hash or FR Hash   */
#ifndef DEBUG_MODE
      hash_input[0] = ((*user_mac_entry)->mac_h >> 24) & 0xff;
      hash_input[1] = ((*user_mac_entry)->mac_h >> 16) & 0xff;
      hash_input[2] = ((*user_mac_entry)->mac_h >> 8) & 0xff;
      hash_input[3] = ((*user_mac_entry)->mac_h) & 0xff;
      hash_input[4] = 0;
      hash_input[5] = 0;
      hash_input[6] =((*user_mac_entry)->vlan_id >> 8)&0xff;/* the phy */
      hash_input[7] =((*user_mac_entry)->vlan_id)&0xff; /* the phy */
      CRC_offset = WPI_update_crc32 (hash_input);
#else
      CRC_offset = WPI_update_crc32 ((WP_U8 *) & (*user_mac_entry)->mac_h);
#endif
      *L2_place = (*user_mac_entry)->mac_h % WPI_LOCAL_TABLE_SIZE;
   }
   *full_offset = (CRC_offset & mask);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,"CRC_offset :%x\n",CRC_offset);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,"full_offset mask :%x\n",mask);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,"full_offset :%x\n",*full_offset);
return WP_OK;
}


WP_status WPI_UserDefGetHashParams(WP_handle iw_system,
                            wpi_dps_hash **user_mac_entry,
                            wpi_dps_hash **level1_ptr,
                            WP_U8 *hash_mode,
                            WP_U32 *full_offset,
                            WP_U16 *L2_place,
                            WP_U8 search_mode,
                            void *hash_handler_object)
{
   WP_U32 CRC_offset;
   WP_U8 hash_input[8];
   WP_U32 wpid =0;
//   WP_U32 iw_sysid;
//   WPI_PTR ptr;
   WP_U32 mask=0;
//   wpi_res_pwe_hash *res_pwehash = NULL;
//   wpi_res_iw_bridging *bridging_node=NULL;

   WPI_INTERNAL_FUNCTION();

   *level1_ptr = (wpi_dps_hash *)0;
   WPI_LOG_MT (WPI_DEBUG_LEVEL_3, "WPI_GetHashParams() ");
iw_system  = iw_system;

     // WPI_fr_dev_param dev_param;
      *hash_mode = WPI_VPI_VCI_HASH_LOOKUP_MODE;
   #if 0
      WPI_GetDlciHashPtr(iw_system,
                         &dev_param,
                         dev_entry);
      WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
                 "return from WPI_GetDlciHashPtr \n");
      *level1_ptr = dev_param.host_hash_ptr;
   #endif
    // *level1_ptr = hash_handler_object;
   wpi_memory *memory;
   memory = WPI_RegistryMemory(wpid);
   WP_U8 *host_offset;
   host_offset = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_HASH_IW_LPM);//WP_VB_ATM_ADDLOOKUP
   *level1_ptr  = (wpi_dps_hash *)WPI_HOST_PTR((WP_U32)hash_handler_object,host_offset );			   
   //printf("WPI_HANDLE_SUBTYPE(iw_system) =	0x%x \n",hash_handler_object);			   
      //mask = dev_param.mask;
      //mask = 0x0f;
      wpi_dps_hash * tmp_ptr;
   
      tmp_ptr=(wpi_dps_hash *)((WP_U32)*level1_ptr - sizeof(wpi_dps_hash));
   WPI_MEM_GET(mask,tmp_ptr->mac_h);
   //mask = tmp_ptr->mac_h;
   //printf("hash Mask is 0x%x\n",mask);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
              "host_hash_ptr : %x \n",(WP_U32)*level1_ptr);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
              "mask : %x \n",(WP_U32)mask);
   if (*level1_ptr == 0)
      return WPI_ERROR(WP_ERR_WDDI);

   if (*hash_mode == WPI_MAC_HASH_LOOKUP_MODE ||
       *hash_mode == WPI_ENET_HASH_LOOKUP_MODE)
   {
#ifndef DEBUG_MODE
      if(search_mode == WPI_IW_HASH_MEMBERSET_INSERT ||
         search_mode == WPI_IW_HASH_MEMBERSET_REMOVE )
      {
         hash_input[0] =0;
         hash_input[1] =0;
         hash_input[2] =0;
         hash_input[3] =0;
         hash_input[4] = ((*user_mac_entry)->vlan_id >> 8) & 0xff;
         hash_input[5] = ((*user_mac_entry)->vlan_id >> 0) & 0xff;
         hash_input[6] = ((*user_mac_entry)->out_port >> 8) & 0xff;
         hash_input[7] = ((*user_mac_entry)->out_port >> 0) & 0xff;
         *L2_place = (*user_mac_entry)->vlan_id % WPI_LOCAL_TABLE_SIZE;
      }
      else
      {     /* Forwarding hash */
         hash_input[0] = ((*user_mac_entry)->mac_h >> 24) & 0xff;
         hash_input[1] = ((*user_mac_entry)->mac_h >> 16) & 0xff;
         hash_input[2] = ((*user_mac_entry)->mac_h >> 8) & 0xff;
         hash_input[3] = ((*user_mac_entry)->mac_h >> 0) & 0xff;
         hash_input[4] = ((*user_mac_entry)->mac_l >> 8) & 0xff;
         hash_input[5] = ((*user_mac_entry)->mac_l >> 0) & 0xff;
         hash_input[6] = ((*user_mac_entry)->vlan_id >> 8) & 0xff;
         hash_input[7] = ((*user_mac_entry)->vlan_id >> 0) & 0xff;
         *L2_place = (*user_mac_entry)->mac_l % WPI_LOCAL_TABLE_SIZE;
      }

      CRC_offset = WPI_update_crc32 (hash_input);
#else
    CRC_offset = WPI_update_crc32 ((WP_U8 *) & (*user_mac_entry)->mac_h);
#endif
   }
   else
   {          /*   ATM Hash or FR Hash   */
#ifndef DEBUG_MODE
      hash_input[0] = ((*user_mac_entry)->mac_h >> 24) & 0xff;
      hash_input[1] = ((*user_mac_entry)->mac_h >> 16) & 0xff;
      hash_input[2] = ((*user_mac_entry)->mac_h >> 8) & 0xff;
      hash_input[3] = ((*user_mac_entry)->mac_h) & 0xff;
      hash_input[4] = 0;
      hash_input[5] = 0;
      hash_input[6] =((*user_mac_entry)->vlan_id >> 8)&0xff;/* the phy */
      hash_input[7] =((*user_mac_entry)->vlan_id)&0xff; /* the phy */
      CRC_offset = WPI_update_crc32 (hash_input);
#else
      CRC_offset = WPI_update_crc32 ((WP_U8 *) & (*user_mac_entry)->mac_h);
#endif
      *L2_place = (*user_mac_entry)->mac_h % WPI_LOCAL_TABLE_SIZE;
   }
   *full_offset = (CRC_offset & mask);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,"CRC_offset :%x\n",CRC_offset);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,"full_offset mask :%x\n",mask);
   WPI_LOG_MT(WPI_DEBUG_LEVEL_3,"full_offset :%x\n",*full_offset);
return WP_OK;
}


WP_status WPI_IwMacAddressFind (WP_handle iw_system,
                                WP_l2_forward *user_mac_entry,
                                wpi_dps_hash **matched_dps_hash,
                                WP_command command)
{
  wpi_dps_hash hash_temp;
  WP_U8 unknown_mac_mode;
  wpi_res_iw_bridging *res_iw_bridging = NULL;
  WP_status lock_status = WP_OK;

  hash_temp.ptr=0;
  hash_temp.mac_h = ((user_mac_entry->mac[0] << 24)
                     | (user_mac_entry->mac[1] << 16)
                     | (user_mac_entry->mac[2] << 8)
                     | (user_mac_entry->mac[3]));
  hash_temp.mac_l = ((user_mac_entry->mac[4] << 8)
                     | (user_mac_entry->mac[5]));

  if( command & WP_IW_SEARCH_UNK_MAC_MODE)
     unknown_mac_mode = user_mac_entry->unknown_mac_mode;
  else
     unknown_mac_mode = WP_IW_UNK_MAC_MODE_DISABLED;

  switch( unknown_mac_mode)
  {
     case WP_IW_UNK_MAC_UC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_UC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     case WP_IW_UNK_MAC_MC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_MC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     default:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_KNOWN_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        break;
  }

  hash_temp.out_port = WPI_HANDLE_INDEX (user_mac_entry->bport_tag);
  switch (user_mac_entry->process_mode)
  {
     case WP_FLOW_BRIDGE_PROCESS_PASS:
        hash_temp.cntrl = WPI_HANDLE_INDEX (user_mac_entry->aggregation);
        break;
     case WP_FLOW_BRIDGE_PROCESS_RES_MC_TERM:
        hash_temp.cntrl = WPI_RESERVED_RES_MC_TERM_FIWT_INDEX;
        break;
     case WP_FLOW_BRIDGE_PROCESS_DIRECT_TERM:
        hash_temp.cntrl = WPI_DPS_HASH_CNTRL_DTERM;
        break;
     case WP_FLOW_BRIDGE_PROCESS_DENY:
        hash_temp.cntrl = WPI_DPS_HASH_CNTRL_DENY;
        break;
     case WP_FLOW_BRIDGE_PROCESS_SVL_UC_PASS:
        hash_temp.cntrl = WPI_RESERVED_DUMMY_SVL_FIWT_INDEX;
        break;
  }

  WPI_REGISTRY_OBJECT_GET(lock_status,
                          WPI_RegistryIwBridgingGet,
                          WPI_REG_LOCKED,
                          &res_iw_bridging,
                          iw_system);
  WPI_RETURN_IF_ERROR_STATUS(lock_status);

  return WPI_IwMacAddressInsert (iw_system,
                                 &hash_temp,
                                 WPI_IW_HASH_SEARCH,
                                 matched_dps_hash,
                                 res_iw_bridging);
}

/***************************************************************************
                               External Functions
 **************************************************************************/
/*****************************************************************************
 *
 * Function: WP_IwMacAddressInsert
 *
 * Purpose: Inserts a Mac entry to the hash
 *
 * Description:
 *
 *
 * Inputs:
 *     usr_handle   A system handle / device handle
 *     user_mac_entry   A pointer to the structure
 *                      WP_l2_forward which includes the entry to insert.
 * Outputs:
 *
 * Return Value:
 *     WP_OK if function completes successfully, or an errorcode otherwise
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_IwMacAddressInsert (WP_handle usr_handle,
                                 WP_l2_forward *user_mac_entry)
{
  wpi_res_registry *wpi_registry_ptr = NULL;
  WP_status status = WP_OK, lock_status = WP_OK;
  wpi_dps_hash hash_temp;
  wpi_dps_hash *not_used;
  WP_U32 index;
  WP_U32 subtype,type;
  wpi_res_fiwt *res_fiwt;
  wpi_res_fiwt_block *fiwt_block;
  wpi_res_iw_bridging *bridging_node=NULL;
  void *hash_handler_object = NULL;
  

  if( ! WPI_HANDLE_IW_SYSTEM(usr_handle) &&
      ! WPI_HANDLE_DEVICE_P(usr_handle) )
     return WPI_ERROR(WP_ERR_HANDLE);
  if (user_mac_entry->process_mode == WP_FLOW_BRIDGE_PROCESS_SPOOFING_ALERT)
     return WP_OK;

  hash_temp.ptr=0;
  hash_temp.mac_h = ((user_mac_entry->mac[0] << 24)
                     | (user_mac_entry->mac[1] << 16)
                     | (user_mac_entry->mac[2] << 8)
                     | (user_mac_entry->mac[3]));
  hash_temp.mac_l = ((user_mac_entry->mac[4] << 8)
                     | (user_mac_entry->mac[5]));

  hash_temp.ptr = WPI_HASH_ENTRY_AGE_F(user_mac_entry->aging_bit_mode)
                |  WPI_HASH_ENTRY_SM_F(user_mac_entry->static_mac_mode)
                |  WPI_HASH_ENTRY_DENY_F(user_mac_entry->deny_mac_sa_mode);
  switch (user_mac_entry->unknown_mac_mode)
  {
     case WP_IW_UNK_MAC_UC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_UC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     case WP_IW_UNK_MAC_MC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_MC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     default:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_KNOWN_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        break;
  }
  switch (user_mac_entry->process_mode)
  {
       case WP_FLOW_BRIDGE_PROCESS_PASS:
          hash_temp.cntrl = WPI_HANDLE_INDEX (user_mac_entry->aggregation);
          if (hash_temp.cntrl <= WPI_DPS_HASH_CNTRL_DENY )
             return WPI_ERROR(WP_ERR_IW_FLOW_AGG_INVALID_HANDLE);

          break;
       case WP_FLOW_BRIDGE_PROCESS_DIRECT_TERM:
          hash_temp.cntrl = WPI_DPS_HASH_CNTRL_DTERM;
          break;
       case WP_FLOW_BRIDGE_PROCESS_RES_MC_TERM:
          hash_temp.cntrl = WPI_RESERVED_RES_MC_TERM_FIWT_INDEX;
          break;
       case WP_FLOW_BRIDGE_PROCESS_DENY:
          hash_temp.cntrl = WPI_DPS_HASH_CNTRL_DENY;
          break;
       case WP_FLOW_BRIDGE_PROCESS_SVL_UC_PASS:
          if (user_mac_entry->aggregation)
             return WPI_ERROR(WP_ERR_IW_FLOW_AGG_INVALID_HANDLE);
          hash_temp.cntrl = WPI_RESERVED_DUMMY_SVL_FIWT_INDEX;
          break;
       default:
          return WPI_ERROR (WP_ERR_INVALID_PROCESS_MODE);
  }
  WPI_WDDI_ATTACH();
  WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
  if (WP_ERROR_P(lock_status))
  {
     WPI_WDDI_DETACH();
     return lock_status;
  }
  if (WPI_REGISTRY_INITIALIZED() == WP_FALSE)
  {
    WPI_WDDI_DETACH();
    return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
  }
  subtype = WPI_HANDLE_SUBTYPE(usr_handle);
  type = WPI_HANDLE_TYPE(usr_handle);
  if(type == WPI_handle_iw_system
     && (subtype == WP_IW_TRANS_BRIDGING_MODE ||
         subtype == WP_IW_VLAN_AWARE_BRIDGING_MODE ))
  {
     WP_U32 wpid;
     wpi_iw_port_tag2h *iw_port_tag2h = NULL;

     bridging_node = (wpi_res_iw_bridging *) WPI_RES_IW_BRIDGING(usr_handle);
     wpid = WPI_HANDLE_WPID(usr_handle);

     if (!bridging_node)
     {
        WPI_WDDI_DETACH();        
        return WPI_ERROR(WP_ERR_HANDLE);
     }     
     else if (bridging_node->forwarding_table_size == WP_IW_HASH_UNUSED)
     {
        WPI_WDDI_DETACH();        
        return WPI_ERROR(WP_ERR_IW_SYS_NO_FORWARDING_TABLE);
     }

     iw_port_tag2h = WPI_RegistryIwGlobalIwPortTag2HGet(wpid);

     index = user_mac_entry->bport_tag;
     hash_temp.out_port = iw_port_tag2h->iw_port_tag2h[index];
     if( hash_temp.out_port == WPI_IW_UNKNOWN_BPORT )
     {
        WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                    "WP_IwMacAddressInsert--  tag2h[%d] =  %x",index,
                    iw_port_tag2h->iw_port_tag2h[index]);
        WPI_WDDI_DETACH();
        return WPI_ERROR(WP_ERR_UNKNOWN_BPORT_IN_THIS_SYS);
     }
     if(wpi_registry_ptr->pwp_block[wpid].iw_ports.
        iw_port_data[hash_temp.out_port].info.bridge->cur_mac_addresses>=
        wpi_registry_ptr->pwp_block[wpid].iw_ports.
        iw_port_data[hash_temp.out_port].info.bridge->max_mac_addresses)
     {
        WPI_WDDI_DETACH();
        return WPI_ERROR(WP_ERR_BPORT_MAX_MAC);
     }

     if( WPI_HANDLE_IW_IWGP_AGGREGATION(user_mac_entry->aggregation))
     {
        WPI_WDDI_DETACH();        
        return WPI_ERROR(WP_ERR_IW_FLOW_AGG_UNSUPPORTED);
     }     

     if ((user_mac_entry->process_mode == WP_FLOW_BRIDGE_PROCESS_PASS ||
          user_mac_entry->process_mode == WP_FLOW_BRIDGE_PROCESS_SVL_UC_PASS) &&
         WPI_HANDLE_SUBTYPE(user_mac_entry->aggregation) == WP_IW_INTER_SYSTEM_MODE)
     {
        fiwt_block = &wpi_registry_ptr->pwp_block[wpid].fiwts;
        res_fiwt = &(fiwt_block->fiwt_data[WPI_HANDLE_INDEX(user_mac_entry->aggregation)]);
        if ((WPI_HANDLE_SUBTYPE(res_fiwt->params.binding) == WP_IW_VLAN_AWARE_BRIDGING_MODE &&
             bridging_node->vlan_aware_mode != WP_IW_BRIDGE_VLAN_AWARE) ||
            (WPI_HANDLE_SUBTYPE(res_fiwt->params.binding) == WP_IW_TRANS_BRIDGING_MODE &&
             bridging_node->vlan_aware_mode != WP_IW_BRIDGE_TRANSPARENT))
        {
           WPI_WDDI_DETACH();
           return WPI_ERROR(WP_ERR_FLOW_AGGREGATION_INCOMPATIBLE_WITH_IW_SYSTEM);
        }
     }

     WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                 "WP_IwMacAddressInsert--  tag2h[%d] =  %x",index,
                 iw_port_tag2h->iw_port_tag2h[index]);
  }

  switch (WPI_HANDLE_TYPE(usr_handle))
  {
     case WPI_handle_iw_system:
        hash_handler_object = (void *)bridging_node;
        break;
     case WPI_handle_device:
     {
        WP_U32 wpid;
        wpi_resource_block *resources;
        wpid = WPI_HANDLE_WPID(usr_handle);
        resources = &wpi_registry_ptr->pwp_resources[wpid];
        hash_handler_object = (void *)&resources->entry[WPI_HANDLE_INDEX(usr_handle)];
     }
     break;
  }

  if (hash_handler_object == NULL)
  {
     WPI_WDDI_DETACH();
     return WPI_ERROR(WP_ERR_WDDI);
  }

  status = WPI_IwMacAddressInsert(usr_handle,
                                  &hash_temp,
                                  WPI_IW_FORWARDING_HASH_INSERT,
                                  &not_used,
                                  hash_handler_object);
  WPI_WDDI_DETACH();
  return status;
}

/*****************************************************************************
 *
 * Function: WP_IwMacAddressModify
 *
 * Purpose: Modifies a Mac entry to the hash
 *
 * Description:
 *
 *
 * Inputs:
 *     usr_handle   A system handle / device handle
 *     user_mac_entry   A pointer to the structure
 *                      WP_l2_forward which includes the entry to insert.
 * Outputs:
 *
 * Return Value:
 *     WP_OK if function completes successfully, or an errorcode otherwise
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_IwMacAddressModify (WP_handle usr_handle,
                                 WP_l2_forward *user_mac_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
  wpi_dps_hash hash_temp;
  wpi_dps_hash *not_used;
  WP_U32 index;
  WP_U32 subtype,type;
  wpi_res_fiwt *res_fiwt;
  wpi_res_fiwt_block *fiwt_block;
  wpi_res_iw_bridging *bridging_node=NULL;
  void *hash_handler_object = NULL;

  if( ! WPI_HANDLE_IW_SYSTEM(usr_handle) &&
      ! WPI_HANDLE_DEVICE_P(usr_handle) )
     return WPI_ERROR(WP_ERR_HANDLE);
  if (user_mac_entry->process_mode == WP_FLOW_BRIDGE_PROCESS_SPOOFING_ALERT)
     return WP_OK;

  hash_temp.ptr=0;
  hash_temp.mac_h = ((user_mac_entry->mac[0] << 24)
                     | (user_mac_entry->mac[1] << 16)
                     | (user_mac_entry->mac[2] << 8)
                     | (user_mac_entry->mac[3]));
  hash_temp.mac_l = ((user_mac_entry->mac[4] << 8)
                     | (user_mac_entry->mac[5]));

  hash_temp.ptr = WPI_HASH_ENTRY_AGE_F(user_mac_entry->aging_bit_mode)
                |  WPI_HASH_ENTRY_SM_F(user_mac_entry->static_mac_mode)
                |  WPI_HASH_ENTRY_DENY_F(user_mac_entry->deny_mac_sa_mode);

  switch (user_mac_entry->unknown_mac_mode)
  {
     case WP_IW_UNK_MAC_UC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_UC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     case WP_IW_UNK_MAC_MC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_MC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     default:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_KNOWN_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        break;
  }

  switch (user_mac_entry->process_mode)
  {
     case WP_FLOW_BRIDGE_PROCESS_PASS:
        hash_temp.cntrl = WPI_HANDLE_INDEX (user_mac_entry->aggregation);
        if (hash_temp.cntrl < WPI_RESERVED_CONTROL_FLOWS)
           return WPI_ERROR(WP_ERR_IW_FLOW_AGG_INVALID_HANDLE);
        break;
     case WP_FLOW_BRIDGE_PROCESS_DIRECT_TERM:
        hash_temp.cntrl = WPI_DPS_HASH_CNTRL_DTERM;
        break;
     case WP_FLOW_BRIDGE_PROCESS_RES_MC_TERM:
        hash_temp.cntrl = WPI_RESERVED_RES_MC_TERM_FIWT_INDEX;
        break;
     case WP_FLOW_BRIDGE_PROCESS_DENY:
        hash_temp.cntrl = WPI_DPS_HASH_CNTRL_DENY;
        break;
     case WP_FLOW_BRIDGE_PROCESS_SVL_UC_PASS:
        if (user_mac_entry->aggregation)
           return WPI_ERROR(WP_ERR_IW_FLOW_AGG_INVALID_HANDLE);
        hash_temp.cntrl = WPI_RESERVED_DUMMY_SVL_FIWT_INDEX;
        break;
     default:
        return WPI_ERROR (WP_ERR_INVALID_PROCESS_MODE);
  }

  WPI_WDDI_ATTACH();
  WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
  if (WP_ERROR_P(lock_status))
  {
     WPI_WDDI_DETACH();
     return lock_status;
  }
  if (WPI_REGISTRY_INITIALIZED() == WP_FALSE)
  {
     WPI_WDDI_DETACH();
     return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
  }

  subtype = WPI_HANDLE_SUBTYPE(usr_handle);
  type = WPI_HANDLE_TYPE(usr_handle);

  if(type == WPI_handle_iw_system
     && (subtype == WP_IW_TRANS_BRIDGING_MODE ||
         subtype == WP_IW_VLAN_AWARE_BRIDGING_MODE ))
  {
     WP_U32 wpid;
     wpi_iw_port_tag2h *iw_port_tag2h = NULL;

     bridging_node = (wpi_res_iw_bridging *) WPI_RES_IW_BRIDGING(usr_handle);
     wpid = WPI_HANDLE_WPID(usr_handle);

     if (!bridging_node)
     {
        WPI_WDDI_DETACH();        
        return WPI_ERROR(WP_ERR_HANDLE);
     }     
     else if (bridging_node->forwarding_table_size == WP_IW_HASH_UNUSED)
     {
        WPI_WDDI_DETACH();        
        return WPI_ERROR(WP_ERR_IW_SYS_NO_FORWARDING_TABLE);
     }
     

     iw_port_tag2h = WPI_RegistryIwGlobalIwPortTag2HGet(wpid);
     index = user_mac_entry->bport_tag;
     hash_temp.out_port = iw_port_tag2h->iw_port_tag2h[index];

     if( hash_temp.out_port == WPI_IW_UNKNOWN_BPORT )
     {
        WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                    "WP_IwMacAddressModify--  tag2h[%d] =  %x",index,
                    iw_port_tag2h->iw_port_tag2h[index]);
        WPI_WDDI_DETACH();
        return WPI_ERROR(WP_ERR_UNKNOWN_BPORT_IN_THIS_SYS);
     }

     if ((user_mac_entry->process_mode == WP_FLOW_BRIDGE_PROCESS_PASS ||
          user_mac_entry->process_mode == WP_FLOW_BRIDGE_PROCESS_SVL_UC_PASS) &&
         WPI_HANDLE_SUBTYPE(user_mac_entry->aggregation) == WP_IW_INTER_SYSTEM_MODE)
     {
        fiwt_block = &wpi_registry_ptr->pwp_block[wpid].fiwts;
        res_fiwt = &(fiwt_block->fiwt_data[WPI_HANDLE_INDEX(user_mac_entry->aggregation)]);
        if ((WPI_HANDLE_SUBTYPE(res_fiwt->params.binding) == WP_IW_VLAN_AWARE_BRIDGING_MODE &&
             bridging_node->vlan_aware_mode != WP_IW_BRIDGE_VLAN_AWARE) ||
            (WPI_HANDLE_SUBTYPE(res_fiwt->params.binding) == WP_IW_TRANS_BRIDGING_MODE &&
             bridging_node->vlan_aware_mode != WP_IW_BRIDGE_TRANSPARENT))
        {
           WPI_WDDI_DETACH();
           return WPI_ERROR(WP_ERR_FLOW_AGGREGATION_INCOMPATIBLE_WITH_IW_SYSTEM);
        }
     }

     WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                 "WP_IwMacAddressModify--  tag2h[%d] =  %x",index,
                 iw_port_tag2h->iw_port_tag2h[index]);
  }

  switch (WPI_HANDLE_TYPE(usr_handle))
  {
     case WPI_handle_iw_system:
        hash_handler_object = (void *)bridging_node;
        break;
     case WPI_handle_device:
     {
        WP_U32 wpid;
        wpi_resource_block *resources;
        wpid = WPI_HANDLE_WPID(usr_handle);
        resources = &wpi_registry_ptr->pwp_resources[wpid];
        hash_handler_object = (void *)&resources->entry[WPI_HANDLE_INDEX(usr_handle)];
     }
     break;
  }

  if (hash_handler_object == NULL)
  {
     WPI_WDDI_DETACH();
     return WPI_ERROR(WP_ERR_WDDI);
  }
  
  status = WPI_IwMacAddressInsert(usr_handle,
                                  &hash_temp,
                                  WPI_IW_FORWARDING_HASH_INSERT,
                                  &not_used,
                                  hash_handler_object);
  WPI_WDDI_DETACH();
  return status;
}

/*****************************************************************************
 *
 * Function: WP_IwMacAddressRemove
 *
 * Purpose: Removes a Mac entry to the hash
 *
 * Description:
 *
 *
 * Inputs:
 *     iw_system   A system handle
 *     user_mac_entry   A pointer to the structure
 *                      WP_l2_forward which includes the entry to remove.
 * Outputs:
 *
 * Return Value:
 *     WP_OK if function completes successfully, or an errorcode otherwise
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/

WP_status WP_IwMacAddressRemove (WP_handle iw_system,
                                 WP_l2_forward *user_mac_entry)
{
  wpi_dps_hash hash_temp;
  WP_U32 index;
  WP_status status, lock_status = WP_OK;
  wpi_res_iw_bridging *res_iw_bridging = NULL;

  hash_temp.ptr=0;
  hash_temp.mac_h = ((user_mac_entry->mac[0] << 24)
                     | (user_mac_entry->mac[1] << 16)
                     | (user_mac_entry->mac[2] << 8)
                     | (user_mac_entry->mac[3]));
  hash_temp.mac_l = ((user_mac_entry->mac[4] << 8)
                     | (user_mac_entry->mac[5]));

  switch (user_mac_entry->unknown_mac_mode)
  {
     case WP_IW_UNK_MAC_UC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_UC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     case WP_IW_UNK_MAC_MC:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_UNK_MC_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        if ((hash_temp.mac_h != WPI_HASH_ENTRY_BC_MAC_HIGH) ||
            (hash_temp.mac_l != WPI_HASH_ENTRY_BC_MAC_LOW))
           return WPI_ERROR( WP_ERR_BROADCAST_MAC_EXPECTED );
        break;
     default:
        hash_temp.vlan_id = WPI_HASH_ENTRY_UNK_ADDR_TYPE_F(WPI_HASH_ENTRY_KNOWN_MAC)
           | WPI_HASH_ENTRY_VLAN_ID_EX(user_mac_entry->vlan_id);
        break;
  }

  index = WPI_HANDLE_INDEX( user_mac_entry->bport_tag);
  hash_temp.out_port =index;
  WPI_WDDI_ATTACH_MT();
  if (WPI_REGISTRY_INITIALIZED() == WP_FALSE)
  {
    WPI_WDDI_DETACH_MT();
    return WPI_ERROR(WP_ERR_INCORRECT_REGISTRY_STATE);
  }

  hash_temp.cntrl = WPI_HANDLE_INDEX (user_mac_entry->aggregation);
  /* iw_system is an IW-system handle. */
  WPI_REGISTRY_OBJECT_GET(lock_status,
                          WPI_RegistryIwBridgingGet,
                          WPI_REG_LOCKED,
                          &res_iw_bridging,
                          iw_system);
  WPI_DETACH_MT_RETURN_IF_ERROR_STATUS(lock_status);
  status = WPI_IwMacAddressRemove (iw_system,
                                   &hash_temp,
                                   WPI_IW_FORWARDING_HASH_REMOVE,
                                   res_iw_bridging);
  WPI_DETACH_MT_RETURN_IF_ERROR_STATUS(status);
  WPI_REGISTRY_OBJECT_RELEASE(lock_status,
                              WPI_RegistryIwBridgingRelease,
                              WPI_REG_UNLOCKED,
                              &res_iw_bridging);
  WPI_DETACH_MT_RETURN_IF_ERROR_STATUS(lock_status);
  WPI_WDDI_DETACH_MT();
  return status;
}

/*******************************************************************
  User Vp Vc hash insert functions
*******************************************************************/

/*****************************************************************************
 *
 * Function: WPI_AtmHashEntryInsert
 *
 * Purpose: Inserts a Vp/Vc entry to the hash
 *
 * Description:
 *
 *
 * Inputs:
 *     port             Port handle
 *     user_atm_entry   A pointer to the structure
 *                      WPI_atm_hash_param which includes the entry to insert.
 * Outputs:
 *
 * Return Value:
 *     WP_OK if function completes successfully, or an errorcode otherwise
 *
 * Called by:
 *     WPI_AtmChannelCreate,WPI_AtmChannelEnable,WPI_AtmChannelDisable
 *
 ****************************************************************************/

WP_status WPI_AtmHashEntryInsert (WP_handle port,
                                  WPI_atm_hash_param * user_atm_entry,
                                  WP_boolean replace_mode)
{
  wpi_dps_hash hash_temp;
  wpi_dps_hash *not_used;
  WP_status status = WP_OK, lock_status = WP_OK;
  wpi_atm *atm = NULL;
  wpi_iw_global *iw_global = NULL;
  void *hash_handler_object = NULL;
  WP_U32 wpid;

  wpid = WPI_HANDLE_WPID(port);
  hash_temp.ptr=0;
  hash_temp.mac_h =
    WPI_ATM_VP_VC_OR (user_atm_entry->vpi, user_atm_entry->vci);
  /* when header is not replaced , new_vpi is reset to zero */
  hash_temp.mac_l = user_atm_entry->new_vpi;
  hash_temp.vlan_id = user_atm_entry->phy;
  hash_temp.out_port = user_atm_entry->trans_code;
  hash_temp.cntrl = user_atm_entry->ch_num;

  switch (WPI_HANDLE_TYPE (port))
  {
     case WPI_handle_iw_aggregation:
        WPI_REGISTRY_OBJECT_GET(lock_status,
                                WPI_RegistryIwGlobalGet,
                                WPI_REG_LOCKED,
                                &iw_global,
                                wpid);
        WPI_RETURN_IF_ERROR_STATUS(lock_status);
        hash_handler_object = iw_global;
        break;
     case WPI_handle_port:
     case WPI_handle_gbond_group:
     case WPI_handle_ima:
        atm = WPI_RegistryAtm(wpid);
        hash_handler_object = atm;
        break;
  }
  
  if (hash_handler_object == NULL)
     return WPI_ERROR(WP_ERR_WDDI);

  if (replace_mode)
  {     
     status = WPI_IwMacAddressInsert (port,
                                      &hash_temp,
                                      WPI_IW_ATM_HASH_INSERT,
                                      &not_used,
                                      hash_handler_object);
  }
  else
  {
     status = WPI_IwMacAddressInsert (port,
                                      &hash_temp,
                                      WPI_IW_ATM_HASH_INSERT | WPI_HASH_DONT_REPLACE,
                                      &not_used,
                                      hash_handler_object);
  }
  return status;
}
/*****************************************************************************
 *
 * Function: WPI_FrHashEntryInsert
 *
 * Purpose: Inserts a Dlci + Device   entry to the hash
 *
 * Description:
 *
 *
 * Inputs:
 *     port             Port handle
 *     user_atm_entry   A pointer to the structure
 *                      WPI_fr_hash_param which includes the entry to insert.
 * Outputs:
 *
 * Return Value:
 *     WP_OK if function completes successfully, or an errorcode otherwise
 *
 * Called by:
 *     WPI_FrChannelCreate
 *
 ****************************************************************************/

WP_status WPI_FrHashEntryInsert (WP_handle dev_handle,
                                 WPI_fr_hash_param * user_fr_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
  wpi_dps_hash hash_temp;
  wpi_dps_hash *not_used;
  wpi_resource_entry *dev_entry = NULL;

  WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
  WPI_RETURN_IF_ERROR_STATUS(lock_status);
  hash_temp.ptr = 0;
  hash_temp.mac_h = user_fr_entry->dlci;
  hash_temp.mac_l = 0;
  hash_temp.vlan_id = user_fr_entry->phy;
  hash_temp.out_port = user_fr_entry->trans_code;
  hash_temp.cntrl = user_fr_entry->ch_num;

  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "dlci : %d \n",user_fr_entry->dlci);
  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "phy : %d \n",user_fr_entry->phy);
  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "ch_num : %d \n",user_fr_entry->ch_num);

  dev_entry = &wpi_registry_ptr->pwp_resources[WPI_HANDLE_WPID(dev_handle)].entry[WPI_HANDLE_INDEX(dev_handle)];
  status = WPI_IwMacAddressInsert (dev_handle,
                                   &hash_temp,
                                   WPI_IW_FR_HASH_INSERT,
                                   &not_used,
                                   dev_entry);
  return status;
}



WP_status WPI_UserDefHashEntryInsert (WP_handle dev_handle,
                                 WP_U32 key1,WP_U16 key2,WP_U16 data)
{
 //  wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK;//, lock_status = WP_OK;
  wpi_dps_hash hash_temp;
  wpi_dps_hash *not_used;
  WPI_WDDI_ATTACH();
  //WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
  //WPI_RETURN_IF_ERROR_STATUS(lock_status);
  hash_temp.ptr = 0;
  hash_temp.mac_h = key1;
  hash_temp.mac_l = 0;
  hash_temp.vlan_id = key2;
  hash_temp.out_port = data;
  hash_temp.cntrl = data&0xfff0;
  status = WPI_UserDefIwMacAddressInsert(dev_handle,
                                   &hash_temp,
                                   WPI_IW_FR_HASH_INSERT,
                                   &not_used,
                                   UserDefHashBase);
  WPI_WDDI_DETACH();
  return status;
}



/*****************************************************************************
 *
 * Function: WPI_FrHashEntryRemove
 *
 * Purpose: Removes a Dlci+device entry from the hash
 *
 * Description:
 *
 *
 * Inputs:
 *     device            Device handle
 *     user_fr_entry   A pointer to the structure WPI_fr_hash_param which
 *                      includes the entry to remove.
 * Outputs:
 *
 * Return Value:
 *     WP_OK if function completes successfully, or an errorcode otherwise
 *
 * Called by:
 *     WPI_FrChannelDelete
 *
 ****************************************************************************/
WP_status WPI_FrHashEntryRemove (WP_handle dev_handle,
                                 WPI_fr_hash_param * user_fr_entry)
{
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
  wpi_dps_hash hash_temp;
  wpi_resource_entry *dev_entry = NULL;

  WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
  WPI_RETURN_IF_ERROR_STATUS(lock_status);
  hash_temp.ptr = 0;
  hash_temp.mac_h = user_fr_entry->dlci;
  hash_temp.mac_l = 0;
  hash_temp.vlan_id = user_fr_entry->phy;
  hash_temp.out_port = user_fr_entry->trans_code;
  hash_temp.cntrl = user_fr_entry->ch_num;

  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "dlci : %d \n",user_fr_entry->dlci);
  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "phy : %d \n",user_fr_entry->phy);
  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "ch_num : %d \n",user_fr_entry->ch_num);

  dev_entry = &wpi_registry_ptr->pwp_resources[WPI_HANDLE_WPID(dev_handle)].entry[WPI_HANDLE_INDEX(dev_handle)];
  /* dev_handle is a device handle. */
  status = WPI_IwMacAddressRemove (dev_handle,
                                   &hash_temp,
                                   WPI_IW_FR_HASH_REMOVE,
                                   dev_entry);
  return status;
}

WP_status WPI_UserDefHashEntryRemove (WP_handle dev_handle,
                                 WP_U32 key1,WP_U16 key2,WP_U16 data)
{
#if 0
   wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK, lock_status = WP_OK;
  wpi_dps_hash hash_temp;
  wpi_resource_entry *dev_entry = NULL;

  WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
  WPI_RETURN_IF_ERROR_STATUS(lock_status);
  hash_temp.ptr = 0;
  hash_temp.mac_h = user_fr_entry->dlci;
  hash_temp.mac_l = 0;
  hash_temp.vlan_id = user_fr_entry->phy;
  hash_temp.out_port = user_fr_entry->trans_code;
  hash_temp.cntrl = user_fr_entry->ch_num;

  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "dlci : %d \n",user_fr_entry->dlci);
  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "phy : %d \n",user_fr_entry->phy);
  WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
             "ch_num : %d \n",user_fr_entry->ch_num);

  dev_entry = &wpi_registry_ptr->pwp_resources[WPI_HANDLE_WPID(dev_handle)].entry[WPI_HANDLE_INDEX(dev_handle)];
  /* dev_handle is a device handle. */
  status = WPI_IwMacAddressRemove (dev_handle,
                                   &hash_temp,
                                   WPI_IW_FR_HASH_REMOVE,
                                   dev_entry);
#endif


  
  //  wpi_res_registry *wpi_registry_ptr = NULL;
	WP_status status = WP_OK;//, lock_status = WP_OK;
   wpi_dps_hash hash_temp;
   WPI_WDDI_ATTACH();
   //WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   //WPI_RETURN_IF_ERROR_STATUS(lock_status);
   hash_temp.ptr = 0;
   hash_temp.mac_h = key1;
   hash_temp.mac_l = 0;
   hash_temp.vlan_id = key2;
   hash_temp.out_port = data;
   hash_temp.cntrl = 0;
   status = WPI_UserDefIwMacAddressRemove(dev_handle,
									&hash_temp,
									WPI_IW_FR_HASH_INSERT,
									UserDefHashBase);
   WPI_WDDI_DETACH();
  return status;
}

WP_status WPI_UserDefHashEntryDisplay (WP_handle dev_handle,
                                 WP_U32 key1,WP_U16 key2,WP_U16* result)
{
  // wpi_res_registry *wpi_registry_ptr = NULL;
   WP_status status = WP_OK;//, lock_status = WP_OK;
   wpi_dps_hash hash_temp;
   wpi_dps_hash hash_out = {0};
//  WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
 // WPI_RETURN_IF_ERROR_STATUS(lock_status);
  hash_temp.ptr = 0;
  hash_temp.mac_h = key1;
  hash_temp.mac_l = 0;
  hash_temp.vlan_id = key2;
  hash_temp.out_port = 0;
  hash_temp.cntrl = 0;
 
  /* dev_handle is a device handle. */
  status = WPI_UserHashIwMacAddressFind (dev_handle,
                                   &hash_temp,
                                   UserDefHashBase,&hash_out);
  if(status == 0)
  *result = ((hash_out.out_port>>8)&0xff)|((hash_out.out_port&0xff)<<8) ;
 
  return status;
}



/*****************************************************************************
 *
 * Function: WPI_AtmHashEntryRemove
 *
 * Purpose: Removes a Vp/Vc entry from the hash
 *
 * Description:
 *
 *
 * Inputs:
 *     port             Port handle
 *     user_Atm_entry   A pointer to the structure WPI_atm_hash_param which
 *                      includes the entry to remove.
 * Outputs:
 *
 * Return Value:
 *     WP_OK if function completes successfully, or an errorcode otherwise
 *
 * Called by:
 *     WPI_AtmChannelDelete
 *
 ****************************************************************************/
WP_status WPI_AtmHashEntryRemove (WP_handle port,
                                  WPI_atm_hash_param * user_atm_entry)
{
  wpi_dps_hash hash_temp;
  WP_status status, lock_status = WP_OK;
  wpi_atm *atm = NULL;
  wpi_iw_global *iw_global = NULL;
  void *hash_handler_object = NULL;
  WP_U32 wpid;

  wpid = WPI_HANDLE_WPID(port);
  hash_temp.ptr = 0;
  hash_temp.mac_h =
    WPI_ATM_VP_VC_OR (user_atm_entry->vpi, user_atm_entry->vci);
  hash_temp.mac_l = user_atm_entry->new_vpi;
  hash_temp.vlan_id = user_atm_entry->phy;
  hash_temp.out_port = user_atm_entry->trans_code;
  hash_temp.cntrl = user_atm_entry->ch_num;

  switch (WPI_HANDLE_TYPE (port))
  {
     case WPI_handle_iw_aggregation:
        WPI_REGISTRY_OBJECT_GET(lock_status,
                                WPI_RegistryIwGlobalGet,
                                WPI_REG_LOCKED,
                                &iw_global,
                                wpid);
        WPI_RETURN_IF_ERROR_STATUS(lock_status);
        hash_handler_object = iw_global;
        break;
     case WPI_handle_port:
     case WPI_handle_gbond_group:
     case WPI_handle_ima:
        atm = WPI_RegistryAtm(wpid);
        hash_handler_object = atm;
        break;
  }

  if (hash_handler_object == NULL)
     return WPI_ERROR(WP_ERR_WDDI);
  
  status = WPI_IwMacAddressRemove (port,
                                   &hash_temp,
                                   WPI_IW_ATM_HASH_REMOVE,
                                   hash_handler_object);
  if (status == WPI_BASIC_ERROR(WP_ERR_NOT_EXIST_IN_HASH))
  {
     /* This error can be ignored.
        It is possible that the hash entry is already occupied by another channel. */
     status = WP_OK;     
  }
  
  return status;
}

/***************************************************************************
                            Internal Functions
 **************************************************************************/

/*********************************************************************

   function name : WPI_IwMacAddressInsert

   parameters :  WP_handle iw_system 19 bits Index |2 bit  winpath id|
                 5 subtype|6 type
                 wpi_dps_hash user_mac_entry

   matched_dps_hash: Is active only in WPI_IW_HASH_SEARCH mode.
                     returns a copy of the dps_hash entry matched.
                     if no match matched_dps_hash is set to NULL.

   description  : The function inserts into the hash table a "MAC BLock"
                     (consisting  of MAC addres and MAC control word)
                     that point on l2 interworking table that points on the
                     flow interworking table that points on a queue assignment
                     table that points on the transmit buffer descriptor .

   called by : user (main)

   input: A 6 bytes entry ; contains the fields : mac_h , mac_l , control_word

   output : none

 ********************************************************************/

WP_status WPI_IwMacAddressInsert (WP_handle iw_system,
                                  wpi_dps_hash *user_mac_entry,
                                  WP_U8 search_mode,
                                  wpi_dps_hash **matched_dps_hash,
                                  void *hash_handler_object)
{
  WP_U32  match_found, mac_vc_l = 0, ii;
  WP_U32 full_offset;
  WP_U16 L2_place;
  wpi_dps_hash *level1_ptr, *prev;
  wpi_dps_hash *level2_ptr = NULL;
  wpi_dps_hash *LinkList_ptr;
  WP_U32 wpid;
  WP_status status, lock_status = WP_OK;
  WP_U16 out_port = 0, cntrl = 0;
  WP_U32 ret_val = 0;
  WP_U8 hash_mode = 0;

  *matched_dps_hash = NULL;/*default value in case no match was found*/

  WPI_RETURN_IF_ERROR(status,WPI_GetHashParams(iw_system,
                                               &user_mac_entry,
                                               &level1_ptr,
                                               &hash_mode,
                                               &full_offset,
                                               &L2_place,
                                               search_mode,
                                               hash_handler_object));
  wpid  = WPI_HANDLE_WPID( iw_system);

#ifdef STATISTICS
  insert_count++;
#endif

  WPI_LOG_MT (WPI_DEBUG_LEVEL_9, "return WPI_GetHashParams ");
  WPI_LOG_MT (WPI_DEBUG_LEVEL_9,
              "level1_ptr %x\n  ",
              (WP_U32)level1_ptr);
  if (((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_MAC_NON_VALID) && MAC_MODE (hash_mode)) ||
      ((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_ATM_NON_VALID) && ATM_MODE (hash_mode)))
  {                           /*  insertion of new block to level 1: */
     /* L1 was available.  Occupation not used at this time. */
     WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 1 ");
     if (search_mode == WPI_IW_HASH_SEARCH)
     {
        if (MAC_MODE(hash_mode))
           return (WPI_MAC_NON_VALID);
        if (ATM_MODE(hash_mode))
           return (WPI_ATM_NON_VALID);
     }

     WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Insert 1 -- Mac ++ ");
     WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                   search_mode,
                                   hash_mode,
                                   wpid);
     WPI_WRITE_HASH_ENTRY(&level1_ptr[full_offset],
                          user_mac_entry,search_mode);
     return WP_OK;             /*  INS1  */
  }
  else /*   left main node of flow    */
  if (((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_MAC_COLLISION)
       && MAC_MODE (hash_mode)) ||
      ((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_ATM_COLLISION)
       && ATM_MODE (hash_mode)))
    {
      level2_ptr =
        ((wpi_dps_hash
          *) (R_L2_PTR (wpid, &level1_ptr[full_offset], hash_mode)));

      if (((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_MAC_NON_VALID) && MAC_MODE (hash_mode)) ||
          ((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_ATM_NON_VALID) && ATM_MODE (hash_mode)))
        {                       /* insertion of new block to level 2: */
           /* L2_place was not previously occupied.  Set the L2_place bit in L1 */
           WP_U32 occupation = R_MAC_H(level1_ptr[full_offset]);
           occupation |= 1 << L2_place;
           W_MAC_H(level1_ptr[full_offset], occupation);

          WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 2 ");
          if (search_mode == WPI_IW_HASH_SEARCH)
          {
             if (MAC_MODE(hash_mode))
                return (WPI_MAC_NON_VALID);
             if (ATM_MODE(hash_mode))
                return (WPI_ATM_NON_VALID);
          }
          if (((R_MAC_H (level2_ptr[L2_place]) != 0) && ATM_MODE (hash_mode)))
          {
             WPI_WRITE_HASH_ENTRY (&level2_ptr[L2_place],
                                   user_mac_entry,
                                   search_mode);
          }
          else
          {
             WPI_WRITE_HASH_ENTRY (&level2_ptr[L2_place],
                                   user_mac_entry,
                                   search_mode);
             W_MAC_L (level1_ptr[full_offset],
                      R_MAC_L (level1_ptr[full_offset]) + 1);
          }
          WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                        search_mode,
                                        hash_mode,
                                        wpid);
          return WP_OK;         /*  INS2  */
        }
      else
        if (((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_MAC_COLLISION)
             && MAC_MODE (hash_mode))
            || ((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_ATM_COLLISION)
                && ATM_MODE (hash_mode)))
        {
           /* L2 collision already occurred, so L1 already has occupied bit set */
          wpi_dps_hash *current;

          /* search the link list */
          prev = WPI_SearchLinkList(wpid,
                                    &level2_ptr[L2_place],
                                    user_mac_entry,
                                    &current,
                                    &match_found,
                                    hash_mode,
                                    search_mode);

          if (match_found == WP_TRUE)
            {   /* change of Control Word of item inside linked list */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 3 ");

              if (search_mode != WPI_IW_HASH_SEARCH)
              {
                 /* Linked-list   Case -
                    switching mac between bports
                    -- Decrement current max_address per bport  */

                 WPI_CURRENT_MAC_PER_BPORT_DEC(current,
                                               search_mode,
                                               hash_mode,
                                               WP_TRUE,
                                               wpid);
                 WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                               search_mode,
                                               hash_mode,
                                               wpid);

                 if(ATM_MODE (hash_mode))
                 {
                    if((search_mode & WPI_HASH_DONT_REPLACE) &&
                       (R_MAC_CNTLS(*current)!= WPI_ATM_NON_VALID))
                       return WPI_ERROR(WP_ERR_CH_ALREADY_CREATED);
                 }
                 WPI_UPDATE_HASH_ENTRY (current,
                                        user_mac_entry,
                                        search_mode,
                                        wpid);
              }
              else
              {
                  /*the matched wpi_dps_hash entry is returned by reference*/
                  *matched_dps_hash = current;
                  out_port = R_OUT_PORT (*current);
                  cntrl = R_MAC_CNTLS (*current);
                  ret_val = cntrl << 16 | out_port;
                  return ret_val;
              }
              return WP_OK;     /*  INS3   */
            }
          else
            {
              /* insertion of a new item to the end of linked list: */
              /* L1 already knows the L2 slot is occupied, so nothing to do. */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 4 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }
              status =
                 WPI_IwInsertHashLinkList(wpid, prev, user_mac_entry,
                                          hash_mode,search_mode);
              if (status != WP_OK)
                 return WPI_ERROR (WP_ERR_IW_MACLOOKUP_ALLOCATION);
              W_MAC_L (level2_ptr[L2_place],
                       R_MAC_L (level2_ptr[L2_place]) + 1);
              W_MAC_L (level1_ptr[full_offset],
                       R_MAC_L (level1_ptr[full_offset]) + 1);
              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /* INS4 */
            }
        }
      else                      /* exact match  */
        {
          if (WPI_KEY_IS_EXIST(level2_ptr[L2_place],
                               user_mac_entry,
                               search_mode))
           {
              /* change of Control Word in level 2 */
              /* Item exists already, so L1 knows L2_place is occupied. */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 5 ");
              if (search_mode != WPI_IW_HASH_SEARCH)
                {
                   /* level 2--
                      Case - switching mac between bports
                      -- Decrement current max_address per bport  */

                   WPI_CURRENT_MAC_PER_BPORT_DEC(&level2_ptr[L2_place],
                                                 search_mode,
                                                 hash_mode,
                                                 WP_TRUE,
                                                 wpid);
                   WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                                 search_mode,
                                                 hash_mode,
                                                 wpid);

                   if(ATM_MODE (hash_mode))
                   {
                      if((search_mode & WPI_HASH_DONT_REPLACE) &&
                         (R_MAC_CNTLS(level2_ptr[L2_place]) != WPI_ATM_NON_VALID))
                         return WPI_ERROR(WP_ERR_CH_ALREADY_CREATED);
                   }

                   WPI_UPDATE_HASH_ENTRY (&level2_ptr[L2_place],
                                          user_mac_entry,
                                          search_mode,
                                          wpid);
                }
              else
                {
                  /*the matched wpi_dps_hash entry is returned by reference*/
                  *matched_dps_hash = &level2_ptr[L2_place];
                  out_port = R_OUT_PORT (level2_ptr[L2_place]);
                  cntrl = R_MAC_CNTLS (level2_ptr[L2_place]);
                  ret_val = cntrl << 16 | out_port;
                  return ret_val;
                }
              return WP_OK;     /* INS5 */
            }
          else
            {
             /* Creation of linked list (only) & insertion of the items: */
              /* L1 already knows L2_place is occupied, so nothing to do. */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 6 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }
              /*Creation of a linked list with 2 items */
              WPI_RETURN_IF_ERROR(status,
                                  WPI_CreateLinkList (&LinkList_ptr,
                                                      wpid,
                                                      &level2_ptr[L2_place],
                                                      user_mac_entry,
                                                      hash_mode,
                                                      search_mode));
               /* counter change in L1 */
              W_MAC_L (level1_ptr[full_offset],
                       R_MAC_L (level1_ptr[full_offset]) + 1);
              /*Concatenation of L2 to LL */
              W_LL_PTR_FROM_L2 (wpid, &level2_ptr[L2_place], LinkList_ptr);
              if (MAC_MODE (hash_mode))
                {
                  W_MAC_CNTLS (level2_ptr[L2_place], WPI_MAC_COLLISION);
                }
              else
                 W_MAC_CNTLS (level2_ptr[L2_place], WPI_ATM_COLLISION);
              /* counter change in L2 */
              W_MAC_L (level2_ptr[L2_place], WPI_ENTRY_COUNT_INIT);

              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /*  INS6  */
            }
        }
    }
  else
    {                           /*  right main node of flow -- exact match */
       if (WPI_KEY_IS_EXIST(level1_ptr[full_offset],
                            user_mac_entry,
                            search_mode))
        {                       /* changing of Control Word in L1 */
          WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 7 ");
          if (search_mode != WPI_IW_HASH_SEARCH)
            {
               /*  level1  Case -
                   switching mac between bports  --
                   Decrement current max_address per bport  */
           /* L1 points to same item, so occupation already known. */
               WPI_CURRENT_MAC_PER_BPORT_DEC(&level1_ptr[full_offset],
                                             search_mode,
                                             hash_mode,
                                             WP_TRUE,
                                             wpid);
               WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                             search_mode,
                                             hash_mode,
                                             wpid);

               if(ATM_MODE (hash_mode))
               {
                  if((search_mode & WPI_HASH_DONT_REPLACE) &&
                     (R_MAC_CNTLS(level1_ptr[full_offset]) != WPI_ATM_NON_VALID))
                     return WPI_ERROR(WP_ERR_CH_ALREADY_CREATED);
               }

               WPI_UPDATE_HASH_ENTRY (&level1_ptr[full_offset],
                                      user_mac_entry,
                                      search_mode,
                                      wpid);
            }
          else
            {
              *matched_dps_hash = &level1_ptr[full_offset];
              out_port = R_OUT_PORT (level1_ptr[full_offset]);
              cntrl = R_MAC_CNTLS (level1_ptr[full_offset]);
              ret_val = cntrl << 16 | out_port;
              return ret_val;
            }
          return WP_OK;         /*  INS7  */
        }
      else
      {

         WPI_PTR tmp_ptr;

         if (L2_place ==
             (mac_vc_l =
              (((MAC_MODE (hash_mode)) ? R_MAC_L (level1_ptr[full_offset]) :
                R_MAC_H (level1_ptr[full_offset])) % WPI_LOCAL_TABLE_SIZE)))
         {  /* The Element in level1 collides with the new Element
               In level 1 and level 2 as well. As a resault we
               create a level 2 and linked-list with 2 elements.*/
           /* L1 points to a different item, with same L2_place.  Set
            * L1 occupied at L2_place. */
            WP_U32 occupation;
            status = WPI_IwMemFreeListGet(&tmp_ptr,
                                          NULL,
                                          wpid,
                                          LEVEL1_HASH_MEM_FREE_LIST);
            WPI_RETURN_IF_ERROR_STATUS(status);

            level2_ptr = (wpi_dps_hash *)tmp_ptr;
            if(level2_ptr == NULL)
               level2_ptr =
                  ((wpi_dps_hash
                    *) (WPI_host_malloc (wpid, WP_VB_HASH_IW_LPM,
                                         sizeof (wpi_dps_hash) *
                                         WPI_LOCAL_TABLE_SIZE,
                                         sizeof (wpi_dps_hash))));
              if (level2_ptr == NULL)
                 return WPI_ERROR (WP_ERR_IW_MACLOOKUP_ALLOCATION);
              /* local table initilization */
              WPI_memclr ((WP_U32 *) level2_ptr, sizeof (wpi_dps_hash) *
                          WPI_LOCAL_TABLE_SIZE);
              if (hash_mode == WPI_VPI_VCI_HASH_LOOKUP_MODE)
                for (ii = 0; ii < WPI_LOCAL_TABLE_SIZE; ii++)
                  {
                    W_MAC_CNTLS (level2_ptr[ii], WPI_ATM_NON_VALID);

                  }

              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 8 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }
              /*Creation of a linked list with 2 items */
              WPI_RETURN_IF_ERROR(status,
                                  WPI_CreateLinkList(&LinkList_ptr,
                                                     wpid,
                                                     &level1_ptr[full_offset]
                                                     ,user_mac_entry,
                                                     hash_mode,
                                                     search_mode));

              W_MAC_L (level2_ptr[L2_place], WPI_ENTRY_COUNT_INIT);
              if (MAC_MODE (hash_mode))
              {
                 W_MAC_CNTLS (level2_ptr[L2_place], WPI_MAC_COLLISION);
              }
              else
              {
                 W_MAC_CNTLS (level2_ptr[L2_place], WPI_ATM_COLLISION);
              }
              /* Concatenation of L2 to LL */
              W_LL_PTR_FROM_L2 (wpid, &level2_ptr[L2_place], LinkList_ptr);

              W_L2_PTR (wpid, &level1_ptr[full_offset], level2_ptr);
              if (MAC_MODE (hash_mode))
              {
                 W_MAC_CNTLS (level1_ptr[full_offset], WPI_MAC_COLLISION);
              }
              else
              {
                 W_MAC_CNTLS (level1_ptr[full_offset], WPI_ATM_COLLISION);
              }

              W_MAC_L (level1_ptr[full_offset], WPI_ENTRY_COUNT_INIT);
              /* set occupation */
              occupation = (1 << L2_place);
              W_MAC_H(level1_ptr[full_offset], occupation);

              /* Concatenation of L1 to L2 */

              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /*  INS8  */
            }
          else
            {
               WPI_PTR tmp_ptr;
               WP_U32 occupation;

               /* Insertion of the 2 items to a new level 2 */
               /* Also, set occupation for both L2_place and mac_vc_l */

              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 9.0 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }

              status = WPI_IwMemFreeListGet(&tmp_ptr,
                                            NULL,
                                            wpid,
                                            LEVEL1_HASH_MEM_FREE_LIST);
              WPI_RETURN_IF_ERROR_STATUS(status);

              level2_ptr = (wpi_dps_hash *)tmp_ptr;

              if(level2_ptr == NULL)
                 level2_ptr =
                    (wpi_dps_hash
                     *) (WPI_host_malloc (wpid, WP_VB_HASH_IW_LPM,
                                          sizeof (wpi_dps_hash) *
                                          WPI_LOCAL_TABLE_SIZE,
                                          sizeof (wpi_dps_hash)));
              if (level2_ptr == NULL)
                return WPI_ERROR (WP_ERR_IW_MACLOOKUP_ALLOCATION);
              /* local table initilization */
              WPI_memclr ((WP_U32 *) level2_ptr, sizeof (wpi_dps_hash) *
                          WPI_LOCAL_TABLE_SIZE);
              if (hash_mode == WPI_VPI_VCI_HASH_LOOKUP_MODE)
                for (ii = 0; ii < WPI_LOCAL_TABLE_SIZE; ii++)
                  {
                    W_MAC_CNTLS (level2_ptr[ii], WPI_ATM_NON_VALID);

                  }

              WPI_COPY_HASH_ENTRY (level2_ptr[mac_vc_l],
                                   level1_ptr[full_offset]);

              WPI_WRITE_HASH_ENTRY (&level2_ptr[L2_place], user_mac_entry,
                                    search_mode);
              W_MAC_L (level1_ptr[full_offset], WPI_ENTRY_COUNT_INIT);

              /* Set the occupation */
              occupation = (1 << L2_place) | (1 << mac_vc_l);
              W_MAC_H(level1_ptr[full_offset], occupation);

              /*  Concatenation of L1 to L2 : */
              W_L2_PTR (wpid, &level1_ptr[full_offset], level2_ptr);
              if (MAC_MODE (hash_mode))
                {

                  W_MAC_CNTLS (level1_ptr[full_offset], WPI_MAC_COLLISION);
                }

              else
                W_MAC_CNTLS (level1_ptr[full_offset], WPI_ATM_COLLISION);
              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /* INS9  */
            }
        }
    }
}


WP_status WPI_UserDefIwMacAddressInsert (WP_handle iw_system,
                                  wpi_dps_hash *user_mac_entry,
                                  WP_U8 search_mode,
                                  wpi_dps_hash **matched_dps_hash,
                                  void *hash_handler_object)
{
  WP_U32  match_found, mac_vc_l = 0, ii;
  WP_U32 full_offset;
  WP_U16 L2_place;
  wpi_dps_hash *level1_ptr, *prev;
  wpi_dps_hash *level2_ptr = NULL;
  wpi_dps_hash *LinkList_ptr;
  WP_U32 wpid;
  WP_status status, lock_status = WP_OK;
  WP_U16 out_port = 0, cntrl = 0;
  WP_U32 ret_val = 0;
  WP_U8 hash_mode = 0;

  *matched_dps_hash = NULL;/*default value in case no match was found*/

  WPI_RETURN_IF_ERROR(status,WPI_UserDefGetHashParams(iw_system,
                                               &user_mac_entry,
                                               &level1_ptr,
                                               &hash_mode,
                                               &full_offset,
                                               &L2_place,
                                               search_mode,
                                               hash_handler_object));
     wpid  = 0;

#ifdef STATISTICS
  insert_count++;
#endif

  WPI_LOG_MT (WPI_DEBUG_LEVEL_9, "return WPI_GetHashParams ");
  WPI_LOG_MT (WPI_DEBUG_LEVEL_9,
              "level1_ptr %x\n  ",
              (WP_U32)level1_ptr);

   //printf("full_offset = %d level1_ptr = 0x%x\n",full_offset ,level1_ptr);

  if (((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_MAC_NON_VALID) && MAC_MODE (hash_mode)) ||
      ((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_ATM_NON_VALID) && ATM_MODE (hash_mode)))
  {                           /*  insertion of new block to level 1: */
     /* L1 was available.  Occupation not used at this time. */
     WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 1 ");
     if (search_mode == WPI_IW_HASH_SEARCH)
     {
        if (MAC_MODE(hash_mode))
           return (WPI_MAC_NON_VALID);
        if (ATM_MODE(hash_mode))
           return (WPI_ATM_NON_VALID);
     }

     WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Insert 1 -- Mac ++ ");
     WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                   search_mode,
                                   hash_mode,
                                   wpid);
     WPI_WRITE_HASH_ENTRY(&level1_ptr[full_offset],
                          user_mac_entry,search_mode);
     return WP_OK;             /*  INS1  */
  }
  else /*   left main node of flow    */
  if (((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_MAC_COLLISION)
       && MAC_MODE (hash_mode)) ||
      ((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_ATM_COLLISION)
       && ATM_MODE (hash_mode)))
    {
      level2_ptr =
        ((wpi_dps_hash
          *) (R_L2_PTR (wpid, &level1_ptr[full_offset], hash_mode)));

      if (((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_MAC_NON_VALID) && MAC_MODE (hash_mode)) ||
          ((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_ATM_NON_VALID) && ATM_MODE (hash_mode)))
        {                       /* insertion of new block to level 2: */
           /* L2_place was not previously occupied.  Set the L2_place bit in L1 */
           WP_U32 occupation = R_MAC_H(level1_ptr[full_offset]);
           occupation |= 1 << L2_place;
           W_MAC_H(level1_ptr[full_offset], occupation);

          WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 2 ");
          if (search_mode == WPI_IW_HASH_SEARCH)
          {
             if (MAC_MODE(hash_mode))
                return (WPI_MAC_NON_VALID);
             if (ATM_MODE(hash_mode))
                return (WPI_ATM_NON_VALID);
          }
          if (((R_MAC_H (level2_ptr[L2_place]) != 0) && ATM_MODE (hash_mode)))
          {
             WPI_WRITE_HASH_ENTRY (&level2_ptr[L2_place],
                                   user_mac_entry,
                                   search_mode);
          }
          else
          {
             WPI_WRITE_HASH_ENTRY (&level2_ptr[L2_place],
                                   user_mac_entry,
                                   search_mode);
             W_MAC_L (level1_ptr[full_offset],
                      R_MAC_L (level1_ptr[full_offset]) + 1);
          }
          WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                        search_mode,
                                        hash_mode,
                                        wpid);
          return WP_OK;         /*  INS2  */
        }
      else
        if (((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_MAC_COLLISION)
             && MAC_MODE (hash_mode))
            || ((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_ATM_COLLISION)
                && ATM_MODE (hash_mode)))
        {
           /* L2 collision already occurred, so L1 already has occupied bit set */
          wpi_dps_hash *current;

          /* search the link list */
          prev = WPI_SearchLinkList(wpid,
                                    &level2_ptr[L2_place],
                                    user_mac_entry,
                                    &current,
                                    &match_found,
                                    hash_mode,
                                    search_mode);

          if (match_found == WP_TRUE)
            {   /* change of Control Word of item inside linked list */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 3 ");

              if (search_mode != WPI_IW_HASH_SEARCH)
              {
                 /* Linked-list   Case -
                    switching mac between bports
                    -- Decrement current max_address per bport  */

                 WPI_CURRENT_MAC_PER_BPORT_DEC(current,
                                               search_mode,
                                               hash_mode,
                                               WP_TRUE,
                                               wpid);
                 WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                               search_mode,
                                               hash_mode,
                                               wpid);

                 if(ATM_MODE (hash_mode))
                 {
                    if((search_mode & WPI_HASH_DONT_REPLACE) &&
                       (R_MAC_CNTLS(*current)!= WPI_ATM_NON_VALID))
                       return WPI_ERROR(WP_ERR_CH_ALREADY_CREATED);
                 }
                 WPI_UPDATE_HASH_ENTRY (current,
                                        user_mac_entry,
                                        search_mode,
                                        wpid);
              }
              else
              {
                  /*the matched wpi_dps_hash entry is returned by reference*/
                  *matched_dps_hash = current;
                  out_port = R_OUT_PORT (*current);
                  cntrl = R_MAC_CNTLS (*current);
                  ret_val = cntrl << 16 | out_port;
                  return ret_val;
              }
              return WP_OK;     /*  INS3   */
            }
          else
            {
              /* insertion of a new item to the end of linked list: */
              /* L1 already knows the L2 slot is occupied, so nothing to do. */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 4 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }
              status =
                 WPI_IwInsertHashLinkList(wpid, prev, user_mac_entry,
                                          hash_mode,search_mode);
              if (status != WP_OK)
                 return WPI_ERROR (WP_ERR_IW_MACLOOKUP_ALLOCATION);
              W_MAC_L (level2_ptr[L2_place],
                       R_MAC_L (level2_ptr[L2_place]) + 1);
              W_MAC_L (level1_ptr[full_offset],
                       R_MAC_L (level1_ptr[full_offset]) + 1);
              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /* INS4 */
            }
        }
      else                      /* exact match  */
        {
          if (WPI_KEY_IS_EXIST(level2_ptr[L2_place],
                               user_mac_entry,
                               search_mode))
           {
              /* change of Control Word in level 2 */
              /* Item exists already, so L1 knows L2_place is occupied. */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 5 ");
              if (search_mode != WPI_IW_HASH_SEARCH)
                {
                   /* level 2--
                      Case - switching mac between bports
                      -- Decrement current max_address per bport  */

                   WPI_CURRENT_MAC_PER_BPORT_DEC(&level2_ptr[L2_place],
                                                 search_mode,
                                                 hash_mode,
                                                 WP_TRUE,
                                                 wpid);
                   WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                                 search_mode,
                                                 hash_mode,
                                                 wpid);

                   if(ATM_MODE (hash_mode))
                   {
                      if((search_mode & WPI_HASH_DONT_REPLACE) &&
                         (R_MAC_CNTLS(level2_ptr[L2_place]) != WPI_ATM_NON_VALID))
                         return WPI_ERROR(WP_ERR_CH_ALREADY_CREATED);
                   }

                   WPI_UPDATE_HASH_ENTRY (&level2_ptr[L2_place],
                                          user_mac_entry,
                                          search_mode,
                                          wpid);
                }
              else
                {
                  /*the matched wpi_dps_hash entry is returned by reference*/
                  *matched_dps_hash = &level2_ptr[L2_place];
                  out_port = R_OUT_PORT (level2_ptr[L2_place]);
                  cntrl = R_MAC_CNTLS (level2_ptr[L2_place]);
                  ret_val = cntrl << 16 | out_port;
                  return ret_val;
                }
              return WP_OK;     /* INS5 */
            }
          else
            {
             /* Creation of linked list (only) & insertion of the items: */
              /* L1 already knows L2_place is occupied, so nothing to do. */
              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 6 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }
              /*Creation of a linked list with 2 items */
              WPI_RETURN_IF_ERROR(status,
                                  WPI_CreateLinkList (&LinkList_ptr,
                                                      wpid,
                                                      &level2_ptr[L2_place],
                                                      user_mac_entry,
                                                      hash_mode,
                                                      search_mode));
               /* counter change in L1 */
              W_MAC_L (level1_ptr[full_offset],
                       R_MAC_L (level1_ptr[full_offset]) + 1);
              /*Concatenation of L2 to LL */
              W_LL_PTR_FROM_L2 (wpid, &level2_ptr[L2_place], LinkList_ptr);
              if (MAC_MODE (hash_mode))
                {
                  W_MAC_CNTLS (level2_ptr[L2_place], WPI_MAC_COLLISION);
                }
              else
                 W_MAC_CNTLS (level2_ptr[L2_place], WPI_ATM_COLLISION);
              /* counter change in L2 */
              W_MAC_L (level2_ptr[L2_place], WPI_ENTRY_COUNT_INIT);

              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /*  INS6  */
            }
        }
    }
  else
    {                           /*  right main node of flow -- exact match */
       if (WPI_KEY_IS_EXIST(level1_ptr[full_offset],
                            user_mac_entry,
                            search_mode))
        {                       /* changing of Control Word in L1 */
          WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 7 ");
          if (search_mode != WPI_IW_HASH_SEARCH)
            {
               /*  level1  Case -
                   switching mac between bports  --
                   Decrement current max_address per bport  */
           /* L1 points to same item, so occupation already known. */
               WPI_CURRENT_MAC_PER_BPORT_DEC(&level1_ptr[full_offset],
                                             search_mode,
                                             hash_mode,
                                             WP_TRUE,
                                             wpid);
               WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                             search_mode,
                                             hash_mode,
                                             wpid);

               if(ATM_MODE (hash_mode))
               {
                  if((search_mode & WPI_HASH_DONT_REPLACE) &&
                     (R_MAC_CNTLS(level1_ptr[full_offset]) != WPI_ATM_NON_VALID))
                     return WPI_ERROR(WP_ERR_CH_ALREADY_CREATED);
               }

               WPI_UPDATE_HASH_ENTRY (&level1_ptr[full_offset],
                                      user_mac_entry,
                                      search_mode,
                                      wpid);
            }
          else
            {
              *matched_dps_hash = &level1_ptr[full_offset];
              out_port = R_OUT_PORT (level1_ptr[full_offset]);
              cntrl = R_MAC_CNTLS (level1_ptr[full_offset]);
              ret_val = cntrl << 16 | out_port;
              return ret_val;
            }
          return WP_OK;         /*  INS7  */
        }
      else
      {

         WPI_PTR tmp_ptr;

         if (L2_place ==
             (mac_vc_l =
              (((MAC_MODE (hash_mode)) ? R_MAC_L (level1_ptr[full_offset]) :
                R_MAC_H (level1_ptr[full_offset])) % WPI_LOCAL_TABLE_SIZE)))
         {  /* The Element in level1 collides with the new Element
               In level 1 and level 2 as well. As a resault we
               create a level 2 and linked-list with 2 elements.*/
           /* L1 points to a different item, with same L2_place.  Set
            * L1 occupied at L2_place. */
            WP_U32 occupation;
            status = WPI_IwMemFreeListGet(&tmp_ptr,
                                          NULL,
                                          wpid,
                                          LEVEL1_HASH_MEM_FREE_LIST);
            WPI_RETURN_IF_ERROR_STATUS(status);

            level2_ptr = (wpi_dps_hash *)tmp_ptr;
            if(level2_ptr == NULL)
               level2_ptr =
                  ((wpi_dps_hash
                    *) (WPI_host_malloc (wpid, WP_VB_HASH_IW_LPM,
                                         sizeof (wpi_dps_hash) *
                                         WPI_LOCAL_TABLE_SIZE,
                                         sizeof (wpi_dps_hash))));
              if (level2_ptr == NULL)
                 return WPI_ERROR (WP_ERR_IW_MACLOOKUP_ALLOCATION);
              /* local table initilization */
              WPI_memclr ((WP_U32 *) level2_ptr, sizeof (wpi_dps_hash) *
                          WPI_LOCAL_TABLE_SIZE);
              if (hash_mode == WPI_VPI_VCI_HASH_LOOKUP_MODE)
                for (ii = 0; ii < WPI_LOCAL_TABLE_SIZE; ii++)
                  {
                    W_MAC_CNTLS (level2_ptr[ii], WPI_ATM_NON_VALID);

                  }

              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 8 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }
              /*Creation of a linked list with 2 items */
              WPI_RETURN_IF_ERROR(status,
                                  WPI_CreateLinkList(&LinkList_ptr,
                                                     wpid,
                                                     &level1_ptr[full_offset]
                                                     ,user_mac_entry,
                                                     hash_mode,
                                                     search_mode));

              W_MAC_L (level2_ptr[L2_place], WPI_ENTRY_COUNT_INIT);
              if (MAC_MODE (hash_mode))
              {
                 W_MAC_CNTLS (level2_ptr[L2_place], WPI_MAC_COLLISION);
              }
              else
              {
                 W_MAC_CNTLS (level2_ptr[L2_place], WPI_ATM_COLLISION);
              }
              /* Concatenation of L2 to LL */
              W_LL_PTR_FROM_L2 (wpid, &level2_ptr[L2_place], LinkList_ptr);

              W_L2_PTR (wpid, &level1_ptr[full_offset], level2_ptr);
              if (MAC_MODE (hash_mode))
              {
                 W_MAC_CNTLS (level1_ptr[full_offset], WPI_MAC_COLLISION);
              }
              else
              {
                 W_MAC_CNTLS (level1_ptr[full_offset], WPI_ATM_COLLISION);
              }

              W_MAC_L (level1_ptr[full_offset], WPI_ENTRY_COUNT_INIT);
              /* set occupation */
              occupation = (1 << L2_place);
              W_MAC_H(level1_ptr[full_offset], occupation);

              /* Concatenation of L1 to L2 */

              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /*  INS8  */
            }
          else
            {
               WPI_PTR tmp_ptr;
               WP_U32 occupation;

               /* Insertion of the 2 items to a new level 2 */
               /* Also, set occupation for both L2_place and mac_vc_l */

              WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Insert 9.0 ");
              if (search_mode == WPI_IW_HASH_SEARCH)
              {
                 if (MAC_MODE(hash_mode))
                    return (WPI_MAC_NON_VALID);
                 if (ATM_MODE(hash_mode))
                    return (WPI_ATM_NON_VALID);
              }

              status = WPI_IwMemFreeListGet(&tmp_ptr,
                                            NULL,
                                            wpid,
                                            LEVEL1_HASH_MEM_FREE_LIST);
              WPI_RETURN_IF_ERROR_STATUS(status);

              level2_ptr = (wpi_dps_hash *)tmp_ptr;

              if(level2_ptr == NULL)
                 level2_ptr =
                    (wpi_dps_hash
                     *) (WPI_host_malloc (wpid, WP_VB_HASH_IW_LPM,
                                          sizeof (wpi_dps_hash) *
                                          WPI_LOCAL_TABLE_SIZE,
                                          sizeof (wpi_dps_hash)));
              if (level2_ptr == NULL)
                return WPI_ERROR (WP_ERR_IW_MACLOOKUP_ALLOCATION);
              /* local table initilization */
              WPI_memclr ((WP_U32 *) level2_ptr, sizeof (wpi_dps_hash) *
                          WPI_LOCAL_TABLE_SIZE);
              if (hash_mode == WPI_VPI_VCI_HASH_LOOKUP_MODE)
                for (ii = 0; ii < WPI_LOCAL_TABLE_SIZE; ii++)
                  {
                    W_MAC_CNTLS (level2_ptr[ii], WPI_ATM_NON_VALID);

                  }

              WPI_COPY_HASH_ENTRY (level2_ptr[mac_vc_l],
                                   level1_ptr[full_offset]);

              WPI_WRITE_HASH_ENTRY (&level2_ptr[L2_place], user_mac_entry,
                                    search_mode);
              W_MAC_L (level1_ptr[full_offset], WPI_ENTRY_COUNT_INIT);

              /* Set the occupation */
              occupation = (1 << L2_place) | (1 << mac_vc_l);
              W_MAC_H(level1_ptr[full_offset], occupation);

              /*  Concatenation of L1 to L2 : */
              W_L2_PTR (wpid, &level1_ptr[full_offset], level2_ptr);
              if (MAC_MODE (hash_mode))
                {

                  W_MAC_CNTLS (level1_ptr[full_offset], WPI_MAC_COLLISION);
                }

              else
                W_MAC_CNTLS (level1_ptr[full_offset], WPI_ATM_COLLISION);
              WPI_CURRENT_MAC_PER_BPORT_INC(user_mac_entry,
                                            search_mode,
                                            hash_mode,
                                            wpid);
              return WP_OK;     /* INS9  */
            }
        }
    }
}

WP_status WPI_UserDefIwMacAddressRemove (WP_handle iw_system,
                                  wpi_dps_hash * user_mac_entry,
                                  WP_U8 search_mode,
                                  void *hash_handler_object)
{
  WP_U32 full_offset;
  WP_U16 L2_place;
  WP_U32 found_match = WP_FALSE;
  wpi_dps_hash *level1_ptr;
  wpi_dps_hash *level2_ptr;
  WP_U32 wpid;
  wpi_dps_hash *prev;
  WP_U8 hash_mode = 0;
  WP_status status, lock_status = WP_OK;

  status = WPI_UserDefGetHashParams(iw_system,
                             &user_mac_entry,
                             &level1_ptr,
                             &hash_mode,
                             &full_offset,
                             &L2_place,
                             search_mode,
                             hash_handler_object);
  if (WP_ERROR_P(status))
     return status;
  wpid  = WPI_HANDLE_WPID( iw_system);

  if (((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_MAC_COLLISION)
       && MAC_MODE (hash_mode)) ||
      ((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_ATM_COLLISION)
       && ATM_MODE (hash_mode)))
    {
      level2_ptr =
        ((wpi_dps_hash *)
         R_L2_PTR (wpid, &level1_ptr[full_offset], hash_mode));

      if (((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_MAC_COLLISION)
           && MAC_MODE (hash_mode))
          || ((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_ATM_COLLISION)
              && ATM_MODE (hash_mode)))
      {
         wpi_dps_hash *current;

         /* Collision in L2, so no change in occupation of L2 */
         /* scan the linked list */
         prev = WPI_SearchLinkList (wpid,
                                    &level2_ptr[L2_place],
                                    user_mac_entry,
                                    &current,
                                    &found_match,
                                    hash_mode,
                                    search_mode);

         if (found_match == WP_TRUE)   /*  match */
         {
            WP_U32 new_ptr;
            WPI_PTR cntrl_bit;

            /* read ptr */
            WPI_MEM_GET(new_ptr,current->ptr);
            /* read cntrl */
            WPI_MEM_GET(cntrl_bit,prev->ptr);
            /* Modify cntrl */
            new_ptr = (new_ptr & 0xfffffff0) | (cntrl_bit & 0xf);
            /* Write ptr + cntrl */
            WPI_MEM_SET(prev->ptr,new_ptr);

            WPI_CURRENT_MAC_PER_BPORT_DEC(current,
                                          search_mode,
                                          hash_mode,
                                          found_match,
                                          wpid);

            WPI_CLEAR_HASH_ENTRY(*current,hash_mode);

            status = WPI_IwMemFreeListPut((WPI_PTR)current,
                                          NULL,
                                          wpid,
                                          LEVEL2_HASH_MEM_FREE_LIST);
            WPI_RETURN_IF_ERROR_STATUS(status);

            /*  Case - linked-list
                -- Decrement current max_address per bport  */

            if (R_MAC_L (level2_ptr[L2_place]) == 2)
            {               /* only 2  items in linked list */
               wpi_dps_hash *head;
               head=(wpi_dps_hash *)R_L2_PTR(wpid,
                                             &level2_ptr[L2_place],
                                             hash_mode);

               WPI_COPY_HASH_ENTRY (level2_ptr[L2_place],
                                    *head);

               status = WPI_IwMemFreeListPut((WPI_PTR)head,
                                             NULL,
                                             wpid,
                                             LEVEL2_HASH_MEM_FREE_LIST);
               WPI_RETURN_IF_ERROR_STATUS(status);

               if (R_MAC_L (level1_ptr[full_offset]) == 2)
               {
                  WP_status status;
                  WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                              " Remove 3.0\n ");
                  status = WPI_IwPopToL2 (level1_ptr,
                                 level2_ptr,
                                 user_mac_entry,
                                 full_offset,
                                 hash_mode);
                  if (status != WP_OK)
                     return status;
                  status = WPI_IwMemFreeListPut((WPI_PTR)level2_ptr,
                                                NULL,
                                                wpid,
                                                LEVEL1_HASH_MEM_FREE_LIST);
                  WPI_RETURN_IF_ERROR_STATUS(status);
                  /* DEL3 */
               }
               else
               {
                  WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 4.0\n ");
                  W_MAC_L (level1_ptr[full_offset],
                           R_MAC_L (level1_ptr[full_offset]) - 1);/*DEL4*/
               }
            }
            else
            {               /* DEL5 */
               WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 5.0\n ");
               W_MAC_L (level2_ptr[L2_place],
                        R_MAC_L (level2_ptr[L2_place]) - 1);
               W_MAC_L (level1_ptr[full_offset],
                        R_MAC_L (level1_ptr[full_offset]) - 1);
            }
            return WP_OK;
         }
         else
         {
            WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 6.0 ");
            return WP_ERR_NOT_EXIST_IN_HASH;
            /* DEL6-search error */
         }
      }
      else
      {
             /* Begining of node 2 (right down) exact match */
         if (WPI_KEY_IS_EXIST(level2_ptr[L2_place],
                              user_mac_entry,
                              search_mode))
         {
            /* Matches in L2, so remove occupation in L1 */
            WP_U32 occupation = R_MAC_H(level1_ptr[full_offset]);
            occupation &= ~(1 << L2_place);
            W_MAC_H(level1_ptr[full_offset], occupation);

            /*  Case - second level
                -- Decrement current max_address per bport  */
            WPI_CURRENT_MAC_PER_BPORT_DEC(&level2_ptr[L2_place],
                                          search_mode,
                                          hash_mode,
                                          WP_TRUE,
                                          wpid);

            WPI_CLEAR_HASH_ENTRY (level2_ptr[L2_place], hash_mode);

            /* remove last block from L2 & folding of this L2 */
            if (R_MAC_L (level1_ptr[full_offset]) == 2)
            {
               WP_status status;
               WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 7.0\n ");
               status = WPI_IwPopToL2 (level1_ptr,
                              level2_ptr,
                              user_mac_entry,
                              full_offset,
                              hash_mode);/* DEL7 */
               WPI_RETURN_IF_ERROR_STATUS(status);
               status = WPI_IwMemFreeListPut((WPI_PTR)level2_ptr,
                                             NULL,
                                             wpid,
                                             LEVEL1_HASH_MEM_FREE_LIST);
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
            else
            {
               WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 8.0\n ");
               W_MAC_L (level1_ptr[full_offset],
                        R_MAC_L (level1_ptr[full_offset]) - 1);
            }               /* DEL8 */
            return WP_OK;
         }
         else
         {
            WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 9.0 ");
            return WP_ERR_NOT_EXIST_IN_HASH;
            /* DEL9  - search error */
         }
      }
    }
  else
  {
               /*  Begining of node 3 (up right) exact match */
     if (WPI_KEY_IS_EXIST(level1_ptr[full_offset],
                          user_mac_entry,
                          search_mode))
     {                       /* remove of cell from L1 */
        WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 10.0 \n");
        /*  Case - first level
            -- Decrement current max_address per bport  */
        WPI_CURRENT_MAC_PER_BPORT_DEC(&level1_ptr[full_offset],
                                      search_mode,
                                      hash_mode,
                                      WP_TRUE,
                                      wpid);
        WPI_CLEAR_HASH_ENTRY (level1_ptr[full_offset], hash_mode);
        return WP_OK;         /*  DEL10  */
     }
     else
     {
        WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 11.0 ");
        return WP_ERR_NOT_EXIST_IN_HASH;
        /*  DEL11 - search error */
     }
  }
}

/*********************************************************************

   function name :  WPI_IwMacAddressRemove

   parameters :  WP_U32 long MAC_Adress

   description : The function deletes from the hash table a mac addres and its
                      control word that point on l2 interworking
                      table that points on the flow interworking table
                      that points on a queue assignment table that points
                      on the transmit buffer descriptor .

   called by : user ( main )

   input :

   output :returns WPI_MAC_NON_VALID incase of an invalid search
           (an error in the MAC address).

********************************************************************/

WP_status WPI_IwMacAddressRemove (WP_handle iw_system,
                                  wpi_dps_hash * user_mac_entry,
                                  WP_U8 search_mode,
                                  void *hash_handler_object)
{
  WP_U32 full_offset;
  WP_U16 L2_place;
  WP_U32 found_match = WP_FALSE;
  wpi_dps_hash *level1_ptr;
  wpi_dps_hash *level2_ptr;
  WP_U32 wpid;
  wpi_dps_hash *prev;
  WP_U8 hash_mode = 0;
  WP_status status, lock_status = WP_OK;

  status = WPI_GetHashParams(iw_system,
                             &user_mac_entry,
                             &level1_ptr,
                             &hash_mode,
                             &full_offset,
                             &L2_place,
                             search_mode,
                             hash_handler_object);
  if (WP_ERROR_P(status))
     return status;
  wpid  = WPI_HANDLE_WPID( iw_system);

  if (((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_MAC_COLLISION)
       && MAC_MODE (hash_mode)) ||
      ((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_ATM_COLLISION)
       && ATM_MODE (hash_mode)))
    {
      level2_ptr =
        ((wpi_dps_hash *)
         R_L2_PTR (wpid, &level1_ptr[full_offset], hash_mode));

      if (((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_MAC_COLLISION)
           && MAC_MODE (hash_mode))
          || ((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_ATM_COLLISION)
              && ATM_MODE (hash_mode)))
      {
         wpi_dps_hash *current;

         /* Collision in L2, so no change in occupation of L2 */
         /* scan the linked list */
         prev = WPI_SearchLinkList (wpid,
                                    &level2_ptr[L2_place],
                                    user_mac_entry,
                                    &current,
                                    &found_match,
                                    hash_mode,
                                    search_mode);

         if (found_match == WP_TRUE)   /*  match */
         {
            WP_U32 new_ptr;
            WPI_PTR cntrl_bit;

            /* read ptr */
            WPI_MEM_GET(new_ptr,current->ptr);
            /* read cntrl */
            WPI_MEM_GET(cntrl_bit,prev->ptr);
            /* Modify cntrl */
            new_ptr = (new_ptr & 0xfffffff0) | (cntrl_bit & 0xf);
            /* Write ptr + cntrl */
            WPI_MEM_SET(prev->ptr,new_ptr);

            WPI_CURRENT_MAC_PER_BPORT_DEC(current,
                                          search_mode,
                                          hash_mode,
                                          found_match,
                                          wpid);

            WPI_CLEAR_HASH_ENTRY(*current,hash_mode);

            status = WPI_IwMemFreeListPut((WPI_PTR)current,
                                          NULL,
                                          wpid,
                                          LEVEL2_HASH_MEM_FREE_LIST);
            WPI_RETURN_IF_ERROR_STATUS(status);

            /*  Case - linked-list
                -- Decrement current max_address per bport  */

            if (R_MAC_L (level2_ptr[L2_place]) == 2)
            {               /* only 2  items in linked list */
               wpi_dps_hash *head;
               head=(wpi_dps_hash *)R_L2_PTR(wpid,
                                             &level2_ptr[L2_place],
                                             hash_mode);

               WPI_COPY_HASH_ENTRY (level2_ptr[L2_place],
                                    *head);

               status = WPI_IwMemFreeListPut((WPI_PTR)head,
                                             NULL,
                                             wpid,
                                             LEVEL2_HASH_MEM_FREE_LIST);
               WPI_RETURN_IF_ERROR_STATUS(status);

               if (R_MAC_L (level1_ptr[full_offset]) == 2)
               {
                  WP_status status;
                  WPI_LOG_MT (WPI_DEBUG_LEVEL_3,
                              " Remove 3.0\n ");
                  status = WPI_IwPopToL2 (level1_ptr,
                                 level2_ptr,
                                 user_mac_entry,
                                 full_offset,
                                 hash_mode);
                  if (status != WP_OK)
                     return status;
                  status = WPI_IwMemFreeListPut((WPI_PTR)level2_ptr,
                                                NULL,
                                                wpid,
                                                LEVEL1_HASH_MEM_FREE_LIST);
                  WPI_RETURN_IF_ERROR_STATUS(status);
                  /* DEL3 */
               }
               else
               {
                  WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 4.0\n ");
                  W_MAC_L (level1_ptr[full_offset],
                           R_MAC_L (level1_ptr[full_offset]) - 1);/*DEL4*/
               }
            }
            else
            {               /* DEL5 */
               WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 5.0\n ");
               W_MAC_L (level2_ptr[L2_place],
                        R_MAC_L (level2_ptr[L2_place]) - 1);
               W_MAC_L (level1_ptr[full_offset],
                        R_MAC_L (level1_ptr[full_offset]) - 1);
            }
            return WP_OK;
         }
         else
         {
            WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 6.0 ");
            return WPI_ERROR(WP_ERR_NOT_EXIST_IN_HASH);
            /* DEL6-search error */
         }
      }
      else
      {
             /* Begining of node 2 (right down) exact match */
         if (WPI_KEY_IS_EXIST(level2_ptr[L2_place],
                              user_mac_entry,
                              search_mode))
         {
            /* Matches in L2, so remove occupation in L1 */
            WP_U32 occupation = R_MAC_H(level1_ptr[full_offset]);
            occupation &= ~(1 << L2_place);
            W_MAC_H(level1_ptr[full_offset], occupation);

            /*  Case - second level
                -- Decrement current max_address per bport  */
            WPI_CURRENT_MAC_PER_BPORT_DEC(&level2_ptr[L2_place],
                                          search_mode,
                                          hash_mode,
                                          WP_TRUE,
                                          wpid);

            WPI_CLEAR_HASH_ENTRY (level2_ptr[L2_place], hash_mode);

            /* remove last block from L2 & folding of this L2 */
            if (R_MAC_L (level1_ptr[full_offset]) == 2)
            {
               WP_status status;
               WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 7.0\n ");
               status = WPI_IwPopToL2 (level1_ptr,
                              level2_ptr,
                              user_mac_entry,
                              full_offset,
                              hash_mode);/* DEL7 */
               WPI_RETURN_IF_ERROR_STATUS(status);
               status = WPI_IwMemFreeListPut((WPI_PTR)level2_ptr,
                                             NULL,
                                             wpid,
                                             LEVEL1_HASH_MEM_FREE_LIST);
               WPI_RETURN_IF_ERROR_STATUS(status);
            }
            else
            {
               WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 8.0\n ");
               W_MAC_L (level1_ptr[full_offset],
                        R_MAC_L (level1_ptr[full_offset]) - 1);
            }               /* DEL8 */
            return WP_OK;
         }
         else
         {
            WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 9.0 ");
            return WPI_ERROR(WP_ERR_NOT_EXIST_IN_HASH);
            /* DEL9  - search error */
         }
      }
    }
  else
  {
               /*  Begining of node 3 (up right) exact match */
     if (WPI_KEY_IS_EXIST(level1_ptr[full_offset],
                          user_mac_entry,
                          search_mode))
     {                       /* remove of cell from L1 */
        WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 10.0 \n");
        /*  Case - first level
            -- Decrement current max_address per bport  */
        WPI_CURRENT_MAC_PER_BPORT_DEC(&level1_ptr[full_offset],
                                      search_mode,
                                      hash_mode,
                                      WP_TRUE,
                                      wpid);
        WPI_CLEAR_HASH_ENTRY (level1_ptr[full_offset], hash_mode);
        return WP_OK;         /*  DEL10  */
     }
     else
     {
        WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " Remove 11.0 ");
        return WPI_ERROR(WP_ERR_NOT_EXIST_IN_HASH);
        /*  DEL11 - search error */
     }
  }
}

WP_status WPI_UserHashIwMacAddressFind (WP_handle iw_system,
                                  wpi_dps_hash * user_mac_entry,
                                  void *hash_handler_object,
                                  wpi_dps_hash * user_entry)
	{
	  WP_U32 full_offset;
	  WP_U16 L2_place;
	  WP_U32 found_match = WP_FALSE;
	  wpi_dps_hash *level1_ptr;
	  wpi_dps_hash *level2_ptr;
	  WP_U32 wpid;
	  wpi_dps_hash *prev;
	  WP_U8 hash_mode = 0;
	  WP_status status = WP_OK;//, lock_status = WP_OK;
	  WP_U8 search_mode = WPI_IW_FR_HASH_INSERT;
	  status = WPI_UserDefGetHashParams(iw_system,
								 &user_mac_entry,
								 &level1_ptr,
								 &hash_mode,
								 &full_offset,
								 &L2_place,
								 search_mode,
								 hash_handler_object);
	  if (WP_ERROR_P(status))
		 return status;
	  wpid	= WPI_HANDLE_WPID( iw_system);
	
	  if (((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_MAC_COLLISION)
		   && MAC_MODE (hash_mode)) ||
		  ((R_MAC_CNTLS (level1_ptr[full_offset]) == WPI_ATM_COLLISION)
		   && ATM_MODE (hash_mode)))
		{
		  level2_ptr =
			((wpi_dps_hash *)
			 R_L2_PTR (wpid, &level1_ptr[full_offset], hash_mode));
	
		  if (((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_MAC_COLLISION)
			   && MAC_MODE (hash_mode))
			  || ((R_MAC_CNTLS (level2_ptr[L2_place]) == WPI_ATM_COLLISION)
				  && ATM_MODE (hash_mode)))
		  {
			 wpi_dps_hash *current;
	
			 /* Collision in L2, so no change in occupation of L2 */
			 /* scan the linked list */
			 prev = WPI_SearchLinkList (wpid,
										&level2_ptr[L2_place],
										user_mac_entry,
										&current,
										&found_match,
										hash_mode,
										search_mode);
	
			 if (found_match == WP_TRUE)   /*  match */
			 {
				//WPI_CLEAR_HASH_ENTRY(*current,hash_mode);
				WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " it exist in level 2 list \n");
				WPI_USR_COPY_HASH_ENTRY(*user_entry, *current);
				
				return WP_OK;
			 }
			 else
			 {
				WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " not exit 6.0 ");
				return WP_ERR_NOT_EXIST_IN_HASH;
				/* DEL6-search error */
			 }
		  }
		  else
		  {
				 /* Begining of node 2 (right down) exact match */
			 if (WPI_KEY_IS_EXIST(level2_ptr[L2_place],
								  user_mac_entry,
								  search_mode))
			 {
				//WPI_CLEAR_HASH_ENTRY (level2_ptr[L2_place], hash_mode);
				WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " it exist in level 2 \n");
				WPI_USR_COPY_HASH_ENTRY(*user_entry, level2_ptr[L2_place]);
				return WP_OK;
			 }
			 else
			 {
				WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " not exist 9.0 ");
				return WP_ERR_NOT_EXIST_IN_HASH;
				/* DEL9  - search error */
			 }
		  }
		}
	  else
	  {
				   /*  Begining of node 3 (up right) exact match */
		 if (WPI_KEY_IS_EXIST(level1_ptr[full_offset],
							  user_mac_entry,
							  search_mode))
		 {						 /* remove of cell from L1 */
			WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " it exist in level 1 \n");
			//WPI_CLEAR_HASH_ENTRY (level1_ptr[full_offset], hash_mode);
			WPI_USR_COPY_HASH_ENTRY(*user_entry, level1_ptr[full_offset]);
			return WP_OK;		  /*  DEL10  */
		 }
		 else
		 {
			WPI_LOG_MT (WPI_DEBUG_LEVEL_3, " not exist 11.0 ");
			return WP_ERR_NOT_EXIST_IN_HASH;
			/*	DEL11 - search error */
		 }
	  }
	}

WP_status WP_UserDefHashTableCreate(WP_U32 wpid,
						  WP_U32 n_entries)
{
  WP_status lock_status = WP_OK;
  WPI_PTR hash_base;
  WPI_PTR Ta_base;
  wpi_dps_hash *host_ptr;
  wpi_memory *memory;
  if(n_entries%16 != 0)
  {
      return -1;
  }
  WPI_WDDI_ATTACH();
  memory = WPI_RegistryMemory(wpid);
  Ta_base = WPI_malloc(WPI_MemoryVbankOfVirtualBus(memory, WP_VB_HASH_IW_LPM),
						 (sizeof(wpi_dps_hash) * (n_entries+1)),/*first wpi_dps_hash memory is for save hash memory base addr*/
						 sizeof(wpi_dps_hash), "hash table");
  WPI_RETURN_IF_WPI_NULL(Ta_base, WP_ERR_MEMORY_ALLOCATION);
  hash_base = Ta_base+sizeof(wpi_dps_hash);  
  host_ptr = (wpi_dps_hash *)
	 WPI_HOST_PTR(Ta_base, WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_HASH_IW_LPM));
  printf("HashTableCreate Ta_base = 0x%x host_ptr = 0x%x\n",Ta_base,host_ptr);  
  WPI_HashClear(host_ptr,1,0);
  WPI_MEM_SET(host_ptr[0].ptr, hash_base);/*store the hash base*/
  WPI_MEM_SET(host_ptr[0].mac_h,n_entries-1);/*this is hash mask*/
  

  WPI_HashClear(host_ptr+1, n_entries, 0xffff);

  /*set TA*/
  wpi_res_reg_bases *res_reg_bases = NULL;
  wpi_ta *ta;
  
  WPI_REGISTRY_OBJECT_GET(lock_status,
						  WPI_RegistryRegisterBaseGet,
						  WPI_REG_LOCKED,
						  &res_reg_bases,
						  wpid);/*wp id =0;*/
  WPI_RETURN_IF_ERROR_STATUS(lock_status);
  
  ta =WPI_TaRegBaseGet(res_reg_bases->reg_bases_ptr);
  //param is bus 7, packet-6 , internal -5, host -4 , application - 3 ... 0 is FMU tx...
  WPI_TaRegisterBSet(ta, WPI_TA_B_CUSTOMER2, Ta_base | 0x7);
  WPI_REGISTRY_OBJECT_RELEASE(lock_status,
							  WPI_RegistryRegisterBaseRelease,
							  WPI_REG_UNLOCKED,
							  &res_reg_bases);
  WPI_RETURN_IF_ERROR_STATUS(lock_status);
  WPI_WDDI_DETACH();
  UserDefHashBase = (void  *) hash_base;
  return WP_OK;
}


/*********************************************************************

   function name : WPI_IwPopToL2

   parameters : wpi_dps_hash *folded_L2
                wpi_dps_hash removed_block

   description  :  moves the last block on the local table (L2)
                   ( the one that was not removed) to a higher level ,the main
                   hash table (L1).

   called by : WP_delete_hash

   input :

   output : WP_ERR_INCONSISTENT_HASH_TABLE if it doesn't find a bit.
            WP_OK                          if normal.

 ********************************************************************/
WP_status WPI_IwPopToL2 (wpi_dps_hash * level1_ptr,
                         wpi_dps_hash * folded_L2,
                         wpi_dps_hash * removed_block,
                         WP_U32 L1_place,
                         WP_U8 hash_mode)
{
  WP_U16 index;
  WP_U32 occupation = R_MAC_H(level1_ptr[L1_place]);

  /* occupation must be non-zero in last 8 bits and have just one bit set */
  occupation &= 0xff;

  /* Find the occupied L2 slot and return it to L1 */
  for (index = 0; index < 8; index++) {
     if (occupation & (1 << index)) {
        WPI_COPY_HASH_ENTRY (level1_ptr[L1_place], folded_L2[index]);
        break;
     }
  }

  if (index < 8)
     return WP_OK;
  return WPI_ERROR(WP_ERR_INCONSISTENT_HASH_TABLE);
}

/*********************************************************************

   function name : WPI_CreateLinkList

   parameters :  wpi_dps_hash *first_block
                 wpi_dps_hash *second_block

   description  : Creates a linked list for specific cell in of the
             local hash table.
             Pushes the first 2 items into the(the ones that collided in L2).

   called by : WP_insert_mac

   input :

   output : pointer to the wpi_dps_hash head

 ********************************************************************/
WP_status WPI_CreateLinkList(wpi_dps_hash **LL_ptr_head,
                             WP_U32 wpid,
                             wpi_dps_hash * first_block,
                             wpi_dps_hash * second_block,
                             WP_U8 hash_mode,
                             WP_U8 search_mode)
{
  wpi_dps_hash *LL_ptr_tail;
  WPI_PTR tmp_ptr;
  WP_U8 *host_offset;
  wpi_memory *memory;
  WP_status status = WP_OK;

  memory = WPI_RegistryMemory(wpid);
  host_offset = WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_HASH_IW_LPM);

  status = WPI_IwMemFreeListGet((WPI_PTR*)LL_ptr_head,
                                NULL,
                                wpid,
                                LEVEL2_HASH_MEM_FREE_LIST);
  WPI_RETURN_IF_ERROR_STATUS(status);

  if(*LL_ptr_head == NULL)
     *LL_ptr_head =
        (wpi_dps_hash
         *) (WPI_host_malloc (wpid, WP_VB_HASH_IW_LPM, sizeof (wpi_dps_hash),
                              sizeof (wpi_dps_hash)));
  if (*LL_ptr_head == NULL)
    return WPI_ERROR(WP_ERR_IW_MACLOOKUP_ALLOCATION);

  status = WPI_IwMemFreeListGet(&tmp_ptr,
                                NULL,
                                wpid,
                                LEVEL2_HASH_MEM_FREE_LIST);
  WPI_RETURN_IF_ERROR_STATUS(status);

  LL_ptr_tail = (wpi_dps_hash *)tmp_ptr;
  if(LL_ptr_tail == NULL)
     LL_ptr_tail =
        (wpi_dps_hash
         *) (WPI_host_malloc (wpid, WP_VB_HASH_IW_LPM, sizeof (wpi_dps_hash),
                              sizeof (wpi_dps_hash)));
  if (LL_ptr_tail == NULL)
    return WPI_ERROR(WP_ERR_IW_MACLOOKUP_ALLOCATION);

  WPI_COPY_HASH_ENTRY (*LL_ptr_tail, *first_block);

/* concatenation between items in linked list !! */

  second_block->ptr = (second_block->ptr & 0xf) |
     ((WPI_PTR)WPI_WPI_PTR(LL_ptr_tail,host_offset) &0xfffffff0);

  WPI_WRITE_HASH_ENTRY ((*LL_ptr_head), second_block,search_mode);

  if (!MAC_MODE (hash_mode))
      WPI_MEM_SET (*((WP_U32 *) LL_ptr_tail),0);

  return WP_OK;
}

/*********************************************************************

   function name : WPI_SearchLinkList

   parameters :  wpi_dps_hash *list_head
                 wpi_dps_hash *scanned_block
                 WP_boolean *match

   description  : searches the  link list for a match
                  (equality in MAC_L & MAC_H ).
                  !!! returns the item previous to the matched item !!!
                 changes the value of match in case of no match.

   called by : WP_insert_mac , WP_delete_mac

   input :

   output : A pointer to the item before the matched item or
            NULL in case of no match.

 ********************************************************************/
wpi_dps_hash*
WPI_SearchLinkList (WP_U32 wpid,
                    wpi_dps_hash *list_head,
                    wpi_dps_hash *scanned_block,
                    wpi_dps_hash **cur,
                    WP_U32 *match,
                    WP_U8 hash_mode,
                    WP_U8 search_mode)
{
  wpi_dps_hash *next_element_checked = list_head, *prev_block = list_head;
  WPI_PTR cur_ptr;
  *match = WP_FALSE;

  WPI_MEM_GET(cur_ptr,prev_block->ptr);
  cur_ptr &= STAT_MAC_AND_AGE_B_MASK;
  next_element_checked =
     (wpi_dps_hash *) R_LL_PTR (wpid, next_element_checked, hash_mode);

  WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " Start searching... ");

  while (cur_ptr != 0)
    {
       if (WPI_KEY_IS_EXIST(*next_element_checked,scanned_block,search_mode))
       {
          *match = WP_TRUE;
          break;
       }
       prev_block = (wpi_dps_hash *) next_element_checked;
       next_element_checked =
          (wpi_dps_hash *) R_LL_PTR (wpid, next_element_checked, hash_mode);
       WPI_MEM_GET(cur_ptr,prev_block->ptr);
       cur_ptr &= STAT_MAC_AND_AGE_B_MASK;
    }

  WPI_LOG_MT (WPI_DEBUG_LEVEL_9, " End of search ! ");
  *cur = next_element_checked;

  return prev_block;
}

/*********************************************************************

   function name : WPI_IwInsertHashLinkList

   parameters :     wpi_dps_hash *list_tail
                    wpi_dps_hash *new_block

   description  : Insert an element to the end of the linked list .

   called by : WP_insert_mac

   input :

   output : none

 ********************************************************************/

WP_status WPI_IwInsertHashLinkList (WP_U32 wpid,
                                    wpi_dps_hash * list_tail,
                                    wpi_dps_hash * new_block,
                                    WP_U8 hash_mode,
                                    WP_U8 search_mode)
{
  wpi_dps_hash *new_tail;
  WPI_PTR tmp_ptr;
  WP_status status = WP_OK;

  status = WPI_IwMemFreeListGet(&tmp_ptr,
                                NULL,
                                wpid,
                                LEVEL2_HASH_MEM_FREE_LIST);
  WPI_RETURN_IF_ERROR_STATUS(status);

  new_tail = (wpi_dps_hash *)tmp_ptr;
  if(new_tail == NULL)
     new_tail =
        (wpi_dps_hash*)
        (WPI_host_malloc (wpid, WP_VB_HASH_IW_LPM, sizeof (wpi_dps_hash),
                          sizeof (wpi_dps_hash)));

   if (new_tail == NULL)
    return WPI_ERROR (WP_ERR_IW_MACLOOKUP_ALLOCATION);

  WPI_WRITE_HASH_ENTRY (new_tail,new_block,search_mode);

/* Connect between new_tail to list_tail */
  W_LL_CONNECT(wpid,list_tail,new_tail);

  if (!MAC_MODE (hash_mode))
      WPI_MEM_SET (*((WP_U32 *) new_tail), 0);

  return WP_OK;
}

/*********************************************************************

   function name : WPI_update_crc32

   parameters :  WP_U32 crc32_accum
                 WP_U8 *data_blk_ptr

   description  : calculates the CRC result

   called by : WP_insert_mac , WP_delete_mac

   input : 6 bytes MAC_Address

   output : 2 bytes CRC calculation

 ********************************************************************/
WP_U32 WPI_update_crc32 (WP_U8 * data_blk_ptr)
{

  WP_U32 crc32_accum = 0;
#ifndef DEBUG_MODE
  WP_S32 i, j;
  WP_U32 crc32_table_entry = 0;

  for (j = 0; j < WPI_CRC_BLOCK_NUM; ++j)
    {
       i = (((crc32_accum >> 24) ^ *data_blk_ptr) & 0xff);
       data_blk_ptr++;
       crc32_table_entry = WPI_RegistryGlobalLockCrc32Table(i);
       crc32_accum = (crc32_accum << 8) ^ crc32_table_entry;
    }
  return crc32_accum;
#else
  crc32_accum = *((WP_U32 *) data_blk_ptr);
  return ((crc32_accum & 0xffff) % 11);
#endif
}

/*********************************************************************

   function name : WPI_structure_analizer

   parameters :

   description  :  Analizes the

   called by : main

   input :  none

   output : none

 ********************************************************************/

#ifdef STATISTICS

void WPI_structure_InsertAnalizer (WP_U32 wpid)
{

  WP_U32 index_L1;
  WP_U32 L1_counter = 0;
  WP_U32 L2_counter = 0;
  WP_U32 LL_counter = 0;
  WP_U32 longest_LL = 0;
  WP_U32 temp;

  WP_U16 index_L2;
  wpi_dps_hash *LinkList_ptr;
  wpi_dps_hash *level2_ptr;

  for (index_L1 = 0; index_L1 < WPI_HASH_TABLE_SIZE; index_L1++)
    {
      if (R_MAC_CNTLS (level1_ptr[index_L1]) != 1
          && R_MAC_CNTLS (level1_ptr[index_L1]) != 0)
        L1_counter++;
      else if (R_MAC_CNTLS (level1_ptr[index_L1]) == 1)
        {
          level2_ptr =
            (wpi_dps_hash *) R_L2_PTR (wpid, &level1_ptr[index_L1],
                                       WPI_MAC_HASH_LOOKUP_MODE);
          for (index_L2 = 0; index_L2 < WPI_LOCAL_TABLE_SIZE; index_L2++)
            {
              if (R_MAC_CNTLS (level2_ptr[index_L2]) != 1
                  && R_MAC_CNTLS (level2_ptr[index_L2]) != 0)
                L2_counter++;
              else
                {
                  temp = R_MAC_L (level2_ptr[index_L2]);
                  LL_counter += temp;
                  if (temp > longest_LL)
                    longest_LL = temp;

                }
            }
        }
    }
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7, "\n\n");
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7, " Structure statistics :\n");
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7, " ---------------------");
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n Number of insertions : %d", insert_count);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n Rate between insertion number and hash table size : %d%%\n",
              100 * insert_count / WPI_HASH_TABLE_SIZE);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n Number of Control Word changes : %d",
              CTRL_change_count);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n Number of items in level1 : %d ", L1_counter);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n Number of items in level2 : %d ", L2_counter);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n Number of items in 1inked lists : %d \n",
              LL_counter);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n L1 insertion rate  %d%%",
              100 * L1_counter / insert_count);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n L2 insertion rate  %d%%",
              100 * L2_counter / insert_count);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n LL insertion rate  %d%%",
              100 * LL_counter / insert_count);
  WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
              "\n Longest Linked List : %d items \n", longest_LL);

  if (CTRL_change_count + L1_counter + L2_counter + LL_counter ==
      insert_count)
    WPI_LOG_MT (WPI_DEBUG_LEVEL_7, "\n\n Test O.K. \n");
  else
    WPI_LOG_MT (WPI_DEBUG_LEVEL_7,
                "\n\n Test failure - mismatch of cells number \n");

}

#endif

wpi_dps_hash *WPI_GetVpVcHashPtr (WP_handle port, WP_U32 wpid, wpi_atm *atm)
{

  /* Commit function always called, but will return if ATM is not needed. */
  if (atm == 0)
    return NULL;

  /* Check if it's an IMA group. */
  if (WPI_HANDLE_TYPE(port) == WPI_handle_ima) {
     wpi_ima *ima_res = WPI_RegistryIma(wpid);
     return atm->host_hash_table_base[ima_res->pg];
  }
#if _WP_MODULE_GBOND_ENABLE_
  if( WPI_HANDLE_TYPE(port) == WPI_handle_gbond_group )
  {
     wpi_atm_gbond_functions *gbond_res = WPI_RegistryGbondFunctions(wpid);
     if( gbond_res->get_port_interface(wpid) & WP_GBOND_ATM_PORT_UPI1 )
     {
        return atm->host_hash_table_base[WPI_ATM_GROUP_UPI1];
     }
     return atm->host_hash_table_base[WPI_ATM_GROUP_UPI2];
  }
#endif
  /* Otherwise, it should be a port */

  switch (WPI_HANDLE_INDEX (port))
  {
     case WP_PORT_UPI1:
        return atm->host_hash_table_base[WPI_ATM_GROUP_UPI1];
     case WP_PORT_UPI2:
        return atm->host_hash_table_base[WPI_ATM_GROUP_UPI2];
     case WP_PORT_UPI3:
        return atm->host_hash_table_base[WPI_ATM_GROUP_UPI3];
     case WP_PORT_UPI4:
        return atm->host_hash_table_base[WPI_ATM_GROUP_UPI4];
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
        return atm->host_hash_table_base[WPI_ATM_GROUP_TDI];
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
        /* ATM over G999 streams */
        return atm->host_hash_table_base[WPI_ATM_GROUP_ENET];
        break;
     default:
        WPI_LOG_MT (WPI_DEBUG_LEVEL_0,
                    "VpVcHashTable fetch - Invalid device !! \n");
  }

  return NULL;
}

/* Read form the RGPT the Hash pointer
                          and the mask   */

void WPI_GetDlciHashPtr (WP_handle dev_handle,
                         WPI_fr_dev_param *dev_param,
                         wpi_resource_entry *dev_entry)
{
  wpi_res_device_pkt_fr *dev_fr,tmp_dev_fr;
  wpi_res_device_trans *dev_trans=NULL;
  WPI_PTR hash_ptr;
  WP_U32 dev_index;
  wpi_res_device_pkt_fr_tdi *tdi_fr=NULL;

  WPI_LOG_MT(WPI_DEBUG_LEVEL_9,
           "WPI_GetDlciHashPtr \n");

  dev_index = WPI_HANDLE_INDEX(dev_handle);
  if(WPI_HANDLE_SUBTYPE(dev_handle) ==  WPI_pg_tdi_trans)
  {
     WPI_LOG_MT(WPI_DEBUG_LEVEL_3,
                "\n\ndev_entry : %x  dev_indx : %d\n",
                (WP_U32)dev_entry,dev_index);
     dev_trans = WPI_RES_DEV_TRANS(dev_entry);
     tmp_dev_fr.dat_base = dev_trans->dat_base;
     tmp_dev_fr.host_dat_base =
        (wpi_dps_fr_dat_entry *)dev_trans->host_dat_base;
     tmp_dev_fr.rx_n_dlci = dev_trans->rx_n_dlci;
     dev_fr = &tmp_dev_fr;
  }
  else
  {
     if(WPI_HANDLE_SUBTYPE(dev_handle) ==  WPI_pg_pkt_tdi)
     {
        WPI_LOG_MT(WPI_DEBUG_LEVEL_9,
                   "\n\ndev_entry : %x  dev_indx : %d\n",
                   (WP_U32)dev_entry,dev_index);
        tdi_fr = dev_entry->resource;
        dev_fr = (wpi_res_device_pkt_fr *)&tdi_fr->fr;
     }
     else
     {
        dev_fr = WPI_RES_DEV_PKT_FR(dev_entry);
     }
  }
   WPI_LOG_MT(WPI_DEBUG_LEVEL_9,
              "\n\ndev_fr : %x  \n",(WP_U32)dev_fr);

  hash_ptr = dev_fr->dat_base;
  WPI_LOG_MT(WPI_DEBUG_LEVEL_9,
             "hash_ptr : %x \n",(WP_U32)hash_ptr);
  dev_param->mask = dev_fr->rx_n_dlci;
  WPI_LOG_MT(WPI_DEBUG_LEVEL_9,
             "mask : %x \n",(WP_U32)dev_param->mask);
  dev_param->host_hash_ptr = (wpi_dps_hash *)dev_fr->host_dat_base;
  WPI_LOG_MT(WPI_DEBUG_LEVEL_9,
             "host_hash_ptr : %x \n",(WP_U32)dev_param->host_hash_ptr);
  return;
}

void WPI_HashClear(wpi_dps_hash *host_hash_ptr, WP_U32 n_entries, WP_U16 ctrl_value)
{
   WP_U32 i;

   WPI_INTERNAL_FUNCTION();
#if defined(WINSIM) || defined(VERILOG)

   WPI_memclr( (WP_U32*)host_hash_ptr, n_entries * sizeof( *host_hash_ptr));
   if( ctrl_value != 0)
      for(i=0; i<n_entries; i++)
      {
         WPI_MEM_SHORT_SET(host_hash_ptr[i].cntrl, ctrl_value);
      }

#else

   for(i=0; i<n_entries; i++)
   {
      WPI_MEM_SET(host_hash_ptr[i].ptr, 0);
      WPI_MEM_SET(host_hash_ptr[i].mac_h, 0);
      WPI_MEM_SHORT_SET(host_hash_ptr[i].mac_l, 0);
      WPI_MEM_SHORT_SET(host_hash_ptr[i].vlan_id, 0);
      WPI_MEM_SHORT_SET(host_hash_ptr[i].out_port, 0);
      WPI_MEM_SHORT_SET(host_hash_ptr[i].cntrl, ctrl_value);
   }

#endif
}

/*****************************************************************************
 *
 * Function: WPI_HashAllocate
 *
 * Purpose: Allocates memory for a hash table
 *
 * Description:
 *     This function allocates memory from the WP_VB_HASH_IW_LPM bus for
 *     IW hash by specified number of hash entries.
 *
 * Inputs:
 *     memory          WDDI memory map structure.
 *     n_entires       Number of entries in the hash.
 *
 * Outputs:
 *     hash_start      Physical hash base address
 *     host_hash_start Virtual hash base address
 *
 * Return Value:
 *     WP_OK on success, WP_ERR_MEMORY_ALLOCATION otherwise.
 *
 * Called by:
 *     WPI_FrHashLookupAlloc (wpi_fr.c)
 *
 ****************************************************************************/
WP_status WPI_HashAllocate(WP_U32 wpid,
                           WP_U32 n_entries,
                           WPI_PTR *hash_start,
                           wpi_dps_hash **host_hash_start,
                           WP_boolean clean_hash)
{
   WPI_PTR hash_base;
   wpi_dps_hash *host_hash_ptr;
   wpi_memory *memory;

   WPI_INTERNAL_FUNCTION();
   WPI_GenerateCrc32Table ();

   memory = WPI_RegistryMemory(wpid);
   hash_base = WPI_malloc(WPI_MemoryVbankOfVirtualBus(memory, WP_VB_HASH_IW_LPM),
                          (sizeof(wpi_dps_hash) * n_entries),
                          sizeof(wpi_dps_hash), "hash table");
   WPI_RETURN_IF_WPI_NULL(hash_base, WP_ERR_MEMORY_ALLOCATION);
   host_hash_ptr = (wpi_dps_hash *)
      WPI_HOST_PTR(hash_base, WPI_MemoryOffsetOfVirtualBus(memory, WP_VB_HASH_IW_LPM));
   if (clean_hash)
      WPI_HashClear(host_hash_ptr, n_entries, 0);

   *hash_start = hash_base;
   *host_hash_start = host_hash_ptr;

   return WP_OK;
}

WP_U32 WPI_HashEntriesGet(WP_U32 hash_size)
{
   WPI_INTERNAL_FUNCTION();
   if (hash_size >= WP_IW_LAST_HASH_ENTRIES)
      return 0;

   else return (1 << wpi_hash_size_power_of_two[hash_size]);
}
