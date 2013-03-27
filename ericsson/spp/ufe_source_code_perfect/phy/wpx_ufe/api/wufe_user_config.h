/***************************************************************************
 * (C) Copyright 2004-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 **************************************************************************/

/***************************************************************************
 *
 * File: wufe_user_config.h
 *
 * Purpose: Prototype definitions
 *
 *
 **************************************************************************/

#ifndef WUFE_USER_CONFIG_H
#define WUFE_USER_CONFIG_H

#ifndef WUFEI_USER_CONFIG_H
#ifdef WUFE_UFE3
#include "../fpga/ufe3/wufei_user_config.h"
#endif
#ifdef WUFE_UFE4
#include "../fpga/ufe4/wufei_user_config.h"
#endif
#endif

/********************************************************
 *  DISPLAY OPTIONS
 *********************************************************/
#define WUFE_DISPLAY_ENABLE 1
#define WUFE_PRINT_ERR_ENABLE 1
#define WUFE_PRINT_DBG_ENABLE 0
#define WUFE_DEBUG_ENABLE_WUFE_CALL_API 0

/********************************************************
 *  Degbug OPTIONS
 *********************************************************/
#define WUFE_DEBUG_VERIFY_WRITE 1

/********************************************************
 *  General OPTIONS
 *********************************************************/
#define WUFE_MAX_N_STM1          WUFEI_MAX_N_STM1
#define WUFE_MAX_N_SPE           WUFEI_MAX_N_SPE

#define WUFE_MAX_N_WINPATHS      WUFEI_MAX_N_WINPATHS
#define WUFE_MAX_N_UFE           WUFEI_MAX_N_UFE
#define WUFE_MAX_N_CORE          WUFEI_MAX_N_CORE
#define WUFE_MAX_N_EMPHYS        WUFEI_MAX_N_EMPHYS
#define WUFE_MAX_N_SBI           WUFEI_MAX_N_SBI
#define WUFE_MAX_N_CAD           WUFEI_MAX_N_CAD
#define WUFE_MAX_N_LINE_PER_SPE  WUFEI_MAX_N_LINE_PER_SPE

#define WUFE_MAX_N_CAD_LINE_HMVIP      WUFEI_MAX_N_CAD_LINE_HMVIP
#define WUFE_MAX_N_CAD_LINE_E1T1       WUFEI_MAX_N_CAD_LINE_E1T1
#define WUFE_MAX_N_CAD_LINE_DS3        WUFEI_MAX_N_CAD_LINE_DS3
#define WUFE_MAX_N_PHY                 WUFEI_MAX_N_PHY
#define WUFE_MAX_N_SBI_LINE            WUFEI_MAX_N_SBI_LINE
#define WUFE_MAX_N_CLOCK_TRANSLATORS   WUFEI_MAX_N_CLOCK_TRANSLATORS
#define WUFE_MAX_N_GENERIC_LINE        WUFEI_MAX_N_GENERIC_LINE

#define WUFE_MAX_N_AUG4         WUFEI_MAX_N_AUG4
#define WUFE_MAX_N_AUG1         WUFEI_MAX_N_AUG1
#define WUFE_MAX_N_AU3          WUFEI_MAX_N_AU3
#define WUFE_MAX_N_TUG2         WUFEI_MAX_N_TUG2
#define WUFE_MAX_N_TU           WUFEI_MAX_N_TU


#define WUFE_MAX_OC3_LINES WUFEI_MAX_OC3_LINES
#define WUFE_MAX_OC6_LINES WUFEI_MAX_OC6_LINES
#define WUFE_MAX_OC12_LINES WUFEI_MAX_OC12_LINES
#define WUFE_MAX_OC24_LINES WUFEI_MAX_OC24_LINES
#define WUFE_MAX_OC48_LINES WUFEI_MAX_OC48_LINES
#define WUFE_MAX_OC3_DCC_LINES WUFEI_MAX_OC3_DCC_LINES
#define WUFE_MAX_OC6_DCC_LINES WUFEI_MAX_OC6_DCC_LINES
#define WUFE_MAX_OC12_DCC_LINES WUFEI_MAX_OC12_DCC_LINES
#define WUFE_MAX_OC48_DCC_LINES WUFEI_MAX_OC48_DCC_LINES

#define WUFE_RX_PARITY_ODD  WUFEI_RX_PARITY_ODD
#define WUFE_RX_PARITY_EVEN WUFEI_RX_PARITY_EVEN
#define WUFE_TX_PARITY_ODD  WUFEI_TX_PARITY_ODD
#define WUFE_TX_PARITY_EVEN WUFEI_TX_PARITY_EVEN

/* Defines not in used in UFE3 */
#define WUFE_MAX_N_DCO  2  /* The maximum number of DCO in the system. Not relevant in UFE3 */
#define WUFE_DIFF2SCK_RATIO_COUNTER_VALUE  4000  /* Set the number of socet clocks pass between each sampling of the
                                                    differential reference clock (in terms of 125usec units). For
                                                    example: To count 1 second place 8000: 8000*125sec = 1sec 
                                                    must comply with WPI_CLOCK_REC_RATIO_UPDATE_125U_DIVIDER */
#define WUFE_XO2SCK_RATIO_COUNTER_VALUE    4000  /* Set the number of socet clocks pass between each sampling of the
                                                    Osillator reference clock (in terms of 125usec units).
                                                    For example: To count 1 second place 8000: 8000*125sec = 1sec
                                                    must comply with WPI_CLOCK_REC_RATIO_UPDATE_125U_DIVIDER */
#endif /* WUFE_USER_CONFIG_H*/
