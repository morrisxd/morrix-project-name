/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_globals.h
 *
 * Purpose: the general registers definition for the UFE4 FPGA
 *This file is here for compilation purposes but is irrelvant for UFE3.
 ****************************************************************************/

#ifndef WUFEI_HW_GLOBALS_H
#define WUFEI_HW_GLOBALS_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_HW_TYPES_H
#include "wufei_hw_types.h"
#endif

#define WUFEI_GEN_CFG_SYS_REG_MACHINE_TRANS 0x1
#define WUFEI_GEN_CFG_SYS_REG_MACHINE_TRANS_MF 0x2

#define WUFEI_GLOBAL_CFG_BANDWIDTH_OC3 0
#define WUFEI_GLOBAL_CFG_BANDWIDTH_OC6 4 /* This configuration is not coded in the HW*/
#define WUFEI_GLOBAL_CFG_BANDWIDTH_OC12 1
#define WUFEI_GLOBAL_CFG_BANDWIDTH_OC24 2
#define WUFEI_GLOBAL_CFG_BANDWIDTH_OC48 3

/* System frequencies (not in used in UFE3) */
#define WUFEI_SYSTEM_CLOCK_155_52 155520000
#define WUFEI_SYSTEM_CLOCK_77_76   77760000
#define WUFEI_SYSTEM_CLOCK_38_88   38880000
#define WUFEI_SYSTEM_CLOCK_19_44   19440000

#define WUFEI_UfeCoreDdrStateGet(ufeid, coreid) WUFE_OK
#define WUFEI_UfeFramerTypeGet(wufe_id) 0

#endif /*WUFEI_HW_GLOBALS_H*/
