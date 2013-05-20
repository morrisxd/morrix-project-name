/*****************************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpv_mem_access.h
 *
 * Purpose: winsim macros to access memory shared with wincomm.
 *
 ****************************************************************************/

#ifndef WPL_MEM_ACCESS_H
#define WPL_MEM_ACCESS_H

#ifndef WPL_TYPES_H
#include "wpl_types.h"
#endif

/* Macro to load a word from an octet stream.  This macro must work whether
 * the memory access is word-aligned or not and must work whether the host
 * is little-endian or not.  The macro definition is placed here because the
 * implementation may be target-dependent.  In the case of verilog and winsim,
 * the implementation must work on a host of either endianness.
 *
 * winsim:  use slow-but-safe */

#define WPI_BE_WORD_ALIGNED(ptr) \
((WP_U32)( \
(((WP_U8 *) (ptr))[0] << 24) | \
(((WP_U8 *) (ptr))[1] << 16) | \
(((WP_U8 *) (ptr))[2] << 8) | \
((WP_U8 *) (ptr))[3]))
#define WPI_BE_WORD(ptr) \
((WP_U32)( \
(((WP_U8 *) (ptr))[0] << 24) | \
(((WP_U8 *) (ptr))[1] << 16) | \
(((WP_U8 *) (ptr))[2] << 8) | \
((WP_U8 *) (ptr))[3]))

#define WPI_BE_SHORT_ALIGNED(ptr) \
((WP_U16)( \
(((WP_U8 *) (ptr))[0] << 8) | \
((WP_U8 *) (ptr))[1]))
#define WPI_BE_SHORT(ptr) \
((WP_U16)( \
(((WP_U8 *) (ptr))[0] << 8) | \
((WP_U8 *) (ptr))[1]))

void WPI_Write32(WP_U32 address, WP_U32 value);
void WPI_Write16(WP_U32 address, WP_U32 value);
void WPI_Write8(WP_U32 address, WP_U32 value);
void WPI_Read32(WP_U32 address, WP_U32 *pvalue);
void WPI_Read16(WP_U32 address, WP_U16 *pvalue);
void WPI_Read8(WP_U32 address, WP_U8 *pvalue);
void WPI_WriteN(WP_U8 *dest, WP_U8 *source, WP_U32 size);
void WPI_ReadN(WP_U8 *dest, WP_U8 *source, WP_U32 size);

#define WP_MEM_SET(mem,value) WPI_Write32((WP_U32)&(mem), value)
#define WP_MEM_SHORT_SET(mem,value) WPI_Write16((WP_U32)&(mem), value)
#define WP_MEM_BYTE_SET(mem,value) WPI_Write8((WP_U32)&(mem), value)
#define WP_MEM_GET(value,mem) WPI_Read32((WP_U32)&(mem), (WP_U32*)&(value))
#define WP_MEM_SHORT_GET(value, mem) WPI_Read16((WP_U32)&(mem), &(value))
#define WP_MEM_BYTE_GET(value, mem) WPI_Read8((WP_U32)&(mem), (WP_U8 *)&(value))
#define WP_MEM_BYTES_SET(dest, source, size) WPI_WriteN(dest, source, size)
#define WP_MEM_BYTES_GET(dest, source, size) WPI_ReadN(dest, source, size)
#define WP_MEM_BYTES_FILL(dest, value, length) { \
WP_U32 j; \
WP_U8 *byte = (WP_U8 *) (dest); \
for (j = 0; j < (length); j++) WP_MEM_BYTE_SET(byte[j], value); \
}

/* A macro to enforce in-order execution of load/store.
 * Nothing needed here. */
#define WPI_ENFORCE_ORDER do {} while (0)

/* A macro determining whether rebooting is permitted. */
#define WPI_REBOOT_ENABLE 0

#endif
