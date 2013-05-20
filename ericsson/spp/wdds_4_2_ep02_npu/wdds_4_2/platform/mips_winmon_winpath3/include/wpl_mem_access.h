/*****************************************************************************
 * (C) Copyright 2003-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpl_mem_access.h
 *
 * Purpose: mips_web_winmon macros to access memory shared with wincomm.
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
 * mips_web_winmon:  big-endian, but needs word alignment */

#define WPI_BE_WORD_ALIGNED(ptr) (*((WP_U32 *)(ptr)))
#define WPI_BE_WORD(ptr) \
((WP_U32)( \
(((WP_U8 *) (ptr))[0] << 24) | \
(((WP_U8 *) (ptr))[1] << 16) | \
(((WP_U8 *) (ptr))[2] << 8) | \
((WP_U8 *) (ptr))[3]))

#define WPI_BE_SHORT_ALIGNED(ptr) (*((WP_U16 *)(ptr)))
#define WPI_BE_SHORT(ptr) \
((WP_U16)( \
(((WP_U8 *) (ptr))[0] << 8) | \
((WP_U8 *) (ptr))[1]))


#if WP_MEM_CHECK
extern void wp_memory_reference_error(void);
#define WP_MEM_SIZE_OK(object, width) \
   if (sizeof(object) != width) wp_memory_reference_error()
#else
#define WP_MEM_SIZE_OK(object, width)
#endif

#define WP_MEM_SET(dest, source) do { \
WP_MEM_SIZE_OK(dest, 4); \
(*(volatile WP_U32 *)(void *)&(dest)) = (source); \
} while (0)

#define WP_MEM_SHORT_SET(dest, source) do { \
WP_MEM_SIZE_OK(dest, 2); \
(*(volatile WP_U16 *)(void *)&(dest)) = (source); \
} while (0)

#define WP_MEM_BYTE_SET(dest, source) do { \
WP_MEM_SIZE_OK(dest, 1); \
(*(volatile WP_U8 *)(void *)&(dest)) = (source); \
} while (0)

#define WP_MEM_GET(dest, source) do { \
WP_MEM_SIZE_OK(source, 4);                    \
(dest) = (*(volatile WP_U32 *)(void *)&(source)); \
} while (0)

#define WP_MEM_SHORT_GET(dest, source) do { \
WP_MEM_SIZE_OK(source, 2); \
WP_MEM_SIZE_OK(dest, 2); \
(dest) = (*(volatile WP_U16 *)(void *)&(source)); \
} while (0)

#define WP_MEM_BYTE_GET(dest, source) do {    \
WP_MEM_SIZE_OK(source, 1); \
WP_MEM_SIZE_OK(dest, 1); \
(dest) = (*(volatile WP_U8 *)(void *)&(source)); \
} while (0)

#define WP_MEM_BYTES_SET(dest, source, length) memcpy(dest, source, length)
#define WP_MEM_BYTES_GET(dest, source, length) memcpy(dest, source, length)

#define WP_MEM_BYTES_FILL(dest, value, length) memset(dest, value, length)

/* A macro to enforce in-order execution of load/store.
 * Nothing needed here. */
#define WPI_ENFORCE_ORDER do {} while (0)

/* A macro determining whether rebooting is permitted. */
#define WPI_REBOOT_ENABLE 1

#endif
