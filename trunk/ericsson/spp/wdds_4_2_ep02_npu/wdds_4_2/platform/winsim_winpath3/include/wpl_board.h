#ifndef _WPL_BOARD_H__
#define _WPL_BOARD_H__

#include "wpi_veneer_memory.h"
#include "wpl_platform.h"


/*****************************************************************************
 *
 * Purpose: This module defines a bank for each application to use
 *
 ****************************************************************************/

#define APP_BANK_PARAM 2
#define APP_BANK_PACKET 0
#define APP_BANK_INTERNAL 0
#define APP_BANK_HOST 1
#define APP_BANK_ATOMIC 5

#define WPL_BOARD_HOST_RANGE     1
#define WPL_BOARD_PARAM_RANGE    2
#define WPL_BOARD_PACKET_RANGE   3
#define WPL_BOARD_INTERNAL_RANGE 4
#define WPL_BOARD_RIF_RANGE      5

#define WPL_NUM_BUSES    (WPI_BUS_LAST - 1)

#define WPL_PAGES 1024
#define WPL_PAGE_SIZE 4096

#define WPL_PROCESSOR_SPEED 350000000

#define WPL_HOST_SIZE 0x04000000

#define WPL_NUM_WINPATHS 1

#endif /*_WPL_BOARD_H__*/
