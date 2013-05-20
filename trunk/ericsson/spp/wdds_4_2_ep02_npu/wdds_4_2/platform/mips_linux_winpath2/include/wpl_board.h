#ifndef _WPL_BOARD_H__
#define _WPL_BOARD_H__


#define WPL_TA_BASE   0x1000

#define WPL_INTERNAL_CLOCK_FREQ_INT  362
#define WPL_INTERNAL_CLOCK_FREQ_FRAC 963

/* offset to memory on internal bus */
#if defined(WPL_WP2_PCI_MEMMAP_WA)
#define WPL_INT_MEM_RAM_OFFSET 0x20000
#define WPL_INT_MEM_RIF_OFFSET 0
#else
#define WPL_INT_MEM_RAM_OFFSET 0
#define WPL_INT_MEM_RIF_OFFSET 0x20000
#endif

/* internal bus size for the linux BSP */
#define WPL_INTERNAL_PLUS_PHYS_SIZE 0x01000000 /*including phy mapping*/

#define WPL_INTERNAL_BUS_SIZE   0x00008000
#define WPL_INTERNAL_BUS_RAM_BASE (WPL_INT_MEM_RAM_OFFSET)

#define WPL_INTERNAL_BUS_RIF_SIZE   0x00040000
#define WPL_INTERNAL_BUS_RIF_BASE   (WPL_INT_MEM_RIF_OFFSET)

#define WPL_PARAM_BUS_SIZE    0x00400000
#define WPL_PARAM_BUS_ADDR    0x10000000

#define WPL_PACKET_BUS_SIZE   0x08000000
#define WPL_PACKET_BUS_ADDR   0x08000000

/************************************
 * Bus values defined for this board.
 ************************************/
#define APP_BANK_PARAM 1
#define APP_BANK_PACKET 0
#define APP_BANK_INTERNAL 0

/* For WP2 application bus is used - chipselect 0*/
/* If host bus is used - chipselect 1 */
#define APP_BANK_HOST 0
#define APP_BANK_ATOMIC 5

#define WPL_EXTERNAL_CLOCK 66666666

#define WPL_NUM_WINPATHS 1

#define WPL_HOST_SIZE 0x04000000

#endif /*_WPL_BOARD_H__*/
