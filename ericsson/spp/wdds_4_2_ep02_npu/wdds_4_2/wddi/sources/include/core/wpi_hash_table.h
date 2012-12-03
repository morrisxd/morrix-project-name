/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpi_hash_table.h
 *
 * Purpose: Hash Table interfaces
 *
 ****************************************************************************/
#ifndef WPI_HASH_TABLE_H
#define WPI_HASH_TABLE_H

#ifndef WPI_WDDI_TYPEDEFS_H
#include "include/core/registry/wpi_wddi_typedefs.h"
#endif
#ifndef WPI_TYPES_H
#include "include/core/registry/wpi_types.h"
#endif
#ifndef WP_IW_H
#include "api/wp_iw.h"
#endif

WP_U32 WPI_update_crc32(WP_U8 *data_blk_ptr);
void WPI_GenerateCrc32Table(void);
WP_status WPI_IwMacAddressFind(WP_handle iw_system,
                               WP_l2_forward *user_mac_entry,
                               wpi_dps_hash **matched_dps_hash,
                               WP_command command);
void WPI_IwInsertParamToL2(wpi_dps_hash *planted_cell,
                           wpi_dps_hash *table2_ptr);
WP_status WPI_IwInsertHashLinkList(WP_U32 wpid,
                                   wpi_dps_hash *list_tail,
                                   wpi_dps_hash *new_block,
                                   WP_U8 hash_mode,
                                   WP_U8 search_mode);
void WPI_IwRemoveLLBlock(WP_U32 wpid,
                         wpi_dps_hash *pre_removed_block);
WP_status WPI_IwPopToL2(wpi_dps_hash *level1_ptr,
                   wpi_dps_hash *folded_L2,
                   wpi_dps_hash *removed_block,
                   WP_U32 L1_place,
                   WP_U8 hash_mode);
void WPI_IwInsertHashL2(wpi_dps_hash *planted_cell,
                        wpi_dps_hash *table2_ptr);
WP_status WPI_CreateLinkList(wpi_dps_hash **head,
                             WP_U32 wpid,
                             wpi_dps_hash *first_block,
                             wpi_dps_hash *second_block,
                             WP_U8 hash_mode,
                             WP_U8 search_mode);
wpi_dps_hash *WPI_SearchLinkList(WP_U32 wpid,
                                 wpi_dps_hash *list_head,
                                 wpi_dps_hash *scanned_block,
                                 wpi_dps_hash **cur,
                                 WP_U32 *match,
                                 WP_U8 hash_mode,
                                 WP_U8 search_mode);

WP_status WPI_AtmHashEntryInsert(WP_handle port,
                                 WPI_atm_hash_param *user_atm_entry,
                                 WP_boolean replace_mode);
WP_status WPI_FrHashEntryInsert(WP_handle dev,
                                WPI_fr_hash_param *user_fr_entry);
WP_status WPI_FrHashEntryRemove(WP_handle dev,
                                WPI_fr_hash_param *user_fr_entry);
WP_status WPI_AtmHashEntryRemove(WP_handle port,
                                 WPI_atm_hash_param *user_atm_entry);

wpi_dps_hash* WPI_GetVpVcHashPtr(WP_handle port,WP_U32 wpid, wpi_atm *atm);

void WPI_GetDlciHashPtr (WP_handle dev_handle,
                         WPI_fr_dev_param *dev_param,
                         wpi_resource_entry *dev_entry);

WP_status WPI_IwMacAddressInsert(WP_handle iw_system,
                                 wpi_dps_hash *user_mac_entry,
                                 WP_U8 search_mode,
                                 wpi_dps_hash **matched_dps_hash,
                                 void *hash_handler_object);
WP_status WPI_IwMacAddressRemove(WP_handle iw_system,
                                 wpi_dps_hash *user_mac_entry,
                                 WP_U8 hash_search,
                                 void *hash_handler_object);

WP_status WPI_HashAllocate(WP_U32 wpid,
                           WP_U32 n_entries,
                           WPI_PTR *hash_start,
                           wpi_dps_hash **host_hash_start,
                           WP_boolean clean_hash);

void WPI_HashClear(wpi_dps_hash *host_hash_ptr, WP_U32 n_entries, WP_U16 ctrl_value);
WP_U32 WPI_HashEntriesGet(WP_U32 hash_size);

extern WP_U32 WPI_crc32_table[256];

WP_status WPI_UserHashIwMacAddressFind (WP_handle iw_system,
                                  wpi_dps_hash * user_mac_entry,
                                  void *hash_handler_object,
                                  wpi_dps_hash * user_entry);
WP_status WPI_UserDefGetHashParams(WP_handle iw_system,
                            wpi_dps_hash **user_mac_entry,
                            wpi_dps_hash **level1_ptr,
                            WP_U8 *hash_mode,
                            WP_U32 *full_offset,
                            WP_U16 *L2_place,
                            WP_U8 search_mode,
                            void *hash_handler_object);
WP_status WPI_UserDefHashEntryInsert (WP_handle dev_handle,
                                 WP_U32 key1,WP_U16 key2,WP_U16 data);

WP_status WPI_UserDefIwMacAddressInsert (WP_handle iw_system,
                                  wpi_dps_hash *user_mac_entry,
                                  WP_U8 search_mode,
                                  wpi_dps_hash **matched_dps_hash,
                                  void *hash_handler_object);

WP_status WPI_UserHashIwMacAddressFind (WP_handle iw_system,
                                  wpi_dps_hash * user_mac_entry,
                                  void *hash_handler_object,
                                  wpi_dps_hash * user_entry);
WP_status WPI_UserDefHashEntryDisplay (WP_handle dev_handle,
                                 WP_U32 key1,WP_U16 key2,WP_U16* result);



WP_status WPI_UserDefHashEntryRemove (WP_handle dev_handle,
                                 WP_U32 key1,WP_U16 key2,WP_U16 data);
WP_status WPI_UserDefIwMacAddressRemove (WP_handle iw_system,
                                  wpi_dps_hash * user_mac_entry,
                                  WP_U8 search_mode,
                                  void *hash_handler_object);


WP_status WP_UserDefHashTableCreate(WP_U32 wpid,
						  WP_U32 n_entries);





#endif /* WPI_HASH_TABLE_H */
