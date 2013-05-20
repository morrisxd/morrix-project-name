#ifndef _WPLI_PLATFORM_H__
#define _WPLI_PLATFORM_H__

WP_U8* WPLI_PhysOffsetGet(WP_U8 i_wpid);

/* A macro to convert a physical address to a virtual address
 * for use ONLY by functions defined within the wddi/phy directory.
 * Normal winmon has one WinPath, and uses the MIPS standard mapping.
 * The second BIM WinPath is directly mapped. */

#define WPL_PHY_VIRTUAL(wpid, physical) \
        ((physical + (WP_U32)WPLI_PhysOffsetGet(wpid)))

#define WPL_RIF_VIRTUAL(wpid, physical) \
        (physical + (WP_U32)WPL_RegOffsetGet(wpid))

#define WPL_BSP_WINMON_VERSION          0x04020000
#define WPL_BSP_MULTI_WINPATH_VERSION   0x04020500

#define MAX_WP_BANKS        7
#define MAX_SIU_NO          5

#define WPL_NUM_BANKS       (MAX_WP_BANKS*WINPATH_NUM_MAX)

// XALYO
#define XS_IOCTL_OS_VALUE(x) (0xAF + x)

#undef WPL_IOCTL_WMM_HOST_CMD
#undef WPL_IOCTL_SUPPRESS_DUP_SIGNAL
#undef WPL_IOCTL_READ_PHYS_MEM_LAYOUT
#undef WPL_IOCTL_ENABLE_BUS_ERR_SIG
#undef WPL_IOCTL_KERNEL_BUS_ERR_REPORT
#undef WPL_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL
#undef WPL_IOCTL_READ_BOARD_GEN_CONF

#define WPL_IOCTL_WMM_HOST_CMD           XS_IOCTL_OS_VALUE(0)
#define WPL_IOCTL_SUPPRESS_DUP_SIGNAL    XS_IOCTL_OS_VALUE(1)
#define WPL_IOCTL_READ_PHYS_MEM_LAYOUT   XS_IOCTL_OS_VALUE(2)
#define WPL_IOCTL_ENABLE_BUS_ERR_SIG     XS_IOCTL_OS_VALUE(3)
#define WPL_IOCTL_KERNEL_BUS_ERR_REPORT  XS_IOCTL_OS_VALUE(4)
#define WPL_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL XS_IOCTL_OS_VALUE(5)
#define WPL_IOCTL_READ_BOARD_GEN_CONF    XS_IOCTL_OS_VALUE(6)

WP_U32 WPLI_BusesRifBaseGet(WP_U8 i_wpid, WP_U32* o_rif_base);

#endif /*_WPLI_PLATFORM_H__*/
