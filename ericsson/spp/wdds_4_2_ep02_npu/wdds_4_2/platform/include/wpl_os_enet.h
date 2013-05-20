/*************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module: wpl_os_enet.h
 * Purpose:
 *
 * This module contains platform-specific data types.
 *****************************************************************************/

#ifndef WPL_OS_ENET_H
#define WPL_OS_ENET_H

/* The OS supports various services which the WDDI might invoke.  The
 * WPL_OsServiceType is an enumeration of such services.   The names
 * may be regarded as commands*/
typedef enum {
   WPL_OsServiceStartEnetRelease,
   WPL_OsServiceStartEnetReleasePermanent,
   WPL_OsServiceFinishEnetRelease,
   WPL_OsServiceStartEnetRestore,
   WPL_OsServiceFinishEnetRestore,
   WPL_OsServiceGetDriverInfo,
   WPL_OsServiceDupIntrSignalAllow,     /* For mips_linux: allow duplicate signal handler */
   WPL_OsServiceDupIntrSignalSuppress,  /* For mips_linux: suppress duplicate signal handlers */
} WPL_OsServiceType;

typedef enum {
   WPL_os_enet_unused = 0,
   WPL_os_enet_keepalive
} WPL_os_enet_usage;


#endif
