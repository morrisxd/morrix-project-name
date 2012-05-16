#ifndef __ADDRESS_H__
#define __ADDRESS_H__


/*--------------------USER MODIFY (NON-LINUX ONLY)---------------*/
#define WEB_PARAMRAM_BASE_PHYS     0x20000000
#define WEB_PARAMRAM_BASE_VIRT     0x20000000
#define WEB_PARAMRAM_SIZE          0x10000000


#define WEB_PACKETRAM_BASE_PHYS    0x30000000
#define WEB_PACKETRAM_BASE_VIRT    0x30000000
#define WEB_PACKETRAM_SIZE         0x10000000


#define WEB_SYSTEMRAM_BASE_PHYS    0x00000000	// HOST BUS
#define WEB_SYSTEMRAM_BASE_VIRT    0x00000000 + 0xA0000000
#define WEB_SYSTEMRAM_SIZE         0x20000000   /*Total System Ram size*/
#define USED_SYSTEMRAM_SIZE        0x004c0000	/*Used System RAM size, ex: Winmon Size + Stress Test Size*/


#define WEB_INTERNAL_BASE_PHYS     0x1ec00000	// Internal RAM
#define WEB_INTERNAL_BASE_VIRT	   0x1ec00000 + 0xA0000000
#define WEB_INTERNAL_SIZE          0x00400000


#define INTERNAL_REGISTERS_VIRT	   0x1e000000 + 0xA0000000	// RIF
#define RESERVED_SYSTEMRAM_SPACE   0x02000000				// reserved area
/*-------------------USER MODIFY (NON-LINUX ONLY)-END--------------*/



#endif


