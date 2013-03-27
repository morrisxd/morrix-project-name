/***************************************************************************
 * (C) Copyright 2004-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 **************************************************************************/

/***************************************************************************
 *
 * File: wufei_user_config.h
 *
 * Purpose: Prototype definitions
 *
 *
 **************************************************************************/

#ifndef WUFEI_USER_CONFIG_H
#define WUFEI_USER_CONFIG_H

/********************************************************
 *  General OPTIONS
 *********************************************************/
#define WUFEI_DEFAULT_WINPATH_ID (0)

#define WUFEI_MAX_N_STM1          (2)
#define WUFEI_MAX_N_SPE           (3)

#define WUFEI_MAX_N_WINPATHS        (4)
#define WUFEI_MAX_N_UFE             (4)
#define WUFEI_MAX_N_CORE            (2)
#define WUFEI_MAX_N_EMPHYS          WUFEI_MAX_N_CORE
#define WUFEI_MAX_N_SBI             (1)
#define WUFEI_MAX_N_CAD             (64)
#define WUFEI_MAX_N_LINE_PER_SPE    (28)

#define WUFEI_MAX_N_CAD_LINE_HMVIP       (64)   /* HMVIP CADs*/
#define WUFEI_MAX_N_CAD_LINE_E1T1        WUFEI_MAX_N_CAD   /* Non HMVIP CADs*/
#define WUFEI_MAX_N_CAD_LINE_DS3         WUFE_IRRELEVANT   /* Non HMVIP CADs*/
#define WUFEI_MAX_N_PHY                  (8192) /*UFE3 FPGA Supports 4096 rspt tspt per core*/
#define WUFEI_MAX_N_SBI_LINE             (336)
#define WUFEI_MAX_N_CLOCK_TRANSLATORS    (32)
#define WUFEI_MAX_N_GENERIC_LINE         (336)
#define WUFEI_SPE_LINE_ADDRESS_SPACE (32)
#define WUFEI_SPE_LINE_HMVIP_ADDRESS_SPACE (128)
#define WUFEI_N_MAX_CAD_E1          WUFEI_MAX_N_CAD_LINE_E1T1
#define WUFEI_N_MAX_CAD_T1          WUFEI_MAX_N_CAD_LINE_E1T1

#define WUFEI_MAX_N_AUG4 (1)
#define WUFEI_MAX_N_AUG1 (4)
#define WUFEI_MAX_N_AU3  (3)
#define WUFEI_MAX_N_TUG2 (7)
#define WUFEI_MAX_N_TU   (4)

#define WUFEI_MAX_PDH_LOOPBACK 16

#define WUFEI_MAX_OC3_LINES 84
#define WUFEI_MAX_OC12_LINES 336
#define WUFEI_MAX_OC24_LINES 672
#define WUFEI_MAX_OC48_LINES 1344
#define WUFEI_MAX_OC12_DCC_LINES 0

#define WUFEI_MAX_OC6_LINES 168
#define WUFEI_MAX_OC3_DCC_LINES 0
#define WUFEI_MAX_OC6_DCC_LINES 0
#define WUFEI_MAX_OC48_DCC_LINES 0


#define WUFEI_RX_PARITY_ODD  0
#define WUFEI_RX_PARITY_EVEN 1
#define WUFEI_TX_PARITY_ODD  0
#define WUFEI_TX_PARITY_EVEN 1

/* Dual EMPHY defines for UFE4 not supported in UFE3 */
#define WUFEI_SECOND_EMPHY_LINE_OFFSET 0xFFFF

#endif /* WUFEI_USER_CONFIG_H*/
