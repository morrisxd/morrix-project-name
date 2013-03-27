/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufe_ufe.h
 *
 * Purpose: contain all the header files nedd by the application.
 *
 ****************************************************************************/
#ifndef WUFE_UFE_H
#define WUFE_UFE_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_API_H
#include "wufe_api.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFE_SYSTEM_H
#include "wufe_system.h"
#endif

#ifndef WUFE_LINE_H
#include "wufe_line.h"
#endif

#ifndef WUFE_PHY_H
#include "wufe_phy.h"
#endif

#ifndef WUFE_EVENTS_H
#include "wufe_events.h"
#endif

#ifndef WUFE_CLK_REC_H
#include "wufe_clk_rec.h"
#endif

#ifndef WUFE_SBI_H
#include "wufe_sbi.h"
#endif

#ifndef WUFE_USER_CONFIG_H
#include "wufe_user_config.h"
#endif
/*****************************
 * WUFE_UfeInit attribute types
 *****************************/
#define WUFE_ATTRIB_CONNECT_OFFSET_S            0
#define WUFE_ATTRIB_CONNECT_OFFSET_W            2
#define WUFE_ATTRIB_CONNECT_OFFSET_F(v) WUFEI_FIELD(v, WUFE_ATTRIB_CONNECT_OFFSET_S)
#define WUFE_ATTRIB_CONNECT_OFFSET_V(f) \
   WUFEI_VALUE(f,WUFE_ATTRIB_CONNECT_OFFSET_S,WUFE_ATTRIB_CONNECT_OFFSET_W)
#define WUFE_ATTRIB_CONNECT_OFFSET_M \
   WUFEI_MASK(WUFE_ATTRIB_CONNECT_OFFSET_S,WUFE_ATTRIB_CONNECT_OFFSET_W)

#define WUFE_ATTRIB_CONNECT_OFFSET(value) WUFE_ATTRIB_CONNECT_OFFSET_F(value)
#define WUFE_ATTRIB_CONNECT_MAX_OFFSET 2

#define WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS (1<<4)
#define WUFE_ATTRIB_CAD_HMVIP_LOOPBACK         (1<<5) /* supported for UFE2 only */


/* The following board support attributes supported by UFE2+ only */
#define WUFE_ATTRIB_BS_LINE_LOOPBACK           (1<<6)
#define WUFE_ATTRIB_BS_SYSTEM_LOOPBACK         (1<<7)
#define WUFE_ATTRIB_LOOPT_ENABLE               (1<<8)

#define WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_S     9
#define WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_W     1
#define WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_F(v) \
   WUFEI_FIELD(v, WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_S)
#define WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_V(f) \
   WUFEI_VALUE(f,WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_S,WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_W)
#define WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_M \
   WUFEI_MASK(WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_S,WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_W)

#define WUFE_ATTRIB_DONT_USE_FRAMER (1<<10)

#define WUFE_ATTRIB_BS_GENERIC_T1 0
#define WUFE_ATTRIB_BS_GENERIC_E1 1

#define WUFE_ATTRIB_INITIALIZE_FIRMWARE (1<<11)

#define WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE(value) WUFE_ATTRIB_BS_GENERIC_TRANSFER_TYPE_F(value)


/* reserved for backward compatibility */
#define  WUFE_LINE_CAD_LOOPBACK           WUFE_ATTRIB_CAD_HMVIP_LOOPBACK
#define  WUFE_IGNORE_BOARD_SUPPORT_CALLS  WUFE_ATTRIB_IGNORE_BOARD_SUPPORT_CALLS

enum
{
   WUFE_CORE_ID_0 = 0,
   WUFE_CORE_ID_1,
   WUFE_CORE_ID_NA = 0xff
};

/*********************************
 * Connection parametes structure
 *********************************/

typedef struct
{
   WP_U8 ufe_id;
   WP_U32 attributes;
} WUFE_init_config;

/***********************************************************
 * WUFE API function prototypes
 ************************************************************/

/***********************************************************
 * WUFE PLL parametes structures
 ************************************************************/
enum WUFEI_Pll_clock_out
{
   apll_clk0,
   p0_clk0,
   p1_clk0
};

enum apll_clk0_rate
{
   /* SONET/SDH MODE
      eth_en = 0
      f_seln = 0 */
   f_77_76,
   f_38_88,
   f_19_44,
   f_9_72,
   f_51_84,
   f_25_92,
   f_12_96,
   f_6_48,


   /* SONET/SDH MODE
      eth_en = 1
      f_seln = 1 */
   f_125,
   f_62_5,
   f_50,
   f_25,
   f_12_5
};

typedef struct
{
   WP_U32 ufeid;
   enum WUFEI_Pll_clock_out clock_out;
   WP_U32 clock_rate;
   enum apll_clk0_rate apll_rate;
}WUFEI_pll_cfg;

#ifndef WUFE_PROTOTYPES_H
#include "wufe_prototypes.h"
#endif

#endif /* WUFE_UFE_H*/
