/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_memory_access_int.h
 *
 * Purpose: Memory Access interfaces
 *
 ****************************************************************************/

#ifndef WUFEI_MEMORY_ACCESS_INT_H
#define WUFEI_MEMORY_ACCESS_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

void WUFEI_IndirectMemset(WP_U32 id, void *indirect_add, WP_U8 pattern, WP_U32 set_size);

void WUFEI_ReadModifyWrite(WP_U32 wufe_id, WP_U32 addr, WP_U8 value);

void WUFEI_DirectMemOrMask (WP_U32 wufe_id,
                            WP_U16 *  address,
                            WP_U16 mask);
void WUFEI_DirectMemAndMask(WP_U32 wufe_id,
                            WP_U16 *  address,WP_U16 mask);

/* Write one byte to direct memory (registers memory) */
void  WUFEI_MEM_8_SET(WP_U32 id, WP_U8 *offset, WP_U8 value);

void WUFEI_MemShortWrite(WP_U32 base, WP_U32 register_num, WP_U16 value);

void WUFEI_MemShortRead(WP_U32 base, WP_U32 register_num, WP_U16 *value);

#endif
