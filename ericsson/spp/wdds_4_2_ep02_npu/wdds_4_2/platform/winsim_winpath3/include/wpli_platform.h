#ifndef _WPLI_PLATFORM_H__
#define _WPLI_PLATFORM_H__

/* A macro to convert a physical address to a virtual address
 * for use ONLY by functions defined within the wddi/phy directory.
 * Normal winmon has one WinPath, and uses the MIPS standard mapping.
 * The second BIM WinPath is directly mapped. */

#define WPL_PHY_VIRTUAL(wpid, physical) (physical)

#define WPL_RIF_VIRTUAL(wpid, physical) (physical)

#endif /*_WPLI_PLATFORM_H__*/
