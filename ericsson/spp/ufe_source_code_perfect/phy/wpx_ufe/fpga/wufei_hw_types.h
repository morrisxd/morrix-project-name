/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_types.h
 *
 * Purpose: the HW memory space type
 *
 ****************************************************************************/
#ifndef WUFEI_HW_TYPES_H
#define WUFEI_HW_TYPES_H

#ifndef WPX_BOARD_DATA_H
#include "wpx_board_data.h"
#endif

#ifdef WP_HW_WINPATH3
#ifndef WPX_PINMUX_H
#include "wpx_pinmux.h"
#endif
#endif

/**************************************************
 * Address translation
 * Example for board not using 3 LSB of the bus
 *
 * DRV address is * 2 from the ABS
 *
 * HW  address is * 8 from the ABS
 *
 *************************************************/

/* valid values are 1,2 or 3.
                               This is the only place in the driver that needs to be 
                               modified in case of change in the register stride */
#if (WPX_PHY_REG_MULTIPLIER == 1)
#define WUFEI_ADDRSHIFT (1) /* On WDS3 the UFE registers reside on a 16-bit boundary */
#else
#define WUFEI_ADDRSHIFT (3) /* On WDS2 the UFE registers reside on a 64-bit boundary */
#endif

#if (WUFEI_ADDRSHIFT == 3) /* port size = 64 bits */
typedef struct
{
      WP_U16 value;
      WP_U16 reserved[3];
}WUFEI_reg_space;
#endif

#if WUFEI_ADDRSHIFT == 2 /* port size = 32 bits */
typedef struct
{
      WP_U16 value;
      WP_U16 reserved;
}WUFEI_reg_space;
#endif

#if WUFEI_ADDRSHIFT == 1 /* port size = 16 bits */
typedef struct
{
      WP_U16 value;
}WUFEI_reg_space;
#endif

#endif /*WUFEI_HW_TYPES_H*/
