#ifndef _WPLI_PLATFORM_H__
#define _WPLI_PLATFORM_H__

WP_U8* WPLI_PhysOffsetGet(WP_U8 i_wpid);

/*  The internal register base address is platform specific. */
#define PHYS_TO_KSEG1(a)         ((a) + (WP_U32)WPLI_PhysOffsetGet(0))
#define PHYS_TO_KSEG0(a)         ((a) + (WP_U32)WPLI_PhysOffsetGet(0))

/* A macro to convert a physical address to a virtual address
 * for use ONLY by functions defined within the wddi/phy directory.
 * Normal winmon has one WinPath, and uses the MIPS standard mapping.
 * The second BIM WinPath is directly mapped. */

#define WPL_PHY_VIRTUAL(wpid, physical) \
   (((wpid) == 0) ? PHYS_TO_KSEG1(physical) : (physical))

#define WPL_RIF_VIRTUAL(wpid, physical) \
   (((wpid) == 0) ? PHYS_TO_KSEG1(physical) : (physical))

#define MAX_WP_BANKS        7
#define MAX_SIU_NO          5

#define WPL_NUM_BANKS       (MAX_WP_BANKS*WINPATH_NUM_MAX)

#define WPL_IOCTL_WMM_HOST_CMD           0
#define WPL_IOCTL_SUPPRESS_DUP_SIGNAL    1
#define WPL_IOCTL_READ_PHYS_MEM_LAYOUT   2
#define WPL_IOCTL_ENABLE_BUS_ERR_SIG     3
#define WPL_IOCTL_KERNEL_BUS_ERR_REPORT  4
#define WPL_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL 5

WP_U32 WPLI_BusesRifBaseGet(WP_U32* o_rif_base);




#endif /*_WPLI_PLATFORM_H__*/
