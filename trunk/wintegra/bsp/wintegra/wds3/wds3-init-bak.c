/*
 * Copyright (C) 2000-2010 Wintegra Inc.  All rights reserved.
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
 * PROM library initialisation code.
 */
#include "linux/init.h"
#include "linux/string.h"
#include "linux/kernel.h"

#include "asm/bootinfo.h"
#include "asm/io.h"
#include "asm/system.h"
#include "asm/cacheflush.h"
#include "asm/traps.h"

#include "asm/wintegra/winmon.h"
#include "asm/wintegra/winpath.h"


int prom_argc;
int *_prom_argv, *_prom_envp;
int clocks_per_msec;

typedef unsigned long WP_U32;
typedef unsigned char WP_U8;

struct t_clocks {
  unsigned int Clockin;
  unsigned int SIU1_Clock;
  unsigned int SIU2_Clock;
  unsigned int SIU3_Clock;
  unsigned int SIU4_Clock;
  unsigned int SIU5_Clock;
  unsigned int MIPS_Clock;
} SystemClocks;

#define ILLEGAL_WINMON_FUNC_PTR 0xffffffff

typedef enum
{
  param_index = 0,
  packet_index,
  internal_index,
  host_index,
  rif_index,
  fpga_index,
  num_of_entries
} buses_index;

struct _bus_memory_map_t _winpath_memory_map[num_of_entries];

/*
 * Winmon pass arguments and environment as 32-bit pointer.
 */

int init_debug = 0;

static inline unsigned char str2hexnum(unsigned char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0; /* foo */
}

static inline void str2eaddr(unsigned char *ea, unsigned char *str)
{
	int i;

	for (i = 0; i < 6; i++) {
		unsigned char num;

		if((*str == '.') || (*str == ':'))
			str++;
		num = str2hexnum(*str++) << 4;
		num |= (str2hexnum(*str++));
		ea[i] = num;
	}
}

int prom_get_ethernet_addr(unsigned char *ethernet_addr)
{
        char *ethaddr_str;

        ethaddr_str = WinMon_getenv("BOARD_MAC_ETHX");
	if (!ethaddr_str) {
	        printk("ethaddr not set in boot prom\n");
		return -1;
	}
	str2eaddr(ethernet_addr, ethaddr_str);

	if (init_debug > 1) {
	        int i;
		printk("get_ethernet_addr: ");
	        for (i=0; i<5; i++)
		        printk("%02x:", (unsigned char)*(ethernet_addr+i));
		printk("%02x\n", *(ethernet_addr+i));
	}

	return 0;
}

static unsigned long getFreq( unsigned long pllcfg1 )
{
  unsigned long pllcfg_val = *((volatile unsigned long*)pllcfg1);
  unsigned long pfd = (pllcfg_val&0x1fff);
  unsigned long prd = ((pllcfg_val>>16)&0x3f);
  unsigned long freq = (WDS3_EXTERNAL_CLOCK/2);
                       
  freq /= (prd+1);
  freq *= (pfd+1);
  freq += 1000;   /* Fix round off error */
  return freq;
}
  
static char __init *getBusName(int bus)
{
  switch (bus)
  {
  case SIU_BUS_PARAM_NUM: return "PARAM";
  case SIU_BUS_PACKET_NUM: return "PACKET";
  case SIU_BUS_INTERNAL_NUM: return "INTERNAL";
  case SIU_BUS_HOST_NUM: return "HOST";
  case SIU_BUS_RIF_NUM: return "RIF";
  case FPGA_NUM: return "FPGA";
  default: return "";
  }
}

static void __init setMemMap(int index, int bus)
{
  _winpath_memory_map[index].bus = bus;
  _winpath_memory_map[index].active    =   WinMon_is_bus_access(bus);
  _winpath_memory_map[index].phys_base =   WinMon_get_bus_address(bus);
  _winpath_memory_map[index].phys_size =   WinMon_get_bus_size(bus);
  _winpath_memory_map[index].application_base = 0xffffffff;
  _winpath_memory_map[index].application_reserved_size = 0;

  printk("Bus %d %s base address 0x%08x, size 0x%08x, %s\n",
          bus,
          getBusName(bus),
          _winpath_memory_map[index].phys_base,
          _winpath_memory_map[index].phys_size,
          (_winpath_memory_map[index].active == 0)?"not active":"active");
}

unsigned int __init get_physical_memory_layout(void)
{
  if((WINMON_FUNC(WINMON_MEM_ADDR_OFS) == ILLEGAL_WINMON_FUNC_PTR) ||
     (WINMON_FUNC(WINMON_MEM_SIZE_OFS) == ILLEGAL_WINMON_FUNC_PTR) ||
     (WINMON_FUNC(WINMON_BUS_ACTIVE_OFS) == ILLEGAL_WINMON_FUNC_PTR))
  {
    printk("\n*** FATAL ERROR: WINMON MUST BE UPGRADED. CAN'T SUPPORT AUTO MEM MAP ***\n\n");
    return -1;
  }
  printk("\nReading physical memory layout from WinMon boot loader ...\n");

  setMemMap(param_index,SIU_BUS_PARAM_NUM);
  setMemMap(packet_index,SIU_BUS_PACKET_NUM);
  setMemMap(internal_index,SIU_BUS_INTERNAL_NUM);
  setMemMap(host_index,SIU_BUS_HOST_NUM);
  setMemMap(rif_index,SIU_BUS_RIF_NUM);
  setMemMap(fpga_index,FPGA_NUM);
  return 0;
}

unsigned int get_memory_layout_attribute_value(
                   unsigned int bus,unsigned int mem_layout_attribute)
{
  unsigned int bus_index;

  switch(bus)
  {
  case SIU_BUS_PARAM_NUM:
    bus_index = param_index;
    break;
  case SIU_BUS_PACKET_NUM:
    bus_index = packet_index;
    break;
  case SIU_BUS_INTERNAL_NUM:
    bus_index = internal_index;
    break;
  case SIU_BUS_HOST_NUM:
    bus_index = host_index;
    break;
  case SIU_BUS_RIF_NUM:
    bus_index = rif_index;
    break;
  case FPGA_NUM:
    bus_index = fpga_index;
    break;
  default:
    return 0xffffffff;
  }

  switch(mem_layout_attribute)
  {
    case MEM_ATTRIB_SIZE:
      return _winpath_memory_map[bus_index].phys_size;
    case MEM_ATTRIB_BASE_ADDRESS:
      return _winpath_memory_map[bus_index].phys_base;
    case BUS_ATTRIB_ACTIVE:
      return _winpath_memory_map[bus_index].active;
    default:
      return 0xffffffff;
  }
  return 0xffffffff;
}

void __init calcSystemClock(void)
{
  SystemClocks.SIU1_Clock    = getFreq( MAP_CLK_SIU1_PLLCFG1 );
  SystemClocks.SIU2_Clock    = getFreq( MAP_CLK_SIU2_PLLCFG1 );
  SystemClocks.SIU3_Clock    = getFreq( MAP_CLK_INTERNAL_PLLCFG1 );
  SystemClocks.SIU4_Clock    = SystemClocks.SIU3_Clock/2;
  SystemClocks.SIU5_Clock    = getFreq( MAP_CLK_SIU4D_PLLCFG1 );
  SystemClocks.MIPS_Clock    = getFreq( MAP_CLK_MIPS_PLLCFG1 );
  SystemClocks.Clockin       = WDS3_EXTERNAL_CLOCK;
  clocks_per_msec            = SystemClocks.SIU4_Clock/1000;
}


WP_U32 getHostSystemClock(void)
{
  /*   printk("\n Calculated System Clock: %lu\n", SystemClocks.SIU3_Clock);
   */
  return SystemClocks.SIU3_Clock;
}

unsigned int getMipsSystemClock(void)
{
  return SystemClocks.MIPS_Clock;
}

int prom_putchar(char c)
{
  return WinMon_putchar(c);
}

#ifdef CONFIG_SERIAL_8250_CONSOLE
static void __init console_config(void)
{
	char console_string[40];
	int baud = 0;
	char parity = '\0', bits = '\0', flow = '\0';
	char *s;

	if ((strstr(prom_getcmdline(), "console=")) == NULL) {
		s = WinMon_getenv("modetty0");
		if (s) {
			while (*s >= '0' && *s <= '9')
				baud = baud*10 + *s++ - '0';
			if (*s == ',') s++;
			if (*s) parity = *s++;
			if (*s == ',') s++;
			if (*s) bits = *s++;
			if (*s == ',') s++;
			if (*s == 'h') flow = 'r';
		}
		if (baud == 0)
			baud = 38400;
		if (parity != 'n' && parity != 'o' && parity != 'e')
			parity = 'n';
		if (bits != '7' && bits != '8')
			bits = '8';
		if (flow == '\0')
			flow = 'r';
		sprintf(console_string, " console=ttyS0,%d%c%c%c", baud, parity, bits, flow);
		strcat(prom_getcmdline(), console_string);
		pr_info("Config serial console:%s\n", console_string);
	}
}
#endif

static void __init mips_nmi_setup(void)
{
	void *base;
	extern char except_vec_nmi;

	base = cpu_has_veic ?
		(void *)(CAC_BASE + 0xa80) :
		(void *)(CAC_BASE + 0x380);
	memcpy(base, &except_vec_nmi, 0x80);
	flush_icache_range((unsigned long)base, (unsigned long)base + 0x80);
}

static void __init mips_ejtag_setup(void)
{
	void *base;
	extern char except_vec_ejtag_debug;

	base = cpu_has_veic ?
		(void *)(CAC_BASE + 0xa00) :
		(void *)(CAC_BASE + 0x300);
	memcpy(base, &except_vec_ejtag_debug, 0x80);
	flush_icache_range((unsigned long)base, (unsigned long)base + 0x80);
}

extern struct plat_smp_ops msmtc_smp_ops;

void __init prom_init(void)
{
#ifdef CONFIG_MIPS_CMP
	int result;
#endif

	prom_argc = fw_arg0;
	_prom_argv = (int *) fw_arg1;
	_prom_envp = (int *) fw_arg2;

	board_nmi_handler_setup = mips_nmi_setup;
	board_ejtag_handler_setup = mips_ejtag_setup;

	pr_info("\nLINUX started...\n");

        get_physical_memory_layout();
        calcSystemClock();
	prom_init_cmdline();
	prom_meminit();
#ifdef CONFIG_SERIAL_8250_CONSOLE
	console_config();
#endif

#ifdef CONFIG_MIPS_CMP
	if (result)
		register_smp_ops(&cmp_smp_ops);
#endif
#ifdef CONFIG_MIPS_MT_SMP
#ifdef CONFIG_MIPS_CMP
	if (!result)
		register_smp_ops(&vsmp_smp_ops);
#else
	register_smp_ops(&vsmp_smp_ops);
#endif
#endif
#ifdef CONFIG_MIPS_MT_SMTC
	register_smp_ops(&msmtc_smp_ops);
#endif
}

extern void wds3_be_init(void);
extern int wds3_be_handler(struct pt_regs *regs, int is_fixup);

const char *get_system_type(void)
{
  return "Wintegra WDS3 WinPath3 Development System";
}

void __init plat_mem_setup(void)
{
  mips_reboot_setup();

  board_be_init = wds3_be_init;
  board_be_handler = wds3_be_handler;
}

#if WINPATH_INSTRUMENTATION_ENABLE
int inst_init = 0;
struct inst_data {
  unsigned int ts;
  unsigned int code;
};

struct inst_struct {
  unsigned int next;
  unsigned int limit;
  struct inst_data data[0];
};

struct inst_struct *inst_base = (struct inst_struct *) 0xac000000;
volatile unsigned int *inst_ts = (volatile unsigned int *) 0xbd021da8;

void inst_mark(unsigned int code) {
  unsigned int next;
  /* initialize first time */
  if (inst_init == 0) {
    inst_base->next = 0;
    inst_base->limit = 4096;
    inst_init = 1;
  }
  /* record mark */
  next = inst_base->next;
  if (next < inst_base->limit) {
    inst_base->next = next + 1;
    inst_base->data[next].ts = *inst_ts;
    inst_base->data[next].code = code;
  }
}

EXPORT_SYMBOL_GPL(inst_mark);
#endif

