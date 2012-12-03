/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpi_lookup.h
 *
 * Purpose: Lookup defintions and structures.
 *
 ****************************************************************************/

#ifndef WPI_LOOKUP_H
#define WPI_LOOKUP_H

#ifndef WPI_WDDI_TYPEDEFS_H
#include "include/core/registry/wpi_wddi_typedefs.h"
#endif

struct wpi_res_lookup
{
      WP_U32 base_ptr;
      WP_U8 *host_ptr;
      WP_U32 wpid;
      WP_U16 vbank;
      WP_U8 *host_offset;
      WP_U32 num_of_entries;
      WP_U32 entry_size;
      WP_U16 reserved;
};

typedef struct
{
	WP_U32 ip_addr;
	WP_U16 src_port;//
	WP_U16 dst_port;
} wp_ip_ep;

typedef struct{
	WP_U32 valid_num;	
	wp_ip_ep ip_ep[64];
}wp_local_rt;

typedef struct{
	WP_U32 flow_num;//this field can hold 2 FIWTs
	WP_U32 port_num[257];//port_num[64];
}wp_globle_rt;



typedef struct{
	WP_U32 valid_num;
	WP_U32 curr_num;
	WP_U32 flow_num[16];//Can be redanduenty eth2
}wp_vport_table;

typedef struct{
	WP_U32 flow_num;//this field can hold 2 FIWTs
	WP_U16 nsei_i;
	WP_U16 bvci_n;
}wp_dl_rt;


typedef struct{
	WP_U32 flow_num[4];//[0]with highest priority [3]with lowest priority
}wp_priority_msg;

typedef struct 
{
	WP_U64 RxNsBytes;
	WP_U64 RxNsDataPackets;
	WP_U64 RxNsSignalPackets;
	WP_U64 RxBssgpBytes;
	WP_U64 RxBssgpDataPackets;
	WP_U64 RxBssgpSignalPackets;
   	WP_U64 TxNsBytes;
	WP_U64 TxNsDataPackets;
	WP_U64 TxNsSignalPackets;
	WP_U64 TxBssgpBytes;
	WP_U64 TxBssgpDataPackets;
	WP_U64 TxBssgpSignalPackets;
}wp_ns_bssgp_statis;


WP_status WPI_LookupCreate( wpi_res_lookup **reg_ptr);

WP_status WPI_LookupDelete(wpi_res_lookup *reg_ptr);

WP_status WPI_LookupEntryInsert(wpi_res_lookup *reg_ptr,
                                WP_U32 key,
                               WP_U32 agg_flow_num,
                                WP_U32 nri,
                               WP_U32 port_num);

WP_status WPI_LookupEntryRemove(wpi_res_lookup *reg_ptr,
                                WP_U32 key,
                                WP_U32 nri);

WP_status WPI_LookupEntryGet(wpi_res_lookup *reg_ptr,
                             WP_U32 key,
                             WP_U32 nri,
                              WP_U32 *port_num);

WP_status WPI_LookupKeyFind(wpi_res_lookup *reg_ptr,
                            WP_U32 *found_index,
                            WP_U32 data);


#endif
