/*************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module: wpl_wmm.h
 * Purpose:
 *
 * This module contains platform-specific data types.
 *****************************************************************************/

#ifndef WPL_WMM_H
#define WPL_WMM_H

typedef struct WPL_wmm_host_cmd
{
   WP_U32 base;
   WP_U32 cmd;
   WP_U32 reg;
   WP_U32 address;
   WP_U32 status;
} WPL_wmm_host_cmd;

#endif
