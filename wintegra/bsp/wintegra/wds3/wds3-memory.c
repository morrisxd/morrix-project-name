/*
 * Copyright (C) 2000-2010 Wintegra, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * PROM library functions for acquiring/using memory descriptors given to
 * us from the Winmon.
 */
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/pfn.h>
#include <linux/string.h>
#include <asm/bootinfo.h>
#include <asm/page.h>
#include <asm/sections.h>

#include <asm/wintegra/winpath.h>
#include <asm/wintegra/winmon.h>

enum winmon_memtypes {
   winmon_dontuse,/*BOOT_MEM_RESERVED: owned by Linux, don't allocate */
   winmon_prom,   /*BOOT_MEM_ROM_DATA: Owned by bootloader: can be taken by Linux after kernel boot */
   winmon_free,   /*BOOT_MEM_RAM: free memory, can be allocated */
   wddi_reserved, /*BOOT_MEM_RESERVED: reserved for WDDI/DPS*/
};
static struct prom_pmemblock mdesc[PROM_MAX_PMEMBLOCKS];

#ifdef DEBUG
static char *mtypes[3] = {
	"Dont use memory",
	"WinMon PROM memory",
	"Free memmory",
};
#endif

/* determined physical memory size, not overridden by command line args  */
unsigned long physical_memsize = 0L;

static struct prom_pmemblock * __init prom_getmdesc(void)
{
	unsigned int memsize;
	char cmdline[CL_SIZE], *ptr;
        int           i = 0;
        wds_winpath_mem_bank_t *bank = 0;
        unsigned int num_of_cores = 1;        

        bank = &(wds.wds_wp[0].wds_winpath_mem_banks[WDS3_SIU_BUS_HOST_NUM-1]);

        if( wds.wds_chip == WINTEGRA_SPO_ID )
        {
           bank = &(wds.wds_wp[0].wds_winpath_mem_banks[WDS3_SIU_BUS_PARAM_NUM-1]);
        }
                
        physical_memsize = bank->wp_phys_bank_size;
        printk(KERN_WARNING
               "memsize not  set to (%ldMb)\n", (physical_memsize >> 20));

	/* SOC-it swaps, or perhaps doesn't swap, when DMA'ing the last
	   word of physical memory */
	physical_memsize -= PAGE_SIZE;

	/* Check the command line for a memsize directive that overrides
	   the physical/default amount */
	strcpy(cmdline, arcs_cmdline);
	ptr = strstr(cmdline, "memsize=");
	if (ptr && (ptr != cmdline) && (*(ptr - 1) != ' '))
		ptr = strstr(ptr, " memsize=");

	if (ptr)
		memsize = memparse(ptr + 8, &ptr);
	else
		memsize = physical_memsize;

	memset(mdesc, 0, sizeof(mdesc));

	mdesc[i].type = winmon_dontuse;
	mdesc[i].base = 0x00000000;
	mdesc[i].size = 0x00001000;
        //printk("memory region %d: base 0x%08x size 0x%08x winmon_dontuse\n",i, (unsigned int)mdesc[i].base, (unsigned int)mdesc[i].size);
        i++;

        /* Winmon (and Minimon) code */
	mdesc[i].type = wddi_reserved;/*should be reserved or else minimon image will be erased*/
	mdesc[i].base = 0x00001000;/*0x00001000 used as base for the RT core (minimon) exception handlers base address */
	mdesc[i].size = 0x009ff000;
        //printk("memory region %d: base 0x%08x size 0x%08x wddi_reserved\n",i, (unsigned int)mdesc[i].base, (unsigned int)mdesc[i].size);
        i++;

        if( ((unsigned int*)WINMON_FUNCTION_TABLE)[WINMON_GET_NUM_OF_ACTIVE_CORES] != 0xffffffff)  
           num_of_cores = WinMon_num_active_cores();
        
        if(num_of_cores == 2)//Dual core
        {           
           /* Minimon sand box (heap, stack etc.) */
           mdesc[i].type = wddi_reserved;
           mdesc[i].base = 0x00A00000;
           mdesc[i].size = 0x04fff000;
           //printk("memory region %d: base 0x%08x size 0x%08x wddi_reserved\n",i, (unsigned int)mdesc[i].base, (unsigned int)mdesc[i].size);
        }
        else//Single core
        {
           /* Linux sand box (heap, stack etc.) */
           mdesc[i].type = winmon_free;
           mdesc[i].base = 0x00A00000;
           mdesc[i].size = 0x04fff000;
           //printk("memory region %d: base 0x%08x size 0x%08x winmon_free\n",i, (unsigned int)mdesc[i].base, (unsigned int)mdesc[i].size);           
        }        
        i++;
        
        /* Linux image */
	mdesc[i].type = winmon_dontuse;
	mdesc[i].base = 0x05A00000;
	mdesc[i].size = CPHYSADDR(PFN_ALIGN((unsigned long)&_end)) - mdesc[i].base;
        //printk("memory region %d: base 0x%08x size 0x%08x winmon_dontuse\n",i, (unsigned int)mdesc[i].base, (unsigned int)mdesc[i].size);
        i++;

        /* Linux sand box (heap, stack etc.) */
        mdesc[i].type = winmon_free;
        mdesc[i].base = CPHYSADDR(PFN_ALIGN(&_end)); 
	mdesc[i].size = bank->wp_application_bank_base_addr - mdesc[i].base;
        //printk("memory region %d: base 0x%08x size 0x%08x winmon_free\n",i, (unsigned int)mdesc[i].base, (unsigned int)mdesc[i].size);
        i++;

        /* Application reserved address space */
        if(bank->wp_application_bank_base_addr < physical_memsize)
        {
           mdesc[i].type = wddi_reserved;
           mdesc[i].base = bank->wp_application_bank_base_addr;
           mdesc[i].size = bank->wp_application_bank_size;
           //printk("memory region %d: base 0x%08x size 0x%08x wddi_reserved\n",i, (unsigned int)mdesc[i].base, (unsigned int)mdesc[i].size);
        }

	return &mdesc[0];
}

static int __init prom_memtype_classify(unsigned int type)
{
	switch (type) {
	case winmon_free:
		return BOOT_MEM_RAM;
	case winmon_prom:
		return BOOT_MEM_ROM_DATA;
	case wddi_reserved:
		return BOOT_MEM_RESERVED;
	default:
		return BOOT_MEM_RESERVED;
	}
}

void __init prom_meminit(void)
{
	struct prom_pmemblock *p;

#ifdef DEBUG
	pr_debug("WinMon MEMORY DESCRIPTOR dump:\n");
	p = prom_getmdesc();
	while (p->size) {
		int i = 0;
		pr_debug("[%d,%p]: base<%08lx> size<%08lx> type<%s>\n",
			 i, p, p->base, p->size, mtypes[p->type]);
		p++;
		i++;
	}
#endif
	p = prom_getmdesc();

	while (p->size) {
		long type;
		unsigned long base, size;

		type = prom_memtype_classify(p->type);
		base = p->base;
		size = p->size;

		add_memory_region(base, size, type);
                p++;
	}
}

void __init prom_free_prom_memory(void)
{
	unsigned long addr;
	int i;

	for (i = 0; i < boot_mem_map.nr_map; i++) {
		if (boot_mem_map.map[i].type != BOOT_MEM_ROM_DATA)
			continue;

		addr = boot_mem_map.map[i].addr;
		free_init_pages("prom memory",
				addr, addr + boot_mem_map.map[i].size);
	}
}
