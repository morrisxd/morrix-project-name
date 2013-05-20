#ifndef _WPLI_PLATFORM_H__
#define _WPLI_PLATFORM_H__

/*  The internal register base address is platform specific. */
#define PHYS_TO_KSEG1(a)         ((a) | 0xa0000000)
#define PHYS_TO_KSEG0(a)         ((a) | 0x80000000)

/* A macro to convert a physical address to a virtual address
 * for use ONLY by functions defined within the wddi/phy directory.
 * Normal winmon has one WinPath, and uses the MIPS standard mapping.
 * The second BIM WinPath is directly mapped. */

#define WPL_PHY_VIRTUAL(wpid, physical) \
(((wpid) == 0) ? PHYS_TO_KSEG1(physical) : (physical))

#define WPL_RIF_VIRTUAL(wpid, physical) \
(((wpid) == 0) ? PHYS_TO_KSEG1(physical) : (physical))

WP_U32 WPLI_BusesRifBaseGet(WP_U32* o_rif_base);


#endif /*_WPLI_PLATFORM_H__*/
