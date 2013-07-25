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
#include "linux/module.h"

#include "asm/bootinfo.h"
#include "asm/io.h"
#include "asm/system.h"
#include "asm/cacheflush.h"
#include "asm/traps.h"

#include "asm/wintegra/winmon.h"
#include "asm/wintegra/winpath.h"
#include "asm/wintegra/version.h"


int prom_argc;
int *_prom_argv, *_prom_envp;
wds_board_descr_t wds;
static spinlock_t wmm_cmd_lock;
char* RIF_BASE_EP_WP_VADR[WDS_WINPATH_NO_MAX];


minimon_descr_t minimon_desc = { .dual_core_enabled = 0,
                                 .comm_start_addr = 0xffffffff,
                                 .print_buff_addr = 0xffffffff };

/*
 * Winmon pass arguments and environment as 32-bit pointer.
 */

static inline unsigned char str2hexnum(unsigned char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return 0; 
}

static inline void str2eaddr(unsigned char *ea, unsigned char *str)
{
  int i;

  for (i = 0; i < 6; i++) 
  {
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
  if (!ethaddr_str) 
  {
    printk("ethaddr not set in boot prom\n");
    return -1;
  }
  str2eaddr(ethernet_addr, ethaddr_str);

#if WINTEGRA_DEBUG_ENABLE
  {
    int i;
    printk("get_ethernet_addr: ");
    for (i=0; i<5; i++)
      printk("%02x:", (unsigned char)*(ethernet_addr+i));
    printk("%02x\n", *(ethernet_addr+i));
  }
#endif
  return 0;
}  

#define WINTEGRA_WINPATH3_REV_A_STR      "WINPATH3_REV_A";
#define WINTEGRA_WINPATH3_REV_B_STR      "WINPATH3_REV_B";
#define WINTEGRA_WINPATH3_REV_B_2_STR    "WINPATH3_REV_B_2";
#define WINTEGRA_WINPATH3SL_REV_A_STR    "WINPATH3_SL_REV_A";
#define WINTEGRA_WINPATH3SL_REV_B_STR    "WINPATH3_SL_REV_B";
#define WINTEGRA_WINPATH3SL_REV_B_1_STR  "WINPATH3_SL_REV_B_1";
#define WINTEGRA_WINPATH3_SPO_STR        "WINPATH3_SPO";
#define WINTEGRA_UNKNOWN_CHIP_STR        "UNKNOWN_CHIP"

unsigned int __init get_physical_memory_layout(void)
{
   int i,j;
  char *str;
/*********************************/
  wds_board_descr_t      *pWds;
  wds_per_winpath_t      *pWp;
  wds_winpath_mem_bank_t *pBank;
/*********************************/

  if( ((unsigned int*)WINMON_FUNCTION_TABLE)[WINMON_GET_MULTI_WP_DESC] == 0xffffffff)
  {
     printk("\n*** FATAL ERROR: WINMON MUST BE UPGRADED. CAN'T SUPPORT AUTO MEM MAP ***\n\n");
     return -1;
  }  

  pWds = (wds_board_descr_t*)Winmon_get_multi_wp_desc();

  memcpy(&wds, pWds, sizeof(wds));

  if( ((unsigned int*)WINMON_FUNCTION_TABLE)[WINMON_GET_MINIMON_DESC] == 0xffffffff)
  {
    printk("\n*** FATAL ERROR: WINMON MUST BE UPGRADED. CAN'T SUPPORT DUAL CORE ***\n\n");
    return -1;
  }
   
  memcpy(&minimon_desc, Winmon_get_minimon_desc(), sizeof(minimon_descr_t));

  pWds = &wds;
  pWp = &(pWds->wds_wp[0]);

  switch(pWds->wds_chip)
  {
     case WINTEGRA_WINPATH3_REV_A_ID:
        str = WINTEGRA_WINPATH3_REV_A_STR;
        break;
     case WINTEGRA_WINPATH3_REV_B_ID:         
        str = WINTEGRA_WINPATH3_REV_B_STR;
        break;
     case WINTEGRA_SUPERLITE_REV_A_ID:        
        str = WINTEGRA_WINPATH3SL_REV_A_STR;
        break;
     case WINTEGRA_SUPERLITE_REV_B_ID:        
        str = WINTEGRA_WINPATH3SL_REV_B_STR;
        break; 
     case WINTEGRA_SUPERLITE_REV_B_1_ID:        
        str = WINTEGRA_WINPATH3SL_REV_B_1_STR;
        break; 
     case WINTEGRA_SPO_ID:
        str = WINTEGRA_WINPATH3_SPO_STR;
        break;
     case WINTEGRA_WINPATH3_REV_B_2_ID:         
        str = WINTEGRA_WINPATH3_REV_B_2_STR;
        break;
        
     default:
        printk("\n*** Unknown chip id was read from Winmon. ID 0x%08x ***\n", pWds->wds_chip);
        printk("PROBABLY WINMON SHOULD BE UPGRADED\n\n");
        str = WINTEGRA_UNKNOWN_CHIP_STR;
  }

  printk("\n\n\n  ********************************************************************************\n");
  printk("  ********************************************************************************\n");
  printk("  **                                                                            **\n");  
  printk("  **     PMC-SIERRA (WIN Devision): Winpath3/SL kernel version %s         **\n", __PMC_WIN_VERSION__);
  printk("  **                                                                            **\n");  
  printk("  ********************************************************************************\n");
  printk("  ********************************************************************************\n\n\n");


  printk("\n************************************\n");
  printk("******** BOARD CONFIGURATION *******\n");
  printk("************************************\n");
  printk("No. of winpath %d\n",pWds->wds_winpath_no);
  printk("Chip: %s\n",str);
  printk("WinMon ver: %d.%d.%d\n",((pWds->wds_winmon_ver&0xff000000)>>24), ((pWds->wds_winmon_ver&0x00ff0000)>>16), ((pWds->wds_winmon_ver&0x0000ff00)>>8));
  printk("Board external clock %d Hz\n",pWds->wds_clock_freq_external);
  printk("RIF base: 0x%08x\n",pWds->wds_winpath_phys_rif_base);
  for(j=0; j<pWds->wds_winpath_no; j++)
  {
     printk("\n************ WP # %d Configuration ************\n",j+1);
     pWp = &(pWds->wds_wp[j]);
     
  printk("WP No. of banks %d\n\n",pWp->wds_winpath_no_of_banks);
  printk("Frequencies:\n");
  printk("MIPS freq: %d Hz\n",pWp->wp_clock_freq_mips);
  for(i = 0; i < 5; i++)
     printk("SIU%d freq: %d Hz\n", i+1, pWp->wp_clock_freq_siu[i]);
  printk("\n************ BANKS INFO ************\n");
  for(i = 0; i < 7; i++)
  {
     pBank = &(pWp->wds_winpath_mem_banks[i]);

#if 0 /// REMOVED. Taken from WinMon
     pBank->wp_application_bank_base_addr = pBank->wp_phys_bank_base_addr;
     pBank->wp_application_bank_size      = pBank->wp_phys_bank_size;

     if( pWds->wds_chip == WINTEGRA_SPO_ID )
     {
        if (pBank->wp_bus == WDS3_SIU_BUS_PARAM_NUM)
        {
           pBank->wp_application_bank_base_addr += pBank->wp_application_bank_size;
           pBank->wp_application_bank_base_addr -= WDS_HOST_SIZE;
           pBank->wp_application_bank_size       = WDS_HOST_SIZE;
        }

     }
     else if (pBank->wp_bus == WDS3_SIU_BUS_HOST_NUM)
     {
        pBank->wp_application_bank_base_addr += pBank->wp_application_bank_size;
        pBank->wp_application_bank_base_addr -= WDS_HOST_SIZE;
        pBank->wp_application_bank_size       = WDS_HOST_SIZE;
     }
     
#endif
     printk("Bank %d: active %d, base 0x%08x, size 0x%08x shared mem. base 0x%08x shared mem. size 0x%08x  -- [%s]\n",i, pBank->wp_bus_active, pBank->wp_phys_bank_base_addr, pBank->wp_phys_bank_size, pBank->wp_application_bank_base_addr, pBank->wp_application_bank_size, pBank->wds_bank_name);
  }
  printk("************************************\n\n");
  }
  
  return 0;
}


unsigned int getHostSystemClock(int WpId)
{
  return wds.wds_wp[WpId].wp_clock_freq_siu[2];
}

unsigned int getMipsSystemClock(void)
{
  return wds.wds_wp[0].wp_clock_freq_mips;
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

  if(get_physical_memory_layout())
     return;

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
  spin_lock_init(&wmm_cmd_lock);
}

/*******************************************************************/
/*   Winpath WMM host command support                              */
/*******************************************************************/
void winpath_wmm_host_cmd_execute(winpath_wmm_host_cmd *wmm_host_cmd_reg)
{
   volatile winpath_wmm *wmm_base = (winpath_wmm *)wmm_host_cmd_reg->base;
   unsigned long timeout = WINPATH_WMM_HOST_CMD_TIMEOUT;

   local_irq_disable();
   spin_lock(&wmm_cmd_lock);

   while ((wmm_base->hcr_cmd & WINPATH_WMM_CMD_VALID) && timeout-->0);
//      udelay(WINPATH_WMM_HOST_CMD_DELAY);
   /* return, do nothing if previous command is not completed */
   if (timeout == 0)
   {
      wmm_host_cmd_reg->status = WINPATH_WMM_HOST_CMD_PREV_FAILED;
      spin_unlock(&wmm_cmd_lock);
      local_irq_enable();
      return;
   }

   /* put the block address to the WMM registers */
   wmm_base->hcr_reg = wmm_host_cmd_reg->reg;
   wmm_base->hcr_addr = wmm_host_cmd_reg->address;

   /* issue the command */
   wmm_base->hcr_cmd = wmm_host_cmd_reg->cmd;

   /* check if the command has been succeeded */
   timeout = WINPATH_WMM_HOST_CMD_TIMEOUT;
   while ((wmm_base->hcr_cmd & WINPATH_WMM_CMD_VALID) && timeout-->0);
//      udelay(WINPATH_WMM_HOST_CMD_DELAY);
   if (timeout == 0)
   {
      wmm_host_cmd_reg->status = WINPATH_WMM_HOST_CMD_CURR_FAILED;
      spin_unlock(&wmm_cmd_lock);
      local_irq_enable();
      return;
   }

   wmm_host_cmd_reg->cmd = wmm_base->hcr_cmd;
   wmm_host_cmd_reg->reg = wmm_base->hcr_reg;
   wmm_host_cmd_reg->address = wmm_base->hcr_addr;
   wmm_host_cmd_reg->status = WINPATH_WMM_HOST_CMD_OK;

   spin_unlock(&wmm_cmd_lock);
   local_irq_enable();
   return;
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

EXPORT_SYMBOL_GPL(winpath_wmm_host_cmd_execute);
